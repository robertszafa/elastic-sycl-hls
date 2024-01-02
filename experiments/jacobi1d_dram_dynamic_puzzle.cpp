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
  return sched.i - 1; 
}
[[clang::always_inline]] int accessSldB1 (const polly_schedule_t &sched) { 
  return sched.i; 
}
[[clang::always_inline]] int accessSldB2 (const polly_schedule_t &sched) { 
  return sched.i + 1; 
}

[[clang::always_inline]] int accessSstA (const polly_schedule_t &sched) { 
  return sched.i; 
}
[[clang::always_inline]] int accessSldA0 (const polly_schedule_t &sched) { 
  return sched.i - 1; 
}
[[clang::always_inline]] int accessSldA1 (const polly_schedule_t &sched) { 
  return sched.i; 
}
[[clang::always_inline]] int accessSldA2 (const polly_schedule_t &sched) { 
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

  using pipe_done_A0 = pipe<class pipe_done_A_0_class, polly_schedule_t, 1024>;
  using pipe_done_A1 = pipe<class pipe_done_A_1_class, polly_schedule_t, 1024>;
  using pipe_done_A2 = pipe<class pipe_done_A_2_class, polly_schedule_t, 1024>;

  using pipe_loaded_A0 = pipe<class pipe_loaded_A0_class, polly_schedule_t, 1024>;
  using pipe_loaded_A1 = pipe<class pipe_loaded_A1_class, polly_schedule_t, 1024>;
  using pipe_loaded_A2 = pipe<class pipe_loaded_A2_class, polly_schedule_t, 1024>;

  using pipe_B0 = pipe<class pipe_B_0_class, float, 1024>;
  using pipe_B1 = pipe<class pipe_B_1_class, float, 1024>;
  using pipe_B2 = pipe<class pipe_B_2_class, float, 1024>;

  using pipe_A0 = pipe<class pipe_A_0_class, float, 1024>;
  using pipe_A1 = pipe<class pipe_A_1_class, float, 1024>;
  using pipe_A2 = pipe<class pipe_A_2_class, float, 1024>;

  using pipe_B_store = pipe<class pipe_B_store_class, float, 1024>;
  using pipe_A_store = pipe<class pipe_A_store_class, float, 1024>;
  
 /*
  Compute
 */
 auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 1; i < kN - 1; i++) {
        pipe_B_store::write(0.33333f * (pipe_A0::read() + pipe_A1::read() +
                                        pipe_A2::read()));
        // pipe_B_store::write(0.33333f * (A[i-1] + A[i] + A[i+1]));
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

    // Signal end of loop nest.
    schedSstB.t++;
    pipe_done_B0::write(schedSstB);
    pipe_done_B1::write(schedSstB);
    pipe_done_B2::write(schedSstB);

  });
  auto sstA = q.single_task<class sstA>([=]() [[intel::kernel_args_restrict]] {
    polly_schedule_t schedSstA;
    for (int t = 0; t < timeSteps; t++) {
      polly_schedule_t schedSldA2 = pipe_loaded_A2::read();

      // for (int i = 1; i < kN - 1; i++) {
      int i = 1;
      while (i < kN - 1) {
        schedSstA = {t, i};

        bool safe = (schedSstA.t < schedSldA2.t) |
                    (accessSstA(schedSstA) <= accessSldA2(schedSldA2)); 

        if (safe) {
          schedSstA.val = pipe_A_store::read();
          A[i] = schedSstA.val;
          i++;
        } 

        bool succ = false;
        auto tmp = pipe_loaded_A2::read(succ);
        if (succ) schedSldA2 = tmp;

        if (t > 0) {
          pipe_done_A0::write(schedSstA);
          pipe_done_A1::write(schedSstA);
          pipe_done_A2::write(schedSstA);
        }
      }
    }

    schedSstA.t++;
    schedSstA.i = -1;
    pipe_done_A0::write(schedSstA);
    pipe_done_A1::write(schedSstA);
    pipe_done_A2::write(schedSstA);
  });

  /*
    Loads from B
  */
  auto sldB0 = q.single_task<class sldB0>([=]() [[intel::kernel_args_restrict]] {
    [[intel::fpga_register]] addr_val_t storeFifo[kStoreFifoSize];
    #pragma unroll
    for (int iC = 0; iC < kStoreFifoSize; ++iC) storeFifo[iC] = {-1, 0};

    for (int t = 0; t < timeSteps; t++) {
      polly_schedule_t schedSstB = pipe_done_B0::read();

      int i = 1;
      while (i < kN - 1) {
        polly_schedule_t schedSldB0 = {t, i};

        bool safe = (schedSldB0.t < schedSstB.t) |
                    (accessSldB0(schedSldB0) <= accessSstB(schedSstB)); 

        tagged_val_t reuse = {0, 0};
        #pragma unroll
        for (int iC = 0; iC < kStoreFifoSize; ++iC) {
          if (storeFifo[iC].addr == accessSldB0(schedSldB0))
            reuse = {storeFifo[iC].val, 1};
        }

        if (safe) {
          auto addrToLd =
              sycl::ext::intel::device_ptr<float>(B + accessSldB0(schedSldB0));
          auto ld = reuse.reuse ? reuse.val : BurstCoalescedLSU::load(addrToLd);

          pipe_B0::write(ld);
          i++;
        } 

        bool succ = false;
        auto tmp = pipe_done_B0::read(succ);
        if (succ) {
          schedSstB = tmp;
          #pragma unroll
          for (int iC = 0; iC < kStoreFifoSize - 1; ++iC)
            storeFifo[iC] = storeFifo[iC + 1];
          storeFifo[kStoreFifoSize - 1] = {accessSstB(schedSstB),
                                           schedSstB.val};
        }
      }
    }
  });

  auto sldB1 = q.single_task<class sldB1>([=]() [[intel::kernel_args_restrict]] {
    [[intel::fpga_register]] addr_val_t storeFifo[kStoreFifoSize];
    #pragma unroll
    for (int iC = 0; iC < kStoreFifoSize; ++iC) storeFifo[iC] = {-1, 0};

    for (int t = 0; t < timeSteps; t++) {
      polly_schedule_t schedSstB = pipe_done_B1::read();

      int i = 1;
      while (i < kN - 1) {
        polly_schedule_t schedSldB1 = {t, i};

        bool safe = (schedSldB1.t < schedSstB.t) |
                    (accessSldB1(schedSldB1) <= accessSstB(schedSstB)); 

        tagged_val_t reuse = {0, 0};
        #pragma unroll
        for (int iC = 0; iC < kStoreFifoSize; ++iC) {
          if (storeFifo[iC].addr == accessSldB1(schedSldB1))
            reuse = {storeFifo[iC].val, 1};
        }

        if (safe) {
          auto addrToLd =
              sycl::ext::intel::device_ptr<float>(B + accessSldB1(schedSldB1));
          auto ld = reuse.reuse ? reuse.val : BurstCoalescedLSU::load(addrToLd);
          pipe_B1::write(ld);
          i++;
        } 

        bool succ = false;
        auto tmp = pipe_done_B1::read(succ);
        if (succ) {
          schedSstB = tmp;
          #pragma unroll
          for (int iC = 0; iC < kStoreFifoSize - 1; ++iC)
            storeFifo[iC] = storeFifo[iC + 1];
          storeFifo[kStoreFifoSize - 1] = {accessSstB(schedSstB),
                                           schedSstB.val};
        }
      }
    }
  });

  auto sldB2 = q.single_task<class sldB2>([=]() [[intel::kernel_args_restrict]] {
    [[intel::fpga_register]] addr_val_t storeFifo[kStoreFifoSize];
    #pragma unroll
    for (int iC = 0; iC < kStoreFifoSize; ++iC) storeFifo[iC] = {-1, 0};

    for (int t = 0; t < timeSteps; t++) {
      polly_schedule_t schedSstB = pipe_done_B2::read();

      int i = 1;
      while (i < kN - 1) {
        polly_schedule_t schedSldB2 = {t, i};

        // The below doesnt work:
        //  (schedSldB2.t < schedSstB.t) & (accessSldB2(schedSldB2) <= accessSstB(schedSstB));
        // because sld2 loads from i+1, but sst only stores up to i.
        bool safe = (schedSldB2.t < schedSstB.t) |
                    (accessSldB2(schedSldB2) <= accessSstB(schedSstB)); 

        tagged_val_t reuse = {0, 0};
        #pragma unroll
        for (int iC = 0; iC < kStoreFifoSize; ++iC) {
          if (storeFifo[iC].addr == accessSldB2(schedSldB2))
            reuse = {storeFifo[iC].val, 1};
        }

        if (safe) {
          auto addrToLd =
              sycl::ext::intel::device_ptr<float>(B + accessSldB2(schedSldB2));
          auto ld = reuse.reuse ? reuse.val : BurstCoalescedLSU::load(addrToLd);
          pipe_B2::write(ld);
          i++;
        } 

        bool succ = false;
        auto tmp = pipe_done_B2::read(succ);
        if (succ) {
          schedSstB = tmp;
          #pragma unroll
          for (int iC = 0; iC < kStoreFifoSize - 1; ++iC)
            storeFifo[iC] = storeFifo[iC + 1];
          storeFifo[kStoreFifoSize - 1] = {accessSstB(schedSstB),
                                           schedSstB.val};
        }
      }
    }
  });


  /*
    Loads from A
  */
  auto sldA0 = q.single_task<class sldA0>([=]() [[intel::kernel_args_restrict]] {
    [[intel::fpga_register]] addr_val_t storeFifo[kStoreFifoSize];
    #pragma unroll
    for (int iC = 0; iC < kStoreFifoSize; ++iC) storeFifo[iC] = {-1, 0};

    polly_schedule_t schedSstA {0, 0};
    for (int t = 0; t < timeSteps; t++) {
      if (t >= 1)
        schedSstA = pipe_done_A0::read();

      int i = 1;
      while (i < kN - 1) {
        polly_schedule_t schedSldA0 = {t, i};

        bool safe = (schedSldA0.t <= schedSstA.t) |
                    (accessSldA0(schedSldA0) <= accessSstA(schedSstA)); 

        tagged_val_t reuse = {0, 0};
        #pragma unroll
        for (int iC = 0; iC < kStoreFifoSize; ++iC) {
          if (storeFifo[iC].addr == accessSldA0(schedSldA0))
            reuse = {storeFifo[iC].val, 1};
        }

        if (safe) {
          auto addrToLd =
              sycl::ext::intel::device_ptr<float>(A + accessSldA0(schedSldA0));
          auto ld = reuse.reuse ? reuse.val : BurstCoalescedLSU::load(addrToLd);

          pipe_A0::write(ld);
          i++;
        } 

        bool succ = false;
        auto tmp = pipe_done_A0::read(succ);
        if (succ) {
          schedSstA = tmp;
          #pragma unroll
          for (int iC = 0; iC < kStoreFifoSize - 1; ++iC)
            storeFifo[iC] = storeFifo[iC + 1];
          storeFifo[kStoreFifoSize - 1] = {accessSstA(schedSstA),
                                           schedSstA.val};
        }
      }
    }
  });

  auto sldA1 = q.single_task<class sldA1>([=]() [[intel::kernel_args_restrict]] {
    [[intel::fpga_register]] addr_val_t storeFifo[kStoreFifoSize];
    #pragma unroll
    for (int iC = 0; iC < kStoreFifoSize; ++iC) storeFifo[iC] = {-1, 0};

    polly_schedule_t schedSstA {0, 0};
    for (int t = 0; t < timeSteps; t++) {
      if (t >= 1)
        schedSstA = pipe_done_A1::read();
    
      int i = 1;
      while (i < kN - 1) {
        polly_schedule_t schedSldA1 = {t, i};

        bool safe = (schedSldA1.t <= schedSstA.t) |
                    (accessSldA1(schedSldA1) <= accessSstA(schedSstA)); 

        tagged_val_t reuse = {0, 0};
        #pragma unroll
        for (int iC = 0; iC < kStoreFifoSize; ++iC) {
          if (storeFifo[iC].addr == accessSldA1(schedSldA1))
            reuse = {storeFifo[iC].val, 1};
        }

        if (safe) {
          auto addrToLd =
              sycl::ext::intel::device_ptr<float>(A + accessSldA1(schedSldA1));
          auto ld = reuse.reuse ? reuse.val : BurstCoalescedLSU::load(addrToLd);
          pipe_A1::write(ld);
          i++;
        } 

        bool succ = false;
        auto tmp = pipe_done_A1::read(succ);
        if (succ) {
          schedSstA = tmp;
          #pragma unroll
          for (int iC = 0; iC < kStoreFifoSize - 1; ++iC)
            storeFifo[iC] = storeFifo[iC + 1];
          storeFifo[kStoreFifoSize - 1] = {accessSstA(schedSstA),
                                           schedSstA.val};
        }
      }
    }
  });

  auto sldA2 = q.single_task<class sldA2>([=]() [[intel::kernel_args_restrict]] {
    [[intel::fpga_register]] addr_val_t storeFifo[kStoreFifoSize];
    #pragma unroll
    for (int iC = 0; iC < kStoreFifoSize; ++iC) storeFifo[iC] = {-1, 0};

    polly_schedule_t schedSstA {0, 0};
    for (int t = 0; t < timeSteps; t++) {
      if (t >= 1)
        schedSstA = pipe_done_A2::read();

      int i = 1;
      while (i < kN - 1) {
        polly_schedule_t schedSldA2 = {t, i};

        // The below doesnt work:
        //  (schedSldA2.t < schedSstA.t) & (accessSldA2(schedSldA2) <= accessSstA(schedSstA));
        // because sld2 loads from i+1, but sst only stores up to i.
        bool safe = (schedSldA2.t <= schedSstA.t) |
                    (accessSldA2(schedSldA2) <= accessSstA(schedSstA)); 

        tagged_val_t reuse = {0, 0};
        #pragma unroll
        for (int iC = 0; iC < kStoreFifoSize; ++iC) {
          if (storeFifo[iC].addr == accessSldA2(schedSldA2))
            reuse = {storeFifo[iC].val, 1};
        }

        if (safe) {
          auto addrToLd =
              sycl::ext::intel::device_ptr<float>(A + accessSldA2(schedSldA2));
          auto ld = reuse.reuse ? reuse.val : BurstCoalescedLSU::load(addrToLd);
          pipe_A2::write(ld);
          
          if (i > 512) {
            // TODO: have fifo here?
            pipe_loaded_A2::write({t, i-512});
          }

          i++;
        } 

        bool succ = false;
        auto tmp = pipe_done_A2::read(succ);
        if (succ) {
          schedSstA = tmp;
          #pragma unroll
          for (int iC = 0; iC < kStoreFifoSize - 1; ++iC)
            storeFifo[iC] = storeFifo[iC + 1];
          storeFifo[kStoreFifoSize - 1] = {accessSstA(schedSstA),
                                           schedSstA.val};
        }
      }
    }

    pipe_loaded_A2::write({timeSteps, kN-2});
  });



  event.wait();
  event2.wait();

  sstB.wait();
  sstA.wait();

  sldB0.wait();
  sldB1.wait();
  sldB2.wait();

  sldA0.wait();
  sldA1.wait();
  sldA2.wait();

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
