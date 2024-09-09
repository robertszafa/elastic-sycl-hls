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

template <typename T>
struct value_t {
  T val;
  bool valid;
};

#define KERNEL_PRAGMAS [[intel::kernel_args_restrict]] [[intel::max_global_work_dim(0)]]

// We use a dynamic burst load-store unit supplied by the intel HLS compiler.
using BurstCoalescedLSU =
    ext::intel::lsu<ext::intel::burst_coalesce<true>,
                    ext::intel::statically_coalesce<false>,
                    ext::intel::prefetch<false>>;

// constexpr addr_t INIT_ACK_ADDR = (1<<29);
constexpr addr_t INIT_ACK_ADDR = 0u;
constexpr addr_t INIT_ACK_SCHED = 1u;
constexpr addr_t STORE_ADDR_SENTINEL = (1<<29) - 1;
constexpr addr_t LOAD_ADDR_SENTINEL = (1<<29) - 2;
constexpr addr_t FINAL_LD_ADDR_ACK = STORE_ADDR_SENTINEL + 1;
constexpr sched_t SCHED_SENTINEL = (1<<30);

/// Unique kernel name generators.
template <int MemId> class StreamingMemoryKernel;
template <int PortId> class StorePortKernel;
template <int PortId> class LoadPortKernel;
template <int LoadId> class LoadLastIterKernel;
template <int StoreId> class StoreLastIterKernel;

// Applaying [[optnone]] to StreamingMemory doesn't apply the attribute to 
// nested lambdas, so apply the attribute to a range of source code.
#pragma clang attribute push (__attribute__((optnone)), apply_to=function)

template <int MEM_ID, typename LoadReqPipes, typename LoadValPipes,
          typename StoreReqPipes, typename StoreValPipes>
[[clang::optnone]] std::vector<event> StreamingMemory(queue &q) {
  using T = decltype(LoadValPipes::template PipeAt<0>::read());
  constexpr uint NUM_LOADS = DepInfo<MEM_ID>{}.NUM_LOADS;
  constexpr uint NUM_STORES = DepInfo<MEM_ID>{}.NUM_STORES;
  constexpr uint LOOP_DEPTH = DepInfo<MEM_ID>{}.MAX_LOOP_DEPTH; 
  std::vector<event> events(NUM_STORES + 1);

  // Sizes of internal buffers.
  constexpr uint DRAM_BURST_BYTES = 64; /// 512-bit DRAM interface
  constexpr uint T_BYTES = sizeof(T);
  constexpr uint BURST_SIZE = ((DRAM_BURST_BYTES + T_BYTES - 1) / T_BYTES) ;
  constexpr uint ST_COMMIT_Q_SIZE = BURST_SIZE + 4;
  constexpr uint ST_ALLOC_Q_SIZE = 2;
  constexpr uint LD_Q_SIZE = 2;

  // Kernels to tag last iterations
  // LOADS:
  using LoadReqInternalPipes = 
    PipeArray<class _LoadReqInternalPipes, ld_req_t<NUM_STORES, LOOP_DEPTH>, 2, NUM_LOADS>;
  UnrolledLoop<NUM_LOADS>([&](auto iLd) {
    constexpr int LoadId = 300 * (MEM_ID + 1) + iLd;
    q.single_task<LoadLastIterKernel<LoadId>>([=]() KERNEL_PRAGMAS {
      static constexpr auto DI = DepInfo<MEM_ID>{};
      
      ld_req_t<NUM_STORES, LOOP_DEPTH> LastReq {0};
      InitBundle(LastReq.sched, 1u);

      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        const ld_req_t<NUM_STORES, LOOP_DEPTH> ThisReq =
            LoadReqPipes::template PipeAt<iLd>::read();

        if (ThisReq.sched[DI.LOAD_LOOP_DEPTH[iLd]] > 1) {
          UnrolledLoop<1, LOOP_DEPTH>([&](int iD) {
            LastReq.isMaxIter[iD] = ThisReq.sched[iD - 1] > LastReq.sched[iD - 1];
          });
          LoadReqInternalPipes::template PipeAt<iLd>::write(LastReq);
        }

        LastReq = ThisReq;

        if (ThisReq.addr == LOAD_ADDR_SENTINEL)
          break;
      }

      UnrolledLoop<LOOP_DEPTH>([&](auto iD) { LastReq.isMaxIter[iD] = true; });
      LoadReqInternalPipes::template PipeAt<iLd>::write(LastReq);
    });
  });

  // STORES:
  using StoreReqInternalPipes = 
    PipeArray<class _StoreReqInternalPipes, st_req_t<LOOP_DEPTH>, 2, NUM_STORES, 2>;
  UnrolledLoop<NUM_STORES>([&](auto iSt) {
    constexpr int StoreId = 300 * (MEM_ID + 1) + iSt;
    q.single_task<StoreLastIterKernel<StoreId>>([=]() KERNEL_PRAGMAS {
      static constexpr auto DI = DepInfo<MEM_ID>{};

      st_req_t<LOOP_DEPTH> LastReq {0};
      InitBundle(LastReq.sched, 1u);

      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        const st_req_t<LOOP_DEPTH> ThisReq =
            StoreReqPipes::template PipeAt<iSt>::read();

        if (ThisReq.sched[DI.STORE_LOOP_DEPTH[iSt]] > 1) {
          UnrolledLoop<1, LOOP_DEPTH>([&](auto iD) {
            LastReq.isMaxIter[iD] = ThisReq.sched[iD - 1] > LastReq.sched[iD - 1];
          });
          StoreReqInternalPipes::template PipeAt<iSt, 0>::write(LastReq);
          StoreReqInternalPipes::template PipeAt<iSt, 1>::write(LastReq);
        }

        LastReq = ThisReq;

        if (ThisReq.addr == STORE_ADDR_SENTINEL)
          break;
      }

      UnrolledLoop<LOOP_DEPTH>([&](auto iD) { LastReq.isMaxIter[iD] = true; });
      StoreReqInternalPipes::template PipeAt<iSt, 0>::write(LastReq);
      StoreReqInternalPipes::template PipeAt<iSt, 1>::write(LastReq);
    });
  });

  // Store ports.
  // Because we use a dynamic burstig LSU, it is important that the store port
  // receives store addresses as soon as possible (hence separate addr and val 
  // pipes). The LSU can issue a burst sooner if it can see the next addr.
  using StorePortValPipes = PipeArray<class _StorePortValPipes, value_t<T>, 1, NUM_STORES>;
  using StoreAckPipes = PipeArray<class _StoreAckPipe, ack_t<LOOP_DEPTH>, 2, NUM_STORES>;
  UnrolledLoop<NUM_STORES>([&](auto iSt) {
    constexpr int StPortId = 100*(MEM_ID+1) + iSt;
    events[iSt] = q.single_task<StorePortKernel<StPortId>>([=]() KERNEL_PRAGMAS {
      ack_t<LOOP_DEPTH> NextAck {INIT_ACK_ADDR};
      InitBundle(NextAck.sched, INIT_ACK_SCHED);
      InitBundle(NextAck.isMaxIter, true);

      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        StoreAckPipes::template PipeAt<iSt>::write(NextAck);

        const auto Req = StoreReqInternalPipes::template PipeAt<iSt, 0>::read();
        if (Req.addr == STORE_ADDR_SENTINEL) break;

        auto Val = StorePortValPipes::template PipeAt<iSt>::read();

        // We construct a pointer from only 32 bits (incorrect for >4GB DRAMs).
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wint-to-pointer-cast"
        auto StorePtr = ext::intel::device_ptr<T>((T*) Req.addr);
        #pragma clang diagnostic pop

        // PRINTF("MEM%d st%d stores address %u\n", MEM_ID, int(iSt), Req.addr);
        if (Val.valid) {
          BurstCoalescedLSU::store(StorePtr, Val.val);
          NextAck = Req;
        } else {
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            NextAck.sched[iD] = Req.sched[iD];
            NextAck.isMaxIter[iD] = Req.isMaxIter[iD];
          });
        }
      }

      // Force any outstanding burst before ACKing addr sentinel.
      atomic_fence(memory_order_seq_cst, memory_scope_work_item);
      ack_t<LOOP_DEPTH> FinalAck{STORE_ADDR_SENTINEL};
      InitBundle(FinalAck.sched, SCHED_SENTINEL);
      InitBundle(FinalAck.isMaxIter, true);
      StoreAckPipes::template PipeAt<iSt>::write(FinalAck);
      PRINTF("STORE PORT %d done\n", int(iSt));
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
          #pragma clang diagnostic push
          #pragma clang diagnostic ignored "-Wint-to-pointer-cast"
          auto LoadPtr = ext::intel::device_ptr<T>((T*) Req.ack.addr);
          #pragma clang diagnostic pop
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
      PRINTF("MEM%d ld%d reused %d/%d\n", MEM_ID, int(iLd), NumReused, NumTotal);
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
    bool NextStoreValuePipeReadValid[NUM_STORES];
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
      NextStoreValueValid[iSt] = T{};
      NextStoreValuePipeReadValid[iSt] = false;

      InitBundle(StoreButrstBuffAddr[iSt], STORE_ADDR_SENTINEL);
      InitBundle(StoreBurstBuffVal[iSt], T{});

      StoreAckAddr[iSt] = INIT_ACK_ADDR;
      InitBundle(StoreAckSched[iSt], INIT_ACK_SCHED);
      InitBundle(StoreAckIsMaxIter[iSt], true);

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

      StoreAckAddr[iLd] = INIT_ACK_ADDR;
      InitBundle(LoadAckSched[iLd], INIT_ACK_SCHED);
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
    // k - common loop depth
    // n - operation a loop depth
    // m - operation b loop depth
    // RAW check - b is a store, a is a load
    // WAR check - b is a load, a is a store

    constexpr auto walkUpStoreToFirstWrap = [&](const auto iSt,
                                                const auto start) {
      for (int i = start; i >= 0; --i) {
        if (DI.STORE_IS_MAX_ITER_NEEDED[iSt][i]) {
          return i;
        }
      }

      return -1;
    };
    // constexpr auto walkUpLoadToFirstWrap = [&](const auto iLd,
    //                                           const auto start) {
    //   for (int i = start; i >= 0; --i) {
    //     if (DI.LOAD_IS_MAX_ITER_NEEDED[iLd][i]) {
    //       return i;
    //     }
    //   }

    //   return -1;
    // };

    auto checkNoRAW = [&](const auto iLd, const auto iSt) {
      constexpr int k = DI.LOAD_STORE_COMMON_LOOP_DEPTH[iLd][iSt];
      constexpr int m = DI.STORE_LOOP_DEPTH[iSt];
      constexpr bool a_prec_b = DI.LOAD_BEFORE_STORE_IN_TOPOLOGICAL_ORDER[iLd][iSt];

      bool ProgramOrderSafe = a_prec_b;
      if constexpr (k >= 0) {
        ProgramOrderSafe =
            a_prec_b ? (NextLoadSched[iLd][k] <= StoreAckSched[iSt][k])
                     : (NextLoadSched[iLd][k] < StoreAckSched[iSt][k]);
      }

      bool NoAddressReset = true;
      if constexpr (m-1 > k+1) {
        UnrolledLoop<k+1, m-1>([&](auto iD) {
          if constexpr (DI.STORE_IS_MAX_ITER_NEEDED[iSt][iD])
            NoAddressReset &= StoreAckIsMaxIter[iSt][iD];
        });
      }

      bool NoBRequestsUntilA =
          a_prec_b ? (NextLoadSched[iLd][k] == StoreAckSched[iSt][k] + 1)
                   : (NextLoadSched[iLd][k] == StoreAckSched[iSt][k]);
      if constexpr (m > k+1) {
        UnrolledLoop<k + 1, m>([&](auto iD) {
          NoBRequestsUntilA &= StoreAckIsMaxIter[iSt][iD];
        });
      }

      bool LoadHasPosDepDistance = false;
      if constexpr (DI.LOAD_STORE_IN_SAME_LOOP[iLd][iSt]) {
        bool CanCheckPosDepDist = true;
        constexpr int mWrapDepth = walkUpStoreToFirstWrap(iSt, m - 1);
        if constexpr (mWrapDepth >= 0 && mWrapDepth < k) {
          // CanCheckPosDepDist = (NextStoreSched[iSt][mWrapDepth] >=
          CanCheckPosDepDist = (StoreAckSched[iSt][mWrapDepth] >=
                                NextLoadSched[iLd][mWrapDepth]);
        }

        LoadHasPosDepDistance = CanCheckPosDepDist && NextLoadPosDepDist[iLd][iSt];
      }

      return ProgramOrderSafe ||
             (NextLoadAddr[iLd] < StoreAckAddr[iSt] && NoAddressReset) ||
             NoBRequestsUntilA || LoadHasPosDepDistance;
    };

    auto checkNoWAR = [&](const auto iLd, const auto iSt) {
      constexpr int k = DI.LOAD_STORE_COMMON_LOOP_DEPTH[iLd][iSt];
      constexpr int m = DI.LOAD_LOOP_DEPTH[iLd];
      constexpr bool a_prec_b = !DI.LOAD_BEFORE_STORE_IN_TOPOLOGICAL_ORDER[iLd][iSt];

      bool ProgramOrderSafe = a_prec_b;
      if constexpr (k >= 0) {
        ProgramOrderSafe =
            a_prec_b ? (NextStoreSched[iSt][k] <= LoadAckSched[iLd][k])
                     : (NextStoreSched[iSt][k] < LoadAckSched[iLd][k]);
      }

      bool NoAddressReset = true;
      if constexpr (m-1 > k+1) {
        UnrolledLoop<k+1, m-1>([&](auto iD) {
          if constexpr (DI.LOAD_IS_MAX_ITER_NEEDED[iLd][iD])
            NoAddressReset &= LoadAckIsMaxIter[iLd][iD];
        });
      }

      bool NoBRequestsUntilA =
          a_prec_b ? (NextStoreSched[iSt][k] == LoadAckSched[iLd][k] + 1)
                   : (NextStoreSched[iSt][k] == LoadAckSched[iLd][k]);
      UnrolledLoop<k + 1, m>([&](auto iD) {
        NoBRequestsUntilA &= LoadAckIsMaxIter[iLd][iD];
      });

      return ProgramOrderSafe ||
             (NextStoreAddr[iSt] < LoadAckAddr[iLd] && NoAddressReset) ||
             NoBRequestsUntilA;
    };

    auto checkNoWAW = [&](const auto iSt, const auto iStOther) {
      constexpr int k = DI.STORE_STORE_COMMON_LOOP_DEPTH[iSt][iStOther];
      constexpr int m = DI.STORE_LOOP_DEPTH[iSt];
      // Store ids are assigned in topological program order.
      constexpr bool a_prec_b = iSt < iStOther;

      bool ProgramOrderSafe = a_prec_b;
      if constexpr (k >= 0) {
        ProgramOrderSafe =
            a_prec_b ? (NextStoreSched[iSt][k] <= StoreAckSched[iStOther][k])
                     : (NextStoreSched[iSt][k] < StoreAckSched[iStOther][k]);
      }

      bool NoAddressReset = true;
      if constexpr (m-1 > k+1) {
        UnrolledLoop<k+1, m-1>([&](auto iD) {
          if constexpr (DI.STORE_IS_MAX_ITER_NEEDED[iStOther][iD])
            NoAddressReset &= StoreAckIsMaxIter[iStOther][iD];
        });
      }

      bool NoBRequestsUntilA =
          a_prec_b ? (NextStoreSched[iSt][k] == StoreAckSched[iStOther][k] + 1)
                   : (NextStoreSched[iSt][k] == StoreAckSched[iStOther][k]);
      UnrolledLoop<k + 1, m>([&](auto iD) {
        NoBRequestsUntilA &= StoreAckIsMaxIter[iStOther][iD];
      });

      return ProgramOrderSafe ||
             (NextStoreAddr[iSt] < StoreAckAddr[iStOther] && NoAddressReset) ||
             NoBRequestsUntilA;
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
          const auto LoadReq = LoadReqInternalPipes::template PipeAt<iLd>::read(succ);
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
          bool ThisSafe[NUM_STORES];
          UnrolledLoop<NUM_STORES>([&](auto iSt) {
            ThisSafe[iSt] = checkNoRAW(iLd, iSt);
            // NoRAW &= checkNoRAW(iLd, iSt);
            NoRAW &= ThisSafe[iSt];

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

          // if (NoRAW) {
          // if (iLd == 0) {
          //   PRINTF("Safe? %d LD%d: %d (%d, %d, %d)  ackST0: %d (%d/%d, %d/%d, %d/%d)  ackST1: %d (%d/%d, %d/%d, %d/%d) [%d, %d]\n", 
          //          NoRAW, int(iLd), NextLoadAddr[iLd], NextLoadSched[iLd][0], NextLoadSched[iLd][1], NextLoadSched[iLd][2],
          //          StoreAckAddr[0], StoreAckSched[0][0], StoreAckIsMaxIter[0][0], StoreAckSched[0][1], StoreAckIsMaxIter[0][1], StoreAckSched[0][2], StoreAckIsMaxIter[0][2],
          //          StoreAckAddr[1], StoreAckSched[1][0], StoreAckIsMaxIter[1][0], StoreAckSched[1][1], StoreAckIsMaxIter[1][1], StoreAckSched[1][2], StoreAckIsMaxIter[1][2],
          //          ThisSafe[0], ThisSafe[1]
          //          );
          // }
          // }

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

          // If no hazards against any store, then move load to "safe" stage.
          // if (NoRAW || Reuse) {
          if (NoRAW) {
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
          auto StoreReq = StoreReqInternalPipes::template PipeAt<iSt, 1>::read(
              StoreValid[iSt][ST_ALLOC_Q_SIZE - 1]);
          StoreAddr[iSt][ST_ALLOC_Q_SIZE - 1] = StoreReq.addr;
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            StoreSched[iSt][iD][ST_ALLOC_Q_SIZE - 1] = StoreReq.sched[iD];
            StoreIsMaxIter[iSt][iD][ST_ALLOC_Q_SIZE - 1] = StoreReq.isMaxIter[iD];
          });
        }
        /** End Rule */

        /** Rule for reading store value. */
        if (!NextStoreValuePipeReadValid[iSt]) {
          value_t<T> valRd = StoreValPipes::template PipeAt<iSt>::read(
              NextStoreValuePipeReadValid[iSt]);
          NextStoreValue[iSt] = valRd.val;
          NextStoreValueValid[iSt] = valRd.valid;
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
          if constexpr (!DI.LOAD_STORE_IN_SAME_CU[iLd][iSt]) {
            NoWAR &= checkNoWAR(iLd, iSt);
          }
        });
        const bool IsSafe = (NoWAW && NoWAR) || !NextStoreValueValid[iSt];
        /** End Rule */

        /** Rule for moving st alloc to st commit queue. */
        if (StoreValid[iSt][0] && NextStoreValuePipeReadValid[iSt] && IsSafe) {
          bool succ = false;
          StorePortValPipes::template PipeAt<iSt>::write(
              {NextStoreValue[iSt], NextStoreValueValid[iSt]}, succ);

          if (succ) {
            // Don't move invalid store to reuse buffer.
            if (NextStoreValueValid[iSt]) {
              ShiftBundle(StoreBurstBuffVal[iSt], NextStoreValue[iSt]);
              ShiftBundle(StoreButrstBuffAddr[iSt], NextStoreAddr[iSt]);
            }

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
            NextStoreValuePipeReadValid[iSt] = false;
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
