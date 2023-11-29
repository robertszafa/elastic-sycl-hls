#include <sycl/sycl.hpp>
#include <sycl/ext/intel/fpga_extensions.hpp>

using namespace sycl;


using BurstCoalescedLSU =
    ext::intel::lsu<ext::intel::burst_coalesce<true>,
                    ext::intel::prefetch<false>>;
                    // ext::intel::statically_coalesce<false>>;

constexpr int kStoreFifoSize = 8;

struct addr_sched_t { int addr; int sched; };

template<typename T>
struct addr_val_t { int addr; int sched; T val; };


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

      // data[LoadAddr] = LoadVal;
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
      auto LoadAddrSched = LoadAddrPipe::read();
      if (LoadAddrSched.addr == INVALID_ADDR)
        break;

      auto Ptr = ext::intel::device_ptr<T>(data + LoadAddrSched.addr);
      auto LoadVal = BurstCoalescedLSU::load(Ptr);
      LoadValPipe::write(LoadVal);

      // Sending Load Value ensures that the signal happens *after* the load.
      SignalPipe::write({LoadAddrSched.addr, LoadAddrSched.sched, LoadVal});
    }

    SignalPipe::write({-1, MAX_ADDR});
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
    // int MaxSignalAddr = INVALID_ADDR;

    int storeFifoAddr[kStoreFifoSize];
    T storeFifoVal[kStoreFifoSize];
    #pragma unroll
    for (int i = 0; i < kStoreFifoSize-1; ++i) {
      storeFifoAddr[i] = INVALID_ADDR;
      storeFifoVal[i] = 0;
    }

    bool LoadAddrDone = true;
    int LoadAddr = 0, LoadSched = 0;
    addr_val_t<T> signal = SignalAddrPipe::read();
    storeFifoVal[kStoreFifoSize - 1] = signal.addr;
    storeFifoVal[kStoreFifoSize - 1] = signal.val;
    while (true) {
      if (LoadAddrDone) {
        auto LoadAddrSched = LoadAddrPipe::read();
        if (LoadAddrSched.addr == INVALID_ADDR) break;
        LoadAddr = LoadAddrSched.addr;
        LoadSched = LoadAddrSched.sched;
      }
      LoadAddrDone = false;

      bool succ = false;
      addr_val_t signalTmp = SignalAddrPipe::read(succ);
      if (succ) {
        signal = signalTmp;

        // Shift
        #pragma unroll
        for (int i = 0; i < kStoreFifoSize - 1; ++i) {
          storeFifoAddr[i] = storeFifoAddr[i + 1];
          storeFifoVal[i] = storeFifoVal[i + 1];
        }
        storeFifoAddr[kStoreFifoSize - 1] = signal.addr;
        storeFifoVal[kStoreFifoSize - 1] = signal.val;
      }
     
      if (LoadSched < signal.sched || (LoadSched == signal.sched && LoadAddr <= signal.addr)) {
        LoadAddrDone = true;
        // CAM
        T val;
        bool reuse = false;
        #pragma unroll
        for (int i = 0; i < kStoreFifoSize; ++i) {
          if (LoadAddr == storeFifoAddr[i]) {
            reuse = true;
            val = storeFifoVal[i];
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
      // data[StoreAddr] = StoreVal;
    }
  });
}

template <typename StoreAddrPipe, typename StoreValPipe, typename SignalPipe,
          typename T>
event SignalingStreamingStore(queue &q, T *data) {
  return q.single_task<StoreAddrPipe>([=]() [[intel::kernel_args_restrict]] {
    while (true) {
      auto StoreAddrSched = StoreAddrPipe::read();
      const auto StoreAddr = StoreAddrSched.addr;
      const auto StoreSched = StoreAddrSched.sched;

      if (StoreAddr == INVALID_ADDR)
        break;

      auto StoreVal = StoreValPipe::read();
      auto Ptr = ext::intel::device_ptr<T>(data + StoreAddr);
      BurstCoalescedLSU::store(Ptr, StoreVal);

      SignalPipe::write({StoreAddr, StoreSched, StoreVal});
    }

    SignalPipe::write({-1, MAX_ADDR});
  });
}

template <typename StoreAddrPipe, typename StoreValPipe, typename SignalAddrPipe,
          typename T>
event GatedStreamingStore(queue &q, T *data) {
  return q.single_task<StoreAddrPipe>([=]() [[intel::kernel_args_restrict]] {
    // int MaxSignalAddr = INVALID_ADDR;

    bool StoreAddrDone = true;
    int StoreAddr = 0, StoreSched = 0;
    addr_val_t<T> signal = SignalAddrPipe::read();
    T StoreVal;
    while (true) {
      if (StoreAddrDone) {
        auto StoreAddrSched = StoreAddrPipe::read();
        if (StoreAddrSched.addr == INVALID_ADDR) break;
        StoreAddr = StoreAddrSched.addr;
        StoreSched = StoreAddrSched.sched;
        StoreVal = StoreValPipe::read();
      }
      StoreAddrDone = false;

      bool succ = false;
      auto signalTmp = SignalAddrPipe::read(succ);
      if (succ) {
        signal = signalTmp;
      }
     
      if (StoreSched < signal.sched || (StoreSched == signal.sched && StoreAddr < signal.addr)) {
        auto Ptr = ext::intel::device_ptr<T>(data + StoreAddr);
        BurstCoalescedLSU::store(Ptr, StoreVal);
        StoreAddrDone = true;
        // NOTE: Send {store_addr, store_val} here to any dependent mem ops.  
      }
    }
  });
}

