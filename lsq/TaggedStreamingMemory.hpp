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
  bool safeNow;
  bool safeAfterTag;
  bool reuse;
  int tag; 
};


// constexpr int INVALID_ADDR = -1;
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
  using LoadMuxIsReuse = pipe<class _LoadMuxIsReuse, bool, kNumBurstValues>;
  using LoadMuxPred = pipe<class _LoadMuxPred, bool, kNumBurstValues>;
  using LoadMuxMemoryVal = pipe<class _LoadMuxMemoryVal, T, kNumBurstValues>;
  using LoadMuxReuseVal = pipe<class _LoadMuxReuseVal, T, kNumBurstValues>;
  q.single_task<LoadPortKernel<id>>([=]() KERNEL_PRAGMAS {
    [[intel::ivdep]]
    while (LoadPortPred::read()) {
      int Addr = LoadPortAddr::read();
      auto LoadPtr = ext::intel::device_ptr<T>(data + Addr);
      auto Val = BurstCoalescedLSU::load(LoadPtr);
      LoadMuxMemoryVal::write(Val);
    }
    PRINTF("DONE load port %d\n", id);
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
    PRINTF("DONE load MUX %d, reused %d/%d\n", id, NumReused, NumTotal);
  });


  using LoadGatePipe = pipe<class _LoadGatePipe, load_command_t, 16>;
  q.single_task<StreamingLoadKernel<id>>([=]() KERNEL_PRAGMAS {
    auto StoreReq = StoreAddrPipe::read();
    int StoreAddr = StoreReq.addr;
    int StoreTag = StoreReq.tag;
    
    auto LoadReq = LoadAddrPipe::read();
    int LoadAddr = LoadReq.addr;
    int LoadTag = LoadReq.tag;
    bool LoadAddrValid = true;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      if (!LoadAddrValid) {
        auto LoadReq = LoadAddrPipe::read(LoadAddrValid);
        if (LoadAddrValid) {
          // LoadAddrValid = (LoadReq.addr != INVALID_ADDR);
          LoadAddr = LoadReq.addr;
          LoadTag = LoadReq.tag;
        }
      }
      if (LoadAddr == MAX_INT) break;

      const bool SafeNow = (LoadTag < StoreTag) ||
                           (LoadTag == StoreTag && LoadAddr > StoreAddr);
      const bool SafeAfterTag = (LoadTag == StoreTag) ||
                                (LoadTag > StoreTag && LoadAddr <= StoreAddr);
      const bool Reuse = (LoadTag >= StoreTag) && (LoadAddr == StoreAddr);

      const bool AnySafe = (SafeNow || SafeAfterTag);

      if (LoadAddrValid && AnySafe) {
        if constexpr (id == 1)
          PRINTF("SAFE: sld %d  addr %d (SafeNow=%d, SafeAfterTag=%d, reuse=%d)\t StoreAddr=%d, StoreTag=%d\n", id, LoadAddr, SafeNow, SafeAfterTag, Reuse, StoreAddr, StoreTag);
        LoadAddrValid = false;
        LoadGatePipe::write({SafeNow, SafeAfterTag, Reuse, StoreTag});

        LoadMuxPred::write(1);
        LoadMuxIsReuse::write(Reuse);
        if (!Reuse) {
          LoadPortAddr::write(LoadAddr);
        } 
      } 

       /** Rule for reading store {addr, tag} pairs. */
      if (LoadAddr > StoreAddr || Reuse) {
        // TODO: Better delay control
        bool succ = false;
        auto StoreReq = StoreAddrPipe::read(succ);
        if (succ) {
          StoreAddr = StoreReq.addr;
          StoreTag = StoreReq.tag;
          // if constexpr (id == 1)
          //   PRINTF("sld %d got st req (%d, %d)   Curr load (%d, %d)\n", id, StoreAddr, StoreTag, LoadAddr, LoadTag);
        }
      }
      /** End Rule */

    } // end while

    while (StoreAddr != MAX_INT) {
      auto StoreReq = StoreAddrPipe::read();
      StoreAddr = StoreReq.addr;
    }

    LoadMuxPred::write(0);
    LoadGatePipe::write({false, false, false, MAX_INT});

    PRINTF("DONE sld %d\n", id);
  });

  return q.single_task<GateKernel<id>>([=]() KERNEL_PRAGMAS {
    T StoreVal = {};
    int LastStoreValTag = 0;
    int LastStoreAckTag = 0;
    int LastStoreAddrTag = 0;

    load_command_t LoadCmd = {};

    bool LoadCmdValid = false;
    
    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      /** Rule for reading store ACKs. Always enabled. */
      bool succAck = false;
      auto tryStoreAckTag = StoreAckPipe::read(succAck);
      if (succAck) {
        LastStoreAckTag = tryStoreAckTag;
      }
      /** End Rule */

      /** Rule for reading store values. */
      if (LastStoreAddrTag > LastStoreValTag) {
        bool succ = false;
        auto tryStoreValTag = StoreValPipe::read(succ);
        if (succ) {
          StoreVal = tryStoreValTag.val;
          LastStoreValTag = tryStoreValTag.tag;
        }
      }
      /** End Rule */

      /** Rule for load command. */
      if (!LoadCmdValid) {
        auto tryLoadCmd = LoadGatePipe::read(LoadCmdValid);
        if (LoadCmdValid) {
          LoadCmd = tryLoadCmd;
          LastStoreAddrTag = LoadCmd.tag;
          // PRINTF("Load %d LastStoreAddrTag = %d\n", id, LastStoreAddrTag);
        }
      }
      /** End Rule*/
      if (LastStoreAddrTag == MAX_INT) break;

      if (LoadCmdValid) {
        if (LoadCmd.safeAfterTag && LoadCmd.reuse) {
          if (LastStoreAddrTag == LastStoreValTag) {
            if constexpr (id == 1)
              PRINTF("Load 1 wrote reuseVal\n");
            LoadMuxReuseVal::write(StoreVal);
            LoadCmdValid = false;
          }
        } else if (LoadCmd.safeNow || (LoadCmd.safeAfterTag &&
                                       LastStoreAddrTag <= LastStoreAckTag)) {
          if constexpr (id == 1)
            PRINTF("Load 1 wrote LoadPortPred\n");
          LoadPortPred::write(1);
          LoadCmdValid = false;
        }
      }
    } // end while

    while (LastStoreAckTag != MAX_INT || LastStoreValTag != MAX_INT) {
      bool succAck = false;
      auto tryStoreAckTag = StoreAckPipe::read(succAck);
      if (succAck) {
        LastStoreAckTag = tryStoreAckTag;
      }

      bool succVal = false;
      auto tryStoreValTag = StoreValPipe::read(succVal);
      if (succVal) {
        LastStoreValTag = tryStoreValTag.tag;
      }
    }

    LoadPortPred::write(0);
    PRINTF("DONE ld gate %d\n", id);
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
    [[intel::ivdep]]
    while (true) {
      auto AddrTag = AddrTagPipe::read();
      if (AddrTag.addr == MAX_INT)
        break;
      auto Val = ValPipe::read();

      auto StorePtr = ext::intel::device_ptr<T>(data + AddrTag.addr);
      BurstCoalescedLSU::store(StorePtr, Val);
      // PRINTF("Stored addr = %d\n", AddrTag.addr);

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

    OutValPipes::write({MAX_INT});
    AckPipes::write(MAX_INT);

    // Drain acks
    // for (int i = 0; i < NUM_BURST_VALS; ++i) {
    //   if (Ack[i]) {
    //     AckPipes::write(true);
    //   }
    // }

    PRINTF("DONE sst %d\n", id);
  });
}
