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

constexpr int N = 100;

// Forward declare kernel name.
class MainKernel;

double sort_kernel(queue & q, std::vector<int> &h_arr) {

  int *arr = fpga_tools::toDevice(h_arr, q);
  const int n = h_arr.size();

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int i, j;
    for (i = 0; i < n - 1; i++) {
      for (j = 0; j < n - i - 1; j++) {
        auto left = arr[j];
        auto right = arr[j + 1];
        if (left > right) {
          arr[j] = right;
          arr[j + 1] = left;
        }
      }
    }
  });

  event.wait();
  // q.copy(arr, h_arr.data(), h_arr.size()).wait();

  // sycl::free(arr, q);


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

    std::vector<int> arr(ARRAY_SIZE);

    for (size_t i=0; i<ARRAY_SIZE; ++i) {
      if (PERCENTAGE == 100)
        arr[i] = ARRAY_SIZE - i;
      else if (PERCENTAGE == 0)
        arr[i] = i;
      else
        arr[i] = rand();
    }

    std::vector<int> arr_cpu(ARRAY_SIZE);
    std::copy_n(arr.data(), ARRAY_SIZE, arr_cpu.data());
    std::sort(arr_cpu.begin(), arr_cpu.end());

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

