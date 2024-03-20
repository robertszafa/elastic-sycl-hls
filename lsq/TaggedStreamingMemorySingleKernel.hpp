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

struct addr_t {
  uint addr;
};

struct addr_tag_t {
  uint addr;
  uint tag;
};

template <int NUM_STORES>
struct addr_tag_mintag_t {
  uint addr;
  uint tag;
  /// Minimum tag that a store dependency has to have.
  uint mintag[NUM_STORES];
};

template <typename T> struct addr_val_t {
  uint addr;
  T val;
};

template <typename T> struct tag_val_t {
  uint tag;
  T val;
};

template <typename T> struct addr_tag_val_t {
  uint addr;
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
  using StorePortAckPipes = PipeArray<class _StAck, uint, BURST_SIZE*4, NUM_STORES>;
  UnrolledLoop<NUM_STORES>([&](auto iSt) {
    events[iSt] = q.single_task<StorePortKernel<MEM_ID, iSt>>([=]() KERNEL_PRAGMAS {
      // Only send ack once {BusrtSize} amount of elements have been stored.
      uint AckTag[BURST_SIZE];
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

        if (ReqValid && ValValid) {
          auto StorePtr = ext::intel::device_ptr<T>(data + Req.addr);
          BurstCoalescedLSU::store(StorePtr, Val);
          ReqValid = false;
          ValValid = false;
        }

        StorePortAckPipes::template PipeAt<iSt>::write(AckTag[0]);
        ShiftBundle(AckTag, Req.tag);
      }
      // Drain acks
      atomic_fence(memory_order_seq_cst, memory_scope_work_item); // force burst
      for (int i = 0; i < BURST_SIZE; ++i) {
        StorePortAckPipes::template PipeAt<iSt>::write(AckTag[i]);
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
    constexpr uint ST_Q_SIZE = 2;
    bool StoreAllocValid[NUM_STORES][ST_Q_SIZE];
    uint StoreAllocAddr[NUM_STORES][ST_Q_SIZE];
    uint StoreAllocTag[NUM_STORES][ST_Q_SIZE];
    bool StoreCommitValid[NUM_STORES][ST_Q_SIZE];
    uint StoreCommitTag[NUM_STORES][ST_Q_SIZE];

    uint LastStoreAllocAddr[NUM_STORES];
    uint LastStoreAllocTag[NUM_STORES];
    uint LastStoreValTag[NUM_STORES];
    T LastStoreValue[NUM_STORES];
    uint StoreAckTag[NUM_STORES];

    // Init store registers
    UnrolledLoop<NUM_STORES>([&] (auto iSt) {
      InitBundle(StoreAllocAddr[iSt], 0u);
      InitBundle(StoreAllocTag[iSt], 1u);
      InitBundle(StoreAllocValid[iSt], false);
      InitBundle(StoreCommitTag[iSt], 0u);
      InitBundle(StoreCommitValid[iSt], false);

      LastStoreAllocAddr[iSt] = 0u;
      LastStoreAllocTag[iSt] = 1u;
      LastStoreValTag[iSt] = 0u;
      LastStoreValue[iSt] = T{};
      StoreAckTag[iSt] = 0u;
    });

    constexpr uint LD_Q_SIZE = 2;
    bool LoadAllocValid[LD_Q_SIZE];
    uint LoadAllocAddr[LD_Q_SIZE];
    uint LoadAllocTag[LD_Q_SIZE];
    uint LoadAllocMinTag[NUM_STORES][LD_Q_SIZE];
    
    bool LoadExecValid[LD_Q_SIZE]; 
    uint LoadExecAddr[LD_Q_SIZE]; 

    bool LoadExecSafeNow[LD_Q_SIZE];
    bool LoadExecSafeAfterTag[LD_Q_SIZE];
    bool LoadExecReuse[LD_Q_SIZE];
    uint LoadExecValWaitTag[LD_Q_SIZE];
    uint LoadExecAckWaitTag[LD_Q_SIZE];
    uint LoadExecAckWaitId[LD_Q_SIZE];
    uint LoadExecValWaitId[LD_Q_SIZE];

    // Init load registers
    InitBundle(LoadAllocValid, false);
    InitBundle(LoadAllocAddr, 1u);
    InitBundle(LoadAllocTag, 0u);
    InitBundle(LoadExecValid, false); 
    InitBundle(LoadExecAddr, 0u);

    InitBundle(LoadExecSafeNow, false);
    InitBundle(LoadExecSafeAfterTag, false);
    InitBundle(LoadExecReuse, false);
    InitBundle(LoadExecValWaitTag, 0u);
    InitBundle(LoadExecAckWaitTag, 0u);
    InitBundle(LoadExecAckWaitId, 0u);
    InitBundle(LoadExecValWaitId, 0u);

    UnrolledLoop<NUM_STORES>(
        [&](auto iSt) { InitBundle(LoadAllocMinTag[iSt], 0u); });

    bool EndSignal = false;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (!EndSignal) {
      EndSignalPipe::read(EndSignal);

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////     LOAD LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////

      /** Rule for shifting load allocation queue. */ 
      if (!LoadAllocValid[0]) {
        ShiftBundle(LoadAllocAddr, 0u);
        ShiftBundle(LoadAllocTag, 0u);
        ShiftBundle(LoadAllocValid, false);

        UnrolledLoop<NUM_STORES>(
            [&](auto iSt) { ShiftBundle(LoadAllocMinTag[iSt], 0u); });
      }
      /** End Rule */

      /** Rule for shifting load execution queue. */ 
      if (!LoadExecValid[0]) {
        ShiftBundle(LoadExecValid, false);
        ShiftBundle(LoadExecAddr, 0u);

        ShiftBundle(LoadExecSafeNow, false);
        ShiftBundle(LoadExecSafeAfterTag, false);
        ShiftBundle(LoadExecReuse, false);
        ShiftBundle(LoadExecValWaitTag, 0u);
        ShiftBundle(LoadExecAckWaitTag, 0u);
        ShiftBundle(LoadExecAckWaitId, 0u);
        ShiftBundle(LoadExecValWaitId, 0u);
      }
      /** End Rule */

      /** Rule for reading new load allocation: {addr, tag} pairs. */
      if (!LoadAllocValid[LD_Q_SIZE - 1]) {
        bool succ = false;
        const auto LoadReq = LoadAddrPipe::read(succ);
        if (succ) {
          LoadAllocAddr[LD_Q_SIZE - 1] = LoadReq.addr;
          LoadAllocTag[LD_Q_SIZE - 1] = LoadReq.tag;
          UnrolledLoop<NUM_STORES>([&](auto iSt) {
            LoadAllocMinTag[iSt][LD_Q_SIZE - 1] = LoadReq.mintag[iSt];
          });
          LoadAllocValid[LD_Q_SIZE - 1] = true;
        }
      }
      /** End Rule */

      /** Rule for checking load safety and reuse. */
      if (LoadAllocValid[0] && !LoadExecValid[LD_Q_SIZE - 1]) {
        bool SafeNow[NUM_STORES];
        bool SafeAfterTag[NUM_STORES];
        bool Reuse[NUM_STORES];
        uint WaitForValTag[NUM_STORES];
        uint WaitForAckTag[NUM_STORES];
        bool AllStoresSafe = true;
        UnrolledLoop<NUM_STORES>([&](uint iSt) {
          SafeNow[iSt] = (LoadAllocTag[0] < LastStoreAllocTag[iSt]);
          SafeAfterTag[iSt] = (LoadAllocTag[0] == LastStoreAllocTag[iSt] ||
                               (LoadAllocTag[0] > LastStoreAllocTag[iSt] &&
                                LoadAllocAddr[0] < LastStoreAllocAddr[iSt]));
          Reuse[iSt] = (LoadAllocTag[0] >= LastStoreAllocTag[iSt]) &&
                       (LoadAllocAddr[0] == LastStoreAllocAddr[iSt]);

          WaitForValTag[iSt] = LastStoreAllocTag[iSt];
          WaitForAckTag[iSt] = LastStoreAllocTag[iSt];

          const bool ThisStoreSafe =
              (LoadAllocMinTag[iSt][0] <= LastStoreAllocTag[iSt]) &&
              (SafeNow[iSt] || SafeAfterTag[iSt] || Reuse[iSt]);
          if constexpr (NUM_STORES > 1)
            AllStoresSafe &= ThisStoreSafe;
          else
            AllStoresSafe = ThisStoreSafe;
        });

        if (AllStoresSafe) {
          // TODO: LD_Q_SIZE-2 ?
          LoadExecSafeNow[LD_Q_SIZE - 1] = SafeNow[0];
          LoadExecSafeAfterTag[LD_Q_SIZE - 1] = SafeAfterTag[0];
          LoadExecReuse[LD_Q_SIZE - 1] = Reuse[0];
          LoadExecValWaitTag[LD_Q_SIZE - 1] = WaitForValTag[0];
          LoadExecAckWaitTag[LD_Q_SIZE - 1] = WaitForAckTag[0];
          LoadExecAckWaitId[LD_Q_SIZE - 1] = 0;
          LoadExecValWaitId[LD_Q_SIZE - 1] = 0;
          UnrolledLoop<1, NUM_STORES>([&](auto iSt) {
            LoadExecSafeNow[LD_Q_SIZE - 1] &= SafeNow[iSt];
            LoadExecSafeAfterTag[LD_Q_SIZE - 1] |= SafeAfterTag[iSt];
            LoadExecReuse[LD_Q_SIZE - 1] |= Reuse[iSt];

            if (SafeAfterTag[iSt] &&
                (WaitForAckTag[iSt] > LoadExecAckWaitTag[LD_Q_SIZE - 1])) {
              LoadExecAckWaitTag[LD_Q_SIZE - 1] = WaitForAckTag[iSt];
              LoadExecAckWaitId[LD_Q_SIZE - 1] = iSt;
            }
            if (Reuse[iSt] &&
                (WaitForValTag[iSt] > LoadExecValWaitTag[LD_Q_SIZE - 1])) {
              LoadExecValWaitTag[LD_Q_SIZE - 1] = WaitForValTag[iSt];
              LoadExecValWaitId[LD_Q_SIZE - 1] = iSt;
            }
          });

          LoadExecValid[LD_Q_SIZE - 1] = true;
          LoadExecAddr[LD_Q_SIZE - 1] = LoadAllocAddr[0];
          LoadAllocValid[0] = false;
        }
      }
      /** End Rule */

      /** Rule for returning reused/loaded values. Reuse takes precedence. 
          Any of {Reuse, SafeNow, SafeAfterTag} implies LoadExecValid. */
      if (LoadExecReuse[0]) {
        if (LoadExecValWaitTag[0] == LastStoreValTag[LoadExecValWaitId[0]]) {
          LoadMuxPredPipe::write(LD_MUX_REUSE);
          LoadMuxReuseValPipe::write(LastStoreValue[LoadExecValWaitId[0]]);
          LoadExecValid[0] = false;
        }
      } else if (LoadExecSafeNow[0] ||
                (LoadExecSafeAfterTag[0] &&
                  (LoadExecAckWaitTag[0] <= StoreAckTag[LoadExecAckWaitId[0]]))) {
        LoadMuxPredPipe::write(LD_MUX_LOAD);
        LoadPortAddrPipe::write(LoadExecAddr[0]);
        LoadExecValid[0] = false;
      }
      /** End Rule */

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////    STORE LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////
      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        /** Rule for always listining for store ACKs. */
        bool succ = false;
        auto tryAckTag = StorePortAckPipes::template PipeAt<iSt>::read(succ);
        if (succ) StoreAckTag[iSt] = tryAckTag;
        /** End Rule */

        /** Rule shifting store allocation queue. */
        if (!StoreAllocValid[iSt][0]) {
          ShiftBundle(StoreAllocAddr[iSt], 0u);
          ShiftBundle(StoreAllocTag[iSt], 0u);
          ShiftBundle(StoreAllocValid[iSt], false);
        }
        /** End Rule */

        /** Rule shifting store commit queue. */
        if (!StoreCommitValid[iSt][0]) {
          ShiftBundle(StoreCommitValid[iSt], false);
          ShiftBundle(StoreCommitTag[iSt], 0u);
        }
        /** End Rule */

        /** Rule for moving store allocation to store commit queue. */
        const bool GetNextAddr = (LastStoreAllocAddr[iSt] <= LoadAllocAddr[0] ||
                                  LastStoreAllocTag[iSt] > LoadAllocTag[0]);
        if (GetNextAddr) {
          if (StoreAllocValid[iSt][0] && !StoreCommitValid[iSt][ST_Q_SIZE - 1]) {
            LastStoreAllocAddr[iSt] = StoreAllocAddr[iSt][0];
            LastStoreAllocTag[iSt] = StoreAllocTag[iSt][0];

            StoreCommitValid[iSt][ST_Q_SIZE - 1] = true;
            StoreCommitTag[iSt][ST_Q_SIZE - 1] = StoreAllocTag[iSt][0];

            StoreAllocValid[iSt][0] = false;
          }
        }
        /** End Rule */

        /** Rule for reading store {addr, tag} pairs. */
        if (!StoreAllocValid[iSt][ST_Q_SIZE - 1]) {
          bool succ = false;
          auto StoreReq = StoreAddrPipes::template PipeAt<iSt>::read(succ);
          if (succ) {
            // PRINTF("Got st req tag %d\n", StoreReq.tag);
            StoreAllocAddr[iSt][ST_Q_SIZE - 1] = StoreReq.addr;
            StoreAllocTag[iSt][ST_Q_SIZE - 1] = StoreReq.tag;
            StoreAllocValid[iSt][ST_Q_SIZE - 1] = true;
          }
        }
        /** End Rule */

        /** Rule for reading store values and writing to store port. */
        const bool GetNextVal =
            (LoadExecValWaitTag[0] > LastStoreValTag[iSt]);
        if (StoreCommitValid[iSt][0] && GetNextVal) {
          bool succ = false;
          auto tryStoreVal = StoreValPipes::template PipeAt<iSt>::read(succ);
          if (succ) {
            // PRINTF("Got st val tag %d\n", StoreCommitTag[iSt][0]);
            LastStoreValue[iSt] = tryStoreVal;
            LastStoreValTag[iSt] = StoreCommitTag[iSt][0];
            StoreCommitValid[iSt][0] = false;
          }
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
