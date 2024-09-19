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

double gsum_sort_kernel(queue &q, std::vector<int> &h_A, std::vector<int> &h_x,
                        std::vector<int> &h_y) {
  const int kN = h_A.size();
  int *A = fpga_tools::toDevice(h_A, q);
  int *x = fpga_tools::toDevice(h_x, q);
  int *y = fpga_tools::toDevice(h_y, q);

  std::vector<std::pair<sycl::event, bool>> events; // {event, measureTimeBool} pairs

  auto main_event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {

    for (int i = 0; i < kN; i++) { 
      A[i] = x[i] + y[i];
    }

    // From: https://en.wikipedia.org/wiki/Bitonic_sorter
    // given an array arr of length n, this code sorts it in place
    // all indices run from 0 to n-1
    for (int k = 2; k <= kN; k <<= 1) { // k is doubled every iteration
      for (int j = k >> 1; j > 0; j >>= 1) { // j is halved at every iteration
        for (int i = 0; i < kN; i++) {
          // Both versions work, but the below stresses our speculation more.
          // int l = i ^ j;
          // int Ai = A[i];
          // int Al = A[l];
          // bool swap = (l > i) &&
          //             (((i & k) == 0 && Ai > Al) || ((i & k) != 0 && Ai < Al));

          // if (swap) {
          //   A[i] = Al;
          //   A[l] = Ai;
          // }          

          int l = i ^ j; 
          if (l > i) {
            int Ai = A[i];
            int Al = A[l];

            bool swap = ((i & k) == 0 && Ai > Al) || ((i & k) != 0 && Ai < Al);

            if (swap) {
              A[i] = Al;
              A[l] = Ai;
            }
          }
        }
      }
    }
  });

  
  events.push_back({main_event, true});
  for (auto &kv : events) kv.first.wait();
  
  q.copy(A, h_A.data(), h_A.size()).wait();

  sycl::free(A, q);

  double max_event_time = 0;
  for (auto &[e, toMeasure] : events) {
    if (toMeasure) {
      auto start = e.get_profiling_info<info::event_profiling::command_start>();
      auto end = e.get_profiling_info<info::event_profiling::command_end>();
      double this_event_time = static_cast<double>(end - start) / 1000000;
      max_event_time = std::max(max_event_time, this_event_time);
    }
  }

  return max_event_time;
}

int main(int argc, char *argv[]) {
  int N = 64;
  int PERCENTAGE = 0;
  try {
    if (argc > 1) {
      N = int(atoi(argv[1]));
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

    std::vector<int> arr(N);
    std::vector<int> arr_cpu(N);
    std::vector<int> x(N, 1);
    std::vector<int> y(N, 2);
    
    for (size_t i=0; i<N; ++i) {
      x[i] = rand();
      y[i] = rand();
    }

    std::copy_n(arr.begin(), N, arr_cpu.begin());

    auto kernel_time = gsum_sort_kernel(q, arr, x, y);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    for (size_t i=0; i<N; ++i) arr_cpu[i] = x[i] + y[i];
    std::sort(arr_cpu.begin(), arr_cpu.end());
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
