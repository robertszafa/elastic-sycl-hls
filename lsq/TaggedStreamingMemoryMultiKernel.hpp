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

/// Unique kernel name generators.
template <int ID> class StreamingLoadKernel;
template <int ID> class LoadPortKernel;
template <int ID> class LoadValMuxKernel;
template <int ID> class StreamingStoreKernel;

template <int id, typename LoadAddrPipe, typename LoadValPipe, 
          typename StoreAddrPipe, typename StoreValPipe, typename StoreAckPipe, 
          typename EndSignalPipe, uint BIT_WIDTH = 32, typename T>
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
    auto StoreReq = StoreAddrPipe::read();
    uint LastStoreAllocAddr = StoreReq.addr;
    uint LastStoreAllocTag = StoreReq.tag;

    uint StoreAckTag = 0;

    T StoreVal = {};
    uint StoreValTag = 0;

    DataBundle<uint, STORE_Q_SIZE> StoreAllocTag(uint{0});
    DataBundle<bool, STORE_Q_SIZE> StoreAllocValid(bool{false});
    StoreAllocTag[STORE_Q_SIZE - 1] = LastStoreAllocTag;
    StoreAllocValid[STORE_Q_SIZE - 1] = true;

    DataBundle<uint, LOAD_Q_SIZE> LoadAddr(uint{0});
    DataBundle<uint, LOAD_Q_SIZE> LoadTag(uint{MAX_INT});
    DataBundle<bool, LOAD_Q_SIZE> LoadAddrValid(bool{false});
    DataBundle<bool, LOAD_Q_SIZE> LoadSafeNow(bool{false});
    DataBundle<bool, LOAD_Q_SIZE> LoadSafeAfterTag(bool{false});
    DataBundle<bool, LOAD_Q_SIZE> LoadReuse(bool{false});
    DataBundle<uint, LOAD_Q_SIZE> LoadWaitForTag(uint{0});

    bool EndSignal = false;
    // uint cycle = 0;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (!EndSignal) {
      // cycle++;

      // Listen for end signal.
      EndSignalPipe::read(EndSignal);

      /** Rule for reading store ACKs. Always enabled */
      bool succ = false;
      auto tryAckAddrTag = StoreAckPipe::read(succ);
      if (succ) {
        StoreAckTag = tryAckAddrTag;
      }
      /** End Rule */
      
      /** Rule for reading store values. */
      if (StoreAllocValid[0]) {
        bool succ = false;
        auto tryStoreValTag = StoreValPipe::read(succ);
        if (succ) {
          StoreVal = tryStoreValTag;
          StoreValTag = StoreAllocTag[0];
          StoreAllocValid[0] = false;
        }
      }
      /** End Rule */

      // TODO: How to delay this if (storeAddr > loadAddr) ?
      /** Rule for reading store {addr, tag} pairs. */
      if (!StoreAllocValid[STORE_Q_SIZE - 1] &&
          (LastStoreAllocAddr <= LoadAddr[0] ||
           LastStoreAllocTag >= LoadTag[0])) {
        bool succ = false;
        auto StoreReq = StoreAddrPipe::read(succ);
        if (succ) {
          LastStoreAllocAddr = StoreReq.addr;
          LastStoreAllocTag = StoreReq.tag;
          StoreAllocValid[STORE_Q_SIZE - 1] = true;
          StoreAllocTag[STORE_Q_SIZE - 1] = LastStoreAllocTag;
        }
      }
      /** End Rule */

      /** Rule for shifting store allocation queue. */
      if (!StoreAllocValid[0]) {
        StoreAllocValid.Shift(bool{false});
        StoreAllocTag.Shift(uint{0});
      }
      /** End Rule */

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

      /** Rule for reusing store value or issuing load. */
      if (LoadAddrValid[0]) {
        if (LoadReuse[0]) {
          if (LoadWaitForTag[0] == StoreValTag) {
            LoadMuxPred::write(uint2_t{LOAD_MUX_PRED::REUSE});
            LoadMuxReuseVal::write(StoreVal);
            LoadAddrValid[0] = false;
          }
        } else if (LoadSafeNow[0] || (LoadSafeAfterTag[0] &&
                                      (LoadWaitForTag[0] <= StoreAckTag))) {
          LoadMuxPred::write(uint2_t{LOAD_MUX_PRED::LOAD});
          LoadPortAddr::write(LoadAddr[0]);
          LoadAddrValid[0] = false;
        } 
      }
      /** End Rule */

      /** Rule for shifting load queues. */
      if (!LoadAddrValid[0]) {
        LoadAddr.Shift(uint{0});
        LoadTag.Shift(uint{0});
        LoadAddrValid.Shift(bool{false});
        LoadSafeNow.Shift(bool{false});
        LoadSafeAfterTag.Shift(bool{false});
        LoadReuse.Shift(bool{false});
        LoadWaitForTag.Shift(uint{0});
      }
      /** End Rule */

      /** Rule for checking load safety and reuse. */
      if (!LoadReuse[0] && !LoadSafeAfterTag[0]) {
        LoadSafeNow[0] = (LoadTag[0] < LastStoreAllocTag ||
                          (LoadTag[0] == LastStoreAllocTag &&
                           LoadAddr[0] != LastStoreAllocAddr));
        LoadSafeAfterTag[0] = (LoadTag[0] >= LastStoreAllocTag &&
                               LoadAddr[0] < LastStoreAllocAddr);
        LoadReuse[0] = (LoadTag[0] >= LastStoreAllocTag) &&
                       (LoadAddr[0] == LastStoreAllocAddr);
        LoadWaitForTag[0] = LastStoreAllocTag;
      }
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
