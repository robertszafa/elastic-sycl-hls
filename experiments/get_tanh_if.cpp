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
using namespace fpga_tools;

// Forward declare kernel name.
class MainKernel;

double get_tanh_if_kernel(queue &q, std::vector<int> &h_A) {
  int* A = fpga_tools::toDevice(h_A, q);
  const int N = h_A.size();

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int result;
    for (int i = 0; i < N; i++) {
      int beta = A[i];

      if (beta >= 20480) 
        result = 4096;
      else 
        result = ((result * result + 19.52381) * result * result + 3.704762) * result;

      A[i] = result;
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

void get_tanh_if_cpu(std::vector<int> &A) {
  int result;
  for (int i = 0; i < A.size(); i++) {
    int beta = A[i];

    if (beta >= 20480)
      result = 4096;
    else
      result =
          ((result * result + 19.52381) * result * result + 3.704762) * result;

    A[i] = result;
  }
}


void init_data(std::vector<int> &idxs, const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind (distribution, generator);

  for (int i = 0; i < idxs.size(); i++) {
    idxs[i] = (dice() < percentage) ? 1 : 30000;
  }
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

    std::vector<int> A, A_cpu;
    init_data(A, PERCENTAGE);
    init_data(A_cpu, PERCENTAGE);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    auto kernel_time = get_tanh_if_kernel(q, A);

    get_tanh_if_cpu(A_cpu);
    if (std::equal(A_cpu.begin(), A_cpu.end(), A.begin()))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

