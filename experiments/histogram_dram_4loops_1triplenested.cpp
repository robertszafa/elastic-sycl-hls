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

// Forward declare kernel name.
class MainKernel;

double histogram_kernel(queue &q, const std::vector<int> &h_idx,
                        std::vector<int> &h_hist, std::vector<int> &h_hist2) {
  const int array_size = h_idx.size();

  int *idx = fpga_tools::toDevice(h_idx, q);
  int *hist = fpga_tools::toDevice(h_hist, q);
  int *hist2 = fpga_tools::toDevice(h_hist2, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int k = 0; k < 2; ++k) {
      for (int j = 0; j < 2; ++j) {
        for (int i = 0; i < array_size; ++i) {
          auto x = hist[i];
          hist[i] = x + 1;
        }
      }

      for (int i = 0; i < array_size; ++i) {
        auto x = hist2[i];
        hist2[i] = x + 2;
      }
    }
  
  });

  event.wait();
  q.copy(hist, h_hist.data(), h_hist.size()).wait();
  q.copy(hist2, h_hist2.data(), h_hist2.size()).wait();

  sycl::free(idx, q);
  sycl::free(hist, q);
  sycl::free(hist2, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void histogram_cpu(const int *idx, int *hist, int *hist2, const int N) {
  for (int k = 0; k < 2; ++k) {
    for (int j = 0; j < 2; ++j) {
      for (int i = 0; i < N; ++i) {
        auto x = hist[k]; 
        hist[i] = x + 1;
      }
    }

    for (int i = 0; i < N; ++i) {
      auto x = hist2[i];
      hist2[i] = x + 2;
    }
  }
}

void init_data(std::vector<int> &feature, std::vector<int> &hist,
                const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 99);
  auto dice = std::bind (distribution, generator);

  int counter=0;
  for (int i = 0; i < feature.size(); i++) {
    feature[i] = (dice() < percentage) ? 1 : i;

    hist[i] = 0.0;
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

    std::vector<int> feature(ARRAY_SIZE);
    std::vector<int> hist(ARRAY_SIZE);
    std::vector<int> hist2(ARRAY_SIZE, 1);
    std::vector<int> hist_cpu(ARRAY_SIZE);
    std::vector<int> hist2_cpu(ARRAY_SIZE, 1);

    init_data(feature, hist,  PERCENTAGE);
    std::copy(hist.begin(), hist.end(), hist_cpu.begin());

    auto kernel_time = histogram_kernel(q, feature, hist, hist2);

    histogram_cpu(feature.data(), hist_cpu.data(), hist2_cpu.data(),
                  hist_cpu.size());

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(hist.begin(), hist.end(), hist_cpu.begin()) &&
        std::equal(hist2.begin(), hist2.end(), hist2_cpu.begin()))
      std::cout << "Passed\n";
    else 
      std::cout << "Failed\n";
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

