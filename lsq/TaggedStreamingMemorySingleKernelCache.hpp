#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/ext/intel/ac_types/ac_int.hpp>
#include <sycl/sycl.hpp>

#include "device_print.hpp"
#include "pipe_utils.hpp"
#include "unrolled_loop.hpp"

using namespace sycl;
using namespace fpga_tools;

using BurstCoalescedLSU = ext::intel::lsu<ext::intel::burst_coalesce<true>,
                                          ext::intel::prefetch<false>>;
using PipelinedLSU = sycl::ext::intel::lsu<>;

/// A gated StreamingLoad receives {addr, value, schedule} data on its gate,
/// and stores {addr, value} in a shift-register FIFO for reuse.
#define KERNEL_PRAGMAS [[intel::kernel_args_restrict]] [[intel::max_global_work_dim(0)]] 

constexpr uint DRAM_BURST_BITS = 512;
constexpr uint MAX_INT = (1<<29);
constexpr int INVALID_ADDR = -1;

struct addr_t {
  int addr;
};

struct addr_tag_t {
  int addr;
  uint tag;
};

template <int NUM_STORES>
struct addr_tag_mintag_t {
  int addr;
  uint tag;
  /// Minimum tag that a store dependency has to have.
  uint mintag[NUM_STORES];
};

template <typename T> struct addr_val_t {
  int addr;
  T val;
};

template <typename T> struct tag_val_t {
  int tag;
  T val;
};

template <typename T> struct addr_tag_val_t {
  int addr;
  uint tag;
  T val;
};

template <typename T, uint SIZE> 
inline void InitBundle(T (&Bundle)[SIZE], const T val) {
  #pragma unroll
  for (uint i = 0; i < SIZE; ++i) {
    Bundle[i] = val;
  }
}

template <typename T, uint SIZE> 
inline void ShiftBundle(T (&Bundle)[SIZE], const T val) {
  #pragma unroll
  for (uint i = 0; i < SIZE - 1; ++i) {
    Bundle[i] = Bundle[i + 1];
  }
  Bundle[SIZE - 1] = val;
}
template <typename T, uint SIZE> 
inline void ShiftBundle(T (&Bundle)[SIZE]) {
  #pragma unroll
  for (uint i = 0; i < SIZE - 1; ++i) {
    Bundle[i] = Bundle[i + 1];
  }
}

/// Unique kernel name generators.
template <int MemId> class StreamingMemoryKernel;
template <int MemId, int PortId> class LoadPortKernel;
template <int MemId, int PortId> class StorePortKernel;
template <int MemId, int PortId> class LoadValMuxKernel;

template <int MEM_ID, typename EndSignalPipe, typename LoadAddrPipe,
          typename LoadValPipe, typename StoreAddrPipes, typename StoreValPipes,
          typename StorePortAddrPipes, typename StorePortValPipes,
          uint NUM_LOADS, uint NUM_STORES, uint BIT_WIDTH = 32, typename T>
std::vector<event> StreamingMemory(queue &q, T *data) {
  std::vector<event> events(NUM_STORES + 1);

  assert(sizeof(T) * 8 == BIT_WIDTH && "Incorrect kBitWidth.");
  constexpr uint BURST_SIZE = (DRAM_BURST_BITS + BIT_WIDTH - 1) / BIT_WIDTH;

  // Store port.
  UnrolledLoop<NUM_STORES>([&](auto iSt) {
    events[iSt] = q.single_task<StorePortKernel<MEM_ID, iSt>>([=]() KERNEL_PRAGMAS {
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      [[intel::speculated_iterations(0)]]
      while (true) {
        auto Req = StorePortAddrPipes::template PipeAt<iSt>::read();
        if (Req.addr == MAX_INT) break;
        auto Val = StorePortValPipes::template PipeAt<iSt>::read();
        auto StorePtr = ext::intel::device_ptr<T>(data + Req.addr);
        BurstCoalescedLSU::store(StorePtr, Val);
      }
      // force final burst
      // atomic_fence(memory_order_seq_cst, memory_scope_work_item); 

      PRINTF("** DONE store port %d\n", int(iSt));
    });
  });

  // Types for specifying action in load mux.
  using ld_mux_pred_t = ac_int<2, false>;
  constexpr ld_mux_pred_t LD_MUX_TERMINATE = ld_mux_pred_t{0};
  constexpr ld_mux_pred_t LD_MUX_REUSE = ld_mux_pred_t{1};
  constexpr ld_mux_pred_t LD_MUX_LOAD = ld_mux_pred_t{2};
  // Pipes for load port and load mux.
  using LoadPortAddrPipe = pipe<class _LoadAddr, uint, BURST_SIZE*4>;
  using LoadMuxLoadValPipe = pipe<class _LoadMuxMemoryVal, T, BURST_SIZE*4>;
  using LoadMuxPredPipe = pipe<class _LoadMuxPred, ld_mux_pred_t, BURST_SIZE*4>;
  using LoadMuxReuseValPipe = pipe<class _LoadMuxReuseVal, T, BURST_SIZE*4>;
  q.single_task<LoadPortKernel<MEM_ID, 0>>([=]() KERNEL_PRAGMAS {
    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      auto Addr = LoadPortAddrPipe::read();
      if (Addr == MAX_INT) break;
      auto LoadPtr = ext::intel::device_ptr<T>(data + Addr);
      auto Val = BurstCoalescedLSU::load(LoadPtr);
      LoadMuxLoadValPipe::write(Val);
    }
    PRINTF("** DONE load port %d\n");
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
      // PRINTF("Ld Val returned (reuse = %d)\n", (Pred == LD_MUX_REUSE));

      LoadValPipe::write(Val);
      NumTotal++;
    }
    PRINTF("** DONE load MUX, reused %d/%d\n", NumReused, NumTotal);
  });

  events[NUM_STORES] = q.single_task<StreamingMemoryKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    constexpr uint ST_ALLOC_Q_SIZE = 4;
    bool StoreAllocValid[NUM_STORES][ST_ALLOC_Q_SIZE];
    int StoreAllocAddr[NUM_STORES][ST_ALLOC_Q_SIZE];
    uint StoreAllocTag[NUM_STORES][ST_ALLOC_Q_SIZE];

    constexpr uint ST_COMMIT_Q_SIZE = BURST_SIZE;
    int StoreCommitAddr[NUM_STORES][ST_COMMIT_Q_SIZE];
    uint StoreCommitTag[NUM_STORES][ST_COMMIT_Q_SIZE];
    T StoreCommitVal[NUM_STORES][ST_COMMIT_Q_SIZE];

    int LastStoreAckAddr[NUM_STORES];
    uint LastStoreAckTag[NUM_STORES];
    int LastStoreAllocAddr[NUM_STORES];
    uint LastStoreAllocTag[NUM_STORES];

    // Init store registers
    UnrolledLoop<NUM_STORES>([&] (auto iSt) {
      InitBundle(StoreAllocValid[iSt], false);
      InitBundle(StoreAllocAddr[iSt], INVALID_ADDR);
      InitBundle(StoreAllocTag[iSt], 0u);

      InitBundle(StoreCommitAddr[iSt], INVALID_ADDR);
      InitBundle(StoreCommitTag[iSt], 0u);
      InitBundle(StoreCommitVal[iSt], T{});

      LastStoreAckAddr[iSt] = INVALID_ADDR;
      LastStoreAckTag[iSt] = 0u;

      LastStoreAllocAddr[iSt] = INVALID_ADDR;
      LastStoreAllocTag[iSt] = 0u;
    });

    constexpr uint LD_Q_SIZE = 4;
    bool LoadValid[LD_Q_SIZE];
    int LoadAddr[LD_Q_SIZE];
    uint LoadTag[LD_Q_SIZE];
    uint LoadMinTag[NUM_STORES][LD_Q_SIZE];
    
    bool LoadSafe[LD_Q_SIZE];
    bool LoadReuse[LD_Q_SIZE];
    T LoadReuseVal[LD_Q_SIZE];

    // Init load registers
    InitBundle(LoadValid, false);
    InitBundle(LoadAddr, INVALID_ADDR);
    InitBundle(LoadTag, 0u);

    InitBundle(LoadSafe, false);
    InitBundle(LoadReuse, false);
    InitBundle(LoadReuseVal, T{});

    UnrolledLoop<NUM_STORES>(
        [&](auto iSt) { InitBundle(LoadMinTag[iSt], 0u); });

    bool EndSignal = false;

    uint cycle = 0;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (!EndSignal) {
      cycle++;

      /** Rule for termination. Terminate a cycle after getting EndSignal. */
      EndSignalPipe::read(EndSignal);
      /** End Rule */

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////    STORE LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////
      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        /** Rule shifting store allocation queue. */
        if (!StoreAllocValid[iSt][0]) {
          if (StoreAllocValid[iSt][1]) {
            LastStoreAllocAddr[iSt] = StoreAllocAddr[iSt][1];
            LastStoreAllocTag[iSt] = StoreAllocTag[iSt][1];
          }

          ShiftBundle(StoreAllocAddr[iSt], INVALID_ADDR);
          ShiftBundle(StoreAllocTag[iSt], 0u);
          ShiftBundle(StoreAllocValid[iSt], false);
        }
        /** End Rule */

        /** Rule for reading store {addr, tag} pairs. */
        if (!StoreAllocValid[iSt][ST_ALLOC_Q_SIZE - 1]) {
          bool succ = false;
          auto StoreReq = StoreAddrPipes::template PipeAt<iSt>::read(succ);
          if (succ) {
            // PRINTF("@%d St%d Req (%d, %d)\n", cycle, int(iSt), StoreReq.addr, StoreReq.tag);
            StoreAllocValid[iSt][ST_ALLOC_Q_SIZE - 1] = true;
            StoreAllocAddr[iSt][ST_ALLOC_Q_SIZE - 1] = StoreReq.addr;
            StoreAllocTag[iSt][ST_ALLOC_Q_SIZE - 1] = StoreReq.tag;
          }
        }
        /** End Rule */

        /** Rule for moving store allocation to store commit queue. */
        if (StoreAllocValid[iSt][0]) {
          bool succ = false;
          auto StoreVal = StoreValPipes::template PipeAt<iSt>::read(succ);
          if (succ) {
            // PRINTF("@%d St%d Val (%d, %d)\n", cycle, int(iSt), StoreAllocAddr[iSt][0], StoreAllocTag[iSt][0]);

            LastStoreAckAddr[iSt] = StoreCommitAddr[iSt][0];
            LastStoreAckTag[iSt] = StoreCommitTag[iSt][0];

            ShiftBundle(StoreCommitAddr[iSt], StoreAllocAddr[iSt][0]);
            ShiftBundle(StoreCommitTag[iSt], StoreAllocTag[iSt][0]);
            ShiftBundle(StoreCommitVal[iSt], StoreVal);

            StoreAllocValid[iSt][0] = false;
          }
        }
        /** End Rule */

      }); // End for all stores

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////     LOAD LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////
      /** Rule for shifting load allocation queue. */ 
      if (!LoadValid[0]) {
        ShiftBundle(LoadValid, false);
        ShiftBundle(LoadAddr, INVALID_ADDR);
        ShiftBundle(LoadTag, 0u);
        UnrolledLoop<NUM_STORES>(
            [&](auto iSt) { ShiftBundle(LoadMinTag[iSt], 0u); });

        ShiftBundle(LoadSafe, false);
        ShiftBundle(LoadReuse, false);
        ShiftBundle(LoadReuseVal, T{});
      }
      /** End Rule */

      /** Rule for reading new load allocation: {addr, tag} pairs. */
      if (!LoadValid[LD_Q_SIZE - 1]) {
        bool succ = false;
        const auto LoadReq = LoadAddrPipe::read(succ);
        if (succ) {
          // PRINTF("@%d Ld Req (%d, %d)\n", cycle, LoadReq.addr, LoadReq.tag);

          LoadValid[LD_Q_SIZE - 1] = true;
          LoadAddr[LD_Q_SIZE - 1] = LoadReq.addr;
          LoadTag[LD_Q_SIZE - 1] = LoadReq.tag;
          UnrolledLoop<NUM_STORES>([&](auto iSt) {
            LoadMinTag[iSt][LD_Q_SIZE - 1] = LoadReq.mintag[iSt];
          });
        }
      }
      /** End Rule */

      /** Rule for checking load safety and reuse. */
      if (LoadValid[0]) {
        bool LoadSafePerStore[NUM_STORES];
        bool LoadReusePerStore[NUM_STORES];
        uint LoadReuseTagPerStore[NUM_STORES]; // A tag > 0 indicates reuse
        T LoadReuseValPerStore[NUM_STORES];
        bool MinTagsSatisfied = (LoadMinTag[0][0] < LastStoreAllocTag[0]);
        UnrolledLoop<NUM_STORES>([&](auto iSt) {
          LoadSafePerStore[iSt] = (LoadTag[0] < LastStoreAllocTag[iSt]) ||
                                  // (LoadTag[0] == LastStoreAllocTag[iSt] &&
                                  //  LoadAddr[0] != LastStoreAllocAddr[iSt]) ||
                                  (LoadTag[0] >= LastStoreAckTag[iSt] &&
                                   LoadAddr[0] < LastStoreAckAddr[iSt]);

          LoadReusePerStore[iSt] = false;
          LoadReuseTagPerStore[iSt] = 0u;
          #pragma unroll
          for (int i = 0; i < ST_COMMIT_Q_SIZE; ++i) {
            if (LoadAddr[0] == StoreCommitAddr[iSt][i]) {
              LoadReusePerStore[iSt] = true;
              LoadReuseTagPerStore[iSt] = StoreCommitTag[iSt][i];
              LoadReuseValPerStore[iSt] = StoreCommitVal[iSt][i];
            }
          }

          if constexpr (iSt > 0)
            MinTagsSatisfied &= (LoadMinTag[iSt][0] < LastStoreAllocTag[iSt]);
        });

        if (MinTagsSatisfied) {
          LoadSafe[0] = LoadSafePerStore[0];
          LoadReuse[0] = LoadReusePerStore[0];
          uint MaxReuseTag = LoadReuseTagPerStore[0];
          LoadReuseVal[0] = LoadReuseValPerStore[0];
          UnrolledLoop<1, NUM_STORES>([&](auto iSt) {
            LoadSafe[0] &= LoadSafePerStore[iSt];
            LoadReuse[0] |= LoadReusePerStore[iSt];

            if (LoadReuseTagPerStore[iSt] > MaxReuseTag) {
              MaxReuseTag = LoadReuseTagPerStore[iSt];
              LoadReuseVal[0] = LoadReuseValPerStore[iSt];
            }
          });

          if (LoadReuse[0]) {
            // PRINTF("%d Reusing load addr %d   LastStoreAlloc=(%d, %d)   "
            //        "LastStoreAckTag=%d   CommitTail=(%d, %d)\n",
            //        cycle, LoadAddr[0], LastStoreAllocAddr[0],
            //        LastStoreAllocTag[0], LastStoreAckTag[0],
            //        StoreCommitAddr[0][ST_COMMIT_Q_SIZE - 1],
            //        StoreCommitTag[0][ST_COMMIT_Q_SIZE - 1]);
            LoadMuxPredPipe::write(LD_MUX_REUSE);
            LoadMuxReuseValPipe::write(LoadReuseVal[0]);
            LoadValid[0] = false;
          } else if (LoadSafe[0]) {
            LoadMuxPredPipe::write(LD_MUX_LOAD);
            LoadPortAddrPipe::write(LoadAddr[0]);
            LoadValid[0] = false;
          }
        }
      }
      /** End Rule */

    } // end while

    LoadMuxPredPipe::write(LD_MUX_TERMINATE);
    LoadPortAddrPipe::write(MAX_INT);

    PRINTF("** DONE Streaming Memory\n");
  });

  return events;
}
