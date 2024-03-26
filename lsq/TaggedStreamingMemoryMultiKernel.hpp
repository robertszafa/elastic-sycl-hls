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
template <int NUM_STORES> struct addr_tag_mintag_t {
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
// Types for specifying action in load mux.
using ld_mux_pred_t = ac_int<2, false>;
constexpr ld_mux_pred_t LD_MUX_TERMINATE = ld_mux_pred_t{0};
constexpr ld_mux_pred_t LD_MUX_REUSE = ld_mux_pred_t{1};
constexpr ld_mux_pred_t LD_MUX_LOAD = ld_mux_pred_t{2};

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
// template <int MemId, int LoadId> class GatedLoadKernel;
template <int LoadId> class GatedLoadKernel;
template <int MemId, int PortId> class LoadPortKernel;
template <int MemId, int PortId> class StorePortKernel;
template <int MemId, int PortId> class LoadValMuxKernel;

template <int MEM_ID, typename EndSignalPipe, typename LoadAddrPipe,
          typename LoadValPipe, typename StoreAddrPipes,
          typename StorePortAddrPipes, typename StorePortValPipes,
          uint NUM_LOADS, uint NUM_STORES, uint BIT_WIDTH = 32, typename T>
std::vector<event> StreamingMemory(queue &q, T *data) {
  std::vector<event> events(NUM_STORES);

  assert(sizeof(T) * 8 == BIT_WIDTH && "Incorrect kBitWidth.");
  constexpr uint BURST_SIZE = (DRAM_BURST_BITS + BIT_WIDTH - 1) / BIT_WIDTH;

  // Store port.
  using StorePortAckPipes =
    PipeArray<class _StAck, uint, BURST_SIZE * 4, NUM_STORES, NUM_LOADS>;
  using StoreValPipes = 
    PipeArray<class _StVal, tag_val_t<T>, BURST_SIZE * 4, NUM_STORES, NUM_LOADS>;
  UnrolledLoop<NUM_STORES>([&](auto iSt) {
    events[iSt] = q.single_task<StorePortKernel<MEM_ID, iSt>>([=]() KERNEL_PRAGMAS {
      // Shift register to introduce {BURST_SIZE} delay between store and ack.
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
          UnrolledLoop<NUM_LOADS>([&](auto iLd) {
            StoreValPipes::template PipeAt<iSt, iLd>::write({Req.tag, Val});
          });

          auto StorePtr = ext::intel::device_ptr<T>(data + Req.addr);
          BurstCoalescedLSU::store(StorePtr, Val);
          ReqValid = false;
          ValValid = false;
        }

        UnrolledLoop<NUM_LOADS>([&](auto iLd) {
          StorePortAckPipes::template PipeAt<iSt, iLd>::write(AckTag[0]);
        });
        ShiftBundle(AckTag, Req.tag);
      }

      // force burst
      atomic_fence(memory_order_seq_cst, memory_scope_work_item); 

      // Drain acks
      for (int i = 0; i < BURST_SIZE; ++i) {
        UnrolledLoop<NUM_LOADS>([&](auto iLd) {
          StorePortAckPipes::template PipeAt<iSt, iLd>::write(AckTag[i]);
        });
      }

      // PRINTF("** DONE store port %d\n", int(iSt));
    });
  });

  // Pipes for load port and load mux.
  using LoadPortAddrPipe = PipeArray<class _LoadAddr, uint, BURST_SIZE*4, NUM_LOADS>;
  using LoadMuxLoadValPipe = PipeArray<class _LoadMuxMemoryVal, T, BURST_SIZE*4, NUM_LOADS>;
  using LoadMuxPredPipe = PipeArray<class _LoadMuxPred, ld_mux_pred_t, BURST_SIZE*4, NUM_LOADS>;
  using LoadMuxReuseValPipe = PipeArray<class _LoadMuxReuseVal, T, BURST_SIZE*4, NUM_LOADS>;

  UnrolledLoop<NUM_LOADS>([&](auto iLd) {

    q.single_task<LoadPortKernel<MEM_ID, iLd>>([=]() KERNEL_PRAGMAS {
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      [[intel::speculated_iterations(0)]]
      while (true) {
        auto Addr = LoadPortAddrPipe::template PipeAt<iLd>::read();
        if (Addr == MAX_INT) break;
        auto LoadPtr = ext::intel::device_ptr<T>(data + Addr);
        auto Val = BurstCoalescedLSU::load(LoadPtr);
        LoadMuxLoadValPipe::template PipeAt<iLd>::write(Val);
      }
      // PRINTF("** DONE load port %d\n", int(iLd));
    });

    q.single_task<LoadValMuxKernel<MEM_ID, iLd>>([=]() KERNEL_PRAGMAS {
      int NumTotal = 0;
      int NumReused = 0;
      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      [[intel::speculated_iterations(0)]]
      while (true) {
        T Val;
        ld_mux_pred_t Pred = LoadMuxPredPipe::template PipeAt<iLd>::read();
        if (Pred == LD_MUX_REUSE) {
          NumReused++;
          Val = LoadMuxReuseValPipe::template PipeAt<iLd>::read();
        } else if (Pred == LD_MUX_LOAD) {
          Val = LoadMuxLoadValPipe::template PipeAt<iLd>::read();
        } else {
          break;
        }

        // PRINTF("Returned load num %d\n", NumTotal);
        LoadValPipe::template PipeAt<iLd>::write(Val);
        NumTotal++;
      }
      PRINTF("** DONE load MUX%d, reused %d/%d\n", int(iLd), NumReused, NumTotal);
    });

    q.single_task<GatedLoadKernel<iLd>>([=]() KERNEL_PRAGMAS {
      bool StoreAllocValid[NUM_STORES];
      int StoreAllocAddr[NUM_STORES];
      uint StoreAllocTag[NUM_STORES];
      uint LastStoreValTag[NUM_STORES];
      T LastStoreValue[NUM_STORES];
      uint StoreAckTag[NUM_STORES];

      // Init store registers
      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        auto StoreReq = StoreAddrPipes::template PipeAt<iSt, iLd>::read();
        StoreAllocAddr[iSt] = StoreReq.addr;
        StoreAllocTag[iSt] = StoreReq.tag;
        StoreAllocValid[iSt] = true;
      });
      InitBundle(LastStoreValTag, 0u);
      InitBundle(LastStoreValue, T{});
      InitBundle(StoreAckTag, 0u);

      constexpr uint LD_Q_SIZE = 2;
      bool LoadAllocValid[LD_Q_SIZE];
      int LoadAllocAddr[LD_Q_SIZE];
      uint LoadAllocTag[LD_Q_SIZE];
      uint LoadAllocMinTag[NUM_STORES][LD_Q_SIZE];
      bool LoadExecValid[LD_Q_SIZE]; 
      int LoadExecAddr[LD_Q_SIZE]; 
      uint LoadExecTag[LD_Q_SIZE]; 
      bool LoadExecSafeNow[LD_Q_SIZE];
      bool LoadExecSafeAfterTag[LD_Q_SIZE];
      bool LoadExecReuse[LD_Q_SIZE];
      uint LoadExecValWaitTag[LD_Q_SIZE];
      uint LoadExecAckWaitTag[LD_Q_SIZE];
      uint LoadExecAckWaitId[LD_Q_SIZE];
      uint LoadExecValWaitId[LD_Q_SIZE];

      int LastLoadAllocAddr = INVALID_ADDR;
      uint LastLoadAllocTag = MAX_INT;

      // Init load registers
      InitBundle(LoadAllocValid, false);
      InitBundle(LoadAllocAddr, INVALID_ADDR);
      InitBundle(LoadAllocTag, MAX_INT);
      InitBundle(LoadExecValid, false); 
      InitBundle(LoadExecAddr, INVALID_ADDR);
      InitBundle(LoadExecTag, MAX_INT);
      InitBundle(LoadExecSafeNow, false);
      InitBundle(LoadExecSafeAfterTag, false);
      InitBundle(LoadExecReuse, false);
      InitBundle(LoadExecValWaitTag, MAX_INT);
      InitBundle(LoadExecAckWaitTag, 0u);
      InitBundle(LoadExecAckWaitId, 0u);
      InitBundle(LoadExecValWaitId, 0u);
      UnrolledLoop<NUM_STORES>(
          [&](auto iSt) { InitBundle(LoadAllocMinTag[iSt], 0u); });

      bool EndSignal = false;

      [[maybe_unused]] uint cycle = 0;

      [[intel::ivdep]]
      [[intel::initiation_interval(1)]]
      [[intel::speculated_iterations(0)]]
      while (!EndSignal) {
        cycle++;

        EndSignalPipe::template PipeAt<iLd>::read(EndSignal);
        
        /** Rule for shifting load allocation queue. */ 
        if (!LoadAllocValid[0]) {
          ShiftBundle(LoadAllocAddr, INVALID_ADDR);
          ShiftBundle(LoadAllocTag, 0u);
          ShiftBundle(LoadAllocValid, false);
          UnrolledLoop<NUM_STORES>(
              [&](auto iSt) { ShiftBundle(LoadAllocMinTag[iSt], 0u); });
        }
        /** End Rule */

        /** Rule for shifting load execution queue. */ 
        if (!LoadExecValid[0]) {
          ShiftBundle(LoadExecValid, false);
          ShiftBundle(LoadExecAddr, INVALID_ADDR);
          ShiftBundle(LoadExecTag, 0u);
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
          const auto LoadReq = LoadAddrPipe::template PipeAt<iLd>::read(succ);
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
          LastLoadAllocAddr = LoadAllocAddr[0];
          LastLoadAllocTag = LoadAllocTag[0];

          bool SafeNow[NUM_STORES];
          bool SafeAfterTag[NUM_STORES];
          bool Reuse[NUM_STORES];
          uint WaitForValTag[NUM_STORES];
          uint WaitForAckTag[NUM_STORES];
          bool AllStoresSafe = true;
          UnrolledLoop<NUM_STORES>([&](uint iSt) {
            SafeNow[iSt] = (LoadAllocTag[0] < StoreAllocTag[iSt]);
            SafeAfterTag[iSt] = (LoadAllocTag[0] == StoreAllocTag[iSt] ||
                                 (LoadAllocTag[0] > StoreAllocTag[iSt] &&
                                  LoadAllocAddr[0] < StoreAllocAddr[iSt]));
            Reuse[iSt] = (LoadAllocTag[0] >= StoreAllocTag[iSt]) &&
                         (LoadAllocAddr[0] == StoreAllocAddr[iSt]);

            WaitForValTag[iSt] = StoreAllocTag[iSt];
            WaitForAckTag[iSt] = StoreAllocTag[iSt];

            const bool ThisStoreSafe =
                (LoadAllocMinTag[iSt][0] < StoreAllocTag[iSt]) &&
                (SafeNow[iSt] || SafeAfterTag[iSt] || Reuse[iSt]);
            AllStoresSafe &= ThisStoreSafe;
          });

          if (AllStoresSafe) {
            // TODO: LD_Q_SIZE-2 ?
            // PRINTF("@%d SAFE addr %d, safety=(%d, %d, %d), storeAlloc=(%d, %d)\n",
            //        cycle, LoadAllocAddr[0], SafeNow[0], SafeAfterTag[0], Reuse[0],
            //        StoreAllocAddr[0], StoreAllocTag[0]);
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
            LoadExecTag[LD_Q_SIZE - 1] = LoadAllocTag[0];
            LoadAllocValid[0] = false;
          }
        }
        /** End Rule */

        /** Rule for returning reused/loaded values. Reuse takes precedence. 
            Any of {Reuse, SafeNow, SafeAfterTag} implies LoadExecValid. */
        uint StIdToWaitForAck = 0u, StIdToWaitForVal = 0u;
        if constexpr (NUM_STORES > 1) {
          StIdToWaitForAck = LoadExecAckWaitId[0];
          StIdToWaitForVal = LoadExecValWaitId[0];
        }
        if (LoadExecReuse[0]) {
          if (LoadExecValWaitTag[0] == LastStoreValTag[StIdToWaitForVal]) {
            LoadMuxPredPipe::template PipeAt<iLd>::write(LD_MUX_REUSE);
            LoadMuxReuseValPipe::template PipeAt<iLd>::write(LastStoreValue[StIdToWaitForVal]);
            LoadExecValid[0] = false;
          }
        } else if (LoadExecSafeNow[0] ||
                  (LoadExecSafeAfterTag[0] &&
                    (LoadExecAckWaitTag[0] <= StoreAckTag[StIdToWaitForAck]))) {
          LoadMuxPredPipe::template PipeAt<iLd>::write(LD_MUX_LOAD);
          LoadPortAddrPipe::template PipeAt<iLd>::write(LoadExecAddr[0]);
          LoadExecValid[0] = false;
        }
        /** End Rule */

        ////////////////////////////////////////////////////////////////
        /// Get store (address, tag) pairs, store ACKs, and store values.
        UnrolledLoop<NUM_STORES>([&](auto iSt) {
          /** Rule for always listining for store ACKs. */
          bool succ = false;
          auto tryAckTag = StorePortAckPipes::template PipeAt<iSt, iLd>::read(succ);
          if (succ) StoreAckTag[iSt] = tryAckTag;
          /** End Rule */

          /** Rule for reading store {addr, tag} pairs. */
          const bool StallNextAddr = (StoreAllocAddr[iSt] > LastLoadAllocAddr &&
                                      StoreAllocTag[iSt] < LastLoadAllocTag);
          if (!StallNextAddr) {
            bool succ = false;
            auto StoreReq = StoreAddrPipes::template PipeAt<iSt, iLd>::read(succ);
            if (succ) {
              // PRINTF("@%d Next.. PrevStAlloc (%d, %d)\n\t  ldAllocHead=(%d, "
              //        "%d, valid=%d), ldExecTail=(%d, %d, valid=%d)\n\n",
              //        cycle, StoreAllocAddr[iSt], StoreAllocTag[iSt],
              //        LoadAllocAddr[0], LoadAllocTag[0], LoadAllocValid[0],
              //        LoadExecAddr[LD_Q_SIZE - 1], LoadExecTag[LD_Q_SIZE - 1],
              //        LoadExecValid[LD_Q_SIZE - 1]);
              StoreAllocAddr[iSt] = StoreReq.addr;
              StoreAllocTag[iSt] = StoreReq.tag;
            }
          }
          /** End Rule */

          /** Rule for reading store values and writing to store port. */
          const bool StallNextVal = LastStoreValTag[iSt] >= LoadExecValWaitTag[0];
          if (!StallNextVal) {
            bool succ = false;
            auto StoreVal = StoreValPipes::template PipeAt<iSt, iLd>::read(succ);
            if (succ) {
              // PRINTF("@%d st val tag %d\n", cycle, StoreVal.tag);
              LastStoreValue[iSt] = StoreVal.val;
              LastStoreValTag[iSt] = StoreVal.tag;
            }
          }
          /** End Rule */

        }); 
        /// End for all stores
        ////////////////////////////////////////////////////////////////

      } // end while

      LoadMuxPredPipe::template PipeAt<iLd>::write(LD_MUX_TERMINATE);
      LoadPortAddrPipe::template PipeAt<iLd>::write(MAX_INT);

      // PRINTF("** DONE Streaming Memory %d\n", int(iLd));
    });
  });

  return events;
}
