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

/// The below types are the possible types of a gate signal.
struct addr_t {
  int addr;
};
struct addr_sched_t {
  int addr;
  int sched;
};
template <typename T> struct addr_sched_val_t {
  int addr;
  int sched;
  T val;
};
template <typename T> struct addr_val_t {
  int addr;
  T val;
};

constexpr int INVALID_ADDR = -1;
constexpr int MAX_INT = 0x7FFFFFFF;

/// Unique kernel name generators.
template <int ID> class StreamingLoadKernel;
template <int ID> class StreamingStoreKernel;

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
template <int id, typename LoadAddrPipe, typename LoadValPipe, 
          typename SendSignalPipe=NoPipe, typename GateSignalPipe=NoPipe, 
          bool InterIteration=false, typename T>
event StreamingLoad(queue &q, T *data) {
  constexpr bool IsGated = !std::is_same<GateSignalPipe, NoPipe>::value;
  constexpr bool IsSignaling = !std::is_same<SendSignalPipe, NoPipe>::value;
  using AddrType = decltype(LoadAddrPipe::read());
  constexpr bool UseSchedule = std::is_same<AddrType, addr_sched_t>::value;

  using KernelID = StreamingLoadKernel<id>;
  return q.single_task<KernelID>([=]() [[intel::kernel_args_restrict]] {
    int storeFifoAddr[kStoreFifoSize];
    T storeFifoVal[kStoreFifoSize];
    // The latest gate signal value.
    // NOTE: Assuming max one gate!
    int GateSignalAddr = -1, GateSignalSched = 0;

    // If this load is gated, then wait for the gate signal before starting.
    // Unless the gate is {InterIteration}, in which case we wiil have to only
    // gate on {schedule > 0}.
    if constexpr (IsGated) {
      #pragma unroll
      for (int i = 0; i < kStoreFifoSize; ++i) {
        storeFifoAddr[i] = INVALID_ADDR;
        storeFifoVal[i] = 0;
      }

      if constexpr (!InterIteration) {
        auto signal = GateSignalPipe::read();
        // Gate {addr, value} are shifted in at at end of the store reuse FIFOs.
        storeFifoAddr[kStoreFifoSize - 1] = signal.addr;
        storeFifoVal[kStoreFifoSize - 1] = signal.val;
        GateSignalAddr = signal.addr;
        if constexpr (UseSchedule)
          GateSignalSched = signal.sched;
      }
    }

    // Execute the load until an INVALID_ADDR arrives.
    int LoadAddr = 0, LoadSched = 0;
    bool LoadAddrDone = true;
    while (true) {
      // Only read new load address and value if the previous load is done.
      if (LoadAddrDone) {
        auto LoadAddrRead = LoadAddrPipe::read();
        LoadAddr = LoadAddrRead.addr;
        if constexpr (UseSchedule) {
          LoadSched = LoadAddrRead.sched;
        } 

        if (LoadAddr == INVALID_ADDR) break;
      }

      // If gated, try reading latest gate. If read successfully, shift in at
      // the end of store reuse FIFOs.
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

      // If gated, then check if the load is safe to do.
      bool IsSafe = true;
      if constexpr (IsGated) {
        if constexpr (UseSchedule) {
          if constexpr (!InterIteration) {
            IsSafe = (LoadSched < GateSignalSched) ||
                     (LoadSched >= GateSignalSched && LoadAddr <= GateSignalAddr);
          } else { 
            IsSafe = (LoadSched <= GateSignalSched) ||
                     (LoadSched > GateSignalSched && LoadAddr <= GateSignalAddr);
          }
        } else { // NoSchedule, only check address.
          IsSafe = LoadAddr <= GateSignalAddr;
        }
      } // else if not gated then IsSafe.

      // Do the load and return value, if safe.
      if (IsSafe) {
        bool reuse = false;
        T reuseVal;
        // Check if we can reuse a value from the store reuse FIFO.
        if constexpr (IsGated) {
          LoadAddrDone = true;

          // TODO: Just use storeFifoVal[(GateSignalAddr - LoadAddr) / step];
          // Youngest store wins.
          #pragma unroll
          for (int i = 0; i < kStoreFifoSize; ++i) {
            if (LoadAddr == storeFifoAddr[i]) {
              reuse = true;
              reuseVal = storeFifoVal[i];
            }
          }

          // If the gate is {InterIteration}, then we can only reuse values
          // from earlier iteration.
          if constexpr (InterIteration) 
            reuse &= (LoadSched > GateSignalSched);
        }

        auto LoadPtr = ext::intel::device_ptr<T>(data + LoadAddr);
        auto LoadVal = reuse ? reuseVal : BurstCoalescedLSU::load(LoadPtr);
        LoadValPipe::write(LoadVal);

        // Use LoadVal here to ensure load has returned a value.
        if constexpr (IsSignaling) {
          if constexpr (UseSchedule)
            SendSignalPipe::write({LoadAddr, LoadSched, LoadVal});
          else
            SendSignalPipe::write({LoadAddr, LoadVal});
        }
      }
    }

    // If signaling, write out MAXI_INT for schedule/address.
    if constexpr (IsSignaling) {
      if constexpr (UseSchedule)
        SendSignalPipe::write({MAX_INT, MAX_INT});
      else
        SendSignalPipe::write({MAX_INT});
    }

    // Drain gate signals.
    if constexpr (IsGated) {
      while (GateSignalAddr != MAX_INT) {
        auto signal = GateSignalPipe::read();
        GateSignalAddr = signal.addr;
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
template <int id, typename StoreAddrPipe, typename StoreValPipe, 
          typename SendSignalPipe=NoPipe, typename GateSignalPipes=NoPipe, 
          int NumGateSignals=0, bool InterIteration=false, typename T>
event StreamingStore(queue &q, T *data) {
  constexpr bool IsGated = !std::is_same<GateSignalPipes, NoPipe>::value;
  constexpr bool IsSignaling = !std::is_same<SendSignalPipe, NoPipe>::value;
  using AddrType = decltype(StoreAddrPipe::read());
  constexpr bool UseSchedule = std::is_same<AddrType, addr_sched_t>::value;

  assert(
      ((!IsGated && NumGateSignals == 0) || (IsGated && NumGateSignals > 0)) &&
      "Incorrect NumGateSignals.");

  using KernelID = StreamingStoreKernel<id>;
  return q.single_task<KernelID>([=]() [[intel::kernel_args_restrict]] {
    // Can't have zero sized arrays.
    int GateSignalAddr[std::max(NumGateSignals, 1)]; 
    int GateSignalSched[std::max(NumGateSignals, 1)];

    // If this store is gated, then wait for the gate signal before starting.
    // Unless the gate is {InterIteration}, in which case we wiil have to only
    // gate on {schedule > 0}.
    if constexpr (IsGated) {
      UnrolledLoop<NumGateSignals>([&](auto k) {
        GateSignalAddr[k] = -1;
        if constexpr (UseSchedule)
          GateSignalSched[k] = 0;
        
        if constexpr (!InterIteration) {
          auto signal = GateSignalPipes:: template PipeAt<k>::read();
          GateSignalAddr[k] = signal.addr;
          if constexpr (UseSchedule)
            GateSignalSched[k] = signal.sched;
        }
      });
    }

    // Execute the store until an INVALID_ADDR arrives.
    // StoreAddr and StoreVal arrive on separate pipes.
    int StoreAddr = 0, StoreSched = 0;
    T StoreVal;
    bool StoreAddrDone = true;
    while (true) {
      // Only read new store address and value if the previous store is done.
      if (StoreAddrDone) {
        auto StoreAddrRead = StoreAddrPipe::read();
        StoreAddr = StoreAddrRead.addr;
        if constexpr (UseSchedule) {
          StoreSched = StoreAddrRead.sched;
        } 

        if (StoreAddr == INVALID_ADDR) break;

        StoreVal = StoreValPipe::read();
      }

      // If the store is gated, then we need to check for safety.
      bool IsSafe = true;
      if constexpr (IsGated) {
        // If gated, this store address might not be stored this iteration.
        StoreAddrDone = false;

        // Try to get the latest gate signal.
        UnrolledLoop<NumGateSignals>([&](auto k) {
          bool succ = false;
          auto signal = GateSignalPipes:: template PipeAt<k>::read(succ);
          if (succ) {
            GateSignalAddr[k] = signal.addr;
            if constexpr (UseSchedule)
              GateSignalSched[k] = signal.sched;
          }
        });

        // Check all gates for safety.
        UnrolledLoop<NumGateSignals>([&](auto k) {
          if constexpr (UseSchedule) {
            if constexpr (!InterIteration) {
              IsSafe &= (StoreSched < GateSignalSched[k]) ||
                        (StoreSched == GateSignalSched[k] &&
                        StoreAddr <= GateSignalAddr[k]);
            } else {
              IsSafe &= (StoreSched == GateSignalSched[k]) ||
                        (StoreSched > GateSignalSched[k] &&
                        StoreAddr <= GateSignalAddr[k]);
            }
          } else { // No Schedule
            IsSafe &= StoreAddr <= GateSignalAddr[k];
          }
        });
      }

      // Execute store, if safe.
      if (IsSafe) {
        if constexpr (IsGated) 
          StoreAddrDone = true;

        auto StorePtr = ext::intel::device_ptr<T>(data + StoreAddr);
        BurstCoalescedLSU::store(StorePtr, StoreVal);

        // Optional signal. Note that {SendSignalPipe} can be an array of pipes
        // and this write will stall untill all writes complete.
        if constexpr (IsSignaling) {
          if constexpr (UseSchedule)
            SendSignalPipe::write({StoreAddr, StoreSched, StoreVal});
          else
            SendSignalPipe::write({StoreAddr, StoreVal});
        }
      }
    }

    // If signaling, write out MAXI_INT for schedule/address.
    if constexpr (IsSignaling) {
      if constexpr (UseSchedule)
        SendSignalPipe::write({MAX_INT, MAX_INT});
      else
        SendSignalPipe::write({MAX_INT});
    }
    
    // PRINTF("Sst draining\n");
    // Drain gate signals.
    if constexpr (IsGated) {
      bool any_left_to_drain = true;
      while (any_left_to_drain) {
        any_left_to_drain = false;
        UnrolledLoop<NumGateSignals>([&](auto k) {
          if (GateSignalAddr[k] != MAX_INT) {
            any_left_to_drain = false;

            bool succ = false;
            auto signal = GateSignalPipes:: template PipeAt<k>::read(succ);
            if (succ) 
              GateSignalAddr[k] = signal.addr;
          }
        });
      }
    }
    
    PRINTF("Done sst %d\n", id);
  });
}
