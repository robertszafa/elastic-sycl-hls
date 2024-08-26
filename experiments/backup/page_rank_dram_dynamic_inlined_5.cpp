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

// using BurstCoalescedLSU = ext::intel::lsu<ext::intel::burst_coalesce<true>,
//                                           ext::intel::prefetch<false>>;
using BurstCoalescedLSU = sycl::ext::intel::experimental::lsu<
  sycl::ext::intel::experimental::burst_coalesce<false>,
  sycl::ext::intel::experimental::statically_coalesce<false>>;

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
template <int PortId> class StoreAckDelayKernel;
template <int PortId> class LoadValMuxKernel;

template <int MEM_ID, typename LoadAddrPipes, typename LoadValPipes,
          typename StoreAddrPipes, typename StoreValPipes,
          uint NUM_LOADS, uint NUM_STORES, uint LOOP_DEPTH, typename T>
std::vector<event> StreamingMemory(queue &q, T *data) {
  std::vector<event> events(NUM_STORES + 1);

  // constexpr uint BYTES_IN_T = sizeof(T);
  /// num_T_values_in_DRAM_burst * 2.
  constexpr uint BURST_SIZE = 32;
      // ((DRAM_BURST_BYTES + BYTES_IN_T - 1) / BYTES_IN_T); // 16
      // ((DRAM_BURST_BYTES + BYTES_IN_T - 1) / BYTES_IN_T) * 2; // 32

  // Store ports.
  // using StorePortReqPipes = PipeArray<class _StorePortAddr, store_req_t<LOOP_DEPTH>, 0, NUM_STORES>;
  // using StorePortValPipes = PipeArray<class _StorePortVal, T, 0, NUM_STORES>;
  // using StoreAckPipes = PipeArray<class _StoreAck, store_req_t<LOOP_DEPTH>, 2, NUM_STORES>;

  using StorePortReqPipes = PipeArray<class _StorePortReq, st_port_req_t<LOOP_DEPTH, T>, 0, NUM_STORES>;
  using StoreAckToDelayPipes = PipeArray<class _StoreAckToDelayPipes, ack_t<LOOP_DEPTH>, 64, NUM_STORES>;
  using StoreAckPipes = PipeArray<class _StoreAckPipe, ack_t<LOOP_DEPTH>, 4, NUM_STORES>;
  UnrolledLoop<NUM_STORES>([&](auto iSt) {
    constexpr int StPortId = 100*(MEM_ID+1) + iSt;
    events[iSt] = q.single_task<StorePortKernel<StPortId>>([=]() KERNEL_PRAGMAS {
      // auto anchor = sycl::ext::oneapi::experimental::properties(
      //     sycl::ext::intel::experimental::latency_anchor_id<StPortId>);
      // auto constraint = sycl::ext::oneapi::experimental::properties(
      //     sycl::ext::intel::experimental::latency_constraint<
      //         StPortId,
      //         sycl::ext::intel::experimental::latency_control_type::min,
      //         1>);

      ack_t<LOOP_DEPTH> Ack{INVALID_ADDR};
      InitBundle(Ack.sched, 0u);
      InitBundle(Ack.isMaxIter, false);

      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        StoreAckToDelayPipes::template PipeAt<iSt>::write(Ack);

        const auto Req = StorePortReqPipes::template PipeAt<iSt>::read();
        if (Req.ack.addr == STORE_ADDR_SENTINEL) break;

        auto StorePtr = ext::intel::device_ptr<T>(data + Req.ack.addr);
        BurstCoalescedLSU::store(StorePtr, Req.val);
        Ack = Req.ack;
      }

      // Force any outstanding burst before ACKing addr sentinel.
      atomic_fence(memory_order_seq_cst, memory_scope_work_item);

      ack_t<LOOP_DEPTH> FinalAck{STORE_ADDR_SENTINEL};
      InitBundle(FinalAck.sched, SCHED_SENTINEL);
      InitBundle(FinalAck.isMaxIter, true);
      StoreAckToDelayPipes::template PipeAt<iSt>::write(FinalAck);
    });
    
    q.single_task<StoreAckDelayKernel<StPortId>>([=]() KERNEL_PRAGMAS {
      addr_t AckAddrQ[BURST_SIZE];
      sched_t AckSchedQ[LOOP_DEPTH][BURST_SIZE];
      bool AckIsMaxIterQ[LOOP_DEPTH][BURST_SIZE];
      InitBundle(AckAddrQ, INVALID_ADDR);
      UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
        InitBundle(AckSchedQ[iD], 0u);
        InitBundle(AckIsMaxIterQ[iD], false);
      });

      ack_t<LOOP_DEPTH> NextAck {INVALID_ADDR};
      InitBundle(NextAck.sched, 0u);
      InitBundle(NextAck.isMaxIter, false);

      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        ack_t<LOOP_DEPTH> AckToSend {AckAddrQ[0]};
        UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
          AckToSend.sched[iD] = AckSchedQ[iD][0];
          AckToSend.isMaxIter[iD] = AckIsMaxIterQ[iD][0];
        });
        StoreAckPipes::template PipeAt<iSt>::write(AckToSend);

        bool succ = false;
        const auto Ack = StoreAckToDelayPipes::template PipeAt<iSt>::read(succ);
        if (succ) NextAck = Ack;

        ShiftBundle(AckAddrQ, NextAck.addr);
        UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
          ShiftBundle(AckSchedQ[iD], NextAck.sched[iD]);
          ShiftBundle(AckIsMaxIterQ[iD], NextAck.isMaxIter[iD]);
        });

        if (AckToSend.addr == STORE_ADDR_SENTINEL) break;
      }
    });
  });

  // Load ports.
  using LoadPortPipes = PipeArray<class _LoadPortPipes, ld_port_req_t<LOOP_DEPTH, T>, 16, NUM_LOADS>;
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

      return storeSchedGreater || 
            //  LoadHasPosDepDistance ||
             (storeSchedEqual && 
              StAddrNoDecrement && StoreHasLargerAddress);
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
              ldAddrNoDecr && LoadAckAddr[iLd] >= NextStoreAddr[iSt]);
    };

    auto checkNoWAW = [&](const auto iSt, const auto iStOther) {
      constexpr int cmnLoopDepth = DI.COMMON_STORE_LOOP_DEPTH[iSt][iStOther];
      constexpr int otherLoopDepth = DI.STORE_LOOP_DEPTH[iStOther];
      constexpr DEP_DIR depDir = (iSt < iStOther) ? BACK : FORWARD;

      bool otherSchedGreater = (iStOther > iSt);
      bool ackSchedGreater = (iStOther > iSt);
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

      return ackSchedGreater ||
            (otherSchedGreater && StoreNoOutstandingAcks[iStOther]) ||
            //  otherSchedGreater ||
             (otherAddrNoDecr && // TODO: Use ack?
            //  TODO: Sched equal
              otherSchedEqual &&
              StoreAckAddr[iStOther] > NextStoreAddrPlusBurst[iSt]);
              // StoreAckAddr[iStOther] > NextStoreAddr[iSt]);
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
              LastSentLoadSched[iLd] = NextLoadSched[iLd][DI.LOAD_LOOP_DEPTH[iLd]];
              LoadValid[iLd][0] = false;
            }
          }
        }
        /** End Rule */

        LoadNoOutstandingAcks[iLd] =
            (LastSentLoadSched[iLd] == LoadAckSched[iLd][DI.LOAD_LOOP_DEPTH[iLd]]);


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
              // StoreReq.addr;
              StoreReq.addr + 32;
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

            StoreAllocValid[iSt][0] = false;
            NextStoreValueValid[iSt] = false;
          }
        }
        /** End Rule */

        StoreNoOutstandingAcks[iSt] =
            (LastSentStoreSched[iSt] == StoreAckSched[iSt][DI.STORE_LOOP_DEPTH[iSt]]);


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

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

// Set the damping factor 'd'
constexpr float d = 0.85;

double page_rank_kernel(queue &q, const std::vector<int> &h_row_ptr,
                        const std::vector<int> &h_col_idx,
                        std::vector<float> &h_val, std::vector<float> &h_p,
                        const int numNodes, const int maxIters) {
  const int *row_ptr = fpga_tools::toDevice(h_row_ptr, q);
  const int *col_idx = fpga_tools::toDevice(h_col_idx, q);
  const float *val = fpga_tools::toDevice(h_val, q);
  float *p = fpga_tools::toDevice(h_p, q);
  float *p_new = fpga_tools::toDevice(h_p, q);
  
  constexpr int LOOP_DEPTH = 3;

  constexpr int NUM_LOADS_p_new = 2;
  constexpr int NUM_STORES_p_new = 2;
  constexpr int LOOP_DEPTH_p_new = 3;
  using LoadAddrPipes_p_new = PipeArray<class _LoadAddr_p_new, load_req_t<NUM_STORES_p_new, LOOP_DEPTH_p_new>, 16, NUM_LOADS_p_new>;
  using LoadValPipes_p_new = PipeArray<class _LoadVal_p_new, float, 16, NUM_LOADS_p_new>;
  using StoreAddrPipes_p_new = PipeArray<class _StoreAddr_p_new, store_req_t<LOOP_DEPTH_p_new>, 16, NUM_STORES_p_new>;
  using StoreValPipes_p_new = PipeArray<class _StoreVal_p_new, float, 16, NUM_STORES_p_new>;
  
  constexpr int NUM_LOADS_p = 1;
  constexpr int NUM_STORES_p = 1;
  constexpr int LOOP_DEPTH_p = 3;
  using LoadAddrPipes_p = PipeArray<class _LoadAddr_p, load_req_t<NUM_STORES_p, LOOP_DEPTH_p>, 16, NUM_LOADS_p>;
  using LoadValPipes_p = PipeArray<class _LoadVal_p, float, 16, NUM_LOADS_p>;
  using StoreAddrPipes_p = PipeArray<class _StoreAddr_p, store_req_t<LOOP_DEPTH_p>, 16, NUM_STORES_p>;
  using StoreValPipes_p = PipeArray<class _StoreVal_p, float, 16, NUM_STORES_p>;

  q.single_task<class AGU_l0>([=]() [[intel::kernel_args_restrict]] {
    store_req_t<LOOP_DEPTH> st_req {0u};
    InitBundle(st_req.sched, 0u);
    InitBundle(st_req.isMaxIter, false);

    for (uint iter = 0; iter < maxIters; ++iter) {
      st_req.sched[0]++;
      st_req.isMaxIter[0] = (iter + 1) == maxIters;

      for (uint i = 0; i < numNodes; i++) {
        st_req.sched[1]++;
        st_req.isMaxIter[1] = (i + 1) == numNodes;

        st_req.addr = i;
        StoreAddrPipes_p_new::PipeAt<0>::write(st_req);
      }
    }

    st_req.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req.sched, SCHED_SENTINEL);
    InitBundle(st_req.isMaxIter, true);
    StoreAddrPipes_p_new::PipeAt<0>::write(st_req);
  });

  q.single_task<class AGU_l1>([=]() [[intel::kernel_args_restrict]] {
    store_req_t<LOOP_DEPTH> st_req_p_new {INVALID_ADDR};
    InitBundle(st_req_p_new.sched, 0u);
    InitBundle(st_req_p_new.isMaxIter, false);

    load_req_t<NUM_STORES_p_new, LOOP_DEPTH> ld_req_p_new {INVALID_ADDR};
    InitBundle(ld_req_p_new.sched, 0u);
    InitBundle(ld_req_p_new.posDepDist, false);
    InitBundle(ld_req_p_new.isMaxIter, false);

    load_req_t<NUM_STORES_p, LOOP_DEPTH> ld_req_p {INVALID_ADDR};
    InitBundle(ld_req_p.sched, 0u);
    InitBundle(ld_req_p.posDepDist, false);
    InitBundle(ld_req_p.isMaxIter, false);

    for (uint iter = 0; iter < maxIters; ++iter) {
      st_req_p_new.sched[0]++;
      st_req_p_new.isMaxIter[0] = (iter + 1) == maxIters;

      ld_req_p_new.sched[0]++;
      ld_req_p_new.isMaxIter[0] = (iter + 1) == maxIters;

      ld_req_p.sched[0]++;
      ld_req_p.isMaxIter[0] = (iter + 1) == maxIters;

      int rowel = 0;
      int curcol = 0;

      for (uint i = 0; i < numNodes; i++) {
        st_req_p_new.sched[1]++;
        st_req_p_new.isMaxIter[1] = (i+1) == numNodes;

        ld_req_p_new.sched[1]++;
        ld_req_p_new.isMaxIter[1] = (i+1) == numNodes;

        ld_req_p.sched[1]++;
        ld_req_p.isMaxIter[1] = (i+1) == numNodes;

        rowel = row_ptr[i + 1] - row_ptr[i];

        int local_curcol = curcol;
        int local_i = i;
        int local_rowel = rowel;
        for (int j = 0; j < local_rowel; j++) {
          st_req_p_new.sched[2]++;
          st_req_p_new.isMaxIter[2] = (j+1) == local_rowel;

          ld_req_p_new.sched[2]++;
          ld_req_p_new.isMaxIter[2] = (j+1) == local_rowel;

          ld_req_p.sched[2]++;
          ld_req_p.isMaxIter[2] = (j+1) == local_rowel;

          ld_req_p.addr = local_i;
          LoadAddrPipes_p::PipeAt<0>::write(ld_req_p);

          ld_req_p_new.addr = col_idx[local_curcol];
          ld_req_p_new.posDepDist[1] = ld_req_p_new.addr > st_req_p_new.addr;
          LoadAddrPipes_p_new::PipeAt<0>::write(ld_req_p_new);

          st_req_p_new.addr = col_idx[local_curcol];
          StoreAddrPipes_p_new::PipeAt<1>::write(st_req_p_new);

          local_curcol++;
        }

        curcol += local_rowel;
      }
    }

    ld_req_p.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_p.sched, SCHED_SENTINEL);
    InitBundle(ld_req_p.isMaxIter, true);
    InitBundle(ld_req_p.posDepDist, true);
    LoadAddrPipes_p::PipeAt<0>::write(ld_req_p);

    ld_req_p_new.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_p_new.sched, SCHED_SENTINEL);
    InitBundle(ld_req_p_new.isMaxIter, true);
    InitBundle(ld_req_p_new.posDepDist, true);
    LoadAddrPipes_p_new::PipeAt<0>::write(ld_req_p_new);

    st_req_p_new.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req_p_new.sched, SCHED_SENTINEL);
    InitBundle(st_req_p_new.isMaxIter, true);
    StoreAddrPipes_p_new::PipeAt<1>::write(st_req_p_new);

  });

  q.single_task<class AGU_l2>([=]() [[intel::kernel_args_restrict]] {
    store_req_t<LOOP_DEPTH> st_req_p {0u};
    InitBundle(st_req_p.sched, 0u);
    InitBundle(st_req_p.isMaxIter, false);

    load_req_t<NUM_STORES_p_new, LOOP_DEPTH> ld_req_p_new {0u};
    InitBundle(ld_req_p_new.sched, 0u);
    InitBundle(ld_req_p_new.posDepDist, false);
    InitBundle(ld_req_p_new.isMaxIter, false);

    for (uint iter = 0; iter < maxIters; ++iter) {
      st_req_p.sched[0]++;
      st_req_p.isMaxIter[0] = (iter + 1) == maxIters;

      ld_req_p_new.sched[0]++;
      ld_req_p_new.isMaxIter[0] = (iter + 1) == maxIters;

      for (uint i = 0; i < numNodes; i++) {
        st_req_p.sched[1]++;
        st_req_p.isMaxIter[1] = (i+1) == numNodes;

        ld_req_p_new.sched[1]++;
        ld_req_p_new.isMaxIter[1] = (i+1) == numNodes;

        ld_req_p_new.addr = i;
        LoadAddrPipes_p_new::PipeAt<1>::write(ld_req_p_new);

        st_req_p.addr = i;
        StoreAddrPipes_p::PipeAt<0>::write(st_req_p);
      }
    }

    ld_req_p_new.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_p_new.sched, SCHED_SENTINEL);
    InitBundle(ld_req_p_new.isMaxIter, true);
    InitBundle(ld_req_p_new.posDepDist, true);
    LoadAddrPipes_p_new::PipeAt<1>::write(ld_req_p_new);

    st_req_p.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req_p.sched, SCHED_SENTINEL);
    InitBundle(st_req_p.isMaxIter, true);
    StoreAddrPipes_p::PipeAt<0>::write(st_req_p);

  });

  auto memEvents =
      StreamingMemory<4, LoadAddrPipes_p_new, LoadValPipes_p_new,
                      StoreAddrPipes_p_new, StoreValPipes_p_new,
                      NUM_LOADS_p_new, NUM_STORES_p_new, LOOP_DEPTH_p_new>(q, p_new);

  auto memEvents2 =
      StreamingMemory<5, LoadAddrPipes_p, LoadValPipes_p, StoreAddrPipes_p,
                      StoreValPipes_p, NUM_LOADS_p, NUM_STORES_p, LOOP_DEPTH_p>(q, p);

  auto event1 = q.single_task<class MainKernel0>([=]() [[intel::kernel_args_restrict]] {
    for (uint iter = 0; iter < maxIters; ++iter) {
      for (uint i = 0; i < numNodes; i++) {
        StoreValPipes_p_new::PipeAt<0>::write(0.0f);
      }
    }
  });

  auto event2 = q.single_task<class MainKernel1>([=]() [[intel::kernel_args_restrict]] {
    for (uint iter = 0; iter < maxIters; ++iter) {
      int curcol = 0;
      for (uint i = 0; i < numNodes; i++) {

        int rowel = row_ptr[i + 1] - row_ptr[i];
        int local_curcol = curcol;
        for (int j = 0; j < rowel; j++) {
          
          auto LoadVal_p = LoadValPipes_p::PipeAt<0>::read();
          auto LoadVal_p_new = LoadValPipes_p_new::PipeAt<0>::read();
          auto StoreVal_p_new = LoadVal_p_new + val[local_curcol] * LoadVal_p;
          StoreValPipes_p_new::PipeAt<1>::write(StoreVal_p_new);

          local_curcol++;
        }
        curcol += rowel;
      }
    }
  });

  auto event3 = q.single_task<class MainKernel2>([=]() [[intel::kernel_args_restrict]] {
    for (uint iter = 0; iter < maxIters; ++iter) {
      for (uint i = 0; i < numNodes; i++) {
        auto LoadVal_p_new = LoadValPipes_p_new::PipeAt<1>::read();
        auto valP = d * LoadVal_p_new + (1.0f - d) / numNodes;
        StoreValPipes_p::PipeAt<0>::write(valP);
      }
    }
  });

  event1.wait();
  event2.wait();
  event3.wait();

  for (auto &e : memEvents) e.wait();
  for (auto &e2 : memEvents2) e2.wait();

  q.copy(p, h_p.data(), h_p.size()).wait();

  sycl::free((void*) row_ptr, q);
  sycl::free((void*) col_idx, q);
  sycl::free((void*) val, q);
  sycl::free(p, q);
  sycl::free(p_new, q);

  auto start = event1.get_profiling_info<info::event_profiling::command_start>();
  auto end = event3.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

inline bool almost_equal(const float x, const float y) {
  const float ulpFloat = static_cast<float>(2);
  const float tolerance = 0.01f;
  return fabsf(x - y) <=
             tolerance * fabsf(x + y) * ulpFloat ||
         fabsf(x - y) < std::numeric_limits<float>::min();
}

void page_rank_cpu(const std::vector<int> &row_ptr,
                   const std::vector<int> &col_idx, std::vector<float> &val,
                   std::vector<float> &p, const int numNodes,
                   const int maxIters) {
  std::vector<float> p_new(p.size());

  for (int iter = 0; iter < maxIters; ++iter) {

    // Initialize p_new as a vector of n 0.0 cells
    for (int i = 0; i < numNodes; i++) {
      p_new[i] = 0.0f; // id = iter
    }

    int rowel = 0;
    int curcol = 0;

    // Sparse PageRank algorithm using an adjacency matrix in CSR format.
    for (int i = 0; i < numNodes; i++) {
      rowel = row_ptr[i + 1] - row_ptr[i];
      for (int j = 0; j < rowel; j++) {
        p_new[col_idx[curcol]] += val[curcol] * p[i]; // id = i = (numNodes-1)
        curcol++;
      }
    }

    for (int i = 0; i < numNodes; i++) {
      p[i] = d * p_new[i] + (1.0f - d) / numNodes;
    }
  }

}

int main(int argc, char *argv[]) {
  int maxIters = 10;
  FILE *fp;
  try {
    if (argc < 2) {
      throw std::invalid_argument("Need filename and max iters.");
    }

    char *filename = argv[1];
    if ((fp = fopen(filename, "r")) == NULL) {
      throw std::invalid_argument("Invalid filename.");
    }
    
    maxIters = int(atoi(argv[2]));
  } catch (exception const &e) {
    std::cout << "Incorrect argv.\nUsage: ./executable [maxIters]\n";
    std::terminate();
  }

  ////////////////////////////////////////////////////////////////////////////
  ////// INIT DATA
  ////////////////////////////////////////////////////////////////////////////
  int numNodes = 0, numEdges = 0;

  // Read the data set and get the number of nodes (n) and edges (e)
  // int n, e;
  char ch;
  char str[100];
  ch = getc(fp);
  while (ch == '#') {
    fgets(str, 100 - 1, fp);
    // Debug: print title of the data set
    // printf("%s",str);
    sscanf(str, "%*s %d %*s %d", &numNodes, &numEdges); // number of nodes
    ch = getc(fp);
  }
  ungetc(ch, fp);

  // DEBUG: Print the number of nodes and edges, skip everything else
  printf("\nGraph data: Nodes: %d, Edges: %d \n\n", numNodes, numEdges);

  auto sizeArr = std::max(numEdges + 1, numNodes + 1);
  std::vector<int> row_ptr(sizeArr, 0), col_idx(sizeArr, 0);
  std::vector<float> val(sizeArr, 0.0f), p(sizeArr, 0.0f);
  
  int fromnode, tonode;
  int cur_row = 0;
  int i = 0;
  // Elements for row
  int elrow = 0;
  // Cumulative numbers of elements
  int curel = 0;
  
  while(!feof(fp)){
    // fscanf(fp,"%d%d",&fromnode,&tonode);
    fscanf(fp, "%d%d", &tonode, &fromnode);
    
    if (tonode > cur_row) { // change the row
      curel = curel + elrow;
      for (int k = cur_row + 1; k <= tonode; k++) {
        row_ptr[k] = curel;
      }
      elrow = 0;
      cur_row = tonode;
    }
    val[i] = 1.0;
    col_idx[i] = fromnode;
    elrow++;
    i++;
  }
  row_ptr[cur_row+1] = curel + elrow - 1;
  ////////////////////////////////////////////////////////////////////////////
  ////// END INIT DATA
  ////////////////////////////////////////////////////////////////////////////

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

    std::vector<float> p_cpu(sizeArr, 0.0f);
    std::copy(p.begin(), p.end(), p_cpu.begin());

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    auto kernel_time = page_rank_kernel(q, row_ptr, col_idx, val, p, numNodes, maxIters);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    page_rank_cpu(row_ptr, col_idx, val, p_cpu, numNodes, maxIters);
    
    if (std::equal(p.begin(), p.end(), p_cpu.begin(), almost_equal)) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failed\n";

      for (int i = 0; i < numNodes; ++i) {
        if (!almost_equal(p[i], p_cpu[i])) {
          std::cout << i << ": fpga != cpu, " << p[i] << " != " << p_cpu[i] << "\n";
        }
      }
    }

  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
