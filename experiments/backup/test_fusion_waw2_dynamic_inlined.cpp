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
struct ack_t {
  addr_t addr;
  sched_t sched[LOOP_DEPTH];
  bool isMaxIter[LOOP_DEPTH];
};

template <int LOOP_DEPTH, typename T>
struct ld_port_req_t {
  ack_t<LOOP_DEPTH> ack;
  T val;
  bool reuse;
};

template <int LOOP_DEPTH, typename T>
struct st_port_req_t {
  T val;
  ack_t<LOOP_DEPTH> ack;
};


#define KERNEL_PRAGMAS [[intel::kernel_args_restrict]] [[intel::max_global_work_dim(0)]] 

using BurstCoalescedLSU = ext::intel::lsu<ext::intel::burst_coalesce<true>,
                                          ext::intel::prefetch<false>>;

/// 512-bit DRAM interface
constexpr uint DRAM_BURST_BYTES = 64;

constexpr addr_t INVALID_ADDR = -1;
constexpr addr_t STORE_ADDR_SENTINEL = (1<<29) - 1;
constexpr addr_t LOAD_ADDR_SENTINEL = (1<<29) - 2;
constexpr addr_t FINAL_LD_ADDR_ACK = STORE_ADDR_SENTINEL + 1;
constexpr sched_t SCHED_SENTINEL = (1<<30);

/// Unique kernel name generators.
template <int MemId> class StreamingMemoryKernel;
template <int PortId> class LoadPortKernel;
template <int PortId> class StorePortKernel;
template <int PortId> class LoadValMuxKernel;

template <int MEM_ID, typename LoadAddrPipes, typename LoadValPipes,
          typename StoreAddrPipes, typename StoreValPipes,
          uint NUM_LOADS, uint NUM_STORES, uint LOOP_DEPTH, typename T>
std::vector<event> StreamingMemory(queue &q, T *data) {
  std::vector<event> events(NUM_STORES + 1);

  constexpr uint BYTES_IN_T = sizeof(T);
  /// num_T_values_in_DRAM_burst * 2.
  constexpr uint BURST_SIZE = 20;
      // ((DRAM_BURST_BYTES + BYTES_IN_T - 1) / BYTES_IN_T); // 16
      // ((DRAM_BURST_BYTES + BYTES_IN_T - 1) / BYTES_IN_T) * 2; // 32

  // Store ports.
  // using StorePortReqPipes = PipeArray<class _StorePortAddr, store_req_t<LOOP_DEPTH>, 0, NUM_STORES>;
  // using StorePortValPipes = PipeArray<class _StorePortVal, T, 0, NUM_STORES>;
  // using StoreAckPipes = PipeArray<class _StoreAck, store_req_t<LOOP_DEPTH>, 2, NUM_STORES>;

  using StorePortReqPipes = PipeArray<class _StorePortReq, st_port_req_t<LOOP_DEPTH, T>, 0, NUM_STORES>;
  using StoreAckPipes = PipeArray<class _StoreAckPipe, ack_t<LOOP_DEPTH>, 4, NUM_STORES>;
  UnrolledLoop<NUM_STORES>([&](auto iSt) {
    constexpr int StPortId = 2000*(MEM_ID+1) + iSt;
    events[iSt] = q.single_task<StorePortKernel<StPortId>>([=]() KERNEL_PRAGMAS {
      ack_t<LOOP_DEPTH> NextAck {0u};
      ack_t<LOOP_DEPTH> LastValidAck {0u};

      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (NextAck.addr != STORE_ADDR_SENTINEL) {
        const auto Req = StorePortReqPipes::template PipeAt<iSt>::read();
        NextAck = Req.ack;

        if (NextAck.addr != STORE_ADDR_SENTINEL) {
          auto StorePtr = ext::intel::device_ptr<T>(data + NextAck.addr);
          BurstCoalescedLSU::store(StorePtr, Req.val);

          StoreAckPipes::template PipeAt<iSt>::write(NextAck);
          LastValidAck = Req.ack;
        }
      }

      // Force any outstanding burst before ACKing addr sentinel.
      atomic_fence(memory_order_seq_cst, memory_scope_work_item);

      auto anchor = sycl::ext::oneapi::experimental::properties(
          sycl::ext::intel::experimental::latency_anchor_id<StPortId>);
      auto constraint = sycl::ext::oneapi::experimental::properties(
          sycl::ext::intel::experimental::latency_constraint<
              StPortId,
              sycl::ext::intel::experimental::latency_control_type::min,
              BURST_SIZE>);
      StoreAckPipes::template PipeAt<iSt>::write(LastValidAck, anchor);
      StoreAckPipes::template PipeAt<iSt>::write(NextAck, constraint);
    });
  });

  // Load ports.
  using LoadPortPipes = PipeArray<class _LoadPortPipes, ld_port_req_t<LOOP_DEPTH, T>, 16, NUM_LOADS>;
  using LoadAckPipes = PipeArray<class _LoadAckPipes, ack_t<LOOP_DEPTH>, 2, NUM_LOADS>;
  UnrolledLoop<NUM_LOADS>([&](auto iLd) {
    constexpr int LoadPortId = 1000*(MEM_ID+1) + iLd;

    q.single_task<LoadValMuxKernel<LoadPortId>>([=]() KERNEL_PRAGMAS {
      auto anchor = sycl::ext::oneapi::experimental::properties(
          sycl::ext::intel::experimental::latency_anchor_id<LoadPortId>);
      auto constraint = sycl::ext::oneapi::experimental::properties(
          sycl::ext::intel::experimental::latency_constraint<
              LoadPortId,
              sycl::ext::intel::experimental::latency_control_type::min, 1>);

      [[maybe_unused]] int NumTotal = 0, NumReused = 0;
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        T Val;
        const auto Req = LoadPortPipes::template PipeAt<iLd>::read();
        if (Req.ack.addr == LOAD_ADDR_SENTINEL)
          break;

        if (Req.reuse) {
          NumReused++;
          Val = Req.val;
        } else {
          // Val = LoadPortValPipes::template PipeAt<iLd>::read();
          auto LoadPtr = ext::intel::device_ptr<T>(data + Req.ack.addr);
          Val = BurstCoalescedLSU::load(LoadPtr);
        }

        // Ensure ld ack is sent after we get the value.
        LoadValPipes::template PipeAt<iLd>::write(Val, anchor);
        LoadAckPipes::template PipeAt<iLd>::write(Req.ack, constraint);
        NumTotal++;
      }

      ack_t<LOOP_DEPTH> maxAck{FINAL_LD_ADDR_ACK};
      InitBundle(maxAck.sched, SCHED_SENTINEL);
      InitBundle(maxAck.isMaxIter, true);
      LoadAckPipes::template PipeAt<iLd>::write(maxAck);
      // PRINTF("** DONE ld%d, reused %d/%d\n", int(iLd), NumReused, NumTotal);
    });
  });

  events[1] = q.single_task<StreamingMemoryKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    /* Store logic registers */
    constexpr uint ST_ALLOC_Q_SIZE = 4;
    bool StoreAllocValid[NUM_STORES][ST_ALLOC_Q_SIZE];
    addr_t StoreAllocAddr[NUM_STORES][ST_ALLOC_Q_SIZE];
    addr_t StoreAllocAddrPlusBurst[NUM_STORES][ST_ALLOC_Q_SIZE];
    sched_t StoreAllocSched[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    bool StoreAllocIsMaxIter[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    // addr_t StoreAllocMaxAddrAtLoop[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    T NextStoreValue[NUM_STORES];
    bool NextStoreValueValid[NUM_STORES];
    
    constexpr uint ST_COMMIT_Q_SIZE = BURST_SIZE;
    addr_t StoreCommitAddr[NUM_STORES][ST_COMMIT_Q_SIZE];
    T StoreCommitVal[NUM_STORES][ST_COMMIT_Q_SIZE];

    // Shortcut for head of StoreAllocQueue
    addr_t NextStoreAddr[NUM_STORES];
    addr_t NextStoreAddrPlusBurst[NUM_STORES];
    sched_t NextStoreSched[NUM_STORES][LOOP_DEPTH];
    bool NextStoreIsMaxIter[NUM_STORES][LOOP_DEPTH];

    addr_t StoreAckAddr[NUM_STORES];
    sched_t StoreAckSched[NUM_STORES][LOOP_DEPTH];
    bool StoreAckIsMaxIter[NUM_STORES][LOOP_DEPTH];

    bool StoreDone[NUM_STORES];

    bool StoreNoOutstandingAcks[NUM_STORES];
    sched_t LastSentStoreSched[NUM_STORES];

    // Init store registers
    UnrolledLoop<NUM_STORES>([&] (auto iSt) {
      InitBundle(StoreAllocValid[iSt], false);
      InitBundle(StoreAllocAddr[iSt], INVALID_ADDR);
      InitBundle(StoreAllocAddrPlusBurst[iSt], INVALID_ADDR);
      UnrolledLoop<LOOP_DEPTH>([&] (auto iD) {
        InitBundle(StoreAllocSched[iSt][iD], 0u);
        InitBundle(StoreAllocIsMaxIter[iSt][iD], false);
      });

      NextStoreValue[iSt] = T{};
      NextStoreValueValid[iSt] = false;

      InitBundle(StoreCommitAddr[iSt], INVALID_ADDR);
      InitBundle(StoreCommitVal[iSt], T{});

      NextStoreAddr[iSt] = INVALID_ADDR;
      NextStoreAddrPlusBurst[iSt] = INVALID_ADDR;
      InitBundle(NextStoreSched[iSt], 0u);
      InitBundle(NextStoreIsMaxIter[iSt], false);

      StoreAckAddr[iSt] = INVALID_ADDR;
      InitBundle(StoreAckSched[iSt], 0u);
      InitBundle(StoreAckIsMaxIter[iSt], false);
      
      StoreDone[iSt] = false;
      
      StoreNoOutstandingAcks[iSt] = true;
      LastSentStoreSched[iSt] = 0u;
    });

    /* Load logic registers */
    constexpr uint LD_Q_SIZE = 4;
    bool LoadValid[NUM_LOADS][LD_Q_SIZE];
    addr_t LoadAddr[NUM_LOADS][LD_Q_SIZE];
    sched_t LoadSched[NUM_LOADS][LOOP_DEPTH][LD_Q_SIZE];
    bool LoadPosDepDist[NUM_LOADS][NUM_STORES][LD_Q_SIZE];
    bool LoadIsMaxIter[NUM_LOADS][LOOP_DEPTH][LD_Q_SIZE];
    // addr_t LoadMaxAddrAtLoop[NUM_LOADS][LOOP_DEPTH][LD_Q_SIZE];

    addr_t LoadAckAddr[NUM_LOADS];
    sched_t LoadAckSched[NUM_LOADS][LOOP_DEPTH];
    bool LoadAckIsMaxIter[NUM_LOADS][LOOP_DEPTH];
    
    // Alias to load_q position 0.
    addr_t NextLoadAddr[NUM_LOADS];
    sched_t NextLoadSched[NUM_LOADS][LOOP_DEPTH];
    bool NextLoadPosDepDist[NUM_LOADS][NUM_STORES];
    bool NextLoadIsMaxIter[NUM_LOADS][LOOP_DEPTH];

    bool LoadDone[NUM_LOADS];

    bool LoadNoOutstandingAcks[NUM_LOADS];
    sched_t LastSentLoadSched[NUM_LOADS];
    
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
      });

      LoadAckAddr[iLd] = INVALID_ADDR;
      InitBundle(LoadAckSched[iLd], 1u);
      InitBundle(LoadAckIsMaxIter[iLd], false);

      NextLoadAddr[iLd] = INVALID_ADDR;
      InitBundle(NextLoadSched[iLd], 0u);
      InitBundle(NextLoadPosDepDist[iLd], false);
      InitBundle(NextLoadIsMaxIter[iLd], false);

      LoadDone[iLd] = false;

      LoadNoOutstandingAcks[iLd] = true;
      LastSentLoadSched[iLd] = 0u;
    });

    /* Lambdas for checking RAW, WAR, WAW hazards. The functions make use of
     * compile-time information, like "at which loop level should we compare
     * schedule values" or "is this a forward/backwards dependence".
     */
    static constexpr auto DI = DepInfo<MEM_ID>{};

    constexpr auto walkUpStoreToFirstWrap = [&](const auto iSt,
                                                const auto start) {
      for (int i = start; i >= 0; --i) {
        if (DI.STORE_IS_MAX_ITER_NEEDED[iSt][i]) {
          return i;
        }
      }

      return -1;
    };
    constexpr auto walkUpLoadToFirstWrap = [&](const auto iLd,
                                              const auto start) {
      for (int i = start; i >= 0; --i) {
        if (DI.LOAD_IS_MAX_ITER_NEEDED[iLd][i]) {
          return i;
        }
      }

      return -1;
    };

    auto checkNoRAW = [&](const auto iLd, const auto iSt) {
      constexpr int cmnLoopDepth = DI.COMMON_LOOP_DEPTH[iLd][iSt];
      constexpr int stLoopDepth = DI.STORE_LOOP_DEPTH[iSt];

      bool storeSchedGreater = (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == BACK);
      if constexpr (cmnLoopDepth >= 0) {
        if constexpr (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == BACK) {
          storeSchedGreater = (NextStoreSched[iSt][cmnLoopDepth] >=
                               NextLoadSched[iLd][cmnLoopDepth]);
        } else {
          storeSchedGreater = (NextStoreSched[iSt][cmnLoopDepth] >
                               NextLoadSched[iLd][cmnLoopDepth]);
        }
      }

      bool LoadHasPosDepDistance = false;
      bool StAddrNoDecrement = true;
      if constexpr (DI.ARE_IN_SAME_LOOP[iLd][iSt]) {
        bool CanCheckPosDepDist = true;
        constexpr int stWrapDepth =
            walkUpStoreToFirstWrap(iSt, stLoopDepth - 1);
        if constexpr (stWrapDepth >= 0) {
          CanCheckPosDepDist = (NextStoreSched[iSt][stWrapDepth] >=
                                NextLoadSched[iLd][stWrapDepth]);

          if constexpr (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == BACK) {
            StAddrNoDecrement = ((NextStoreSched[iSt][stWrapDepth] + 1) >=
                                 NextLoadSched[iLd][stWrapDepth]);
          } else {
            StAddrNoDecrement = (NextStoreSched[iSt][stWrapDepth] >=
                                 NextLoadSched[iLd][stWrapDepth]);
          }
        }

        LoadHasPosDepDistance = CanCheckPosDepDist && NextLoadPosDepDist[iLd];
      } else {
        UnrolledLoop<stLoopDepth, std::max(cmnLoopDepth, 0)>([&](auto iD) {
          if constexpr (DI.STORE_IS_MAX_ITER_NEEDED[iSt][iD]) {
            StAddrNoDecrement &= NextStoreIsMaxIter[iSt][iD];
          }
        });
      }

      bool StoreHasLargerAddress = NextStoreAddr[iSt] > NextLoadAddr[iLd];

      return storeSchedGreater || LoadHasPosDepDistance ||
             (StAddrNoDecrement && StoreHasLargerAddress);
    };

    auto checkNoWAR = [&](const auto iLd, const auto iSt) {
      constexpr int cmnLoopDepth = DI.COMMON_LOOP_DEPTH[iLd][iSt];
      constexpr int loadLoopDepth = DI.LOAD_LOOP_DEPTH[iLd];

      bool ackLoadSchedGreater = (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == FORWARD);
      bool nextLoadSchedGreater = (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == FORWARD);
      bool loadSchedEqaul = true;
      if constexpr (cmnLoopDepth >= 0) {
        constexpr int loadWrapFromCmn = walkUpLoadToFirstWrap(iLd, cmnLoopDepth);
        constexpr int eqCheckLoopDepth =
            (loadWrapFromCmn >= 0) ? loadWrapFromCmn : cmnLoopDepth;

        if constexpr (eqCheckLoopDepth >= 0) {
          loadSchedEqaul = (LoadAckSched[iLd][eqCheckLoopDepth] ==
                            NextStoreSched[iSt][eqCheckLoopDepth]);

          if constexpr (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == FORWARD) {
            loadSchedEqaul |= ((LoadAckSched[iLd][eqCheckLoopDepth] + 1) ==
                               NextStoreSched[iSt][eqCheckLoopDepth]);
          }
        }

        if constexpr (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == FORWARD) {
          ackLoadSchedGreater = (LoadAckSched[iLd][cmnLoopDepth] >=
                                 NextStoreSched[iSt][cmnLoopDepth]);
          nextLoadSchedGreater = (NextLoadSched[iLd][cmnLoopDepth] >=
                                  NextStoreSched[iSt][cmnLoopDepth]);
        } else {
          ackLoadSchedGreater = (LoadAckSched[iLd][cmnLoopDepth] >
                                 NextStoreSched[iSt][cmnLoopDepth]);
          nextLoadSchedGreater = (NextLoadSched[iLd][cmnLoopDepth] >
                                  NextStoreSched[iSt][cmnLoopDepth]);
        }
      }

      bool ldAddrNoDecr = true;
      UnrolledLoop<loadLoopDepth, std::max(cmnLoopDepth, 0)>([&](auto iD) {
        if constexpr (DI.LOAD_IS_MAX_ITER_NEEDED[iLd][iD]) {
          ldAddrNoDecr &= LoadAckIsMaxIter[iLd][iD];
        }
      });

      return // ackLoadSchedGreater ||
             // (nextLoadSchedGreater && LoadNoOutstandingAcks[iLd]) ||
             nextLoadSchedGreater ||
             (loadSchedEqaul &&  // TODO: is equal needed?
              ldAddrNoDecr && LoadAckAddr[iLd] >= NextStoreAddr[iSt]);
    };

    auto checkNoWAW = [&](const auto iSt, const auto iStOther) {
      constexpr int cmnLoopDepth = DI.COMMON_STORE_LOOP_DEPTH[iSt][iStOther];
      constexpr int otherLoopDepth = DI.STORE_LOOP_DEPTH[iStOther];
      constexpr DEP_DIR depDir = (iSt < iStOther) ? BACK : FORWARD;

      bool otherSchedGreater = (iStOther > iSt);
      bool otherSchedEqual = true;
      if constexpr (cmnLoopDepth >= 0) {
        constexpr int otherWrapFromCmn = walkUpStoreToFirstWrap(iSt, cmnLoopDepth);
        constexpr int eqCheckLoopDepth =
            (otherWrapFromCmn >= 0) ? otherWrapFromCmn : cmnLoopDepth;

        if constexpr (eqCheckLoopDepth >= 0) {
          otherSchedEqual = (StoreAckSched[iStOther][eqCheckLoopDepth] ==
                             NextStoreSched[iSt][eqCheckLoopDepth]);
          if constexpr (depDir == BACK) {
            otherSchedEqual |= ((StoreAckSched[iStOther][eqCheckLoopDepth] + 1) 
                                == NextStoreSched[iSt][eqCheckLoopDepth]);
          }
        }

        if constexpr (depDir == BACK) {
          otherSchedGreater = (NextStoreSched[iStOther][cmnLoopDepth] >=
                               NextStoreSched[iSt][cmnLoopDepth]);
          // otherSchedGreater = (StoreAckSched[iStOther][cmnLoopDepth] >=
          //                      NextStoreSched[iSt][cmnLoopDepth]);
        } else {
          otherSchedGreater = (NextStoreSched[iStOther][cmnLoopDepth] >
                               NextStoreSched[iSt][cmnLoopDepth]);
          // otherSchedGreater = (StoreAckSched[iStOther][cmnLoopDepth] >
          //                      NextStoreSched[iSt][cmnLoopDepth]);
        }
      }

      bool otherAddrNoDecr = true;
      UnrolledLoop<otherLoopDepth, std::max(cmnLoopDepth, 0)>([&](auto iD) {
        if constexpr (DI.STORE_IS_MAX_ITER_NEEDED[iStOther][iD]) {
          // otherAddrNoDecr &= NextStoreIsMaxIter[iStOther][iD];
          otherAddrNoDecr &= StoreAckIsMaxIter[iStOther][iD];
        }
      });

      return //otherAckSchedGreater ||
            //  (otherNextSchedGreater && StoreNoOutstandingAcks[iStOther]) ||
             otherSchedGreater ||
             (otherAddrNoDecr && // TODO: Use ack?
            //  TODO: Sched equal
              otherSchedEqual &&
              // StoreAckAddr[iStOther] > NextStoreAddrPlusBurst[iSt]);
              StoreAckAddr[iStOther] > NextStoreAddr[iSt]);
    };

    bool EndSignal = false;
    bool StoresDone = false;
    bool LoadsDone = false;

    [[maybe_unused]] uint cycle = 0; // Used only for debug prints.

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    while (!EndSignal) {
      cycle++;

      /** Rule enabling termination signal once all stores and loads have
          finished (MAX_INT sentinel value received). */
      if (LoadsDone && LoadsDone)
        EndSignal = true;

      StoresDone = true;
      UnrolledLoop<NUM_STORES>([&](auto iSt) { StoresDone &= StoreDone[iSt]; });
      LoadsDone = true;
      UnrolledLoop<NUM_LOADS>([&](auto iLd) { LoadsDone &= LoadDone[iLd]; });
      /** End Rule */

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////     LOAD LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////
      UnrolledLoop<NUM_LOADS>([&](auto iLd) {
        LoadDone[iLd] = (LoadAckAddr[iLd] == FINAL_LD_ADDR_ACK);

        /** Rule for reading load ACKs (always enabled). */ 
        bool succ = false;
        auto ldAck = LoadAckPipes::template PipeAt<iLd>::read(succ);
        if (succ) {
          LoadAckAddr[iLd] = ldAck.addr;
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            LoadAckSched[iLd][iD] = ldAck.sched[iD];
            LoadAckIsMaxIter[iLd][iD] = ldAck.isMaxIter[iD];
          });

        }
        /** End Rule */

        /** Rule for shifting load queue. */ 
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
        if (LoadValid[iLd][0]) {
          bool ThisReuse[NUM_STORES];
          T ThisReuseVal[NUM_STORES];
          bool NoRAW = true;
          UnrolledLoop<NUM_STORES>([&](auto iSt) {
            NoRAW &= checkNoRAW(iLd, iSt);
            
            ThisReuse[iSt] = false;
            ThisReuseVal[iSt] = T{};
            #pragma unroll
            for (int i = 0; i < ST_COMMIT_Q_SIZE; ++i) {
              // On multiple matches this will choose the one with highest tag.
              if (NextLoadAddr[iLd] == StoreCommitAddr[iSt][i]) {
                ThisReuse[iSt] = true;
                ThisReuseVal[iSt] = StoreCommitVal[iSt][i];
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
                    Reuse = true;
                    ReuseVal = ThisReuseVal[iSt];
                  }
                }
              });
            });

            bool succ = false;
            ack_t<LOOP_DEPTH> MuxAck{NextLoadAddr[iLd]};
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              MuxAck.sched[iD] = NextLoadSched[iLd][iD];
              MuxAck.isMaxIter[iD] = NextLoadIsMaxIter[iLd][iD];
            });
            LoadPortPipes::template PipeAt<iLd>::write(
                {MuxAck, ReuseVal, Reuse}, succ);

            if (succ) {
              LoadValid[iLd][0] = false;
              // LastSentLoadSched[iLd] = NextLoadSched[iLd][DI.LOAD_LOOP_DEPTH[iLd]];
            }
          } 
        }
        /** End Rule */

        // LoadNoOutstandingAcks[iLd] =
        //     (LastSentLoadSched[iLd] == LoadAckSched[iLd][DI.LOAD_LOOP_DEPTH[iLd]]);

      }); // End for all loads.

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////    STORE LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////
      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        StoreDone[iSt] = (StoreAckAddr[iSt] == STORE_ADDR_SENTINEL);

        /** Rule for reading store ACKs (always enabled). */ 
        bool succ = false;
        auto stAck = StoreAckPipes::template PipeAt<iSt>::read(succ);
        if (succ) {
          StoreAckAddr[iSt] = stAck.addr;
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            StoreAckSched[iSt][iD] = stAck.sched[iD];
            StoreAckIsMaxIter[iSt][iD] = stAck.isMaxIter[iD];
          });
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

        /** Rule for reading store req. */
        if (!StoreAllocValid[iSt][ST_ALLOC_Q_SIZE - 1]) {
          auto StoreReq = StoreAddrPipes::template PipeAt<iSt>::read(
              StoreAllocValid[iSt][ST_ALLOC_Q_SIZE - 1]);
          StoreAllocAddr[iSt][ST_ALLOC_Q_SIZE - 1] = StoreReq.addr;
          StoreAllocAddrPlusBurst[iSt][ST_ALLOC_Q_SIZE - 1] =
              StoreReq.addr;
              // StoreReq.addr + BURST_SIZE;
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            StoreAllocSched[iSt][iD][ST_ALLOC_Q_SIZE - 1] = StoreReq.sched[iD];
            StoreAllocIsMaxIter[iSt][iD][ST_ALLOC_Q_SIZE - 1] =
                StoreReq.isMaxIter[iD];
          });
        }
        /** End Rule */

        /** Rule for reading store value. */
        if (!NextStoreValueValid[iSt]) {
          NextStoreValue[iSt] = StoreValPipes::template PipeAt<iSt>::read(
              NextStoreValueValid[iSt]);
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

        const bool IsSafe = (NoWAW && NoWAR);
        if (StoreAllocValid[iSt][0] && ((NextStoreValueValid[iSt] && IsSafe) ||
            (NextStoreAddr[iSt] == STORE_ADDR_SENTINEL))) {
          ack_t<LOOP_DEPTH> ack{NextStoreAddr[iSt]};
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            ack.sched[iD] = NextStoreSched[iSt][iD];
            ack.isMaxIter[iD] = NextStoreIsMaxIter[iSt][iD];
          });

          bool succ = false;
          StorePortReqPipes::template PipeAt<iSt>::write(
              {NextStoreValue[iSt], ack}, succ);

          if (succ) {
            ShiftBundle(StoreCommitVal[iSt], NextStoreValue[iSt]);
            ShiftBundle(StoreCommitAddr[iSt], NextStoreAddr[iSt]);

            StoreAllocValid[iSt][0] = false;
            NextStoreValueValid[iSt] = false;

            LastSentStoreSched[iSt] = NextStoreSched[iSt][DI.STORE_LOOP_DEPTH[iSt]];

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

        // StoreNoOutstandingAcks[iSt] =
        //     (LastSentStoreSched[iSt] == StoreAckSched[iSt][DI.STORE_LOOP_DEPTH[iSt]]);


      }); // End for all stores

    } // end while

    // PRINTF("** DONE StreamingMemory %d\n", MEM_ID);

  });

  return events;
}




















#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <vector>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "memory_utils.hpp"
#include "exception_handler.hpp"
#include "device_print.hpp"

#include "pipe_utils.hpp"
#include "unrolled_loop.hpp"


using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double test_kernel_war(queue &q, const int NI, const int NJ, const int NK,
                       const int NUM_ITERS, std::vector<int> &h_D) {

  int *D = fpga_tools::toDevice(h_D, q);

  constexpr int NUM_LOADS = 1;
  constexpr int NUM_STORES = 3;
  constexpr int LOOP_DEPTH = 3;
  using LoadAddrPipes = PipeArray<class _LoadAddrC, load_req_t<NUM_STORES, LOOP_DEPTH>, 16, NUM_LOADS>;
  using LoadValPipes = PipeArray<class _LoadValC, int, 16, NUM_LOADS>;

  using StoreAddrPipes = PipeArray<class _StoreAddrC, store_req_t<LOOP_DEPTH>, 16, NUM_STORES>;
  using StoreValPipes = PipeArray<class _StoreValC, int, 16, NUM_STORES>;

  q.single_task<class AGU0>([=]() [[intel::kernel_args_restrict]] {
    store_req_t<LOOP_DEPTH> st_req_1 {0u};
    InitBundle(st_req_1.sched, 0u);
    InitBundle(st_req_1.isMaxIter, false);

    for (uint iters = 0; iters < NUM_ITERS; iters++) {
      st_req_1.sched[0]++;
      st_req_1.isMaxIter[0] = (iters + 1) == NUM_ITERS;
     
      for (uint j = 0; j < NI; j++) {
        st_req_1.sched[1]++;
        st_req_1.isMaxIter[1] = (j + 1) == NI;
      
        for (uint i = 0; i < NI; i++) {
          st_req_1.sched[2]++;
          st_req_1.isMaxIter[2] = (i + 1) == NI;
          
          st_req_1.addr = i;
          StoreAddrPipes::PipeAt<0>::write(st_req_1);
        }
      }
    }
  
    st_req_1.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req_1.sched, SCHED_SENTINEL);
    InitBundle(st_req_1.isMaxIter, true);
    StoreAddrPipes::PipeAt<0>::write(st_req_1);
    // PRINTF("DONE AGU 0\n");
  });

  q.single_task<class AGU1>([=]() [[intel::kernel_args_restrict]] {
    store_req_t<LOOP_DEPTH> st_req_1 {0u};
    InitBundle(st_req_1.sched, 0u);
    InitBundle(st_req_1.isMaxIter, false);

    for (uint iters = 0; iters < NUM_ITERS; iters++) {
      st_req_1.sched[0]++;
      st_req_1.isMaxIter[0] = (iters + 1) == NUM_ITERS;
     
      for (uint j = 0; j < NI; j++) {
        st_req_1.sched[1]++;
        st_req_1.isMaxIter[1] = (j + 1) == NI;
      
        st_req_1.addr = j;
        StoreAddrPipes::PipeAt<1>::write(st_req_1);
      }
    }
  
    st_req_1.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req_1.sched, SCHED_SENTINEL);
    InitBundle(st_req_1.isMaxIter, true);
    StoreAddrPipes::PipeAt<1>::write(st_req_1);
    // PRINTF("DONE AGU 1\n");
  });

  q.single_task<class AGU2>([=]() [[intel::kernel_args_restrict]] {
    load_req_t<NUM_STORES, LOOP_DEPTH> ld_req_1 {0u};
    InitBundle(ld_req_1.sched, 0u);
    InitBundle(ld_req_1.posDepDist, false);
    InitBundle(ld_req_1.isMaxIter, false);
    store_req_t<LOOP_DEPTH> st_req_1 {INVALID_ADDR};
    InitBundle(st_req_1.sched, 0u);
    InitBundle(st_req_1.isMaxIter, false);

    for (uint iters = 0; iters < NUM_ITERS; iters++) {
      st_req_1.sched[0]++;
      ld_req_1.sched[0]++;
      st_req_1.isMaxIter[0] = (iters + 1) == NUM_ITERS;
      ld_req_1.isMaxIter[0] = (iters + 1) == NUM_ITERS;

      for (uint i = 0; i < NI; i++) {
        st_req_1.sched[1]++;
        ld_req_1.sched[1]++;
        st_req_1.isMaxIter[1] = (i+1) == NI;
        ld_req_1.isMaxIter[1] = (i+1) == NI;

        ld_req_1.addr = i;
        LoadAddrPipes::PipeAt<0>::write(ld_req_1);

        st_req_1.addr = i;
        StoreAddrPipes::PipeAt<2>::write(st_req_1);
      }
    }

    ld_req_1.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_1.sched, SCHED_SENTINEL);
    InitBundle(ld_req_1.isMaxIter, true);
    InitBundle(ld_req_1.posDepDist, true);
    st_req_1.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req_1.sched, SCHED_SENTINEL);
    InitBundle(st_req_1.isMaxIter, true);
    LoadAddrPipes::PipeAt<0>::write(ld_req_1);
    StoreAddrPipes::PipeAt<2>::write(st_req_1);
    
    // PRINTF("DONE AGU 2\n");
  });

  auto memEvents = StreamingMemory<2, LoadAddrPipes, LoadValPipes,
                                   StoreAddrPipes, StoreValPipes, 
                                   NUM_LOADS, NUM_STORES, LOOP_DEPTH>(q, D);

  auto event = q.single_task<class MainKernel0>([=]() [[intel::kernel_args_restrict]] {
    for (uint iters = 0; iters < NUM_ITERS; iters++) {

      for (uint j = 0; j < NI; j++) {
        for (uint i = 0; i < NI; i++) {
          // D[i] = iters;
          StoreValPipes::PipeAt<0>::write(1000);
        }
      }
    }
  });
  
  auto event1 = q.single_task<class MainKernel1>([=]() [[intel::kernel_args_restrict]] {
    for (uint iters = 0; iters < NUM_ITERS; iters++) {

      for (uint j = 0; j < NI; j++) {
        // for (int i = 0; i < NI; i++) {
          // D[i] = iters;
          StoreValPipes::PipeAt<1>::write(1);
        // }
      }
    }
  });

  auto event2 = q.single_task<class MainKernel2>([=]() [[intel::kernel_args_restrict]] {
    for (uint iters = 0; iters < NUM_ITERS; iters++) {
      for (uint i = 0; i < NI; i++) {
        // D[i] += 1;
        // if () {
        auto ldVal = LoadValPipes::PipeAt<0>::read();
        // PRINTF("ldVal at i=%d is %d\n", i, ldVal)
        StoreValPipes::PipeAt<2>::write(ldVal + 20);
        // if (ldVal != 2) {
        // }
      }
    }

    // MemEndSignal::write(0);
  });

  event.wait();
  event1.wait();
  event2.wait();
  for (auto &e : memEvents) e.wait();
  
  q.copy(D, h_D.data(), h_D.size()).wait();

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void test_kernel_cpu(const int NI, const int NJ, const int NK, const int NUM_ITERS,
                     std::vector<int> &D) {
    for (int iters = 0; iters < NUM_ITERS; iters++) {

      for (int j = 0; j < NI; j++) {
        for (int i = 0; i < NI; i++) {
          D[i] = 1000;
        }
      }
      
      for (int j = 0; j < NI; j++) {
        // for (int i = 0; i < NI; i++) {
          D[j] = 1;
        // }
      }

      for (int i = 0; i < NI; i++) {
        // if (D[i] < 1000000)
          D[i] += 20;
      }

    }
}

int main(int argc, char *argv[]) {
  int N = 10;
  int NUM_ITERS = 1;
  try {
    if (argc > 1)
      N = int(atoi(argv[1]));
    if (argc > 1)
      NUM_ITERS = int(atoi(argv[2]));
  } catch (exception const &e) {
    std::cout << "Incorrect argv.\nUsage:\n"
              << "  ./executable [ARRAY_SIZE] [PERCENTAGE (% of iterations "
                 "with dependencies.)]\n";
    std::terminate();
  }

#if FPGA_SIM
  auto d_selector = sycl::ext::intel::fpga_simulator_selector_v;
#elif FPGA_HW 
  auto d_selector = sycl::ext::intel::fpga_selector_v;
#else  // #if FPGA_EMULATOR
  auto d_selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif
  try {
    // Enable profiling.
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    const int NI = N;
    const int NJ = N;
    const int NK = N;
    const int S = N*N;

    std::vector<int> D(S, 4); 
    std::vector<int> D_cpu(S, 4); 

    auto kernel_time = test_kernel_war(q, NI, NJ, NK, NUM_ITERS, D);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    test_kernel_cpu(NI, NJ, NK, NUM_ITERS, D_cpu);

    if (std::equal(D.begin(), D.end(), D_cpu.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failed\n";

      for (int i = 0; i < S; ++i) {
        if (D[i] != D_cpu[i]) {
          std::cout << i << ": " << D[i] << " != " << D_cpu[i] << "\n";
        }
      }
    }
    
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
