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
using PipelinedLSU = sycl::ext::intel::lsu<>;

/// A gated StreamingLoad receives {addr, value, schedule} data on its gate,
/// and stores {addr, value} in a shift-register FIFO for reuse.
constexpr int kStoreFifoSize = 64;

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
constexpr int MAX_INT = (1<<31);

/// Unique kernel name generators.
template <int id> class StreamingLoadKernel;
template <int id> class StreamingStoreKernel;
template <int id> class MuxStreamingLoadKernel;
template <int id> class MuxStreamingStoreKernel;
template <int id> class MuxInLoadOrder;
template <int id> class MuxInStoreOrder;

class NoPipe;

template <int id, typename AddrPipe, typename ValPipe,
          typename InOrder = NoPipe, int NumInOrder = 0,
          typename OutOrder = NoPipe, int NumOutOrder = 0,
          typename T>
event StreamingLoad(queue &q, T *data, int diff=0) {
  using InOrderMuxed = pipe<MuxInLoadOrder<id>, addr_tag_val_t<T>, 32>;
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
            auto tmp = InOrder:: template PipeAt<k>::read(tmpSucc);
            if (tmpSucc) {
              in[k] = tmp;
              succ[k] = true;
            }
          }
        });

        if (valid) {
          InOrderMuxed::write(winner);
          next_tag++;
        }
      }

      InOrderMuxed::write({MAX_INT, MAX_INT});
    });
  }

  return q.single_task<StreamingLoadKernel<id>>([=]() [[intel::kernel_args_restrict]] {
    T storeFifoVal[kStoreFifoSize];
    int storeFifoAddr[kStoreFifoSize];
    #pragma unroll
    for (int i = 0; i < kStoreFifoSize; ++i) storeFifoAddr[i] = INVALID_ADDR;

    int InOrderAddr = -1;
    int InOrderTag = 0;
    T InOrderVal = {};

    int CurrAddr = 0;
    int CurrTag = 0;
    bool LoadAddrDone = true;

    int NumTotal = 0;
    int NumReused = 0;

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
          NewInOrder = InOrder:: template PipeAt<0>::read(NewInOrderValid);
        } else {
          NewInOrder = InOrderMuxed::read(NewInOrderValid);
        }

        if (NewInOrderValid) {
          InOrderAddr = NewInOrder.addr;
          InOrderTag = NewInOrder.tag;
          InOrderVal = NewInOrder.val;

          #pragma unroll
          for (int i = 0; i < kStoreFifoSize - 1; ++i) {
            storeFifoAddr[i] = storeFifoAddr[i + 1];
            storeFifoVal[i] = storeFifoVal[i + 1];
          }
          storeFifoAddr[kStoreFifoSize - 1] = InOrderAddr;
          storeFifoVal[kStoreFifoSize - 1] = InOrderVal;
        }

        // TODO: Maybe move the below before the pipe read.
        IsSafe = (CurrTag - diff <= 0) || 
                 (CurrTag - diff == InOrderTag) ||
                 (CurrTag - diff < InOrderTag && CurrAddr <= InOrderAddr);

        // #pragma unroll
        // for (int i = 0; i < kStoreFifoSize; ++i) {
        UnrolledLoop<kStoreFifoSize>([&](auto i) {
          // TODO: No need for tag comparison.
          if (storeFifoAddr[i] == CurrAddr) {
            reuse = true;
            reuseVal = storeFifoVal[i];
          }
        }
        );

      }

      // Do the load and return value, if safe.
      if (IsSafe) {
        LoadAddrDone = true;

        NumTotal++;
        if (reuse)
          NumReused++;

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
        if constexpr (NumInOrder == 1) {
          in = InOrder::template PipeAt<0>::read();
        } else {
          in = InOrderMuxed::read();
        }
        InOrderAddr = in.addr;
      }
    }

    PRINTF("Done sld %d\tReused %d/%d\n", id, NumReused, NumTotal);

  });
}

template <int id, typename AddrPipe, typename ValPipe,
          typename InOrder=NoPipe, int NumInOrder=0,
          typename OutOrder=NoPipe, int NumOutOrder=0,
          typename T>
event StreamingStore(queue &q, T *data, int diff=0) {
  using InOrderMuxed = pipe<MuxInStoreOrder<id>, addr_tag_val_t<T>, 32>;
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
            auto tmp = InOrder:: template PipeAt<k>::read(tmpSucc);
            if (tmpSucc) {
              in[k] = tmp;
              succ[k] = true;
            }
          }
        });

        if (valid) {
          InOrderMuxed::write(winner);
          curr_tag = winner.tag;
        }
      }

      InOrderMuxed::write({MAX_INT, MAX_INT});
    });
  }

  return q.single_task<StreamingStoreKernel<id>>([=]() [[intel::kernel_args_restrict]] {
    int InOrderAddr = -1;
    int InOrderTag = 0;

    // TODO: Remove InOrder buffer?
    addr_tag_val_t<T> InOrderBuffer[kStoreFifoSize];
    bool InOrderBufferValid[kStoreFifoSize];
    #pragma unroll
    for (int i = 0; i < kStoreFifoSize; ++i) 
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
          NewInOrder = InOrder:: template PipeAt<0>::read(NewInOrderValid);
        } else {
          NewInOrder = InOrderMuxed::read(NewInOrderValid);
        }
        
        // Shift on every clock.
        if (InOrderBufferValid[0]) {
          InOrderTag = InOrderBuffer[0].tag;
          InOrderAddr = InOrderBuffer[0].addr;
        }
        #pragma unroll
        for (int i = 0; i < kStoreFifoSize - 1; ++i) {
          InOrderBufferValid[i] = InOrderBufferValid[i + 1];
          InOrderBuffer[i] = InOrderBuffer[i + 1];
        }
        InOrderBuffer[kStoreFifoSize - 1] = NewInOrder;
        InOrderBufferValid[kStoreFifoSize - 1] = NewInOrderValid;

        // IsSafe &= (CurrTag - diff <= InOrderTag && CurrAddr <= InOrderAddr);
        IsSafe = (CurrTag - diff <= 0) || 
                 (CurrTag - diff == InOrderTag) ||
                 (CurrTag - diff < InOrderTag && CurrAddr <= InOrderAddr);
      }

      // Execute store, if safe.
      if (IsSafe) {
        auto StorePtr = ext::intel::device_ptr<T>(data + CurrAddr);
        BurstCoalescedLSU::store(StorePtr, StoreVal);
        // PipelinedLSU::store(StorePtr, StoreVal);

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

    // PRINTF("InOrderAddr=%d in sst %d\n", InOrderAddr, id);
    for (int i = 0; i < kStoreFifoSize; ++i) {
      if (InOrderBufferValid[i]) {
        InOrderAddr = InOrderBuffer[i].addr;
      }
    }

    if constexpr (NumInOrder > 0) {
      while (InOrderAddr != MAX_INT) {
        addr_tag_val_t<T> in;
        if constexpr (NumInOrder == 1) {
          in = InOrder::template PipeAt<0>::read();
        } else {
          in = InOrderMuxed::read();
        }
        InOrderAddr = in.addr;
      }
    }

    PRINTF("Done sst %d\n", id);

  });
}
