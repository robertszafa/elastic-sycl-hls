class MainKernel_agu;
class MainKernel_1_agu;
class MainKernel_2_agu;
class MainKernel_3_agu;
class MainKernel_1;
class MainKernel_2;
class MainKernel_3;
#ifndef __DEPENDENCY_TABLE_HPP__
  #define __DEPENDENCY_TABLE_HPP__

enum DEP_DIR {
  BACK,    // First load, then store in a loop.
  FORWARD, // First store, then load in a loop.
};

template <int MEM_ID> struct DepInfo;

#endif

template <> struct DepInfo<100> {
  static constexpr int NUM_LOADS = 3;
  static constexpr int NUM_STORES = 3;
  static constexpr int MAX_LOOP_DEPTH = 3;

  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {
      2,
      2,
      1,
  };
  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][MAX_LOOP_DEPTH] = {
      {
          1,
          1,
          0,
      },
      {
          1,
          1,
          0,
      },
      {
          1,
          0,
          0,
      },
  };
  static constexpr bool LOAD_STORE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {
      {
          0,
          1,
          0,
      },
      {
          0,
          0,
          1,
      },
      {
          0,
          0,
          0,
      },
  };
  static constexpr bool LOAD_STORE_IN_SAME_THREAD[NUM_LOADS][NUM_STORES] = {
      {
          0,
          1,
          0,
      },
      {
          0,
          0,
          1,
      },
      {
          0,
          0,
          0,
      },
  };
  static constexpr int LOAD_STORE_COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {
      {
          0,
          2,
          1,
      },
      {
          0,
          1,
          2,
      },
      {
          0,
          0,
          0,
      },
  };
  static constexpr DEP_DIR LOAD_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {
          FORWARD,
          BACK,
          BACK,
      },
      {
          FORWARD,
          FORWARD,
          BACK,
      },
      {
          FORWARD,
          FORWARD,
          FORWARD,
      },
  };

  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {
      1,
      2,
      2,
  };
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][MAX_LOOP_DEPTH] = {
      {
          1,
          0,
          0,
      },
      {
          1,
          1,
          0,
      },
      {
          1,
          1,
          0,
      },
  };
  static constexpr bool STORE_STORE_IN_SAME_LOOP[NUM_STORES][NUM_STORES] = {
      {
          1,
          0,
          0,
      },
      {
          0,
          1,
          0,
      },
      {
          0,
          0,
          1,
      },
  };
  static constexpr int STORE_STORE_COMMON_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {
      {
          1,
          0,
          0,
      },
      {
          0,
          2,
          1,
      },
      {
          0,
          1,
          2,
      },
  };
  static constexpr int STORE_STORE_DEP_DIR[NUM_STORES][NUM_STORES] = {
      {
          FORWARD,
          BACK,
          BACK,
      },
      {
          FORWARD,
          FORWARD,
          BACK,
      },
      {
          FORWARD,
          FORWARD,
          FORWARD,
      },
  };

}; // end DepInfo

template <> struct DepInfo<101> {
  static constexpr int NUM_LOADS = 2;
  static constexpr int NUM_STORES = 1;
  static constexpr int MAX_LOOP_DEPTH = 2;

  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {
      1,
      1,
  };
  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][MAX_LOOP_DEPTH] = {
      {
          1,
          0,
      },
      {
          1,
          0,
      },
  };
  static constexpr bool LOAD_STORE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {
      {
          0,
      },
      {
          0,
      },
  };
  static constexpr bool LOAD_STORE_IN_SAME_THREAD[NUM_LOADS][NUM_STORES] = {
      {
          0,
      },
      {
          0,
      },
  };
  static constexpr int LOAD_STORE_COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {
      {
          0,
      },
      {
          0,
      },
  };
  static constexpr DEP_DIR LOAD_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {
          BACK,
      },
      {
          BACK,
      },
  };

  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {
      1,
  };
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][MAX_LOOP_DEPTH] = {
      {
          1,
          0,
      },
  };
  static constexpr bool STORE_STORE_IN_SAME_LOOP[NUM_STORES][NUM_STORES] = {
      {
          1,
      },
  };
  static constexpr int STORE_STORE_COMMON_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {
      {
          1,
      },
  };
  static constexpr int STORE_STORE_DEP_DIR[NUM_STORES][NUM_STORES] = {
      {
          FORWARD,
      },
  };

}; // end DepInfo









#ifndef __STREAMING_MEMORY_HPP__
#define __STREAMING_MEMORY_HPP__

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

/// Add bits for higher capacity DRAM.
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

// We use a dynamic burst load-store unit supplied by the intel HLS compiler.
using BurstCoalescedLSU =
    ext::intel::lsu<ext::intel::burst_coalesce<true>,
                    ext::intel::statically_coalesce<false>,
                    ext::intel::prefetch<false>>;

constexpr addr_t INIT_ACK_ADDR = (1<<29);
constexpr addr_t STORE_ADDR_SENTINEL = (1<<29) - 1;
constexpr addr_t LOAD_ADDR_SENTINEL = (1<<29) - 2;
constexpr addr_t FINAL_LD_ADDR_ACK = STORE_ADDR_SENTINEL + 1;
constexpr sched_t SCHED_SENTINEL = (1<<30);

/// Unique kernel name generators.
template <int MemId> class StreamingMemoryKernel;
template <int PortId> class StorePortKernel;
template <int PortId> class LoadPortKernel;

// Applaying [[optnone]] to StreamingMemory doesn't apply the attribute to
// nested lambdas, so apply the attribute to a range of source code.
#pragma clang attribute push (__attribute__((optnone)), apply_to=function)

template <int MEM_ID, typename LoadReqPipes, typename LoadValPipes,
          typename StoreReqPipes, typename StoreValPipes, typename T>
std::vector<event> StreamingMemory(queue &q, T *data) {
  constexpr uint NUM_LOADS = DepInfo<MEM_ID>{}.NUM_LOADS;
  constexpr uint NUM_STORES = DepInfo<MEM_ID>{}.NUM_STORES;
  constexpr uint LOOP_DEPTH = DepInfo<MEM_ID>{}.MAX_LOOP_DEPTH;
  std::vector<event> events(NUM_STORES + 1);

  // Sizes of internal buffers.
  constexpr uint DRAM_BURST_BYTES = 64; /// 512-bit DRAM interface
  constexpr uint T_BYTES = sizeof(T);
  constexpr uint BURST_SIZE = ((DRAM_BURST_BYTES + T_BYTES - 1) / T_BYTES) ;
  constexpr uint ST_COMMIT_Q_SIZE = BURST_SIZE * 2;
  constexpr uint ST_ALLOC_Q_SIZE = 2;
  constexpr uint LD_Q_SIZE = 2;

  // Store ports.
  // Because we use a dynamic burstig LSU, it is important that the store port
  // receives store addresses as soon as possible (hence separate addr and val
  // pipes). The LSU can issue a burst sooner if it can see the next addr.
  using StorePortValPipes = PipeArray<class _StorePortValPipes, T, 1, NUM_STORES>;
  using StoreAckPipes = PipeArray<class _StoreAckPipe, ack_t<LOOP_DEPTH>, 2, NUM_STORES>;
  UnrolledLoop<NUM_STORES>([&](auto iSt) {
    constexpr int StPortId = 100*(MEM_ID+1) + iSt;
    events[iSt] = q.single_task<StorePortKernel<StPortId>>([=]() KERNEL_PRAGMAS {
                              ack_t<LOOP_DEPTH> AckForLastSt{INIT_ACK_ADDR};
          InitBundle(AckForLastSt.sched, 0u);
          InitBundle(AckForLastSt.isMaxIter, true);

          constexpr int ACK_DELAY = 16; // 8
          addr_t AckAddr[ACK_DELAY];
          sched_t AckSched[LOOP_DEPTH][ACK_DELAY];
          bool AckIsMaxIter[LOOP_DEPTH][ACK_DELAY];
          InitBundle(AckAddr, INIT_ACK_ADDR);
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            InitBundle(AckSched[iD], 0u);
            InitBundle(AckIsMaxIter[iD], true);
          });

          bool ReqValid = false;
          bool ValValid = false;
          st_req_t<LOOP_DEPTH> StReq;
          T StVal = {};

          [[intel::ivdep]] [[intel::initiation_interval(
              1)]] [[intel::speculated_iterations(0)]] while (true) {
            ack_t<LOOP_DEPTH> NextAck;
            NextAck.addr = AckAddr[0];
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              NextAck.sched[iD] = AckSched[iD][0];
              NextAck.isMaxIter[iD] = AckIsMaxIter[iD][0];
            });
            StoreAckPipes::template PipeAt<iSt>::write(NextAck);

            ShiftBundle(AckAddr, AckForLastSt.addr);
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              ShiftBundle(AckSched[iD], AckForLastSt.sched[iD]);
              ShiftBundle(AckIsMaxIter[iD], AckForLastSt.isMaxIter[iD]);
            });

            if (!ReqValid) {
              StReq = StoreReqPipes::template PipeAt<iSt, 0>::read(ReqValid);
            } else if (StReq.addr == STORE_ADDR_SENTINEL) {
              break;
            }

            if (!ValValid) {
              StVal = StorePortValPipes::template PipeAt<iSt>::read(ValValid);
            }

            if (ReqValid && ValValid) {
              auto StorePtr = ext::intel::device_ptr<T>(data + StReq.addr);
              BurstCoalescedLSU::store(StorePtr, StVal);
              AckForLastSt = StReq;

              ReqValid = false;
              ValValid = false;
            }
          }

          atomic_fence(memory_order_seq_cst, memory_scope_work_item);

          // for (int i = 0; i < ACK_DELAY; ++i) {
          //   ack_t<LOOP_DEPTH> NextAck;
          //   NextAck.addr = AckAddr[i];
          //   UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
          //     NextAck.sched[iD] = AckSched[iD][i];
          //     NextAck.isMaxIter[iD] = AckIsMaxIter[iD][i];
          //   });
          //   StoreAckPipes::template PipeAt<iSt>::write(NextAck);
          // }

          // Force any outstanding burst before ACKing addr sentinel.
          ack_t<LOOP_DEPTH> FinalAck{STORE_ADDR_SENTINEL};
          InitBundle(FinalAck.sched, SCHED_SENTINEL);
          InitBundle(FinalAck.isMaxIter, true);
          StoreAckPipes::template PipeAt<iSt>::write(FinalAck);
    });
  });

  // Load ports.
  using LoadPortReqPipes = PipeArray<class _LoadPortPipes, ld_port_req_t<LOOP_DEPTH, T>, 1, NUM_LOADS>;
  using LoadAckPipes = PipeArray<class _LoadAckPipes, ack_t<LOOP_DEPTH>, 2, NUM_LOADS>;
  UnrolledLoop<NUM_LOADS>([&](auto iLd) {
    constexpr int LoadPortId = 2000*(MEM_ID+1) + iLd;
    q.single_task<LoadPortKernel<LoadPortId>>([=]() KERNEL_PRAGMAS {
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
        const auto Req = LoadPortReqPipes::template PipeAt<iLd>::read();
        if (Req.ack.addr == LOAD_ADDR_SENTINEL)
          break;

        T Val;
        if (Req.reuse) {
          NumReused++;
          Val = Req.val;
        } else {
          // Silence clang warning about int to pointer conversion.
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
      // PRINTF("MEM%d ld%d reused %d/%d\n", MEM_ID, int(iLd), NumReused, NumTotal);
    });
  });


  events[1] = q.single_task<StreamingMemoryKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    // TDOD: Add hints about the maximum address at any given loop depth?
    // addr_t StoreAllocMaxAddrAtLoop[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    // addr_t LoadMaxAddrAtLoop[NUM_LOADS][LOOP_DEPTH][LD_Q_SIZE];

    /* Store logic registers */
    // Internal queue of store requests (purely for improved fmax).
    bool StoreValid[NUM_STORES][ST_ALLOC_Q_SIZE];
    addr_t StoreAddr[NUM_STORES][ST_ALLOC_Q_SIZE];
    sched_t StoreSched[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    bool StoreIsMaxIter[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    // Shortcut for the head of the internal store queue.
    addr_t NextStoreAddr[NUM_STORES];
    sched_t NextStoreSched[NUM_STORES][LOOP_DEPTH];
    bool NextStoreIsMaxIter[NUM_STORES][LOOP_DEPTH];
    T NextStoreValue[NUM_STORES];
    bool NextStoreValueValid[NUM_STORES];
    // This buffer ahadows the burst buffer in the store port.
    addr_t StoreButrstBuffAddr[NUM_STORES][ST_COMMIT_Q_SIZE];
    T StoreBurstBuffVal[NUM_STORES][ST_COMMIT_Q_SIZE];
    // The latest store sent to the HLS memory controller. Note that this is not
    // the actual ACK received from the memory system (no access to thaty in HLS).
    addr_t StoreAckAddr[NUM_STORES];
    sched_t StoreAckSched[NUM_STORES][LOOP_DEPTH];
    bool StoreAckIsMaxIter[NUM_STORES][LOOP_DEPTH];
    // We keep track if all store requests sent to the store port have been ACKed.
    bool StoreNoOutstandingAcks[NUM_STORES];
    sched_t LastSentStoreSched[NUM_STORES];
    // Set to true when all stores have been served (sentinel request value).
    bool StoreDone[NUM_STORES];

    // Init store registers
    UnrolledLoop<NUM_STORES>([&] (auto iSt) {
      InitBundle(StoreValid[iSt], false);
      InitBundle(StoreAddr[iSt], 0u);
      UnrolledLoop<LOOP_DEPTH>([&] (auto iD) {
        InitBundle(StoreSched[iSt][iD], 0u);
        InitBundle(StoreIsMaxIter[iSt][iD], false);
      });

      NextStoreAddr[iSt] = 0u;
      InitBundle(NextStoreSched[iSt], 0u);
      InitBundle(NextStoreIsMaxIter[iSt], false);
      NextStoreValue[iSt] = T{};
      NextStoreValueValid[iSt] = false;

      InitBundle(StoreButrstBuffAddr[iSt], STORE_ADDR_SENTINEL);
      InitBundle(StoreBurstBuffVal[iSt], T{});

      StoreAckAddr[iSt] = INIT_ACK_ADDR;
      InitBundle(StoreAckSched[iSt], 0u);
      InitBundle(StoreAckIsMaxIter[iSt], true);

      StoreNoOutstandingAcks[iSt] = true;
      LastSentStoreSched[iSt] = 0u;

      StoreDone[iSt] = false;
    });

    /* Load logic registers */
    // Internal queue of load requests (purely for improved fmax).
    bool LoadValid[NUM_LOADS][LD_Q_SIZE];
    addr_t LoadAddr[NUM_LOADS][LD_Q_SIZE];
    sched_t LoadSched[NUM_LOADS][LOOP_DEPTH][LD_Q_SIZE];
    bool LoadPosDepDist[NUM_LOADS][NUM_STORES][LD_Q_SIZE];
    bool LoadIsMaxIter[NUM_LOADS][LOOP_DEPTH][LD_Q_SIZE];
    // Alias to load queue head.
    addr_t NextLoadAddr[NUM_LOADS];
    sched_t NextLoadSched[NUM_LOADS][LOOP_DEPTH];
    bool NextLoadPosDepDist[NUM_LOADS][NUM_STORES];
    bool NextLoadIsMaxIter[NUM_LOADS][LOOP_DEPTH];
    // The lates requeust for which we have returned a load value.
    addr_t LoadAckAddr[NUM_LOADS];
    sched_t LoadAckSched[NUM_LOADS][LOOP_DEPTH];
    bool LoadAckIsMaxIter[NUM_LOADS][LOOP_DEPTH];
    // We keep track if all load requests sent to the load port have been ACKed.
    bool LoadNoOutstandingAcks[NUM_LOADS];
    sched_t LastSentLoadSched[NUM_LOADS];
    // Set to true when all loads have been served (sentinel request value).
    bool LoadDone[NUM_LOADS];

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

      LoadAckAddr[iLd] = INIT_ACK_ADDR;
      InitBundle(LoadAckSched[iLd], 0u);
      InitBundle(LoadAckIsMaxIter[iLd], true);

      NextLoadAddr[iLd] = 0u;
      InitBundle(NextLoadSched[iLd], 0u);
      InitBundle(NextLoadPosDepDist[iLd], false);
      InitBundle(NextLoadIsMaxIter[iLd], false);

      LoadDone[iLd] = false;

      LoadNoOutstandingAcks[iLd] = true;
      LastSentLoadSched[iLd] = 0u;
    });

    ///////////////////////////////////////////////////////////////////////////
    // Disambiguation logic. RAW, WAR, WAW hazard checks implemented as lamdas.
    // The dependency info is supplied by the compiler.
    ///////////////////////////////////////////////////////////////////////////
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
                  constexpr int CmnLoopDepth = DI.LOAD_STORE_COMMON_LOOP_DEPTH[iLd][iSt];
      constexpr int StLoopDepth = DI.STORE_LOOP_DEPTH[iSt];
      constexpr int StWrapDepth = walkUpStoreToFirstWrap(iSt, StLoopDepth - 1);

      bool StoreNextSchedGreater = (DI.LOAD_STORE_DEP_DIR[iLd][iSt] == BACK);
      bool StoreAckSchedGreater = (DI.LOAD_STORE_DEP_DIR[iLd][iSt] == BACK);
      if constexpr (CmnLoopDepth >= 0) {
        if constexpr (DI.LOAD_STORE_DEP_DIR[iLd][iSt] == BACK) {
          StoreNextSchedGreater = (NextStoreSched[iSt][CmnLoopDepth] >=
                               NextLoadSched[iLd][CmnLoopDepth]);
          StoreAckSchedGreater = (StoreAckSched[iSt][CmnLoopDepth] >=
                                  NextLoadSched[iLd][CmnLoopDepth]);
        } else {
          StoreNextSchedGreater = (NextStoreSched[iSt][CmnLoopDepth] >
                                   NextLoadSched[iLd][CmnLoopDepth]);
          StoreAckSchedGreater = (StoreAckSched[iSt][CmnLoopDepth] >
                                  NextLoadSched[iLd][CmnLoopDepth]);
        }
      }

      bool StoreSchedEqual = true;
      if constexpr (StWrapDepth >= 0) {
        if constexpr (DI.LOAD_STORE_DEP_DIR[iLd][iSt] == BACK) {
          StoreSchedEqual = ((NextStoreSched[iSt][StWrapDepth] + 1) >=
                              NextLoadSched[iLd][StWrapDepth]);
        } else {
          StoreSchedEqual = (NextStoreSched[iSt][StWrapDepth] >=
                             NextLoadSched[iLd][StWrapDepth]);
        }
      }

      bool LoadHasPosDepDistance = false;
      bool StAddrNoDecrement = true;
      if constexpr (DI.LOAD_STORE_IN_SAME_LOOP[iLd][iSt]) {
        bool CanCheckPosDepDist = true;
        if constexpr (StWrapDepth >= 0) {
          CanCheckPosDepDist = (StoreAckSched[iSt][StWrapDepth] >=
                                NextLoadSched[iLd][StWrapDepth]);
          // CanCheckPosDepDist = (NextStoreSched[iSt][StWrapDepth] >=
          //                       NextLoadSched[iLd][StWrapDepth]);
          // CanCheckPosDepDist = (NextStoreSched[iSt][CmnLoopDepth] >=
          //                       NextLoadSched[iLd][CmnLoopDepth]);

          // if constexpr (DI.LOAD_STORE_DEP_DIR[iLd][iSt] == BACK) {
            // StAddrNoDecrement = ((NextStoreSched[iSt][StWrapDepth] + 1) >=
                                //  NextLoadSched[iLd][StWrapDepth]);
          // } else {
            StAddrNoDecrement = (NextStoreSched[iSt][StWrapDepth] >=
            // StAddrNoDecrement = (StoreAckSched[iSt][StWrapDepth] >=
                                 NextLoadSched[iLd][StWrapDepth]);
          // }
        }

        LoadHasPosDepDistance =
            CanCheckPosDepDist && NextLoadPosDepDist[iLd][iSt];
      } else {
        UnrolledLoop<StLoopDepth, std::max(CmnLoopDepth, 0)>([&](auto iD) {
          if constexpr (DI.STORE_IS_MAX_ITER_NEEDED[iSt][iD]) {
            StAddrNoDecrement &= NextStoreIsMaxIter[iSt][iD];
          }
        });
      }

      bool StoreWillNotTouchLoadAddr = (StoreSchedEqual && StAddrNoDecrement &&
                                        NextStoreAddr[iSt] > NextLoadAddr[iLd]);

      bool StoreSchedGreater =
          StoreAckSchedGreater ||
          (StoreNextSchedGreater && StoreNoOutstandingAcks[iSt]);

      return StoreSchedGreater || LoadHasPosDepDistance ||
             StoreWillNotTouchLoadAddr;
    };

    auto checkNoWAR = [&](const auto iLd, const auto iSt) {
      constexpr int CmnLoopDepth = DI.LOAD_STORE_COMMON_LOOP_DEPTH[iLd][iSt];
      constexpr int LoadLoopDepth = DI.LOAD_LOOP_DEPTH[iLd];

      bool AckLoadSchedGreater = (DI.LOAD_STORE_DEP_DIR[iLd][iSt] == FORWARD);
      bool NextLoadSchedGreater = (DI.LOAD_STORE_DEP_DIR[iLd][iSt] == FORWARD);
      bool LoadSchedEqual = true;
      if constexpr (CmnLoopDepth >= 0) {
        constexpr int LoadWrapFromCmn = walkUpLoadToFirstWrap(iLd, CmnLoopDepth);
        constexpr int EqCheckLoopDepth =
            (LoadWrapFromCmn >= 0) ? LoadWrapFromCmn : CmnLoopDepth;

        if constexpr (EqCheckLoopDepth >= 0) {
          LoadSchedEqual = (LoadAckSched[iLd][EqCheckLoopDepth] ==
                            NextStoreSched[iSt][EqCheckLoopDepth]);

          if constexpr (DI.LOAD_STORE_DEP_DIR[iLd][iSt] == FORWARD) {
            LoadSchedEqual |= ((LoadAckSched[iLd][EqCheckLoopDepth] + 1) ==
                               NextStoreSched[iSt][EqCheckLoopDepth]);
          }
        }

        if constexpr (DI.LOAD_STORE_DEP_DIR[iLd][iSt] == FORWARD) {
          AckLoadSchedGreater = (LoadAckSched[iLd][CmnLoopDepth] >=
                                 NextStoreSched[iSt][CmnLoopDepth]);
          NextLoadSchedGreater = (NextLoadSched[iLd][CmnLoopDepth] >=
                                  NextStoreSched[iSt][CmnLoopDepth]);
        } else {
          AckLoadSchedGreater = (LoadAckSched[iLd][CmnLoopDepth] >
                                 NextStoreSched[iSt][CmnLoopDepth]);
          NextLoadSchedGreater = (NextLoadSched[iLd][CmnLoopDepth] >
                                  NextStoreSched[iSt][CmnLoopDepth]);
        }
      }

      bool LdAddrNoDecr = true;
      UnrolledLoop<LoadLoopDepth, std::max(CmnLoopDepth, 0)>([&](auto iD) {
        if constexpr (DI.LOAD_IS_MAX_ITER_NEEDED[iLd][iD]) {
          LdAddrNoDecr &= LoadAckIsMaxIter[iLd][iD];
        }
      });

      bool LoadSchedIsGreater = AckLoadSchedGreater ||
                          (NextLoadSchedGreater && LoadNoOutstandingAcks[iLd]);
      bool LoadWillNotTouchStoreAddr = (LoadSchedEqual && LdAddrNoDecr &&
                                        LoadAckAddr[iLd] > NextStoreAddr[iSt]);

      return LoadSchedIsGreater || LoadWillNotTouchStoreAddr;
    };

    /// Return true if there is no src->dst WAW hazard.
    auto checkNoWAW = [&](const auto dst, const auto src) {
      // constexpr DEP_DIR DepDir = DI.STORE_STORE_COMMON_LOOP_DEPTH[iStOther][iSt];
      // constexpr int EqCheckLoopDepth =
      //     (SrcWrapFromCmn >= 0) ? SrcWrapFromCmn : CmnLoopDepth;

      constexpr int CmnLoopDepth = DI.STORE_STORE_COMMON_LOOP_DEPTH[dst][src];
      constexpr int SrcLoopDepth = DI.STORE_LOOP_DEPTH[src];
      constexpr DEP_DIR DepDir = (dst < src) ? BACK : FORWARD;
      constexpr int SrcWrapFromCmn = walkUpStoreToFirstWrap(dst, CmnLoopDepth);

      bool SrcSchedEqual = true;
      if constexpr (SrcWrapFromCmn >= 0) {
        SrcSchedEqual = (StoreAckSched[src][SrcWrapFromCmn] ==
                         NextStoreSched[dst][SrcWrapFromCmn]);
        if constexpr (DepDir == BACK) {
          SrcSchedEqual |= ((StoreAckSched[src][SrcWrapFromCmn] + 1) ==
                            NextStoreSched[dst][SrcWrapFromCmn]);
        }
      }

      bool SrcAckSchedGreater = (src > dst);
      bool SrcNextSchedGreater = (src > dst);
      if constexpr (CmnLoopDepth >= 0) {
        if constexpr (DepDir == BACK) {
          SrcAckSchedGreater = (StoreAckSched[src][CmnLoopDepth] >=
                                NextStoreSched[dst][CmnLoopDepth]);
          SrcNextSchedGreater = (NextStoreSched[src][CmnLoopDepth] >=
                                 NextStoreSched[dst][CmnLoopDepth]);
        } else {
          SrcAckSchedGreater = (StoreAckSched[src][CmnLoopDepth] >
                                NextStoreSched[dst][CmnLoopDepth]);
          SrcNextSchedGreater = (NextStoreSched[src][CmnLoopDepth] >
                                 NextStoreSched[dst][CmnLoopDepth]);
        }
      }

      bool SrcAddrNoDecr = true;
      // UnrolledLoop<SrcLoopDepth, std::max(CmnLoopDepth, 0)>([&](auto iD) {
      UnrolledLoop<CmnLoopDepth+1, SrcLoopDepth+1>([&](auto iD) {
        if constexpr (DI.STORE_IS_MAX_ITER_NEEDED[src][iD]) {
          SrcAddrNoDecr &= StoreAckIsMaxIter[src][iD];
        }
      });

      bool SrcWillNotTouchAddr = SrcSchedEqual && SrcAddrNoDecr &&
                                 (StoreAckAddr[src] > (NextStoreAddr[dst] + 16));
      bool SrcSchedGreater =
          SrcAckSchedGreater ||
          (SrcNextSchedGreater && StoreNoOutstandingAcks[src]);

      return SrcSchedGreater || SrcWillNotTouchAddr;
    };
    ///////////////////////////////////////////////////////////////////////////
    // End disambiguation logic.
    ///////////////////////////////////////////////////////////////////////////


    bool EndSignal = false;
    bool StoresDone = false;
    bool LoadsDone = false;

    [[maybe_unused]] uint cycle = 0; // Used only for debug prints.

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    while (!EndSignal) {
      cycle++;

      /** Rule enabling termination signal once all stores & loads are done. */
      EndSignal = (StoresDone && LoadsDone);
      StoresDone = true;
      UnrolledLoop<NUM_STORES>([&](auto iSt) { StoresDone &= StoreDone[iSt]; });
      LoadsDone = true;
      UnrolledLoop<NUM_LOADS>([&](auto iLd) { LoadsDone &= LoadDone[iLd]; });
      /** End Rule */

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////     LOAD LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////
      UnrolledLoop<NUM_LOADS>([&](auto iLd) {
        // This load is done if we receive an ACK with a sentinel value.
        LoadDone[iLd] = (LoadAckSched[iLd][0] == SCHED_SENTINEL);

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
          const auto LoadReq = LoadReqPipes::template PipeAt<iLd>::read(succ);
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
              if (NextLoadAddr[iLd] == StoreButrstBuffAddr[iSt][i]) {
                ThisReuse[iSt] = true;
                ThisReuseVal[iSt] = StoreBurstBuffVal[iSt][i];
              }
            }
          });

          // If no hazards against any store, then move load to "safe" stage.
          if (NoRAW) {
            bool Reuse = false;
            T ReuseVal = T{};
            UnrolledLoop<NUM_STORES>([&](auto iSt) {
              // There can be onyl one hit bacause stores invalidate store
              // commit queue entries of other stores.
              if (ThisReuse[iSt]) {
                Reuse = true;
                ReuseVal = ThisReuseVal[iSt];
              }
            });

            bool succ = false;
            ack_t<LOOP_DEPTH> MuxAck{NextLoadAddr[iLd]};
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              MuxAck.sched[iD] = NextLoadSched[iLd][iD];
              MuxAck.isMaxIter[iD] = NextLoadIsMaxIter[iLd][iD];
            });
            ld_port_req_t<LOOP_DEPTH, T> Req = {MuxAck, ReuseVal, Reuse};
            LoadPortReqPipes::template PipeAt<iLd>::write(Req, succ);

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
        // This store is done if we receive an ACK with a sentinel value.
        StoreDone[iSt] = (StoreAckSched[iSt][0] == SCHED_SENTINEL);

        StoreNoOutstandingAcks[iSt] = (LastSentStoreSched[iSt] ==
                                       StoreAckSched[iSt][DI.STORE_LOOP_DEPTH[iSt]]);

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
        if (!StoreValid[iSt][0]) {
          if (StoreValid[iSt][1]) {
            NextStoreAddr[iSt] = StoreAddr[iSt][1];
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              NextStoreSched[iSt][iD] = StoreSched[iSt][iD][1];
              NextStoreIsMaxIter[iSt][iD] = StoreIsMaxIter[iSt][iD][1];
            });
          }

          ShiftBundle(StoreValid[iSt], false);
          ShiftBundle(StoreAddr[iSt], 0u);
          UnrolledLoop<LOOP_DEPTH>([&] (auto iD) {
            ShiftBundle(StoreSched[iSt][iD], 0u);
            ShiftBundle(StoreIsMaxIter[iSt][iD], false);
          });
        }
        /** End Rule */

        /** Rule for reading store req. */
        if (!StoreValid[iSt][ST_ALLOC_Q_SIZE - 1]) {
          auto StoreReq = StoreReqPipes::template PipeAt<iSt, 1>::read(
              StoreValid[iSt][ST_ALLOC_Q_SIZE - 1]);
          StoreAddr[iSt][ST_ALLOC_Q_SIZE - 1] = StoreReq.addr;
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            StoreSched[iSt][iD][ST_ALLOC_Q_SIZE - 1] = StoreReq.sched[iD];
            StoreIsMaxIter[iSt][iD][ST_ALLOC_Q_SIZE - 1] = StoreReq.isMaxIter[iD];
          });
        }
        /** End Rule */

        /** Rule for reading store value. */
        if (!NextStoreValueValid[iSt]) {
          NextStoreValue[iSt] = StoreValPipes::template PipeAt<iSt>::read(
            NextStoreValueValid[iSt]);
        }
        /** End Rule */

        /** Rule for checking the safety of the next store. */
        bool NoWAW = true;
        UnrolledLoop<NUM_STORES>([&](auto iStOther) {
          if constexpr (iSt != iStOther) {
            NoWAW &= checkNoWAW(iSt, iStOther);
          }
        });
        bool NoWAR = true;
        UnrolledLoop<NUM_LOADS>([&](auto iLd) {
          if constexpr (!DI.LOAD_STORE_IN_SAME_THREAD[iLd][iSt]) {
            NoWAR &= checkNoWAR(iLd, iSt);
          }
        });
        const bool IsSafe = (NoWAW && NoWAR);
        /** End Rule */

        /** Rule for moving st alloc to st commit queue. */
        if (StoreValid[iSt][0] && NextStoreValueValid[iSt] && IsSafe) {
          bool succ = false;
          StorePortValPipes::template PipeAt<iSt>::write(NextStoreValue[iSt], succ);

          if (succ) {
            LastSentStoreSched[iSt] = NextStoreSched[iSt][DI.STORE_LOOP_DEPTH[iSt]];

            ShiftBundle(StoreBurstBuffVal[iSt], NextStoreValue[iSt]);
            ShiftBundle(StoreButrstBuffAddr[iSt], NextStoreAddr[iSt]);

            // Check if this store overrides another store in its burst buffer.
            UnrolledLoop<NUM_STORES>([&](auto iStOther) {
              if constexpr (iStOther != iSt) {
                #pragma unroll
                for (int i = 0; i < ST_COMMIT_Q_SIZE; ++i) {
                  if (NextStoreAddr[iSt] == StoreButrstBuffAddr[iStOther][i]) {
                    // Invalid address, i.e. a load will never requiest this.
                    StoreButrstBuffAddr[iStOther][i] = STORE_ADDR_SENTINEL;
                  }
                }
              }
            });

            StoreValid[iSt][0] = false;
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

#pragma clang attribute pop

#endif





















#include "device_print.hpp"
#include "exception_handler.hpp"
#include "memory_utils.hpp"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/sycl.hpp>
#include <vector>
using namespace sycl;
class MainKernel;
constexpr float d = 0.85;
double page_rank_kernel(queue &q, const std::vector<int> &h_row_ptr,
                        const std::vector<int> &h_col_idx,
                        std::vector<float> &h_val, std::vector<float> &h_p,
                        const int numNodes, const int maxIters) {
  float *p_new = fpga_tools::toDevice(h_p, q);
  float *p = fpga_tools::toDevice(h_p, q);
  const int *row_ptr = fpga_tools::toDevice(h_row_ptr, q);
  const int *col_idx = fpga_tools::toDevice(h_col_idx, q);
  const float *val = fpga_tools::toDevice(h_val, q);
  using LoadReqPipes_100 =
      PipeArray<class LoadReqPipes_100_, ld_req_t<3, 3>, 16, 3>;
  using LoadValPipes_100 = PipeArray<class LoadValPipes_100_, float, 16, 3>;
  using StoreReqPipes_100 =
      PipeArray<class StoreReqPipes_100_, st_req_t<3>, 16, 3, 2>;
  using StoreValPipes_100 = PipeArray<class StoreValPipes_100_, float, 16, 3>;
  using LoadReqPipes_101 =
      PipeArray<class LoadReqPipes_101_, ld_req_t<1, 2>, 16, 2>;
  using LoadValPipes_101 = PipeArray<class LoadValPipes_101_, float, 16, 2>;
  using StoreReqPipes_101 =
      PipeArray<class StoreReqPipes_101_, st_req_t<2>, 16, 1, 2>;
  using StoreValPipes_101 = PipeArray<class StoreValPipes_101_, float, 16, 1>;
  auto event_MainKernel_agu =
      q.single_task<MainKernel_agu>([=]() [[intel::kernel_args_restrict]] {

        st_req_t<3> st_req_100_0;
        st_req_100_0.addr = 0u;
        st_req_100_0.sched[0] = 0u;
        st_req_100_0.isMaxIter[0] = false;
        st_req_100_0.sched[1] = 0u;
        st_req_100_0.isMaxIter[1] = false;
        st_req_100_0.sched[2] = 0u;
        st_req_100_0.isMaxIter[2] = false;

        for (uint iter = 0; iter < maxIters; ++iter) {
          st_req_100_0.sched[0]++;
          st_req_100_0.isMaxIter[0] = (iter + 1) == maxIters;

          for (uint i = 0; i < numNodes; ++i) {
            st_req_100_0.sched[1]++;
            st_req_100_0.isMaxIter[1] = (i + 1) == numNodes;

            st_req_100_0.addr = i;
            StoreReqPipes_100::PipeAt<0, 0>::write(st_req_100_0);
            StoreReqPipes_100::PipeAt<0, 1>::write(st_req_100_0);
          }
        }

        st_req_100_0.addr = STORE_ADDR_SENTINEL;
        st_req_100_0.sched[0] = SCHED_SENTINEL;
        st_req_100_0.isMaxIter[0] = true;
        st_req_100_0.sched[1] = SCHED_SENTINEL;
        st_req_100_0.isMaxIter[1] = true;
        StoreReqPipes_100::PipeAt<0, 0>::write(st_req_100_0);
        StoreReqPipes_100::PipeAt<0, 1>::write(st_req_100_0);
      });

  auto event_MainKernel_1_agu =
      q.single_task<MainKernel_1_agu>([=]() [[intel::kernel_args_restrict]] {
        ld_req_t<1, 2> ld_req_101_0;
        ld_req_101_0.addr = 0u;
        ld_req_101_0.posDepDist[0] = false;
        ld_req_101_0.sched[0] = 0u;
        ld_req_101_0.isMaxIter[0] = false;
        ld_req_101_0.sched[1] = 0u;
        ld_req_101_0.isMaxIter[1] = false;
        ld_req_t<3, 3> ld_req_100_0;
        ld_req_100_0.addr = 0u;
        ld_req_100_0.posDepDist[0] = false;
        ld_req_100_0.posDepDist[1] = false;
        ld_req_100_0.posDepDist[2] = false;
        ld_req_100_0.sched[0] = 0u;
        ld_req_100_0.isMaxIter[0] = false;
        ld_req_100_0.sched[1] = 0u;
        ld_req_100_0.isMaxIter[1] = false;
        ld_req_100_0.sched[2] = 0u;
        ld_req_100_0.isMaxIter[2] = false;
        st_req_t<3> st_req_100_1;
        st_req_100_1.addr = 0u;
        st_req_100_1.sched[0] = 0u;
        st_req_100_1.isMaxIter[0] = false;
        st_req_100_1.sched[1] = 0u;
        st_req_100_1.isMaxIter[1] = false;
        st_req_100_1.sched[2] = 0u;
        st_req_100_1.isMaxIter[2] = false;

        for (uint iter = 0; iter < maxIters; ++iter) {
          ld_req_101_0.sched[0]++;
          ld_req_101_0.isMaxIter[0] = (iter + 1) == maxIters;
          ld_req_100_0.sched[0]++;
          ld_req_100_0.isMaxIter[0] = (iter + 1) == maxIters;
          st_req_100_1.sched[0]++;
          st_req_100_1.isMaxIter[0] = (iter + 1) == maxIters;

          int curcol = 0;

          for (uint i = 0; i < numNodes; i += 2) {
            ld_req_101_0.sched[1]++;
            ld_req_101_0.isMaxIter[1] = (i + 1) == numNodes;
            ld_req_100_0.sched[1]++;
            ld_req_100_0.isMaxIter[1] = (i + 1) == numNodes;
            st_req_100_1.sched[1]++;
            st_req_100_1.isMaxIter[1] = (i + 1) == numNodes;

            ld_req_101_0.addr = i;

            LoadReqPipes_101::PipeAt<0>::write(ld_req_101_0);

            int rowel = row_ptr[i + 1] - row_ptr[i];
            int local_curcol = curcol;
            curcol += rowel;
            for (int j = 0; j < rowel; j++) {
              ld_req_100_0.sched[2]++;
              ld_req_100_0.isMaxIter[2] = (j + 1) == rowel;
              st_req_100_1.sched[2]++;
              st_req_100_1.isMaxIter[2] = (j + 1) == rowel;

              ld_req_100_0.addr = col_idx[local_curcol];
              ld_req_100_0.posDepDist[1] = ld_req_100_0.addr > st_req_100_1.addr;

              st_req_100_1.addr = col_idx[local_curcol];

              LoadReqPipes_100::PipeAt<0>::write(ld_req_100_0);
              StoreReqPipes_100::PipeAt<1, 0>::write(st_req_100_1);
              StoreReqPipes_100::PipeAt<1, 1>::write(st_req_100_1);
              local_curcol++;
            }
            rowel = row_ptr[i + 1 + 1] - row_ptr[i + 1];
            curcol += rowel;
          }
        }

        ld_req_101_0.addr = LOAD_ADDR_SENTINEL;
        ld_req_101_0.posDepDist[0] = true;
        ld_req_101_0.sched[0] = SCHED_SENTINEL;
        ld_req_101_0.isMaxIter[0] = true;
        ld_req_101_0.sched[1] = SCHED_SENTINEL;
        ld_req_101_0.isMaxIter[1] = true;

        ld_req_100_0.addr = LOAD_ADDR_SENTINEL;
        ld_req_100_0.posDepDist[0] = true;
        ld_req_100_0.posDepDist[1] = true;
        ld_req_100_0.posDepDist[2] = true;
        ld_req_100_0.sched[0] = SCHED_SENTINEL;
        ld_req_100_0.isMaxIter[0] = true;
        ld_req_100_0.sched[1] = SCHED_SENTINEL;
        ld_req_100_0.isMaxIter[1] = true;
        ld_req_100_0.sched[2] = SCHED_SENTINEL;
        ld_req_100_0.isMaxIter[2] = true;

        st_req_100_1.addr = STORE_ADDR_SENTINEL;
        st_req_100_1.sched[0] = SCHED_SENTINEL;
        st_req_100_1.isMaxIter[0] = true;
        st_req_100_1.sched[1] = SCHED_SENTINEL;
        st_req_100_1.isMaxIter[1] = true;
        st_req_100_1.sched[2] = SCHED_SENTINEL;
        st_req_100_1.isMaxIter[2] = true;

        LoadReqPipes_101::PipeAt<0>::write(ld_req_101_0);
        LoadReqPipes_100::PipeAt<0>::write(ld_req_100_0);
        StoreReqPipes_100::PipeAt<1, 0>::write(st_req_100_1);
        StoreReqPipes_100::PipeAt<1, 1>::write(st_req_100_1);
      });

  auto event_MainKernel_2_agu =
      q.single_task<MainKernel_2_agu>([=]() [[intel::kernel_args_restrict]] {
        ld_req_t<1, 2> ld_req_101_1;
        ld_req_101_1.addr = 0u;
        ld_req_101_1.posDepDist[0] = false;
        ld_req_101_1.sched[0] = 0u;
        ld_req_101_1.isMaxIter[0] = false;
        ld_req_101_1.sched[1] = 0u;
        ld_req_101_1.isMaxIter[1] = false;
        ld_req_t<3, 3> ld_req_100_1;
        ld_req_100_1.addr = 0u;
        ld_req_100_1.posDepDist[0] = false;
        ld_req_100_1.posDepDist[1] = false;
        ld_req_100_1.posDepDist[2] = false;
        ld_req_100_1.sched[0] = 0u;
        ld_req_100_1.isMaxIter[0] = false;
        ld_req_100_1.sched[1] = 0u;
        ld_req_100_1.isMaxIter[1] = false;
        ld_req_100_1.sched[2] = 0u;
        ld_req_100_1.isMaxIter[2] = false;
        st_req_t<3> st_req_100_2;
        st_req_100_2.addr = 0u;
        st_req_100_2.sched[0] = 0u;
        st_req_100_2.isMaxIter[0] = false;
        st_req_100_2.sched[1] = 0u;
        st_req_100_2.isMaxIter[1] = false;
        st_req_100_2.sched[2] = 0u;
        st_req_100_2.isMaxIter[2] = false;

        for (uint iter = 0; iter < maxIters; ++iter) {
          ld_req_101_1.sched[0]++;
          ld_req_101_1.isMaxIter[0] = (iter + 1) == maxIters;
          ld_req_100_1.sched[0]++;
          ld_req_100_1.isMaxIter[0] = (iter + 1) == maxIters;
          st_req_100_2.sched[0]++;
          st_req_100_2.isMaxIter[0] = (iter + 1) == maxIters;

          int curcol = 0;

          for (uint i = 0; i < numNodes; i += 2) {
            ld_req_101_1.sched[1]++;
            ld_req_101_1.isMaxIter[1] = (i + 2) == numNodes;
            ld_req_100_1.sched[1]++;
            ld_req_100_1.isMaxIter[1] = (i + 2) == numNodes;
            st_req_100_2.sched[1]++;
            st_req_100_2.isMaxIter[1] = (i + 2) == numNodes;

            ld_req_101_1.addr = i + 1;

            LoadReqPipes_101::PipeAt<1>::write(ld_req_101_1);

            int rowel = row_ptr[i + 1] - row_ptr[i];
            curcol += rowel;
            rowel = row_ptr[i + 1 + 1] - row_ptr[i + 1];

            int local_curcol = curcol;
            curcol += rowel;
            for (int j = 0; j < rowel; j++) {
              ld_req_100_1.sched[2]++;
              ld_req_100_1.isMaxIter[2] = (j + 1) == rowel;
              st_req_100_2.sched[2]++;
              st_req_100_2.isMaxIter[2] = (j + 1) == rowel;

              ld_req_100_1.addr = col_idx[local_curcol];
              ld_req_100_1.posDepDist[2] = ld_req_100_1.addr > st_req_100_2.addr;

              st_req_100_2.addr = col_idx[local_curcol];

              LoadReqPipes_100::PipeAt<1>::write(ld_req_100_1);
              StoreReqPipes_100::PipeAt<2, 0>::write(st_req_100_2);
              StoreReqPipes_100::PipeAt<2, 1>::write(st_req_100_2);
              local_curcol++;
            }
          }
        }

        ld_req_101_1.addr = LOAD_ADDR_SENTINEL;
        ld_req_101_1.posDepDist[0] = true;
        ld_req_101_1.sched[0] = SCHED_SENTINEL;
        ld_req_101_1.isMaxIter[0] = true;
        ld_req_101_1.sched[1] = SCHED_SENTINEL;
        ld_req_101_1.isMaxIter[1] = true;

        ld_req_100_1.addr = LOAD_ADDR_SENTINEL;
        ld_req_100_1.posDepDist[0] = true;
        ld_req_100_1.posDepDist[1] = true;
        ld_req_100_1.posDepDist[2] = true;
        ld_req_100_1.sched[0] = SCHED_SENTINEL;
        ld_req_100_1.isMaxIter[0] = true;
        ld_req_100_1.sched[1] = SCHED_SENTINEL;
        ld_req_100_1.isMaxIter[1] = true;
        ld_req_100_1.sched[2] = SCHED_SENTINEL;
        ld_req_100_1.isMaxIter[2] = true;

        st_req_100_2.addr = STORE_ADDR_SENTINEL;
        st_req_100_2.sched[0] = SCHED_SENTINEL;
        st_req_100_2.isMaxIter[0] = true;
        st_req_100_2.sched[1] = SCHED_SENTINEL;
        st_req_100_2.isMaxIter[1] = true;
        st_req_100_2.sched[2] = SCHED_SENTINEL;
        st_req_100_2.isMaxIter[2] = true;

        LoadReqPipes_101::PipeAt<1>::write(ld_req_101_1);
        LoadReqPipes_100::PipeAt<1>::write(ld_req_100_1);
        StoreReqPipes_100::PipeAt<2, 0>::write(st_req_100_2);
        StoreReqPipes_100::PipeAt<2, 1>::write(st_req_100_2);
      });

  auto event_MainKernel_3_agu =
      q.single_task<MainKernel_3_agu>([=]() [[intel::kernel_args_restrict]] {
        ld_req_t<3, 3> ld_req_100_2;
        ld_req_100_2.addr = 0u;
        ld_req_100_2.posDepDist[0] = false;
        ld_req_100_2.posDepDist[1] = false;
        ld_req_100_2.sched[0] = 0u;
        ld_req_100_2.isMaxIter[0] = false;
        ld_req_100_2.sched[1] = 0u;
        ld_req_100_2.isMaxIter[1] = false;
        ld_req_100_2.sched[2] = 0u;
        ld_req_100_2.isMaxIter[2] = false;
        st_req_t<2> st_req_101_0;
        st_req_101_0.addr = 0u;
        st_req_101_0.sched[0] = 0u;
        st_req_101_0.isMaxIter[0] = false;
        st_req_101_0.sched[1] = 0u;
        st_req_101_0.isMaxIter[1] = false;
        for (uint iter = 0; iter < maxIters; ++iter) {
          ld_req_100_2.sched[0]++;
          ld_req_100_2.isMaxIter[0] = (iter + 1) == maxIters;
          st_req_101_0.sched[0]++;
          st_req_101_0.isMaxIter[0] = (iter + 1) == maxIters;
          for (uint i = 0; i < numNodes; i++) {
            ld_req_100_2.sched[1]++;
            ld_req_100_2.isMaxIter[1] = (i+1) == numNodes;
            st_req_101_0.sched[1]++;
            st_req_101_0.isMaxIter[1] = (i+1) == numNodes;

            ld_req_100_2.addr = i;
            st_req_101_0.addr = i;

            LoadReqPipes_100::PipeAt<2>::write(ld_req_100_2);
            StoreReqPipes_101::PipeAt<0, 0>::write(st_req_101_0);
            StoreReqPipes_101::PipeAt<0, 1>::write(st_req_101_0);
          }
        }

        ld_req_100_2.addr = LOAD_ADDR_SENTINEL;
        ld_req_100_2.posDepDist[0] = true;
        ld_req_100_2.posDepDist[1] = true;
        ld_req_100_2.posDepDist[2] = true;
        ld_req_100_2.sched[0] = SCHED_SENTINEL;
        ld_req_100_2.isMaxIter[0] = true;
        ld_req_100_2.sched[1] = SCHED_SENTINEL;
        ld_req_100_2.isMaxIter[1] = true;
        ld_req_100_2.sched[2] = SCHED_SENTINEL;
        ld_req_100_2.isMaxIter[2] = true;

        st_req_101_0.addr = STORE_ADDR_SENTINEL;
        st_req_101_0.sched[0] = SCHED_SENTINEL;
        st_req_101_0.isMaxIter[0] = true;
        st_req_101_0.sched[1] = SCHED_SENTINEL;
        st_req_101_0.isMaxIter[1] = true;

        LoadReqPipes_100::PipeAt<2>::write(ld_req_100_2);
        StoreReqPipes_101::PipeAt<0, 0>::write(st_req_101_0);
        StoreReqPipes_101::PipeAt<0, 1>::write(st_req_101_0);
      });




  auto event_MainKernel_1 =
      q.single_task<MainKernel_1>([=]() [[intel::kernel_args_restrict]] {
        for (uint iter = 0; iter < maxIters; ++iter) {
          int curcol = 0;
          for (uint i = 0; i < numNodes; i += 2) {
            auto p_val = LoadValPipes_101::PipeAt<0>::read();

            int rowel = row_ptr[i + 1] - row_ptr[i];
            int local_curcol = curcol;

            curcol += rowel;
            for (int j = 0; j < rowel; j++) {
              auto p_new_load = LoadValPipes_100::PipeAt<0>::read();
              auto p_new_store = p_new_load + (val[local_curcol] * p_val);
              StoreValPipes_100::PipeAt<1>::write(p_new_store);
              local_curcol++;
            }
            rowel = row_ptr[i + 1 + 1] - row_ptr[i + 1];
            curcol += rowel;
          }
        }
      });

  auto event_MainKernel_2 =
      q.single_task<MainKernel_2>([=]() [[intel::kernel_args_restrict]] {
        for (uint iter = 0; iter < maxIters; ++iter) {
          int curcol = 0;
          for (uint i = 0; i < numNodes; i += 2) {
            auto p_val = LoadValPipes_101::PipeAt<1>::read();

            int rowel = row_ptr[i + 1] - row_ptr[i];
            curcol += rowel;

            rowel = row_ptr[i + 1 + 1] - row_ptr[i + 1];
            int local_curcol = curcol;

            curcol += rowel;
            for (int j = 0; j < rowel; j++) {
              auto p_new_load = LoadValPipes_100::PipeAt<1>::read();
              auto p_new_store = p_new_load + (val[local_curcol] * p_val);
              StoreValPipes_100::PipeAt<2>::write(p_new_store);
              local_curcol++;
            }
          }
        }
      });

  auto event_MainKernel_3 =
      q.single_task<MainKernel_3>([=]() [[intel::kernel_args_restrict]] {
        for (uint iter = 0; iter < maxIters; ++iter) {
          for (uint i = 0; i < numNodes; i++) {
            auto p_val =
                d * LoadValPipes_100::PipeAt<2>::read() + (1.0f - d) / numNodes;
            StoreValPipes_101::PipeAt<0>::write(p_val);
          }
        }
      });

  auto memEvents_100 = StreamingMemory<100, LoadReqPipes_100, LoadValPipes_100,
                                       StoreReqPipes_100, StoreValPipes_100>(q, p_new);
  auto memEvents_101 = StreamingMemory<101, LoadReqPipes_101, LoadValPipes_101,
                                       StoreReqPipes_101, StoreValPipes_101>(q, p);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (uint iter = 0; iter < maxIters; ++iter) {
      for (uint i = 0; i < numNodes; i++) {
        StoreValPipes_100::PipeAt<0>::write(0.0f);
      }
    }
  });

  for (auto &e : memEvents_100)
    e.wait();
  for (auto &e : memEvents_101)
    e.wait();
  event_MainKernel_agu.wait();

  event_MainKernel_1_agu.wait();

  event_MainKernel_2_agu.wait();

  event_MainKernel_3_agu.wait();

  event_MainKernel_1.wait();

  event_MainKernel_2.wait();

  event_MainKernel_3.wait();

  event.wait();
  q.copy(p, h_p.data(), h_p.size()).wait();
  sycl::free((void *)row_ptr, q);
  sycl::free((void *)col_idx, q);
  sycl::free((void *)val, q);
  sycl::free(p, q);
  sycl::free(p_new, q);
  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;
  return time_in_ms;
}
void page_rank_cpu(const std::vector<int> &row_ptr,
                   const std::vector<int> &col_idx, std::vector<float> &val,
                   std::vector<float> &p, const int numNodes,
                   const int maxIters) {
  std::vector<float> p_new(p.size());
  for (int iter = 0; iter < maxIters; ++iter) {
    for (int i = 0; i < numNodes; i++) {
      p_new[i] = 0.0f;
    }
    int rowel = 0;
    int curcol = 0;
    for (int i = 0; i < numNodes; i++) {
      rowel = row_ptr[i + 1] - row_ptr[i];
      for (int j = 0; j < rowel; j++) {
        p_new[col_idx[curcol]] += val[curcol] * p[i];
        curcol++;
      }
    }
# 133 "/tmp/page_rank_dram_2xunroll_3773_elastic_pass_workdir/page_rank_dram_2xunroll.cpp"
    for (int i = 0; i < numNodes; i++) {
      p[i] = d * p_new[i] + (1.0f - d) / numNodes;
    }
  }
}
inline bool almost_equal(const float x, const float y) {
  const float ulpFloat = static_cast<float>(2);
  const float tolerance = 0.01f;
  return (fabsf(x - y) <= tolerance * fabsf(x + y) * ulpFloat) ||
         (fabsf(x - y) < std::numeric_limits<float>::min()) ||
         (std::isinf(x) && std::isinf(y)) || (std::isnan(x) && std::isnan(y));
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
  int numNodes = 0, numEdges = 0;
  char ch;
  char str[100];
  ch = getc(fp);
  while (ch == '#') {
    fgets(str, 100 - 1, fp);
    sscanf(str, "%*s %d %*s %d", &numNodes, &numEdges);
    ch = getc(fp);
  }
  ungetc(ch, fp);
  printf("\nGraph data: Nodes: %d, Edges: %d \n\n", numNodes, numEdges);
  auto sizeArr = std::max(numEdges + 1, numNodes + 1);
  std::vector<int> row_ptr(sizeArr, 0), col_idx(sizeArr, 0);
  std::vector<float> val(sizeArr, 0.0f), p(sizeArr, 0.0f);
  int fromnode, tonode;
  int cur_row = 0;
  int i = 0;
  int elrow = 0;
  int curel = 0;
  while (!feof(fp)) {
    fscanf(fp, "%d%d", &tonode, &fromnode);
    if (tonode > cur_row) {
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
  row_ptr[cur_row + 1] = curel + elrow - 1;
#if FPGA_SIM
  auto d_selector = sycl::ext::intel::fpga_simulator_selector_v;
#elif FPGA_HW
  auto d_selector = sycl::ext::intel::fpga_selector_v;
#else
  auto d_selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif
  try {
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);
    std::vector<float> p_cpu(sizeArr, 0.0f);
    std::copy(p.begin(), p.end(), p_cpu.begin());
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";
    auto kernel_time =
        page_rank_kernel(q, row_ptr, col_idx, val, p, numNodes, maxIters);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";
    page_rank_cpu(row_ptr, col_idx, val, p_cpu, numNodes, maxIters);
    if (std::equal(p.begin(), p.end(), p_cpu.begin(), almost_equal)) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failed\n";
      for (int i = 0; i < numNodes; ++i) {
        if (!almost_equal(p[i], p_cpu[i])) {
          std::cout << i << ": fpga != cpu, " << p[i] << " != " << p_cpu[i]
                    << "\n";
        }
      }
    }
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }
  return 0;
}