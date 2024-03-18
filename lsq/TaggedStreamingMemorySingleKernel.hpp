#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/ext/intel/ac_types/ac_int.hpp>
#include <sycl/sycl.hpp>

#include "unrolled_loop.hpp"
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

struct addr_tag_mintag_t {
  uint addr;
  uint tag;
  /// Minimum tag that a store dependency has to have.
  uint mintag;
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
          uint NUM_LOADS, uint BIT_WIDTH = 32, typename T>
std::vector<event> StreamingMemory(queue &q, T *data) {
  std::vector<event> events(1);

  assert(sizeof(T) * 8 == BIT_WIDTH && "Incorrect kBitWidth.");
  constexpr uint BURST_SIZE = (DRAM_BURST_BITS + BIT_WIDTH - 1) / BIT_WIDTH;

  // Store port.
  using StorePortAckPipe = pipe<class _StorePortAck, int, BURST_SIZE*4>;
  events[0] = q.single_task<StorePortKernel<MEM_ID, 0>>([=]() KERNEL_PRAGMAS {
    DataBundle<uint, BURST_SIZE> AckTag(uint{0});

    addr_tag_t Req = addr_tag_t{};
    bool ReqValid = false;
    T Val = T{};
    bool ValValid = false;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      if (!ReqValid) {
        auto tryReq = StoreAddrPipes::template PipeAt<0>::read(ReqValid);
        if (ReqValid) Req = tryReq;
      }
      if (Req.addr == MAX_INT) break;
      
      if (!ValValid) {
        auto tryVal = StoreValPipes::template PipeAt<0>::read(ValValid);
        if (ValValid) Val = tryVal;
      }

      if (ReqValid && ValValid) {
        auto StorePtr = ext::intel::device_ptr<T>(data + Req.addr);
        BurstCoalescedLSU::store(StorePtr, Val);
        ReqValid = false;
        ValValid = false;
      }

      StorePortAckPipe::write(AckTag[0]);
      AckTag.Shift(Req.tag);
    }
    // Drain acks
    atomic_fence(memory_order_seq_cst, memory_scope_work_item); // force burst
    for (int i = 0; i < BURST_SIZE; ++i) {
      StorePortAckPipe::write(AckTag[i]);
    }
    // StorePortAckPipe::write(MAX_INT);
  });

  // Types for specifying action in load mux.
  using ld_mux_pred_t = ac_int<2, false>;
  constexpr ld_mux_pred_t LD_MUX_TERMINATE = ld_mux_pred_t{0};
  constexpr ld_mux_pred_t LD_MUX_REUSE = ld_mux_pred_t{1};
  constexpr ld_mux_pred_t LD_MUX_LOAD = ld_mux_pred_t{2};
  // Pipes for load port and load mux.
  using LoadPortAddrPipe = pipe<class _LoadAddr, int, BURST_SIZE*4>;
  using LoadMuxLoadValPipe = pipe<class _LoadMuxMemoryVal, T, BURST_SIZE*4>;
  using LoadMuxPredPipe = pipe<class _LoadMuxPred, ld_mux_pred_t, BURST_SIZE*4>;
  using LoadMuxReuseValPipe = pipe<class _LoadMuxReuseVal, T, BURST_SIZE*4>;
  q.single_task<LoadPortKernel<MEM_ID, 0>>([=]() KERNEL_PRAGMAS {
    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      int Addr = LoadPortAddrPipe::read();
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

      LoadValPipe::write(Val);
      NumTotal++;
    }
    PRINTF("** DONE load MUX, reused %d/%d\n", NumReused, NumTotal);
  });

  q.single_task<StreamingMemoryKernel<MEM_ID>>([=]() KERNEL_PRAGMAS {
    constexpr uint ST_Q_SIZE = 2;
    DataBundle<uint, ST_Q_SIZE> StoreAllocAddr(uint{0});
    DataBundle<uint, ST_Q_SIZE> StoreAllocTag(uint{1});
    DataBundle<bool, ST_Q_SIZE> StoreAllocValid(bool{false});
    uint LastStoreAllocAddr = uint{0};
    uint LastStoreAllocTag = uint{1};

    T LastStoreValue = T{};
    uint LastStoreValTag = uint{0};
    DataBundle<uint, ST_Q_SIZE> StoreCommitTag(uint{0});
    DataBundle<uint, ST_Q_SIZE> StoreCommitValid(bool{false});

    uint StoreAckTag = uint{0};

    constexpr uint LD_Q_SIZE = 2;
    DataBundle<uint, LD_Q_SIZE> LoadAllocAddr(uint{1});
    DataBundle<uint, LD_Q_SIZE> LoadAllocTag(uint{0});
    DataBundle<bool, LD_Q_SIZE> LoadAllocValid(bool{false});
    
    DataBundle<bool, LD_Q_SIZE> LoadExecSafeNow(bool{false});
    DataBundle<bool, LD_Q_SIZE> LoadExecSafeAfterTag(bool{false});
    DataBundle<bool, LD_Q_SIZE> LoadExecReuse(bool{false});
    DataBundle<uint, LD_Q_SIZE> LoadExecWaitForTag(uint{0});
    DataBundle<uint, LD_Q_SIZE> LoadExecAddr(uint{0});
    DataBundle<uint, LD_Q_SIZE> LoadExecValid(bool{false});

    bool EndSignal = false;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (!EndSignal) {
      if (!StoreAllocValid[0])
        EndSignalPipe::read(EndSignal);

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////     LOAD LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////

      /** Rule for shifting load allocation queue. */ 
      if (!LoadAllocValid[0]) {
        LoadAllocAddr.Shift(uint{0});
        LoadAllocTag.Shift(uint{0});
        LoadAllocValid.Shift(bool{false});
      }
      /** End Rule */

      /** Rule for shifting load execution queue. */ 
      if (!LoadExecValid[0]) {
        LoadExecSafeNow.Shift(bool{false});
        LoadExecSafeAfterTag.Shift(bool{false});
        LoadExecReuse.Shift(bool{false});
        LoadExecWaitForTag.Shift(uint{0});
        LoadExecValid.Shift(uint{0});
        LoadExecAddr.Shift(uint{0});
      }
      /** End Rule */

      /** Rule for reading new load allocation: {addr, tag} pairs. */
      if (!LoadAllocValid[LD_Q_SIZE - 1]) {
        bool succ = false;
        auto LoadReq = LoadAddrPipe::read(succ);
        if (succ) {
          LoadAllocAddr[LD_Q_SIZE - 1] = LoadReq.addr;
          LoadAllocTag[LD_Q_SIZE - 1] = LoadReq.tag;
          LoadAllocValid[LD_Q_SIZE - 1] = true;
        }
      }
      /** End Rule */

      /** Rule for checking load safety and reuse. */
      if (LoadAllocValid[0] && !LoadExecValid[LD_Q_SIZE - 1]) {
        LoadExecSafeNow[LD_Q_SIZE - 1] = (LoadAllocTag[0] < LastStoreAllocTag);
        LoadExecSafeAfterTag[LD_Q_SIZE - 1] =
            (LoadAllocTag[0] == LastStoreAllocTag ||
             (LoadAllocTag[0] > LastStoreAllocTag &&
              LoadAllocAddr[0] < LastStoreAllocAddr));
        LoadExecReuse[LD_Q_SIZE - 1] = (LoadAllocTag[0] >= LastStoreAllocTag) &&
                                       (LoadAllocAddr[0] == LastStoreAllocAddr);
        LoadExecWaitForTag[LD_Q_SIZE - 1] = LastStoreAllocTag;

        const bool AnySafe = (LoadExecSafeNow[LD_Q_SIZE - 1] ||
                              LoadExecSafeAfterTag[LD_Q_SIZE - 1] ||
                              LoadExecReuse[LD_Q_SIZE - 1]);
        LoadExecAddr[LD_Q_SIZE - 1] = LoadAllocAddr[0];
        LoadExecValid[LD_Q_SIZE - 1] = AnySafe;
        LoadAllocValid[0] = !AnySafe;
      }
      /** End Rule */

      /** Rule for returning reused/loaded value via the load MUX kernel.  
          Reuse takes precedence over loading, in case both are true. 
          Any of {Reuse, SafeNow, SafeAfterTag} implies LoadExecValid. */
      if (LoadExecReuse[0]) {
        if (LoadExecWaitForTag[0] == LastStoreValTag) {
          LoadMuxPredPipe::write(LD_MUX_REUSE);
          LoadMuxReuseValPipe::write(LastStoreValue);
          LoadExecValid[0] = false;
        }
      } else if (LoadExecSafeNow[0] ||
                 (LoadExecSafeAfterTag[0] &&
                  (LoadExecWaitForTag[0] <= StoreAckTag))) {
        LoadMuxPredPipe::write(LD_MUX_LOAD);
        LoadPortAddrPipe::write(LoadExecAddr[0]);
        LoadExecValid[0] = false;
      }
      /** End Rule */

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////    STORE LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////

      /** Rule for always listining for store ACKs. */
      bool succ = false;
      auto tryAckTag = StorePortAckPipe::read(succ);
      if (succ) StoreAckTag = tryAckTag;
      /** End Rule */

      if (!StoreAllocValid[0]) {
        StoreAllocAddr.Shift(uint{0});
        StoreAllocTag.Shift(uint{0});
        StoreAllocValid.Shift(bool{false});
      }
      if (!StoreCommitValid[0]) {
        StoreCommitValid.Shift(false);
        StoreCommitTag.Shift(0);
      }

      /** Rule for moving store allocation to store commit queue. */
      const bool GetNextStoreAlloc = (LastStoreAllocAddr <= LoadAllocAddr[0] || 
                                      LastStoreAllocTag > LoadAllocTag[0]);
      if (GetNextStoreAlloc) {
        if (StoreAllocValid[0] && !StoreCommitValid[ST_Q_SIZE - 1]) {
          LastStoreAllocAddr = StoreAllocAddr[0];
          LastStoreAllocTag = StoreAllocTag[0];

          StoreCommitValid[ST_Q_SIZE - 1] = true;
          StoreCommitTag[ST_Q_SIZE - 1] = StoreAllocTag[0];

          StoreAllocValid[0] = false;
        }
      }
      /** End Rule */

      /** Rule for reading store {addr, tag} pairs. */
      if (!StoreAllocValid[ST_Q_SIZE - 1]) {
        bool succ = false;
        auto StoreReq = StoreAddrPipes::template PipeAt<1>::read(succ);
        if (succ) {
          StoreAllocAddr[ST_Q_SIZE - 1] = StoreReq.addr;
          StoreAllocTag[ST_Q_SIZE - 1] = StoreReq.tag;
          StoreAllocValid[ST_Q_SIZE - 1] = true;
        }
      }
      /** End Rule */

      /** Rule for reading store values and writing to store port. */
      const bool GetNextStoreVal = (LoadExecWaitForTag[0] >= LastStoreValTag);
      if (StoreCommitValid[0] && GetNextStoreVal) {
        bool succ = false;
        auto tryStoreVal = StoreValPipes::template PipeAt<1>::read(succ);
        if (succ) {
          LastStoreValTag = StoreCommitTag[0];
          LastStoreValue = tryStoreVal;
          StoreCommitValid[0] = false;
        }
      }
      /** End Rule */

    } // end while

    LoadMuxPredPipe::write(LD_MUX_TERMINATE);
    LoadPortAddrPipe::write(MAX_INT);

    // PRINTF("** DONE Streaming Memory\n");
  });

  return events;
}
