#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/sycl.hpp>
#include <type_traits>

#include "pipe_utils.hpp"
#include "tuple.hpp"
#include "unrolled_loop.hpp"
#include "device_print.hpp"

using namespace sycl;
using namespace fpga_tools;

using BurstCoalescedLSU = ext::intel::lsu<ext::intel::burst_coalesce<true>,
                                          ext::intel::prefetch<false>>;
// ext::intel::statically_coalesce<false>>;

/// A gated StreamingLoad receives {addr, value, schedule} data on its gate,
/// and stores {addr, value} in a shift-register FIFO for reuse.
constexpr int kStoreFifoSize = 64;
constexpr int kStoreFifoSizeSST = 64;

struct addr_t {
  int addr;
};

struct addr_tag_t {
  int addr;
  int tag;
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

constexpr int INVALID_ADDR = -1;
constexpr int MAX_INT = 0x7FFFFFFF;

/// Unique kernel name generators.
template <int id> class StreamingLoadKernel;
template <int id> class StreamingStoreKernel;
template <int id> class MuxStreamingLoadKernel;
template <int id> class MuxStreamingStoreKernel;
template <int id> class MuxInLoadOrder;
template <int id> class MuxInStoreOrder;

class NoPipe;

/// Given a {LoadAddrPipe} sequence, return loads from data[addr] on the
/// {LoadValPipe}. Stop when INVALID_ADDR is received on {LoadAddrPipe}.
///
/// Signaling:
///   For each load, send a signal on the SignalPipe with the loaded address,
///   and (optional) schedule. On termination, send one final {MAX_ADDR} signal.
///
/// Gated:
///   A load is only performed when the maximum address supplied by
///   {SignalAddrPipe} *is not lower* than the load address.
///   Note that {SendSignalPipe} can be a PipeArray.
///
/// UseSchedule:
///   This load (or memory operations from SendSignalPipe/GateSignalPipe) are
///   executed in a loop, so we need the *schedule* of the loop (cuurent iter)
///   to determine safety during gating.
template <int id, typename AddrPipe, typename ValPipe,
          typename InOrders = NoPipe, int NumInOrder = 0,
          typename OutOrder = NoPipe, int NumOutOrder = 0,
          typename T>
event StreamingLoad(queue &q, T *data, int diff=0) {
  using InOrder = pipe<MuxInLoadOrder<id>, addr_tag_val_t<float>, 32>;
  if constexpr (NumInOrder >= 2) {
    q.single_task<MuxStreamingLoadKernel<id>>([=]() [[intel::kernel_args_restrict]] {
      // The expected next tag, always increasing by one.
      uint next_tag = 1;

      addr_tag_val_t<T> in[NumInOrder];
      bool succ[NumInOrder];
      UnrolledLoop<NumInOrder>([&](auto k) {
        succ[k] = false;
        in[k] = {-1, 0};
      });

      [[intel::initiation_interval(1)]]
      [[intel::speculated_iterations(0)]]
      while (1) {
        bool all_done = true;
        UnrolledLoop<NumInOrder>([&](auto k) {
          all_done &= (in[k].addr == MAX_INT);
        });
        if (all_done)
          break;

        // Choose one, based on tag.
        bool valid = false;
        addr_tag_val_t<T> winner;
        UnrolledLoop<NumInOrder>([&](auto k) {
          if (succ[k]) {
            if (!valid && in[k].tag == next_tag) {
              valid = true;
              succ[k] = false;
              winner = in[k];
            }
          } else {
            bool tmpSucc = false;
            auto tmp = InOrders:: template PipeAt<k>::read(tmpSucc);
            if (tmpSucc) {
              in[k] = tmp;
              succ[k] = true;
            }
          }
        });

        if (valid) {
          InOrder::write(winner);
          next_tag++;
        }
      }

      InOrder::write({MAX_INT, MAX_INT});
    });
  }

  return q.single_task<StreamingLoadKernel<id>>([=]() [[intel::kernel_args_restrict]] {
    addr_tag_val_t<T> storeFifo[kStoreFifoSize];
    bool storeFifoValid[kStoreFifoSize];
    #pragma unroll
    for (int i = 0; i < kStoreFifoSize; ++i) {
      storeFifoValid[i] = false;
    }

    int InOrderAddr = -1;
    int InOrderTag = 0;
    T InOrderVal = {};

    int CurrAddr = 0;
    int CurrTag = 0;
    bool LoadAddrDone = true;

    while (true) {
      // Only read new load address and value if the previous load is done.
      if (LoadAddrDone) {
        auto AddrTag = AddrPipe::read();
        CurrAddr = AddrTag.addr;
        CurrTag = AddrTag.tag;
        if (CurrAddr == INVALID_ADDR) break;
      }

      // If gated, try reading latest gate. If read successfully, shift in at
      // the end of store reuse FIFOs.
      bool IsSafe = true;
      bool reuse = false;
      T reuseVal {};
      if constexpr (NumInOrder > 0) {
        LoadAddrDone = false;

        bool NewInOrderValid = false;
        addr_tag_val_t<T> NewInOrder;
        if constexpr (NumInOrder == 1) {
          NewInOrder = InOrders:: template PipeAt<0>::read(NewInOrderValid);
        } else {
          NewInOrder = InOrder::read(NewInOrderValid);
        }

        if (storeFifoValid[0]) {
          InOrderAddr = storeFifo[0].addr;
          InOrderTag = storeFifo[0].tag;
          InOrderVal = storeFifo[0].val;
        }

        #pragma unroll
        for (int i = 0; i < kStoreFifoSize - 1; ++i) {
          storeFifo[i] = storeFifo[i + 1];
          storeFifoValid[i] = storeFifoValid[i + 1];
        }
        storeFifo[kStoreFifoSize - 1] = NewInOrder;
        storeFifoValid[kStoreFifoSize - 1] = NewInOrderValid;

        // TODO: Maybe move the below before the pipe read.
        IsSafe = (CurrTag - diff <= InOrderTag && CurrAddr <= InOrderAddr);

        reuse = CurrAddr == InOrderAddr;
        reuseVal = InOrderVal;
      }

      // Do the load and return value, if safe.
      if (IsSafe) {
        LoadAddrDone = true;

        auto LoadPtr = ext::intel::device_ptr<T>(data + CurrAddr);
        auto LoadVal = reuse ? reuseVal : BurstCoalescedLSU::load(LoadPtr);
        ValPipe::write(LoadVal);

        // Use LoadVal here to ensure load has returned a value.
        if constexpr (NumOutOrder > 0)
          OutOrder::write({CurrAddr, CurrTag, LoadVal});
      }
    }

    // If signaling, write out MAXI_INT for schedule/address.
    if constexpr (NumOutOrder > 0)
      OutOrder::write({MAX_INT, MAX_INT});

    if constexpr (NumInOrder > 0) {
      while (InOrderAddr != MAX_INT) {
        addr_tag_val_t<T> in;
        bool succ = false;
        if constexpr (NumInOrder == 1) {
          in = InOrders::template PipeAt<0>::read(succ);
        } else {
          in = InOrder::read(succ);
        }
        InOrderAddr = in.addr;
      }
    }

    PRINTF("Done sld %d\n", id);

  });
}

/// Given a {StoreAddrPipe} and {StoreValPipe} sequence, store to data.
/// Stop when INVALID_ADDR is received on {LoadAddrPipe}.
///
/// Signaling:
///   For each store, send a signal on the SignalPipe with the stored address,
///   value, and (optional) schedule. On termination, send one final {MAX_ADDR}
///   signal
///
/// Gated:
///   A store is only performed when the maximum address supplied by
///   {SignalAddrPipe} *is not lower* than the store address.
///
/// UseSchedule:
///   This store (or memory operations from SendSignalPipe/GateSignalPipe) are
///   executed in a loop, so we need the *schedule* of the loop (cuurent iter)
///   to determine safety during gating.
template <int id, typename AddrPipe, typename ValPipe,
          typename InOrders=NoPipe, int NumInOrder=0,
          typename OutOrder=NoPipe, int NumOutOrder=0,
          typename T>
event StreamingStore(queue &q, T *data, int diff=0) {
  using InOrder = pipe<MuxInStoreOrder<id>, addr_tag_val_t<float>, 32>;
  if constexpr (NumInOrder >= 2) {
    q.single_task<MuxStreamingStoreKernel<id>>([=]() [[intel::kernel_args_restrict]] {
      // The expected next tag, always increasing by one.
      uint curr_tag = 0;

      addr_tag_val_t<T> in[NumInOrder];
      bool succ[NumInOrder];
      UnrolledLoop<NumInOrder>([&](auto k) {
        succ[k] = false;
        in[k] = {-1, 0};
      });

      [[intel::initiation_interval(1)]]
      [[intel::speculated_iterations(0)]]
      while (1) {
        bool all_done = true;
        UnrolledLoop<NumInOrder>([&](auto k) {
          all_done &= (in[k].addr == MAX_INT);
        });
        if (all_done)
          break;

        // Choose one, based on tag.
        bool valid = false;
        addr_tag_val_t<T> winner;
        UnrolledLoop<NumInOrder>([&](auto k) {
          if (succ[k]) {
            if (!valid && in[k].tag > curr_tag) {
              valid = true;
              succ[k] = false;
              winner = in[k];
            } else if (in.tag <= curr_tag) {
              succ[k] = false; // Lower tag gets ignored
            }
          } else {
            bool tmpSucc = false;
            auto tmp = InOrders:: template PipeAt<k>::read(tmpSucc);
            if (tmpSucc) {
              in[k] = tmp;
              succ[k] = true;
            }
          }
        });

        if (valid) {
          InOrder::write(winner);
          curr_tag = winner.tag;
        }
      }

      InOrder::write({MAX_INT, MAX_INT});
    });
  }

  return q.single_task<StreamingStoreKernel<id>>([=]() [[intel::kernel_args_restrict]] {
    int InOrderAddr = -1;
    int InOrderTag = 0;

    addr_tag_val_t<T> InOrderBuffer[kStoreFifoSizeSST];
    bool InOrderBufferValid[kStoreFifoSizeSST];
    #pragma unroll
    for (int i = 0; i < kStoreFifoSizeSST; ++i) 
      InOrderBufferValid[i] = false;

    int CurrAddr = 0;
    int CurrTag = 0;
    T StoreVal;
    bool StoreValValid = false;
    bool StoreAddrValid = false;

    while (true) {
      if (!StoreAddrValid) {
        auto AddrTag = AddrPipe::read(StoreAddrValid);
        if (StoreAddrValid) {
          CurrAddr = AddrTag.addr;
          CurrTag = AddrTag.tag;
        }
      }

      if (CurrAddr == INVALID_ADDR) break;

      if (!StoreValValid) {
        auto tmpVal = ValPipe::read(StoreValValid);
        if (StoreValValid)
          StoreVal = tmpVal;
      }

      // If the store is gated, then we need to check for safety.
      bool IsSafe = StoreAddrValid & StoreValValid;
      if constexpr (NumInOrder > 0) {
        bool NewInOrderValid = false;
        addr_tag_val_t<T> NewInOrder;
        if constexpr (NumInOrder == 1) {
          NewInOrder = InOrders:: template PipeAt<0>::read(NewInOrderValid);
        } else {
          NewInOrder = InOrder::read(NewInOrderValid);
        }
        
        // Shift on every clock.
        if (InOrderBufferValid[0]) {
          InOrderTag = InOrderBuffer[0].tag;
          InOrderAddr = InOrderBuffer[0].addr;
        }
        #pragma unroll
        for (int i = 0; i < kStoreFifoSizeSST - 1; ++i) {
          InOrderBufferValid[i] = InOrderBufferValid[i + 1];
          InOrderBuffer[i] = InOrderBuffer[i + 1];
        }
        InOrderBuffer[kStoreFifoSizeSST - 1] = NewInOrder;
        InOrderBufferValid[kStoreFifoSizeSST - 1] = NewInOrderValid;

        IsSafe &= (CurrTag - diff <= InOrderTag && CurrAddr <= InOrderAddr);
      }

      // Execute store, if safe.
      if (IsSafe) {
        auto StorePtr = ext::intel::device_ptr<T>(data + CurrAddr);
        BurstCoalescedLSU::store(StorePtr, StoreVal);
        StoreAddrValid = false;
        StoreValValid = false;

        if constexpr (NumOutOrder > 0)
          OutOrder::write({CurrAddr, CurrTag, StoreVal});
      }
    }

    // If signaling, write out MAXI_INT for schedule/address.
    if constexpr (NumOutOrder > 0) {
      OutOrder::write({MAX_INT, MAX_INT});
    }

    if constexpr (NumInOrder > 0) {
      while (InOrderAddr != MAX_INT) {
        addr_tag_val_t<T> in;
        bool succ = false;
        if constexpr (NumInOrder == 1) {
          in = InOrders::template PipeAt<0>::read(succ);
        } else {
          in = InOrder::read(succ);
        }
        InOrderAddr = in.addr;
      }
    }

    PRINTF("Done sst %d\n", id);

  });
}
