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

template <int MEM_ID, typename EndSignalPipe, typename LoadAddrPipe,
          typename LoadValPipe, typename StoreAddrPipe, typename StoreValPipe,
          uint NUM_LOADS, uint BIT_WIDTH = 32, typename T>
std::vector<event> StreamingMemory(queue &q, T *data) {
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

  using LoadPortAddr = pipe<class _LoadAddr, int, NUM_BURST_VALS>;
  using LoadPortPred = pipe<class _LoadPortPred, bool, NUM_BURST_VALS>;
  using LoadMuxIsReuse = pipe<class _LoadMuxIsReuse, bool, NUM_BURST_VALS>;
  using LoadMuxPred = pipe<class _LoadMuxPred, bool, NUM_BURST_VALS>;
  using LoadMuxMemoryVal = pipe<class _LoadMuxMemoryVal, T, NUM_BURST_VALS>;
  using LoadMuxReuseVal = pipe<class _LoadMuxReuseVal, T, NUM_BURST_VALS>;
  q.single_task<LoadPortKernel<MEM_ID, 0>>([=]() KERNEL_PRAGMAS {
    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (LoadPortPred::read()) {
      int Addr = LoadPortAddr::read();
      auto LoadPtr = ext::intel::device_ptr<T>(data + Addr);
      auto Val = BurstCoalescedLSU::load(LoadPtr);
      LoadMuxMemoryVal::write(Val);
    }
  });
  q.single_task<LoadValMuxKernel<MEM_ID, 0>>([=]() KERNEL_PRAGMAS {
    int NumTotal = 0;
    int NumReused = 0;
    while (LoadMuxPred::read()) {
      NumTotal++;
      T val;
      if (LoadMuxIsReuse::read()) {
        NumReused++;
        val = LoadMuxReuseVal::read();
      } else {
        val = LoadMuxMemoryVal::read();
      }
      LoadValPipe::write(val);
    }
    PRINTF("DONE load MUX, reused %d/%d\n", NumReused, NumTotal);
  });

  q.single_task<StreamingMemoryKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    auto StoreReq = StoreAddrPipe::read();
    uint StoreAddr = StoreReq.addr;
    uint StoreTag = StoreReq.tag;
    uint LastStoreAddr = StoreAddr;
    uint LastStoreTag = StoreTag;
    bool StoreAddrValid = true;

    T StoreVal = {};
    bool StoreValValid = false;

    DataBundle<uint, NUM_BURST_VALS> CommitQueueAddr(uint{MAX_INT});
    DataBundle<T, NUM_BURST_VALS> CommitQueueVal(T{});

    uint LoadAddr = 0;
    uint LoadTag = 0;
    bool LoadAddrValid = false;

    bool EndSignal = false;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (!EndSignal) {
      if (!StoreAddrValid)
        EndSignalPipe::read(EndSignal);

      /** Rule for reading load {addr, tag} pairs. */
      if (!LoadAddrValid) {
        auto LoadReq = LoadAddrPipe::read(LoadAddrValid);
        if (LoadAddrValid) {
          LoadAddr = LoadReq.addr;
          LoadTag = LoadReq.tag;
        }
      }
      /** End Rule */

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
      if (StoreAddrValid && !StoreValValid) {
        auto tryStoreValTag = StoreValPipe::read(StoreValValid);
        if (StoreValValid) {
          StoreVal = tryStoreValTag;
          StorePortReqPipe::write({StoreAddr, StoreVal});
          LastStoreAddr = StoreAddr;
          LastStoreTag = StoreTag;

          CommitQueueAddr.Shift(StoreAddr);
          CommitQueueVal.Shift(StoreVal);
        }
      }
      /** End Rule */

      /** Rule for checking load safety and reuse. */
      const bool LoadAfterTag = (LoadTag <= LastStoreTag) ||
                                (LoadTag > LastStoreTag && LoadAddr < LastStoreAddr);
      const bool ReuseAfterTag = (LoadTag >= LastStoreTag && LoadAddr <= LastStoreAddr);
      bool ReuseHit = false;
      T ReuseVal = {};
      #pragma unroll
      for (int i = 0; i < NUM_BURST_VALS; ++i) {
        if (CommitQueueAddr[i] == LoadAddr) {
          ReuseHit = true;
          ReuseVal = CommitQueueVal[i];
        }
      }
      /** End Rule */

      /** Rule for returning reused/loaded value via the load MUX kernel.  */
      if (LoadAddrValid) {
        if (ReuseAfterTag && ReuseHit) {
          LoadMuxPred::write(1);
          LoadMuxIsReuse::write(1);
          LoadMuxReuseVal::write(ReuseVal);
          LoadAddrValid = false;
        } else if (LoadAfterTag) {
          // If no reuse, then we trigger the load port kernel.
          LoadPortPred::write(1);
          LoadPortAddr::write(LoadAddr);
          LoadMuxPred::write(1);
          LoadMuxIsReuse::write(0);
          LoadAddrValid = false;
        }
      }
      /** End Rule */

      /** Rule for getting the next store address/value pair. */
      if (StoreValValid && (LoadAddr >= StoreAddr || LoadTag <= StoreTag)) {
        StoreValValid = false;
        StoreAddrValid = false;
      }
      /** End Rule */

    } // end while

    LoadMuxPred::write(0);
    LoadPortPred::write(0);
    // StorePortPredPipe::write(0);

    // PRINTF("DONE Streaming Memory\n");
  });

  return events;
}