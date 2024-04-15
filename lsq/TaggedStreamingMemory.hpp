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

constexpr uint DRAM_BURST_BYTES = 32;
constexpr uint MAX_INT = (1<<30);
constexpr int INVALID_ADDR = -1;

// Types for (addr, tag) memory requests.
using addr_t = int;
using tag_t = uint;

struct addr_tag_t {
  addr_t addr;
  tag_t tag;
};
template <int NUM_STORES>
struct addr_tag_mintag_t {
  addr_t addr;
  tag_t tag;
  tag_t loopStartTag[NUM_STORES];
  // Minimum tag that a store dependency has to have.
  tag_t mintag[NUM_STORES];
  bool posDepDist[NUM_STORES];
};
// Types for specifying action in load mux.
using ld_mux_pred_t = ac_int<2, false>;
constexpr ld_mux_pred_t LD_MUX_TERMINATE = ld_mux_pred_t{0};
constexpr ld_mux_pred_t LD_MUX_REUSE = ld_mux_pred_t{1};
constexpr ld_mux_pred_t LD_MUX_LOAD = ld_mux_pred_t{2};

// Functions for shifting shift-register bundles.
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

template <int MEM_ID, typename LoadAddrPipes, typename LoadValPipes,
          typename StoreAddrPipes, typename StoreValPipes,
          typename EndSignalPipe, uint NUM_LOADS, uint NUM_STORES, typename T>
std::vector<event> StreamingMemory(queue &q, T *data) {
  std::vector<event> events(NUM_STORES + 1);

  constexpr uint BYTES_IN_T = sizeof(T);
  /// How many T values fit in a DRAM burst.
  constexpr uint BURST_SIZE = (DRAM_BURST_BYTES + BYTES_IN_T - 1) / BYTES_IN_T;

  // Store ports.
  using StorePortAddrPipes = PipeArray<class _StorePortAddr, addr_t, BURST_SIZE, NUM_STORES>;
  using StorePortValPipes = PipeArray<class _StorePortVal, T, BURST_SIZE, NUM_STORES>;
  UnrolledLoop<NUM_STORES>([&](auto iSt) {
    events[iSt] = q.single_task<StorePortKernel<MEM_ID, iSt>>([=]() KERNEL_PRAGMAS {
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        addr_t Addr = StorePortAddrPipes::template PipeAt<iSt>::read();
        if (Addr == MAX_INT) break;
        T Val = StorePortValPipes::template PipeAt<iSt>::read();
        auto StorePtr = ext::intel::device_ptr<T>(data + Addr);
        BurstCoalescedLSU::store(StorePtr, Val);
      }
      // Force any outstanding burst.
      atomic_fence(memory_order_seq_cst, memory_scope_work_item); 
    });
  });

  // Load ports and MUXex. A MUX selects between a val forwarded from a store
  // BURST buffer (LoadMuxReuseVal), and a val loaded from memory (LoadPortVal).
  using LoadPortAddrPipes = PipeArray<class _LoadAddr, tag_t, BURST_SIZE, NUM_LOADS>;
  using LoadPortValPipes = PipeArray<class _LoadPortVal, T, BURST_SIZE, NUM_LOADS>;
  using LoadMuxPredPipes = PipeArray<class _LoadMuxPred, ld_mux_pred_t, BURST_SIZE, NUM_LOADS>;
  using LoadMuxReuseValPipes = PipeArray<class _LoadMuxReuseVal, T, BURST_SIZE, NUM_LOADS>;
  UnrolledLoop<NUM_LOADS>([&](auto iLd) {
    q.single_task<LoadPortKernel<MEM_ID, iLd>>([=]() KERNEL_PRAGMAS {
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        addr_t Addr = LoadPortAddrPipes::template PipeAt<iLd>::read();
        if (Addr == MAX_INT) break;
        auto LoadPtr = ext::intel::device_ptr<T>(data + Addr);
        T Val = BurstCoalescedLSU::load(LoadPtr);
        LoadPortValPipes::template PipeAt<iLd>::write(Val);
      }
    });
    q.single_task<LoadValMuxKernel<MEM_ID, iLd>>([=]() KERNEL_PRAGMAS {
      // [[maybe_unused]] int NumTotal = 0, NumReused = 0;
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        T Val;
        ld_mux_pred_t Pred = LoadMuxPredPipes::template PipeAt<iLd>::read();
        if (Pred == LD_MUX_REUSE) {
          // NumReused++;
          Val = LoadMuxReuseValPipes::template PipeAt<iLd>::read();
        } else if (Pred == LD_MUX_LOAD) {
          Val = LoadPortValPipes::template PipeAt<iLd>::read();
        } else {
          break;
        }

        LoadValPipes::template PipeAt<iLd>::write(Val);
        // NumTotal++;
      }
      // PRINTF("** DONE ld%d, reused %d/%d\n", int(iLd), NumReused, NumTotal);
    });
  });

  events[1] = q.single_task<StreamingMemoryKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    constexpr uint ST_ALLOC_Q_SIZE = 2;
    bool StoreAllocValid[NUM_STORES][ST_ALLOC_Q_SIZE];
    addr_t StoreAllocAddr[NUM_STORES][ST_ALLOC_Q_SIZE];
    tag_t StoreAllocTag[NUM_STORES][ST_ALLOC_Q_SIZE];

    constexpr uint ST_COMMIT_Q_SIZE = BURST_SIZE;
    addr_t StoreCommitAddr[NUM_STORES][ST_COMMIT_Q_SIZE];
    tag_t StoreCommitTag[NUM_STORES][ST_COMMIT_Q_SIZE];
    T StoreCommitVal[NUM_STORES][ST_COMMIT_Q_SIZE];

    addr_t NextStoreAddr[NUM_STORES];
    tag_t NextStoreTag[NUM_STORES];

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
    addr_t LoadAddr[NUM_LOADS][LD_Q_SIZE];
    tag_t LoadTag[NUM_LOADS][LD_Q_SIZE];

    tag_t LoadLoopStartTag[NUM_LOADS][NUM_STORES][LD_Q_SIZE];
    tag_t LoadMinTag[NUM_LOADS][NUM_STORES][LD_Q_SIZE];
    bool LoadPosDepDist[NUM_LOADS][NUM_STORES][LD_Q_SIZE];

    // Once a load allocation is deemed safe to execute/reuse, it is moved to
    // this stage to issue load request or send reuse value.
    bool SafeLoadValid[NUM_LOADS][LD_Q_SIZE];
    addr_t SafeLoadAddr[NUM_LOADS][LD_Q_SIZE];
    bool SafeLoadReuse[NUM_LOADS][LD_Q_SIZE];
    T SafeLoadReuseVal[NUM_LOADS][LD_Q_SIZE];
    bool SafeLoadMuxPipeSucc[NUM_LOADS][LD_Q_SIZE];
    bool SafeLoadAddrPipeSucc[NUM_LOADS][LD_Q_SIZE];
    bool SafeLoadReusePipeSucc[NUM_LOADS][LD_Q_SIZE];
    
    // Init load registers
    UnrolledLoop<NUM_LOADS>([&](auto iLd) {
      InitBundle(LoadValid[iLd], false);
      InitBundle(LoadAddr[iLd], INVALID_ADDR);
      InitBundle(LoadTag[iLd], 0u);

      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        InitBundle(LoadLoopStartTag[iLd][iSt], 0u);
        InitBundle(LoadMinTag[iLd][iSt], 0u);
        InitBundle(LoadPosDepDist[iLd][iSt], false);
      });

      InitBundle(SafeLoadValid[iLd], false);
      InitBundle(SafeLoadAddr[iLd], INVALID_ADDR);
      InitBundle(SafeLoadReuse[iLd], false);
      InitBundle(SafeLoadReuseVal[iLd], T{});
      InitBundle(SafeLoadMuxPipeSucc[iLd], false);
      InitBundle(SafeLoadAddrPipeSucc[iLd], false);
      InitBundle(SafeLoadReusePipeSucc[iLd], false);
    });

    bool EndSignal = false;

    [[maybe_unused]] uint cycle = 0; // Used only for debug prints.

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    while (!EndSignal) {
      cycle++;

      /** Rule for listening for termination signal. Enabled when all stores
          have finished (MAX_INT sentinel value received on the StoreAddr pipe).
          Terminate a cycle after getting EndSignal.*/
      bool OutstandingStores = false;
      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        if (NextStoreAddr[iSt] != MAX_INT)
          OutstandingStores = true;
      });
      if (!OutstandingStores)
        EndSignalPipe::read(EndSignal);
      /** End Rule */

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////     LOAD LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////
      UnrolledLoop<NUM_LOADS>([&](auto iLd) {
        /** Rule for shifting load queues. */ 
        if (!LoadValid[iLd][0]) {
          ShiftBundle(LoadValid[iLd], false);
          ShiftBundle(LoadAddr[iLd], INVALID_ADDR);
          ShiftBundle(LoadTag[iLd], 0u);
          UnrolledLoop<NUM_STORES>([&](auto iSt) {
            ShiftBundle(LoadLoopStartTag[iLd][iSt], 0u);
            ShiftBundle(LoadMinTag[iLd][iSt], 0u);
            ShiftBundle(LoadPosDepDist[iLd][iSt], false);
          });
        }

        if (!SafeLoadValid[iLd][0]) {
          ShiftBundle(SafeLoadValid[iLd], false);
          ShiftBundle(SafeLoadAddr[iLd], INVALID_ADDR);
          ShiftBundle(SafeLoadReuse[iLd], false);
          ShiftBundle(SafeLoadReuseVal[iLd], T{});
          ShiftBundle(SafeLoadMuxPipeSucc[iLd], false);
          ShiftBundle(SafeLoadAddrPipeSucc[iLd], false);
          ShiftBundle(SafeLoadReusePipeSucc[iLd], false);
        }
        /** End Rule */

        /** Rule for sending address to load port (which returns loaded value to
            MUX), or for sending reuse value directly to MUX. The pipe writes to
            the "LoadMux" and to the "LoadPort" kernels are non-blocking. This
            stage is finished only once all pipes have been written.*/
        if (SafeLoadValid[iLd][0]) {
          if (!SafeLoadMuxPipeSucc[iLd][0]) {
            auto MUX_CODE = SafeLoadReuse[iLd][0] ? LD_MUX_REUSE : LD_MUX_LOAD;
            LoadMuxPredPipes::template PipeAt<iLd>::write(
                MUX_CODE, SafeLoadMuxPipeSucc[iLd][0]);
          }

          if (SafeLoadReuse[iLd][0]) {
            if (!SafeLoadReusePipeSucc[iLd][0]) {
              LoadMuxReuseValPipes::template PipeAt<iLd>::write(
                  SafeLoadReuseVal[iLd][0], SafeLoadReusePipeSucc[iLd][0]);
            }
          } else {
            if (!SafeLoadAddrPipeSucc[iLd][0]) {
              LoadPortAddrPipes::template PipeAt<iLd>::write(
                  SafeLoadAddr[iLd][0], SafeLoadAddrPipeSucc[iLd][0]);
            }
          }

          if (SafeLoadMuxPipeSucc[iLd][0] && (SafeLoadReusePipeSucc[iLd][0] ||
              SafeLoadAddrPipeSucc[iLd][0])) {
            SafeLoadValid[iLd][0] = false;
          }
        }
        /** End Rule */

        /** Rule for reading new load allocation: {addr, tag} pairs. */
        if (!LoadValid[iLd][LD_Q_SIZE - 1]) {
          bool succ = false;
          const addr_tag_mintag_t<NUM_STORES> LoadReq =
              LoadAddrPipes::template PipeAt<iLd>::read(succ);
          if (succ) {
            LoadValid[iLd][LD_Q_SIZE - 1] = true;
            LoadAddr[iLd][LD_Q_SIZE - 1] = LoadReq.addr;
            LoadTag[iLd][LD_Q_SIZE - 1] = LoadReq.tag;
            UnrolledLoop<NUM_STORES>([&](auto iSt) {
              LoadLoopStartTag[iLd][iSt][LD_Q_SIZE - 1] = LoadReq.loopStartTag[iSt];
              LoadMinTag[iLd][iSt][LD_Q_SIZE - 1] = LoadReq.mintag[iSt];
              LoadPosDepDist[iLd][iSt][LD_Q_SIZE - 1] = LoadReq.posDepDist[iSt];
            });
          }
        }
        /** End Rule */

        /** Rule for checking load safety and reuse. */
        if (LoadValid[iLd][0] && !SafeLoadValid[iLd][LD_Q_SIZE - 1]) {
          bool ThisSafe[NUM_STORES];
          tag_t ThisReuseTag[NUM_STORES];
          T ThisReuseVal[NUM_STORES];
          bool AllSafe = true;
          UnrolledLoop<NUM_STORES>([&](auto iSt) {
            // Three cases when no st->ld dependency exists:
            //  1. Load comes before store in program order (load tag is lower).
            //  2. Load comes after store in program order, but both are in the
            //     same loop and the dependency distance is positive.
            //  3. Load comes after store in program order, but reads from a
            //     lower address.
            ThisSafe[iSt] = (LoadTag[iLd][0] < NextStoreTag[iSt]) ||
                            (LoadLoopStartTag[iLd][iSt][0] < NextStoreTag[iSt] &&
                             LoadPosDepDist[iLd][iSt][0]) ||
                            (LoadMinTag[iLd][iSt][0] < NextStoreTag[iSt] &&
                             LoadAddr[iLd][0] < NextStoreAddr[iSt]);
            AllSafe &= ThisSafe[iSt];

            ThisReuseTag[iSt] = 0u;
            ThisReuseVal[iSt] = T{};
            #pragma unroll
            for (int i = 0; i < ST_COMMIT_Q_SIZE; ++i) {
              // On multiple matches this will choose the one with highest tag.
              if (LoadAddr[iLd][0] == StoreCommitAddr[iSt][i]) {
                ThisReuseTag[iSt] = StoreCommitTag[iSt][i];
                ThisReuseVal[iSt] = StoreCommitVal[iSt][i];
              }
            }
          });

          // If no hazards against any store, then move load to "safe" stage.
          if (AllSafe) {
            // We know there is a reuse if any ThisReuseTag is above 0 and above
            // the minTag for that store. If more matches, than choose max.
            bool Reuse = (ThisReuseTag[0] > LoadMinTag[iLd][0][0]);
            tag_t ReuseTag = ThisReuseTag[0];
            T ReuseVal = ThisReuseVal[0];
            UnrolledLoop<1, NUM_STORES>([&](auto iSt) {
              if (ThisReuseTag[iSt] > ReuseTag &&
                  ThisReuseTag[iSt] > LoadMinTag[iLd][iSt][0]) {
                Reuse = true;
                ReuseTag = ThisReuseTag[iSt];
                ReuseVal = ThisReuseVal[iSt];
              }
            });

            SafeLoadValid[iLd][LD_Q_SIZE - 1] = true;
            SafeLoadAddr[iLd][LD_Q_SIZE - 1] = LoadAddr[iLd][0];
            SafeLoadReuse[iLd][LD_Q_SIZE - 1] = Reuse;
            SafeLoadReuseVal[iLd][LD_Q_SIZE - 1] = ReuseVal;

            LoadValid[iLd][0] = false;
          }
        }
        /** End Rule */

      }); // End for all loads.


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
          const addr_tag_t StoreReq =
              StoreAddrPipes::template PipeAt<iSt>::read(succ);
          if (succ) {
            StorePortAddrPipes:: template PipeAt<iSt>::write(StoreReq.addr);

            StoreAllocValid[iSt][ST_ALLOC_Q_SIZE - 1] = true;
            StoreAllocAddr[iSt][ST_ALLOC_Q_SIZE - 1] = StoreReq.addr;
            StoreAllocTag[iSt][ST_ALLOC_Q_SIZE - 1] = StoreReq.tag;
          }
        }
        /** End Rule */

        /** Rule for getting st val and moving st alloc to st commit queue. */
        if (StoreAllocValid[iSt][0]) {
          bool succ = false;
          const T StoreVal = StoreValPipes::template PipeAt<iSt>::read(succ);
          if (succ) {
            StorePortValPipes::template PipeAt<iSt>::write(StoreVal);

            ShiftBundle(StoreCommitAddr[iSt], StoreAllocAddr[iSt][0]);
            ShiftBundle(StoreCommitTag[iSt], StoreAllocTag[iSt][0]);
            ShiftBundle(StoreCommitVal[iSt], StoreVal);

            StoreAllocValid[iSt][0] = false;

            // Check if this store overrides another store in its burst buffer.
            UnrolledLoop<NUM_STORES>([&](auto iStOther) {
              if constexpr (iStOther != iSt) {
                #pragma unroll
                for (int i = 0; i < ST_COMMIT_Q_SIZE; ++i) {
                  if (NextStoreAddr[iSt] == StoreCommitAddr[iStOther][i]) {
                    StoreCommitAddr[iStOther][i] = INVALID_ADDR;
                  }
                }
              }
            });
          }
        }
        /** End Rule */

      }); // End for all stores

    } // end while

    LoadMuxPredPipes::write(LD_MUX_TERMINATE);
    LoadPortAddrPipes::write(MAX_INT);

  });

  return events;
}
