#ifndef __DATA_UNIT_DRAM_HPP__
#define __DATA_UNIT_DRAM_HPP__

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

/// Use only offset in (base + offset) addr representation.
using addr_t = uint;
using sched_t = uint;

template <int LOOP_DEPTH>
struct st_req_t {
  addr_t addr;
  sched_t sched[LOOP_DEPTH];
  bool isLastIter[LOOP_DEPTH];
};
template <int NUM_STORES, int LOOP_DEPTH>
struct ld_req_t {
  addr_t addr;
  sched_t sched[LOOP_DEPTH];
  bool posDepDist[NUM_STORES];
  bool isLastIter[LOOP_DEPTH];
};

template <int LOOP_DEPTH>
using ack_t = st_req_t<LOOP_DEPTH>;

template <int LOOP_DEPTH, typename T>
struct ld_port_req_t {
  ack_t<LOOP_DEPTH> ack;
};

template <int LOOP_DEPTH, typename T>
struct st_port_req_t {
  T val;
  ack_t<LOOP_DEPTH> ack;
};

template <typename T>
struct tagged_val_t {
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
template <int MemId> class DataUnitDRAMKernel;
template <int PortId> class StorePortKernel;
template <int PortId> class LoadPortKernel;
template <int LoadId> class LoadLastIterKernel;
template <int StoreId> class StoreLastIterKernel;
template <int StoreId> class StoreAckDelayKernel;

// Applaying [[optnone]] to DataUnitDRAM doesn't apply the attribute to 
// nested lambdas, so apply the attribute to a range of source code.
#pragma clang attribute push (__attribute__((optnone)), apply_to=function)

template <int MEM_ID, typename LoadReqPipes, typename LoadValPipes,
          typename StoreReqPipes, typename StoreValPipes>
[[clang::optnone]] std::vector<event> DataUnitDRAM(queue &q) {
  using T = decltype(LoadValPipes::template PipeAt<0>::read());
  constexpr uint NUM_LOADS = DepInfo<MEM_ID>{}.NUM_LOADS;
  constexpr uint NUM_STORES = DepInfo<MEM_ID>{}.NUM_STORES;
  constexpr uint LOOP_DEPTH = DepInfo<MEM_ID>{}.MAX_LOOP_DEPTH; 
  std::vector<event> events(NUM_STORES);

  // Sizes of internal buffers.
  constexpr uint DRAM_BURST_BYTES = 64; /// 512-bit DRAM interface
  constexpr uint T_BYTES = sizeof(T);
  constexpr uint BURST_SIZE = ((DRAM_BURST_BYTES + T_BYTES - 1) / T_BYTES) ;
  constexpr uint ST_PENDING_BUFF_SIZE = BURST_SIZE + 4;
  constexpr uint ST_REQ_Q_SIZE = 2;
  constexpr uint LD_REQ_Q_SIZE = 2;

  // Store ports.
  // Because we use a dynamic burstig LSU, it is important that the store port
  // receives store addresses as soon as possible (hence separate addr and val 
  // pipes). The LSU can issue a burst sooner if it can see the next addr.
  using StorePortValPipes = PipeArray<class _StorePortValPipes, tagged_val_t<T>, 1, NUM_STORES>;
  using StoreAckPipes = PipeArray<class _StoreAckPipe, ack_t<LOOP_DEPTH>, 2, NUM_STORES>;
  using StoreAckDelayPipes = PipeArray<class _StoreAckDelayPipes, ack_t<LOOP_DEPTH>, 2, NUM_STORES>;
  UnrolledLoop<NUM_STORES>([&](auto iSt) {
    constexpr int StPortId = 100*(MEM_ID+1) + iSt;
    events[iSt] = q.single_task<StorePortKernel<StPortId>>([=]() KERNEL_PRAGMAS {
      ack_t<LOOP_DEPTH> NextAck {INIT_ACK_ADDR};
      InitBundle(NextAck.sched, INIT_ACK_SCHED);
      InitBundle(NextAck.isLastIter, true);

      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        StoreAckDelayPipes::template PipeAt<iSt>::write(NextAck);

        const auto Req = StoreReqPipes::template PipeAt<iSt, 0>::read();
        if (Req.addr == STORE_ADDR_SENTINEL) break;

        auto Val = StorePortValPipes::template PipeAt<iSt>::read();

        // We construct a pointer from only 32 bits (incorrect for >4GB DRAMs).
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wint-to-pointer-cast"
        auto StorePtr = ext::intel::device_ptr<T>((T*) Req.addr);
        // Use only offset of base+offset address for larger DRAM capacities.
        // auto StorePtr = ext::intel::device_ptr<T>(data + Req.addr);
        #pragma clang diagnostic pop

        // PRINTF("MEM%d st%d stores address %u\n", MEM_ID, int(iSt), Req.addr);
        if (Val.valid) {
          BurstCoalescedLSU::store(StorePtr, Val.val);
          NextAck = Req;
        } else {
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            NextAck.sched[iD] = Req.sched[iD];
            NextAck.isLastIter[iD] = Req.isLastIter[iD];
          });
        }
      }

      // Force any outstanding burst before ACKing addr sentinel.
      atomic_fence(memory_order_seq_cst, memory_scope_work_item);
      ack_t<LOOP_DEPTH> FinalAck{STORE_ADDR_SENTINEL};
      InitBundle(FinalAck.sched, SCHED_SENTINEL);
      InitBundle(FinalAck.isLastIter, true);
      StoreAckDelayPipes::template PipeAt<iSt>::write(FinalAck);
      // PRINTF("STORE PORT %d done\n", int(iSt));
    });

    q.single_task<StoreAckDelayKernel<StPortId>>([=]() KERNEL_PRAGMAS {
      addr_t AckAddrQ[ST_PENDING_BUFF_SIZE];
      sched_t AckSchedQ[LOOP_DEPTH][ST_PENDING_BUFF_SIZE];
      bool AckIsLastIterQ[LOOP_DEPTH][ST_PENDING_BUFF_SIZE];
      InitBundle(AckAddrQ, INIT_ACK_ADDR);
      UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
        InitBundle(AckSchedQ[iD], INIT_ACK_SCHED);
        InitBundle(AckIsLastIterQ[iD], true);
      });
      ack_t<LOOP_DEPTH> NextAck {INIT_ACK_ADDR};
      InitBundle(NextAck.sched, INIT_ACK_SCHED);
      InitBundle(NextAck.isLastIter, true);

      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        ack_t<LOOP_DEPTH> AckToSend {AckAddrQ[0]};
        UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
          AckToSend.sched[iD] = AckSchedQ[iD][0];
          AckToSend.isLastIter[iD] = AckIsLastIterQ[iD][0];
        });
        StoreAckPipes::template PipeAt<iSt>::write(AckToSend);

        bool succ = false;
        const auto Ack = StoreAckDelayPipes::template PipeAt<iSt>::read(succ);
        if (succ) NextAck = Ack;

        ShiftBundle(AckAddrQ, NextAck.addr);
        UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
          ShiftBundle(AckSchedQ[iD], NextAck.sched[iD]);
          ShiftBundle(AckIsLastIterQ[iD], NextAck.isLastIter[iD]);
        });

        if (AckToSend.addr == STORE_ADDR_SENTINEL) break;
      }
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

      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        const auto Req = LoadPortReqPipes::template PipeAt<iLd>::read();
        if (Req.ack.addr == LOAD_ADDR_SENTINEL)
          break;

        // Silence clang warning about int to pointer conversion.
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wint-to-pointer-cast"
        auto LoadPtr = ext::intel::device_ptr<T>((T*) Req.ack.addr);
        // auto LoadPtr = ext::intel::device_ptr<T>(data + Req.ack.addr);
        #pragma clang diagnostic pop
        T Val = BurstCoalescedLSU::load(LoadPtr);

        // Ensure ld ack is sent after we get the value by using a constraint.
        LoadValPipes::template PipeAt<iLd>::write(Val, anchor);
        LoadAckPipes::template PipeAt<iLd>::write(Req.ack, constraint);
      }

      ack_t<LOOP_DEPTH> maxAck{FINAL_LD_ADDR_ACK};
      InitBundle(maxAck.sched, SCHED_SENTINEL);
      InitBundle(maxAck.isLastIter, true);
      LoadAckPipes::template PipeAt<iLd>::write(maxAck);
    });
  });


  auto du_event = q.single_task<DataUnitDRAMKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    // TDOD: Add hints about the maximum address at any given loop depth?
    // addr_t StoreAllocMaxAddrAtLoop[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    // addr_t LoadMaxAddrAtLoop[NUM_LOADS][LOOP_DEPTH][LD_Q_SIZE];

    /* Store logic registers */
    // Internal queue of store requests (purely for improved fmax).
    bool StoreValid[NUM_STORES][ST_REQ_Q_SIZE];
    addr_t StoreAddr[NUM_STORES][ST_REQ_Q_SIZE];
    sched_t StoreSched[NUM_STORES][LOOP_DEPTH][ST_REQ_Q_SIZE];
    bool StoreIsLastIter[NUM_STORES][LOOP_DEPTH][ST_REQ_Q_SIZE];
    // Shortcut for the head of the internal store queue.
    addr_t NextStoreAddr[NUM_STORES];
    sched_t NextStoreSched[NUM_STORES][LOOP_DEPTH];
    bool NextStoreIsLastIter[NUM_STORES][LOOP_DEPTH];
    T NextStoreValue[NUM_STORES];
    bool NextStoreValueIsValid[NUM_STORES];
    bool StoreValuePipeReadValid[NUM_STORES];
    // The latest store sent to the HLS memory controller. Note that this is not 
    // the actual ACK received from the memory system (no access to thaty in HLS).
    addr_t AckStoreAddr[NUM_STORES];
    sched_t AckStoreSched[NUM_STORES][LOOP_DEPTH];
    bool AckStoreIsLastIter[NUM_STORES][LOOP_DEPTH];
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
        InitBundle(StoreIsLastIter[iSt][iD], false);
      });

      NextStoreAddr[iSt] = 0u;
      InitBundle(NextStoreSched[iSt], 0u);
      InitBundle(NextStoreIsLastIter[iSt], false);
      NextStoreValue[iSt] = T{};
      NextStoreValueIsValid[iSt] = T{};
      StoreValuePipeReadValid[iSt] = false;

      AckStoreAddr[iSt] = INIT_ACK_ADDR;
      InitBundle(AckStoreSched[iSt], INIT_ACK_SCHED);
      InitBundle(AckStoreIsLastIter[iSt], true);

      StoreNoOutstandingAcks[iSt] = true;
      LastSentStoreSched[iSt] = INIT_ACK_SCHED;

      StoreDone[iSt] = false;
    });

    /* Load logic registers */
    // Internal queue of load requests (purely for improved fmax).
    bool LoadValid[NUM_LOADS][LD_REQ_Q_SIZE];
    addr_t LoadAddr[NUM_LOADS][LD_REQ_Q_SIZE];
    sched_t LoadSched[NUM_LOADS][LOOP_DEPTH][LD_REQ_Q_SIZE];
    bool LoadPosDepDist[NUM_LOADS][NUM_STORES][LD_REQ_Q_SIZE];
    bool LoadIsLastIter[NUM_LOADS][LOOP_DEPTH][LD_REQ_Q_SIZE];
    // Alias to load queue head.
    addr_t NextLoadAddr[NUM_LOADS];
    sched_t NextLoadSched[NUM_LOADS][LOOP_DEPTH];
    bool NextLoadPosDepDist[NUM_LOADS][NUM_STORES];
    bool NextLoadIsLastIter[NUM_LOADS][LOOP_DEPTH];
    // The lates requeust for which we have returned a load value. 
    addr_t AckLoadAddr[NUM_LOADS];
    sched_t AckLoadSched[NUM_LOADS][LOOP_DEPTH];
    bool AckLoadIsLastIter[NUM_LOADS][LOOP_DEPTH];
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
        InitBundle(LoadIsLastIter[iLd][iD], false);
      });

      AckLoadAddr[iLd] = INIT_ACK_ADDR;
      InitBundle(AckLoadSched[iLd], INIT_ACK_SCHED);
      InitBundle(AckLoadIsLastIter[iLd], true);

      NextLoadAddr[iLd] = 0u;
      InitBundle(NextLoadSched[iLd], 0u);
      InitBundle(NextLoadPosDepDist[iLd], false);
      InitBundle(NextLoadIsLastIter[iLd], false);

      LoadDone[iLd] = false;

      LoadNoOutstandingAcks[iLd] = true;
      LastSentLoadSched[iLd] = 0u;
    });

    ///////////////////////////////////////////////////////////////////////////
    // Disambiguation logic. RAW, WAR, WAW hazard checks implemented as lamdas.
    // The dependency info is supplied by the compiler. DependencyTable.hpp has 
    // a few examples.
    ///////////////////////////////////////////////////////////////////////////
    static constexpr auto DI = DepInfo<MEM_ID>{};
    // k - common loop depth
    // l - deepest monotonic loop depth of operation b, such that l<=k
    // n - operation a loop depth
    // m - operation b loop depth
    // RAW check - b is a store, a is a load
    // WAR check - b is a load, a is a store

    constexpr auto walkUpStoreToFirstReset = [&](const auto iSt, const auto k) {
      for (int i = k; i >= 0; --i)
        if (DI.STORE_IS_LAST_ITER_NEEDED[iSt][i])
          return i;
      return -1;
    };
    constexpr auto walkUpLoadToFirstReset = [&](const auto iLd, const auto k) {
      for (int i = k; i >= 0; --i)
        if (DI.LOAD_IS_LAST_ITER_NEEDED[iLd][i])
          return i;
      return -1;
    };

    auto checkNoRAW = [&](const auto iLd, const auto iSt) {
      constexpr int k = DI.LOAD_STORE_COMMON_LOOP_DEPTH[iLd][iSt];
      constexpr int l = walkUpStoreToFirstReset(iSt, k);
      constexpr int m = DI.STORE_LOOP_DEPTH[iSt];
      constexpr bool a_prec_b = DI.LOAD_BEFORE_STORE_IN_TOPOLOGICAL_ORDER[iLd][iSt];

      // (Program Order Safety Check)
      bool ProgramOrderSafe = a_prec_b;
      bool ProgramOrderSafeAgainstNext = a_prec_b;
      if constexpr (k >= 0) {
        ProgramOrderSafe =
            a_prec_b ? (NextLoadSched[iLd][k] <= AckStoreSched[iSt][k])
                     : (NextLoadSched[iLd][k] < AckStoreSched[iSt][k]);
        ProgramOrderSafeAgainstNext =
            a_prec_b ? (NextLoadSched[iLd][k] <= NextStoreSched[iSt][k])
                     : (NextLoadSched[iLd][k] < NextStoreSched[iSt][k]);
      }

      // (No Address Reset Check)
      bool NoAddressReset = true;
      if constexpr (l >= 0) {
        NoAddressReset = a_prec_b ? (NextLoadSched[iLd][l] == (AckStoreSched[iSt][l] + 1))
                                  : (NextLoadSched[iLd][l] == AckStoreSched[iSt][l]);
      }
      if constexpr (m > k + 1) {
        UnrolledLoop<k+1, m>([&](auto iD) {
          if constexpr (DI.STORE_IS_LAST_ITER_NEEDED[iSt][iD])
            NoAddressReset &= AckStoreIsLastIter[iSt][iD];
        });
      }

      bool LoadHasPosDepDistance = false;
      if constexpr (DI.LOAD_STORE_IN_SAME_LOOP[iLd][iSt]) {
        LoadHasPosDepDistance = NextLoadPosDepDist[iLd][iSt];
        if constexpr (l >= 0) 
          LoadHasPosDepDistance &= (NextLoadSched[iLd][l] == AckStoreSched[iSt][l]);
      }

      // (Deadlock avoidance for repeated addresses)
      bool NoBRequestsUntilA = (StoreNoOutstandingAcks[iSt] && ProgramOrderSafeAgainstNext);

      // (Hazard Safety Check) -- relaxed because we use forwarding from pending
      // buffer of not ACKed stores. RAW also checks for positive dependence distance.
      return ProgramOrderSafe ||
             ((NextLoadAddr[iLd] < AckStoreAddr[iSt]) && NoAddressReset) ||
             LoadHasPosDepDistance || NoBRequestsUntilA;
    };
    /*------------------------------------------------------------------------*/
    auto checkNoWAW = [&](const auto iSt, const auto iStOther) {
      constexpr int k = DI.STORE_STORE_COMMON_LOOP_DEPTH[iSt][iStOther];
      constexpr int l = walkUpStoreToFirstReset(iStOther, k);
      constexpr int m = DI.STORE_LOOP_DEPTH[iSt];
      // Store ids are assigned in topological program order.
      constexpr bool a_prec_b = iSt < iStOther;

      // (Program Order Safety Check)
      bool ProgramOrderSafe = a_prec_b;
      bool ProgramOrderSafeAgainstNext = a_prec_b;
      if constexpr (k >= 0) {
        ProgramOrderSafe =
            a_prec_b ? (NextStoreSched[iSt][k] <= AckStoreSched[iStOther][k])
                     : (NextStoreSched[iSt][k] < AckStoreSched[iStOther][k]);
        ProgramOrderSafeAgainstNext =
            a_prec_b ? (NextStoreSched[iSt][k] <= NextStoreSched[iStOther][k])
                     : (NextStoreSched[iSt][k] < NextStoreSched[iStOther][k]);
      }

      // (No Address Reset Check)
      bool NoAddressReset = true;
      if constexpr (l >= 0) {
        NoAddressReset =
          a_prec_b ? (NextStoreSched[iSt][l] == (AckStoreSched[iStOther][l]+1))
                   : (NextStoreSched[iSt][l] == AckStoreSched[iStOther][l]);
      }
      if constexpr (m > k+1) {
        UnrolledLoop<k+1, m>([&](auto iD) {
          if constexpr (DI.STORE_IS_LAST_ITER_NEEDED[iStOther][iD])
            NoAddressReset &= AckStoreIsLastIter[iStOther][iD];
        });
      }

      // (Deadlock avoidance for repeated addresses)
      bool NoBRequestsUntilA = (StoreNoOutstandingAcks[iStOther] && ProgramOrderSafeAgainstNext);

      // (Hazard Safety Check)
      return ProgramOrderSafe ||
             (NextStoreAddr[iSt] < AckStoreAddr[iStOther] && NoAddressReset) ||
             NoBRequestsUntilA;
    };
    /*------------------------------------------------------------------------*/
    auto checkNoWAR = [&](const auto iLd, const auto iSt) {
      constexpr int k = DI.LOAD_STORE_COMMON_LOOP_DEPTH[iLd][iSt];
      constexpr int l = walkUpLoadToFirstReset(iLd, k);
      constexpr int m = DI.LOAD_LOOP_DEPTH[iLd];
      constexpr bool a_prec_b = !DI.LOAD_BEFORE_STORE_IN_TOPOLOGICAL_ORDER[iLd][iSt];

      // (Program Order Safety Check)
      bool ProgramOrderSafe = a_prec_b;
      bool ProgramOrderSafeAgainstNext = a_prec_b;
      if constexpr (k >= 0) {
        ProgramOrderSafe =
            a_prec_b ? (NextStoreSched[iSt][k] <= AckLoadSched[iLd][k])
                     : (NextStoreSched[iSt][k] < AckLoadSched[iLd][k]);
        ProgramOrderSafeAgainstNext =
            a_prec_b ? (NextStoreSched[iSt][k] <= NextLoadSched[iLd][k])
                     : (NextStoreSched[iSt][k] < NextLoadSched[iLd][k]);
      }

      // (No Address Reset Check)
      bool NoAddressReset = true;
      if constexpr (l >= 0) {
        NoAddressReset = a_prec_b ? (NextStoreSched[iSt][l] == (AckLoadSched[iLd][l] + 1))
                                  : (NextStoreSched[iSt][l] == AckLoadSched[iLd][l]);
      }
      if constexpr (m > k+1) {
        UnrolledLoop<k+1, m>([&](auto iD) {
          if constexpr (DI.LOAD_IS_LAST_ITER_NEEDED[iLd][iD])
            NoAddressReset &= AckLoadIsLastIter[iLd][iD];
        });
      }

      // (Deadlock avoidance for repeated addresses)
      bool NoBRequestsUntilA = (LoadNoOutstandingAcks[iLd] && ProgramOrderSafeAgainstNext);

      // (Hazard Safety Check)
      return ProgramOrderSafe ||
             (NextStoreAddr[iSt] < AckLoadAddr[iLd] && NoAddressReset) ||
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
        LoadDone[iLd] = (AckLoadSched[iLd][0] == SCHED_SENTINEL);

        LoadNoOutstandingAcks[iLd] = (LastSentLoadSched[iLd] ==
                                      AckLoadSched[iLd][DI.LOAD_LOOP_DEPTH[iLd]]);

        /** Rule for reading load ACKs (always enabled). */
        bool succ = false;
        auto ldAck = LoadAckPipes::template PipeAt<iLd>::read(succ);
        if (succ) {
          AckLoadAddr[iLd] = ldAck.addr;
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            AckLoadSched[iLd][iD] = ldAck.sched[iD];
            AckLoadIsLastIter[iLd][iD] = ldAck.isLastIter[iD];
          });
        }
        /** End Rule */

        /** Rule for shifting load queue. */
        if (!LoadValid[iLd][0]) {
          if (LoadValid[iLd][1]) {
            NextLoadAddr[iLd] = LoadAddr[iLd][1];
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              NextLoadSched[iLd][iD] = LoadSched[iLd][iD][1];
              NextLoadIsLastIter[iLd][iD] = LoadIsLastIter[iLd][iD][1];
            });
            UnrolledLoop<NUM_STORES>([&](auto iSt) {
              NextLoadPosDepDist[iLd][iSt] = LoadPosDepDist[iLd][iSt][1];
            });
          }

          ShiftBundle(LoadValid[iLd], false);
          ShiftBundle(LoadAddr[iLd], 0u);
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            ShiftBundle(LoadSched[iLd][iD], 0u);
            ShiftBundle(LoadIsLastIter[iLd][iD], false);
          });
          UnrolledLoop<NUM_STORES>(
              [&](auto iSt) { ShiftBundle(LoadPosDepDist[iLd][iSt], false); });
        }
        /** End Rule */

        /** Rule for reading new load allocation: {addr, tag} pairs. */
        if (!LoadValid[iLd][LD_REQ_Q_SIZE - 1]) {
          bool succ = false;
          const auto LoadReq = LoadReqPipes::template PipeAt<iLd>::read(succ);
          if (succ) {
            LoadValid[iLd][LD_REQ_Q_SIZE - 1] = true;
            LoadAddr[iLd][LD_REQ_Q_SIZE - 1] = LoadReq.addr;
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              LoadSched[iLd][iD][LD_REQ_Q_SIZE - 1] = LoadReq.sched[iD];
              LoadIsLastIter[iLd][iD][LD_REQ_Q_SIZE - 1] = LoadReq.isLastIter[iD];
            });
            UnrolledLoop<NUM_STORES>([&](auto iSt) {
              LoadPosDepDist[iLd][iSt][LD_REQ_Q_SIZE - 1] = LoadReq.posDepDist[iSt];
            });
          }
        }
        /** End Rule */

        /** Rule for checking load safety and reuse. */
        if (LoadValid[iLd][0]) {
          bool NoRAW = true;
          UnrolledLoop<NUM_STORES>([&](auto iSt) {
            NoRAW &= checkNoRAW(iLd, iSt);
          });

          // If no hazards against any store, then move load to "safe" stage.
          if (NoRAW) {
            bool succ = false;
            ack_t<LOOP_DEPTH> MuxAck{NextLoadAddr[iLd]};
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              MuxAck.sched[iD] = NextLoadSched[iLd][iD];
              MuxAck.isLastIter[iD] = NextLoadIsLastIter[iLd][iD];
            });
            ld_port_req_t<LOOP_DEPTH, T> Req = {MuxAck};
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
        StoreDone[iSt] = (AckStoreSched[iSt][0] == SCHED_SENTINEL);

        StoreNoOutstandingAcks[iSt] =
          (AckStoreSched[iSt][DI.STORE_LOOP_DEPTH[iSt]] == LastSentStoreSched[iSt]);

        /** Rule for reading store ACKs (always enabled). */
        bool succ = false;
        auto stAck = StoreAckPipes::template PipeAt<iSt>::read(succ);
        if (succ) {
          AckStoreAddr[iSt] = stAck.addr;
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            AckStoreSched[iSt][iD] = stAck.sched[iD];
            AckStoreIsLastIter[iSt][iD] = stAck.isLastIter[iD];
          });
        }
        /** End Rule */

        /** Rule shifting store allocation queue. */
        if (!StoreValid[iSt][0]) {
          if (StoreValid[iSt][1]) {
            NextStoreAddr[iSt] = StoreAddr[iSt][1];
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              NextStoreSched[iSt][iD] = StoreSched[iSt][iD][1];
              NextStoreIsLastIter[iSt][iD] = StoreIsLastIter[iSt][iD][1];
            });
          }

          ShiftBundle(StoreValid[iSt], false);
          ShiftBundle(StoreAddr[iSt], 0u);
          UnrolledLoop<LOOP_DEPTH>([&] (auto iD) {
            ShiftBundle(StoreSched[iSt][iD], 0u);
            ShiftBundle(StoreIsLastIter[iSt][iD], false);
          });
        }
        /** End Rule */

        /** Rule for reading store req. */
        if (!StoreValid[iSt][ST_REQ_Q_SIZE - 1]) {
          auto StoreReq = StoreReqPipes::template PipeAt<iSt, 1>::read(
              StoreValid[iSt][ST_REQ_Q_SIZE - 1]);
          StoreAddr[iSt][ST_REQ_Q_SIZE - 1] = StoreReq.addr;
          UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
            StoreSched[iSt][iD][ST_REQ_Q_SIZE - 1] = StoreReq.sched[iD];
            StoreIsLastIter[iSt][iD][ST_REQ_Q_SIZE - 1] = StoreReq.isLastIter[iD];
          });
        }
        /** End Rule */

        /** Rule for reading store value. */
        if (!StoreValuePipeReadValid[iSt]) {
          tagged_val_t<T> valRd = StoreValPipes::template PipeAt<iSt>::read(
              StoreValuePipeReadValid[iSt]);
          NextStoreValue[iSt] = valRd.val;
          NextStoreValueIsValid[iSt] = valRd.valid;
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
        const bool IsSafe = (NoWAW && NoWAR) || !NextStoreValueIsValid[iSt];
        /** End Rule */

        /** Rule for moving st alloc to st commit queue. */
        if (StoreValid[iSt][0] && StoreValuePipeReadValid[iSt] && IsSafe) {
          bool succ = false;
          StorePortValPipes::template PipeAt<iSt>::write(
              {NextStoreValue[iSt], NextStoreValueIsValid[iSt]}, succ);

          if (succ) {
            StoreValid[iSt][0] = false;
            StoreValuePipeReadValid[iSt] = false;
            NextStoreValueIsValid[iSt] = true;
            LastSentStoreSched[iSt] = NextStoreSched[iSt][DI.STORE_LOOP_DEPTH[iSt]];
          }
        }
        /** End Rule */

      }); // End for all stores

    } // end while

  });

  return events;
}

#pragma clang attribute pop

#endif
