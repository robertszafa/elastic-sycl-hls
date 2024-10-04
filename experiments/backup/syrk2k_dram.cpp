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

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double syrk2k_kernel(queue &q, std::vector<float> &h_A, std::vector<float> &h_B,
                   std::vector<float> &h_C, const float alpha, const float beta,
                   const int N, const int M) {
  auto *A = fpga_tools::toDevice(h_A, q);
  auto *B = fpga_tools::toDevice(h_B, q);
  auto *C = fpga_tools::toDevice(h_C, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    // BLAS PARAMS
    // UPLO  = 'L'
    // TRANS = 'N'
    // A is NxM
    // B is NxM
    // C is NxN
    for (int i = 0; i < N; i++) {
      for (int j = 0; j <= i; j++) {
        C[i * N + j] *= beta;
      }

      for (int k = 0; k < M; k++) {
        for (int j = 0; j <= i; j++) {
          C[i * N + j] += A[j * N + k] * alpha * B[i * N + k] +
                          B[j * N + k] * alpha * A[i * N + k];
        }
      }
    }
  });

  event.wait();
  q.copy(C, h_C.data(), h_C.size()).wait();

  sycl::free(A, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void syrk2k_cpu(std::vector<float> &A, std::vector<float> &B,
                std::vector<float> &C, const float alpha, const float beta,
                const int N, const int M) {
  // BLAS PARAMS
  // UPLO  = 'L'
  // TRANS = 'N'
  // A is NxM
  // B is NxM
  // C is NxN
  for (int i = 0; i < N; i++) {
    for (int j = 0; j <= i; j++) {
      C[i * N + j] *= beta;
    }

    for (int k = 0; k < M; k++) {
      for (int j = 0; j <= i; j++) {
        C[i * N + j] += A[j * N + k] * alpha * B[i * N + k] +
                        B[j * N + k] * alpha * A[i * N + k];
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

    auto kernel_time = syrk2k_kernel(q, A, B, C, 1.0f, 1.0f, N, N);
    syrk2k_cpu(A_cpu, B_cpu, C_cpu, 1.0f, 1.0f, N, N);

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