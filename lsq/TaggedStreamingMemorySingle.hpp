#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/sycl.hpp>

#include "unrolled_loop.hpp"
#include "pipe_utils.hpp"
#include "constexpr_math.hpp"
#include "device_print.hpp"

using namespace sycl;
using namespace fpga_tools;

using BurstCoalescedLSU = ext::intel::lsu<ext::intel::burst_coalesce<true>,
                                          ext::intel::prefetch<false>>;
using PipelinedLSU = sycl::ext::intel::lsu<>;

/// A gated StreamingLoad receives {addr, value, schedule} data on its gate,
/// and stores {addr, value} in a shift-register FIFO for reuse.
#define KERNEL_PRAGMAS [[intel::kernel_args_restrict]] [[intel::max_global_work_dim(0)]] 

constexpr uint kStoreFifoSize = 32;
constexpr uint kDramBurstBitSize = 512;

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


template <int id, typename LoadAddrPipes, typename LoadValPipes,
          typename StoreAddrPipe, typename StoreValPipe, int kNumLoads,
          typename EndSignalPipe, typename T>
std::vector<event> StreamingMemory(queue &q, T *data) {
  std::vector<event> storeEvents(1);

  constexpr uint kPipeDepth = (kDramBurstBitSize + sizeof(T) - 1) / sizeof(T);

  using LoadPortAddr = PipeArray<class _LoadAddr, int, kPipeDepth, kNumLoads>;
  using LoadPortPred = PipeArray<class _LoadPortPred, bool, kPipeDepth, kNumLoads>;
  using LoadMuxSelector = PipeArray<class _LoadMuxSelector, bool, kPipeDepth, kNumLoads>;
  using LoadMuxPred = PipeArray<class _LoadMuxPred, bool, kPipeDepth, kNumLoads>;
  using LoadMuxMemoryVal = PipeArray<class _LoadMuxMemoryVal, T, kPipeDepth, kNumLoads>;
  using LoadMuxReuseVal = PipeArray<class _LoadMuxBypassVal, T, kPipeDepth, kNumLoads>;
  UnrolledLoop<kNumLoads>([&](auto iLd) {
    q.single_task<LoadPortKernel<id, iLd>>([=]() KERNEL_PRAGMAS {
      while (LoadPortPred:: template PipeAt<iLd>::read()) {
        int Addr = LoadPortAddr:: template PipeAt<iLd>::read();
        auto LoadPtr = ext::intel::device_ptr<T>(data + Addr);
        auto Val = BurstCoalescedLSU::load(LoadPtr);
        LoadMuxMemoryVal:: template PipeAt<iLd>::write(Val);
      }
    });

    q.single_task<LoadValMuxKernel<id, iLd>>([=]() KERNEL_PRAGMAS {
      while (LoadMuxPred:: template PipeAt<iLd>::read()) {
        T val;
        if (LoadMuxSelector:: template PipeAt<iLd>::read()) {
          val = LoadMuxReuseVal:: template PipeAt<iLd>::read();
        } else {
          val = LoadMuxMemoryVal:: template PipeAt<iLd>::read();
        }
        LoadValPipes:: template PipeAt<iLd>::write(val);
      }
    });
  });

  using StorePortPred = pipe<class _StorePortPred, bool, kPipeDepth>;
  using StorePortReq = pipe<class _StorePortReq, addr_tags_val_t<T>, kPipeDepth>;
  using StorePortAck = pipe<class _StorePortAck, addr_tags_val_t<T>, kPipeDepth>;
  storeEvents[0] = q.single_task<StorePortKernel<id, 0>>([=]() KERNEL_PRAGMAS {
    while (StorePortPred::read()) {
      auto Req = StorePortReq::read();
      auto StorePtr = ext::intel::device_ptr<T>(data + Req.addr);
      BurstCoalescedLSU::store(StorePtr, Req.val);
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

    int LastStoreAddr = -1;
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
    
    int LoadAddr[kNumLoads];
    int LoadGlobalTag[kNumLoads];
    int LoadLocalTag[kNumLoads];
    bool LoadAddrValid[kNumLoads];

    int NumTotal[kNumLoads];
    int NumReused[kNumLoads];

    UnrolledLoop<kNumLoads>([&](auto iLd) {
      LoadAddr[iLd] = 0;
      LoadGlobalTag[iLd] = 0;
      LoadLocalTag[iLd] = 0;
      LoadAddrValid[iLd] = false;

      NumTotal[iLd] = 0;
      NumReused[iLd] = 0;
    });

    bool EndSignal = false;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (!EndSignal) {
      UnrolledLoop<kNumLoads>([&](auto iLd) {
        if (!LoadAddrValid[iLd]) {
          auto LoadReq = LoadAddrPipes:: template PipeAt<iLd>::read(LoadAddrValid[iLd]);
          if (LoadAddrValid[iLd]) {
            LoadAddr[iLd] = LoadReq.addr;
            LoadGlobalTag[iLd] = LoadReq.globalTag;
            LoadLocalTag[iLd] = LoadReq.localTag;
          }
        }
      });
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

        #pragma unroll
        for (uint i = 0; i < kStoreFifoSize - 1; ++i) {
          FifoStoreAddr[i] = FifoStoreAddr[i + 1];
          FifoStoreVal[i] = FifoStoreVal[i + 1];
        }
        FifoStoreAddr[kStoreFifoSize - 1] = LastStoreAddr;
        FifoStoreVal[kStoreFifoSize - 1] = LastStoreVal;
      }
      
      UnrolledLoop<kNumLoads>([&](auto iLd) {
        const bool LoadInLaterLoop = (LoadGlobalTag[iLd] > LastStoreGlobalTag);
        const bool LoadInSameLoop = (LoadGlobalTag[iLd] == LastStoreGlobalTag);
        const bool LoadInPrevLoop = (LoadGlobalTag[iLd] < LastStoreGlobalTag);
        const bool LoadLaterInloop = (LoadLocalTag[iLd] > LastStoreLocalTag);
        const bool LoadNextInLoop = (LoadLocalTag[iLd] == LastStoreLocalTag);
        const bool LoadAddressLower = (LoadAddr[iLd] <= LastStoreAddr);

        bool IsLoadSafe = (LoadInLaterLoop && LoadAddressLower) ||
                          (LoadInSameLoop && LoadNextInLoop) ||
                          (LoadInSameLoop && LoadLaterInloop && LoadAddressLower) ||
                          LoadInPrevLoop;

        bool Reuse = false;
        T ReuseVal = {};
        UnrolledLoop<kStoreFifoSize>([&](auto iStFifo) {
          if (FifoStoreAddr[iStFifo] == LoadAddr[iLd]) {
            Reuse = true;
            ReuseVal = FifoStoreVal[iStFifo];
          }
        });

        if (IsLoadSafe && LoadAddrValid[iLd]) {
          LoadAddrValid[iLd] = false;

          NumTotal[iLd]++;
          if (Reuse) NumReused[iLd]++;

          LoadMuxPred:: template PipeAt<iLd>::write(1);
          LoadMuxSelector:: template PipeAt<iLd>::write(Reuse);
          if (Reuse) {
            LoadMuxReuseVal:: template PipeAt<iLd>::write(ReuseVal);
          } else {
            LoadPortPred:: template PipeAt<iLd>::write(1);
            LoadPortAddr:: template PipeAt<iLd>::write(LoadAddr[iLd]);
          }
        }
      });

      if (!ackSucc)
        EndSignalPipe::read(EndSignal);
    } // end while


    // Terminates all st/ld ports and ld muxes. 
    StorePortPred::write(0);
    LoadPortPred::write(0);
    LoadMuxPred::write(0);

    for (int i = 0; i < kNumLoads; ++i)
      PRINTF("Load %d reused %d/%d\n", i, NumReused[i], NumTotal[i]);
  });

  return storeEvents;
}
