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
#include "device_print.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double histogram_2_addresses_1_decoupled_kernel(queue &q,
                                                const std::vector<int> &h_idx,
                                                std::vector<float> &h_hist,
                                                const std::vector<int> &h_idx2,
                                                std::vector<int> &h_hist2) {
  const int N = h_idx.size();

  int *idx = fpga_tools::toDevice(h_idx, q);
  float *hist = fpga_tools::toDevice(h_hist, q);

  int *idx2 = fpga_tools::toDevice(h_idx2, q);
  auto *hist2 = fpga_tools::toDevice(h_hist2, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < N; ++i) {
      auto idx_scalar = idx[i];
      auto x = hist[idx_scalar];
      hist[idx_scalar] = x + 10.0;

      auto idx_scalar2 = idx2[i];
      auto x2 = hist2[idx_scalar2];
      if (x2 > 0 && x2 < N) 
        hist2[x2] = x2 + 3; // this address. gen cannot be decoupled.
    }
  });

  event.wait();
  q.copy(hist, h_hist.data(), h_hist.size()).wait();
  q.copy(hist2, h_hist2.data(), h_hist2.size()).wait();

  sycl::free(idx, q);
  sycl::free(hist, q);
  sycl::free(idx2, q);
  sycl::free(hist2, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void histogram_cpu(const int *idx, float *hist, const int *idx2, int *hist2,
                   const int N) {
  for (int i = 0; i < N; ++i) {
    auto idx_scalar = idx[i];
    auto x = hist[idx_scalar];
    hist[idx_scalar] = x + 10.0;

    auto idx_scalar2 = idx2[i];
    auto x2 = hist2[idx_scalar2];
    if (x2 > 0 && x2 < N) 
      hist2[x2] = x2 + 3; // this address. gen cannot be decoupled.
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
    std::cout << "Running on device: " << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<int> feature(ARRAY_SIZE, 0);
    std::vector<float> hist(ARRAY_SIZE, 0.0);
    std::vector<float> hist_cpu(ARRAY_SIZE, 0.0);
    std::vector<int> hist2(ARRAY_SIZE, 1);
    std::vector<int> hist2_cpu(ARRAY_SIZE, 1);
    
    // std::iota(feature.begin(), feature.end(), 0);
    for (size_t i=0; i<ARRAY_SIZE; ++i) {
      feature[i] = rand() % ARRAY_SIZE;
      hist2[i] = rand() % 10;
      hist2_cpu[i] = hist2[i];
    }

    auto kernel_time = histogram_2_addresses_1_decoupled_kernel(q, feature, hist, feature, hist2);

    histogram_cpu(feature.data(), hist_cpu.data(), feature.data(),
                  hist2_cpu.data(), hist_cpu.size());

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(hist.begin(), hist.end(), hist_cpu.begin()) &&
        std::equal(hist2.begin(), hist2.end(), hist2_cpu.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failed\n";

      for (size_t i=0; i<ARRAY_SIZE; ++i) {
        if (hist2[i] != hist2_cpu[i])
          std::cout << i << ": " << hist2[i] << " != " << hist2_cpu[i] << "\n";
      }
    }
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

