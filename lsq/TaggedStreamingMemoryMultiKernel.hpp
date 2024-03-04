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



/// Unique kernel name generators.
template <int MemId> class StreamingLoadKernel;
template <int MemId> class LoadPortKernel;
template <int MemId> class StorePortKernel;
template <int MemId> class LoadValMuxKernel;
template <int MemId> class StreamingStoreKernel;

template <int id, typename LoadAddrPipe, typename LoadValPipe, 
          typename StoreAddrPipe, typename StoreValPipe, typename StoreAckPipe, 
          typename EndSignalPipe, uint BIT_WIDTH = 32, typename T>
event StreamingLoad(queue &q, T *data) {
  assert(sizeof(T) * 8 == BIT_WIDTH && "Incorrect kBitWidth.");
  constexpr uint NUM_BURST_VALS = (DRAM_BURST_BITS + BIT_WIDTH - 1) / BIT_WIDTH;

  using LoadPortAddr = pipe<class _LoadAddr, int, NUM_BURST_VALS>;
  using LoadPortPred = pipe<class _LoadPortPred, bool, NUM_BURST_VALS>;
  using LoadMuxIsReuse = pipe<class _LoadMuxIsReuse, bool, NUM_BURST_VALS>;
  using LoadMuxPred = pipe<class _LoadMuxPred, bool, NUM_BURST_VALS>;
  using LoadMuxMemoryVal = pipe<class _LoadMuxMemoryVal, T, NUM_BURST_VALS>;
  using LoadMuxReuseVal = pipe<class _LoadMuxReuseVal, T, NUM_BURST_VALS>;
  q.single_task<LoadPortKernel<id>>([=]() KERNEL_PRAGMAS {
    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (LoadPortPred::read()) {
      int Addr = LoadPortAddr::read();
      auto LoadPtr = ext::intel::device_ptr<T>(data + Addr);
      auto Val = BurstCoalescedLSU::load(LoadPtr);
      LoadMuxMemoryVal::write(Val);
    }
    // Ensure all stores commited before finishing kernel.
    // PRINTF("DONE load port %d\n", id);
  });
  q.single_task<LoadValMuxKernel<id>>([=]() KERNEL_PRAGMAS {
    int NumTotal = 0;
    int NumReused = 0;
    while (LoadMuxPred::read()) {
      NumTotal++;
      T val;
      if (LoadMuxIsReuse::read()) {
        NumReused++;
        val = LoadMuxReuseVal::read();
      } else {
        val = LoadMuxMemoryVal::read();
      }
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

    DataBundle<uint, NUM_BURST_VALS> StoreAllocTag(uint{0});
    DataBundle<bool, NUM_BURST_VALS> StoreAllocValid(bool{false});
    StoreAllocTag[NUM_BURST_VALS-1] = LastStoreAllocTag;
    StoreAllocValid[NUM_BURST_VALS-1] = true;

    // auto LoadReq = LoadAddrPipe::read();
    constexpr uint LOAD_Q_SIZE = 4;
    DataBundle<uint, LOAD_Q_SIZE> LoadAddr(uint{0});
    DataBundle<uint, LOAD_Q_SIZE> LoadTag(uint{0});
    DataBundle<bool, LOAD_Q_SIZE> LoadAddrValid(bool{false});
    DataBundle<bool, LOAD_Q_SIZE> LoadSafeNow(bool{false});
    DataBundle<bool, LOAD_Q_SIZE> LoadSafeAfterTag(bool{false});
    DataBundle<bool, LOAD_Q_SIZE> LoadReuse(bool{false});
    DataBundle<uint, LOAD_Q_SIZE> LoadWaitForTag(uint{0});

    DataBundle<bool, LOAD_Q_SIZE> LoadPrinted(bool{false});

    bool EndSignal = false;
    uint cycle = 0;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (!EndSignal) {
      cycle++;

      // Listen for end signal.
      EndSignalPipe::read(EndSignal);

      /** Rule for reading store ACKs. Always enabled */
      bool succ = false;
      auto tryAckAddrTag = StoreAckPipe::read(succ);
      if (succ) {
        StoreAckTag = tryAckAddrTag;
        // PRINTF("@%d: StoreAck %d \n", cycle, StoreAckTag);
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

      /** Rule for shifting store allocation queue. */
      if (!StoreAllocValid[0]) {
        StoreAllocValid.Shift(bool{false});
        StoreAllocTag.Shift(uint{0});
      }
      /** End Rule */

      // TODO: How to delay this if (storeAddr > loadAddr) ?
      /** Rule for reading store {addr, tag} pairs. */
      if (!StoreAllocValid[NUM_BURST_VALS - 1] &&
          (LastStoreAllocAddr < LoadAddr[0])) {
        bool succ = false;
        auto StoreReq = StoreAddrPipe::read(succ);
        if (succ) {
          LastStoreAllocAddr = StoreReq.addr;
          LastStoreAllocTag = StoreReq.tag;
          StoreAllocValid[LOAD_Q_SIZE - 1] = true;
          StoreAllocTag[LOAD_Q_SIZE - 1] = LastStoreAllocTag;
        }
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
            LoadMuxPred::write(1);
            LoadMuxIsReuse::write(1);
            LoadMuxReuseVal::write(StoreVal);

            LoadAddrValid[0] = false;
            // PRINTF("@%d: returned (reused) %d\n", cycle, LoadAddr[0]);
          }
        } else if (LoadSafeNow[0] || (LoadSafeAfterTag[0] &&
                                      (LoadWaitForTag[0] <= StoreAckTag))) {
          LoadMuxPred::write(1);
          LoadMuxIsReuse::write(0);
          LoadPortPred::write(1);
          LoadPortAddr::write(LoadAddr[0]);

          LoadAddrValid[0] = false;
          // PRINTF("@%d: returned %d\n", cycle, LoadAddr[0]);
        } 
        // else if (!LoadPrinted[0]){
        //   LoadPrinted[0] = true;
        //   PRINTF("@%d: load (%d, %d) waits for ack %d (curr rcvd ack %d) (reuse=%d, LoadSafeAfterTag=%d)\n", cycle, LoadAddr[0], LoadTag[0], LoadWaitForTag[0], StoreAckTag, LoadReuse[0], LoadSafeAfterTag[0]);
        // }
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
        // PRINTF("@%d: LoadAddr[0] = %d, LoadTag[0] = %d\n", cycle, LoadAddr[0], LoadTag[0]);

        LoadPrinted.Shift(bool{false});
      }
      /** End Rule */

      /** Rule for checking load safety and reuse. */
      if (!LoadReuse[0] && !LoadSafeAfterTag[0]) {
        LoadSafeNow[0] = (LoadTag[0] < LastStoreAllocTag ||
                          (LoadTag[0] == LastStoreAllocTag &&
                            LoadAddr[0] != LastStoreAllocAddr));
        LoadSafeAfterTag[0] = (LoadTag[0] >= LastStoreAllocTag &&
                               LoadAddr[0] < LastStoreAllocAddr);
        // (LoadTag[0] == LastStoreAllocTag) || // Covered by LoadReuse & Safe Now
        LoadReuse[0] = (LoadTag[0] >= LastStoreAllocTag) &&
                      (LoadAddr[0] == LastStoreAllocAddr);
        LoadWaitForTag[0] = LastStoreAllocTag;
      }
      /** End Rule */

    } // end while

    LoadMuxPred::write(0);
    LoadPortPred::write(0);

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
    // AckPipes::write(MAX_INT);

    PRINTF("DONE Streaming Store %d\n", id);
  });
}
