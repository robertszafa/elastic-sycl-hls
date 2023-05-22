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

constexpr int N = 100;

double kernel_fpga(queue &q, std::vector<int> &h_w, std::vector<int> &h_all_zero) {

  int *w = fpga_tools::toDevice(h_w, q);
  int *all_zero = fpga_tools::toDevice(h_all_zero, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int j, i, temp;

    int data[N * N];

    for (int j = 0; j < N; j++) {
      temp = all_zero[j];
      if (!all_zero) {
        for (i = 0; i < N; i++) {
          data[w[i]] += w[i * N + j] * data[i];
        }
      }
    }
  });

  event.wait();

  sycl::free(w, q);
  sycl::free(all_zero, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<int> &h_w, std::vector<int> &h_all_zero,
               const data_distribution distr, const uint percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind(distribution, generator);

  for (int r = 0; r < N; ++r) {
    if (distr == data_distribution::ALL_WAIT) {
      h_all_zero[r] = 0;
      for (int c = 0; c < N; ++c) {
        h_w[r*N + c] = 0;
      }
    }
    else if (distr == data_distribution::NO_WAIT) {
      h_all_zero[r] = 1;
      for (int c = 0; c < N; ++c) {
        h_w[r*N + c] = r*N + c;
      }
    }
    else {
      h_all_zero[r] = (dice() <= percentage) ? 0 : 1;
      for (int c = 0; c < N; ++c) {
        h_w[r*N + c] = (dice() <= percentage) ? 0 : r*N + c;
      }
    }

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

template <class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
almost_equal(T x, T y) {
  std::ostringstream xSS, ySS;
  xSS << x;
  ySS << y;
  if (x == y || xSS.str() == ySS.str())
    return true;
  // the machine epsilon has to be scaled to the magnitude of the values used
  // and multiplied by the desired precision in ULPs (units in the last place)
  return std::fabs(x - y) <=
             std::numeric_limits<T>::epsilon() * std::fabs(x + y) * 2
         // unless the result is subnormal
         || std::fabs(x - y) < std::numeric_limits<T>::min();
}

int main(int argc, char *argv[]) {
  // Get A_SIZE and forward/no-forward from args.
  uint M = 64;
  auto DATA_DISTR = data_distribution::ALL_WAIT;
  uint PERCENTAGE = 5;
  try {
    if (argc > 1) {
      M = uint(atoi(argv[1]));
    }
    if (argc > 2) {
      DATA_DISTR = data_distribution(atoi(argv[2]));
    }
    if (argc > 3) {
      PERCENTAGE = uint(atoi(argv[3]));
      std::cout << "Percentage is " << PERCENTAGE << "\n";
      if (PERCENTAGE < 0 || PERCENTAGE > 100)
        throw std::invalid_argument("Invalid percentage.");
    }
  } catch (exception const &e) {
    std::cout << "Incorrect argv.\nUsage:\n";
    std::cout << "  ./hist [ARRAY_SIZE] [data_distribution (0/1/2)] "
                 "[PERCENTAGE (only for data_distr 2)]\n";
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
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<int> w(N*N);
    std::vector<int> all_zero(N);

    init_data(w, all_zero, DATA_DISTR, PERCENTAGE);

    auto kernel_time = kernel_fpga(q, w, all_zero);

    // Wait for all work to finish.
    q.wait();

    std::cout << "Kernel time (ms): " << kernel_time << "\n";

  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
