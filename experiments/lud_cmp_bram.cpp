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

constexpr int kN = 10;

double lud_cmp_kernel(queue &q, std::vector<int> &h_A) {

  int *A = fpga_tools::toDevice(h_A, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int y[kN], x[kN], b[kN];

    for (int i = 0; i < kN; i++) {

      for (int j = 0; j < i; j++) {
        auto w = A[i * kN + j];
        for (int k = 0; k < j; k++) {
          w -= A[i * kN + k] * A[k * kN + j];
        }
        A[i * kN + j] = w / A[j * kN + j];
      }

      for (int j = i; j < kN; j++) {
        auto w = A[i * kN + j];
        for (int k = 0; k < i; k++) {
          w -= A[i * kN + k] * A[k * kN + j];
        }
        A[i * kN + j] = w;
      }

    }

    for (int i = 0; i < kN; i++) {
      auto w = b[i];
      for (int j = 0; j < i; j++)
        w -= A[i * kN + j] * y[j];
      y[i] = w;
    }

    for (int i = kN - 1; i >= 0; i--) {
      auto w = y[i];
      for (int j = i + 1; j < kN; j++)
        w -= A[i * kN + j] * x[j];
      x[i] = w / A[i * kN + i];
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


int main(int argc, char *argv[]) {
  int ARRAY_SIZE = 1000;
  int PERCENTAGE = 0;
  try {
    if (argc > 1) {
      ARRAY_SIZE = int(atoi(argv[1]));
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

    std::vector<int> A(ARRAY_SIZE, 1);

    auto kernel_time = lud_cmp_kernel(q, A);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

