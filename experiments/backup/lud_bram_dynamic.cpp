#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <vector>
#include <random>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "memory_utils.hpp"
#include "exception_handler.hpp"
#include "pipe_utils.hpp"
#include "unrolled_loop.hpp"

// #include "ArbiterBRAM.hpp"
// #include "ArbiterBRAM_1.hpp"
// #include "MultiPortedBRAM.hpp"
#include "ArbiterBRAM_MUX.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

constexpr int kN = 10;

#define TEST 1

double lud_cmp_kernel(queue &q, std::vector<float> &h_A) {

  float *dramA = fpga_tools::toDevice(h_A, q);

  using loop1_pred = pipe<class loop1_pred_, bool, 32>;
  using loop2_pred = pipe<class loop2_pred_, bool, 32>;
  using loop1_i = pipe<class loop1_i_, int, 32>;
  using loop2_i = pipe<class loop2_i_, int, 32>;

  constexpr int NUM_LDS = 5;
  using end_signal = pipe<class end_signal_, bool, 1>;
  using pipe_load_addr = PipeArray<class rd_addr_, int, 32, NUM_LDS>;
  using pipe_load_val = PipeArray<class rd_data_, float, 32, NUM_LDS>;
  using pipe_store = PipeArray<class wr_addr_, AddrAndData<float>, 32, 2>;


  auto memArb = ArbiterBRAM<float, pipe_load_addr, pipe_load_val, pipe_store, end_signal, kN*kN, 5, 2>(q);

  using pred_l2 = pipe<class pred_l2_, int, 32>;
  using pred_l2_after_l1 = pipe<class pred_l2_after_l1_, int, 32>;

  using pred_AGU_1 = pipe<class pred_AGU_1_, int, 32>;
  auto addressGen1 = q.single_task<class AGU1>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < kN; i++) {
      pred_AGU_1::read();
      for (int j = 0; j < i; j++) {
        pipe_load_addr::PipeAt<0>::write(i * kN + j);
        for (int k = 0; k < j; k++) {
          pipe_load_addr::PipeAt<0>::write(i * kN + k);
          pipe_load_addr::PipeAt<1>::write(k * kN + j);
        }
        pipe_load_addr::PipeAt<1>::write(j * kN + j);
      }
    }
  });
  
  using pred_AGU_2 = pipe<class pred_AGU_2_, int, 32>;
  auto addressGen2 = q.single_task<class AGU2>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < kN; i++) {
      pred_AGU_2::read();
      pred_l2::read();
      for (int l = i; l < kN; l++) {
        pipe_load_addr::PipeAt<2>::write(i * kN + l);
        for (int m = 0; m < i; m++) {
          // if (l == i)
          //   pred_l2::read();

          pipe_load_addr::PipeAt<3>::write(i * kN + m);
          pipe_load_addr::PipeAt<2>::write(m * kN + l);
        }
      }
    }
  });

  auto event1 = q.single_task<class MainKernel0>([=]() [[intel::kernel_args_restrict]] {
    while(loop1_pred::read()) {
      int i = loop1_i::read();

      for (int j = 0; j < i; j++) {
        auto w = pipe_load_val::PipeAt<0>::read();
        for (int k = 0; k < j; k++) {
          w -= pipe_load_val::PipeAt<0>::read() * pipe_load_val::PipeAt<1>::read();
        }

        auto newA = w / pipe_load_val::PipeAt<1>::read();
        pipe_store::PipeAt<0>::write({i * kN + j, newA});
        // pred_l2::write(1);
      }
      pred_l2::write(1);
    }
  });

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    #if TEST
    for (int i = 0; i < kN*kN; i++) 
      pipe_store::PipeAt<1>::write({i, dramA[i]});
    #endif

    [[intel::disable_loop_pipelining]]
    for (int i = 0; i < kN; i++) {
      pred_AGU_1::write(1);
      loop1_pred::write(1);
      loop1_i::write(i);
      // pred_l2::read();
      // for (int j = 0; j < i; j++) {
      //   pipe_load_addr::PipeAt<0>::write(i * kN + j);
      //   auto w = pipe_load_val::PipeAt<0>::read();

      //   for (int k = 0; k < j; k++) {
      //     pipe_load_addr::PipeAt<0>::write(i * kN + k);
      //     auto A1 = pipe_load_val::PipeAt<0>::read();

      //     pipe_load_addr::PipeAt<1>::write(k * kN + j);
      //     auto A2 = pipe_load_val::PipeAt<1>::read();

      //     w -= A1 * A2;
      //   }

      //   pipe_load_addr::PipeAt<1>::write(j * kN + j);
      //   auto newA = w / pipe_load_val::PipeAt<1>::read();

      //   pipe_store::PipeAt<0>::write({i * kN + j, newA});
      // }


      pred_AGU_2::write(1);
      for (int l = i; l < kN; l++) {
        // pipe_load_addr::PipeAt<2>::write(i * kN + l);
        auto w = pipe_load_val::PipeAt<2>::read();

        for (int m = 0; m < i; m++) {
          // pipe_load_addr::PipeAt<2>::write(i * kN + m);
          auto A1 = pipe_load_val::PipeAt<2>::read();

          // pipe_load_addr::PipeAt<3>::write(m * kN + l);
          auto A2 = pipe_load_val::PipeAt<3>::read();

          w -= A1 * A2;
        }
        pipe_store::PipeAt<1>::write({i * kN + l, w});
      }
    }
    loop1_pred::write(0);

    #if TEST
    for (int i = 0; i < kN*kN; i++) {
      pipe_load_addr::PipeAt<4>::write(i);
      dramA[i] = pipe_load_val::PipeAt<4>::read();
    }
    #endif

    end_signal::write(1);
  });

  event.wait();
  // event1.wait();
  memArb.wait();
  // addressGen1.wait();
  // addressGen2.wait();

  q.copy(dramA, h_A.data(), h_A.size()).wait();

  sycl::free(dramA, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void lud_cmp_cpu(std::vector<float> &A) {
  for (int i = 0; i < kN; i++) {

    for (int j = 0; j < i; j++) {
      auto w = A[i * kN + j];
      for (int k = 0; k < j; k++) {
        w -= A[i * kN + k] * A[k * kN + j];
      }
      A[i * kN + j] = w / A[j * kN + j];
    }

    for (int l = i; l < kN; l++) {
      auto w = A[i * kN + l];
      for (int m = 0; m < i; m++) {
        w -= A[i * kN + m] * A[m * kN + l];
      }
      A[i * kN + l] = w;
    }

  }
}

inline bool almost_equal(const float x, const float y) {
  const float ulpFloat = static_cast<float>(2);
  const float tolerance = 0.0001f;
  return fabsf(x - y) <=
             tolerance * fabsf(x + y) * ulpFloat ||
         fabsf(x - y) < std::numeric_limits<float>::min();
}

int main(int argc, char *argv[]) {
  int N = kN;
  int PERCENTAGE = 0;
  try {
    if (argc > 1) {
      N = int(atoi(argv[1]));
    }
    if (argc > 2) {
      PERCENTAGE = int(atoi(argv[2]));
      
      if (PERCENTAGE < 0 || PERCENTAGE > 100)
        throw std::invalid_argument("Invalid percentage.");
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
#else  // #if FPGA_EMULATOR
  auto d_selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif
  try {
    // Enable profiling.
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: " << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<float> A(N*N, 2.0f);
    std::vector<float> A_cpu(N*N, 2.0f);
    // std::iota(A.begin(), A.end(), 1.0f);
    // std::iota(A_cpu.begin(), A_cpu.end(), 1.0f);
    for (size_t i=0; i<kN*kN; ++i) {
      A[i] = float(rand() % 100);
      A_cpu[i] = A[i];
    }
    

    auto kernel_time = lud_cmp_kernel(q, A);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    lud_cmp_cpu(A_cpu);

    #if TEST
    if (std::equal(A.begin(), A.end(), A_cpu.begin(), almost_equal))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
    
    for (size_t i=0; i<kN*kN; ++i) {
      if (!almost_equal(A[i],A_cpu[i]))
        std::cout << i << ": " << A[i] << " != " << A_cpu[i] << "\n";
    }
    #endif
    
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

