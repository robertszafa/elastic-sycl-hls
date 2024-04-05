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
constexpr uint MAX_INT = (1<<30);
constexpr int INVALID_ADDR = -1;

// Types for (addr, tag) memory requests.
struct addr_tag_t {
  int addr;
  uint tag;
};
template <int NUM_STORES>
struct addr_tag_mintag_t {
  int addr;
  uint tag;
  uint loopStartTag;
  // Minimum tag that a store dependency has to have.
  uint mintag[NUM_STORES];
  bool posDepDist[NUM_STORES];
};
// Types for specifying action in load mux.
using ld_mux_pred_t = ac_int<2, false>;
constexpr ld_mux_pred_t LD_MUX_TERMINATE = ld_mux_pred_t{0};
constexpr ld_mux_pred_t LD_MUX_REUSE = ld_mux_pred_t{1};
constexpr ld_mux_pred_t LD_MUX_LOAD = ld_mux_pred_t{2};

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

template <int MEM_ID, typename EndSignalPipe, typename LoadAddrPipes,
          typename LoadValPipes, typename StoreAddrPipes, typename StoreValPipes,
          uint NUM_LOADS, uint NUM_STORES, uint BIT_WIDTH = 32, typename T>
std::vector<event> StreamingMemory(queue &q, T *data) {
  std::vector<event> events(NUM_STORES + 1);

  assert(sizeof(T) * 8 == BIT_WIDTH && "Incorrect kBitWidth.");
  constexpr uint BURST_SIZE = (DRAM_BURST_BITS + BIT_WIDTH - 1) / BIT_WIDTH;

  // Store ports.
  using StorePortAddrPipes = PipeArray<class _StorePortAddr, int, BURST_SIZE*4, NUM_STORES>;
  using StorePortValPipes = PipeArray<class _StorePortVal, T, BURST_SIZE*4, NUM_STORES>;
  UnrolledLoop<NUM_STORES>([&](auto iSt) {
    events[iSt] = q.single_task<StorePortKernel<MEM_ID, iSt>>([=]() KERNEL_PRAGMAS {
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      [[intel::speculated_iterations(0)]]
      while (true) {
        auto Addr = StorePortAddrPipes::template PipeAt<iSt>::read();
        if (Addr == MAX_INT) break;
        auto Val = StorePortValPipes::template PipeAt<iSt>::read();
        auto StorePtr = ext::intel::device_ptr<T>(data + Addr);
        BurstCoalescedLSU::store(StorePtr, Val);
      }
      // force final burst?
      atomic_fence(memory_order_seq_cst, memory_scope_work_item); 
      // PRINTF("** DONE store port%d\n", int(iSt));
    });
  });

  // Load ports.

  // Pipes for load port and load mux.
  using LoadPortAddrPipes = PipeArray<class _LoadAddr, uint, BURST_SIZE*4, NUM_LOADS>;
  using LoadMuxLoadValPipes = PipeArray<class _LoadMuxMemoryVal, T, BURST_SIZE*4, NUM_LOADS>;
  using LoadMuxPredPipes = PipeArray<class _LoadMuxPred, ld_mux_pred_t, BURST_SIZE*4, NUM_LOADS>;
  using LoadMuxReuseValPipes = PipeArray<class _LoadMuxReuseVal, T, BURST_SIZE*4, NUM_LOADS>;
  UnrolledLoop<NUM_LOADS>([&](auto iLd) {
    q.single_task<LoadPortKernel<MEM_ID, iLd>>([=]() KERNEL_PRAGMAS {
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      [[intel::speculated_iterations(0)]]
      while (true) {
        auto Addr = LoadPortAddrPipes::template PipeAt<iLd>::read();
        if (Addr == MAX_INT) break;
        auto LoadPtr = ext::intel::device_ptr<T>(data + Addr);
        auto Val = BurstCoalescedLSU::load(LoadPtr);
        LoadMuxLoadValPipes::template PipeAt<iLd>::write(Val);
      }
    });
    q.single_task<LoadValMuxKernel<MEM_ID, iLd>>([=]() KERNEL_PRAGMAS {
      int NumTotal = 0;
      int NumReused = 0;
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      [[intel::speculated_iterations(0)]]
      while (true) {
        T Val;
        ld_mux_pred_t Pred = LoadMuxPredPipes::template PipeAt<iLd>::read();
        if (Pred == LD_MUX_REUSE) {
          NumReused++;
          Val = LoadMuxReuseValPipes::template PipeAt<iLd>::read();
        } else if (Pred == LD_MUX_LOAD) {
          Val = LoadMuxLoadValPipes::template PipeAt<iLd>::read();
        } else {
          break;
        }
        // PRINTF("Ld%d Val returned (reuse = %d)\n", int(iLd), (Pred == LD_MUX_REUSE));

        LoadValPipes::template PipeAt<iLd>::write(Val);
        NumTotal++;
      }
      PRINTF("** DONE load%d MUX, reused %d/%d\n", int(iLd), NumReused, NumTotal);
    });
  });

  events[NUM_STORES] = q.single_task<StreamingMemoryKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    constexpr uint ST_ALLOC_Q_SIZE = 2;
    bool StoreAllocValid[NUM_STORES][ST_ALLOC_Q_SIZE];
    int StoreAllocAddr[NUM_STORES][ST_ALLOC_Q_SIZE];
    uint StoreAllocTag[NUM_STORES][ST_ALLOC_Q_SIZE];

    constexpr uint ST_COMMIT_Q_SIZE = BURST_SIZE;
    int StoreCommitAddr[NUM_STORES][ST_COMMIT_Q_SIZE];
    uint StoreCommitTag[NUM_STORES][ST_COMMIT_Q_SIZE];
    T StoreCommitVal[NUM_STORES][ST_COMMIT_Q_SIZE];

    int NextStoreAddr[NUM_STORES];
    uint NextStoreTag[NUM_STORES];

    // Init store registers
    UnrolledLoop<NUM_STORES>([&] (auto iSt) {
      InitBundle(StoreAllocValid[iSt], false);
      InitBundle(StoreAllocAddr[iSt], INVALID_ADDR);
      InitBundle(StoreAllocTag[iSt], 0u);

      InitBundle(StoreCommitAddr[iSt], INVALID_ADDR);
      InitBundle(StoreCommitTag[iSt], 0u);
      InitBundle(StoreCommitVal[iSt], T{});

      NextStoreAddr[iSt] = INVALID_ADDR;
      NextStoreTag[iSt] = 0u;
    });

    constexpr uint LD_Q_SIZE = 4;
    bool LoadValid[NUM_LOADS][LD_Q_SIZE];
    int LoadAddr[NUM_LOADS][LD_Q_SIZE];
    uint LoadTag[NUM_LOADS][LD_Q_SIZE];
    uint LoadLoopStartTag[NUM_LOADS][LD_Q_SIZE];
    uint LoadMinTag[NUM_LOADS][NUM_STORES][LD_Q_SIZE];
    bool LoadPosDepDist[NUM_LOADS][NUM_STORES][LD_Q_SIZE];
    
    bool LoadSafe[NUM_LOADS][LD_Q_SIZE];
    bool LoadReuse[NUM_LOADS][LD_Q_SIZE];
    T LoadReuseVal[NUM_LOADS][LD_Q_SIZE];

    // Init load registers
    UnrolledLoop<NUM_LOADS>([&](auto iLd) {
      InitBundle(LoadValid[iLd], false);
      InitBundle(LoadAddr[iLd], INVALID_ADDR);
      InitBundle(LoadTag[iLd], 0u);
      InitBundle(LoadLoopStartTag[iLd], 0u);

      InitBundle(LoadSafe[iLd], false);
      InitBundle(LoadReuse[iLd], false);
      InitBundle(LoadReuseVal[iLd], T{});
      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        InitBundle(LoadMinTag[iLd][iSt], 0u);
        InitBundle(LoadPosDepDist[iLd][iSt], false);
      });
    });

    bool EndSignal = false;

    [[maybe_unused]] uint cycle = 0; // Used only for debug prints.

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (!EndSignal) {
      cycle++;

      /** Rule for termination. Terminate a cycle after getting EndSignal. */
      bool OutstandingStores = false;
      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        if (NextStoreAddr[iSt] != MAX_INT)
          OutstandingStores = true;
      });
      if (!OutstandingStores)
        EndSignalPipe::read(EndSignal);
      /** End Rule */

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////    STORE LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////
      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        /** Rule shifting store allocation queue. */
        if (!StoreAllocValid[iSt][0]) {
          if (StoreAllocValid[iSt][1]) {
            NextStoreAddr[iSt] = StoreAllocAddr[iSt][1];
            NextStoreTag[iSt] = StoreAllocTag[iSt][1];
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
            StorePortAddrPipes:: template PipeAt<iSt>::write(StoreReq.addr);

            StoreAllocValid[iSt][ST_ALLOC_Q_SIZE - 1] = true;
            StoreAllocAddr[iSt][ST_ALLOC_Q_SIZE - 1] = StoreReq.addr;
            StoreAllocTag[iSt][ST_ALLOC_Q_SIZE - 1] = StoreReq.tag;
          }
        }
        /** End Rule */

        /** Rule for getting st val and moving st alloc to st commit queue. */
        bool StoreSafe = true;
        UnrolledLoop<NUM_STORES>([&](auto iStOther) {
          if constexpr (iStOther != iSt) {
            if (NextStoreTag[iStOther] < NextStoreTag[iSt] &&
                NextStoreAddr[iStOther] <= NextStoreAddr[iSt]) {
              StoreSafe = false;
            }
          }
        });
        if (StoreAllocValid[iSt][0] && StoreSafe) {
          bool succ = false;
          auto StoreVal = StoreValPipes::template PipeAt<iSt>::read(succ);
          if (succ) {
            StorePortValPipes::template PipeAt<iSt>::write(StoreVal);

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
      UnrolledLoop<NUM_LOADS>([&](auto iLd) {
        /** Rule for shifting load allocation queue. */ 
        if (!LoadValid[iLd][0]) {
          ShiftBundle(LoadValid[iLd], false);
          ShiftBundle(LoadAddr[iLd], INVALID_ADDR);
          ShiftBundle(LoadTag[iLd], 0u);
          ShiftBundle(LoadLoopStartTag[iLd], 0u);
          UnrolledLoop<NUM_STORES>([&](auto iSt) {
            ShiftBundle(LoadMinTag[iLd][iSt], 0u);
            ShiftBundle(LoadPosDepDist[iLd][iSt], false);
          });
          ShiftBundle(LoadSafe[iLd], false);
          ShiftBundle(LoadReuse[iLd], false);
          ShiftBundle(LoadReuseVal[iLd], T{});
        }
        /** End Rule */

        /** Rule for reading new load allocation: {addr, tag} pairs. */
        if (!LoadValid[iLd][LD_Q_SIZE - 1]) {
          bool succ = false;
          const auto LoadReq = LoadAddrPipes::template PipeAt<iLd>::read(succ);
          if (succ) {
            LoadValid[iLd][LD_Q_SIZE - 1] = true;
            LoadAddr[iLd][LD_Q_SIZE - 1] = LoadReq.addr;
            LoadTag[iLd][LD_Q_SIZE - 1] = LoadReq.tag;
            LoadLoopStartTag[iLd][LD_Q_SIZE - 1] = LoadReq.loopStartTag;
            UnrolledLoop<NUM_STORES>([&](auto iSt) {
              LoadMinTag[iLd][iSt][LD_Q_SIZE - 1] = LoadReq.mintag[iSt];
              LoadPosDepDist[iLd][iSt][LD_Q_SIZE - 1] = LoadReq.posDepDist[iSt];
            });
          }
        }
        /** End Rule */

        /** Rule for checking load safety and reuse. */
        if (LoadValid[iLd][0]) {
          bool ThisSafe[NUM_STORES];
          bool ThisReuse[NUM_STORES];
          uint ThisReuseTag[NUM_STORES];
          T ThisReuseVal[NUM_STORES];
          bool AllSafe = true;
          UnrolledLoop<NUM_STORES>([&](auto iSt) {
            ThisSafe[iSt] = (LoadLoopStartTag[iLd][0] < NextStoreTag[iSt] && 
                             LoadPosDepDist[iLd][iSt][0]) ||
                            LoadTag[iLd][0] < NextStoreTag[iSt] ||
                            (LoadMinTag[iLd][iSt][0] < NextStoreTag[iSt] &&
                             LoadAddr[iLd][0] < NextStoreAddr[iSt]);
            AllSafe &= ThisSafe[iSt];

            ThisReuse[iSt] = false;
            ThisReuseTag[iSt] = 0u;
            ThisReuseVal[iSt] = T{};
            #pragma unroll
            for (int i = 0; i < ST_COMMIT_Q_SIZE; ++i) {
              if (LoadAddr[iLd][0] == StoreCommitAddr[iSt][i] &&
                  LoadMinTag[iLd][iSt][0] < StoreCommitTag[iSt][i]) {
                ThisReuse[iSt] = true;
                ThisReuseTag[iSt] = StoreCommitTag[iSt][i];
                ThisReuseVal[iSt] = StoreCommitVal[iSt][i];
              }
            }
          });

          if (AllSafe) {
            LoadSafe[iLd][0] = true;
            LoadReuse[iLd][0] = ThisReuse[0];
            LoadReuseVal[iLd][0] = ThisReuseVal[0];
            uint MaxReuseTag = ThisReuseTag[0];
            UnrolledLoop<1, NUM_STORES>([&](auto iSt) {
              LoadReuse[iLd][0] |= ThisReuse[iSt];

              if (ThisReuseTag[iSt] > MaxReuseTag) {
                MaxReuseTag = ThisReuseTag[iSt];
                LoadReuseVal[iLd][0] = ThisReuseVal[iSt];
              }
            });

            if (LoadReuse[iLd][0]) {
              LoadMuxPredPipes::template PipeAt<iLd>::write(LD_MUX_REUSE);
              LoadMuxReuseValPipes::template PipeAt<iLd>::write(LoadReuseVal[iLd][0]);
            } else if (LoadSafe[iLd][0]) {
              LoadMuxPredPipes::template PipeAt<iLd>::write(LD_MUX_LOAD);
              LoadPortAddrPipes::template PipeAt<iLd>::write(LoadAddr[iLd][0]);
            }

            LoadValid[iLd][0] = false;
          }
        }
        /** End Rule */
      });

    } // end while

    LoadMuxPredPipes::write(LD_MUX_TERMINATE);
    LoadPortAddrPipes::write(MAX_INT);

    // PRINTF("** DONE Streaming Memory\n");
  });

  return events;
}
