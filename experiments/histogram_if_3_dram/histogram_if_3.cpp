#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <vector>
#include <random>

#include <sycl/ext/intel/fpga_extensions.hpp>


#include "memory_utils.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double histogram_if_kernel(queue &q, const std::vector<int> &h_idx, const std::vector<int> &h_weight,
                           std::vector<float> &h_hist) {
  const int array_size = h_idx.size();

  int *idx = fpga_tools::toDevice(h_idx, q);
  float *hist = fpga_tools::toDevice(h_hist, q);
  int *weight = fpga_tools::toDevice(h_weight, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < array_size; ++i) {
      auto wt = weight[i];
      auto idx_scalar = idx[i];
      auto x = hist[idx_scalar];

      if (x > 0) {
        hist[idx_scalar] = 10 + wt;
        // hist[idx_scalar] = x + wt; // if we use x for the calc, then it's
        // baseAddr->storeI dep is found
      }
    }
  });

  event.wait();
  q.copy(hist, h_hist.data(), h_hist.size()).wait();

  sycl::free(idx, q);
  sycl::free(hist, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void histogram_if_cpu(const int *idx, const int *weight, float *hist, const int N) {
  for (int i = 0; i < N; ++i) {
    auto wt = weight[i];
    auto idx_scalar = idx[i];
    auto x = hist[idx_scalar];

    if (x > 0.0) {
      hist[idx_scalar] = 10 + wt;
      // hist[idx_scalar] = x + wt; // if we use x for the calc, then it's baseAddr->storeI dep is found
    }
  }
}

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<int> &feature, std::vector<int> &weight, std::vector<float> &hist,
               const data_distribution distr, const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind (distribution, generator);

  int counter=0;
  for (int i = 0; i < feature.size(); i++) {
    if (distr == data_distribution::ALL_WAIT) {
      feature[i] = (feature.size() >= 4) ? i % 4 : 0;
    }
    else if (distr == data_distribution::NO_WAIT) {
      feature[i] = i;
    }
    else {
      feature[i] = (dice() <= percentage) ? feature[std::max(i-1, 0)] : i;
    }

    weight[i] = (i % 2 == 0) ? 1 : 0;
    hist[i] = (i % 2 == 0) ? 1.0 : 0.0;
  }
}

// Create an exception handler for asynchronous SYCL exceptions
static auto exception_handler = [](sycl::exception_list e_list) {
  for (std::exception_ptr const &e : e_list) {
    try {
      std::rethrow_exception(e);
    } catch (std::exception const &e) {
#if _DEBUG
      std::cout << "Failure" << std::endl;
#endif
      std::terminate();
    }
  }
};

int main(int argc, char *argv[]) {
  // Get A_SIZE and forward/no-forward from args.
  int ARRAY_SIZE = 64;
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
    std::cout << "  ./executable [ARRAY_SIZE] [data_distribution (0/1/2)] [PERCENTAGE (only for "
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

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: " << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<int> feature(ARRAY_SIZE);
    std::vector<int> weight(ARRAY_SIZE);
    std::vector<float> hist(ARRAY_SIZE);
    std::vector<float> hist_cpu(ARRAY_SIZE);

    init_data(feature, weight, hist, DATA_DISTR, PERCENTAGE);
    std::copy(hist.begin(), hist.end(), hist_cpu.begin());

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = histogram_if_kernel(q, feature, weight, hist);

    // Wait for all work to finish.
    q.wait();

    histogram_if_cpu(feature.data(), weight.data(), hist_cpu.data(), hist_cpu.size());

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(hist.begin(), hist.end(), hist_cpu.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failed\n";
      std::cout << "sum(fpga) = " << std::accumulate(hist.begin(), hist.end(), 0.0) << "\n";
      std::cout << "sum(cpu) = " << std::accumulate(hist_cpu.begin(), hist_cpu.end(), 0.0) << "\n";
    }
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

