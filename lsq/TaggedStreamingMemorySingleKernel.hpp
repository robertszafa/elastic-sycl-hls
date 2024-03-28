#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/ext/intel/ac_types/ac_int.hpp>
#include <sycl/sycl.hpp>

#include "device_print.hpp"
#include "pipe_utils.hpp"
#include "unrolled_loop.hpp"

using namespace sycl;
using namespace fpga_tools;

using BurstCoalescedLSU = ext::intel::lsu<ext::intel::burst_coalesce<true>,
                                          ext::intel::prefetch<false>>;
using PipelinedLSU = sycl::ext::intel::lsu<>;

/// A gated StreamingLoad receives {addr, value, schedule} data on its gate,
/// and stores {addr, value} in a shift-register FIFO for reuse.
#define KERNEL_PRAGMAS [[intel::kernel_args_restrict]] [[intel::max_global_work_dim(0)]]

constexpr uint DRAM_BURST_BITS = 512;
constexpr uint MAX_INT = (1 << 30);
constexpr int INVALID_ADDR = -1;
// Types for specifying action in load mux.
using ld_mux_pred_t = ac_int<2, false>;
constexpr ld_mux_pred_t LD_MUX_TERMINATE = ld_mux_pred_t{0};
constexpr ld_mux_pred_t LD_MUX_REUSE = ld_mux_pred_t{1};
constexpr ld_mux_pred_t LD_MUX_LOAD = ld_mux_pred_t{2};

struct addr_t {
  int addr;
};

struct addr_tag_t {
  int addr;
  uint tag;
};

template <int NUM_STORES> struct addr_tag_mintag_t {
  int addr;
  uint tag;
  /// Minimum tag that a store dependency has to have.
  uint mintag[NUM_STORES];
};


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

/// Unique kernel name generators.
template <int MemId> class StreamingMemoryKernel;
template <int MemId, int PortId> class LoadPortKernel;
template <int MemId, int PortId> class StorePortKernel;
template <int MemId, int PortId> class LoadValMuxKernel;

template <int MEM_ID, typename EndSignalPipe, typename LoadAddrPipes,
          typename LoadValPipes, typename StoreAddrPipes, typename StoreValPipes,
          typename StorePortAddrPipes, typename StorePortValPipes,
          uint NUM_LOADS, uint NUM_STORES, uint BIT_WIDTH = 32, typename T>
std::vector<event> StreamingMemory(queue &q, T *data) {
  std::vector<event> events(NUM_STORES);

  assert(sizeof(T) * 8 == BIT_WIDTH && "Incorrect kBitWidth.");
  constexpr uint BURST_SIZE = (DRAM_BURST_BITS + BIT_WIDTH - 1) / BIT_WIDTH;

  // Store port.
  using StorePortAckPipes = PipeArray<class _StAck, addr_tag_t, 2, NUM_STORES>;
  UnrolledLoop<NUM_STORES>([&](auto iSt) {
    events[iSt] = q.single_task<StorePortKernel<MEM_ID, iSt>>([=]() KERNEL_PRAGMAS {
      // Only send ack once {BusrtSize} amount of elements have been stored.
      bool AckValid[BURST_SIZE];
      int AckAddr[BURST_SIZE];
      uint AckTag[BURST_SIZE];
      InitBundle(AckValid, false);
      InitBundle(AckAddr, INVALID_ADDR);
      InitBundle(AckTag, 0u);

      addr_tag_t Req = addr_tag_t{};
      T Val = T{};
      bool ReqValid = false;
      bool ValValid = false;

      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      [[intel::speculated_iterations(0)]]
      while (true) {
        if (!ReqValid) {
          auto tryReq = StorePortAddrPipes::template PipeAt<iSt>::read(ReqValid);
          if (ReqValid) Req = tryReq;
        }
        if (Req.addr == MAX_INT) break;
        
        if (!ValValid) {
          auto tryVal = StorePortValPipes::template PipeAt<iSt>::read(ValValid);
          if (ValValid) Val = tryVal;
        }

        bool NextAckValid = false;
        if (ReqValid && ValValid) {
          auto StorePtr = ext::intel::device_ptr<T>(data + Req.addr);
          BurstCoalescedLSU::store(StorePtr, Val);
          ReqValid = false;
          ValValid = false;
          NextAckValid = true;
        }

        if (AckValid[0])
          StorePortAckPipes::template PipeAt<iSt>::write({AckAddr[0], AckTag[0]});
        ShiftBundle(AckValid, NextAckValid);
        ShiftBundle(AckAddr, Req.addr);
        ShiftBundle(AckTag, Req.tag);
      }
      // Drain acks
      atomic_fence(memory_order_seq_cst, memory_scope_work_item); // force burst
      for (int i = 0; i < BURST_SIZE; ++i) {
        if (AckValid[i])
          StorePortAckPipes::template PipeAt<iSt>::write({AckAddr[i], AckTag[i]});
      }
      // StorePortAckPipes::template PipeAt<iSt>::write({MAX_INT, MAX_INT});
    });
  });


  // Pipes for load port and load mux.
  using LoadPortAddrPipes =
      PipeArray<class _LoadAddr, uint, BURST_SIZE * 4, NUM_LOADS>;
  using LoadMuxLoadValPipes =
      PipeArray<class _LoadMuxMemoryVal, T, BURST_SIZE, NUM_LOADS>;
  using LoadMuxPredPipes =
      PipeArray<class _LoadMuxPred, ld_mux_pred_t, BURST_SIZE, NUM_LOADS>;
  using LoadMuxReuseValPipes =
      PipeArray<class _LoadMuxReuseVal, T, BURST_SIZE, NUM_LOADS>;
  UnrolledLoop<NUM_LOADS>([&](auto iLd) {
    q.single_task<LoadPortKernel<MEM_ID, iLd>>([=]() KERNEL_PRAGMAS {
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      [[intel::speculated_iterations(0)]]
      while (true) {
        auto Addr = LoadPortAddrPipes::template PipeAt<iLd>::read();
        if (Addr == MAX_INT) break;
        auto LoadPtr = ext::intel::device_ptr<T>(data + Addr);
        auto Val = BurstCoalescedLSU::load(LoadPtr);
        LoadMuxLoadValPipes::template PipeAt<iLd>::write(Val);
      }
    });
    q.single_task<LoadValMuxKernel<MEM_ID, iLd>>([=]() KERNEL_PRAGMAS {
      int NumTotal = 0;
      int NumReused = 0;
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      [[intel::speculated_iterations(0)]]
      while (true) {
        T Val;
        ld_mux_pred_t Pred = LoadMuxPredPipes::template PipeAt<iLd>::read();
        if (Pred == LD_MUX_REUSE) {
          NumReused++;
          Val = LoadMuxReuseValPipes::template PipeAt<iLd>::read();
        } else if (Pred == LD_MUX_LOAD) {
          Val = LoadMuxLoadValPipes::template PipeAt<iLd>::read();
        } else {
          break;
        }

        // PRINTF("Returned load num %d\n", NumTotal);
        LoadValPipes::template PipeAt<iLd>::write(Val);
        NumTotal++;
      }
      PRINTF("** DONE load MUX, reused %d/%d\n", NumReused, NumTotal);
    });
  });

  q.single_task<StreamingMemoryKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    constexpr uint ST_Q_SIZE = 2;
    bool StoreAllocValid[NUM_STORES][ST_Q_SIZE];
    int StoreAllocAddr[NUM_STORES][ST_Q_SIZE];
    uint StoreAllocTag[NUM_STORES][ST_Q_SIZE];

    int NextStoreAddr[NUM_STORES];
    uint NextStoreTag[NUM_STORES];

    int LastStoreAddr[NUM_STORES];
    uint LastStoreTag[NUM_STORES];
    T LastStoreVal[NUM_STORES];

    int StoreAckAddr[NUM_STORES];
    uint StoreAckTag[NUM_STORES];

    // Init store registers
    UnrolledLoop<NUM_STORES>([&] (auto iSt) {
      InitBundle(StoreAllocValid[iSt], false);
      InitBundle(StoreAllocAddr[iSt], INVALID_ADDR);
      InitBundle(StoreAllocTag[iSt], 0u);

      NextStoreAddr[iSt] = INVALID_ADDR;
      NextStoreTag[iSt] = 0u;

      LastStoreAddr[iSt] = INVALID_ADDR;
      LastStoreTag[iSt] = 0u;
      LastStoreVal[iSt] = T{};

      StoreAckAddr[iSt] = INVALID_ADDR;
      StoreAckTag[iSt] = 0u;
    });

    bool LoadAllocValid[NUM_LOADS];
    int LoadAllocAddr[NUM_LOADS];
    uint LoadAllocTag[NUM_LOADS];
    uint LoadAllocMinTag[NUM_LOADS][NUM_STORES];
    InitBundle(LoadAllocValid, false);
    InitBundle(LoadAllocAddr, INVALID_ADDR);
    InitBundle(LoadAllocTag, 0u);
    UnrolledLoop<NUM_LOADS>(
        [&](auto iLd) { InitBundle(LoadAllocMinTag[iLd], 0u); });

    bool LoadExecSafeToLoad[NUM_LOADS];
    bool LoadExecReuse[NUM_LOADS];
    uint LoadExecReuseTag[NUM_LOADS];
    T LoadExecReuseVal[NUM_LOADS];
    InitBundle(LoadExecSafeToLoad, false);
    InitBundle(LoadExecReuse, false);
    InitBundle(LoadExecReuseTag, 0u);
    InitBundle(LoadExecReuseVal, T{});

    bool EndSignal = false;

    [[maybe_unused]] uint cycle = 0;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (!EndSignal) {
      cycle++;
      EndSignalPipe::read(EndSignal);

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////     LOAD LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////

      UnrolledLoop<NUM_LOADS>([&](auto iLd) {
        /** Rule for reading new load allocation: {addr, tag} pairs. */
        if (!LoadAllocValid[iLd]) {
          bool succ = false;
          const auto LoadReq = LoadAddrPipes::template PipeAt<iLd>::read(succ);
          if (succ) {
            LoadAllocAddr[iLd] = LoadReq.addr;
            LoadAllocTag[iLd] = LoadReq.tag;
            UnrolledLoop<NUM_STORES>([&](auto iSt) {
              LoadAllocMinTag[iLd][iSt] = LoadReq.mintag[iSt];
            });
            LoadAllocValid[iLd] = true;
          }
        }
        /** End Rule */

        /** Rule for checking load safety and reuse. */
        if (LoadAllocValid[iLd]) {
          bool Safe[NUM_STORES];
          bool Reuse[NUM_STORES];
          T ReuseVal[NUM_STORES];
          uint ReuseTag[NUM_STORES];
          bool AllMinTag = true;
          bool AllSafe = true;
          bool AnyReuse = false;
          UnrolledLoop<NUM_STORES>([&](uint iSt) {
            // TODO: CAM INTO STORE ALLOCS
            Safe[iSt] = (LoadAllocTag[iLd] < NextStoreTag[iSt]) ||
                        (LoadAllocTag[iLd] >= StoreAckTag[iSt] &&
                         LoadAllocAddr[iLd] <= StoreAckAddr[iSt]);
            Reuse[iSt] = (LoadAllocTag[iLd] >= LastStoreTag[iSt]) &&
                         (LoadAllocAddr[iLd] == LastStoreAddr[iSt]);
            ReuseVal[iSt] = LastStoreVal[iSt];
            ReuseTag[iSt] = Reuse[iSt] ? LastStoreTag[iSt] : 0u;

            AllMinTag &= (LoadAllocMinTag[iLd][iSt] <= NextStoreTag[iSt]);
            AllSafe &= Safe[iSt];
            AnyReuse |= Reuse[iSt];
          });

          LoadExecSafeToLoad[iLd] = Safe[0];
          LoadExecReuse[iLd] = Reuse[0];
          LoadExecReuseTag[iLd] = ReuseTag[0];
          LoadExecReuseVal[iLd] = ReuseVal[0];
          UnrolledLoop<1, NUM_STORES>([&](auto iSt) {
            LoadExecSafeToLoad[iLd] &= Safe[iSt];
            LoadExecReuse[iLd] |= Reuse[iSt];

            if (ReuseTag[iSt] > LoadExecReuseTag[iLd]) {
              LoadExecReuseTag[iLd] = ReuseTag[iSt];
              LoadExecReuseVal[iLd] = ReuseVal[iSt];
            }
          });

          if (AllMinTag && (AllSafe || AnyReuse)) {
            // if (iLd == 1) {
            //   PRINTF(
            //       "@%d SAFE (%d, %d)  Reuse=(%d, tag=%d) NextStore (%d, %d) "
            //       "LastStore (%d, %d)  Ack (%d, %d)\n",
            //       cycle, LoadAllocAddr[iLd], LoadAllocTag[iLd],
            //       LoadExecReuse[iLd], LoadExecReuseTag[iLd],
            //       NextStoreAddr[0], NextStoreTag[0], LastStoreAddr[0],
            //       LastStoreTag[0], StoreAckAddr[0], StoreAckTag[0]);
            // }

            if (LoadExecReuse[iLd]) {
              LoadMuxPredPipes::template PipeAt<iLd>::write(LD_MUX_REUSE);
              LoadMuxReuseValPipes::template PipeAt<iLd>::write(LoadExecReuseVal[iLd]);
            } else if (LoadExecSafeToLoad[iLd]) {
              LoadMuxPredPipes::template PipeAt<iLd>::write(LD_MUX_LOAD);
              LoadPortAddrPipes::template PipeAt<iLd>::write(LoadAllocAddr[iLd]);
            }

            LoadAllocValid[iLd] = false;
          }
        }
        /** End Rule */
      });

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////    STORE LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////
      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        /** Rule for always listining for store ACKs. */
        bool succ = false;
        auto tryAck = StorePortAckPipes::template PipeAt<iSt>::read(succ);
        if (succ) {
          StoreAckTag[iSt] = tryAck.tag;
          StoreAckAddr[iSt] = tryAck.addr;
        }
        /** End Rule */

        /** Rule for reading store {addr, tag} pairs. */
        if (!StoreAllocValid[iSt][ST_Q_SIZE - 1]) {
          bool succ = false;
          auto tryStoreReq = StoreAddrPipes::template PipeAt<iSt>::read(succ);
          if (succ) {
            // PRINTF("@%d Got st req tag %d\n", cycle, tryStoreReq.tag);
            StoreAllocAddr[iSt][ST_Q_SIZE - 1] = tryStoreReq.addr;
            StoreAllocTag[iSt][ST_Q_SIZE - 1] = tryStoreReq.tag;
            StoreAllocValid[iSt][ST_Q_SIZE - 1] = true;
          }
        }
        /** End Rule */

        /** Rule for reading store values and writing to store port. */
        if (StoreAllocValid[iSt][0]) {
          bool succ = false;
          auto tryStoreVal = StoreValPipes::template PipeAt<iSt>::read(succ);
          if (succ) {
            // PRINTF("@%d Got st val tag %d\n", cycle, StoreAllocTag[iSt][0]);
            LastStoreVal[iSt] = tryStoreVal;
            LastStoreTag[iSt] = StoreAllocTag[iSt][0];
            LastStoreAddr[iSt] = StoreAllocAddr[iSt][0];

            StoreAllocValid[iSt][0] = false;
          }
        }
        /** End Rule */

        /** Rule shifting store allocation queue. */
        if (!StoreAllocValid[iSt][0]) {
          if (StoreAllocValid[iSt][1]) {
            NextStoreAddr[iSt] = StoreAllocAddr[iSt][1];
            NextStoreTag[iSt] = StoreAllocTag[iSt][1];
          }

          ShiftBundle(StoreAllocAddr[iSt], INVALID_ADDR);
          ShiftBundle(StoreAllocTag[iSt], 0u);
          ShiftBundle(StoreAllocValid[iSt], false);
        }
        /** End Rule */

      }); // End for all stores

    } // end while

    LoadMuxPredPipes::write(LD_MUX_TERMINATE);
    LoadPortAddrPipes::write(MAX_INT);

    PRINTF("** DONE Streaming Memory\n");
  });

  return events;
}
