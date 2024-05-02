#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/ext/intel/ac_types/ac_int.hpp>
#include <sycl/sycl.hpp>

#include "device_print.hpp"
#include "constexpr_math.hpp"
#include "pipe_utils.hpp"
#include "unrolled_loop.hpp"

#include "dependencies.hpp"

using namespace sycl;
using namespace fpga_tools;

template <int iLd, int iSt>
inline bool inSameLoop(const uint (&LoadSched)[NUM_LOADS][LOOP_DEPTH],
                       const uint (&StoreSched)[NUM_LOADS][LOOP_DEPTH]) {
  if constexpr (COMMON_LOOP_DEPTH[iLd][iSt] == 0) {
    return true;
  }

  return (StoreSched[iSt][COMMON_LOOP_DEPTH[iLd][iSt] - 1] >=
          LoadSched[iLd][COMMON_LOOP_DEPTH[iLd][iSt] - 1]);
  // return (StoreSched[iSt][COMMON_LOOP_DEPTH[iLd][iSt] - 1] ==
  //         LoadSched[iLd][COMMON_LOOP_DEPTH[iLd][iSt] - 1]);
}

template <int iSt, int start> 
constexpr int walkUpStoreToFirstWrap() {
  for (int i = start - 1; i >= 0; --i) {
    if (STORE_IS_MAX_ITER_NEEDED[iSt][i]) {
      return i;
    }
  }

  return start;
}
template <int iLd, int start> 
constexpr int walkUpLoadToFirstWrap() {
  for (int i = start - 1; i >= 0; --i) {
    if (LOAD_IS_MAX_ITER_NEEDED[iLd][i]) {
      return i;
    }
  }

  return start;
}

template <int iLd, int iSt>
inline bool storeHasMinSched(
    const uint (&LoadSched)[NUM_LOADS][LOOP_DEPTH],
    const uint (&StoreSched)[NUM_LOADS][LOOP_DEPTH],
    const bool (&StoreIsMaxIter)[NUM_STORES][LOOP_DEPTH]) {
  constexpr int depthForMinIter =
      walkUpStoreToFirstWrap<iSt, COMMON_LOOP_DEPTH[iLd][iSt]>();

  // PRINTF("ld%d st%d depthForMinIter=%d\n", int(iLd), int(iSt),
  // depthForMinIter)

  bool hasMinSched = true;
  if constexpr (depthForMinIter >= 0) {
    bool storeSchedEqual = true, storeSchedGreater = true;
    if constexpr (LOAD_TO_STORE_DEP_DIR[iLd][iSt] == BACK) {
      storeSchedEqual = ((StoreSched[iSt][depthForMinIter] + 1) ==
                         LoadSched[iLd][depthForMinIter]);
      storeSchedGreater = ((StoreSched[iSt][depthForMinIter] + 1) >
                           LoadSched[iLd][depthForMinIter]);
    } else {
      storeSchedEqual =
          (StoreSched[iSt][depthForMinIter] == LoadSched[iLd][depthForMinIter]);
      storeSchedGreater =
          (StoreSched[iSt][depthForMinIter] > LoadSched[iLd][depthForMinIter]);
    }

    // For every loop between STORE_LOOP_DEPTH[iSt] and common loop depth
    // By definition of walkUpToFirstWrap, no maxIter required between
    // COMMON_LOOP_DEPTH[iLd][iSt] and kDepthForMinIter.
    bool maxIterSatisfied = true;
    UnrolledLoop<STORE_LOOP_DEPTH[iSt], COMMON_LOOP_DEPTH[iLd][iSt]>(
        [&](auto iD) {
          if constexpr (STORE_IS_MAX_ITER_NEEDED[iSt][iD]) {
            maxIterSatisfied &= StoreIsMaxIter[iSt][iD];
          }
        });

    hasMinSched = storeSchedGreater || (storeSchedEqual && maxIterSatisfied);
  }

  return hasMinSched;
}

template <int iLd, int iSt>
inline bool loadHasMinSched(
    const uint (&LoadSched)[NUM_LOADS][LOOP_DEPTH],
    const uint (&StoreSched)[NUM_LOADS][LOOP_DEPTH],
    const bool (&LoadIsMaxIter)[NUM_STORES][LOOP_DEPTH]) {
  constexpr int depthForMinIter =
      walkUpLoadToFirstWrap<iLd, COMMON_LOOP_DEPTH[iLd][iSt]>();
  
  // PRINTF("depthForMinIter = %d\n", depthForMinIter);

  bool hasMinSched = true;
  if constexpr (depthForMinIter >= 0) {
    bool loadSchedEqual = true, loadSchedGreater = true;
    if constexpr (LOAD_TO_STORE_DEP_DIR[iLd][iSt] == FORWARD) {
      loadSchedEqual = ((LoadSched[iLd][depthForMinIter] + 1) ==
                         StoreSched[iSt][depthForMinIter]);
      loadSchedGreater = ((LoadSched[iLd][depthForMinIter] + 1) >
                           StoreSched[iSt][depthForMinIter]);
    } else {
      loadSchedEqual =
          (StoreSched[iSt][depthForMinIter] == LoadSched[iLd][depthForMinIter]);
      loadSchedGreater =
          (StoreSched[iSt][depthForMinIter] < LoadSched[iLd][depthForMinIter]);
    }

    bool maxIterSatisfied = true;
    UnrolledLoop<LOAD_LOOP_DEPTH[iLd], COMMON_LOOP_DEPTH[iLd][iSt]>(
        [&](auto iD) {
          if constexpr (LOAD_IS_MAX_ITER_NEEDED[iLd][iD]) {
            maxIterSatisfied &= LoadIsMaxIter[iLd][iD];
          }
        });

    hasMinSched = loadSchedGreater || (loadSchedEqual && maxIterSatisfied);
  }

  return hasMinSched;
}

using BurstCoalescedLSU = ext::intel::lsu<ext::intel::burst_coalesce<true>,
                                          ext::intel::prefetch<false>>;
using PipelinedLSU = sycl::ext::intel::lsu<>;

/// A gated StreamingLoad receives {addr, value, schedule} data on its gate,
/// and stores {addr, value} in a shift-register FIFO for reuse.
#define KERNEL_PRAGMAS [[intel::kernel_args_restrict]] [[intel::max_global_work_dim(0)]] 

constexpr uint DRAM_BURST_BYTES = 32;
constexpr int MAX_INT = (1<<30);
constexpr uint MAX_UINT = (1<<30);
constexpr int INVALID_ADDR = -1;

using addr_t = int;
using tag_t = uint;
using seq_t = uint;

template <int LOOP_DEPTH>
struct store_req_t {
  addr_t addr;
  seq_t sched[LOOP_DEPTH];
  bool isMaxIter[LOOP_DEPTH];
};
template <int NUM_STORES, int LOOP_DEPTH>
struct load_req_t {
  addr_t addr;
  seq_t sched[LOOP_DEPTH];
  bool isMaxIter[LOOP_DEPTH];
  bool posDepDist[NUM_STORES];
};

template <int LOOP_DEPTH>
struct load_ack_t {
  addr_t addr;
  seq_t sched[LOOP_DEPTH];
  bool isMaxIter[LOOP_DEPTH];
};
template <int LOOP_DEPTH>
struct load_mux_req_t {
  bool reuse;
  load_ack_t<LOOP_DEPTH> ack;
};

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
          uint NUM_LOADS, uint NUM_STORES, uint LOOP_DEPTH, typename T>
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
  using LoadMuxPredPipes = PipeArray<class _LoadMuxPred, load_mux_req_t<LOOP_DEPTH>, BURST_SIZE, NUM_LOADS>;
  using LoadMuxAckPipes = PipeArray<class _LoadMuxAckPipes, load_ack_t<LOOP_DEPTH>, BURST_SIZE, NUM_LOADS>;
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
      // PRINTF("** DONE ld port %d\n", int(iLd));
    });
    q.single_task<LoadValMuxKernel<MEM_ID, iLd>>([=]() KERNEL_PRAGMAS {
      [[maybe_unused]] int NumTotal = 0, NumReused = 0;
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      while (true) {
        T Val;
        const auto MuxReq = LoadMuxPredPipes::template PipeAt<iLd>::read();
        if (MuxReq.ack.addr == MAX_INT) break;

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

      load_ack_t<LOOP_DEPTH> maxAck{MAX_INT};
      InitBundle(maxAck.sched, MAX_UINT);
      InitBundle(maxAck.isMaxIter, true);
      LoadMuxAckPipes::template PipeAt<iLd>::write(maxAck);
      PRINTF("** DONE ld%d, reused %d/%d\n", int(iLd), NumReused, NumTotal);
    });
  });

  events[1] = q.single_task<StreamingMemoryKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    constexpr uint ST_ALLOC_Q_SIZE = 2;
    bool StoreAllocValid[NUM_STORES][ST_ALLOC_Q_SIZE];
    addr_t StoreAllocAddr[NUM_STORES][ST_ALLOC_Q_SIZE];
    seq_t StoreAllocSched[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    bool StoreAllocIsMaxIter[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    // addr_t StoreAllocMaxAddrAtLoop[NUM_STORES][LOOP_DEPTH][ST_ALLOC_Q_SIZE];
    
    constexpr uint ST_COMMIT_Q_SIZE = BURST_SIZE;
    addr_t StoreCommitAddr[NUM_STORES][ST_COMMIT_Q_SIZE];
    seq_t StoreCommitSched[NUM_STORES][LOOP_DEPTH][ST_COMMIT_Q_SIZE];
    bool StoreCommitIsMaxIter[NUM_STORES][LOOP_DEPTH][ST_COMMIT_Q_SIZE];
    T StoreCommitVal[NUM_STORES][ST_COMMIT_Q_SIZE];

    addr_t NextStoreAddr[NUM_STORES];
    seq_t NextStoreSched[NUM_STORES][LOOP_DEPTH];
    bool NextStoreIsMaxIter[NUM_STORES][LOOP_DEPTH];

    addr_t LastStoreAddr[NUM_STORES];
    seq_t LastStoreSched[NUM_STORES][LOOP_DEPTH];
    bool LastStoreIsMaxIter[NUM_STORES][LOOP_DEPTH];

    // Init store registers
    UnrolledLoop<NUM_STORES>([&] (auto iSt) {
      InitBundle(StoreAllocValid[iSt], false);
      InitBundle(StoreAllocAddr[iSt], INVALID_ADDR);
      UnrolledLoop<LOOP_DEPTH>([&] (auto iIter) {
        InitBundle(StoreAllocSched[iSt][iIter], 0u);
        InitBundle(StoreAllocIsMaxIter[iSt][iIter], false);
        InitBundle(StoreCommitSched[iSt][iIter], 0u);
        InitBundle(StoreCommitIsMaxIter[iSt][iIter], false);
      });

      InitBundle(StoreCommitAddr[iSt], INVALID_ADDR);
      InitBundle(StoreCommitVal[iSt], T{});

      NextStoreAddr[iSt] = INVALID_ADDR;
      InitBundle(NextStoreSched[iSt], 0u);
      InitBundle(NextStoreIsMaxIter[iSt], false);

      LastStoreAddr[iSt] = INVALID_ADDR;
      InitBundle(LastStoreSched[iSt], 0u);
      InitBundle(LastStoreIsMaxIter[iSt], false);
    });

    constexpr uint LD_Q_SIZE = 4;
    bool LoadValid[NUM_LOADS][LD_Q_SIZE];
    addr_t LoadAddr[NUM_LOADS][LD_Q_SIZE];
    seq_t LoadSched[NUM_LOADS][LOOP_DEPTH][LD_Q_SIZE];
    bool LoadIsMaxIter[NUM_LOADS][LOOP_DEPTH][LD_Q_SIZE];
    bool LoadPosDepDist[NUM_LOADS][NUM_STORES][LD_Q_SIZE];
    // addr_t LoadMaxAddrAtLoop[NUM_LOADS][LOOP_DEPTH][LD_Q_SIZE];

    addr_t LoadAckAddr[NUM_LOADS];
    seq_t LoadAckSched[NUM_LOADS][LOOP_DEPTH];
    bool LoadAckIsMaxIter[NUM_LOADS][LOOP_DEPTH];

    // Alias to load_q position 0.
    addr_t NextLoadAddr[NUM_LOADS];
    seq_t NextLoadSched[NUM_LOADS][LOOP_DEPTH];
    bool NextLoadIsMaxIter[NUM_LOADS][LOOP_DEPTH];
    bool NextLoadPosDepDist[NUM_LOADS][NUM_STORES];

    // Once a load allocation is deemed safe to execute/reuse, it is moved to
    // this stage to issue load request or send reuse value.
    bool SafeLoadValid[NUM_LOADS][LD_Q_SIZE];
    addr_t SafeLoadAddr[NUM_LOADS][LD_Q_SIZE];
    seq_t SafeLoadSched[NUM_LOADS][LOOP_DEPTH][LD_Q_SIZE];
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
      UnrolledLoop<LOOP_DEPTH>([&](auto iIter) {
        InitBundle(LoadSched[iLd][iIter], 0u);
        InitBundle(LoadIsMaxIter[iLd][iIter], false);
        InitBundle(SafeLoadSched[iLd][iIter], 0u);
        InitBundle(SafeLoadIsMaxIter[iLd][iIter], false);
      });

      LoadAckAddr[iLd] = INVALID_ADDR;
      InitBundle(LoadAckSched[iLd], 0u);
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
        if (NextStoreAddr[iSt] != MAX_INT)
          AnyStoresLeft = true;
      });
      AnyLoadsLeft = false;
      UnrolledLoop<NUM_LOADS>([&](auto iLd) {
        if (LoadAckAddr[iLd] != MAX_INT)
          AnyLoadsLeft = true;
      });
      /** End Rule */

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////     LOAD LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////
      UnrolledLoop<NUM_LOADS>([&](auto iLd) {
        bool succ = false;
        auto ldAck = LoadMuxAckPipes::template PipeAt<iLd>::read(succ);
        if (succ) {
          LoadAckAddr[iLd] = ldAck.addr;
          // PRINTF("ld%d ack %d (%d, %d, %d) (%d, %d, %d)\n", int(iLd),
          //        ldAck.addr, ldAck.sched[0], ldAck.sched[1], ldAck.sched[2],
          //        ldAck.isMaxIter[0], ldAck.isMaxIter[1], ldAck.isMaxIter[2]);
          UnrolledLoop<LOOP_DEPTH>([&](auto iIter) {
            LoadAckSched[iLd][iIter] = ldAck.sched[iIter];
            LoadAckIsMaxIter[iLd][iIter] = ldAck.isMaxIter[iIter];
          });
        }

        /** Rule for shifting load queues. */ 
        if (!LoadValid[iLd][0]) {
          if (LoadValid[iLd][1]) {
            NextLoadAddr[iLd] = LoadAddr[iLd][1];
            UnrolledLoop<LOOP_DEPTH>([&](auto iIter) {
              NextLoadSched[iLd][iIter] = LoadSched[iLd][iIter][1];
              NextLoadIsMaxIter[iLd][iIter] = LoadIsMaxIter[iLd][iIter][1];
            });
            UnrolledLoop<NUM_STORES>([&](auto iSt) {
              NextLoadPosDepDist[iLd][iSt] = LoadPosDepDist[iLd][iSt][1];
            });
          }

          ShiftBundle(LoadValid[iLd], false);
          ShiftBundle(LoadAddr[iLd], INVALID_ADDR);
          UnrolledLoop<LOOP_DEPTH>([&](auto iIter) {
            ShiftBundle(LoadSched[iLd][iIter], 0u);
            ShiftBundle(LoadIsMaxIter[iLd][iIter], false);
          });
          UnrolledLoop<NUM_STORES>(
              [&](auto iSt) { ShiftBundle(LoadPosDepDist[iLd][iSt], false); });
        }

        if (!SafeLoadValid[iLd][0]) {
          ShiftBundle(SafeLoadValid[iLd], false);
          ShiftBundle(SafeLoadAddr[iLd], INVALID_ADDR);
          UnrolledLoop<LOOP_DEPTH>([&](auto iIter) {
            ShiftBundle(SafeLoadSched[iLd][iIter], 0u);
            ShiftBundle(SafeLoadIsMaxIter[iLd][iIter], false);
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
            load_ack_t<LOOP_DEPTH> MuxAck{SafeLoadAddr[iLd][0]};
            UnrolledLoop<LOOP_DEPTH>([&](auto iIter) {
              MuxAck.sched[iIter] = SafeLoadSched[iLd][iIter][0];
              MuxAck.isMaxIter[iIter] = SafeLoadIsMaxIter[iLd][iIter][0];
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
            UnrolledLoop<LOOP_DEPTH>([&](auto iIter) {
              LoadSched[iLd][iIter][LD_Q_SIZE - 1] = LoadReq.sched[iIter];
              LoadIsMaxIter[iLd][iIter][LD_Q_SIZE - 1] = LoadReq.isMaxIter[iIter];
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
          seq_t ThisReuseSched[NUM_STORES][LOOP_DEPTH];
          bool ThisReuseIsMaxIter[NUM_STORES][LOOP_DEPTH];
          T ThisReuseVal[NUM_STORES];
          bool NoRAW = true;
          UnrolledLoop<NUM_STORES>([&](auto iSt) {
            // Load is before store if dep is 'back' and and no common loop.
            bool LoadIsBeforeStore = (LOAD_TO_STORE_DEP_DIR[iLd][iSt] == BACK);
            if constexpr (COMMON_LOOP_DEPTH[iLd][iSt] >= 0) {
              // If common loop then compare the iteration of common the common 
              // loop. Use ">=" if ld before st in the common loop, else ">".
              if constexpr (LOAD_TO_STORE_DEP_DIR[iLd][iSt] == BACK) {
                LoadIsBeforeStore =
                    (NextStoreSched[iSt][COMMON_LOOP_DEPTH[iLd][iSt]] >=
                     NextLoadSched[iLd][COMMON_LOOP_DEPTH[iLd][iSt]]);
              } else {
                LoadIsBeforeStore =
                    (NextStoreSched[iSt][COMMON_LOOP_DEPTH[iLd][iSt]] >
                     NextLoadSched[iLd][COMMON_LOOP_DEPTH[iLd][iSt]]);
              }
            }

            // For the min sched, start at the common loop depth and then walk
            // up the schedule tree of the store until you hit the first loop
            // where the loop whose iterator does not monotonically increase the
            // value of the address expression. 
            // (kDepthForMinIter can be equal to COMMON_LOOP_DEPTH[iLd][iSt])
            bool minSchedSatisfied = storeHasMinSched<iLd, iSt>(
                NextLoadSched, NextStoreSched, NextStoreIsMaxIter);
                // NextLoadSched, LastStoreSched, LastStoreIsMaxIter);

            bool LoadHasPosDepDistance = false;
            if constexpr (ARE_IN_SAME_LOOP[iLd][iSt]) {
              bool areInSameLoop =
                  inSameLoop<iLd, iSt>(NextLoadSched, NextStoreSched);
              LoadHasPosDepDistance = areInSameLoop && NextLoadPosDepDist[iLd];
            }

            bool StoreHasLargerAddress = NextStoreAddr[iSt] > NextLoadAddr[iLd];
            // No st->ld dependency exists if:
            NoRAW &= (LoadIsBeforeStore || LoadHasPosDepDistance ||
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
            [[maybe_unused]] int ReuseId = 0;
            UnrolledLoop<NUM_STORES>([&](auto iSt) {
              if constexpr (LOAD_TO_STORE_DEP_DIR[iLd][iSt] == BACK) {
                if (ThisReuse[iSt]) {
                  bool minSchedSatisfied = storeHasMinSched<iLd, iSt>(
                      NextLoadSched, ThisReuseSched, ThisReuseIsMaxIter);

                  if (minSchedSatisfied) {
                    Reuse = true;
                    ReuseVal = ThisReuseVal[iSt];
                    ReuseId = iSt;
                  }
                }
              }
            });
            // Forward deps have precedence.
            UnrolledLoop<NUM_STORES>([&](auto iSt) {
              if constexpr (LOAD_TO_STORE_DEP_DIR[iLd][iSt] == FORWARD) {
                if (ThisReuse[iSt]) {
                  bool minSchedSatisfied = storeHasMinSched<iLd, iSt>(
                      NextLoadSched, ThisReuseSched, ThisReuseIsMaxIter);

                  if (minSchedSatisfied) {
                    Reuse = true;
                    ReuseVal = ThisReuseVal[iSt];
                    ReuseId = iSt;
                  }
                }
              }
            });

            // if constexpr (iLd == 1) {
            //   PRINTF("Load%d addr %d (%d, %d, %d) reuse=%d from st%d   st0(%d, %d, %d)   st1(%d, %d, %d)\n"
            //          "Next st0 (%d, %d, %d), Next st1 (%d, %d, %d)\n"
            //          "Last st0 (%d, %d, %d), Last st1 (%d, %d, %d)\n\n",
            //          int(iLd), NextLoadAddr[iLd], NextLoadSched[iLd][0],
            //          NextLoadSched[iLd][1], NextLoadSched[iLd][2], Reuse, ReuseId,
            //          ThisReuseSched[0][0], ThisReuseSched[0][1], ThisReuseSched[0][2], 
            //          ThisReuseSched[1][0], ThisReuseSched[1][1], ThisReuseSched[1][2],
            //          NextStoreSched[0][0], NextStoreSched[0][1], NextStoreSched[0][2], 
            //          NextStoreSched[1][0], NextStoreSched[1][1], NextStoreSched[1][2],
            //          LastStoreSched[0][0], LastStoreSched[0][1], LastStoreSched[0][2], 
            //          LastStoreSched[1][0], LastStoreSched[1][1], LastStoreSched[1][2]
            //          );
            // }

            SafeLoadValid[iLd][LD_Q_SIZE - 1] = true;
            SafeLoadAddr[iLd][LD_Q_SIZE - 1] = LoadAddr[iLd][0];
            UnrolledLoop<LOOP_DEPTH>([&](auto iIter) {
              SafeLoadSched[iLd][iIter][LD_Q_SIZE-1] = NextLoadSched[iLd][iIter];
              SafeLoadIsMaxIter[iLd][iIter][LD_Q_SIZE-1] = NextLoadIsMaxIter[iLd][iIter];
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
        /** Rule shifting store allocation queue. */
        if (!StoreAllocValid[iSt][0]) {
          if (StoreAllocValid[iSt][1]) {
            NextStoreAddr[iSt] = StoreAllocAddr[iSt][1];
            // PRINTF("NextStoreAddr[%d] is %d\n", int(iSt), NextStoreAddr[iSt]);
            UnrolledLoop<LOOP_DEPTH>([&](auto iIter) {
              NextStoreSched[iSt][iIter] = StoreAllocSched[iSt][iIter][1];
              NextStoreIsMaxIter[iSt][iIter] = StoreAllocIsMaxIter[iSt][iIter][1];
            });
          }

          ShiftBundle(StoreAllocValid[iSt], false);
          ShiftBundle(StoreAllocAddr[iSt], INVALID_ADDR);
          UnrolledLoop<LOOP_DEPTH>([&] (auto iIter) {
            ShiftBundle(StoreAllocSched[iSt][iIter], 0u);
            ShiftBundle(StoreAllocIsMaxIter[iSt][iIter], false);
          });
        }
        /** End Rule */

        /** Rule for reading store {addr, tag} pairs. */
        if (!StoreAllocValid[iSt][ST_ALLOC_Q_SIZE - 1]) {
          bool succ = false;
          const store_req_t StoreReq =
              StoreAddrPipes::template PipeAt<iSt>::read(succ);
          if (succ) {
            StorePortAddrPipes:: template PipeAt<iSt>::write(StoreReq.addr);

            StoreAllocValid[iSt][ST_ALLOC_Q_SIZE - 1] = true;
            StoreAllocAddr[iSt][ST_ALLOC_Q_SIZE - 1] = StoreReq.addr;
            UnrolledLoop<LOOP_DEPTH>([&](auto iIter) {
              StoreAllocSched[iSt][iIter][ST_ALLOC_Q_SIZE - 1] =
                  StoreReq.sched[iIter];
              StoreAllocIsMaxIter[iSt][iIter][ST_ALLOC_Q_SIZE - 1] =
                  StoreReq.isMaxIter[iIter];
            });
          }
        }
        /** End Rule */

        /** Rule for getting st val and moving st alloc to st commit queue. */
        bool NoWAW = true;
        // UnrolledLoop<NUM_STORES>([&](auto iStOther) {
        //   if constexpr (iStOther != iSt) {
        //     if ((StoreAllocMinLoopTagStores[iSt][iStOther][0] >
        //          NextStoreLoopTag[iStOther]) ||
        //         (NextStoreTag[iSt] > NextStoreTag[iStOther] &&
        //          NextStoreAddr[iSt] >= NextStoreAddr[iStOther])) {
        //       NoWAW = false;
        //     }
        //   }
        // });

        bool NoWAR = true;
        UnrolledLoop<NUM_LOADS>([&](auto iLd) {
          bool minLdSchedSatisfied = loadHasMinSched<iLd, iSt>(
              LoadAckSched, NextStoreSched, LoadAckIsMaxIter);
              // NextLoadSched, NextStoreSched, NextLoadIsMaxIter);

          constexpr auto commonIter = COMMON_LOOP_DEPTH[iLd][iSt];
          if (!minLdSchedSatisfied ||
              (NextStoreSched[iSt][commonIter] >= LoadAckSched[iLd][commonIter] &&
               NextStoreAddr[iSt] > LoadAckAddr[iLd])) {
            NoWAR = false;
          }
        });

        if (StoreAllocValid[iSt][0] && NoWAW && NoWAR) {
          bool succ = false;
          const T StoreVal = StoreValPipes::template PipeAt<iSt>::read(succ);
          if (succ) {
            // PRINTF("NoWAR for st %d (%d, %d),   LoadAck %d (%d, %d, %d)\n",
            //        NextStoreAddr[0], NextStoreSched[0][0], NextStoreSched[0][1],
            //        LoadAckAddr[0], LoadAckSched[0][0], LoadAckSched[0][1],
            //        LoadAckSched[0][2]);

            StorePortValPipes::template PipeAt<iSt>::write(StoreVal);

            LastStoreAddr[iSt] = StoreAllocAddr[iSt][0];
            UnrolledLoop<LOOP_DEPTH>([&](auto iIter) {
              LastStoreSched[iSt][iIter] = StoreAllocSched[iSt][iIter][0];
              LastStoreIsMaxIter[iSt][iIter] = StoreAllocIsMaxIter[iSt][iIter][0];

              ShiftBundle(StoreCommitSched[iSt][iIter],
                          StoreAllocSched[iSt][iIter][0]);
              ShiftBundle(StoreCommitIsMaxIter[iSt][iIter],
                          StoreAllocIsMaxIter[iSt][iIter][0]);
            });
            ShiftBundle(StoreCommitVal[iSt], StoreVal);
            if (StoreAllocAddr[iSt][0] == MAX_INT) {
              ShiftBundle(StoreCommitAddr[iSt], INVALID_ADDR);
            } else {
              ShiftBundle(StoreCommitAddr[iSt], StoreAllocAddr[iSt][0]);
            }

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

    PRINTF("** DONE StreamingMemory %d\n", MEM_ID);

  });

  return events;
}
