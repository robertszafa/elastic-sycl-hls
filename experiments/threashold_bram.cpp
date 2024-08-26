#include <algorithm>
#include <iostream>
#include <limits>
#include <math.h>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <sycl/sycl.hpp>
#include <vector>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "exception_handler.hpp"
#include "memory_utils.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

constexpr int kN = 1000;

// Setting TEST will ensure test data is transfered from FPGA DRAM to to BRAM
// and back. This adds latency, so leave unset for the benchmarks.
#define TEST 1

double threashold_kernel(queue &q, const std::vector<int> &h_idx,
                         std::vector<int> &h_red, std::vector<int> &h_blue,
                         std::vector<int> &h_green, int N, int th) {
  int *idx = fpga_tools::toDevice(h_idx, q);
  int *red_dram = fpga_tools::toDevice(h_red, q);
  int *green_dram = fpga_tools::toDevice(h_green, q);
  int *blue_dram = fpga_tools::toDevice(h_blue, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    [[intel::singlepump]] 
    [[intel::max_replicates(1)]] 
    [[intel::numbanks(1)]] 
    int red[kN];
    [[intel::singlepump]] 
    [[intel::max_replicates(1)]] 
    [[intel::numbanks(1)]] 
    int green[kN];
    [[intel::singlepump]] 
    [[intel::max_replicates(1)]] 
    [[intel::numbanks(1)]] 
    int blue[kN];

    for (int i = 0; i < N; ++i) {
      red[i] = red_dram[i];
      green[i] = green_dram[i];
      blue[i] = blue_dram[i];
    }

    for (unsigned i = 0; i < N; i++) {
      int sum = red[idx[i]] + green[idx[i]] + blue[idx[i]];
      if (sum >= th) {
        red[idx[i]] = 0;
        green[idx[i]] = 0;
        blue[idx[i]] = 0;
      }
    }

    for (int i = 0; i < N; ++i) {
      red_dram[i] = red[i];
      green_dram[i] = green[i];
      blue_dram[i] = blue[i];
    }
  });

  event.wait();
  q.copy(red_dram, h_red.data(), h_red.size()).wait();
  q.copy(green_dram, h_green.data(), h_green.size()).wait();
  q.copy(blue_dram, h_blue.data(), h_blue.size()).wait();

  sycl::free(idx, q);
  sycl::free(red_dram, q);
  sycl::free(green_dram, q);
  sycl::free(blue_dram, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void threashold_cpu(const std::vector<int> &idx, std::vector<int> &red,
                    std::vector<int> &green, std::vector<int> &blue,
                    const int N, int th) {
  for (unsigned i = 0; i < N; i++) {
    int sum = red[idx[i]] + green[idx[i]] + blue[idx[i]];
    if (sum >= th) {
      red[idx[i]] = 0;
      green[idx[i]] = 0;
      blue[idx[i]] = 0;
    }
  }
}

void init_data(std::vector<int> &idx, std::vector<int> &red,
               std::vector<int> &green, std::vector<int> &blue,
               const int percentage_raw, int PERCENTAGE_MISSPECULATION) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 99);
  auto dice = std::bind(distribution, generator);

  for (int i = 0; i < idx.size(); i++) {
    idx[i] = (dice() < percentage_raw) ? 1 : i;
    red[i] = (dice() < PERCENTAGE_MISSPECULATION) ? 0 : 50;
    green[i] = (dice() < PERCENTAGE_MISSPECULATION) ? 0 : 50;
    blue[i] = (dice() < PERCENTAGE_MISSPECULATION) ? 0 : 50;
  }
}

int main(int argc, char *argv[]) {
  int ARRAY_SIZE = 1000;
  int PERCENTAGE_RAW = 0;
  int PERCENTAGE_MISSPECULATION = 0;
  try {
    if (argc > 1) {
      ARRAY_SIZE = int(atoi(argv[1]));
    }
    if (argc > 2) {
      PERCENTAGE_RAW = int(atoi(argv[2]));
      if (PERCENTAGE_RAW < 0 || PERCENTAGE_RAW > 100)
        throw std::invalid_argument("Invalid raw percentage.");
    }
    if (argc > 3) {
      PERCENTAGE_MISSPECULATION = int(atoi(argv[3]));
      if (PERCENTAGE_MISSPECULATION < 0 || PERCENTAGE_MISSPECULATION > 100)
        throw std::invalid_argument("Invalid branch percentage.");
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
#else // #if FPGA_EMULATOR
  auto d_selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif

  try {
    // Enable profiling.
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<int> idx(ARRAY_SIZE);
    std::vector<int> red(ARRAY_SIZE);
    std::vector<int> green(ARRAY_SIZE);
    std::vector<int> blue(ARRAY_SIZE);
    std::vector<int> red_cpu(ARRAY_SIZE);
    std::vector<int> green_cpu(ARRAY_SIZE);
    std::vector<int> blue_cpu(ARRAY_SIZE);

    init_data(idx, red, green, blue, PERCENTAGE_RAW, PERCENTAGE_MISSPECULATION);
    std::copy(red.begin(), red.end(), red_cpu.begin());
    std::copy(green.begin(), green.end(), green_cpu.begin());
    std::copy(blue.begin(), blue.end(), blue_cpu.begin());

    threashold_cpu(idx, red_cpu, green_cpu, blue_cpu, ARRAY_SIZE, 100);

    auto kernel_time = threashold_kernel(q, idx, red, green, blue, ARRAY_SIZE, 100);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(red.begin(), red.end(), red_cpu.begin())) {
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

