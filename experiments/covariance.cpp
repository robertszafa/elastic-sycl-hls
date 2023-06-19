#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <vector>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "memory_utils.hpp"
#include "exception_handler.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

constexpr int N = 32;

// No need to test if result correct since this kernel will not be transformed,
// because the static scheduler achieves II=1.
double covariance_kernel(queue &q) {
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



int main(int argc, char *argv[]) {
  int ARRAY_SIZE = 10;
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
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    auto kernel_time = kernel_2mm(q);


    std::cout << "\nKernel time (ms): " << kernel_time << "\n";
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
