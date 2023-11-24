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

using PipelinedLSU = sycl::ext::intel::lsu<>;
using BurstCoalescedLSU =
    ext::intel::lsu<ext::intel::burst_coalesce<true>,
                    ext::intel::prefetch<false>>;
                    // ext::intel::statically_coalesce<false>>;

constexpr int kDramDelay = 1024;
constexpr int kStoreFifoSize = 8;

struct addr_val_t { int addr; float val; };

struct tagged_val_t { float val; bool reuse; };

struct polly_schedule_t { 
  int t; int i; float val; 

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


double simple_loop_fusion_kernel(queue &q, std::vector<float> &h_A,
                       std::vector<float> &h_B, const int timeSteps) {
  const int kN = h_A.size();

  auto *A = fpga_tools::toDevice(h_A, q);
  auto *B = fpga_tools::toDevice(h_B, q);

  using pipe_done_B0 = pipe<class pipe_done_B_0_class, tagged_val_t, 64>;
  using pipe_done_A0 = pipe<class pipe_done_A_0_class, polly_schedule_t, 64>;
  using pipe_loaded_A0 = pipe<class pipe_loaded_A0_class, polly_schedule_t, 64>;
  using pipe_B0 = pipe<class pipe_B_0_class, float, 64>;
  using pipe_A0 = pipe<class pipe_A_0_class, float, 64>;

  using pipe_B_store = pipe<class pipe_B_store_class, float, 64>;
  using pipe_A_store = pipe<class pipe_A_store_class, float, 64>;
  
 /*
  Compute
 */
 auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 0; i < kN; i++) {
        pipe_B_store::write(0.33333f * pipe_A0::read());
        // pipe_B_store::write(0.33333f * A[i]);
      }
    }
  });
  auto event2 = q.single_task<class MainKernel2>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 0; i < kN; i++) {
        pipe_A_store::write(0.33333f * pipe_B0::read());
      }
    }
  });
  

  /*
    B
  */
  auto sstB = q.single_task<class sstB>([=]() [[intel::kernel_args_restrict]] {
    bool signalSent = false;

    polly_schedule_t schedSstB;
    polly_schedule_t schedSldB0 = {0, 0};

    [[intel::fpga_register]] addr_val_t storeFifo[kStoreFifoSize];
    #pragma unroll
    for (int iC = 0; iC < kStoreFifoSize; ++iC) storeFifo[iC] = {-1, 0};

    for (int t = 0; t < timeSteps; t++) {
      for (int i = 0; i < kN; i++) {
        auto val = pipe_B_store::read();
        B[i] = val;

        schedSstB = {t, i, val};
        bool reuse = accessSldB0(schedSldB0) == accessSstB(schedSstB);
        bool safe = accessSldB0(schedSldB0) <= accessSstB(schedSstB);
        if (safe) {
          pipe_done_B0::write({val, reuse});
          schedSldB0.i++;
        } 
        // else (access_store < access_load), so wait for more stores

      }
    }

    while (schedSldB0.i < kN) {
      pipe_done_B0::write({0, 0});
      schedSldB0.i++;
    }
  });

  auto sldB0 = q.single_task<class sldB0>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 0; i < kN; i++) {
        auto reuse = pipe_done_B0::read();

        auto addrToLd =
            sycl::ext::intel::device_ptr<float>(B + i);
        auto ld = reuse.reuse ? reuse.val : BurstCoalescedLSU::load(addrToLd);
        pipe_B0::write(ld);
      }
    }
  });

  /*
    A
  */
  auto sldA0 = q.single_task<class sldA0>([=]() [[intel::kernel_args_restrict]] {
    polly_schedule_t schedSldA0;
    polly_schedule_t schedSstA = {0, 0};
    
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 0; i < kN; i++) {
        // auto reuse = pipe_done_A0::read();
        tagged_val_t reuse = {0, 0};

        auto addrToLd =
            sycl::ext::intel::device_ptr<float>(A + i);
        auto ld = reuse.reuse ? reuse.val : BurstCoalescedLSU::load(addrToLd);
        pipe_A0::write(ld);

        schedSldA0 = {t, i};
        bool safe = accessSstA(schedSstA) <= accessSldA0(schedSldA0);
        if (safe) {
          pipe_loaded_A0::write({});
          schedSstA.i++;
        }
      }
    }

    while (schedSstA.i < kN) {
      pipe_loaded_A0::write({});
      schedSstA.i++;
    }
  });

  auto sstA = q.single_task<class sstA>([=]() [[intel::kernel_args_restrict]] {
    polly_schedule_t schedSstA;

    for (int t = 0; t < timeSteps; t++) {
      for (int i = 0; i < kN; i++) {
        auto _ = pipe_loaded_A0::read();

        schedSstA = {t, i};
        
        schedSstA.val = pipe_A_store::read();
        A[i] = schedSstA.val;
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
