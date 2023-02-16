#include <CL/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <limits>
#include <math.h>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <vector>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "memory_utils.hpp"
// #include "onchip_memory_with_cache.hpp"
#include "unrolled_loop.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;


double histogram_kernel(queue &q, const std::vector<int> &h_idx, std::vector<int> &h_hist) {
  const int array_size = h_idx.size();

  int *idx = fpga_tools::toDevice(h_idx, q);
  // int *hist = fpga_tools::toDevice(h_hist, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    constexpr int k_cache_depth = 8;
    constexpr int k_mem_depth = 10000;
    int data_[k_mem_depth];
    [[intel::fpga_register]] int cache_val_[k_cache_depth];
    [[intel::fpga_register]] int cache_addr_[k_cache_depth];
    [[intel::fpga_register]] bool cache_valid_[k_cache_depth];

    for (int i = 0; i < array_size; ++i) {
      auto idx_scalar = idx[i];
      
      int return_val;
      auto addr = idx_scalar;
      bool in_cache = false;

      /* READ */
      // check the cache, newest value will take precedence
      fpga_tools::UnrolledLoop<k_cache_depth>([&](auto i) {
        if ((cache_addr_[i] == addr) && (cache_valid_[i])) {
          return_val = cache_val_[i];
          in_cache = true;
        }
      });
      // if not in the cache, fetch from memory
      if (!in_cache) 
        return_val = data_[addr];
      /* READ */

      auto x = return_val;
      x = x + 1;

      // hist.write(idx_scalar, x + 1);
      // write the value from the end of the cache into the memory

      /* WRITE */
      auto val = x;
      if (cache_valid_[0]) 
        data_[cache_addr_[0]] = cache_val_[0];

      // Shift the values in the cache
      fpga_tools::UnrolledLoop<k_cache_depth - 1>([&](auto i) {
        if (cache_addr_[i + 1] == addr) {
          cache_valid_[i] = false;  // invalidate old cache entry at same addr
        } else {
          cache_valid_[i] = cache_valid_[i + 1];
        }
        cache_val_[i] = cache_val_[i + 1];
        cache_addr_[i] = cache_addr_[i + 1];
      });

      // write the new value into the cache
      cache_val_[k_cache_depth - 1] = val;
      cache_addr_[k_cache_depth - 1] = addr;
      cache_valid_[k_cache_depth - 1] = true;
      /* WRITE */

    }
  });

  event.wait();
  // q.copy(hist, h_hist.data(), h_hist.size()).wait();

  sycl::free(idx, q);
  // sycl::free(hist, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void histogram_cpu(const int *idx, int *hist, const int N) {
  for (int i = 0; i < N; ++i) {
    auto idx_scalar = idx[i];
    auto x = hist[idx_scalar];
    hist[idx_scalar] = x + 1;
  }
}

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<int> &feature, std::vector<int> &hist,
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

    hist[i] = 0.0;
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

#if FPGA_EMULATOR
  ext::intel::fpga_emulator_selector d_selector;
#elif FPGA
  ext::intel::fpga_selector d_selector;
#else
  default_selector d_selector;
#endif
  try {
    // Enable profiling.
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: " << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<int> feature(ARRAY_SIZE);
    std::vector<int> hist(ARRAY_SIZE);
    std::vector<int> hist_cpu(ARRAY_SIZE);

    init_data(feature, hist, DATA_DISTR, PERCENTAGE);
    std::copy(hist.begin(), hist.end(), hist_cpu.begin());

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = histogram_kernel(q, feature, hist);

    // Wait for all work to finish.
    q.wait();

    histogram_cpu(feature.data(), hist_cpu.data(), hist_cpu.size());

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

