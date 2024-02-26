#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/ext/intel/ac_types/ac_int.hpp>
#include <sycl/sycl.hpp>

#include "unrolled_loop.hpp"
#include "pipe_utils.hpp"
#include "constexpr_math.hpp"
#include "device_print.hpp"


using namespace sycl;
using namespace fpga_tools;

using BurstCoalescedLSU = ext::intel::lsu<ext::intel::burst_coalesce<true>,
                                          ext::intel::prefetch<false>>;
using PipelinedLSU = sycl::ext::intel::lsu<>;

/// A gated StreamingLoad receives {addr, value, schedule} data on its gate,
/// and stores {addr, value} in a shift-register FIFO for reuse.
#define KERNEL_PRAGMAS [[intel::kernel_args_restrict]] [[intel::max_global_work_dim(0)]] 

constexpr uint STORE_FIFO_SIZE = 4;
constexpr uint DRAM_BURST_BITS = 512;

struct addr_t {
  int addr;
};

struct addr_tag_t {
  int addr;
  int tag;
};


struct addr_tags_t {
  int addr;
  int globalTag;
  int localTag;
};

template <typename T> struct addr_val_t {
  int addr;
  T val;
};

template <typename T> struct tag_val_t {
  int tag;
  T val;
};

template <typename T> struct addr_tag_val_t {
  int addr;
  int tag;
  T val;
};

struct load_command_t {
  bool reuse;
  // Wait for storeValTag if reuse, else wait for store ack.
  int waitForTag; 
  // int waitForStoreAckTag; 
};


constexpr int INVALID_ADDR = -1;
constexpr int MAX_INT = (1<<30);

/// Unique kernel name generators.
template <int id> class GateKernel;
template <int id> class StreamingLoadKernel;
template <int id> class StreamingStoreKernel;
template <int id> class LoadPortKernel;
template <int id> class LoadValMuxKernel;

template <int id, typename LoadAddrPipe, typename LoadValPipe,
          typename StoreAddrPipe, typename StoreValPipe, typename StoreAckPipe,
          int kBitWidth = 32, typename T>
event StreamingLoad(queue &q, T *data) {
  assert(sizeof(T) * 8 == kBitWidth && "Incorrect kBitWidth.");
  constexpr uint kNumBurstValues = (DRAM_BURST_BITS + kBitWidth - 1) / kBitWidth;

  using LoadPortAddr = pipe<class _LoadAddr, int, kNumBurstValues>;
  using LoadPortPred = pipe<class _LoadPortPred, bool, kNumBurstValues>;
  using LoadMuxIsReuse = pipe<class _LoadMuxSelector, bool, kNumBurstValues>;
  using LoadMuxPred = pipe<class _LoadMuxPred, bool, kNumBurstValues>;
  using LoadMuxMemoryVal = pipe<class _LoadMuxMemoryVal, T, kNumBurstValues>;
  using LoadMuxReuseVal = pipe<class _LoadMuxBypassVal, T, kNumBurstValues>;
  q.single_task<LoadPortKernel<id>>([=]() KERNEL_PRAGMAS {
    while (LoadPortPred::read()) {
      int Addr = LoadPortAddr::read();
      auto LoadPtr = ext::intel::device_ptr<T>(data + Addr);
      auto Val = BurstCoalescedLSU::load(LoadPtr);
      LoadMuxMemoryVal::write(Val);
    }
  });
  q.single_task<LoadValMuxKernel<id>>([=]() KERNEL_PRAGMAS {
    while (LoadMuxPred::read()) {
      T val;
      if (LoadMuxIsReuse::read()) {
        val = LoadMuxReuseVal::read();
      } else {
        val = LoadMuxMemoryVal::read();
      }
      LoadValPipe::write(val);
    }
  });


  using LoadGatePipe = pipe<class _LoadGatePipe, load_command_t, 16>;
  return q.single_task<StreamingLoadKernel<id>>([=]() KERNEL_PRAGMAS {
    int StoreAddr = -2;
    int StoreTag = 0;
    
    int LoadAddr = {};
    int LoadTag = {};
    bool LoadAddrValid = false;

    int NumTotal = 0;
    int NumReused = 0;
    // constexpr int MAX_STORE_ADDR_DELAY = 8; 

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      if (LoadAddr == INVALID_ADDR && StoreAddr == INVALID_ADDR) break;

      if (!LoadAddrValid) {
        auto LoadReq = LoadAddrPipe::read(LoadAddrValid);
        if (LoadAddrValid) {
          LoadAddrValid = (LoadReq.addr != INVALID_ADDR);
          LoadAddr = LoadReq.addr;
          LoadTag = LoadReq.tag;
        }
      }

      bool Safe = ((LoadTag <= StoreTag) || (LoadAddr <= StoreAddr));
      bool Reuse = (LoadAddr == StoreAddr);
      
      // if (LoadAddrValid) {
      //   PRINTF("sld %d trying to load addr %d\t StoreAddr=%d, StoreTag=%d\n", id, LoadAddr, StoreAddr, StoreTag);
      // }

      if (LoadAddrValid && Safe) {
        // PRINTF("sld %d SAFE addr %d (reuse=%d)\t StoreAddr=%d, StoreTag=%d\n", id, LoadAddr, Reuse, StoreAddr, StoreTag);
        LoadAddrValid = false;
        int WaitForTag = (LoadTag < StoreTag) ? 0 : StoreTag;
        LoadGatePipe::write({Reuse, WaitForTag});

        LoadMuxPred::write(1);
        LoadMuxIsReuse::write(Reuse);
        if (!Reuse) {
          LoadPortAddr::write(LoadAddr);
        } else {
          NumReused++;
        }
        NumTotal++;
      } 
      
      // TODO: Better delay control
      // if (!(Safe && LoadAddr < StoreAddr-MAX_STORE_ADDR_DELAY)) {
      if (!(Safe && LoadAddr < StoreAddr)) {
        bool succ = false;
        auto StoreReq = StoreAddrPipe::read(succ);
        if (succ) {
          StoreAddr = StoreReq.addr;
          StoreTag = StoreReq.tag;
        }
      }
    } // end while

    LoadMuxPred::write(0);
    LoadGatePipe::write({1, MAX_INT});

    // PRINTF("DONE sld %d: reused %d/%d\n", id, NumReused, NumTotal);
  });

  return q.single_task<GateKernel<id>>([=]() KERNEL_PRAGMAS {
    T StoreVal = {};
    int StoreValTag = 0;
    int StoreAckTag = 0;
    int WaitForAckTag = 0;
    int WaitForValTag = 1;

    load_command_t LoadCmd = {};

    bool LoadCmdValid = false;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      if (WaitForValTag == MAX_INT)
        break;

      /** Rule for reading store ACKs. Always enabled. */
      bool succAck = false;
      auto tryStoreAckTag = StoreAckPipe::read(succAck);
      if (succAck) {
        StoreValTag = tryStoreAckTag;
      }
      /** End Rule */

      /** Rule for reading store values. */
      if (WaitForValTag > StoreValTag) {
        bool succ = false;
        auto tryStoreValTag = StoreValPipe::read(succ);
        if (succ) {
          StoreVal = tryStoreValTag.val;
          StoreValTag = tryStoreValTag.tag;
        }
      }
      /** End Rule */

      /** Rule for load command. */
      if (!LoadCmdValid) {
        auto tryLoadCmd = LoadGatePipe::read(LoadCmdValid);
        if (LoadCmdValid) {
          LoadCmd = tryLoadCmd;
          WaitForValTag = LoadCmd.waitForTag; 
          if (!LoadCmd.reuse)
            WaitForAckTag = LoadCmd.waitForTag; 

          PRINTF("Gate %d waiting on ValTag = %d, AckTag = %d\n", id, WaitForValTag, WaitForAckTag);
        }
      }
      /** End Rule*/

      if (LoadCmd.reuse && WaitForValTag == StoreValTag) {
        LoadMuxReuseVal::write(StoreVal);
        LoadCmdValid = false;
      } else if (WaitForAckTag <= StoreAckTag) {
        LoadPortPred::write(1);
        LoadCmdValid = false;
      }
    } // end while

    PRINTF("DONE ld gate %d\n", id);
    LoadPortPred::write(0);
  });
}

template <int id, typename AddrTagPipe, typename ValPipe, typename AckPipes,
          typename OutValPipes, int BIT_WIDTH = 32, typename T>
event StreamingStore(queue &q, T *data) {
  assert(sizeof(T) * 8 == BIT_WIDTH && "Incorrect kBitWidth.");
  // constexpr uint NUM_BURST_VALS = (DRAM_BURST_BITS + BIT_WIDTH - 1) / BIT_WIDTH;
  // constexpr uint NUM_BURST_VALS = 2;
  // using t_burst_count = ac_int<BitsForMaxValue<kNumBurstValues>(), false>;

  return q.single_task<StreamingStoreKernel<id>>([=]() KERNEL_PRAGMAS {
    // bool Ack[NUM_BURST_VALS];
    // #pragma unroll
    // for (int i = 0; i < NUM_BURST_VALS; ++i) Ack[i] = false;

    // int numAck = 0;
    while (true) {
      auto AddrTag = AddrTagPipe::read();
      if (AddrTag.addr == INVALID_ADDR)
        break;
      auto Val = ValPipe::read();

      auto StorePtr = ext::intel::device_ptr<T>(data + AddrTag.addr);
      BurstCoalescedLSU::store(StorePtr, Val);
      PRINTF("Stored addr = %d\n", AddrTag.addr);

      // if (Ack[0]) {
        OutValPipes::write({AddrTag.tag, Val});
        AckPipes::write(AddrTag.tag);
      //   numAck++;
      //   PRINTF("Store wrote ack num %d\n", numAck);
      // }

      // #pragma unroll
      // for (int i = 0; i < NUM_BURST_VALS - 1; ++i) Ack[i] = Ack[i + 1];
      // Ack[NUM_BURST_VALS - 1] = true;
    }

    // Drain acks
    // for (int i = 0; i < NUM_BURST_VALS; ++i) {
    //   if (Ack[i]) {
    //     AckPipes::write(true);
    //   }
    // }

    PRINTF("DONE sst %d\n", id);
  });
}
