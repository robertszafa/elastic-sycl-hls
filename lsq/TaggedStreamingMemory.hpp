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

using addr_t = uint;
using sched_t = uint;

template <int LOOP_DEPTH>
struct st_req_t {
  addr_t addr;
  sched_t sched[LOOP_DEPTH];
  bool isMaxIter[LOOP_DEPTH];
};

template <int NUM_STORES, int LOOP_DEPTH>
struct ld_req_t {
  addr_t addr;
  sched_t sched[LOOP_DEPTH];
  bool isMaxIter[LOOP_DEPTH];
  bool posDepDist[NUM_STORES];
};

template <int LOOP_DEPTH>
using ack_t = st_req_t<LOOP_DEPTH>;

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

using BurstCoalescedLSU =
    ext::intel::lsu<ext::intel::burst_coalesce<true>,
                    ext::intel::statically_coalesce<false>,
                    ext::intel::prefetch<false>>;


constexpr addr_t STORE_ADDR_SENTINEL = (1<<29) - 1;
constexpr addr_t LOAD_ADDR_SENTINEL = (1<<29) - 2;
constexpr addr_t FINAL_LD_ADDR_ACK = STORE_ADDR_SENTINEL + 1;
constexpr sched_t SCHED_SENTINEL = (1<<30);

/// Unique kernel name generators.
template <int MemId> class StreamingMemoryKernel;
template <int PortId> class LoadPortKernel;
template <int PortId> class StorePortKernel;
template <int PortId> class StoreAckDelayKernel;
template <int PortId> class LoadValMuxKernel;

template <int MEM_ID, typename LoadAddrPipes, typename LoadValPipes,
          typename StoreAddrPipes, typename StoreValPipes,
          uint NUM_LOADS, uint NUM_STORES, uint LOOP_DEPTH, typename T>
std::vector<event> StreamingMemory(queue &q, T *data) {
  std::vector<event> events(NUM_STORES + 1);

  constexpr uint DRAM_BURST_BYTES = 64; /// 512-bit DRAM interface
  constexpr uint T_BYTES = sizeof(T);
  constexpr uint BURST_SIZE = ((DRAM_BURST_BYTES + T_BYTES - 1) / T_BYTES);

  // Store ports.
  using StorePortValPipes = PipeArray<class _StorePortValPipes, T, 1, NUM_STORES>;
  using StoreAckPipes = PipeArray<class _StoreAckPipe, ack_t<LOOP_DEPTH>, 2, NUM_STORES>;
  UnrolledLoop<NUM_STORES>([&](auto iSt) {
    constexpr int StPortId = 100*(MEM_ID+1) + iSt;
    events[iSt] = q.single_task<StorePortKernel<StPortId>>([=]() KERNEL_PRAGMAS {
      ack_t<LOOP_DEPTH> NextAck {0u};
      InitBundle(NextAck.sched, 0u);
      InitBundle(NextAck.isMaxIter, false);

      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        StoreAckPipes::template PipeAt<iSt>::write(NextAck);

        const auto Req = StoreAddrPipes::template PipeAt<iSt, 0>::read();
        if (Req.addr == STORE_ADDR_SENTINEL) break;

        auto Val = StorePortValPipes::template PipeAt<iSt>::read();
        auto StorePtr = ext::intel::device_ptr<T>(data + Req.addr);
        BurstCoalescedLSU::store(StorePtr, Val);
        NextAck = Req;
      }

      // Force any outstanding burst before ACKing addr sentinel.
      atomic_fence(memory_order_seq_cst, memory_scope_work_item);

      ack_t<LOOP_DEPTH> FinalAck{STORE_ADDR_SENTINEL};
      InitBundle(FinalAck.sched, SCHED_SENTINEL);
      InitBundle(FinalAck.isMaxIter, true);
      StoreAckPipes::template PipeAt<iSt>::write(FinalAck);
    });
  });

  // Load ports.
  using LoadPortPipes = PipeArray<class _LoadPortPipes, ld_port_req_t<LOOP_DEPTH, T>, 1, NUM_LOADS>;
  using LoadAckPipes = PipeArray<class _LoadAckPipes, ack_t<LOOP_DEPTH>, 2, NUM_LOADS>;
  UnrolledLoop<NUM_LOADS>([&](auto iLd) {
    constexpr int LoadPortId = 2000*(MEM_ID+1) + iLd;
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
        const auto Req = LoadPortPipes::template PipeAt<iLd>::read();
        if (Req.ack.addr == LOAD_ADDR_SENTINEL)
          break;

        T Val;
        if (Req.reuse) {
          NumReused++;
          Val = Req.val;
        } else {
          auto LoadPtr = ext::intel::device_ptr<T>(data + Req.ack.addr);
          Val = BurstCoalescedLSU::load(LoadPtr);
        }

        // Ensure ld ack is sent after we get the value by using a constraint.
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
    sched_t StoreAllocSched[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    bool StoreAllocIsMaxIter[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    T NextStoreValue[NUM_STORES];
    bool NextStoreValueValid[NUM_STORES];
    // addr_t StoreAllocMaxAddrAtLoop[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];

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

    // Init store registers
    UnrolledLoop<NUM_STORES>([&] (auto iSt) {
      InitBundle(StoreAllocValid[iSt], false);
      InitBundle(StoreAllocAddr[iSt], 0u);
      UnrolledLoop<LOOP_DEPTH>([&] (auto iD) {
        InitBundle(StoreAllocSched[iSt][iD], 0u);
        InitBundle(StoreAllocIsMaxIter[iSt][iD], false);
      });

      NextStoreValue[iSt] = T{};
      NextStoreValueValid[iSt] = false;

      InitBundle(StoreCommitAddr[iSt], STORE_ADDR_SENTINEL);
      InitBundle(StoreCommitVal[iSt], T{});

      NextStoreAddr[iSt] = 0u;
      NextStoreAddrPlusBurst[iSt] = 0u;
      InitBundle(NextStoreSched[iSt], 0u);
      InitBundle(NextStoreIsMaxIter[iSt], false);

      StoreAckAddr[iSt] = 0u;
      InitBundle(StoreAckSched[iSt], 0u);
      InitBundle(StoreAckIsMaxIter[iSt], false);

      StoreDone[iSt] = false;
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
      InitBundle(LoadAddr[iLd], 0u);
      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        InitBundle(LoadPosDepDist[iLd][iSt], false);
      });
      UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
        InitBundle(LoadSched[iLd][iD], 0u);
        InitBundle(LoadIsMaxIter[iLd][iD], false);
      });

      LoadAckAddr[iLd] = 0u;
      InitBundle(LoadAckSched[iLd], 1u);
      InitBundle(LoadAckIsMaxIter[iLd], false);

      NextLoadAddr[iLd] = 0u;
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
      [[maybe_unused]] bool storeSchedLower = (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] != BACK);
      bool storeSchedEqual = true;
      if constexpr (cmnLoopDepth >= 0) {
        constexpr int stWrapFromCmn =
            DI.ARE_IN_SAME_LOOP[iLd][iSt]
                ? walkUpStoreToFirstWrap(iSt, cmnLoopDepth - 1)
                : walkUpStoreToFirstWrap(iSt, cmnLoopDepth);
        constexpr int eqCheckLoopDepth =
            (stWrapFromCmn >= 0) ? stWrapFromCmn : cmnLoopDepth;

        if constexpr (eqCheckLoopDepth >= 0) {
          storeSchedEqual = (NextStoreSched[iSt][eqCheckLoopDepth] ==
                             NextLoadSched[iLd][eqCheckLoopDepth]);
          if constexpr (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == BACK) {
            storeSchedEqual |= ((NextStoreSched[iSt][eqCheckLoopDepth] + 1) ==
                                NextLoadSched[iLd][eqCheckLoopDepth]);
          }
        }

        if constexpr (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == BACK) {
          storeSchedGreater = (NextStoreSched[iSt][cmnLoopDepth] >=
                               NextLoadSched[iLd][cmnLoopDepth]);
          storeSchedLower = (NextStoreSched[iSt][cmnLoopDepth] <
                               NextLoadSched[iLd][cmnLoopDepth]);
        } else {
          storeSchedGreater = (NextStoreSched[iSt][cmnLoopDepth] >
                               NextLoadSched[iLd][cmnLoopDepth]);
          storeSchedLower = (NextStoreSched[iSt][cmnLoopDepth] <=
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
          // CanCheckPosDepDist = (StoreAckSched[iSt][stWrapDepth] >=
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

      // if (storeSchedEqual && StAddrNoDecrement && StoreHasLargerAddress) {
      // if (!storeSchedGreater) {
      // if (!storeSchedGreater && !NextLoadPosDepDist[iLd]) {
      //   SafeLoadWaitForStoreAck[iLd][iSt][LD_Q_SIZE - 1] =
      //       NextStoreSched[iSt][DI.STORE_LOOP_DEPTH[iSt]];
      // }

      return storeSchedGreater ||
             LoadHasPosDepDistance ||
             (StAddrNoDecrement && StoreHasLargerAddress);
              // && storeSchedEqual
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

      return ackLoadSchedGreater ||
             (nextLoadSchedGreater && LoadNoOutstandingAcks[iLd]) ||
            //  nextLoadSchedGreater ||
             (loadSchedEqaul &&  // TODO: is equal needed?
              ldAddrNoDecr && LoadAckAddr[iLd] > NextStoreAddr[iSt]);
    };

    auto checkNoWAW = [&](const auto iSt, const auto iStOther) {
      constexpr int cmnLoopDepth = DI.COMMON_STORE_LOOP_DEPTH[iSt][iStOther];
      constexpr int otherLoopDepth = DI.STORE_LOOP_DEPTH[iStOther];
      constexpr DEP_DIR depDir = (iSt < iStOther) ? BACK : FORWARD;

      bool otherSchedGreater = (iStOther > iSt);
      [[maybe_unused]] bool ackSchedGreater = (iStOther > iSt);
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
          ackSchedGreater = (StoreAckSched[iStOther][cmnLoopDepth] >=
                             NextStoreSched[iSt][cmnLoopDepth]);
        } else {
          otherSchedGreater = (NextStoreSched[iStOther][cmnLoopDepth] >
                               NextStoreSched[iSt][cmnLoopDepth]);
          ackSchedGreater = (StoreAckSched[iStOther][cmnLoopDepth] >
                             NextStoreSched[iSt][cmnLoopDepth]);
        }
      }

      bool otherAddrNoDecr = true;
      UnrolledLoop<otherLoopDepth, std::max(cmnLoopDepth, 0)>([&](auto iD) {
        if constexpr (DI.STORE_IS_MAX_ITER_NEEDED[iStOther][iD]) {
          // otherAddrNoDecr &= NextStoreIsMaxIter[iStOther][iD];
          otherAddrNoDecr &= StoreAckIsMaxIter[iStOther][iD];
        }
      });

      return // ackSchedGreater ||
            // (otherSchedGreater && StoreNoOutstandingAcks[iStOther]) ||
            otherSchedGreater ||
             (otherAddrNoDecr && // TODO: Use ack? //  TODO: Sched equal
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
      if (StoresDone && LoadsDone)
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

        LoadNoOutstandingAcks[iLd] = (LastSentLoadSched[iLd] ==
                                      LoadAckSched[iLd][DI.LOAD_LOOP_DEPTH[iLd]]);

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
          ShiftBundle(LoadAddr[iLd], 0u);
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
            ld_port_req_t<LOOP_DEPTH, T> Req = {MuxAck, ReuseVal, Reuse};
            LoadPortPipes::template PipeAt<iLd>::write(Req, succ);

            if (succ) {
              LastSentLoadSched[iLd] = NextLoadSched[iLd][DI.LOAD_LOOP_DEPTH[iLd]];
              LoadValid[iLd][0] = false;
            }
          }
        }
        /** End Rule */

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
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              NextStoreSched[iSt][iD] = StoreAllocSched[iSt][iD][1];
              NextStoreIsMaxIter[iSt][iD] = StoreAllocIsMaxIter[iSt][iD][1];
            });
          }

          ShiftBundle(StoreAllocValid[iSt], false);
          ShiftBundle(StoreAllocAddr[iSt], 0u);
          UnrolledLoop<LOOP_DEPTH>([&] (auto iD) {
            ShiftBundle(StoreAllocSched[iSt][iD], 0u);
            ShiftBundle(StoreAllocIsMaxIter[iSt][iD], false);
          });
        }
        /** End Rule */

        /** Rule for reading store req. */
        if (!StoreAllocValid[iSt][ST_ALLOC_Q_SIZE - 1]) {
          auto StoreReq = StoreAddrPipes::template PipeAt<iSt, 1>::read(
              StoreAllocValid[iSt][ST_ALLOC_Q_SIZE - 1]);
          StoreAllocAddr[iSt][ST_ALLOC_Q_SIZE - 1] = StoreReq.addr;
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            StoreAllocSched[iSt][iD][ST_ALLOC_Q_SIZE - 1] = StoreReq.sched[iD];
            StoreAllocIsMaxIter[iSt][iD][ST_ALLOC_Q_SIZE - 1] =
                StoreReq.isMaxIter[iD];
          });
        }
        /** End Rule */

        if (!NextStoreValueValid[iSt]) {
          NextStoreValue[iSt] = StoreValPipes::template PipeAt<iSt>::read(
            NextStoreValueValid[iSt]);
        }

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
        if (StoreAllocValid[iSt][0] && NextStoreValueValid[iSt] && IsSafe) {
          bool succ = false;
          StorePortValPipes::template PipeAt<iSt>::write(NextStoreValue[iSt], succ);

          if (succ) {
            ShiftBundle(StoreCommitVal[iSt], NextStoreValue[iSt]);
            ShiftBundle(StoreCommitAddr[iSt], NextStoreAddr[iSt]);

            // LastSentStoreSched[iSt] = NextStoreSched[iSt][DI.STORE_LOOP_DEPTH[iSt]];

            // Check if this store overrides another store in its burst buffer.
            UnrolledLoop<NUM_STORES>([&](auto iStOther) {
              if constexpr (iStOther != iSt) {
                #pragma unroll
                for (int i = 0; i < ST_COMMIT_Q_SIZE; ++i) {
                  if (NextStoreAddr[iSt] == StoreCommitAddr[iStOther][i]) {
                    // Invalid address, i.e. a load will never requiest this.
                    StoreCommitAddr[iStOther][i] = STORE_ADDR_SENTINEL;
                  }
                }
              }
            });

            StoreAllocValid[iSt][0] = false;
            NextStoreValueValid[iSt] = false;
          }
        }
        /** End Rule */

      }); // End for all stores

    } // end while

    // PRINTF("** DONE StreamingMemory %d\n", MEM_ID);

  });

  return events;
}
