#include <CL/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <vector>
#include <random>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "store_queue.hpp"
#include "memory_utils.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;


double get_tanh_double_kernel(queue &q, std::vector<double> &h_A, const std::vector<int> h_addr_in,
                       const std::vector<int> h_addr_out) {
  const uint array_size = h_A.size();

  double* A = toDevice(h_A, q);
  int* addr_in = toDevice(h_addr_in, q);
  int* addr_out = toDevice(h_addr_out, q);

  auto event = q.submit([&](handler &hnd) {
    hnd.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
      double result, is_neg, beta;
      for (int i = 0; i < array_size; i++) {
        beta = A[addr_in[i]];

        result = ((beta*beta+19.52381)*beta*beta+3.704762)*beta;

        A[addr_out[i]] = result;
      }
    });
  });

  event.wait();
  q.copy(A, h_A.data(), h_A.size()).wait();

  sycl::free(A, q);
  sycl::free(addr_in, q);
  sycl::free(addr_out, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}


enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<double> &A, std::vector<int> &addr_in, std::vector<int> &addr_out,
               const data_distribution distr, const uint percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind(distribution, generator);

  addr_in[0] = 0;
  addr_in[1] = 1;
  for (int i = 0; i < A.size(); i++) {
    A[i] = i % 2 == 0? ((double)rand()/RAND_MAX*2.0-1.0) : 1.2;

    if (distr == data_distribution::ALL_WAIT) {
      addr_in[i] = std::max(i - 1, 0);
      addr_out[i] = std::max(i - 1, 0);
    } else if (distr == data_distribution::NO_WAIT) {
      addr_in[i] = i;
      addr_out[i] = i;
    } else {
      addr_in[i] = (dice() <= percentage) ? addr_in[std::max(i - 1, 0)] : i;
      addr_out[i] = addr_in[i];
    }
  }
}


void get_tanh_double_cpu(std::vector<double> &A, const std::vector<int> addr_in,
                  const std::vector<int> addr_out) {
  int i;
  double result, is_neg, beta;

  for (i = 0; i < A.size(); i++) {
    beta = A[addr_in[i]];

    result = ((beta*beta+19.52381)*beta*beta+3.704762)*beta;

    A[addr_out[i]] = result;
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

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almost_equal(T x, T y)
{
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::fabs(x-y) <= std::numeric_limits<T>::epsilon() * std::fabs(x+y) * 2
        // unless the result is subnormal
        || std::fabs(x-y) < std::numeric_limits<T>::min();
}

int main(int argc, char *argv[]) {
  // Get A_SIZE and forward/no-forward from args.
  // defaulats
  uint ARRAY_SIZE = 64;
  auto DATA_DISTR = data_distribution::ALL_WAIT;
  uint PERCENTAGE = 5;
  try {
    if (argc > 1) {
      ARRAY_SIZE = uint(atoi(argv[1]));
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
    std::cout << "  ./hist [ARRAY_SIZE] [data_distribution (0/1/2)] [PERCENTAGE (only for "
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

    std::vector<double> A(ARRAY_SIZE);
    std::vector<int> addr_in(ARRAY_SIZE);
    std::vector<int> addr_out(ARRAY_SIZE);

    init_data(A, addr_in, addr_out, DATA_DISTR, PERCENTAGE);

    std::vector<double> A_cpu(ARRAY_SIZE);
    std::copy(A.begin(), A.end(), A_cpu.begin());

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = get_tanh_double_kernel(q, A, addr_in, addr_out);

    // Wait for all work to finish.
    q.wait();

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    get_tanh_double_cpu(A_cpu, addr_in, addr_out);
    if (std::equal(A.begin(), A.end(), A_cpu.begin(), almost_equal<double>)) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failed";
      std::cout << " sum(A_fpga) = " << std::accumulate(A.begin(), A.end(), 0.0) << "\n";
      std::cout << " sum(A_cpu) = " << std::accumulate(A_cpu.begin(), A_cpu.end(), 0.0) << "\n";
    }
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

