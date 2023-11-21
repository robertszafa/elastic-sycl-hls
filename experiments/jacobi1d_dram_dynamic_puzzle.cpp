#include <algorithm>
#include <iostream>
#include <limits>
#include <math.h>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <sycl/sycl.hpp>
#include <vector>

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

constexpr int LOOP_FINISHED_CODE = -2;

struct polly_schedule_t { 
  int t; int i; float val; 
  
  void increment(const int tMax, const int iMax) {
    i = (i + 1);
    if (i == iMax) {
      t += 1;
      i = 0;
    }
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

struct tagged_val_t { float val; bool reuse; };


[[clang::always_inline]] int accessSstB (const polly_schedule_t &sched) { 
  return sched.i; 
}
[[clang::always_inline]] int accessSldB0 (const polly_schedule_t &sched) { 
  return sched.i - 1; 
}
[[clang::always_inline]] int accessSldB1 (const polly_schedule_t &sched) { 
  return sched.i; 
}
[[clang::always_inline]] int accessSldB2 (const polly_schedule_t &sched) { 
  return sched.i + 1; 
}



double jacobi1d_kernel(queue &q, std::vector<float> &h_A,
                       std::vector<float> &h_B, const int timeSteps) {
  const int kN = h_A.size();

  auto *A = fpga_tools::toDevice(h_A, q);
  auto *B = fpga_tools::toDevice(h_B, q);

  using pipe_done_B0 = pipe<class pipe_done_B_0_class, polly_schedule_t, 1024>;
  using pipe_done_B1 = pipe<class pipe_done_B_1_class, polly_schedule_t, 1024>;
  using pipe_done_B2 = pipe<class pipe_done_B_2_class, polly_schedule_t, 1024>;
  using pipe_done_A = pipe<class pipe_done_A_class, polly_schedule_t, 1024>;
  using pipe_gate_B0 = pipe<class pipe_gate_B_0_class, tagged_val_t, 1024>;
  using pipe_gate_B1 = pipe<class pipe_gate_B_1_class, tagged_val_t, 1024>;
  using pipe_gate_B2 = pipe<class pipe_gate_B_2_class, tagged_val_t, 1024>;

  using pipe_B0 = pipe<class pipe_B_0_class, float, 1024>;
  using pipe_B1 = pipe<class pipe_B_1_class, float, 1024>;
  using pipe_B2 = pipe<class pipe_B_2_class, float, 1024>;

  using pipe_A_0 = pipe<class pipe_A_0_class, float, 1024>;
  using pipe_A_1 = pipe<class pipe_A_1_class, float, 1024>;
  using pipe_A_2 = pipe<class pipe_A_2_class, float, 1024>;

  using pipe_B_store = pipe<class pipe_B_store_class, float, 1024>;
  using pipe_A_store = pipe<class pipe_A_store_class, float, 1024>;
  
 /*
  Compute
 */
 auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 1; i < kN - 1; i++) {
        // pipe_B_store::write(0.33333f * (pipe_A_0::read() + pipe_A_1::read() +
        //                                 pipe_A_2::read()));
        pipe_B_store::write(0.33333f * (A[i-1] + A[i] + A[i+1]));
      }
    }
  });
  auto event2 = q.single_task<class MainKernel2>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 1; i < kN - 1; i++) {
        pipe_A_store::write(0.33333f * (pipe_B0::read() + pipe_B1::read() +
                                        pipe_B2::read()));
      }
    }
  });
  
  /*
    Stores
  */
  auto sstB = q.single_task<class sstB>([=]() [[intel::kernel_args_restrict]] {
    bool signalSent = false;

    polly_schedule_t schedSstB;

    for (int t = 0; t < timeSteps; t++) {
      for (int i = 1; i < kN - 1; i++) {
        auto val = pipe_B_store::read();
        B[i] = val;

        schedSstB = {t, i, val};
        pipe_done_B0::write(schedSstB);
        pipe_done_B1::write(schedSstB);
        pipe_done_B2::write(schedSstB);
      }
    }

    // schedSstB.t++;
    // pipe_done_B0::write(schedSstB);
    // pipe_done_B1::write(schedSstB);
    // pipe_done_B2::write(schedSstB);
  });
  auto sstA = q.single_task<class sstA>([=]() [[intel::kernel_args_restrict]] {
    bool signalSent = false;

    for (int t = 0; t < timeSteps; t++) {
      for (int i = 1; i < kN - 1; i++) {
        A[i] = pipe_A_store::read();

        // pipe_done_A::write(1);
      }
    }
  });

  /*
    Loads
  */
  // auto sldA = q.single_task<class sldA>([=]() [[intel::kernel_args_restrict]] {
  //   for (int t = 0; t < timeSteps; t++) {
  //     // if (t > 0)
  //     //   pipe_done_A::read();

  //     for (int i = 1; i < kN - 1; i++) {
  //       pipe_A_0::write(BurstCoalescedLSU::load(
  //           sycl::ext::intel::device_ptr<float>(A + (i - 1))));
  //       pipe_A_1::write(BurstCoalescedLSU::load(
  //           sycl::ext::intel::device_ptr<float>(A + (i))));
  //       pipe_A_2::write(BurstCoalescedLSU::load(
  //           sycl::ext::intel::device_ptr<float>(A + (i + 1))));
  //     }
  //   }
  // });

  auto gateSldB0 = q.single_task<class gateSldB0>([=]() [[intel::kernel_args_restrict]] {
    polly_schedule_t schedSstB = pipe_done_B0::read();
    polly_schedule_t schedSldB0;
    
    for (int t = 0; t < timeSteps; t++) {
      int i = 1;
      while (i < kN - 1) {
        schedSldB0 = {t, i};

        bool safe = (schedSldB0 < schedSstB) | (schedSstB.i >= schedSldB0.i);
        bool reuse = (schedSstB.i == (schedSldB0.i-1));
        schedSldB0.val = schedSstB.val;

        if (safe) {
          pipe_gate_B0::write({schedSldB0.val, reuse});
          i++;
        } 

        bool succ = false;
        auto tmp = pipe_done_B0::read(succ);
        if (succ) schedSstB = tmp;
      }
    }
  });
  auto sldB0 = q.single_task<class sldB0>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 1; i < kN - 1; i++) {
        tagged_val_t valTagged = pipe_gate_B0::read();

        auto addrToLd = sycl::ext::intel::device_ptr<float>(B + i - 1);
        auto ld =
            valTagged.reuse ? valTagged.val : BurstCoalescedLSU::load(addrToLd);

        pipe_B0::write(ld);
      }
    }
  });

  auto gateSldB1 = q.single_task<class gateSldB1>([=]() [[intel::kernel_args_restrict]] {
    polly_schedule_t schedSstB = pipe_done_B1::read();
    polly_schedule_t schedSldB1;
    
    for (int t = 0; t < timeSteps; t++) {
      int i = 1;
      while (i < kN - 1) {
        schedSldB1 = {t, i};

        bool safe = (schedSldB1 < schedSstB) | (schedSstB.i >= schedSldB1.i);
        bool reuse = (schedSstB.i == (schedSldB1.i));
        schedSldB1.val = schedSstB.val;

        if (safe) {
          pipe_gate_B1::write({schedSldB1.val, reuse});
          i++;
        } 

        bool succ = false;
        auto tmp = pipe_done_B1::read(succ);
        if (succ) schedSstB = tmp;
      }
    }
  });
  auto sldB1 = q.single_task<class sldB1>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 1; i < kN - 1; i++) {
        tagged_val_t valTagged = pipe_gate_B1::read();

        auto addrToLd = sycl::ext::intel::device_ptr<float>(B + i);
        auto ld =
            valTagged.reuse ? valTagged.val : BurstCoalescedLSU::load(addrToLd);

        pipe_B1::write(ld);
      }
    }
  });

  auto gateSldB2 = q.single_task<class gateSldB2>([=]() [[intel::kernel_args_restrict]] {
    polly_schedule_t schedSstB = pipe_done_B2::read();
    polly_schedule_t schedSldB2;
    
    for (int t = 0; t < timeSteps; t++) {
      int i = 1;
      while (i < kN - 1) {
        schedSldB2 = {t, i};

        bool safe = (schedSldB2 < schedSstB) | (schedSstB.i >= schedSldB2.i);
        bool reuse = (schedSstB.i == (schedSldB2.i+1));
        schedSldB2.val = schedSstB.val;

        if (safe) {
          pipe_gate_B2::write({schedSldB2.val, reuse});
          i++;
        } 

        bool succ = false;
        auto tmp = pipe_done_B2::read(succ);
        if (succ) schedSstB = tmp;
      }
    }
  });
  auto sldB2 = q.single_task<class sldB2>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 1; i < kN - 1; i++) {
        tagged_val_t valTagged = pipe_gate_B2::read();

        auto addrToLd = sycl::ext::intel::device_ptr<float>(B + i + 1);
        auto ld =
            valTagged.reuse ? valTagged.val : BurstCoalescedLSU::load(addrToLd);

        pipe_B2::write(ld);
      }
    }
  });


  event.wait();
  event2.wait();

  sstB.wait();
  sstA.wait();
  sldB0.wait();
  sldB1.wait();
  sldB2.wait();

  gateSldB0.wait();
  gateSldB1.wait();
  gateSldB2.wait();

  q.copy(A, h_A.data(), h_A.size()).wait();
  q.copy(B, h_B.data(), h_B.size()).wait();

  sycl::free(A, q);
  sycl::free(B, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void jacobi1d_cpu(std::vector<float> &A, std::vector<float> &B,
                  const int timeSteps) {
  const int kN = A.size();

  for (int t = 0; t < timeSteps; t++) {
    for (int i = 1; i < kN - 1; i++) {
      B[i] = 0.33333f * (A[i - 1] + A[i] + A[i + 1]);
    }

    for (int i = 1; i < kN - 1; i++) {
      A[i] = 0.33333f * (B[i - 1] + B[i] + B[i + 1]);
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

    auto kernel_time = jacobi1d_kernel(q, A, B, TIME_STEPS);
    jacobi1d_cpu(A_cpu, B_cpu, TIME_STEPS);

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
