#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/ext/intel/ac_types/ac_int.hpp>
#include <sycl/sycl.hpp>

#include "unrolled_loop.hpp"
#include "tuple.hpp"
#include "pipe_utils.hpp"
#include "constexpr_math.hpp"
#include "data_bundle.hpp"
#include "device_print.hpp"


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
  using StorePortAckPipes = PipeArray<class _StorePortAck, uint, BURST_SIZE*4, NUM_STORES>;
  UnrolledLoop<NUM_STORES>([&](auto iSt) {
    events[iSt] = q.single_task<StorePortKernel<MEM_ID, iSt>>([=]() KERNEL_PRAGMAS {
      // Only send ack once {BusrtSize} amount of elements have been stored.
      DataBundle<uint, BURST_SIZE> AckTag(uint{0});

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
        AckTag.Shift(Req.tag);
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

      PRINTF("Returned load num %d\n", NumTotal);
      LoadValPipe::write(Val);
      NumTotal++;
    }
    PRINTF("** DONE load MUX, reused %d/%d\n", NumReused, NumTotal);
  });

  q.single_task<StreamingMemoryKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    constexpr uint ST_Q_SIZE = 2;
    DataBundleArray<uint, ST_Q_SIZE, NUM_STORES> StoreAllocAddr(uint{0});
    DataBundleArray<uint, ST_Q_SIZE, NUM_STORES> StoreAllocTag(uint{1});
    DataBundleArray<bool, ST_Q_SIZE, NUM_STORES> StoreAllocValid(bool{false});
    DataBundle<uint, NUM_STORES> LastStoreAllocAddr(uint{0});
    DataBundle<uint, NUM_STORES> LastStoreAllocTag(uint{1});

    DataBundle<T, NUM_STORES> LastStoreValue(T{});
    DataBundle<uint, NUM_STORES> LastStoreValTag(uint{0});
    DataBundleArray<uint, ST_Q_SIZE, NUM_STORES> StoreCommitTag(uint{0});
    DataBundleArray<uint, ST_Q_SIZE, NUM_STORES> StoreCommitValid(bool{false});

    DataBundle<uint, NUM_STORES> StoreAckTag(uint{0});

    constexpr uint LD_Q_SIZE = 2;
    DataBundle<uint, LD_Q_SIZE> LoadAllocAddr(uint{1});
    DataBundle<uint, LD_Q_SIZE> LoadAllocTag(uint{0});
    DataBundle<bool, LD_Q_SIZE> LoadAllocValid(bool{false});
    DataBundleArray<uint, LD_Q_SIZE, NUM_STORES> LoadAllocMinTag(uint{0});
    
    DataBundleArray<bool, LD_Q_SIZE, NUM_STORES> LoadExecSafeNow(bool{false});
    DataBundleArray<bool, LD_Q_SIZE, NUM_STORES> LoadExecSafeAfterTag(bool{false});
    DataBundleArray<bool, LD_Q_SIZE, NUM_STORES> LoadExecReuse(bool{false});
    DataBundleArray<uint, LD_Q_SIZE, NUM_STORES> LoadExecValWaitTag(uint{0});
    DataBundleArray<uint, LD_Q_SIZE, NUM_STORES> LoadExecAckWaitTag(uint{0});
    DataBundle<uint, LD_Q_SIZE> LoadExecAddr(uint{0});
    DataBundle<bool, LD_Q_SIZE> LoadExecValid(bool{false});

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
        LoadAllocAddr.Shift(uint{0});
        LoadAllocTag.Shift(uint{0});
        LoadAllocMinTag.Shift(uint{0});
        LoadAllocValid.Shift(bool{false});
      }
      /** End Rule */

      /** Rule for shifting load execution queue. */ 
      if (!LoadExecValid[0]) {
        LoadExecValid.Shift(bool{false});
        LoadExecSafeNow.Shift(bool{false});
        LoadExecSafeAfterTag.Shift(bool{false});
        LoadExecReuse.Shift(bool{false});
        LoadExecValWaitTag.Shift(uint{0});
        LoadExecAckWaitTag.Shift(uint{0});
        LoadExecAddr.Shift(uint{0});
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
            LoadAllocMinTag[LD_Q_SIZE - 1][iSt] = LoadReq.mintag[iSt];
          });
          LoadAllocValid[LD_Q_SIZE - 1] = true;
        }
      }
      /** End Rule */

      /** Rule for checking load safety and reuse. */
      if (LoadAllocValid[0] && !LoadExecValid[LD_Q_SIZE - 1]) {
        bool IsLoadSafe = true;
        UnrolledLoop<NUM_STORES>([&](auto iSt) {
          LoadExecSafeNow[LD_Q_SIZE - 1][iSt] =
              (LoadAllocTag[0] < LastStoreAllocTag[iSt]);
          LoadExecSafeAfterTag[LD_Q_SIZE - 1][iSt] =
              (LoadAllocTag[0] == LastStoreAllocTag[iSt] ||
               (LoadAllocTag[0] > LastStoreAllocTag[iSt] &&
                LoadAllocAddr[0] < LastStoreAllocAddr[iSt]));
          LoadExecReuse[LD_Q_SIZE - 1][iSt] =
              (LoadAllocTag[0] >= LastStoreAllocTag[0]) &&
              (LoadAllocAddr[0] == LastStoreAllocAddr[0]);

          LoadExecValWaitTag[LD_Q_SIZE - 1][iSt] = LastStoreAllocTag[iSt];
          LoadExecAckWaitTag[LD_Q_SIZE - 1][iSt] = LastStoreAllocTag[iSt];
          LoadExecAddr[LD_Q_SIZE - 1] = LoadAllocAddr[0];

          const bool ThisStoreSafe =
              (LoadAllocMinTag[0][iSt] <= LastStoreAllocTag[iSt]) &&
              (LoadExecSafeNow[LD_Q_SIZE - 1][iSt] ||
               LoadExecSafeAfterTag[LD_Q_SIZE - 1][iSt] ||
               LoadExecReuse[LD_Q_SIZE - 1][iSt]);
          if constexpr (NUM_STORES > 1)
            IsLoadSafe &= ThisStoreSafe;
          else
            IsLoadSafe = ThisStoreSafe;
        });

        if (IsLoadSafe) {
          // PRINTF("> Safe (%d, %d) (%d, %d, %d)   LastStoreAlloc=(%d, %d)   "
          //        "LoadMinTag=%d\n",
          //        LoadAllocAddr[0], LoadAllocTag[0],
          //        LoadExecSafeNow[LD_Q_SIZE - 1][0],
          //        LoadExecSafeAfterTag[LD_Q_SIZE - 1][0],
          //        LoadExecReuse[LD_Q_SIZE - 1][0], LastStoreAllocAddr[0],
          //        LastStoreAllocTag[0], LoadAllocMinTag[0][0]);

          LoadExecValid[LD_Q_SIZE - 1] = true;
          LoadAllocValid[0] = false;
        }
      }
      /** End Rule */

      /** Rule for returning reused/loaded values. Reuse takes precedence. 
          Any of {Reuse, SafeNow, SafeAfterTag} implies LoadExecValid. */
      bool AllSafeNow = LoadExecSafeNow[0][0];
      bool AnySafeAfterTag = LoadExecSafeAfterTag[0][0];
      bool AnyReuse = LoadExecReuse[0][0];
      uint MaxAckTag = LoadExecAckWaitTag[0][0];
      uint MaxValTag = LoadExecValWaitTag[0][0];
      uint MaxAckTagStoreId = 0;
      uint MaxValTagStoreId = 0;
      UnrolledLoop<1, NUM_STORES>([&](auto iSt) {
        AllSafeNow &= LoadExecSafeNow[0][iSt];
        AnySafeAfterTag |= LoadExecSafeAfterTag[0][iSt];
        AnyReuse |= LoadExecReuse[0][iSt];

        if (LoadExecSafeAfterTag[0][iSt] &&
            (LoadExecAckWaitTag[0][iSt] > MaxAckTag)) {
          MaxAckTag = LoadExecAckWaitTag[0][iSt];
          MaxAckTagStoreId = iSt;
        }
        if (LoadExecReuse[0][iSt] &&
            (LoadExecValWaitTag[0][iSt] > MaxValTag)) {
          MaxValTag = LoadExecValWaitTag[0][iSt];
          MaxValTagStoreId = iSt;
        }
      });

      if (LoadExecValid[0]) {
        if (AnyReuse) {
          if (MaxValTag == LastStoreValTag[MaxValTagStoreId]) {
            LoadMuxPredPipe::write(LD_MUX_REUSE);
            LoadMuxReuseValPipe::write(LastStoreValue[MaxValTagStoreId]);
            LoadExecValid[0] = false;
          }
        } else if (AllSafeNow ||
                  (AnySafeAfterTag &&
                    (MaxAckTag <= StoreAckTag[MaxAckTagStoreId]))) {
          LoadMuxPredPipe::write(LD_MUX_LOAD);
          LoadPortAddrPipe::write(LoadExecAddr[0]);
          LoadExecValid[0] = false;
        }
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

        if (!StoreAllocValid[iSt][0]) {
          StoreAllocAddr[iSt].Shift(uint{0});
          StoreAllocTag[iSt].Shift(uint{0});
          StoreAllocValid[iSt].Shift(bool{false});
        }
        if (!StoreCommitValid[iSt][0]) {
          StoreCommitValid[iSt].Shift(false);
          StoreCommitTag[iSt].Shift(0);
        }

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
            PRINTF("Got st req tag %d\n", StoreReq.tag);
            StoreAllocAddr[iSt][ST_Q_SIZE - 1] = StoreReq.addr;
            StoreAllocTag[iSt][ST_Q_SIZE - 1] = StoreReq.tag;
            StoreAllocValid[iSt][ST_Q_SIZE - 1] = true;
          }
        }
        /** End Rule */

        /** Rule for reading store values and writing to store port. */
        const bool GetNextVal =
            (LoadExecValWaitTag[0][iSt] >= LastStoreValTag[iSt]);
        if (StoreCommitValid[iSt][0] && GetNextVal) {
          bool succ = false;
          auto tryStoreVal = StoreValPipes::template PipeAt<iSt>::read(succ);
          if (succ) {
            PRINTF("Got st val tag %d\n", StoreCommitTag[iSt][0]);
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

    // PRINTF("** DONE Streaming Memory\n");
  });

  return events;
}
