#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/sycl.hpp>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <vector>

#include "memory_utils.hpp"
#include "exception_handler.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

constexpr int kN = 100;

double sort_kernel(queue &q, std::vector<int> &h_A) {
  const int N = h_A.size();
  int *A_dram = fpga_tools::toDevice(h_A, q);

  auto event = q.submit([&](sycl::handler &h) {
    h.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
      int A[kN];

      #ifdef TEST
      for (i = 0; i < N ; i++) 
        A[i] = A_dram[i];
      #endif

      int i, j;
      for (i = 0; i < N - 1; i++) {
        for (j = 0; j < N - i - 1; j++) {
          auto left = A[j];
          auto right = A[j + 1];
          if (left >= right) {
            A[j] = right;
            A[j + 1] = left;
          }
        }
      }

      #ifdef TEST
      for (i = 0; i < N ; i++) 
        A_dram[i] = A[i];
      #endif

    });
  });

  event.wait();
  
  q.copy(A_dram, h_A.data(), h_A.size()).wait();

  sycl::free(A_dram, q);

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
  std::cout << "SIMULATION:\n";
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
    auto device = q.get_device();

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << device.get_info<sycl::info::device::name>().c_str() << "\n";

    std::vector<int> arr(ARRAY_SIZE);
    std::vector<int> arr_cpu(ARRAY_SIZE);
    
    for (size_t i=0; i<kN; ++i) {
      if (PERCENTAGE == 100)
        arr[i] = kN - i;
      else if (PERCENTAGE == 0)
        arr[i] = i;
      else
        arr[i] = rand();
    }

    std::copy_n(arr.begin(), kN, arr_cpu.begin());

    auto kernel_time = sort_kernel(q, arr);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(arr.begin(), arr.end(), arr_cpu.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failed\n";
    }

  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
