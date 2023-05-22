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

constexpr int N = 1000;

double kernel_fpga(queue &q, const std::vector<int> h_idx) {
  int* idx = fpga_tools::toDevice(h_idx, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int A[N];

    int atanh[12] = {0x08C9, 0x0416, 0x0202, 0x0100, 0x0080, 0x0064,
                     0x0032, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001};
    int cosh[5] = {0x1000, 0x18B0, 0x3C31, 0xA115, 0x1B4EE};
    int sinh[5] = {0x0, 0x12CD, 0x3A07, 0xA049, 0x1B4A3};

    for (int i = 0; i < N; i++) {
      // Input angle
      // Result of tanh, sinh and cosh
      // int result = 4096; // Saturation effect
      int beta = A[idx[i]];

      // Implement approximate range of the hyperbolic CORDIC block
      beta = ((beta * beta + 19.52381) * beta * beta + 3.704762) * beta;

      // Central symmetry correction
      A[idx[i]] = beta;
    }
  });

  event.wait();

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<int> &idxs, const data_distribution distr,
               const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind (distribution, generator);

  for (int i = 0; i < idxs.size(); i++) {
    if (distr == data_distribution::ALL_WAIT) 
      idxs[i] = 1;
    else if (distr == data_distribution::NO_WAIT) 
      idxs[i] = 0;
    else 
      idxs[i] = (dice() <= percentage) ? 1 : 0;
  }
}



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

    std::vector<int> idxs;
    init_data(idxs, DATA_DISTR, PERCENTAGE);


    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = kernel_fpga(q, idxs);

    // Wait for all work to finish.
    q.wait();

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

