#include <sycl/sycl.hpp>
#include <sycl/ext/intel/fpga_extensions.hpp>

using namespace sycl;


using BurstCoalescedLSU =
    ext::intel::lsu<ext::intel::burst_coalesce<true>,
                    ext::intel::prefetch<false>>;
                    // ext::intel::statically_coalesce<false>>;

constexpr int kStoreFifoSize = 8;

template<typename T>
struct addr_val_t { int addr; T val; };

constexpr int INVALID_ADDR = -1;
constexpr int MAX_ADDR = 0x7FFFFFFF;


/////////////////////////////////////////////////////////////////////
// Loads
/////////////////////////////////////////////////////////////////////

/// Given a {LoadAddrPipe} sequence, return loads from data[addr] on the
/// {LoadValPipe}. Stop when INVALID_ADDR is received on {LoadAddrPipe}.
template <typename LoadAddrPipe, typename LoadValPipe, typename T>
event StreamingLoad(queue &q, T *data) {
  return q.single_task<LoadAddrPipe>([=]() [[intel::kernel_args_restrict]] {
    while (true) {
      auto LoadAddr = LoadAddrPipe::read();
      if (LoadAddr == INVALID_ADDR)
        break;

      auto Ptr = ext::intel::device_ptr<T>(data + LoadAddr);
      auto LoadVal = BurstCoalescedLSU::load(Ptr);
      LoadValPipe::write(LoadVal);
    }
  });
}

/// Given a {LoadAddrPipe} sequence, return loads from data[addr] on the
/// {LoadValPipe}. Stop when INVALID_ADDR is received on {LoadAddrPipe}.
/// Signaling:
///   For each load, send a signal on the SignalPipe with the loaded address.
///   On termination, send one final {MAX_ADDR} signal
template <typename LoadAddrPipe, typename LoadValPipe, typename SignalPipe,
          typename T>
event SignalingStreamingLoad(queue &q, T *data) {
  return q.single_task<LoadAddrPipe>([=]() [[intel::kernel_args_restrict]] {
    while (true) {
      auto LoadAddr = LoadAddrPipe::read();
      if (LoadAddr == INVALID_ADDR)
        break;

      auto Ptr = ext::intel::device_ptr<T>(data + LoadAddr);
      auto LoadVal = BurstCoalescedLSU::load(Ptr);
      LoadValPipe::write(LoadVal);

      // Sending Load Value ensures that the signal happens *after* the load.
      SignalPipe::write({LoadAddr, LoadVal});
    }

    SignalPipe::write({MAX_ADDR});
  });
}

/// Given a {LoadAddrPipe} sequence, return loads from data[addr] on the
/// {LoadValPipe}. Stop when INVALID_ADDR is received on {LoadAddrPipe}.
/// Gated:
///   A load is only performed when the maximum address supplied by
///   {SignalAddrPipe} *is not lower* than the load address.
template <typename LoadAddrPipe, typename LoadValPipe, typename SignalAddrPipe,
          typename T>
event GatedStreamingLoad(queue &q, T *data) {
  return q.single_task<LoadAddrPipe>([=]() [[intel::kernel_args_restrict]] {
    int MaxSignalAddr = -1;

    addr_val_t<T> storeFifo[kStoreFifoSize];
    #pragma unroll
    for (int i = 0; i < kStoreFifoSize; ++i) {
      storeFifo[i] = {-1, 0};
    }

    bool LoadAddrDone = true;
    int LoadAddr;
    while (true) {
      if (LoadAddrDone) {
        LoadAddr = LoadAddrPipe::read();
        if (LoadAddr == INVALID_ADDR) break;
      }
      LoadAddrDone = false;

      bool succ = false;
      addr_val_t signal = SignalAddrPipe::read(succ);
      if (succ) {
        MaxSignalAddr = (signal.addr > MaxSignalAddr) ? signal.addr : MaxSignalAddr;

        // Shift
        #pragma unroll
        for (int i = 0; i < kStoreFifoSize - 1; ++i) 
          storeFifo[i] = storeFifo[i + 1];
        storeFifo[kStoreFifoSize - 1] = signal;
      }
     
      if (LoadAddr <= MaxSignalAddr) {
        LoadAddrDone = true;
        // CAM
        T val;
        bool reuse = false;
        #pragma unroll
        for (int i = 0; i < kStoreFifoSize; ++i) {
          if (LoadAddr == storeFifo[i].addr) {
            reuse = true;
            val = storeFifo[i].val;
          }
        }

        auto LoadPtr = ext::intel::device_ptr<T>(data + LoadAddr);
        auto ld = reuse ? val : BurstCoalescedLSU::load(LoadPtr);
        LoadValPipe::write(ld);
      }
    }
  });
}

/////////////////////////////////////////////////////////////////////
// Stores
/////////////////////////////////////////////////////////////////////

template <typename StoreAddrPipe, typename StoreValPipe, typename T>
event StreamingStore(queue &q, T *data) {
  return q.single_task<StoreAddrPipe>([=]() [[intel::kernel_args_restrict]] {
    while (true) {
      auto StoreAddr = StoreAddrPipe::read();
      if (StoreAddr == INVALID_ADDR)
        break;
      auto StoreVal = StoreValPipe::read();

      auto Ptr = ext::intel::device_ptr<T>(data + StoreAddr);
      BurstCoalescedLSU::store(Ptr, StoreVal);
    }
  });
}

template <typename StoreAddrPipe, typename StoreValPipe, typename SignalPipe,
          typename T>
event SignalingStreamingStore(queue &q, T *data) {
  return q.single_task<StoreAddrPipe>([=]() [[intel::kernel_args_restrict]] {
    while (true) {
      auto StoreAddr = StoreAddrPipe::read();
      if (StoreAddr == INVALID_ADDR)
        break;
      auto StoreVal = StoreValPipe::read();

      auto Ptr = ext::intel::device_ptr<T>(data + StoreAddr);
      BurstCoalescedLSU::store(Ptr, StoreVal);

      SignalPipe::write({StoreAddr, StoreVal});
    }

    SignalPipe::write({MAX_ADDR});
  });
}

template <typename StoreAddrPipe, typename StoreValPipe, typename SignalAddrPipe,
          typename T>
event GatedStreamingStore(queue &q, T *data) {
  return q.single_task<StoreAddrPipe>([=]() [[intel::kernel_args_restrict]] {
    int MaxSignalAddr = -1;

    bool StoreAddrDone = true;
    int StoreAddr;
    T StoreVal;
    while (true) {
      if (StoreAddrDone) {
        StoreAddr = StoreAddrPipe::read();
        if (StoreAddr == INVALID_ADDR) break;
        StoreVal = StoreValPipe::read();
      }
      StoreAddrDone = false;

      bool succ = false;
      addr_val_t signal = SignalAddrPipe::read(succ);
      if (succ) {
        MaxSignalAddr = (signal.addr > MaxSignalAddr) ? signal.addr : MaxSignalAddr;
      }
     
      if (StoreAddr < MaxSignalAddr) {
        auto Ptr = ext::intel::device_ptr<T>(data + StoreAddr);
        BurstCoalescedLSU::store(Ptr, StoreVal);
        StoreAddrDone = true;
        // NOTE: Send {store_addr, store_val} here to any dependent mem ops.  
      }
    }
  });
}

