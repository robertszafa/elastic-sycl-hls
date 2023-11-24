#include <algorithm>
#include <iostream>
#include <limits>
#include <math.h>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <sycl/sycl.hpp>
#include <vector>
#include <functional>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "exception_handler.hpp"
#include "memory_utils.hpp"

#include "pipe_utils.hpp"
#include "unrolled_loop.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;
class MainKernel2;

using PipelinedLSU = sycl::ext::intel::lsu<>;
using BurstCoalescedLSU =
    ext::intel::lsu<ext::intel::burst_coalesce<true>,
                    ext::intel::prefetch<false>>;
                    // ext::intel::statically_coalesce<false>>;

constexpr int kDramDelay = 512;
constexpr int kStoreFifoSize = 8;

struct addr_val_t { int addr; float val; };

struct tagged_val_t { float val; bool reuse; };

struct polly_schedule_t { 
  int t; int i; 
  int tMax; int iMax;

  void increment() {
    auto iNew = i + 1;
    if (iNew == iMax) {
      i = 0;
      t++;
    } else {
      i = iNew;
    }
  }

  bool done() {
    return t >= tMax;
  }

  bool operator<(const polly_schedule_t &other) const {
    if (other.t == this->t) return this->i < other.i;
    return this->t < other.t;
  }

  bool operator>(const polly_schedule_t &other) const {
    if (other.t == this->t) return this->i > other.i;
    return this->t > other.t;
  }
  
  bool operator>=(const polly_schedule_t &other) const {
    if (other.t == this->t) return this->i >= other.i;
    return this->t >= other.t;
  }
};


[[clang::always_inline]] int accessSstB (const polly_schedule_t &sched) { 
  return sched.i; 
}
[[clang::always_inline]] int accessSldB0 (const polly_schedule_t &sched) { 
  return sched.i; 
}

[[clang::always_inline]] int accessSstA (const polly_schedule_t &sched) { 
  return sched.i; 
}
[[clang::always_inline]] int accessSldA0 (const polly_schedule_t &sched) { 
  return sched.i; 
}

template <typename SignalPipe, typename T>
inline void
StoreToLoadDependence(const int StoreAddr,
                      const polly_schedule_t &StoreSchedule, const int LoadAddr,
                      polly_schedule_t &LoadSchedule, const T StoreVal) {
  bool reuse = StoreAddr == LoadAddr;
  bool safe = StoreAddr >= LoadAddr;
  if (safe) {
    SignalPipe::write({StoreVal, reuse});
    LoadSchedule.increment();
  }
}

template <typename SignalPipe>
inline void LoadToStoreDependence(const int LoadAddr,
                                  const polly_schedule_t &LoadSchedule,
                                  const int StoreAddr,
                                  polly_schedule_t &StoreSchedule) {

  bool safe = LoadAddr >= StoreAddr;
  if (safe) {
    SignalPipe::write({});
    StoreSchedule.increment();
  }
}

template <typename SignalPipe>
inline void SignalUntilDone(polly_schedule_t &Schedule) {
  while (!Schedule.done()) {
    SignalPipe::write({});
    Schedule.increment();
  }
}

double simple_loop_fusion_kernel(queue &q, std::vector<float> &h_A,
                       std::vector<float> &h_B, const int timeSteps) {
  const int kN = h_A.size();

  auto *A = fpga_tools::toDevice(h_A, q);
  auto *B = fpga_tools::toDevice(h_B, q);

  using signal_storeB0_loadB0 =
      pipe<class pipe_done_B_0_class, tagged_val_t, 64>;
  using signal_loadB0_storeB0 =
      pipe<class pipe_loaded_B0_class, tagged_val_t, kDramDelay>;
  using signal_loadA0_storeA0 =
      pipe<class pipe_loaded_A0_class, tagged_val_t, 64>;
  using signal_storeA0_loadA0 = 
      pipe<class pipe_done_A_0_class, tagged_val_t, kDramDelay>;

  using pipe_loadB0 = pipe<class pipe_B_0_class, float, 64>;
  using pipe_loadA0 = pipe<class pipe_A_0_class, float, 64>;
  using pipe_storeB0 = pipe<class pipe_B_store_class, float, 64>;
  using pipe_storeA0 = pipe<class pipe_A_store_class, float, 64>;
  
 /*
  Compute
 */
 auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 0; i < kN; i++) {
        pipe_storeB0::write(0.33333f * pipe_loadA0::read());
      }
    }
  });
  auto event2 = q.single_task<MainKernel2>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 0; i < kN; i++) {
        pipe_storeA0::write(0.33333f * pipe_loadB0::read());
      }
    }
  });
  

  /*
    B
  */
  
  auto sldB0 = q.single_task<class sldB0>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 0; i < kN; i++) {
        auto reuse = signal_storeB0_loadB0::read();

        auto addrToLd =
            sycl::ext::intel::device_ptr<float>(B + i);
        auto ld = reuse.reuse ? reuse.val : BurstCoalescedLSU::load(addrToLd);
        pipe_loadB0::write(ld);
      }

      if (timeSteps > 1) {
        for (int i = 0; i < kDramDelay; ++i) 
          signal_loadB0_storeB0::write({});
      }
    }
  });
  auto sstB = q.single_task<class sstB>([=]() [[intel::kernel_args_restrict]] {
    bool signalSent = false;

    polly_schedule_t schedSstB = {0, 0, timeSteps, kN};
    polly_schedule_t ScheduleLoadB0 = {0, 0, timeSteps, kN};

    for (int t = 0; t < timeSteps; t++) {
      if (t > 0) {
        for (int i = 0; i < kDramDelay; ++i) 
          signal_loadB0_storeB0::read();
      }

      for (int i = 0; i < kN; i++) {
        auto val = pipe_storeB0::read();
        B[i] = val;

        schedSstB.t = t;
        schedSstB.i = i;
        StoreToLoadDependence<signal_storeB0_loadB0>(
            accessSstB(schedSstB), schedSstB, accessSldB0(ScheduleLoadB0),
            ScheduleLoadB0, val);
      }
    }

    SignalUntilDone<signal_storeB0_loadB0>(ScheduleLoadB0);
  });

  /*
    A
  */
  auto sldA0 = q.single_task<class sldA0>([=]() [[intel::kernel_args_restrict]] {
    polly_schedule_t ScheduleLoadA0 = {0, 0, timeSteps, kN};
    polly_schedule_t ScheduleStoreA0 = {0, 0, timeSteps, kN};
    
    for (int t = 0; t < timeSteps; t++) {
      if (t > 0) {
        for (int i = 0; i < kDramDelay; ++i) 
          signal_storeA0_loadA0::read();
      }

      for (int i = 0; i < kN; i++) {
        // auto reuse = pipe_done_A0::read();
        tagged_val_t reuse = {0, 0};

        auto addrToLd =
            sycl::ext::intel::device_ptr<float>(A + i);
        auto ld = reuse.reuse ? reuse.val : BurstCoalescedLSU::load(addrToLd);
        pipe_loadA0::write(ld);

        ScheduleLoadA0.t = t;
        ScheduleLoadA0.i = i;
        LoadToStoreDependence<signal_loadA0_storeA0>(
            accessSldA0(ScheduleLoadA0), ScheduleLoadA0,
            accessSstA(ScheduleStoreA0), ScheduleStoreA0);
      }
    }

    SignalUntilDone<signal_loadA0_storeA0>(ScheduleStoreA0);
  });
  auto sstA = q.single_task<class sstA>([=]() [[intel::kernel_args_restrict]] {
    polly_schedule_t ScheduleStoreA0;

    for (int t = 0; t < timeSteps; t++) {
      for (int i = 0; i < kN; i++) {
        auto _ = signal_loadA0_storeA0::read();

        ScheduleStoreA0 = {t, i};
        
        auto val = pipe_storeA0::read();
        A[i] = val;
      }

      if (timeSteps > 1) {
        for (int i = 0; i < kDramDelay; ++i) 
          signal_storeA0_loadA0::write({});
      }
    }
  });


  event.wait();
  event2.wait();

  sstB.wait();
  sstA.wait();

  sldB0.wait();
  sldA0.wait();

  q.copy(A, h_A.data(), h_A.size()).wait();
  q.copy(B, h_B.data(), h_B.size()).wait();

  sycl::free(A, q);
  sycl::free(B, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void simple_loop_fusion_cpu(std::vector<float> &A, std::vector<float> &B,
                  const int timeSteps) {
  const int kN = A.size();

  for (int t = 0; t < timeSteps; t++) {
    for (int i = 0; i < kN; i++) {
      B[i] = 0.33333f * A[i];
    }

    for (int i = 0; i < kN; i++) {
      A[i] = 0.33333f * B[i];
    }
  }
}

int main(int argc, char *argv[]) {
  int ARRAY_SIZE = 1000;
  int TIME_STEPS = 1;
  try {
    if (argc > 1) {
      ARRAY_SIZE = int(atoi(argv[1]));
    }
    if (argc > 2) {
      TIME_STEPS = int(atoi(argv[2]));
      if (TIME_STEPS < 0)
        throw std::invalid_argument("Invalid time steps.");
    }
  } catch (exception const &e) {
    std::cout << "Incorrect argv.\nUsage:\n"
              << "  ./executable [ARRAY_SIZE] [PERCENTAGE (% of iterations "
                 "with dependencies.)]\n";
    std::terminate();
  }

#if FPGA_SIM
  auto d_selector = sycl::ext::intel::fpga_simulator_selector_v;
#elif FPGA_HW
  auto d_selector = sycl::ext::intel::fpga_selector_v;
#else // #if FPGA_EMULATOR
  auto d_selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif

  try {
    // Enable profiling.
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<float> A(ARRAY_SIZE, 1);
    std::vector<float> A_cpu(ARRAY_SIZE, 1);
    std::vector<float> B(ARRAY_SIZE, 2);
    std::vector<float> B_cpu(ARRAY_SIZE, 2);

    auto kernel_time = simple_loop_fusion_kernel(q, A, B, TIME_STEPS);
    simple_loop_fusion_cpu(A_cpu, B_cpu, TIME_STEPS);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    bool equalAs = std::equal(A.begin(), A.end(), A_cpu.begin());
    bool equalBs = std::equal(B.begin(), B.end(), B_cpu.begin());
    if (equalAs && equalBs)
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
    
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
