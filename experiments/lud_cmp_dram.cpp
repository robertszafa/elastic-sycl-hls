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

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double lud_cmp_kernel(queue &q, std::vector<int> &h_A, std::vector<int> &h_y,
                      std::vector<int> &h_x, std::vector<int> &h_b,
                      const int N) {

  int *A = fpga_tools::toDevice(h_A, q);
  int *y = fpga_tools::toDevice(h_y, q);
  int *x = fpga_tools::toDevice(h_x, q);
  int *b = fpga_tools::toDevice(h_b, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < i; j++) {
        auto w = A[i * N + j];
        for (int k = 0; k < j; k++) {
          w -= A[i * N + k] * A[k * N + j];
        }
        A[i * N + j] = w / A[j * N + j];
      }

      for (int j = i; j < N; j++) {
        auto w = A[i * N + j];
        for (int k = 0; k < i; k++) {
          w -= A[i * N + k] * A[k * N + j];
        }
        A[i * N + j] = w;
      }
    }

    for (int i = 0; i < N; i++) {
      auto w = b[i];
      for (int j = 0; j < i; j++)
        w -= A[i * N + j] * y[j];
      y[i] = w;
    }

    for (int i = 0; i < N; i++) {
      auto w = y[i];
      for (int j = i + 1; j < N; j++)
        w -= A[i * N + j] * x[j];
      x[i] = w / A[i * N + i];
    }
  });

  event.wait();
  q.copy(x, h_x.data(), h_x.size()).wait();

  sycl::free(A, q);
  sycl::free(y, q);
  sycl::free(x, q);
  sycl::free(b, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void lud_cmp_cpu(std::vector<int> &A, std::vector<int> &y, std::vector<int> &x,
                 std::vector<int> &b, const int N) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < i; j++) {
      auto w = A[i * N + j];
      for (int k = 0; k < j; k++) {
        w -= A[i * N + k] * A[k * N + j];
      }
      A[i * N + j] = w / A[j * N + j];
    }

    for (int j = i; j < N; j++) {
      auto w = A[i * N + j];
      for (int k = 0; k < i; k++) {
        w -= A[i * N + k] * A[k * N + j];
      }
      A[i * N + j] = w;
    }
  }

  for (int i = 0; i < N; i++) {
    auto w = b[i];
    for (int j = 0; j < i; j++)
      w -= A[i * N + j] * y[j];
    y[i] = w;
  }

  for (int i = N - 1; i >= 0; i--) {
    auto w = y[i];
    for (int j = i + 1; j < N; j++)
      w -= A[i * N + j] * x[j];
    x[i] = w / A[i * N + i];
  }
}

int main(int argc, char *argv[]) {
  int N = 1000;
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
#else  // #if FPGA_EMULATOR
  auto d_selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif
  try {
    // Enable profiling.
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: " << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<int> A(N*N, 1), A_cpu(N*N, 1);
    std::vector<int> y(N*N, 1), y_cpu(N*N, 1);
    std::vector<int> x(N*N, 1), x_cpu(N*N, 1);
    std::vector<int> b(N*N, 1), b_cpu(N*N, 1);

    auto kernel_time = lud_cmp_kernel(q, A, y, x, b, N*N);
    lud_cmp_cpu(A_cpu, y_cpu, x_cpu, b_cpu, N);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(x.begin(), x.end(), x_cpu.begin()))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
    
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

