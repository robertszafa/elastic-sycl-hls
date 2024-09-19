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
#include "device_print.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double gemm_kernel(queue &q, std::vector<float> &h_A, std::vector<float> &h_B,
                   std::vector<float> &h_C, const float alpha, const float beta,
                   const uint NI, const uint NJ, const uint NK) {
  auto *A = fpga_tools::toDevice(h_A, q);
  auto *B = fpga_tools::toDevice(h_B, q);
  auto *C = fpga_tools::toDevice(h_C, q);

  std::vector<sycl::event> events;

  auto main_event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    // BLAS PARAMS
    // TRANSA = 'N'
    // TRANSB = 'N'
    //  => Form C := alpha*A*B + beta*C,
    // A is NIxNK
    // B is NKxNJ
    // C is NIxNJ
    //
    // 19k cycles for a 10x10 array
    
    for (uint i = 0; i < NI; i++) {
      for (uint j = 0; j < NJ; j++) {
        auto ld0_val = C[i * NI + j];
        auto st0_val = ld0_val + beta;
        C[i * NI + j] = st0_val;
      }

      for (uint k = 0; k < NK; k++) {
        for (uint j = 0; j < NJ; j++) {
          auto ld1_val = C[i * NI + j];
          auto st1_val = ld1_val + (alpha * A[i * NI + k] * B[k * NK + j]);
          C[i * NI + j] = st1_val;
        }
      }
    }

    // Below is a manually fused version. 16k cycles.
    /*
    for (int i = 0; i < NI; i++) { 
      for (int k = 0; k < NK; k++) {
        for (int j = 0; j < NJ; j++) {
          auto v = C[i * NI + j]; 
          if (k == 0)
            v += beta;
          v += alpha * A[i * NI + k] * B[k * NK + j];
          C[i * NI + j] = v;
        }
      }
    }
    
    */
  });

  
  events.push_back(main_event);
  sycl::event::wait(events);
  q.copy(C, h_C.data(), h_C.size()).wait();

  sycl::free(A, q);

  double max_event_time = 0;
  for (auto &e : events) {
    auto start = e.get_profiling_info<info::event_profiling::command_start>();
    auto end = e.get_profiling_info<info::event_profiling::command_end>();
    double this_event_time = static_cast<double>(end - start) / 1000000;
    max_event_time = std::max(max_event_time, this_event_time);
  }

  return max_event_time;
}

void gemm_cpu(std::vector<float> &A, std::vector<float> &B,
              std::vector<float> &C, const float alpha, const float beta,
              const int NI, const int NJ, const int NK) {
  // BLAS PARAMS
  // TRANSA = 'N'
  // TRANSB = 'N'
  //  => Form C := alpha*A*B + beta*C,
  // A is NIxNK
  // B is NKxNJ
  // C is NIxNJ
  for (int i = 0; i < NI; i++) {
    for (int j = 0; j < NJ; j++) {
      C[i * NI + j] += beta;
    }

    for (int k = 0; k < NK; k++) {
      for (int j = 0; j < NJ; j++) {
        C[i * NI + j] += alpha * A[i * NI + k] * B[k * NK + j];
      }
    }
  }
}

int main(int argc, char *argv[]) {
  int N = 10;
  try {
    if (argc > 1) {
      N = int(atoi(argv[1]));
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

    std::vector<float> A(N*N, 1);
    std::vector<float> A_cpu(N*N, 1);
    std::vector<float> B(N*N, 1);
    std::vector<float> B_cpu(N*N, 1);
    std::vector<float> C(N*N, 1);
    std::vector<float> C_cpu(N*N, 1);

    auto kernel_time = gemm_kernel(q, A, B, C, 1.0f, 1.0f, N, N, N);
    gemm_cpu(A_cpu, B_cpu, C_cpu, 1.0f, 1.0f, N, N, N);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(C.begin(), C.end(), C_cpu.begin()))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
