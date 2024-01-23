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
constexpr int kStoreFifoSize = 16;

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


template <int id, typename AddrPipe, typename ValPipe,
          typename InOrders = NoPipe, int NumInOrder = 0,
          typename OutOrder = NoPipe, int NumOutOrder = 0,
          typename T>
event StreamingLoad(queue &q, T *data, int diff=0) {
  using InOrder = pipe<MuxInLoadOrder<id>, addr_tag_val_t<T>, 4*NumInOrder>;
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
          // PRINTF("Winner of MUX with tag %d, addr %d\n", winner.tag, winner.addr);
          next_tag++;
        }
      }

      InOrder::write({MAX_INT, MAX_INT});
      PRINTF("Done MUX\n");
    });
  }

  return q.single_task<StreamingLoadKernel<id>>([=]() [[intel::kernel_args_restrict]] {
    int storeFifoAddr[kStoreFifoSize];
    T storeFifoVal[kStoreFifoSize];
    #pragma unroll
    for (int i = 0; i < kStoreFifoSize; ++i) storeFifoAddr[i] = INVALID_ADDR;

    int InOrderAddr = -1;
    int InOrderTag = 0;

    int CurrAddr = 0;
    int CurrTag = 0;
    bool CurrAddrValid = false;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]] 
    // [[intel::speculated_iterations(0)]]
    while (true) {
      /** Read Addr */
      if (!CurrAddrValid) {
        auto AddrTag = AddrPipe::read(CurrAddrValid);
        if (CurrAddrValid) {
          CurrAddr = AddrTag.addr;
          CurrTag = AddrTag.tag;
          if (CurrAddr == INVALID_ADDR) break;
        }
      }  
      /** End Read Addr */

      /** Read InOrder */
      if constexpr (NumInOrder > 0) {
        bool succ = false;
        addr_tag_val_t<T> in;
        if constexpr (NumInOrder == 1) {
          in = InOrders:: template PipeAt<0>::read(succ);
        } else {
          in = InOrder::read(succ);
        }
        if (succ) {
          #pragma unroll
          for (int i = 0; i < kStoreFifoSize - 1; ++i) {
            storeFifoAddr[i] = storeFifoAddr[i + 1];
            storeFifoVal[i] = storeFifoVal[i + 1];
          }
          storeFifoAddr[kStoreFifoSize - 1] = in.addr;
          storeFifoVal[kStoreFifoSize - 1] = in.val;
          InOrderTag = in.tag;
          InOrderAddr = in.addr;
          // PRINTF("In sld%d got (tag=%d, addr=%d)\nCurrently loading (tag=%d, addr=%d)\n\n", 
          //         id, in.tag, in.addr, CurrTag, CurrAddr);
        }
      }
      /** End Read InOrder */

      /** Check Safety */
      bool IsSafe = true;
      if constexpr (NumInOrder > 0) {
        IsSafe = (CurrTag == InOrderTag) || (CurrTag - diff <= InOrderTag && CurrAddr <= InOrderAddr);
      }
      /** End Check Safety */

      /** Get reuse value */
      bool reuse = false;
      T reuseVal;
      #pragma unroll
      for (int i = 0; i < kStoreFifoSize; ++i) {
        if (CurrAddr == storeFifoAddr[i]) {
          reuse = true;
          reuseVal = storeFifoVal[i];
        }
      }
      /** End Get reuse value */

      /** Execute load */
      if (IsSafe && CurrAddrValid) {
        CurrAddrValid = false;
        auto LoadPtr = ext::intel::device_ptr<T>(data + CurrAddr);
        auto LoadVal = reuse ? reuseVal : BurstCoalescedLSU::load(LoadPtr);
        ValPipe::write(LoadVal);
        if (id == 1) {
          // PRINTF("Loaded and returned (tag=%d, addr=%d)\n\n", 
          //         CurrTag, CurrAddr);
        }

        // Use LoadVal here to ensure load has returned a value.
        if constexpr (NumOutOrder > 0)
          OutOrder::write({CurrAddr, CurrTag, LoadVal});
      }
      /** End Execute load */

    } // End While Loop

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


template <int id, typename AddrPipe, typename ValPipe,
          typename InOrders=NoPipe, int NumInOrder=0,
          typename OutOrder=NoPipe, int NumOutOrder=0,
          typename T>
event StreamingStore(queue &q, T *data, int diff=0) {
  using InOrder = pipe<MuxInStoreOrder<id>, addr_tag_val_t<T>, 4*NumInOrder>;
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
      PRINTF("Done MUX\n");
    });
  }

  return q.single_task<StreamingStoreKernel<id>>([=]() [[intel::kernel_args_restrict]] {
    int InOrderAddr = -1;
    int InOrderTag = 0;

    int CurrAddr = 0;
    int CurrTag = 0;
    bool CurrValid = false;
    T CurrVal;

    [[intel::ivdep]]
    [[intel::initiation_interval(1)]] 
    [[intel::speculated_iterations(0)]]
    while (true) {
      /** Read Addr */
      if (!CurrValid) {
        auto AddrTag = AddrPipe::read(CurrValid);
        if (CurrValid) {
          CurrAddr = AddrTag.addr;
          CurrTag = AddrTag.tag;
          if (CurrAddr == INVALID_ADDR) break;
          CurrVal = ValPipe::read();
        }
      }  
      /** End Read Addr */

      /** Read InOrder */
      if constexpr (NumInOrder > 0) {
        bool succ = false;
        addr_tag_val_t<T> in;
        if constexpr (NumInOrder == 1) {
          in = InOrders:: template PipeAt<0>::read(succ);
        } else {
          in = InOrder::read(succ);
        }
        if (succ) {
          InOrderTag = in.tag;
          InOrderAddr = in.addr;
        }
      }
      /** End Read InOrder */

      /** Check Safety */
      bool IsSafe = true;
      if constexpr (NumInOrder > 0) {
        IsSafe = (CurrTag - diff <= InOrderTag && CurrAddr <= InOrderAddr);
      }
      /** End Check Safety */

      /** Execute store */
      if (IsSafe && CurrValid) {
        CurrValid = false;
        auto StorePtr = ext::intel::device_ptr<T>(data + CurrAddr);
        BurstCoalescedLSU::store(StorePtr, CurrVal);

        if constexpr (NumOutOrder > 0) {
          OutOrder::write({CurrAddr, CurrTag, CurrVal});
        }
      }
      /** End Execute store */

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

    PRINTF("Done sst %d\n", id);

  });
}