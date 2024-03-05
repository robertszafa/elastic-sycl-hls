#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/ext/intel/ac_types/ac_int.hpp>
#include <sycl/sycl.hpp>

#include "unrolled_loop.hpp"
#include "pipe_utils.hpp"
#include "constexpr_math.hpp"
#include "tuple.hpp"
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

using uint2_t = ac_int<2, false>;
enum LOAD_MUX_PRED { TERMINATE, REUSE, LOAD };

template <typename T, uint SIZE> 
inline void InitShiftReg(T (&ShiftReg)[SIZE], const T val) {
  #pragma unroll
  for (uint i = 0; i < SIZE; ++i) {
    ShiftReg[i] = val;
  }
}

template <typename T, uint SIZE> 
inline void DoShiftReg(T (&ShiftReg)[SIZE], const T in) {
  #pragma unroll
  for (uint i = 0; i < SIZE - 1; ++i) {
    ShiftReg[i] = ShiftReg[i + 1];
  }
  ShiftReg[SIZE - 1] = in;
}

/// Unique kernel name generators.
template <int ID> class StreamingLoadKernel;
template <int ID> class LoadPortKernel;
template <int ID> class LoadValMuxKernel;
template <int ID> class StreamingStoreKernel;

template <int id, typename LoadAddrPipe, typename LoadValPipe,
          typename StoreAddrPipes, typename StoreValPipes,
          typename StoreAckPipes, typename EndSignalPipe,
          uint NUM_STORES = 1, uint BIT_WIDTH = 32, typename T>
event StreamingLoad(queue &q, T *data) {
  assert(sizeof(T) * 8 == BIT_WIDTH && "Incorrect kBitWidth.");
  constexpr uint NUM_BURST_VALS = (DRAM_BURST_BITS + BIT_WIDTH - 1) / BIT_WIDTH;
  constexpr uint LOAD_PORT_Q_SIZE = NUM_BURST_VALS * 4;
  constexpr uint STORE_Q_SIZE = NUM_BURST_VALS;
  constexpr uint LOAD_Q_SIZE = 2;

  using LoadPortAddr = pipe<class _LoadAddr, int, LOAD_PORT_Q_SIZE>;
  using LoadMuxMemoryVal = pipe<class _LoadMuxMemoryVal, T, LOAD_PORT_Q_SIZE>;
  q.single_task<LoadPortKernel<id>>([=]() KERNEL_PRAGMAS {
    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      int Addr = LoadPortAddr::read();
      if (Addr == MAX_INT) break;
      auto LoadPtr = ext::intel::device_ptr<T>(data + Addr);
      auto Val = BurstCoalescedLSU::load(LoadPtr);
      LoadMuxMemoryVal::write(Val);
    }
  });
  using LoadMuxPred = pipe<class _LoadMuxPred, uint2_t, LOAD_PORT_Q_SIZE>;
  using LoadMuxReuseVal = pipe<class _LoadMuxReuseVal, T, LOAD_PORT_Q_SIZE>;
  q.single_task<LoadValMuxKernel<id>>([=]() KERNEL_PRAGMAS {
    int NumTotal = 0;
    int NumReused = 0;
    while (true) {
      uint2_t pred = LoadMuxPred::read();

      T val;
      if (pred == uint2_t{LOAD_MUX_PRED::REUSE}) {
        NumReused++;
        val = LoadMuxReuseVal::read();
      } else if (pred == uint2_t{LOAD_MUX_PRED::LOAD}) {
        val = LoadMuxMemoryVal::read();
      } else {
        break;
      }
      NumTotal++;
      LoadValPipe::write(val);
    }
    PRINTF("DONE load MUX, reused %d/%d\n", NumReused, NumTotal);
  });

  return q.single_task<StreamingLoadKernel<id>>([=]() KERNEL_PRAGMAS {
    [[intel::fpga_register]] uint LoadAddr[LOAD_Q_SIZE];
    [[intel::fpga_register]] uint LoadTag[LOAD_Q_SIZE];
    [[intel::fpga_register]] bool LoadAddrValid[LOAD_Q_SIZE];
    InitShiftReg(LoadAddr, uint{0});
    InitShiftReg(LoadTag, uint{MAX_INT});
    InitShiftReg(LoadAddrValid, bool{false});

    [[intel::fpga_register]] bool LoadSafeNow[NUM_STORES][LOAD_Q_SIZE];
    [[intel::fpga_register]] bool LoadSafeAfterTag[NUM_STORES][LOAD_Q_SIZE];
    [[intel::fpga_register]] bool LoadReuse[NUM_STORES][LOAD_Q_SIZE];
    [[intel::fpga_register]] uint LoadWaitForTag[NUM_STORES][LOAD_Q_SIZE];

    [[intel::fpga_register]] uint StoreAllocTagQ[NUM_STORES][STORE_Q_SIZE];
    [[intel::fpga_register]] bool StoreAllocValidQ[NUM_STORES][STORE_Q_SIZE];

    [[intel::fpga_register]] uint LastStoreAllocAddr[NUM_STORES];
    [[intel::fpga_register]] uint LastStoreAllocTag[NUM_STORES];
    [[intel::fpga_register]] uint StoreAckTag[NUM_STORES];
    [[intel::fpga_register]] T StoreVal[NUM_STORES];
    [[intel::fpga_register]] uint StoreValTag[NUM_STORES];

    UnrolledLoop<NUM_STORES>([&](auto iSt) {
      auto StoreReq = StoreAddrPipes::template PipeAt<iSt>::read();

      LastStoreAllocAddr[iSt] = StoreReq.addr;
      LastStoreAllocTag[iSt] = StoreReq.tag;
      StoreAckTag[iSt] = 0;
      StoreVal[iSt] = T{};
      StoreValTag[iSt] = 0;

      InitShiftReg(StoreAllocTagQ[iSt], uint{0});
      InitShiftReg(StoreAllocValidQ[iSt], bool{false});
      StoreAllocTagQ[iSt][STORE_Q_SIZE - 1] = StoreReq.tag;
      StoreAllocValidQ[iSt][STORE_Q_SIZE - 1] = true;

      InitShiftReg(LoadSafeNow[iSt], bool{false});
      InitShiftReg(LoadSafeAfterTag[iSt], bool{false});
      InitShiftReg(LoadReuse[iSt], bool{false});
      InitShiftReg(LoadWaitForTag[iSt], uint{0});
    });

    bool EndSignal = false;
    // uint cycle = 0;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (!EndSignal) {
      // cycle++;

      // Listen for end signal.
      EndSignalPipe::read(EndSignal);

      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        /** Rule for reading store ACKs. Always enabled */
        bool succ = false;
        auto tryAckAddrTag = StoreAckPipes::template PipeAt<iSt>::read(succ);
        if (succ) {
          StoreAckTag[iSt] = tryAckAddrTag;
        }
        /** End Rule */
        
        /** Rule for reading store values. */
        if (StoreAllocValidQ[iSt][0]) {
          bool succ = false;
          auto tryStoreValTag = StoreValPipes::template PipeAt<iSt>::read(succ);
          if (succ) {
            StoreVal[iSt] = tryStoreValTag;
            StoreValTag[iSt] = StoreAllocTagQ[iSt][0];
            StoreAllocValidQ[iSt][0] = false;
          }
        }
        /** End Rule */

        // TODO: How to delay this if (storeAddr > loadAddr) ?
        /** Rule for reading store {addr, tag} pairs. */
        if (!StoreAllocValidQ[iSt][STORE_Q_SIZE - 1] &&
            (LastStoreAllocAddr[iSt] <= LoadAddr[0] ||
             LastStoreAllocTag[iSt] >= LoadTag[0])) {
          bool succ = false;
          auto StoreReq = StoreAddrPipes::template PipeAt<iSt>::read(succ);
          if (succ) {
            LastStoreAllocAddr[iSt] = StoreReq.addr;
            LastStoreAllocTag[iSt] = StoreReq.tag;
            StoreAllocValidQ[iSt][STORE_Q_SIZE - 1] = true;
            StoreAllocTagQ[iSt][STORE_Q_SIZE - 1] = StoreReq.tag;
          }
        }
        /** End Rule */

        /** Rule for shifting store allocation queue. */
        if (!StoreAllocValidQ[iSt][0]) {
          DoShiftReg(StoreAllocValidQ[iSt], bool{false});
          DoShiftReg(StoreAllocTagQ[iSt], uint{0});
        }
        /** End Rule */
      });

      /////////////////////////////////////////////////////////////////////////
      //////////////////////////     LOAD LOGIC     ///////////////////////////
      /////////////////////////////////////////////////////////////////////////

      /** Rule for reading load {addr, tag} pairs. */
      if (!LoadAddrValid[LOAD_Q_SIZE - 1]) {
        auto LoadReq = LoadAddrPipe::read(LoadAddrValid[LOAD_Q_SIZE - 1]);
        LoadAddr[LOAD_Q_SIZE - 1] = LoadReq.addr;
        LoadTag[LOAD_Q_SIZE - 1] = LoadReq.tag;
      }
      /** End Rule */

      /** Rule for checking if we can reuse or load safely. */
      bool IsReuse = false;
      bool IsReuseValValid = false;
      T ReuseVal = T{};
      bool IsLoadSafe = true;
      UnrolledLoop<NUM_STORES>([&](auto iSt) {
        // "Any" reduction to find reuse value.
        if (LoadReuse[iSt][0]) {
          IsReuse = true;
          IsReuseValValid = (LoadWaitForTag[iSt][0] == StoreValTag[iSt]);
          ReuseVal = StoreVal[iSt];
        }

        // "All" reduction to check safety.
        IsLoadSafe &= (LoadSafeNow[iSt][0] ||
                       (LoadSafeAfterTag[iSt][0] &&
                        (LoadWaitForTag[iSt][0] <= StoreAckTag[iSt])));
      });
      /** End Rule */

      /** Rule for reusing store value or issuing load. */
      if (LoadAddrValid[0]) {
        if (IsReuse) {
          if (IsReuseValValid) {
            LoadMuxPred::write(uint2_t{LOAD_MUX_PRED::REUSE});
            LoadMuxReuseVal::write(ReuseVal);
            LoadAddrValid[0] = false;
          }
        } else if (IsLoadSafe) {
          LoadMuxPred::write(uint2_t{LOAD_MUX_PRED::LOAD});
          LoadPortAddr::write(LoadAddr[0]);
          LoadAddrValid[0] = false;
        } 
      }
      /** End Rule */

      /** Rule for shifting load queues. */
      if (!LoadAddrValid[0]) {
        DoShiftReg(LoadAddr, uint{0});
        DoShiftReg(LoadTag, uint{0});
        DoShiftReg(LoadAddrValid, bool{false});

        UnrolledLoop<NUM_STORES>([&](auto iSt) {
          DoShiftReg(LoadSafeNow[iSt], bool{false});
          DoShiftReg(LoadSafeAfterTag[iSt], bool{false});
          DoShiftReg(LoadReuse[iSt], bool{false});
          DoShiftReg(LoadWaitForTag[iSt], uint{0});
        });
      }
      /** End Rule */

      /** Rule for checking load safety and reuse. */
        UnrolledLoop<NUM_STORES>([&](auto iSt) {
        if (!LoadReuse[iSt][0] && !LoadSafeAfterTag[iSt][0]) {
          LoadSafeNow[iSt][0] = (LoadTag[0] < LastStoreAllocTag[iSt] ||
                                 (LoadTag[0] == LastStoreAllocTag[iSt] &&
                                  LoadAddr[0] != LastStoreAllocAddr[iSt]));
          LoadSafeAfterTag[iSt][0] = (LoadTag[0] >= LastStoreAllocTag[iSt] &&
                                      LoadAddr[0] < LastStoreAllocAddr[iSt]);
          LoadReuse[iSt][0] = (LoadTag[0] >= LastStoreAllocTag[iSt]) &&
                              (LoadAddr[0] == LastStoreAllocAddr[iSt]);
          LoadWaitForTag[iSt][0] = LastStoreAllocTag[iSt];
        }
      });
      /** End Rule */

    } // end while

    LoadMuxPred::write(uint2_t{LOAD_MUX_PRED::TERMINATE});
    LoadPortAddr::write(MAX_INT);

    PRINTF("DONE Streaming Load %d\n", id);
  });
}

template <int id, typename AddrTagPipe, typename ValPipe, typename AckPipes,
          uint BIT_WIDTH = 32, typename T>
event StreamingStore(queue &q, T *data) {
  assert(sizeof(T) * 8 == BIT_WIDTH && "Incorrect kBitWidth.");
  constexpr uint NUM_BURST_VALS = (DRAM_BURST_BITS + BIT_WIDTH - 1) / BIT_WIDTH;
  // constexpr uint NUM_BURST_VALS = 32;

  return q.single_task<StreamingStoreKernel<id>>([=]() KERNEL_PRAGMAS {
    DataBundle<uint, NUM_BURST_VALS> AckTag(uint{0});

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      auto AddrTag = AddrTagPipe::read();
      if (AddrTag.addr == MAX_INT) break;
      auto Val = ValPipe::read();

      auto StorePtr = ext::intel::device_ptr<T>(data + AddrTag.addr);
      BurstCoalescedLSU::store(StorePtr, Val);

      AckPipes::write(AckTag[0]);
      AckTag.Shift(AddrTag.tag);
    }
    
    // Ensure all stores commited before draining ACKs.
    atomic_fence(memory_order_seq_cst, memory_scope_work_item);

    // Drain acks
    for (int i = 0; i < NUM_BURST_VALS; ++i) AckPipes::write(AckTag[i]);
    AckPipes::write(MAX_INT);

    PRINTF("DONE Streaming Store %d\n", id);
  });
}
