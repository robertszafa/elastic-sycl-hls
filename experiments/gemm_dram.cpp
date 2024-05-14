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
                   const int NI, const int NJ, const int NK) {
  auto *A = fpga_tools::toDevice(h_A, q);
  auto *B = fpga_tools::toDevice(h_B, q);
  auto *C = fpga_tools::toDevice(h_C, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    // BLAS PARAMS
    // TRANSA = 'N'
    // TRANSB = 'N'
    //  => Form C := alpha*A*B + beta*C,
    // A is NIxNK
    // B is NKxNJ
    // C is NIxNJ
    //
    // 19k cycles for a 10x10 array
    uint tag = 0;
    for (int i = 0; i < NI; i++) {
      for (int j = 0; j < NJ; j++) {
        auto ld0_addr = i * NI + j;
        auto ld0_tag = tag;
        auto ld0_val = C[i * NI + j];
        tag++;
        auto st0_val = ld0_val + beta;
        auto st0_tag = tag;
        auto st0_addr = i*NI + j;
        // PRINTF("ld0 (%d, %d) = %f\nst0 (%d, %d) = %f\n", 
        //        ld0_addr, ld0_tag, ld0_val, st0_addr, st0_tag, st0_val);
        C[i * NI + j] = st0_val;
      }

      for (int k = 0; k < NK; k++) {
        for (int j = 0; j < NJ; j++) {
          auto ld1_val = C[i * NI + j];
          auto ld1_addr = i * NI + j;
          auto ld1_tag = tag;
          tag++;
          auto st1_val = ld1_val + (alpha * A[i * NI + k] * B[k * NK + j]);
          auto st1_addr = i * NI + j;
          auto st1_tag = tag;

        PRINTF("ld1 (%d, %d) = %f\nst1 (%d, %d) = %f\n", 
               ld1_addr, ld1_tag, ld1_val, st1_addr, st1_tag, st1_val);

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

  event.wait();
  q.copy(C, h_C.data(), h_C.size()).wait();

  sycl::free(A, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
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
