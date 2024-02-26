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

template <typename T> struct addr_tag_val_t {
  int addr;
  int tag;
  T val;
};

struct load_command_t {
  bool reuseStoreVal;
  // Wait for storeValTag if reuse, else wait for store ack.
  int waitForStoreValOrAck; 
}


constexpr int INVALID_ADDR = -1;
constexpr int MAX_INT = (1<<30);

/// Unique kernel name generators.
template <int id> class StreamingLoadKernel;
template <int id> class AddressComparisonKernel;
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


  // using LoadGatePipe = pipe<class _LoadGatePipe, load_command_t, 16>;
  // return q.single_task<AddressComparisonKernel<id>>([=]() KERNEL_PRAGMAS {
  //   uint8_t CountStoreAddr = 0;

  //   int StoreAddr = -1;
  //   int StoreTag = 0;
    
  //   int LoadAddr = {};
  //   int LoadTag = {};
  //   bool LoadAddrValid = false;

  //   constexpr int MAX_STORE_ADDR_DELAY = 8; 

  //   [[intel::ivdep]]
  //   [[intel::initiation_interval(1)]]
  //   [[intel::speculated_iterations(0)]]
  //   while (true) {
  //     if (LoadAddr == INVALID_ADDR && StoreAddr == MAX_INT) break;

  //     if (!LoadAddrValid) {
  //       auto LoadReq = LoadAddrPipe::read(LoadAddrValid);
  //       LoadAddr = LoadReq.addr;
  //       LoadTag = LoadReq.tag;
  //     }

  //     bool Safe = (LoadTag <= StoreTag) || (LoadAddr <= StoreAddr);
  //     bool Reuse = (LoadAddr == StoreAddr);
  //       // if (LoadAddr[0] < StoreAddr) {
  //       //   LoadWaitingForAckNum[0] = CountStoreAddr;
  //       // }
    
  //     if (Safe && LoadAddr != INVALID_ADDR) {
  //       int WaitForStoreNum = (LoadTag < StoreTag) ? 0 : CountStoreAddr;
  //       LoadGatePipe::write({Reuse, WaitForStoreNum});

  //       if (!Reuse)
  //         LoadPortAddr::write(LoadAddr);
  //     } 
      
  //     // TODO: Better delay control
  //     if (!(Safe && LoadAddr < StoreAddr-MAX_STORE_ADDR_DELAY)) {
  //       bool succ = false;
  //       auto StoreReq = StoreAddrPipe::read(succ);
  //       if (succ) {
  //         CountStoreAddr++;
  //         StoreAddr = StoreReq.addr;
  //         StoreTag = StoreReq.tag;
  //       }
  //     }
  //   } // end while
  // });

  return q.single_task<StreamingLoadKernel<id>>([=]() KERNEL_PRAGMAS {
    uint8_t CountStoreAddr = 0;
    uint8_t CountStoreVal = 0;
    uint8_t CountStoreAck = 0;

    int StoreAddr = -1;
    int StoreTag = 0;
    T StoreVal = {};
    // bool StoreAddrValid = false;
    // bool StoreValValid = false;

    constexpr int kLoadFifoSize = 2;
    int LoadAddr[kLoadFifoSize];
    int LoadTag[kLoadFifoSize];
    bool LoadAddrValid[kLoadFifoSize];
    bool LoadSafe[kLoadFifoSize];
    bool LoadReuse[kLoadFifoSize];
    bool LoadWaitingForAckNum[kLoadFifoSize];
    bool LoadWaitingForValNum[kLoadFifoSize];
    #pragma unroll 
    for (int i = 0; i < kLoadFifoSize; ++i) {
      LoadAddrValid[i] = false;
      LoadAddr[i] = {};
      LoadTag[i] = {};
    }

    int NumTotal = 0;
    int NumReused = 0;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]]
    [[intel::speculated_iterations(0)]]
    while (true) {
      bool succAck = false;
      StoreAckPipe::read(succAck);
      if (succAck) {
        CountStoreAck++;
        if constexpr (id == 1)
          PRINTF("Load 1 got ack num %d\n", CountStoreAck);
      }

      if (!LoadAddrValid[kLoadFifoSize - 1]) {
        auto LoadReq = LoadAddrPipe::read(LoadAddrValid[kLoadFifoSize - 1]);
        LoadAddr[kLoadFifoSize - 1] = LoadReq.addr;
        LoadTag[kLoadFifoSize - 1] = LoadReq.tag;
      }

      if (!LoadAddrValid[0]) {
        #pragma unroll
        for (int i = 0; i < kLoadFifoSize - 1; ++i) {
          LoadAddrValid[i] = LoadAddrValid[i + 1];
          LoadAddr[i] = LoadAddr[i + 1];
          LoadTag[i] = LoadTag[i + 1];
          LoadSafe[i] = LoadSafe[i + 1];
          LoadReuse[i] = LoadReuse[i + 1];
          LoadWaitingForAckNum[i] = LoadWaitingForAckNum[i + 1];
          LoadWaitingForValNum[i] = LoadWaitingForValNum[i + 1];
        }
        LoadAddr[kLoadFifoSize - 1] = {};
        LoadTag[kLoadFifoSize - 1] = {};
        LoadAddrValid[kLoadFifoSize - 1] = false;
        LoadSafe[kLoadFifoSize - 1] = false;
        LoadReuse[kLoadFifoSize - 1] = false;
        LoadWaitingForAckNum[kLoadFifoSize - 1] = 0;
        LoadWaitingForValNum[kLoadFifoSize - 1] = 0;
      }

      // if (LoadAddr[0] == INVALID_ADDR) break;

      if (!LoadSafe[0]) {
        if (LoadTag[0] <= StoreTag) {
          LoadSafe[0] = true;
        } else if (LoadAddr[0] <= StoreAddr) {
          LoadSafe[0] = true;
        } 

        if (LoadAddr[0] == StoreAddr) {
          LoadReuse[0] = true;
          LoadWaitingForValNum[0] = CountStoreAddr;
        } else if (LoadAddr[0] < StoreAddr) {
          LoadWaitingForAckNum[0] = CountStoreAddr;
        }
        // if constexpr (id == 1)
        //   PRINTF("Load 1 checking addr = %d (safe = %d, reuse = %d, WaitingForVal = %d, WaitingForAck = %d)\tStCounts (%d, %d, %d)\n", LoadAddr[0], LoadSafe[0], LoadReuse[0], LoadWaitingForValNum[0], LoadWaitingForAckNum[0], CountStoreAddr, CountStoreVal, CountStoreAck);
      }

      bool DoLoad = true;
      if constexpr (id == 1) {
        DoLoad = LoadSafe[0] && 
                      (CountStoreVal >= LoadWaitingForValNum[0]) &&
                      (CountStoreAck >= LoadWaitingForAckNum[0]);
      }
      if (DoLoad && (LoadAddr[0] != INVALID_ADDR)) {
        LoadAddrValid[0] = false;
        // if constexpr (id == 1)
        //   PRINTF("Load %d read addr = %d (reuse = %d)\tStCounts (%d, %d, %d)\n", id, LoadAddr[0], LoadReuse[0], CountStoreAddr, CountStoreVal, CountStoreAck);

        LoadMuxPred::write(1);
        LoadMuxIsReuse::write(LoadReuse[0]);
        if (LoadReuse[0]) {
          LoadMuxReuseVal::write(StoreVal);
          NumReused++;
        } else {
          LoadPortPred::write(1);
          LoadPortAddr::write(LoadAddr[0]);
        }
        NumTotal++;
      }

      if (CountStoreVal < CountStoreAddr) {
        bool succ = false;
        auto tryStoreVal = StoreValPipe::read(succ);
        if (succ) {
          StoreVal = tryStoreVal;
          CountStoreVal++;
          // if constexpr (id == 1) 
          //   PRINTF("Load 1 has stVal num  %d\n", CountStoreVal);
        }
      }
      
      if ((LoadAddrValid[0] && !LoadSafe[0]) || (DoLoad && StoreAddr <= LoadAddr[0]) || LoadAddr[0] == INVALID_ADDR) {
        bool succ = false;
        auto tryStoreAddr = StoreAddrPipe::read(succ);
        if (succ) {
          StoreAddr = tryStoreAddr.addr;
          StoreTag = tryStoreAddr.tag;
          CountStoreAddr++;
          // if constexpr (id == 1) 
          //   PRINTF("Load 1 got stAddr = %d\tTrying to load addr, tag = %d, %d\n", StoreAddr, LoadAddr[0], LoadTag[0]);
        }
      }

      if (LoadAddr[0] == INVALID_ADDR && StoreAddr == MAX_INT && CountStoreAck == CountStoreAddr)
        break;
    } // end while

    // Terminates all st/ld ports and ld muxes. 
    LoadPortPred::write(0);
    LoadMuxPred::write(0);

    PRINTF("Load %d reused %d/%d\n", id, NumReused, NumTotal);
    // PRINTF("sld %d done\n", id);
  });
}

template <int id, typename AddrValPipe, typename AckPipes, int BIT_WIDTH = 32,
          typename T>
event StreamingStore(queue &q, T *data) {
  assert(sizeof(T) * 8 == BIT_WIDTH && "Incorrect kBitWidth.");
  constexpr uint NUM_BURST_VALS = (DRAM_BURST_BITS + BIT_WIDTH - 1) / BIT_WIDTH;
  // constexpr uint NUM_BURST_VALS = 2;
  // using t_burst_count = ac_int<BitsForMaxValue<kNumBurstValues>(), false>;

  return q.single_task<StreamingStoreKernel<id>>([=]() KERNEL_PRAGMAS {
    // bool Ack[NUM_BURST_VALS];
    // #pragma unroll
    // for (int i = 0; i < NUM_BURST_VALS; ++i) Ack[i] = false;

    int numAck = 0;
    while (true) {
      auto AddrVal = AddrValPipe::read();
      if (AddrVal.addr == INVALID_ADDR)
        break;

      auto StorePtr = ext::intel::device_ptr<T>(data + AddrVal.addr);
      BurstCoalescedLSU::store(StorePtr, AddrVal.val);
      PRINTF("Stored addr = %d\n", AddrVal.addr);

      // if (Ack[0]) {
        AckPipes::write(true);
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

    PRINTF("sst %d done\n", id);
  });
}
