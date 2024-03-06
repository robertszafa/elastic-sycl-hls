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

struct addr_tag_mintag_t {
  uint addr;
  uint tag;
  /// Minimum tag that a store dependency has to have.
  uint mintag;
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


/// Unique kernel name generators.
template <int MemId> class StreamingMemoryKernel;
template <int MemId, int PortId> class LoadPortKernel;
template <int MemId, int PortId> class StorePortKernel;
template <int MemId, int PortId> class LoadValMuxKernel;

template <int MEM_ID, typename EndSignalPipe, typename LoadAddrPipe,
          typename LoadValPipe, typename StoreAddrPipe, typename StoreValPipe,
          uint NUM_LOADS, uint BIT_WIDTH = 32, typename T>
std::vector<event> StreamingLoad(queue &q, T *data) {
  std::vector<event> events(1);

  assert(sizeof(T) * 8 == BIT_WIDTH && "Incorrect kBitWidth.");
  constexpr uint BURST_SIZE = (DRAM_BURST_BITS + BIT_WIDTH - 1) / BIT_WIDTH;

  // Store port.
  using StorePortReqPipe = pipe<class _StorePortReq, addr_val_t<T>, BURST_SIZE*4>;
  events[0] = q.single_task<StorePortKernel<MEM_ID, 0>>([=]() KERNEL_PRAGMAS {
    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      auto Req = StorePortReqPipe::read();
      if (Req.addr == MAX_INT) break;
      auto StorePtr = ext::intel::device_ptr<T>(data + Req.addr);
      BurstCoalescedLSU::store(StorePtr, Req.val);
    }
  });

  // Types for specifying action in load mux.
  using ld_mux_pred_t = ac_int<2, false>;
  constexpr ld_mux_pred_t LD_MUX_TERMINATE = ld_mux_pred_t{0};
  constexpr ld_mux_pred_t LD_MUX_REUSE = ld_mux_pred_t{1};
  constexpr ld_mux_pred_t LD_MUX_LOAD = ld_mux_pred_t{2};
  // Pipes for load port and load mux.
  using LoadPortAddrPipe = pipe<class _LoadAddr, int, BURST_SIZE*4>;
  using LoadMuxLoadValPipe = pipe<class _LoadMuxMemoryVal, T, BURST_SIZE*4>;
  using LoadMuxPredPipe = pipe<class _LoadMuxPred, ld_mux_pred_t, BURST_SIZE*4>;
  using LoadMuxReuseValPipe = pipe<class _LoadMuxReuseVal, T, BURST_SIZE*4>;
  q.single_task<LoadPortKernel<MEM_ID, 0>>([=]() KERNEL_PRAGMAS {
    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      int Addr = LoadPortAddrPipe::read();
      if (Addr == MAX_INT) break;
      auto LoadPtr = ext::intel::device_ptr<T>(data + Addr);
      auto Val = BurstCoalescedLSU::load(LoadPtr);
      LoadMuxLoadValPipe::write(Val);
    }
  });
  q.single_task<LoadValMuxKernel<MEM_ID, 0>>([=]() KERNEL_PRAGMAS {
    int NumTotal = 0;
    int NumReused = 0;
    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      T Val;
      ld_mux_pred_t Pred = LoadMuxPredPipe::read();
      if (Pred == LD_MUX_REUSE) {
        NumReused++;
        Val = LoadMuxReuseValPipe::read();
      } else if (Pred == LD_MUX_LOAD) {
        Val = LoadMuxLoadValPipe::read();
      } else {
        break;
      }

      LoadValPipe::write(Val);
      NumTotal++;
    }
    PRINTF("DONE load MUX, reused %d/%d\n", NumReused, NumTotal);
  });

  q.single_task<StreamingMemoryKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    DataBundle<uint, BURST_SIZE> StAllocAddrQ(uint{MAX_INT});
    DataBundle<uint, BURST_SIZE> StAllocTagQ(uint{0});
    DataBundle<bool, BURST_SIZE> StAllocValidQ(bool{false});

    DataBundle<uint, BURST_SIZE> StCommitAddrQ(uint{0});
    DataBundle<uint, BURST_SIZE> StCommitTagQ(uint{MAX_INT});
    DataBundle<T, BURST_SIZE> StCommitValQ(T{});

    // For convenience, same as AllocationQueue tail.
    uint LastStoreAllocAddr = 0;
    uint LastStoreAllocTag = 0;

    // These hold the most-recently-shifted-away values from the commit queue.
    uint LastStoreCommitAddr = 0;
    uint LastStoreCommitTag = MAX_INT;

    uint LoadAddr = 0;
    uint LoadTag = 0;
    bool LoadAddrValid = false;

    bool EndSignal = false;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (!EndSignal) {
      // if (!StoreAddrValid)
        EndSignalPipe::read(EndSignal);

      /** Rule for reading load {addr, tag} pairs. */
      if (!LoadAddrValid) {
        bool succ = false;
        auto LoadReq = LoadAddrPipe::read(succ);
        if (succ) {
          LoadAddrValid = true;
          LoadAddr = LoadReq.addr;
          LoadTag = LoadReq.tag;
          // PRINTF("Next load req (%d, %d)\n", LoadAddr, LoadTag);
        }
      }
      /** End Rule */

      /** Rule for delaying the store stream to increase st->ld reuse. */
      const bool GetNextStoreAlloc = (LastStoreAllocAddr <= LoadAddr ||
                                      LastStoreAllocTag >= LoadTag);
      /** End Rule */

      /** Rule for reading store {addr, tag} pairs. */
      if (!StAllocValidQ[BURST_SIZE - 1] && GetNextStoreAlloc) {
        bool succ = false;
        auto StoreReq = StoreAddrPipe::read(succ);
        if (succ) {
          StAllocValidQ[BURST_SIZE - 1] = true;
          StAllocAddrQ[BURST_SIZE - 1] = StoreReq.addr;
          StAllocTagQ[BURST_SIZE - 1] = StoreReq.tag;

          LastStoreAllocAddr = StoreReq.addr;
          LastStoreAllocTag = StoreReq.tag;
          // PRINTF("Got StoreAlloc (%d, %d)\n", LastStoreAllocAddr, LastStoreAllocTag);
        }
      }
      /** End Rule */

      /** Rule for reading store values and writing to store port. 
          On getting store val, move store allocation to store commit queue. 
          Capture the shifted away store address and tag. */
      if (StAllocValidQ[0]) {
        bool succ = false;
        auto StoreVal = StoreValPipe::read(succ);
        if (succ) {
          StorePortReqPipe::write({StAllocAddrQ[0], StoreVal});

          LastStoreCommitAddr = StCommitAddrQ[0];
          LastStoreCommitTag = StCommitTagQ[0];

          StCommitAddrQ.Shift(StAllocAddrQ[0]);
          StCommitTagQ.Shift(StAllocTagQ[0]);
          StCommitValQ.Shift(StoreVal);

          StAllocValidQ[0] = false;
        }
      }
      /** End Rule */

      /** Rule for shifting store allocation queue. */
      if (!StAllocValidQ[0]) {
        StAllocAddrQ.Shift(uint{MAX_INT});
        StAllocTagQ.Shift(uint{0});
        StAllocValidQ.Shift(false);
      }
      /** End Rule */

      /** Rule for checking load safety and reuse. */
      const bool LoadBeforeStore = (LoadTag < LastStoreAllocTag);
      const bool LoadFromDiffAddrRightAfterStore =
          (LoadTag == LastStoreAllocTag && LoadAddr != LastStoreAllocAddr);
      const bool LoadFromLowerAddrAfterStore =
          (LoadTag >= LastStoreCommitTag && LoadAddr <= LastStoreCommitAddr);
      const bool LoadIsSafe = (LoadBeforeStore ||
                               LoadFromDiffAddrRightAfterStore ||
                               LoadFromLowerAddrAfterStore);

      // For checking reuse, lookup the store commit queue.
      bool ReuseHit = false;
      T ReuseVal = {};
      #pragma unroll
      for (int i = 0; i < BURST_SIZE; ++i) {
        if (StCommitAddrQ[i] == LoadAddr && LoadTag >= StCommitTagQ[i]) {
          ReuseVal = StCommitValQ[i];
          ReuseHit = true;
        }
      }
      // const bool LoadCanReuse =
      //     (LoadTag >= LastStoreAllocTag && LoadAddr <= LastStoreAllocAddr);
      /** End Rule */

      /** Rule for returning reused/loaded value via the load MUX kernel.  
          Reuse takes precedence over loading, in case both are true. */
      if (LoadAddrValid) {
        if (ReuseHit) {
          // PRINTF("Reused (%d, %d)\n", LoadAddr, LoadTag);
          LoadMuxPredPipe::write(LD_MUX_REUSE);
          LoadMuxReuseValPipe::write(ReuseVal);
          LoadAddrValid = false;
        } else if (LoadIsSafe) {
          // PRINTF("Loaded (%d, %d)   LastStoreAlloc (%d, %d)   LastStoreCommit (%d, %d)    safe (%d, %d, %d)\n", 
          //         LoadAddr, LoadTag, LastStoreAllocAddr, LastStoreAllocTag, LastStoreCommitAddr, LastStoreCommitTag, LoadBeforeStore, LoadFromDiffAddrRightAfterStore, LoadFromLowerAddrAfterStore);
          LoadMuxPredPipe::write(LD_MUX_LOAD);
          LoadPortAddrPipe::write(LoadAddr);
          LoadAddrValid = false;
        }
      }
      /** End Rule */

    } // end while

    LoadMuxPredPipe::write(LD_MUX_TERMINATE);
    LoadPortAddrPipe::write(MAX_INT);
    StorePortReqPipe::write({MAX_INT});

    PRINTF("DONE Streaming Memory\n");
  });

  return events;
}
