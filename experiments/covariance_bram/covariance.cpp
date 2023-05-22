#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <vector>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "memory_utils.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };

constexpr int N = 32;

double kernel_fpga(queue &q) {
  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    float data[N*N], cov[N*N];

    for (int j = 0; j < N; j++) {
      float m = 0.0f;
      // #pragma unroll
      for (int i = 0; i < N; i++)
        m += data[i*N + j];
      m /= 0.73f;
      // #pragma unroll
      for (int i = 0; i < N; i++)
        data[i*N + j] -= m;
    }

    for (int i = 0; i < N; i++) {
      for (int j = i; j < N; j++) {
        float c = 0.0f;
        // #pragma unroll
        for (int k = 0; k < N; k++)
          c += data[k*N + i] * data[k*N + j];
        c /= -0.27f;
        cov[i*N + j] = c;
        cov[j*N + i] = c;
      }
    }

    data[0] = cov[0];
  });

  event.wait();

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

// Create an exception handler for asynchronous SYCL exceptions
static auto exception_handler = [](sycl::exception_list e_list) {
  for (std::exception_ptr const &e : e_list) {
    try {
      std::rethrow_exception(e);
    } catch (std::exception const &e) {
#if _DEBUG
      std::cout << "Failure" << std::endl;
#endif
      std::terminate();
    }
  }
};

int main(int argc, char *argv[]) {
  // Get A_SIZE and forward/no-forward from args.
  int ARRAY_SIZE = 1000;
  auto DATA_DISTR = data_distribution::ALL_WAIT;
  int PERCENTAGE = 5;
  try {
    if (argc > 1) {
      ARRAY_SIZE = int(atoi(argv[1]));
    }
    if (argc > 2) {
      DATA_DISTR = data_distribution(atoi(argv[2]));
    }
    if (argc > 3) {
      PERCENTAGE = int(atoi(argv[3]));
      std::cout << "Percentage is " << PERCENTAGE << "\n";
      if (PERCENTAGE < 0 || PERCENTAGE > 100)
        throw std::invalid_argument("Invalid percentage.");
    }
  } catch (exception const &e) {
    std::cout << "Incorrect argv.\nUsage:\n";
    std::cout << "  ./executable [ARRAY_SIZE] [data_distribution (0/1/2)] "
                 "[PERCENTAGE (only for "
                 "data_distr 2)]\n";
    std::cout << "    0 - all_wait, 1 - no_wait, 2 - PERCENTAGE wait\n";
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
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = kernel_fpga(q);

    // Wait for all work to finish.
    q.wait();

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
