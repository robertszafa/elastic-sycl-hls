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

struct load_command_t {
  bool safeNow;
  bool safeAfterTag;
  bool reuse;
  uint tag; 
};


/// Unique kernel name generators.
template <int id> class GateKernel;
template <int id> class StreamingLoadKernel;
template <int id> class StreamingStoreKernel;
template <int id> class LoadPortKernel;
template <int id> class StorePortKernel;
template <int id> class LoadValMuxKernel;

template <int id, typename LoadAddrPipe, typename LoadValPipe,
          typename StoreAddrPipe, typename StoreValPipe, typename StoreAckPipe,
          uint BIT_WIDTH = 32, typename T>
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
    PRINTF("DONE load MUX %d, reused %d/%d\n", id, NumReused, NumTotal);
  });


  using LoadGatePipe = pipe<class _LoadGatePipe, load_command_t, 16>;
  q.single_task<StreamingLoadKernel<id>>([=]() KERNEL_PRAGMAS {
    auto StoreReq = StoreAddrPipe::read();
    uint StoreAddr = StoreReq.addr;
    uint StoreTag = StoreReq.tag;
    
    auto LoadReq = LoadAddrPipe::read();
    uint LoadAddr = LoadReq.addr;
    uint LoadTag = LoadReq.tag;
    bool LoadAddrValid = true;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      if (!LoadAddrValid) {
        auto LoadReq = LoadAddrPipe::read(LoadAddrValid);
        if (LoadAddrValid) {
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
        }
      }
      /** End Rule */

    } // end while

    // Drain store addresses.
    while (StoreAddr != MAX_INT) {
      auto StoreReq = StoreAddrPipe::read();
      StoreAddr = StoreReq.addr;
    }

    LoadMuxPred::write(0);
    LoadGatePipe::write({false, false, false, MAX_INT});

    // PRINTF("DONE sld %d\n", id);
  });

  return q.single_task<GateKernel<id>>([=]() KERNEL_PRAGMAS {
    T StoreVal = {};
    uint LastStoreValTag = 0;
    uint LastStoreAckTag = 0;
    uint LastStoreAddrTag = 0;

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
        }
      }
      /** End Rule*/
      if (LastStoreAddrTag == MAX_INT) break;

      if (LoadCmdValid) {
        if (LoadCmd.safeAfterTag && LoadCmd.reuse) {
          if (LastStoreAddrTag == LastStoreValTag) {
            LoadMuxReuseVal::write(StoreVal);
            LoadCmdValid = false;
          }
        } else if (LoadCmd.safeNow || (LoadCmd.safeAfterTag &&
                                       LastStoreAddrTag <= LastStoreAckTag)) {
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
    // PRINTF("DONE ld gate %d\n", id);
  });
}

template <int id, typename AddrTagPipe, typename ValPipe, typename AckPipes,
          typename OutValPipes, uint BIT_WIDTH = 32, typename T>
event StreamingStore(queue &q, T *data) {
  assert(sizeof(T) * 8 == BIT_WIDTH && "Incorrect kBitWidth.");
  constexpr uint NUM_BURST_VALS = (DRAM_BURST_BITS + BIT_WIDTH - 1) / BIT_WIDTH;
  // using t_burst_count = ac_int<BitsForMaxValue<kNumBurstValues>(), false>;

  using StorePortPred = pipe<class _StorePortPred, bool, NUM_BURST_VALS>;
  using StorePortReq = pipe<class _StorePortReq, addr_tag_val_t<T>, NUM_BURST_VALS>;
  auto event = q.single_task<StorePortKernel<id>>([=]() KERNEL_PRAGMAS {
    bool AckValid[NUM_BURST_VALS];
    uint AckTag[NUM_BURST_VALS];
    #pragma unroll
    for (int i = 0; i < NUM_BURST_VALS; ++i) {
      AckValid[i] = false;
      AckTag[i] = 0;
    }

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (StorePortPred::read()) {
      auto Req = StorePortReq::read();
      auto StorePtr = ext::intel::device_ptr<T>(data + Req.addr);
      BurstCoalescedLSU::store(StorePtr, Req.val);

      if (AckValid[0]) {
        AckPipes::write(AckTag[0]);
      }

      #pragma unroll
      for (int i = 0; i < NUM_BURST_VALS - 1; ++i) {
        AckValid[i] = AckValid[i + 1];
        AckTag[i] = AckTag[i + 1];
      }
      AckValid[NUM_BURST_VALS - 1] = true;
      AckTag[NUM_BURST_VALS - 1] = Req.tag;
    }

    // Ensure all stores commited before draining ACKs.
    atomic_fence(memory_order_seq_cst, memory_scope_work_item);

    // Drain acks
    for (int i = 0; i < NUM_BURST_VALS; ++i) {
      if (AckValid[i]) {
        AckPipes::write(AckTag[i]);
      }
    }

    AckPipes::write({MAX_INT});
  });

  return q.single_task<StreamingStoreKernel<id>>([=]() KERNEL_PRAGMAS {
    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      auto AddrTag = AddrTagPipe::read();
      if (AddrTag.addr == MAX_INT)
        break;
      auto Val = ValPipe::read();

      OutValPipes::write({AddrTag.tag, Val});

      StorePortPred::write(1);
      StorePortReq::write({AddrTag.addr, AddrTag.tag, Val});
    }

    StorePortPred::write(0);
    OutValPipes::write({MAX_INT});

    // PRINTF("DONE sst %d\n", id);
  });
}
