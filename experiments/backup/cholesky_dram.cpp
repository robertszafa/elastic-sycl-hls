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

double cholesky_kernel(queue &q, std::vector<float> &h_A, const int N) {
  auto *A = fpga_tools::toDevice(h_A, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < N; i++) {
      
      // L00:
      for (int j = 0; j < i; j++) {

        auto a = A[i * N + j]; // ld0

        // L000:
        for (int k = 0; k < j; k++) {
          a -= A[i * N + k] * A[j * N + k]; // ld1, ld2
        }

        A[i * N + j] = a / A[j * N + j]; // ld3, st0
      }

      auto a = A[i * N + i]; // ld4

      // L01:
      for (int k = 0; k < i; k++) {
        a -= A[i * N + k] * A[i * N + k]; // ld5
      }

      A[i * N + i] = a; // st1
    }
  });

  event.wait();
  q.copy(A, h_A.data(), h_A.size()).wait();

  sycl::free(A, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void cholesky_cpu(std::vector<float> &A, const int N) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < i; j++) {
      auto a = A[i * N + j];
      for (int k = 0; k < j; k++) {
        a -= A[i * N + k] * A[j * N + k];
      }
      A[i * N + j] = a / A[j * N + j];
    }

    auto a = A[i * N + i];
    for (int k = 0; k < i; k++) {
      a -= A[i * N + k] * A[i * N + k];
    }
    A[i * N + i] = a; // sycl::sqrt(a)
  }
}

inline bool almost_equal(const float x, const float y) {
  float ulpFloat = static_cast<float>(2);
  return fabsf(x - y) <=
             std::numeric_limits<float>::epsilon() * fabsf(x + y) * ulpFloat ||
         fabsf(x - y) < std::numeric_limits<float>::min();
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

    std::vector<float> A(N*N);
    std::vector<float> A_cpu(N*N);
    std::iota(A.begin(), A.end(), 1);
    std::iota(A_cpu.begin(), A_cpu.end(), 1);

    auto kernel_time = cholesky_kernel(q, A, N);
    cholesky_cpu(A_cpu, N);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(A.begin(), A.end(), A_cpu.begin(), almost_equal))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}