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
template <int MemId> class GatedLoadKernel;
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

      PRINTF("** DONE store port %d\n", int(iSt));
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
      PRINTF("** DONE load port 0\n");
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

  q.single_task<GatedLoadKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    bool StoreAllocValid[NUM_STORES];
    int StoreAllocAddr[NUM_STORES];
    uint StoreAllocTag[NUM_STORES];

    uint LastStoreValTag[NUM_STORES];
    T LastStoreValue[NUM_STORES];
    uint StoreAckTag[NUM_STORES];

    InitBundle(StoreAllocValid, false);
    InitBundle(StoreAllocAddr, INVALID_ADDR);
    InitBundle(StoreAllocTag, 0u);
    InitBundle(LastStoreValTag, 0u);
    InitBundle(LastStoreValue, T{});
    InitBundle(StoreAckTag, 0u);

    bool LoadAllocValid = false;
    int LoadAllocAddr = INVALID_ADDR;
    uint LoadAllocTag = 0u;
    bool LoadAllocMinTagSatisfied = false;
    bool LoadAllocSafeNow = false;
    bool LoadAllocSafeAfterTag = false;
    bool LoadAllocReuse = false;
    uint LoadAllocValWaitTag = 0u;
    uint LoadAllocAckWaitTag = 0u;
    uint LoadAllocAckWaitId = 0u;
    uint LoadAllocValWaitId = 0u;
    uint LoadAllocMinTag[NUM_STORES];
    InitBundle(LoadAllocMinTag, 0u);

    bool EndSignal = false;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (!EndSignal) {
      EndSignalPipe::template PipeAt<0>::read(EndSignal);

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
      if (LoadAllocValid) {
        bool SafeNow[NUM_STORES];
        bool SafeAfterTag[NUM_STORES];
        bool Reuse[NUM_STORES];
        uint WaitForValTag[NUM_STORES];
        uint WaitForAckTag[NUM_STORES];
        bool MinTagSatisfied[NUM_STORES];
        UnrolledLoop<NUM_STORES>([&](uint iSt) {
          SafeNow[iSt] = (LoadAllocTag < StoreAllocTag[iSt]);
          SafeAfterTag[iSt] = (LoadAllocTag == StoreAllocTag[iSt] ||
                               (LoadAllocTag > StoreAllocTag[iSt] &&
                                LoadAllocAddr < StoreAllocAddr[iSt]));
          Reuse[iSt] = (LoadAllocTag >= StoreAllocTag[iSt]) &&
                       (LoadAllocAddr == StoreAllocAddr[iSt]);
          MinTagSatisfied[iSt] = (LoadAllocMinTag[iSt] <= StoreAllocTag[iSt]);

          WaitForValTag[iSt] = StoreAllocTag[iSt];
          WaitForAckTag[iSt] = StoreAllocTag[iSt];
        });

        // PRINTF("SAFE addr %d, safety=(%d, %d, %d), storeAlloc=(%d, %d)\n",
        //        LoadAllocAddr[0], SafeNow[0], SafeAfterTag[0], Reuse[0],
        //  StoreAllocAddr[0], StoreAllocTag[0]);
        // TODO: LD_Q_SIZE-2 ?
        LoadAllocMinTagSatisfied = MinTagSatisfied[0];
        LoadAllocSafeNow = SafeNow[0];
        LoadAllocSafeAfterTag = SafeAfterTag[0];
        LoadAllocReuse = Reuse[0];
        LoadAllocValWaitTag = WaitForValTag[0];
        LoadAllocAckWaitTag = WaitForAckTag[0];
        LoadAllocAckWaitId = 0;
        LoadAllocValWaitId = 0;
        UnrolledLoop<1, NUM_STORES>([&](auto iSt) {
          LoadAllocMinTagSatisfied &= MinTagSatisfied[iSt];
          LoadAllocSafeNow &= SafeNow[iSt];
          LoadAllocSafeAfterTag |= SafeAfterTag[iSt];
          LoadAllocReuse |= Reuse[iSt];

          if (SafeAfterTag[iSt] && (WaitForAckTag[iSt] > LoadAllocAckWaitTag)) {
            LoadAllocAckWaitTag = WaitForAckTag[iSt];
            LoadAllocAckWaitId = iSt;
          }
          if (Reuse[iSt] && (WaitForValTag[iSt] > LoadAllocValWaitTag)) {
            LoadAllocValWaitTag = WaitForValTag[iSt];
            LoadAllocValWaitId = iSt;
          }
        });

        if (LoadAllocMinTagSatisfied) {
          if (LoadAllocReuse) {
            if (LoadAllocValWaitTag == LastStoreValTag[LoadAllocValWaitId]) {
              LoadMuxPredPipe::write(LD_MUX_REUSE);
              LoadMuxReuseValPipe::write(LastStoreValue[LoadAllocValWaitId]);
              LoadAllocValid = false;
            }
          } else if (LoadAllocSafeNow ||
                     (LoadAllocSafeAfterTag &&
                      (LoadAllocAckWaitTag <= StoreAckTag[LoadAllocAckWaitId]))) {
            LoadMuxPredPipe::write(LD_MUX_LOAD);
            LoadPortAddrPipe::write(LoadAllocAddr);
            LoadAllocValid = false;
          }
        }
      }
      /** End Rule */

      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        /** Rule for always listining for store ACKs. */
        bool succ = false;
        auto tryAckTag = StorePortAckPipes::template PipeAt<iSt, 0>::read(succ);
        if (succ) StoreAckTag[iSt] = tryAckTag;
        /** End Rule */

        /** Rule for reading store {addr, tag} pairs. */
        const bool GetNextAddr = (StoreAllocAddr[iSt] <= LoadAllocAddr ||
                                  StoreAllocTag[iSt] >= LoadAllocTag);
        if (GetNextAddr) {
          bool succ = false;
          auto StoreReq = StoreAddrPipes::template PipeAt<iSt, 0>::read(succ);
          if (succ) {
            StoreAllocAddr[iSt] = StoreReq.addr;
            StoreAllocTag[iSt] = StoreReq.tag;
          }
        }
        /** End Rule */

        /** Rule for reading store values and writing to store port. */
        const bool GetNextVal = LastStoreValTag[iSt] < StoreAllocTag[iSt];
        if (GetNextVal) {
          bool succ = false;
          auto StoreVal = StoreValPipes::template PipeAt<iSt, 0>::read(succ);
          if (succ) {
            LastStoreValue[iSt] = StoreVal.val;
            LastStoreValTag[iSt] = StoreVal.tag;
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
