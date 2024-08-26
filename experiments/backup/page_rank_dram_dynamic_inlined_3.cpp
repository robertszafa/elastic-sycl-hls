enum DEP_DIR {
  BACK, // First load, then store in a loop.
  FORWARD, // First store, then load in a loop.
};

template <int MEM_ID> struct DepInfo;

// gemm
template <> struct DepInfo<0> {
  static constexpr int NUM_LOADS = 2;
  static constexpr int NUM_STORES = 2;
  static constexpr int LOOP_DEPTH = 3;

  static constexpr int COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {{1, 0},
                                                                   {0, 2}};
  static constexpr int COMMON_STORE_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {
      {1, 0}, {0, 2}};
  static constexpr DEP_DIR LOAD_TO_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {BACK, BACK}, {FORWARD, BACK}};
  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {1, 2};
  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {1, 2};

  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][LOOP_DEPTH] = {
      {false, false, false},
      {false, true, false},
  };
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][LOOP_DEPTH] = {
      {false, false, false},
      {false, true, false},
  };
  static constexpr bool ARE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {
      {true, false},
      {false, true},
  };
};

// test_fusion_war2
template <> struct DepInfo<1> {
  static constexpr int NUM_LOADS = 1;
  static constexpr int NUM_STORES = 1;
  static constexpr int LOOP_DEPTH = 3;

  static constexpr int COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {{0}};
  static constexpr int COMMON_STORE_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {{1}};
  static constexpr DEP_DIR LOAD_TO_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {BACK}};
  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {2};
  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {1};

  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][LOOP_DEPTH] = {
      {false, true, false}};
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][LOOP_DEPTH] = {
      {true, false}};
  static constexpr bool ARE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {{false}};
};

// test_fusion_waw2
template <> struct DepInfo<2> {
  static constexpr int NUM_LOADS = 1;
  static constexpr int NUM_STORES = 3;
  static constexpr int LOOP_DEPTH = 3;

  static constexpr int COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {{0, 0, 1}};
  static constexpr int COMMON_STORE_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {
      {2, 0, 0},
      {0, 1, 0},
      {0, 0, 1},
  };
  static constexpr DEP_DIR LOAD_TO_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {FORWARD, FORWARD, BACK}};
  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {1};
  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {2, 1, 1};

  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][LOOP_DEPTH] = {
      {true, false, false}};
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][LOOP_DEPTH] = {
      {true, true, false}, {true, false, false}, {true, false, false}};
  static constexpr bool ARE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {
      {false, false, true},
  };
};

// 2mm
template <> struct DepInfo<3> {
  static constexpr int NUM_LOADS = 2;
  static constexpr int NUM_STORES = 1;
  static constexpr int LOOP_DEPTH = 3;

  static constexpr int COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {{1}, {-1}};
  static constexpr int COMMON_STORE_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {{1}};
  static constexpr DEP_DIR LOAD_TO_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {BACK}, {FORWARD}};
  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {1, 2};
  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {1};

  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][LOOP_DEPTH] = {
      {false, false, false}, {false, true, false}};
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][LOOP_DEPTH] = {
      {false, false, false}};
  static constexpr bool ARE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {
      {true},
      {false},
  };
};


// page rank p_new
template <> struct DepInfo<4> {
  static constexpr int NUM_LOADS = 2;
  static constexpr int NUM_STORES = 2;
  static constexpr int LOOP_DEPTH = 3;

  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {2, 1};
  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {1, 2};
  static constexpr int COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {{0, 2},
                                                                   {0, 0}};
  static constexpr int COMMON_STORE_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {
      {1, 0}, {0, 2}};
  static constexpr DEP_DIR LOAD_TO_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {FORWARD, BACK}, {FORWARD, FORWARD}};

  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][LOOP_DEPTH] = {
      {true, true, false},
      {true, false, false}
  };
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][LOOP_DEPTH] = {
      {true, false, false},
      {true, true, false}
  };
  static constexpr bool ARE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {
      {false, true},
      {false, false}
  };
};

// page rank p
template <> struct DepInfo<5> {
  static constexpr int NUM_LOADS = 1;
  static constexpr int NUM_STORES = 1;
  static constexpr int LOOP_DEPTH = 2;

  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {1};
  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {1};
  static constexpr int COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {{0}};
  static constexpr int COMMON_STORE_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {{1}};

  static constexpr DEP_DIR LOAD_TO_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {BACK}};

  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][LOOP_DEPTH] = {
      {true, false},
  };
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][LOOP_DEPTH] = {
      {true, false},
  };
  static constexpr bool ARE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {{false}};
};

// lud
template <> struct DepInfo<6> {
  static constexpr int NUM_LOADS = 7;
  static constexpr int NUM_STORES = 2;
  static constexpr int LOOP_DEPTH = 3;

  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {1, 2, 2, 1, 1, 2, 2};
  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {1, 1};
  static constexpr int COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {
      {1, 0}, {1, 0}, {1, 0}, {1, 0}, {0, 1}, {0, 1}, {0, 1}};
  static constexpr int COMMON_STORE_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {
      {1, 0},
      {0, 1},
  };
  static constexpr DEP_DIR LOAD_TO_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {BACK, BACK},
      {BACK, BACK},
      {BACK, BACK},
      {BACK, BACK},
      {FORWARD, BACK},
      {FORWARD, BACK},
      {FORWARD, BACK}
  };
  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][LOOP_DEPTH] = {
      {false, false, false}, {false, true, false}, {true, true, false},
      {true, false, false},

      {false, false, false}, {false, true, false}, {true, true, false},
  };
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][LOOP_DEPTH] = {
      {false, false, false},
      {false, false, false},
  };
  static constexpr bool ARE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {
      {true, false}, {false, false}, {false, false}, {true, false},

      {false, true}, {false, false}, {false, false},
  };
};

// bnn
template <> struct DepInfo<8> {
  static constexpr int NUM_LOADS = 2;
  static constexpr int NUM_STORES = 2;
  static constexpr int LOOP_DEPTH = 2;

  static constexpr int LOAD_LOOP_DEPTH[NUM_LOADS] = {1, 0};
  static constexpr int STORE_LOOP_DEPTH[NUM_STORES] = {1, 0};
  static constexpr int COMMON_LOOP_DEPTH[NUM_LOADS][NUM_STORES] = {{1, -1},
                                                                   {-1, 0}};
  static constexpr int COMMON_STORE_LOOP_DEPTH[NUM_STORES][NUM_STORES] = {
      {1, -1}, {-1, 0}};
  static constexpr DEP_DIR LOAD_TO_STORE_DEP_DIR[NUM_LOADS][NUM_STORES] = {
      {BACK, BACK}, {FORWARD, BACK}};

  static constexpr bool LOAD_IS_MAX_ITER_NEEDED[NUM_LOADS][LOOP_DEPTH] = {
      {true, false},
      {false}
  };
  static constexpr bool STORE_IS_MAX_ITER_NEEDED[NUM_STORES][LOOP_DEPTH] = {
      {true, false},
      {false}
  };
  static constexpr bool ARE_IN_SAME_LOOP[NUM_LOADS][NUM_STORES] = {
      {true, false},
      {false, true}
  };
};













#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/ext/intel/ac_types/ac_int.hpp>
#include <sycl/sycl.hpp>

#include "device_print.hpp"
#include "pipe_utils.hpp"
#include "unrolled_loop.hpp"
#include "data_bundle.hpp"

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

// We use a dynamic burst load-store unit supplied by the intel HLS compiler.
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
template <int PortId> class StorePortKernel;
template <int PortId> class LoadPortKernel;

template <int MEM_ID, typename LoadReqPipes, typename LoadValPipes,
          typename StoreReqPipes, typename StoreValPipes,
          uint NUM_LOADS, uint NUM_STORES, uint LOOP_DEPTH, typename T>
std::vector<event> StreamingMemory(queue &q, T *data) {
  std::vector<event> events(NUM_STORES + 1);

  // Sizes of internal buffers.
  constexpr uint DRAM_BURST_BYTES = 64; /// 512-bit DRAM interface
  constexpr uint T_BYTES = sizeof(T);
  constexpr uint BURST_SIZE = ((DRAM_BURST_BYTES + T_BYTES - 1) / T_BYTES) ;
  constexpr uint ST_COMMIT_Q_SIZE = BURST_SIZE + 4;
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
      ack_t<LOOP_DEPTH> NextAck {0u};
      InitBundle(NextAck.sched, 0u);
      InitBundle(NextAck.isMaxIter, false);

      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        StoreAckPipes::template PipeAt<iSt>::write(NextAck);

        const auto Req = StoreReqPipes::template PipeAt<iSt, 0>::read();
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
      // PRINTF("MEM%d ld%d reused %d/%d\n", MEM_ID int(iLd), NumReused, NumTotal);
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

      StoreAckAddr[iSt] = 0u;
      InitBundle(StoreAckSched[iSt], 0u);
      InitBundle(StoreAckIsMaxIter[iSt], false);

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
      constexpr int cmnLoopDepth = DI.COMMON_LOOP_DEPTH[iLd][iSt];
      constexpr int stLoopDepth = DI.STORE_LOOP_DEPTH[iSt];

      bool StoreSchedGreater = (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == BACK);
      if constexpr (cmnLoopDepth >= 0) {
        if constexpr (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == BACK) {
          StoreSchedGreater = (NextStoreSched[iSt][cmnLoopDepth] >=
                               NextLoadSched[iLd][cmnLoopDepth]);
        } else {
          StoreSchedGreater = (NextStoreSched[iSt][cmnLoopDepth] >
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

        LoadHasPosDepDistance = CanCheckPosDepDist && NextLoadPosDepDist[iLd][iSt];
      } else {
        UnrolledLoop<stLoopDepth, std::max(cmnLoopDepth, 0)>([&](auto iD) {
          if constexpr (DI.STORE_IS_MAX_ITER_NEEDED[iSt][iD]) {
            StAddrNoDecrement &= NextStoreIsMaxIter[iSt][iD];
          }
        });
      }

      bool StoreWillNotTouchLoadAddr =
          (StAddrNoDecrement && NextStoreAddr[iSt] > NextLoadAddr[iLd]);

      return StoreSchedGreater || LoadHasPosDepDistance ||
             StoreWillNotTouchLoadAddr;
    };

    auto checkNoWAR = [&](const auto iLd, const auto iSt) {
      constexpr int cmnLoopDepth = DI.COMMON_LOOP_DEPTH[iLd][iSt];
      constexpr int loadLoopDepth = DI.LOAD_LOOP_DEPTH[iLd];

      bool AckLoadSchedGreater = (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == FORWARD);
      bool NextLoadSchedGreater = (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == FORWARD);
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
          AckLoadSchedGreater = (LoadAckSched[iLd][cmnLoopDepth] >=
                                 NextStoreSched[iSt][cmnLoopDepth]);
          NextLoadSchedGreater = (NextLoadSched[iLd][cmnLoopDepth] >=
                                  NextStoreSched[iSt][cmnLoopDepth]);
        } else {
          AckLoadSchedGreater = (LoadAckSched[iLd][cmnLoopDepth] >
                                 NextStoreSched[iSt][cmnLoopDepth]);
          NextLoadSchedGreater = (NextLoadSched[iLd][cmnLoopDepth] >
                                  NextStoreSched[iSt][cmnLoopDepth]);
        }
      }

      bool ldAddrNoDecr = true;
      UnrolledLoop<loadLoopDepth, std::max(cmnLoopDepth, 0)>([&](auto iD) {
        if constexpr (DI.LOAD_IS_MAX_ITER_NEEDED[iLd][iD]) {
          ldAddrNoDecr &= LoadAckIsMaxIter[iLd][iD];
        }
      });

      bool LoadSchedIsGreater = AckLoadSchedGreater ||
                          (NextLoadSchedGreater && LoadNoOutstandingAcks[iLd]);
      bool LoadWillNotTouchStoreAddr = (loadSchedEqaul && ldAddrNoDecr &&
                                        LoadAckAddr[iLd] > NextStoreAddr[iSt]);

      return LoadSchedIsGreater || LoadWillNotTouchStoreAddr;
    };

    auto checkNoWAW = [&](const auto iSt, const auto iStOther) {
      constexpr int cmnLoopDepth = DI.COMMON_STORE_LOOP_DEPTH[iSt][iStOther];
      constexpr int otherLoopDepth = DI.STORE_LOOP_DEPTH[iStOther];
      constexpr DEP_DIR depDir = (iSt < iStOther) ? BACK : FORWARD;

      bool OtherSchedGreater = (iStOther > iSt);
      bool OtherSchedEqual = true;
      if constexpr (cmnLoopDepth >= 0) {
        constexpr int otherWrapFromCmn = walkUpStoreToFirstWrap(iSt, cmnLoopDepth);
        constexpr int eqCheckLoopDepth =
            (otherWrapFromCmn >= 0) ? otherWrapFromCmn : cmnLoopDepth;

        if constexpr (eqCheckLoopDepth >= 0) {
          OtherSchedEqual = (StoreAckSched[iStOther][eqCheckLoopDepth] ==
                             NextStoreSched[iSt][eqCheckLoopDepth]);
          if constexpr (depDir == BACK) {
            OtherSchedEqual |= ((StoreAckSched[iStOther][eqCheckLoopDepth] +1) == 
                                NextStoreSched[iSt][eqCheckLoopDepth]);
          }
        }

        if constexpr (depDir == BACK) {
          OtherSchedGreater = (NextStoreSched[iStOther][cmnLoopDepth] >=
                               NextStoreSched[iSt][cmnLoopDepth]);
        } else {
          OtherSchedGreater = (NextStoreSched[iStOther][cmnLoopDepth] >
                               NextStoreSched[iSt][cmnLoopDepth]);
        }
      }

      bool otherAddrNoDecr = true;
      UnrolledLoop<otherLoopDepth, std::max(cmnLoopDepth, 0)>([&](auto iD) {
        if constexpr (DI.STORE_IS_MAX_ITER_NEEDED[iStOther][iD]) {
          otherAddrNoDecr &= StoreAckIsMaxIter[iStOther][iD];
        }
      });

      bool OtherWillNotTouchAddr =
          (otherAddrNoDecr && OtherSchedEqual &&
           StoreAckAddr[iStOther] > NextStoreAddr[iSt]);

      return OtherSchedGreater || OtherWillNotTouchAddr;
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
          if constexpr (!DI.ARE_IN_SAME_LOOP[iLd][iSt]) {
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
  
  constexpr int NUM_LOADS_p_new = 2;
  constexpr int NUM_STORES_p_new = 2;
  constexpr int LOOP_DEPTH_p_new = 3;
  using LoadAddrPipes_p_new = PipeArray<class _LoadAddr_p_new, ld_req_t<NUM_STORES_p_new, LOOP_DEPTH_p_new>, 16, NUM_LOADS_p_new>;
  using LoadValPipes_p_new = PipeArray<class _LoadVal_p_new, float, 16, NUM_LOADS_p_new>;
  using StoreAddrPipes_p_new = PipeArray<class _StoreAddr_p_new, st_req_t<LOOP_DEPTH_p_new>, 16, NUM_STORES_p_new, 2>;
  using StoreValPipes_p_new = PipeArray<class _StoreVal_p_new, float, 16, NUM_STORES_p_new>;
  
  constexpr int NUM_LOADS_p = 1;
  constexpr int NUM_STORES_p = 1;
  constexpr int LOOP_DEPTH_p = 2;
  using LoadAddrPipes_p = PipeArray<class _LoadAddr_p, ld_req_t<NUM_STORES_p, LOOP_DEPTH_p>, 16, NUM_LOADS_p>;
  using LoadValPipes_p = PipeArray<class _LoadVal_p, float, 16, NUM_LOADS_p>;
  using StoreAddrPipes_p = PipeArray<class _StoreAddr_p, st_req_t<LOOP_DEPTH_p>, 16, NUM_STORES_p, 2>;
  using StoreValPipes_p = PipeArray<class _StoreVal_p, float, 16, NUM_STORES_p>;

  // using LoopPredPipe = PipeArray<class _LoopPredPipe, bool, 16, LOOP_DEPTH_p, LOOP_DEPTH_p>;

  q.single_task<class AGU_l0>([=]() [[intel::kernel_args_restrict]] {
    st_req_t<LOOP_DEPTH_p_new> st_req {0u};
    InitBundle(st_req.sched, 0u);
    InitBundle(st_req.isMaxIter, false);

    for (uint iter = 0; iter < maxIters; ++iter) {
      st_req.sched[0]++;
      st_req.isMaxIter[0] = (iter + 1) == maxIters;

      for (uint i = 0; i < numNodes; i++) {
        st_req.sched[1]++;
        st_req.isMaxIter[1] = (i + 1) == numNodes;

        st_req.addr = i;
        StoreAddrPipes_p_new::PipeAt<0, 0>::write(st_req);
        StoreAddrPipes_p_new::PipeAt<0, 1>::write(st_req);
      }
    }

    st_req.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req.sched, SCHED_SENTINEL);
    InitBundle(st_req.isMaxIter, true);
    StoreAddrPipes_p_new::PipeAt<0, 0>::write(st_req);
    StoreAddrPipes_p_new::PipeAt<0, 1>::write(st_req);

    // PRINTF("** DONE AGU 0\n");
  });

  q.single_task<class AGU_l1>([=]() [[intel::kernel_args_restrict]] {
    st_req_t<LOOP_DEPTH_p_new> st_req_p_new {0u};
    InitBundle(st_req_p_new.sched, 0u);
    InitBundle(st_req_p_new.isMaxIter, false);

    ld_req_t<NUM_STORES_p_new, LOOP_DEPTH_p_new> ld_req_p_new {0u};
    InitBundle(ld_req_p_new.sched, 0u);
    InitBundle(ld_req_p_new.posDepDist, false);
    InitBundle(ld_req_p_new.isMaxIter, false);

    ld_req_t<NUM_STORES_p, LOOP_DEPTH_p> ld_req_p {0u};
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

      int curcol = 0;
      for (uint i = 0; i < numNodes; i++) {
        st_req_p_new.sched[1]++;
        st_req_p_new.isMaxIter[1] = (i+1) == numNodes;

        ld_req_p_new.sched[1]++;
        ld_req_p_new.isMaxIter[1] = (i+1) == numNodes;

        ld_req_p.sched[1]++;
        ld_req_p.isMaxIter[1] = (i+1) == numNodes;

        const auto rowel = row_ptr[i + 1] - row_ptr[i];

        int local_curcol = curcol;
        ld_req_p.addr = i;
        LoadAddrPipes_p::PipeAt<0>::write(ld_req_p);

        for (int j = 0; j < rowel; j++) {
          st_req_p_new.sched[2]++;
          st_req_p_new.isMaxIter[2] = (j+1) == rowel;

          ld_req_p_new.sched[2]++;
          ld_req_p_new.isMaxIter[2] = (j+1) == rowel;

          ld_req_p_new.addr = col_idx[local_curcol];
          ld_req_p_new.posDepDist[1] = ld_req_p_new.addr > st_req_p_new.addr;
          LoadAddrPipes_p_new::PipeAt<0>::write(ld_req_p_new);

          st_req_p_new.addr = col_idx[local_curcol];
          StoreAddrPipes_p_new::PipeAt<1, 0>::write(st_req_p_new);
          StoreAddrPipes_p_new::PipeAt<1, 1>::write(st_req_p_new);

          local_curcol++;
        }

        curcol += rowel;
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
    StoreAddrPipes_p_new::PipeAt<1, 0>::write(st_req_p_new);
    StoreAddrPipes_p_new::PipeAt<1, 1>::write(st_req_p_new);

    // PRINTF("** DONE AGU 1\n");
  });

  q.single_task<class AGU_l2>([=]() [[intel::kernel_args_restrict]] {
    st_req_t<LOOP_DEPTH_p> st_req_p {0u};
    InitBundle(st_req_p.sched, 0u);
    InitBundle(st_req_p.isMaxIter, false);

    ld_req_t<NUM_STORES_p_new, LOOP_DEPTH_p_new> ld_req_p_new {0u};
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
        StoreAddrPipes_p::PipeAt<0, 0>::write(st_req_p);
        StoreAddrPipes_p::PipeAt<0, 1>::write(st_req_p);
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
    StoreAddrPipes_p::PipeAt<0, 0>::write(st_req_p);
    StoreAddrPipes_p::PipeAt<0, 1>::write(st_req_p);

    // PRINTF("** DONE AGU 1\n");
  });

  auto memEvents =
      StreamingMemory<4, LoadAddrPipes_p_new, LoadValPipes_p_new,
                      StoreAddrPipes_p_new, StoreValPipes_p_new,
                      NUM_LOADS_p_new, NUM_STORES_p_new, LOOP_DEPTH_p_new>(
          q, p_new);

  auto memEvents2 =
      StreamingMemory<5, LoadAddrPipes_p, LoadValPipes_p, StoreAddrPipes_p,
                      StoreValPipes_p, NUM_LOADS_p, NUM_STORES_p, LOOP_DEPTH_p>(
          q, p);

  auto event1 = q.single_task<class MainKernel0>([=]() [[intel::kernel_args_restrict]] {
    for (uint iter = 0; iter < maxIters; ++iter) {
      for (uint i = 0; i < numNodes; i++) {
        StoreValPipes_p_new::PipeAt<0>::write(0.0f);
      }
    }

    // StoreValPipes_p_new::PipeAt<0>::write({});
    // PRINTF("** DONE MainKernel0\n");
  });

  auto event2 = q.single_task<class MainKernel1>([=]() [[intel::kernel_args_restrict]] {
    for (uint iter = 0; iter < maxIters; ++iter) {
      int curcol = 0;
      for (uint i = 0; i < numNodes; i++) {

        int rowel = row_ptr[i + 1] - row_ptr[i];
        auto local_curcol = curcol;
        auto LoadVal_p = LoadValPipes_p::PipeAt<0>::read();
        for (int j = 0; j < rowel; j++) {
          // PRINTF("ld0 (%d, %d, %d), p[%d] = %f\n", iter, i, j, i, LoadVal_p);

          auto LoadVal_p_new = LoadValPipes_p_new::PipeAt<0>::read();
          // PRINTF("ld1 (%d, %d, %d), p_new[%d] = %f\n", iter, i, j,
                //  col_idx[local_curcol], LoadVal_p_new);

          auto StoreVal_p_new = LoadVal_p_new + val[local_curcol] * LoadVal_p;
          // PRINTF("st1 (%d, %d, %d), p_new[%d] = %f\n", iter, i, j,
                //  col_idx[local_curcol], StoreVal_p_new);

          StoreValPipes_p_new::PipeAt<1>::write(StoreVal_p_new);

          local_curcol++;
        }
        curcol += rowel;
      }
    }
    // StoreValPipes_p_new::PipeAt<1>::write({});
    // PRINTF("** DONE MainKernel1\n");
  });

  auto event3 = q.single_task<class MainKernel2>([=]() [[intel::kernel_args_restrict]] {
    for (uint iter = 0; iter < maxIters; ++iter) {
      for (uint i = 0; i < numNodes; i++) {
        auto LoadVal_p_new = LoadValPipes_p_new::PipeAt<1>::read();
        // PRINTF("ld2 (%d, %d), p_new[%d] = %f\n", iter, i, i, LoadVal_p_new);

        auto valP = d * LoadVal_p_new + (1.0f - d) / numNodes;
        // PRINTF("st2 (%d, %d), p[%d] = %f\n", iter, i, i, valP);
        StoreValPipes_p::PipeAt<0>::write(valP);
      }
    }

    // StoreValPipes_p::PipeAt<0>::write({});
    // PRINTF("** DONE MainKernel2\n");
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
    // Open the data set
    // char filename[] = "/home/rob/git/elastic-sycl-hls/experiments/web-NotreDame.txt";
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
