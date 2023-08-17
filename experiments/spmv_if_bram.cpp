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

constexpr int kM = 100;

double spmv_if_kernel(queue &q, std::vector<int> &h_w,
                      std::vector<int> &h_all_zero, std::vector<int> &h_data,
                      const int M) {
  int *w = fpga_tools::toDevice(h_w, q);
  int *all_zero = fpga_tools::toDevice(h_all_zero, q);
  int *dram_data = fpga_tools::toDevice(h_data, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int data[kM *kM];

    #if TEST
    for (int i=0; i<N*N; ++i)
      data[i] = data_dram[i];
    #endif

    for (int j = 0; j < M; j++) {
      if (!all_zero[j]) {
        for (int i = 0; i < M; i++) {
          data[w[i]] += w[i * M + j] * data[i];
        }
      }
    }

    #if TEST
    for (int i=0; i<N*N; ++i)
      data_dram[i] = data[i];
    #endif 
  });

  event.wait();

  q.copy(dram_data, h_data.data(), h_data.size()).wait();
  sycl::free(dram_data, q);

  sycl::free(w, q);
  sycl::free(all_zero, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void spmv_if_cpu(std::vector<int> &w, std::vector<int> &all_zero,
                 std::vector<int> &data) {
    for (int j = 0; j < kM; j++) {
      if (!all_zero[j]) {
        for (int i = 0; i < kM; i++) {
          data[i] += w[i * kM + j] * data[i];
        }
      }
    }
}

void init_data(std::vector<int> &h_w, std::vector<int> &h_all_zero,
                const uint percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 99);
  auto dice = std::bind(distribution, generator);

  for (int r = 0; r < kM; ++r) {
    h_all_zero[r] = (dice() < percentage) ? 0 : 1;
    
    for (int c = 0; c < kM; ++c) 
      h_w[r*kM + c] = (dice() < percentage) ? 0 : r*kM + c;
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

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<int> w(kM*kM);
    std::vector<int> all_zero(kM);
    std::vector<int> h_data(kM, 1);
    std::vector<int> cpu_data(kM, 1);

    init_data(w, all_zero, PERCENTAGE);

    auto kernel_time = spmv_if_kernel(q, w, all_zero, h_data, kM);

    std::cout << "Kernel time (ms): " << kernel_time << "\n";

    #if TEST
    spmv_if_cpu(w, all_zero, cpu_data);
    if (std::equal(cpu_data.begin(), cpu_data.begin(), h_data.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failed\n";
    }
    #endif

  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
