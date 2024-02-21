#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/sycl.hpp>

#include "unrolled_loop.hpp"
#include "device_print.hpp"

using namespace sycl;
using namespace fpga_tools;

using BurstCoalescedLSU = ext::intel::lsu<ext::intel::burst_coalesce<true>,
                                          ext::intel::prefetch<false>>;
using PipelinedLSU = sycl::ext::intel::lsu<>;

/// A gated StreamingLoad receives {addr, value, schedule} data on its gate,
/// and stores {addr, value} in a shift-register FIFO for reuse.
#define KERNEL_PRAGMAS [[intel::kernel_args_restrict]] [[intel::max_global_work_dim(0)]] 

constexpr int kStoreFifoSize = 16;
constexpr int kStoreBurstSize = 16;

struct addr_t {
  int addr;
};

struct addr_tag_t {
  int addr;
  int tag;
};

struct addr_tags_t {
  int addr;
  int globalTag;
  int localTag;
};

template <typename T> struct addr_val_t {
  int addr;
  T val;
};

template <typename T> struct addr_tag_val_t {
  int addr;
  int tag;
  T val;
};

template <typename T> struct addr_tags_val_t {
  int addr;
  int globalTag;
  int localTag;
  T val;
};

constexpr int INVALID_ADDR = -1;
constexpr int MAX_INT = (1<<30);

/// Unique kernel name generators.
template <int id> class StreamingMemoryKernel;
template <int id0, int id1> class LoadPortKernel;
template <int id0, int id1> class StorePortKernel;
template <int id0, int id1> class LoadValMuxKernel;

class NoPipe;

template <typename PipeToRead, typename T>
inline void TryPipeReadIfNotValid(T &res, bool &valid) {
  if (!valid) {
    auto tmp = PipeToRead::read(valid);
    if (valid) {
      res = tmp;
    }
  }
}

template <int id, typename LoadAddrPipe, typename LoadValPipe,
          typename StoreAddrPipe, typename StoreValPipe, typename EndSignalPipe,
          typename T>
std::vector<event> StreamingMemory(queue &q, T *data) {
  std::vector<event> storeEvents(1);

  using LoadPortAddr = pipe<class _LoadAddr, int, kStoreBurstSize>;
  using LoadPortPred = pipe<class _LoadPortPred, bool, kStoreBurstSize>;
  using LoadMuxSelector = pipe<class _LoadMuxSelector, bool, kStoreBurstSize>;
  using LoadMuxPred = pipe<class _LoadMuxPred, bool, kStoreBurstSize>;
  using LoadMuxMemoryVal = pipe<class _LoadMuxMemoryVal, T, kStoreBurstSize>;
  using LoadMuxReuseVal = pipe<class _LoadMuxBypassVal, T, kStoreBurstSize>;
  q.single_task<LoadPortKernel<id, 0>>([=]() KERNEL_PRAGMAS {
    while (LoadPortPred::read()) {
      int Addr = LoadPortAddr::read();
      auto LoadPtr = ext::intel::device_ptr<T>(data + Addr);
      auto Val = BurstCoalescedLSU::load(LoadPtr);
      LoadMuxMemoryVal::write(Val);
    }
  });

  q.single_task<LoadValMuxKernel<id, 0>>([=]() KERNEL_PRAGMAS {
    while (LoadMuxPred::read()) {
      T val;
      if (LoadMuxSelector::read())
        val = LoadMuxReuseVal::read();
      else
        val = LoadMuxMemoryVal::read();

      LoadValPipe::write(val);
    }
  });

  using StorePortPred = pipe<class _StorePortPred, bool, kStoreBurstSize>;
  using StorePortReq = pipe<class _StorePortReq, addr_tags_val_t<T>, kStoreBurstSize>;
  using StorePortAck = pipe<class _StorePortAck, addr_tags_val_t<T>, kStoreBurstSize>;
  storeEvents[0] = q.single_task<StorePortKernel<id, 0>>([=]() KERNEL_PRAGMAS {
    while (StorePortPred::read()) {
      auto Req = StorePortReq::read();
      auto StorePtr = ext::intel::device_ptr<T>(data + Req.addr);
      BurstCoalescedLSU::store(StorePtr, Req.val);
      PRINTF("Store port stored addr %d\n", Req.addr);

      StorePortAck::write(Req);
    }

    StorePortAck::write({MAX_INT, MAX_INT, MAX_INT});
  });


  q.single_task<StreamingMemoryKernel<id>>([=]() KERNEL_PRAGMAS {
    int NextStoreAddr = 0;
    int NextStoreGlobalTag = 0;
    int NextStoreLocalTag = 0;
    bool NextStoreAddrValid = false;
    T NextStoreVal = {};
    bool NextStoreValValid = false;

    int LastStoreAddr = 0;
    int LastStoreGlobalTag = 0;
    int LastStoreLocalTag = 0;
    T LastStoreVal = {};

    int FifoStoreAddr[kStoreFifoSize];
    int FifoStoreVal[kStoreFifoSize];
    #pragma unroll
    for (uint i = 0; i < kStoreFifoSize; ++i) {
      FifoStoreAddr[i] = INVALID_ADDR;
      FifoStoreVal[i] = {};
    }
    
    int LoadAddr = 0;
    int LoadGlobalTag = 0;
    int LoadLocalTag = 0;
    bool LoadAddrValid = false;

    int NumTotal = 0;
    int NumReused = 0;

    bool EndSignal = false;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    while (!EndSignal) {
      if (NextStoreAddr == INVALID_ADDR)
        EndSignalPipe::read(EndSignal);

      if (!LoadAddrValid) {
        auto LoadReq = LoadAddrPipe::read(LoadAddrValid);
        if (LoadAddrValid) {
          LoadAddr = LoadReq.addr;
          LoadGlobalTag = LoadReq.globalTag;
          LoadLocalTag = LoadReq.localTag;
        }
      }
      if (!NextStoreAddrValid) {
        auto StoreReq = StoreAddrPipe::read(NextStoreAddrValid);
        if (NextStoreAddrValid) {
          NextStoreAddr = StoreReq.addr;
          NextStoreGlobalTag = StoreReq.globalTag;
          NextStoreLocalTag = StoreReq.localTag;
        }
      }
      if (!NextStoreValValid) {
        auto Val = StoreValPipe::read(NextStoreValValid);
        if (NextStoreValValid) {
          NextStoreVal = Val;
        }
      }

      bool StoreEnded = (NextStoreAddrValid && (NextStoreAddr == INVALID_ADDR));
      if ((NextStoreAddrValid && NextStoreValValid) || StoreEnded) {
        NextStoreAddrValid = false;
        NextStoreValValid = false;

        StorePortPred::write(!StoreEnded);
        if (!StoreEnded) {
          StorePortReq::write({NextStoreAddr, NextStoreLocalTag,
                              NextStoreGlobalTag, NextStoreVal});
        }
      }

      bool ackSucc = false;
      auto ack = StorePortAck::read(ackSucc);
      if (ackSucc) {
        LastStoreAddr = ack.addr;
        LastStoreGlobalTag = ack.globalTag;
        LastStoreLocalTag = ack.localTag;
        LastStoreVal = ack.val;
        PRINTF("Store ack for StoreAddr = %d\n", LastStoreAddr);

        #pragma unroll
        for (uint i = 0; i < kStoreFifoSize - 1; ++i) {
          FifoStoreAddr[i] = FifoStoreAddr[i + 1];
          FifoStoreVal[i] = FifoStoreVal[i + 1];
        }
        FifoStoreAddr[kStoreFifoSize - 1] = LastStoreAddr;
        FifoStoreVal[kStoreFifoSize - 1] = LastStoreVal;
      }
      
      const bool IsFromPrevLoop = (LoadGlobalTag > LastStoreGlobalTag);
      const bool IsFromSameLoop = (LoadGlobalTag == LastStoreGlobalTag);
      const bool IsFromNextLoop = (LoadGlobalTag < LastStoreGlobalTag);
      const bool IsLaterInLocalLoop = (LoadLocalTag > LastStoreLocalTag);
      const bool IsNextInLocalLoop = (LoadLocalTag == LastStoreLocalTag);
      const bool IsAddressLE = (LoadAddr <= LastStoreAddr);

      bool IsLoadSafe = (IsFromPrevLoop && IsAddressLE) ||
                        (IsFromSameLoop && (IsNextInLocalLoop || (IsLaterInLocalLoop && IsAddressLE))) ||
                        IsFromNextLoop;

      bool Reuse = false;
      T ReuseVal = {};
      UnrolledLoop<kStoreFifoSize>([&](auto k) {
        if (FifoStoreAddr[k] == LoadAddr) {
          Reuse = true;
          ReuseVal = FifoStoreVal[k];
        }
      });

      if (IsLoadSafe && LoadAddrValid) {
        LoadAddrValid = false;
        PRINTF("Safe LoadAddr = %d\n", LoadAddr);

        NumTotal++;
        if (Reuse) {
          NumReused++;
          PRINTF("Resued = %d\n", LoadAddr);
        }

        LoadMuxPred::write(1);
        LoadMuxSelector::write(Reuse);
        if (Reuse) {
          LoadMuxReuseVal::write(ReuseVal);
        } else {
          LoadPortPred::write(1);
          LoadPortAddr::write(LoadAddr);
        }
      }
    }


    StorePortPred::write(0);
    LoadPortPred::write(0);
    LoadMuxPred::write(0);

    PRINTF("Done streamingMemory %d\tReused %d/%d\n", id, NumReused, NumTotal);
  });

  return storeEvents;
}
