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
  std::vector<event> events(2);

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
    atomic_fence(memory_order_seq_cst, memory_scope_work_item); // force burst
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
      // PRINTF("Loaded %d\n", Addr);
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

  events[1] = q.single_task<StreamingMemoryKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    constexpr uint ST_ALLOC_Q_SIZE = 4;
    DataBundle<uint, ST_ALLOC_Q_SIZE> StAllocAddrQ(uint{MAX_INT});
    DataBundle<uint, ST_ALLOC_Q_SIZE> StAllocTagQ(uint{0});
    DataBundle<bool, ST_ALLOC_Q_SIZE> StAllocValidQ(bool{false});

    // TODO: Try with 64 st_commit_size.
    constexpr uint ST_COMMIT_Q_SIZE = BURST_SIZE;
    DataBundle<uint, ST_COMMIT_Q_SIZE> StCommitAddrQ(uint{0});
    DataBundle<uint, ST_COMMIT_Q_SIZE> StCommitTagQ(uint{MAX_INT});
    DataBundle<T, ST_COMMIT_Q_SIZE> StCommitValQ(T{});

    // For convenience, same as AllocationQueue tail.
    uint LastStoreAllocAddr = 0;
    uint LastStoreAllocTag = 0;

    // These hold the most-recently-shifted-away values from the commit queue.
    uint LastStoreCommitAddr = 0;
    uint LastStoreCommitTag = MAX_INT;
    // The most-recently-shifted-in value to the commit queue.
    uint FirstStoreCommitTag = MAX_INT;

    constexpr uint LD_Q_SIZE = 4;
    DataBundle<uint, LD_Q_SIZE> LoadAddr(uint{0});
    DataBundle<uint, LD_Q_SIZE> LoadTag(uint{0});
    DataBundle<bool, LD_Q_SIZE> LoadAddrValid(bool{false});

    bool EndSignal = false;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (!EndSignal) {
      if (!StAllocValidQ[0])
        EndSignalPipe::read(EndSignal);

      /** Rule for delaying the store stream to increase st->ld reuse. */
      const bool GetNextStoreAlloc = (LastStoreAllocAddr <= LoadAddr[0] ||
                                      LastStoreAllocTag >= LoadTag[0]);
      /** End Rule */
      /** Rule for reading store {addr, tag} pairs. */
      if (!StAllocValidQ[ST_ALLOC_Q_SIZE - 1] && GetNextStoreAlloc) {
        bool succ = false;
        auto StoreReq = StoreAddrPipe::read(succ);
        if (succ) {
          StAllocValidQ[ST_ALLOC_Q_SIZE - 1] = true;
          StAllocAddrQ[ST_ALLOC_Q_SIZE - 1] = StoreReq.addr;
          StAllocTagQ[ST_ALLOC_Q_SIZE - 1] = StoreReq.tag;

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
          FirstStoreCommitTag = StAllocTagQ[0];

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
      

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////     LOAD LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////

      /** Rule for shifting load queue. */ 
      if (!LoadAddrValid[0]) {
        LoadAddr.Shift(uint{0});
        LoadTag.Shift(uint{0});
        LoadAddrValid.Shift(bool{false});
      }
      /** End Rule */

      /** Rule for reading load {addr, tag} pairs. */
      if (!LoadAddrValid[LD_Q_SIZE - 1]) {
        bool succ = false;
        auto LoadReq = LoadAddrPipe::read(succ);
        if (succ) {
          LoadAddrValid[LD_Q_SIZE - 1] = true;
          LoadAddr[LD_Q_SIZE - 1] = LoadReq.addr;
          LoadTag[LD_Q_SIZE - 1] = LoadReq.tag;
          // PRINTF("Next load req (%d, %d)\n", LoadAddr, LoadTag);
        }
      }
      /** End Rule */

      /** Rule for checking load safety and reuse. */
      const bool LdBeforeSt = (LoadTag[0] < LastStoreAllocTag);
      const bool LdRightAfterSt = (LoadTag[0] == LastStoreCommitTag);
      const bool LdLowerAddrAfterSt = (LoadTag[0] > LastStoreCommitTag &&
                                       LoadAddr[0] <= LastStoreCommitAddr);
      const bool SafeToLoad =
          (LdBeforeSt || LdRightAfterSt || LdLowerAddrAfterSt);

      // For checking reuse, lookup the store commit queue.
      const bool CanReuse = LoadTag[0] >= FirstStoreCommitTag;
      bool ReuseHit = false;
      T ReuseVal = {};
      #pragma unroll
      for (int i = 0; i < ST_COMMIT_Q_SIZE; ++i) {
        if (StCommitAddrQ[i] == LoadAddr[0]) {
          ReuseVal = StCommitValQ[i];
          ReuseHit = true;
        }
      }
      /** End Rule */

      /** Rule for returning reused/loaded value via the load MUX kernel.  
          Reuse takes precedence over loading, in case both are true. */
      if (LoadAddrValid[0]) {
        if (CanReuse && ReuseHit) {
          // PRINTF("Reused (%d, %d)\n", LoadAddr, LoadTag);
          LoadMuxPredPipe::write(LD_MUX_REUSE);
          LoadMuxReuseValPipe::write(ReuseVal);
          LoadAddrValid[0] = false;
        } else if (SafeToLoad) {
          // PRINTF("Loaded (%d, %d)   LastStoreAlloc (%d, %d)   LastStoreCommit (%d, %d)    safe (%d, %d, %d)\n", 
          //         LoadAddr[0], LoadTag[0], LastStoreAllocAddr, LastStoreAllocTag, LastStoreCommitAddr, LastStoreCommitTag, LdBeforeSt, LdRightAfterSt, LdLowerAddrAfterSt);
          LoadMuxPredPipe::write(LD_MUX_LOAD);
          LoadPortAddrPipe::write(LoadAddr[0]);
          LoadAddrValid[0] = false;
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
