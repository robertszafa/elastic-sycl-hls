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
#include "device_print.hpp"

#include "pipe_utils.hpp"
#include "unrolled_loop.hpp"


using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double test_kernel_war(queue &q, const int NUM_ITERS, const int NI, const int NJ, const int NK,
                       std::vector<int> &h_D) {

  int *D = fpga_tools::toDevice(h_D, q);

  std::vector zeroVec(h_D.size(), 0);
  int* tmp = fpga_tools::toDevice(zeroVec, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
  for (int iters = 0; iters < NUM_ITERS; iters++) {
    for (int j = 0; j < NI; j++) {
      for (int i = 0; i < NI; i++) {
        D[i] = tmp[i] + j;
      }
    }

    for (int i = 10; i < NI; i++) {
      tmp[i] = iters;
    }
  }
  });

  event.wait();
  
  q.copy(D, h_D.data(), h_D.size()).wait();

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void test_kernel_cpu(const int NUM_ITERS, const int NI, const int NJ, const int NK,
                     std::vector<int> &D) {
  std::vector tmp(D.size(), 0);

  for (int iters = 0; iters < NUM_ITERS; iters++) {

    for (int j = 0; j < NI; j++) {
      for (int i = 0; i < NI; i++) {
        D[i] = tmp[i] + j;
      }
    }

    for (int i = 10; i < NI; i++) {
      tmp[i] = iters;
    }
  }
}

int main(int argc, char *argv[]) {
  int N = 10;
  int NUM_ITERS = 1;
  try {
    if (argc > 1)
      N = int(atoi(argv[1]));
    if (argc > 1)
      NUM_ITERS = int(atoi(argv[2]));
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

    const int NI = N;
    const int NJ = N;
    const int NK = N;
    const int S = N;

    std::vector<int> D(S, 4); 
    std::vector<int> D_cpu(S, 4); 

    auto kernel_time = test_kernel_war(q, NUM_ITERS, NI, NJ, NK, D);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    test_kernel_cpu(NUM_ITERS, NI, NJ, NK, D_cpu);

    if (std::equal(D.begin(), D.end(), D_cpu.begin()))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
    
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
