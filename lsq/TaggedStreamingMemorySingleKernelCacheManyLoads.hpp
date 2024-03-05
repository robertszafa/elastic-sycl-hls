#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/ext/intel/ac_types/ac_int.hpp>
#include <sycl/sycl.hpp>

#include "unrolled_loop.hpp"
#include "pipe_utils.hpp"
#include "constexpr_math.hpp"
#include "data_bundle.hpp"
#include "device_print.hpp"


using namespace sycl;
using namespace fpga_tools;

using BurstCoalescedLSU = ext::intel::lsu<ext::intel::burst_coalesce<true>,
                                          ext::intel::prefetch<false>>;
using PipelinedLSU = sycl::ext::intel::lsu<>;

/// A gated StreamingLoad receives {addr, value, schedule} data on its gate,
/// and stores {addr, value} in a shift-register FIFO for reuse.
#define KERNEL_PRAGMAS [[intel::kernel_args_restrict]] [[intel::max_global_work_dim(0)]] 

constexpr uint DRAM_BURST_BITS = 512;
constexpr uint MAX_INT = (1<<30);

struct addr_t {
  uint addr;
};

struct addr_tag_t {
  uint addr;
  uint tag;
};

template <typename T> struct addr_val_t {
  uint addr;
  T val;
};

template <typename T> struct tag_val_t {
  uint tag;
  T val;
};

template <typename T> struct addr_tag_val_t {
  uint addr;
  uint tag;
  T val;
};

struct load_command_t {
  bool safeNow;
  bool safeAfterTag;
  bool reuse;
  uint tag; 
};


/// Unique kernel name generators.
template <int MemId> class StreamingMemoryKernel;
template <int MemId, int PortId> class LoadPortKernel;
template <int MemId, int PortId> class StorePortKernel;
template <int MemId, int PortId> class LoadValMuxKernel;

template <int MEM_ID, typename EndSignalPipe, typename LoadAddrPipes,
          typename LoadValPipes, typename StoreAddrPipe, typename StoreValPipe,
          uint NUM_LOADS, uint BIT_WIDTH = 32, typename T>
std::vector<event> StreamingLoad(queue &q, T *data) {
  std::vector<event> events(1);

  assert(sizeof(T) * 8 == BIT_WIDTH && "Incorrect kBitWidth.");
  constexpr uint NUM_BURST_VALS = (DRAM_BURST_BITS + BIT_WIDTH - 1) / BIT_WIDTH;

  using StorePortPredPipe = pipe<class _StorePortPred, bool, NUM_BURST_VALS>;
  using StorePortReqPipe = pipe<class _StorePortReq, addr_val_t<T>, NUM_BURST_VALS>;
  events[0] = q.single_task<StorePortKernel<MEM_ID, 0>>([=]() KERNEL_PRAGMAS {
    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (StorePortPredPipe::read()) {
      auto Req = StorePortReqPipe::read();
      auto StorePtr = ext::intel::device_ptr<T>(data + Req.addr);
      BurstCoalescedLSU::store(StorePtr, Req.val);
    }
  });

  using LoadPortAddr = PipeArray<class _LoadAddr, int, NUM_BURST_VALS*4, NUM_LOADS>;
  using LoadPortPred = PipeArray<class _LoadPortPred, bool, NUM_BURST_VALS*4, NUM_LOADS>;
  using LoadMuxIsReuse = PipeArray<class _LoadMuxIsReuse, bool, NUM_BURST_VALS*4, NUM_LOADS>;
  using LoadMuxPred = PipeArray<class _LoadMuxPred, bool, NUM_BURST_VALS*4, NUM_LOADS>;
  using LoadMuxMemoryVal = PipeArray<class _LoadMuxMemoryVal, T, NUM_BURST_VALS*4, NUM_LOADS>;
  using LoadMuxReuseVal = PipeArray<class _LoadMuxReuseVal, T, NUM_BURST_VALS*4, NUM_LOADS>;
  UnrolledLoop<NUM_LOADS>([&] (auto iLd) {
    q.single_task<LoadPortKernel<MEM_ID, iLd>>([=]() KERNEL_PRAGMAS {
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      [[intel::speculated_iterations(0)]]
      while (LoadPortPred:: template PipeAt<iLd>::read()) {
        int Addr = LoadPortAddr:: template PipeAt<iLd>::read();
        auto LoadPtr = ext::intel::device_ptr<T>(data + Addr);
        auto Val = BurstCoalescedLSU::load(LoadPtr);
        LoadMuxMemoryVal:: template PipeAt<iLd>::write(Val);
      }
    });
    q.single_task<LoadValMuxKernel<MEM_ID, iLd>>([=]() KERNEL_PRAGMAS {
      int NumTotal = 0;
      int NumReused = 0;
      while (LoadMuxPred:: template PipeAt<iLd>::read()) {
        NumTotal++;
        T val;
        if (LoadMuxIsReuse:: template PipeAt<iLd>::read()) {
          NumReused++;
          val = LoadMuxReuseVal:: template PipeAt<iLd>::read();
        } else {
          val = LoadMuxMemoryVal:: template PipeAt<iLd>::read();
        }
        LoadValPipes:: template PipeAt<iLd>::write(val);
      }
      PRINTF("DONE load MUX, reused %d/%d\n", NumReused, NumTotal);
    });
  });

  q.single_task<StreamingMemoryKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    auto StoreReq = StoreAddrPipe::read();
    uint StoreAddr = StoreReq.addr;
    uint StoreTag = StoreReq.tag;
    uint LastStoreAddr = StoreAddr;
    uint LastStoreTag = StoreTag;
    bool StoreAddrValid = true;

    DataBundle<uint, NUM_BURST_VALS> CommitQueueAddr(uint{MAX_INT});
    DataBundle<T, NUM_BURST_VALS> CommitQueueVal(T{});

    DataBundle<uint, NUM_LOADS> LoadAddr(uint{MAX_INT});
    DataBundle<uint, NUM_LOADS> LoadTag(T{});
    DataBundle<bool, NUM_LOADS> LoadAddrValid(bool{false});

    bool EndSignal = false;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (!EndSignal) {
      if (!StoreAddrValid)
        EndSignalPipe::read(EndSignal);

      UnrolledLoop<NUM_LOADS>([&](auto iLd) {
        /** Rule for reading load {addr, tag} pairs. */
        if (!LoadAddrValid[iLd]) {
          auto LoadReq =
              LoadAddrPipes::template PipeAt<iLd>::read(LoadAddrValid[iLd]);
          if (LoadAddrValid[iLd]) {
            LoadAddr[iLd] = LoadReq.addr;
            LoadTag[iLd] = LoadReq.tag;
          }
        }
        /** End Rule */
      });

      /** Rule for reading store {addr, tag} pairs. */
      if (!StoreAddrValid) {
        auto StoreReq = StoreAddrPipe::read(StoreAddrValid);
        if (StoreAddrValid) {
          StoreAddr = StoreReq.addr;
          StoreTag = StoreReq.tag;
          StoreAddrValid = (StoreAddr != MAX_INT);
          StorePortPredPipe::write(StoreAddr != MAX_INT);
        }
      }
      /** End Rule */

      /** Rule for reading store values and writing to store port. */
      if (StoreAddrValid) {
        bool StoreValValid = false;
        auto StoreVal = StoreValPipe::read(StoreValValid);
        if (StoreValValid) {
          StorePortReqPipe::write({StoreAddr, StoreVal});

          LastStoreAddr = StoreAddr;
          LastStoreTag = StoreTag;

          CommitQueueAddr.Shift(StoreAddr);
          CommitQueueVal.Shift(StoreVal);

          StoreAddrValid = false;
        }
      }
      /** End Rule */

      UnrolledLoop<NUM_LOADS>([&](auto iLd) {
        /** Rule for checking load safety and reuse. */
        const bool LoadAfterTag =
            (LoadTag[iLd] < StoreTag) ||
            (LoadTag[iLd] == StoreTag && LoadAddr[iLd] != StoreAddr) ||
            (LoadTag[iLd] > LastStoreTag && LoadAddr[iLd] < LastStoreAddr);
        const bool ReuseAfterTag =
            (LoadTag[iLd] >= LastStoreTag && LoadAddr[iLd] <= LastStoreAddr);
        bool ReuseHit = false;
        T ReuseVal = {};
        #pragma unroll
        for (int i = 0; i < NUM_BURST_VALS; ++i) {
          if (CommitQueueAddr[i] == LoadAddr[iLd]) {
            ReuseHit = true;
            ReuseVal = CommitQueueVal[i];
          }
        }
        /** End Rule */

        /** Rule for returning reused/loaded value via the load MUX kernel.  */
        if (LoadAddrValid[iLd]) {
          if (ReuseAfterTag && ReuseHit) {
            LoadMuxPred::template PipeAt<iLd>::write(1);
            LoadMuxIsReuse::template PipeAt<iLd>::write(1);
            LoadMuxReuseVal::template PipeAt<iLd>::write(ReuseVal);
            LoadAddrValid[iLd] = false;
          } else if (LoadAfterTag) {
            // If no reuse, then we trigger the load port kernel.
            LoadPortPred::template PipeAt<iLd>::write(1);
            LoadPortAddr::template PipeAt<iLd>::write(LoadAddr[iLd]);
            LoadMuxPred::template PipeAt<iLd>::write(1);
            LoadMuxIsReuse::template PipeAt<iLd>::write(0);
            LoadAddrValid[iLd] = false;
          }
        }
        /** End Rule */
      });

      // bool DelayStoreStream = false;
      // UnrolledLoop<NUM_LOADS>([&](auto iLd) {
      //   if (LoadAddr[iLd] > StoreAddr && LoadTag[iLd] > StoreTag)
      //     DelayStoreStream = true;
      // });

    } // end while

    LoadMuxPred::write(0);
    LoadPortPred::write(0);
    // StorePortPredPipe::write(0);

    // PRINTF("DONE Streaming Memory\n");
  });

  return events;
}
