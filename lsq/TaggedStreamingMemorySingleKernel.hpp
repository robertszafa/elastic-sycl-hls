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
constexpr uint MAX_INT = (1<<30);
constexpr int INVALID_ADDR = -1;

struct addr_t {
  int addr;
};

struct addr_tag_t {
  int addr;
  uint tag;
};

template <int NUM_STORES>
struct addr_tag_mintag_t {
  int addr;
  uint tag;
  /// Minimum tag that a store dependency has to have.
  uint mintag[NUM_STORES];
};

template <typename T> struct addr_val_t {
  int addr;
  T val;
};

template <typename T> struct tag_val_t {
  uint tag;
  T val;
};

template <typename T> struct addr_tag_val_t {
  int addr;
  uint tag;
  T val;
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

template <int MEM_ID, typename EndSignalPipe, typename LoadAddrPipe,
          typename LoadValPipe, typename StoreAddrPipes, typename StoreValPipes,
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
      addr_tag_t Ack[BURST_SIZE];
      InitBundle(Ack, {INVALID_ADDR, 0u});

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

        if (ReqValid && ValValid) {
          auto StorePtr = ext::intel::device_ptr<T>(data + Req.addr);
          BurstCoalescedLSU::store(StorePtr, Val);
          ReqValid = false;
          ValValid = false;
        }

        StorePortAckPipes::template PipeAt<iSt>::write(Ack[0]);
        ShiftBundle(Ack, Req);
      }
      // Drain acks
      atomic_fence(memory_order_seq_cst, memory_scope_work_item); // force burst
      for (int i = 0; i < BURST_SIZE; ++i) {
        StorePortAckPipes::template PipeAt<iSt>::write(Ack[i]);
      }
      // StorePortAckPipe::write(MAX_INT);
    });
  });

  // Types for specifying action in load mux.
  using ld_mux_pred_t = ac_int<2, false>;
  constexpr ld_mux_pred_t LD_MUX_TERMINATE = ld_mux_pred_t{0};
  constexpr ld_mux_pred_t LD_MUX_REUSE = ld_mux_pred_t{1};
  constexpr ld_mux_pred_t LD_MUX_LOAD = ld_mux_pred_t{2};
  // Pipes for load port and load mux.
  using LoadPortAddrPipe = pipe<class _LoadAddr, uint, BURST_SIZE*4>;
  using LoadMuxLoadValPipe = pipe<class _LoadMuxMemoryVal, T, BURST_SIZE*4>;
  using LoadMuxPredPipe = pipe<class _LoadMuxPred, ld_mux_pred_t, BURST_SIZE*4>;
  using LoadMuxReuseValPipe = pipe<class _LoadMuxReuseVal, T, BURST_SIZE*4>;
  q.single_task<LoadPortKernel<MEM_ID, 0>>([=]() KERNEL_PRAGMAS {
    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      auto Addr = LoadPortAddrPipe::read();
      if (Addr == MAX_INT) break;
      auto LoadPtr = ext::intel::device_ptr<T>(data + Addr);
      auto Val = BurstCoalescedLSU::load(LoadPtr);
      LoadMuxLoadValPipe::write(Val);
    }
  });
  q.single_task<LoadValMuxKernel<MEM_ID, 0>>([=]() KERNEL_PRAGMAS {
    int NumTotal = 0;
    int NumReused = 0;
    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      T Val;
      ld_mux_pred_t Pred = LoadMuxPredPipe::read();
      if (Pred == LD_MUX_REUSE) {
        NumReused++;
        Val = LoadMuxReuseValPipe::read();
      } else if (Pred == LD_MUX_LOAD) {
        Val = LoadMuxLoadValPipe::read();
      } else {
        break;
      }

      // PRINTF("Returned load num %d\n", NumTotal);
      LoadValPipe::write(Val);
      NumTotal++;
    }
    PRINTF("** DONE load MUX, reused %d/%d\n", NumReused, NumTotal);
  });

  q.single_task<StreamingMemoryKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    constexpr uint ST_Q_SIZE = 16;
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

    bool LoadAllocValid = false;
    int LoadAllocAddr = INVALID_ADDR;
    uint LoadAllocTag = 0u;
    uint LoadAllocMinTag[NUM_STORES];
    InitBundle(LoadAllocMinTag, 0u); 
    
    bool LoadExecValid = false; 
    int LoadExecAddr = INVALID_ADDR; 
    bool LoadExecSafeToLoad = false;
    bool LoadExecReuse = false;
    uint LoadExecReuseTag = 0u;
    uint LoadExecReuseStoreId = 0u;
    T LoadExecReuseVal = T{};
    bool LoadExecHasReuseVal = false;

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

      /** Rule for reading new load allocation: {addr, tag} pairs. */
      if (!LoadAllocValid) {
        bool succ = false;
        const auto LoadReq = LoadAddrPipe::read(succ);
        if (succ) {
          LoadAllocAddr = LoadReq.addr;
          LoadAllocTag = LoadReq.tag;
          UnrolledLoop<NUM_STORES>([&](auto iSt) {
            LoadAllocMinTag[iSt] = LoadReq.mintag[iSt];
          });
          LoadAllocValid = true;
        }
      }
      /** End Rule */

      /** Rule for checking load safety and reuse. */
      if (LoadAllocValid && !LoadExecValid) {
        bool Safe[NUM_STORES];
        bool Reuse[NUM_STORES];
        uint ReuseTag[NUM_STORES];
        bool AllStoresSafeOrReuse = true;
        UnrolledLoop<NUM_STORES>([&](uint iSt) {
          // TODO: CAM INTO STORE ALLOCS
          // bool LookAheadSafe = (LoadAllocTag < StoreAllocTag[iSt][ST_Q_SIZE]);
          // #pragma unroll
          // for (int i = 0; i < ST_Q_SIZE; ++i) {
          //   if (LoadAllocTag >= StoreAllocTag[iSt][i] &&
          //       LoadAllocAddr == StoreAllocAddr[iSt][i]) {
          //     LookAheadSafe = false;
          //   }
          // }

          Safe[iSt] = (LoadAllocTag < NextStoreTag[iSt]) ||
                      (LoadAllocTag >= StoreAckTag[iSt] &&
                       LoadAllocAddr <= StoreAckAddr[iSt]);
          // Reuse[iSt] = (LoadAllocTag >= LastStoreTag[iSt]) &&
          //              (LoadAllocAddr == LastStoreAddr[iSt]);
          Reuse[iSt] = (LoadAllocTag >= NextStoreTag[iSt]) &&
                       (LoadAllocAddr == NextStoreAddr[iSt]);
          ReuseTag[iSt] = Reuse[iSt] ? NextStoreTag[iSt] : 0u;

          bool ThisIsSafe = (LoadAllocMinTag[iSt] <= NextStoreTag[iSt]) &&
                            (Safe[iSt] || Reuse[iSt]);
          AllStoresSafeOrReuse &= ThisIsSafe;
        });

        LoadExecAddr = LoadAllocAddr;
        LoadExecSafeToLoad = Safe[0];
        LoadExecReuse = Reuse[0];
        LoadExecReuseTag = ReuseTag[0];
        LoadExecReuseStoreId = 0;
        UnrolledLoop<1, NUM_STORES>([&](auto iSt) {
          LoadExecSafeToLoad &= Safe[iSt];
          LoadExecReuse |= Reuse[iSt];

          if (Reuse[iSt] && (ReuseTag[iSt] > LoadExecReuseTag)) {
            LoadExecReuseTag = ReuseTag[iSt];
            LoadExecReuseStoreId = iSt;
          }
        });

        if (AllStoresSafeOrReuse) {
          // PRINTF("@%d SAFE (%d, %d)  Reuse=%d NextStore (%d, %d)  LastStore (%d, %d)  Ack "
          //        "(%d, %d)\n",
          //        cycle, LoadAllocAddr, LoadAllocTag, LoadExecReuse, 
          //        NextStoreAddr[0], NextStoreTag[0], 
          //        LastStoreAddr[0], LastStoreTag[0],
          //        StoreAckAddr[0], StoreAckTag[0]
          //        );
          LoadExecValid = true;
          LoadAllocValid = false;
        }
      }
      /** End Rule */

      if (LoadExecValid &&
          (LoadExecReuseTag == LastStoreTag[LoadExecReuseStoreId])) {
        LoadExecReuseVal = LastStoreVal[LoadExecReuseStoreId];
        LoadExecHasReuseVal = true;
      }

      /** Rule for returning reused/loaded values. Reuse takes precedence. 
          Any of {Reuse, SafeNow, SafeAfterTag} implies LoadExecValid. */
      if (LoadExecValid) {
        if (LoadExecReuse) {
          if (LoadExecHasReuseVal) {
            LoadMuxPredPipe::write(LD_MUX_REUSE);
            LoadMuxReuseValPipe::write(LoadExecReuseVal);
            LoadExecValid = false;
            LoadExecHasReuseVal = false;
          }
        } else if (LoadExecSafeToLoad) {
          LoadMuxPredPipe::write(LD_MUX_LOAD);
          LoadPortAddrPipe::write(LoadExecAddr);
          LoadExecValid = false;
          LoadExecHasReuseVal = false;
        }
      }
      /** End Rule */

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

          ShiftBundle(StoreAllocAddr[iSt], 0);
          ShiftBundle(StoreAllocTag[iSt], 0u);
          ShiftBundle(StoreAllocValid[iSt], false);
        }
        /** End Rule */

      }); // End for all stores

    } // end while

    LoadMuxPredPipe::write(LD_MUX_TERMINATE);
    LoadPortAddrPipe::write(MAX_INT);

    PRINTF("** DONE Streaming Memory\n");
  });

  return events;
}
