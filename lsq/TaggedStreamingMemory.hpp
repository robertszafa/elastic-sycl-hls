#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/ext/intel/ac_types/ac_int.hpp>
#include <sycl/sycl.hpp>

#include "device_print.hpp"
#include "constexpr_math.hpp"
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
struct ld_ack_t {
  addr_t addr;
  sched_t sched[LOOP_DEPTH];
  bool isMaxIter[LOOP_DEPTH];
};

template <int LOOP_DEPTH>
struct load_mux_req_t {
  bool reuse;
  ld_ack_t<LOOP_DEPTH> ack;
};

template <typename T> struct addr_val_t {
  addr_t addr;
  T val;
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
constexpr sched_t SCHED_SENTINEL = (1<<30);

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

  constexpr uint T_BYTES = sizeof(T);
  constexpr uint BURST_SIZE = ((DRAM_BURST_BYTES + T_BYTES - 1) / T_BYTES);
  constexpr uint PIPE_DELAY = 2;

  // Store ports.
  using StorePortReqPipes = PipeArray<class _StorePortReq, addr_val_t<T>, 0, NUM_STORES>;
  UnrolledLoop<NUM_STORES>([&](auto iSt) {
    events[iSt] = q.single_task<StorePortKernel<MEM_ID, iSt>>([=]() KERNEL_PRAGMAS {
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        const auto Req = StorePortReqPipes::template PipeAt<iSt>::read();
        if (Req.addr == STORE_ADDR_SENTINEL) break;
        auto StorePtr = ext::intel::device_ptr<T>(data + Req.addr);
        BurstCoalescedLSU::store(StorePtr, Req.val);
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
  using LoadMuxAckPipes = PipeArray<class _LoadMuxAckPipes, ld_ack_t<LOOP_DEPTH>, BURST_SIZE, NUM_LOADS>;
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

      ld_ack_t<LOOP_DEPTH> maxAck{FINAL_LD_ADDR_ACK};
      InitBundle(maxAck.sched, SCHED_SENTINEL);
      InitBundle(maxAck.isMaxIter, true);
      LoadMuxAckPipes::template PipeAt<iLd>::write(maxAck);
      // PRINTF("** DONE ld%d, reused %d/%d\n", int(iLd), NumReused, NumTotal);
    });
  });

  events[1] = q.single_task<StreamingMemoryKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    /* Store logic registers */
    constexpr uint ST_ALLOC_Q_SIZE = 4;
    bool StoreAllocValid[NUM_STORES][ST_ALLOC_Q_SIZE];
    addr_t StoreAllocAddr[NUM_STORES][ST_ALLOC_Q_SIZE];
    addr_t StoreAllocAddrPlusDelay[NUM_STORES][ST_ALLOC_Q_SIZE];
    sched_t StoreAllocSched[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    bool StoreAllocIsMaxIter[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    // addr_t StoreAllocMaxAddrAtLoop[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    
    // Shortcut for head of StoreAllocQueue
    addr_t NextStoreAddr[NUM_STORES];
    addr_t NextStoreAddrPlusDelay[NUM_STORES];
    sched_t NextStoreSched[NUM_STORES][LOOP_DEPTH];
    bool NextStoreIsMaxIter[NUM_STORES][LOOP_DEPTH];

    bool NextStoreValueValid[NUM_STORES];
    T NextStoreValue[NUM_STORES];

    bool StoreDone[NUM_STORES];

    constexpr uint ST_COMMIT_Q_SIZE = BURST_SIZE + PIPE_DELAY;
    addr_t StoreCommitAddr[NUM_STORES][ST_COMMIT_Q_SIZE];
    T StoreCommitVal[NUM_STORES][ST_COMMIT_Q_SIZE];

    // Init store registers
    UnrolledLoop<NUM_STORES>([&] (auto iSt) {
      InitBundle(StoreAllocValid[iSt], false);
      InitBundle(StoreAllocAddr[iSt], INVALID_ADDR);
      InitBundle(StoreAllocAddrPlusDelay[iSt], INVALID_ADDR);
      UnrolledLoop<LOOP_DEPTH>([&] (auto iD) {
        InitBundle(StoreAllocSched[iSt][iD], 0u);
        InitBundle(StoreAllocIsMaxIter[iSt][iD], false);
      });
      StoreDone[iSt] = false;
      NextStoreValueValid[iSt] = false;
      NextStoreValue[iSt] = T{};

      InitBundle(StoreCommitAddr[iSt], INVALID_ADDR);
      InitBundle(StoreCommitVal[iSt], T{});

      NextStoreAddr[iSt] = INVALID_ADDR;
      NextStoreAddrPlusDelay[iSt] = INVALID_ADDR;
      InitBundle(NextStoreIsMaxIter[iSt], false);
      InitBundle(NextStoreSched[iSt], 0u);
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
    
    bool LoadDone[NUM_LOADS];

    // Init load registers
    UnrolledLoop<NUM_LOADS>([&](auto iLd) {
      InitBundle(LoadValid[iLd], false);
      InitBundle(LoadAddr[iLd], INVALID_ADDR);
      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        InitBundle(LoadPosDepDist[iLd][iSt], false);
      });
      UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
        InitBundle(LoadSched[iLd][iD], 0u);
        InitBundle(SafeLoadSched[iLd][iD], 0u);
        InitBundle(LoadIsMaxIter[iLd][iD], false);
        InitBundle(SafeLoadIsMaxIter[iLd][iD], false);
      });
      LoadDone[iLd] = false;

      LoadAckAddr[iLd] = INVALID_ADDR;
      InitBundle(LoadAckSched[iLd], 1u);
      InitBundle(LoadAckIsMaxIter[iLd], false);

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

      bool loadSchedGreater = (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == FORWARD);
      bool loadSchedEqaul = true;
      if constexpr (cmnLoopDepth >= 0) {
        constexpr int loadWrapFromCmn = walkUpLoadToFirstWrap(iLd, cmnLoopDepth);
        constexpr int eqCheckLoopDepth =
            (loadWrapFromCmn >= 0) ? loadWrapFromCmn : cmnLoopDepth;

        if constexpr (eqCheckLoopDepth >= 0) {
          loadSchedEqaul = (LoadAckSched[iLd][eqCheckLoopDepth] ==
                            NextStoreSched[iSt][eqCheckLoopDepth]);
        }

        if constexpr (DI.LOAD_TO_STORE_DEP_DIR[iLd][iSt] == FORWARD) {
          loadSchedGreater = (NextLoadSched[iLd][cmnLoopDepth] >=
                              NextStoreSched[iSt][cmnLoopDepth]);
        } else {
          loadSchedGreater = (NextLoadSched[iLd][cmnLoopDepth] >
                              NextStoreSched[iSt][cmnLoopDepth]);
        }
      }

      bool ldAddrNoDecr = true;
      UnrolledLoop<loadLoopDepth, std::max(cmnLoopDepth, 0)>([&](auto iD) {
        if constexpr (DI.LOAD_IS_MAX_ITER_NEEDED[iLd][iD]) {
          ldAddrNoDecr &= LoadAckIsMaxIter[iLd][iD];
        }
      });

      return loadSchedGreater || (loadSchedEqaul && ldAddrNoDecr &&
                                  LoadAckAddr[iLd] >= NextStoreAddr[iSt]);
    };

    auto checkNoWAW = [&](const auto iSt, const auto iStOther) {
      constexpr int cmnLoopDepth = DI.COMMON_STORE_LOOP_DEPTH[iSt][iStOther];
      constexpr int otherLoopDepth = DI.STORE_LOOP_DEPTH[iStOther];
      constexpr DEP_DIR depDir = (iSt < iStOther) ? BACK : FORWARD;

      bool otherSchedGreater = (iStOther > iSt);
      bool otherSchedEqual = true;
      if constexpr (cmnLoopDepth >= 0) {
        constexpr int otherWrapFromCmn =
            walkUpStoreToFirstWrap(iStOther, cmnLoopDepth);
        constexpr int eqCheckLoopDepth =
            (otherWrapFromCmn >= 0) ? otherWrapFromCmn : cmnLoopDepth;
        if constexpr (eqCheckLoopDepth >= 0) {
          otherSchedEqual = (NextStoreSched[iStOther][eqCheckLoopDepth] ==
                             NextStoreSched[iSt][eqCheckLoopDepth]);
        }

        if constexpr (depDir == BACK) {
          otherSchedGreater = (NextStoreSched[iStOther][cmnLoopDepth] >=
                               NextStoreSched[iSt][cmnLoopDepth]);
        } else {
          otherSchedGreater = (NextStoreSched[iStOther][cmnLoopDepth] >
                               NextStoreSched[iSt][cmnLoopDepth]);
        }
      }

      bool otherAddrNoDecr = true;
      UnrolledLoop<otherLoopDepth, std::max(cmnLoopDepth, 0)>([&](auto iD) {
        if constexpr (DI.STORE_IS_MAX_ITER_NEEDED[iStOther][iD]) {
          otherAddrNoDecr &= NextStoreIsMaxIter[iStOther][iD];
        }
      });

      const bool otherAddrGreater =
          NextStoreAddr[iStOther] > NextStoreAddrPlusDelay[iSt];

      return otherSchedGreater ||
             (otherSchedEqual && otherAddrNoDecr && otherAddrGreater);
      // StoreAckAddr[iStOther] >= NextStoreAddrPlusBurst[iSt]);
      // StoreAckAddr[iStOther] >= NextStoreAddr[iSt]);
      // TODO: try these:
      // NextStoreAddr[iStOther] > NextStoreAddr[iSt]+2);
      // LastStoreAddr[iStOther] > NextStoreAddr[iSt]);
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
        auto ldAck = LoadMuxAckPipes::template PipeAt<iLd>::read(succ);
        if (succ) {
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
            ld_ack_t<LOOP_DEPTH> MuxAck{SafeLoadAddr[iLd][0]};
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
        StoreDone[iSt] = (NextStoreAddr[iSt] == STORE_ADDR_SENTINEL);

        /** Rule shifting store allocation queue. */
        if (!StoreAllocValid[iSt][0]) {
          if (StoreAllocValid[iSt][1]) {
            NextStoreAddr[iSt] = StoreAllocAddr[iSt][1];
            NextStoreAddrPlusDelay[iSt] = StoreAllocAddrPlusDelay[iSt][1];
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              NextStoreSched[iSt][iD] = StoreAllocSched[iSt][iD][1];
              NextStoreIsMaxIter[iSt][iD] = StoreAllocIsMaxIter[iSt][iD][1];
            });
          }

          ShiftBundle(StoreAllocValid[iSt], false);
          ShiftBundle(StoreAllocAddr[iSt], INVALID_ADDR);
          ShiftBundle(StoreAllocAddrPlusDelay[iSt], INVALID_ADDR);
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
            StoreAllocValid[iSt][ST_ALLOC_Q_SIZE - 1] = true;
            StoreAllocAddr[iSt][ST_ALLOC_Q_SIZE - 1] = StoreReq.addr;
            StoreAllocAddrPlusDelay[iSt][ST_ALLOC_Q_SIZE - 1] =
                StoreReq.addr + RoundUpPow2(PIPE_DELAY);
            UnrolledLoop<LOOP_DEPTH>([&](auto iD) {
              StoreAllocSched[iSt][iD][ST_ALLOC_Q_SIZE-1] = StoreReq.sched[iD];
              StoreAllocIsMaxIter[iSt][iD][ST_ALLOC_Q_SIZE-1] = StoreReq.isMaxIter[iD];
            });
          }
        }
        /** End Rule */

        /** Rule for reading the next store value. */
        if (!NextStoreValueValid[iSt]) {
          NextStoreValue[iSt] = StoreValPipes::template PipeAt<iSt>::read(
              NextStoreValueValid[iSt]);
        }
        /** End Rule */

        /** Rule for moving st alloc to st commit queue. */
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

        if (StoreAllocValid[iSt][0] && NextStoreValueValid[iSt] && 
            NoWAW && NoWAR) {
          bool succ = false;
          StorePortReqPipes::template PipeAt<iSt>::write(
              {NextStoreAddr[iSt], NextStoreValue[iSt]}, succ);
          if (succ) {
            ShiftBundle(StoreCommitVal[iSt], NextStoreValue[iSt]);
            ShiftBundle(StoreCommitAddr[iSt], NextStoreAddr[iSt]);

            StoreAllocValid[iSt][0] = false;
            NextStoreValueValid[iSt] = false;

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

    // Terminate store port.
    UnrolledLoop<NUM_STORES>([&](auto iSt) {
      StorePortReqPipes::template PipeAt<iSt>::write({STORE_ADDR_SENTINEL});
    });

    // PRINTF("** DONE StreamingMemory %d\n", MEM_ID);

  });

  return events;
}