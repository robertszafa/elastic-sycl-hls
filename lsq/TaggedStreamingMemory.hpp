#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/ext/intel/ac_types/ac_int.hpp>
#include <sycl/sycl.hpp>

#include "device_print.hpp"
#include "pipe_utils.hpp"
#include "unrolled_loop.hpp"
#include "data_bundle.hpp"


#include "DependencyTable.hpp"

using namespace sycl;
using namespace fpga_tools;

using addr_t = int;
using sched_t = uint;

template <int LOOP_DEPTH>
struct store_req_t {
  addr_t addr;
  sched_t sched[LOOP_DEPTH];
  bool isMaxIter[LOOP_DEPTH];
};
template <int NUM_STORES, int LOOP_DEPTH>
struct load_req_t {
  addr_t addr;
  sched_t sched[LOOP_DEPTH];
  bool isMaxIter[LOOP_DEPTH];
  bool posDepDist[NUM_STORES];
};

template <int LOOP_DEPTH>
struct load_mux_req_t {
  bool reuse;
  store_req_t<LOOP_DEPTH> ack;
};

#define KERNEL_PRAGMAS [[intel::kernel_args_restrict]] [[intel::max_global_work_dim(0)]] 

using BurstCoalescedLSU = ext::intel::lsu<ext::intel::burst_coalesce<true>,
                                          ext::intel::prefetch<false>>;

/// 512-bit DRAM interface
constexpr uint DRAM_BURST_BYTES = 64;

constexpr addr_t INVALID_ADDR = -1;
constexpr addr_t STORE_ADDR_SENTINEL = (1<<30) - 1;
constexpr addr_t LOAD_ADDR_SENTINEL = (1<<30) - 2;
constexpr addr_t FINAL_LD_ADDR_ACK = STORE_ADDR_SENTINEL + 1;
constexpr sched_t SCHED_SENTINEL = (1<<31);

/// Unique kernel name generators.
template <int MemId> class StreamingMemoryKernel;
template <int MemId, int PortId> class LoadPortKernel;
template <int MemId, int PortId> class StorePortKernel;
template <int MemId, int PortId> class LoadValMuxKernel;

template <int MEM_ID, typename LoadAddrPipes, typename LoadValPipes,
          typename StoreAddrPipes, typename StoreValPipes,
          uint NUM_LOADS, uint NUM_STORES, uint LOOP_DEPTH, typename T>
std::vector<event> StreamingMemory(queue &q, T *data) {
  std::vector<event> events(NUM_STORES + 1);

  constexpr uint BYTES_IN_T = sizeof(T);
  /// num_T_values_in_DRAM_burst * 2.
  constexpr uint BURST_SIZE =
      ((DRAM_BURST_BYTES + BYTES_IN_T - 1) / BYTES_IN_T) * 2;

  // Store ports.
  using StorePortAddrPipes = PipeArray<class _StorePortAddr, addr_t, BURST_SIZE, NUM_STORES>;
  using StorePortValPipes = PipeArray<class _StorePortVal, T, BURST_SIZE, NUM_STORES>;
  using StoreAckPipes = PipeArray<class _StoreAck, addr_t, BURST_SIZE, NUM_STORES>;
  UnrolledLoop<NUM_STORES>([&](auto iSt) {
    events[iSt] = q.single_task<StorePortKernel<MEM_ID, iSt>>([=]() KERNEL_PRAGMAS {
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        const auto Addr = StorePortAddrPipes::template PipeAt<iSt>::read();

        if (Addr != STORE_ADDR_SENTINEL) {
          T Val = StorePortValPipes::template PipeAt<iSt>::read();
          auto StorePtr = ext::intel::device_ptr<T>(data + Addr);
          BurstCoalescedLSU::store(StorePtr, Val);
        }

        StoreAckPipes::template PipeAt<iSt>::write(Addr);

        if (Addr == STORE_ADDR_SENTINEL) break;
      }
      // Force any outstanding burst.
      atomic_fence(memory_order_seq_cst, memory_scope_work_item); 
    });
  });

  // Load ports and MUXex. A MUX selects between a val forwarded from a store
  // BURST buffer (LoadMuxReuseVal), and a val loaded from memory (LoadPortVal).
  using LoadPortAddrPipes = PipeArray<class _LoadAddr, addr_t, BURST_SIZE, NUM_LOADS>;
  using LoadPortValPipes = PipeArray<class _LoadPortVal, T, BURST_SIZE, NUM_LOADS>;
  using LoadMuxPredPipes = PipeArray<class _LoadMuxPred, load_mux_req_t<LOOP_DEPTH>, BURST_SIZE, NUM_LOADS>;
  using LoadMuxAckPipes = PipeArray<class _LoadMuxAckPipes, store_req_t<LOOP_DEPTH>, BURST_SIZE, NUM_LOADS>;
  using LoadMuxReuseValPipes = PipeArray<class _LoadMuxReuseVal, T, BURST_SIZE, NUM_LOADS>;
  UnrolledLoop<NUM_LOADS>([&](auto iLd) {
    q.single_task<LoadPortKernel<MEM_ID, iLd>>([=]() KERNEL_PRAGMAS {
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        const addr_t Addr = LoadPortAddrPipes::template PipeAt<iLd>::read();
        if (Addr == LOAD_ADDR_SENTINEL) break;
        auto LoadPtr = ext::intel::device_ptr<T>(data + Addr);
        T Val = BurstCoalescedLSU::load(LoadPtr);
        LoadPortValPipes::template PipeAt<iLd>::write(Val);
      }
      // PRINTF("** DONE ld port %d\n", int(iLd));
    });
    q.single_task<LoadValMuxKernel<MEM_ID, iLd>>([=]() KERNEL_PRAGMAS {
      [[maybe_unused]] int NumTotal = 0, NumReused = 0;
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        T Val;
        const auto MuxReq = LoadMuxPredPipes::template PipeAt<iLd>::read();
        if (MuxReq.ack.addr == LOAD_ADDR_SENTINEL) break;

        if (MuxReq.reuse) {
          NumReused++;
          Val = LoadMuxReuseValPipes::template PipeAt<iLd>::read();
        } else {
          Val = LoadPortValPipes::template PipeAt<iLd>::read();
        } 

        LoadMuxAckPipes::template PipeAt<iLd>::write(MuxReq.ack);
        LoadValPipes::template PipeAt<iLd>::write(Val);
        NumTotal++;
      }

      store_req_t<LOOP_DEPTH> maxAck{FINAL_LD_ADDR_ACK};
      InitBundle(maxAck.sched, SCHED_SENTINEL);
      InitBundle(maxAck.isMaxIter, true);
      LoadMuxAckPipes::template PipeAt<iLd>::write(maxAck);
      PRINTF("** DONE ld%d, reused %d/%d\n", int(iLd), NumReused, NumTotal);
    });
  });

  events[1] = q.single_task<StreamingMemoryKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    /* Store logic registers */
    constexpr uint ST_ALLOC_Q_SIZE = 2;
    bool StoreAllocValid[NUM_STORES][ST_ALLOC_Q_SIZE];
    addr_t StoreAllocAddr[NUM_STORES][ST_ALLOC_Q_SIZE];
    addr_t StoreAllocAddrPlusBurst[NUM_STORES][ST_ALLOC_Q_SIZE];
    sched_t StoreAllocSched[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    bool StoreAllocIsMaxIter[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    // addr_t StoreAllocMaxAddrAtLoop[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    
    constexpr uint ST_COMMIT_Q_SIZE = BURST_SIZE;
    addr_t StoreCommitAddr[NUM_STORES][ST_COMMIT_Q_SIZE];
    sched_t StoreCommitSched[NUM_STORES][LOOP_DEPTH][ST_COMMIT_Q_SIZE];
    bool StoreCommitIsMaxIter[NUM_STORES][LOOP_DEPTH][ST_COMMIT_Q_SIZE];
    T StoreCommitVal[NUM_STORES][ST_COMMIT_Q_SIZE];

    addr_t NextStoreAddr[NUM_STORES];
    addr_t NextStoreAddrPlusBurst[NUM_STORES];
    sched_t NextStoreSched[NUM_STORES][LOOP_DEPTH];
    bool NextStoreIsMaxIter[NUM_STORES][LOOP_DEPTH];

    addr_t StoreAckAddr[NUM_STORES];

    // Init store registers
    UnrolledLoop<NUM_STORES>([&] (auto iSt) {
      InitBundle(StoreAllocValid[iSt], false);
      InitBundle(StoreAllocAddr[iSt], INVALID_ADDR);
      InitBundle(StoreAllocAddrPlusBurst[iSt], INVALID_ADDR);
      UnrolledLoop<LOOP_DEPTH>([&] (auto iD) {
        InitBundle(StoreAllocSched[iSt][iD], 0u);
        InitBundle(StoreAllocIsMaxIter[iSt][iD], false);
        InitBundle(StoreCommitSched[iSt][iD], 0u);
        InitBundle(StoreCommitIsMaxIter[iSt][iD], false);
      });

      InitBundle(StoreCommitAddr[iSt], INVALID_ADDR);
      InitBundle(StoreCommitVal[iSt], T{});

      NextStoreAddr[iSt] = INVALID_ADDR;
      NextStoreAddrPlusBurst[iSt] = INVALID_ADDR;
      InitBundle(NextStoreSched[iSt], 0u);
      InitBundle(NextStoreIsMaxIter[iSt], false);

      StoreAckAddr[iSt] = INVALID_ADDR;
    });

    /* Load logic registers */
    constexpr uint LD_Q_SIZE = 4;
    bool LoadValid[NUM_LOADS][LD_Q_SIZE];
    addr_t LoadAddr[NUM_LOADS][LD_Q_SIZE];
    sched_t LoadSched[NUM_LOADS][LOOP_DEPTH][LD_Q_SIZE];
    bool LoadIsMaxIter[NUM_LOADS][LOOP_DEPTH][LD_Q_SIZE];
    bool LoadPosDepDist[NUM_LOADS][NUM_STORES][LD_Q_SIZE];
    // addr_t LoadMaxAddrAtLoop[NUM_LOADS][LOOP_DEPTH][LD_Q_SIZE];

    addr_t LoadAckAddr[NUM_LOADS];
    sched_t LoadAckSched[NUM_LOADS][LOOP_DEPTH];
    bool LoadAckIsMaxIter[NUM_LOADS][LOOP_DEPTH];

    // Alias to load_q position 0.
    addr_t NextLoadAddr[NUM_LOADS];
    sched_t NextLoadSched[NUM_LOADS][LOOP_DEPTH];
    bool NextLoadIsMaxIter[NUM_LOADS][LOOP_DEPTH];
    bool NextLoadPosDepDist[NUM_LOADS][NUM_STORES];

    // Once a load allocation is deemed safe to execute/reuse, it is moved to
    // this stage to issue load request or send reuse value.
    bool SafeLoadValid[NUM_LOADS][LD_Q_SIZE];
    addr_t SafeLoadAddr[NUM_LOADS][LD_Q_SIZE];
    sched_t SafeLoadSched[NUM_LOADS][LOOP_DEPTH][LD_Q_SIZE];
    bool SafeLoadIsMaxIter[NUM_LOADS][LOOP_DEPTH][LD_Q_SIZE];
    bool SafeLoadReuse[NUM_LOADS][LD_Q_SIZE];
    T SafeLoadReuseVal[NUM_LOADS][LD_Q_SIZE];
    bool SafeLoadMuxPipeSucc[NUM_LOADS][LD_Q_SIZE];
    bool SafeLoadAddrPipeSucc[NUM_LOADS][LD_Q_SIZE];
    bool SafeLoadReusePipeSucc[NUM_LOADS][LD_Q_SIZE];
    
    // Init load registers
    UnrolledLoop<NUM_LOADS>([&](auto iLd) {
      InitBundle(LoadValid[iLd], false);
      InitBundle(LoadAddr[iLd], INVALID_ADDR);
      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        InitBundle(LoadPosDepDist[iLd][iSt], false);
      });
      UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
        InitBundle(LoadSched[iLd][iD], 0u);
        InitBundle(LoadIsMaxIter[iLd][iD], false);
        InitBundle(SafeLoadSched[iLd][iD], 0u);
        InitBundle(SafeLoadIsMaxIter[iLd][iD], false);
      });

      LoadAckAddr[iLd] = INVALID_ADDR;
      InitBundle(LoadAckSched[iLd], 0u);
      InitBundle(LoadAckIsMaxIter[iLd], true);

      NextLoadAddr[iLd] = INVALID_ADDR;
      InitBundle(NextLoadSched[iLd], 0u);
      InitBundle(NextLoadPosDepDist[iLd], false);
      InitBundle(NextLoadIsMaxIter[iLd], false);

      InitBundle(SafeLoadValid[iLd], false);
      InitBundle(SafeLoadAddr[iLd], INVALID_ADDR);
      InitBundle(SafeLoadReuse[iLd], false);
      InitBundle(SafeLoadReuseVal[iLd], T{});
      InitBundle(SafeLoadMuxPipeSucc[iLd], false);
      InitBundle(SafeLoadAddrPipeSucc[iLd], false);
      InitBundle(SafeLoadReusePipeSucc[iLd], false);
    });

    /* Lambdas for checking RAW, WAR, WAW hazards. The functions make use of
     * compile-time information, like "at which loop level should we compare
     * schedule values" or "is this a forward/backwards dependence".
     */
    static constexpr auto DI = DepInfo<MEM_ID>{};
    constexpr auto walkUpStoreToFirstWrap = [&](const auto iSt,
                                                const auto start) {
      for (int i = start - 1; i >= 0; --i) {
        if (DI.STORE_IS_MAX_ITER_NEEDED[iSt][i]) {
          return i;
        }
      }

      return start;
    };

    constexpr auto walkUpLoadToFirstWrap = [&](const auto iLd,
                                               const auto start) {
      for (int i = start - 1; i >= 0; --i) {
        if (DI.LOAD_IS_MAX_ITER_NEEDED[iLd][i]) {
          return i;
        }
      }

      return start;
    };

    auto canCheckPosDepDist = [&](const auto iLd, const auto iSt) {
      constexpr int cmnDepth = DI.COMMON_LOOP_DEPTH[iLd][iSt];
      constexpr int stWrapDepth = walkUpStoreToFirstWrap(iSt, cmnDepth);

      if constexpr ((cmnDepth <= 0) || (stWrapDepth == cmnDepth))
        return true;

      return (NextStoreSched[iSt][stWrapDepth] >=
              NextLoadSched[iLd][stWrapDepth]);
    };

    auto checkNoWAW = [&](const auto iStThis, const auto iStOther) {
      constexpr int cmnDepth = DI.COMMON_STORE_LOOP_DEPTH[iStThis][iStOther];
      constexpr int depthToCheck = walkUpStoreToFirstWrap(iStOther, cmnDepth);

      bool thisSchedSmaller = (iStThis < iStOther);
      bool otherSchedMaxIterSatisfied = true;
      if constexpr (depthToCheck >= 0) {
        if constexpr (iStThis < iStOther) {
          thisSchedSmaller = (NextStoreSched[iStThis][depthToCheck] <=
                              NextStoreSched[iStOther][depthToCheck]);
        } else {
          thisSchedSmaller = (NextStoreSched[iStThis][depthToCheck] <
                              NextStoreSched[iStOther][depthToCheck]);
        }

        UnrolledLoop<DI.STORE_LOOP_DEPTH[iStOther], cmnDepth>([&](auto iD) {
          if constexpr (DI.STORE_IS_MAX_ITER_NEEDED[iStOther][iD]) {
            // otherSchedMaxIterSatisfied &= StoreAckIsMaxIter[iStOther][iD];
            otherSchedMaxIterSatisfied &= NextStoreIsMaxIter[iStOther][iD];
          }
        });
      }

      return (thisSchedSmaller ||
              (NextStoreAddrPlusBurst[iStThis] <= StoreAckAddr[iStOther] &&
               otherSchedMaxIterSatisfied));
    };

    auto checkNoWAR = [&](const auto iLd, const auto iSt) {
      constexpr int cmnDepth = DI.COMMON_LOOP_DEPTH[iLd][iSt];
      constexpr int depthForMinIter =
          walkUpLoadToFirstWrap(iLd, DI.COMMON_LOOP_DEPTH[iLd][iSt]);

      bool hasMinSched = true;
      bool loadSchedEqual = true;
      bool loadSchedGreater = (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == FORWARD);

      if constexpr (depthForMinIter >= 0) {
        if constexpr (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == FORWARD) {
          loadSchedEqual = ((LoadAckSched[iLd][depthForMinIter] + 1) ==
                            NextStoreSched[iSt][depthForMinIter]);
          loadSchedGreater = ((LoadAckSched[iLd][depthForMinIter] + 1) >
                              NextStoreSched[iSt][depthForMinIter]);
        } else {
          loadSchedEqual = (LoadAckSched[iLd][depthForMinIter] ==
                            NextStoreSched[iSt][depthForMinIter]);
          loadSchedGreater = (LoadAckSched[iLd][depthForMinIter] >
                              NextStoreSched[iSt][depthForMinIter]);
        }

        bool maxIterSatisfied = true;
        UnrolledLoop<DI.LOAD_LOOP_DEPTH[iLd], cmnDepth>([&](auto iD) {
          if constexpr (DI.LOAD_IS_MAX_ITER_NEEDED[iLd][iD]) {
            maxIterSatisfied &= LoadAckIsMaxIter[iLd][iD];
          }
        });

        hasMinSched = (loadSchedEqual && maxIterSatisfied); 
        // || //(loadSchedEqual && //LoadIsMaxIter[iLd][depthForMinIter + 1]);
      }

      return (loadSchedGreater || hasMinSched);
      // return LoadAckAddr[iLd] == LOAD_ADDR_SENTINEL || loadSchedGreater ||
      //        (hasMinSched && LoadAckAddr[iLd] >= NextStoreAddr[iSt]) ||
            //  (LoadAckSched[iLd][cmnDepth] >= NextStoreSched[iSt][cmnDepth]);
    };

    /// The below is called when checking against NextStore and against a match
    /// in the store commit queue. Thus, it uses function parameters.
    auto storeHasMinSched = [&](const auto iLd, const auto iSt,
                                const sched_t(&LoadSched)[LOOP_DEPTH],
                                const sched_t(&StoreSched)[LOOP_DEPTH],
                                const bool(&StoreIsMaxIter)[LOOP_DEPTH]) {
      constexpr int cmnDepth = DI.COMMON_LOOP_DEPTH[iLd][iSt];
      constexpr int depthToCheck = walkUpStoreToFirstWrap(iSt, cmnDepth);

      bool hasMinSched = true;
      if constexpr (depthToCheck >= 0) {
        bool storeSchedEqual = true, storeSchedGreater = true;

        if constexpr (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == BACK) {
          storeSchedEqual =
              ((StoreSched[depthToCheck] + 1) == LoadSched[depthToCheck]);
          storeSchedGreater =
              ((StoreSched[depthToCheck] + 1) > LoadSched[depthToCheck]);
        } else {
          storeSchedEqual =
              (StoreSched[depthToCheck] == LoadSched[depthToCheck]);
          storeSchedGreater =
              (StoreSched[depthToCheck] > LoadSched[depthToCheck]);
        }

        // By definition of walkUpToFirstWrap, no maxIter required between
        // COMMON_LOOP_DEPTH[iLd][iSt] and kDepthForMinIter.
        bool maxIterSatisfied = true;
        UnrolledLoop<DI.STORE_LOOP_DEPTH[iSt], cmnDepth>([&](auto iD) {
          if constexpr (DI.STORE_IS_MAX_ITER_NEEDED[iSt][iD]) {
            maxIterSatisfied &= StoreIsMaxIter[iD];
          }
        });

        hasMinSched =
            storeSchedGreater || (storeSchedEqual && maxIterSatisfied);
      }

      return hasMinSched;
    };

    bool EndSignal = false;
    bool AnyStoresLeft = true;
    bool AnyLoadsLeft = true;

    [[maybe_unused]] uint cycle = 0; // Used only for debug prints.

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    while (!EndSignal) {
      cycle++;

      /** Rule enabling termination signal once all stores and loads have
          finished (MAX_INT sentinel value received). */
      if (!AnyStoresLeft && !AnyLoadsLeft)
        EndSignal = true;

      AnyStoresLeft = false;
      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        if (StoreAckAddr[iSt] != STORE_ADDR_SENTINEL)
          AnyStoresLeft = true;
      });
      AnyLoadsLeft = false;
      UnrolledLoop<NUM_LOADS>([&](auto iLd) {
        if (LoadAckAddr[iLd] != FINAL_LD_ADDR_ACK)
          AnyLoadsLeft = true;
      });
      /** End Rule */

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////     LOAD LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////
      UnrolledLoop<NUM_LOADS>([&](auto iLd) {
        /** Rule for reading load ACKs (always enabled). */ 
        bool succ = false;
        auto ldAck = LoadMuxAckPipes::template PipeAt<iLd>::read(succ);
        if (succ) {
          // PRINTF("mem%d load%d ack addr %d\n", int(MEM_ID), int(iLd), ldAck.addr);
          LoadAckAddr[iLd] = ldAck.addr;
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            LoadAckSched[iLd][iD] = ldAck.sched[iD];
            LoadAckIsMaxIter[iLd][iD] = ldAck.isMaxIter[iD];
          });
        }
        /** End Rule */

        /** Rule for shifting load queues. */ 
        if (!LoadValid[iLd][0]) {
          if (LoadValid[iLd][1]) {
            NextLoadAddr[iLd] = LoadAddr[iLd][1];
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              NextLoadSched[iLd][iD] = LoadSched[iLd][iD][1];
              NextLoadIsMaxIter[iLd][iD] = LoadIsMaxIter[iLd][iD][1];
            });
            UnrolledLoop<NUM_STORES>([&](auto iSt) {
              NextLoadPosDepDist[iLd][iSt] = LoadPosDepDist[iLd][iSt][1];
            });
          }

          ShiftBundle(LoadValid[iLd], false);
          ShiftBundle(LoadAddr[iLd], INVALID_ADDR);
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            ShiftBundle(LoadSched[iLd][iD], 0u);
            ShiftBundle(LoadIsMaxIter[iLd][iD], false);
          });
          UnrolledLoop<NUM_STORES>(
              [&](auto iSt) { ShiftBundle(LoadPosDepDist[iLd][iSt], false); });
        }

        if (!SafeLoadValid[iLd][0]) {
          ShiftBundle(SafeLoadValid[iLd], false);
          ShiftBundle(SafeLoadAddr[iLd], INVALID_ADDR);
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            ShiftBundle(SafeLoadSched[iLd][iD], 0u);
            ShiftBundle(SafeLoadIsMaxIter[iLd][iD], false);
          });
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
            store_req_t<LOOP_DEPTH> MuxAck{SafeLoadAddr[iLd][0]};
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              MuxAck.sched[iD] = SafeLoadSched[iLd][iD][0];
              MuxAck.isMaxIter[iD] = SafeLoadIsMaxIter[iLd][iD][0];
            });
            LoadMuxPredPipes::template PipeAt<iLd>::write(
                {SafeLoadReuse[iLd][0], MuxAck}, SafeLoadMuxPipeSucc[iLd][0]);
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
          const auto LoadReq = LoadAddrPipes::template PipeAt<iLd>::read(succ);
          if (succ) {
            LoadValid[iLd][LD_Q_SIZE - 1] = true;
            LoadAddr[iLd][LD_Q_SIZE - 1] = LoadReq.addr;
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              LoadSched[iLd][iD][LD_Q_SIZE - 1] = LoadReq.sched[iD];
              LoadIsMaxIter[iLd][iD][LD_Q_SIZE - 1] = LoadReq.isMaxIter[iD];
            });
            UnrolledLoop<NUM_STORES>([&](auto iSt) {
              LoadPosDepDist[iLd][iSt][LD_Q_SIZE - 1] = LoadReq.posDepDist[iSt];
            });
          }
        }
        /** End Rule */

        /** Rule for checking load safety and reuse. */
        if (LoadValid[iLd][0] && !SafeLoadValid[iLd][LD_Q_SIZE - 1]) {
          bool ThisReuse[NUM_STORES];
          sched_t ThisReuseSched[NUM_STORES][LOOP_DEPTH];
          bool ThisReuseIsMaxIter[NUM_STORES][LOOP_DEPTH];
          T ThisReuseVal[NUM_STORES];
          bool NoRAW = true;
          UnrolledLoop<NUM_STORES>([&](auto iSt) {
            // Load is before store if dep is 'back' and and no common loop.
            bool LoadBeforeStore = (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == BACK);
            if constexpr (DI.COMMON_LOOP_DEPTH[iLd][iSt] >= 0) {
              // If common loop then compare the iteration of common the common 
              // loop. Use ">=" if ld before st in the common loop, else ">".
              if constexpr (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == BACK) {
                LoadBeforeStore =
                    (NextStoreSched[iSt][DI.COMMON_LOOP_DEPTH[iLd][iSt]] >=
                     NextLoadSched[iLd][DI.COMMON_LOOP_DEPTH[iLd][iSt]]);
              } else {
                LoadBeforeStore =
                    (NextStoreSched[iSt][DI.COMMON_LOOP_DEPTH[iLd][iSt]] >
                     NextLoadSched[iLd][DI.COMMON_LOOP_DEPTH[iLd][iSt]]);
              }
            }

            bool LoadHasPosDepDistance = false;
            if constexpr (DI.ARE_IN_SAME_LOOP[iLd][iSt]) {
              LoadHasPosDepDistance =
                  canCheckPosDepDist(iLd, iSt) && NextLoadPosDepDist[iLd];
            }

            bool minSchedSatisfied =
                storeHasMinSched(iLd, iSt, NextLoadSched[iLd],
                                 NextStoreSched[iSt], NextStoreIsMaxIter[iSt]);

            bool StoreHasLargerAddress = NextStoreAddr[iSt] > NextLoadAddr[iLd];

            NoRAW &= (LoadBeforeStore || LoadHasPosDepDistance ||
                      (minSchedSatisfied && StoreHasLargerAddress));

            ThisReuse[iSt] = false;
            InitBundle(ThisReuseSched[iSt], 0u);
            ThisReuseVal[iSt] = T{};
            #pragma unroll
            for (int i = 0; i < ST_COMMIT_Q_SIZE; ++i) {
              // On multiple matches this will choose the one with highest tag.
              if (NextLoadAddr[iLd] == StoreCommitAddr[iSt][i]) {
                ThisReuse[iSt] = true;
                ThisReuseVal[iSt] = StoreCommitVal[iSt][i];
                UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
                  ThisReuseSched[iSt][iD] = StoreCommitSched[iSt][iD][i];
                  ThisReuseIsMaxIter[iSt][iD] = StoreCommitIsMaxIter[iSt][iD][i];
                });
              }
            }
          });

          // If no hazards against any store, then move load to "safe" stage.
          if (NoRAW) {
            bool Reuse = false;
            T ReuseVal = T{};

            UnrolledLoop<2>([&](auto iEval) {
              UnrolledLoop<NUM_STORES>([&](auto iSt) {
                constexpr auto DepDir = DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt];
                // Forward deps have precedence (i.e. evaluated last).
                if constexpr ((iEval == 0 && DepDir == BACK) ||
                              (iEval == 1 && DepDir == FORWARD)) {
                  if (ThisReuse[iSt]) {
                    bool minSchedSatisfied = storeHasMinSched(
                        iLd, iSt, NextLoadSched[iLd], ThisReuseSched[iSt],
                        ThisReuseIsMaxIter[iSt]);

                    if (minSchedSatisfied) {
                      Reuse = true;
                      ReuseVal = ThisReuseVal[iSt];
                    }
                  }
                }
              });
            });
            
            SafeLoadValid[iLd][LD_Q_SIZE - 1] = true;
            SafeLoadAddr[iLd][LD_Q_SIZE - 1] = LoadAddr[iLd][0];
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              SafeLoadSched[iLd][iD][LD_Q_SIZE-1] = NextLoadSched[iLd][iD];
              SafeLoadIsMaxIter[iLd][iD][LD_Q_SIZE-1] = NextLoadIsMaxIter[iLd][iD];
            });
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
        /** Rule for reading store ACKs (always enabled). */ 
        bool succ = false;
        auto stAck = StoreAckPipes::template PipeAt<iSt>::read(succ);
        if (succ) {
          StoreAckAddr[iSt] = stAck;
        }
        /** End Rule */

        /** Rule shifting store allocation queue. */
        if (!StoreAllocValid[iSt][0]) {
          if (StoreAllocValid[iSt][1]) {
            NextStoreAddr[iSt] = StoreAllocAddr[iSt][1];
            NextStoreAddrPlusBurst[iSt] = StoreAllocAddrPlusBurst[iSt][1];
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              NextStoreSched[iSt][iD] = StoreAllocSched[iSt][iD][1];
              NextStoreIsMaxIter[iSt][iD] = StoreAllocIsMaxIter[iSt][iD][1];
            });
          }

          ShiftBundle(StoreAllocValid[iSt], false);
          ShiftBundle(StoreAllocAddr[iSt], INVALID_ADDR);
          ShiftBundle(StoreAllocAddrPlusBurst[iSt], INVALID_ADDR);
          UnrolledLoop<LOOP_DEPTH>([&] (auto iD) {
            ShiftBundle(StoreAllocSched[iSt][iD], 0u);
            ShiftBundle(StoreAllocIsMaxIter[iSt][iD], false);
          });
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
            StoreAllocAddrPlusBurst[iSt][ST_ALLOC_Q_SIZE - 1] =
                StoreReq.addr + BURST_SIZE;
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              StoreAllocSched[iSt][iD][ST_ALLOC_Q_SIZE - 1] =
                  StoreReq.sched[iD];
              StoreAllocIsMaxIter[iSt][iD][ST_ALLOC_Q_SIZE - 1] =
                  StoreReq.isMaxIter[iD];
            });
          }
        }
        /** End Rule */

        /** Rule for getting st val and moving st alloc to st commit queue. */
        bool NoWAW = true;
        UnrolledLoop<NUM_STORES>([&](auto iStOther) {
          if constexpr (iSt != iStOther) {
            NoWAW &= checkNoWAW(iSt, iStOther);
          }
        });

        bool NoWAR = true;
        UnrolledLoop<NUM_LOADS>([&](auto iLd) {
          if constexpr (!DI.ARE_IN_SAME_LOOP[iLd][iSt]) {
            NoWAR &= checkNoWAR(iLd, iSt);
          }
        });

        if (StoreAllocValid[iSt][0] && NoWAW && NoWAR) {
          bool succ = false;
          auto StoreVal = StoreValPipes::template PipeAt<iSt>::read(succ);
          if (succ) {
            StorePortValPipes::template PipeAt<iSt>::write(StoreVal);

            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              ShiftBundle(StoreCommitSched[iSt][iD],
                          StoreAllocSched[iSt][iD][0]);
              ShiftBundle(StoreCommitIsMaxIter[iSt][iD],
                          StoreAllocIsMaxIter[iSt][iD][0]);
            });
            ShiftBundle(StoreCommitVal[iSt], StoreVal);
            ShiftBundle(StoreCommitAddr[iSt], StoreAllocAddr[iSt][0]);

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

    // PRINTF("** DONE StreamingMemory %d\n", MEM_ID);

  });

  return events;
}
