#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/sycl.hpp>
#include <type_traits>

#include "pipe_utils.hpp"
#include "tuple.hpp"
#include "unrolled_loop.hpp"
#include "data_bundle.hpp"

using namespace sycl;
using namespace fpga_tools;

using BurstCoalescedLSU = ext::intel::lsu<ext::intel::burst_coalesce<true>,
                                          ext::intel::cache<0>,
                                          ext::intel::statically_coalesce<false>,
                                          ext::intel::prefetch<false>>;

constexpr int kStoreFifoSize = 8;

struct addr_sched_t {
  int addr;
  int sched;
};

template <typename T> struct addr_val_sched_t {
  int addr;
  int sched;
  T val;
};

constexpr int INVALID_ADDR = -1;
constexpr int MAX_INT = 0x7FFFFFFF;

template <typename ID> class StreamingLoadKernel;
template <typename ID> class StreamingStoreKernel;

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
///
/// UseSchedule:
///   This load (or memory operations from SendSignalPipe/GateSignalPipe) are
///   executed in a loop, so we need the *schedule* of the loop (cuurent iter)
///   to determine safety during gating.
template <int NumLoads, typename LoadAddrPipes, typename LoadValPipes, 
          typename SendSignalPipe=NoPipe, typename GateSignalPipe=NoPipe, 
          typename T>
event StreamingLoad(queue &q, T *data) {

  using KernelID = StreamingLoadKernel<LoadAddrPipes>;

  constexpr bool IsGated = !std::is_same<GateSignalPipe, NoPipe>::value;
  constexpr bool IsSignaling = !std::is_same<SendSignalPipe, NoPipe>::value;
  using AddrType = decltype(LoadAddrPipes:: template PipeAt<0>::read());
  constexpr bool UseSchedule = std::is_same<AddrType, addr_sched_t>::value;

  // // LoadAddrPipes
  // constexpr int NumLoads = LoadAddrPipes::template GetDimSize<0>();

  return q.single_task<KernelID>([=]() [[intel::kernel_args_restrict]] {
    int storeFifoAddr[kStoreFifoSize];
    T storeFifoVal[kStoreFifoSize];
    int GateSignalAddr, GateSignalSched;

    if constexpr (IsGated) {
      #pragma unroll
      for (int i = 0; i < kStoreFifoSize-1; ++i) {
        storeFifoAddr[i] = INVALID_ADDR;
        storeFifoVal[i] = 0;
      }

      auto signal = GateSignalPipe::read();
      storeFifoAddr[kStoreFifoSize - 1] = signal.addr;
      storeFifoVal[kStoreFifoSize - 1] = signal.val;
      GateSignalAddr = signal.addr;
      if constexpr (UseSchedule)
        GateSignalSched = signal.sched;
    }

    // int LoadAddr = 0, LoadSched = 0;
    // bool LoadAddrDone = true;
    // DataBundle<int, NumLoads> LoadAddr, LoadSched;
    NTuple<int, NumLoads> LoadAddrTp, LoadSchedTp;
    bool LoadAddrDone = true;

    while (true) {
      if (LoadAddrDone) {
        UnrolledLoop<NumLoads>([&](auto iLd) {
          auto LoadAddrRead = LoadAddrPipes:: template PipeAt<iLd>::read();
          if constexpr (UseSchedule) {
            LoadAddrTp. template get<iLd>() = LoadAddrRead.addr;
            LoadSchedTp. template get<iLd>() = LoadAddrRead.sched;
          } else {
            LoadAddrTp. template get<iLd>() = LoadAddrRead;
          }
        });

        if (LoadAddrTp. template get<0>() == INVALID_ADDR) break;
      }

      if constexpr (IsGated) {
        LoadAddrDone = false;
      
        bool succ = false;
        auto signal = GateSignalPipe::read(succ);
        if (succ) {
          GateSignalAddr = signal.addr;
          if constexpr (UseSchedule) 
            GateSignalSched = signal.sched;

          // Shift
          #pragma unroll
          for (int i = 0; i < kStoreFifoSize - 1; ++i) {
            storeFifoAddr[i] = storeFifoAddr[i + 1];
            storeFifoVal[i] = storeFifoVal[i + 1];
          }
          storeFifoAddr[kStoreFifoSize - 1] = signal.addr;
          storeFifoVal[kStoreFifoSize - 1] = signal.val;
        }
      }

      auto& LoadSched = LoadSchedTp. template get<0>();
      NTuple<bool, NumLoads> IsSafeTp;
      UnrolledLoop<NumLoads>([&](auto iLd) {
        auto& LoadAddr = LoadAddrTp. template get<iLd>();
        auto& IsSafe = IsSafeTp. template get<iLd>();

        IsSafe = true;
        if constexpr (IsGated) {
          if constexpr (UseSchedule) {
            IsSafe = (LoadSched < GateSignalSched) ||
                     (LoadSched == GateSignalSched && LoadAddr <= GateSignalAddr);
          } else {
            IsSafe = LoadAddr <= GateSignalAddr;
          }
        }
      });

      NTuple<bool, NumLoads> ReuseTp;
      NTuple<T, NumLoads> ReuseValTp;
      UnrolledLoop<NumLoads>(
          [&](auto iLd) { ReuseTp.template get<iLd>() = false; });

      if constexpr (IsGated) {
        // CAM reuse FIFO
        #pragma unroll
        for (int i = 0; i < kStoreFifoSize; ++i) {
          UnrolledLoop<NumLoads>([&](auto iLd) {
            if (LoadAddrTp. template get<iLd>() == storeFifoAddr[i]) {
              ReuseTp. template get<iLd>() = true;
              ReuseValTp. template get<iLd>() = storeFifoVal[i];
            }
          });
        }
      }

      bool AllSafe = true;
      UnrolledLoop<NumLoads>(
          [&](auto iLd) { AllSafe &= IsSafeTp.template get<iLd>(); });

      int MinAddr = MAX_INT;
      T LoadValDep; // Signal only after LoadVal received.
      if (AllSafe) {
        if constexpr (IsGated) 
          LoadAddrDone = true;

        UnrolledLoop<NumLoads>([&](auto iLd) {
          auto& LoadAddr = LoadAddrTp. template get<iLd>();
          auto& LoadSched = LoadSchedTp. template get<iLd>();
          auto& Reuse = ReuseTp. template get<iLd>();
          auto& ReuseVal = ReuseValTp. template get<iLd>();

          auto LoadPtr = ext::intel::device_ptr<T>(data + LoadAddr);
          auto LoadVal = Reuse ? ReuseVal : BurstCoalescedLSU::load(LoadPtr);
          LoadValPipes:: template PipeAt<iLd>::write(LoadVal);


          if (LoadAddr < MinAddr) {
            MinAddr = LoadAddr;
            LoadValDep = LoadVal;
          }
        });

        if constexpr (IsSignaling) {
          // All loads have same schedule.
          if constexpr (UseSchedule)
            SendSignalPipe::write({MinAddr, LoadSched, LoadValDep});
          else
            SendSignalPipe::write({MinAddr, LoadValDep});
        }
      }
    }

    if constexpr (IsSignaling) {
      if constexpr (UseSchedule)
        SendSignalPipe::write({-1, MAX_INT});
      else
        SendSignalPipe::write({MAX_INT});
    }
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
template <typename StoreAddrPipe, typename StoreValPipe, 
          typename SendSignalPipe=NoPipe, typename GateSignalPipe=NoPipe, 
          typename T>
event StreamingStore(queue &q, T *data) {

  using KernelID = StreamingStoreKernel<StoreAddrPipe>;

  constexpr bool IsGated = !std::is_same<GateSignalPipe, NoPipe>::value;
  constexpr bool IsSignaling = !std::is_same<SendSignalPipe, NoPipe>::value;
  using AddrType = decltype(StoreAddrPipe::read());
  constexpr bool UseSchedule = std::is_same<AddrType, addr_sched_t>::value;

  return q.single_task<KernelID>([=]() [[intel::kernel_args_restrict]] {
    int GateSignalAddr, GateSignalSched;

    if constexpr (IsGated) {
      auto signal = GateSignalPipe::read();
      GateSignalAddr = signal.addr;
      if constexpr (UseSchedule)
        GateSignalSched = signal.sched;
    }

    int StoreAddr = 0, StoreSched = 0;
    T StoreVal;
    bool StoreAddrDone = true;
    while (true) {
      if (StoreAddrDone) {
        auto LoadAddrRead = StoreAddrPipe::read();
        if constexpr (UseSchedule) {
          StoreAddr = LoadAddrRead.addr;
          StoreSched = LoadAddrRead.sched;
        } else {
          StoreAddr = LoadAddrRead;
        }

        if (StoreAddr == INVALID_ADDR) break;

        StoreVal = StoreValPipe::read();
      }

      if constexpr (IsGated) {
        StoreAddrDone = false;
      
        bool succ = false;
        auto signal = GateSignalPipe::read(succ);
        if (succ) {
          GateSignalAddr = signal.addr;
          if constexpr (UseSchedule) 
            GateSignalSched = signal.sched;
        }
      }

      bool IsSafe = true;
      if constexpr (IsGated) {
        if constexpr (UseSchedule) {
          IsSafe = (StoreSched < GateSignalSched) ||
                   (StoreSched == GateSignalSched && StoreAddr <= GateSignalAddr);
        } else {
          IsSafe = StoreAddr <= GateSignalAddr;
        }
      }

      if (IsSafe) {
        if constexpr (IsGated) 
          StoreAddrDone = true;

        auto StorePtr = ext::intel::device_ptr<T>(data + StoreAddr);
        BurstCoalescedLSU::store(StorePtr, StoreVal);

        if constexpr (IsSignaling) {
          if constexpr (UseSchedule)
            SendSignalPipe::write({StoreAddr, StoreSched, StoreVal});
          else
            SendSignalPipe::write({StoreAddr, StoreVal});
        }
      }
    }

    if constexpr (IsSignaling) {
      if constexpr (UseSchedule)
        SendSignalPipe::write({-1, MAX_INT});
      else
        SendSignalPipe::write({MAX_INT});
    }
  });
}
