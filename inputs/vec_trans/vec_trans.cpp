#include <CL/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <vector>
#include <random>

#include <sycl/ext/intel/fpga_extensions.hpp>


#include "memory_utils.hpp"

using namespace sycl;
using namespace fpga_tools;

// Forward declare kernel name.
class MainKernel;

double vec_trans_kernel(queue &q, std::vector<int> &h_A, const std::vector<int> &h_b) {

  const int N = h_A.size();

  int *A = fpga_tools::toDevice(h_A, q);
  int *b = fpga_tools::toDevice(h_b, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < N; i++) {
      int d = A[i];
      A[b[i]] =
          (((((((d + 112) * d + 23) * d + 36) * d + 82) * d + 127) * d + 2) *
               d +
           20) *
              d +
          100;
    }
  });

  event.wait();
  q.copy(A, h_A.data(), h_A.size()).wait();

  sycl::free(A, q);
  sycl::free(b, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void vec_trans_cpu(std::vector<int> &A, const std::vector<int> &b) {
  const int N = A.size();

  for (int i = 0; i < N; i++) {
    int d = A[i];
    A[b[i]] = (((((((d + 112) * d + 23) * d + 36) * d + 82) * d + 127) * d + 2) * d + 20) * d + 100;
  }
}

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<int> &A, std::vector<int> &b, const data_distribution distr, 
               const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind (distribution, generator);

  for (int i = 0; i < A.size(); i++) {
    if (distr == data_distribution::ALL_WAIT) {
      b[i] = std::max(i-2, 0);
    }
    else if (distr == data_distribution::NO_WAIT) {
      b[i] = i;
    }
    else {
      b[i] = (dice() <= percentage) ? std::max(i-2, 0) : i;
    }

    A[i] = i % 50-25;
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
  int ARRAY_SIZE = 100;
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

    std::vector<int> A(ARRAY_SIZE);
    std::vector<int> b(ARRAY_SIZE); 

    init_data(A, b, DATA_DISTR, PERCENTAGE);

    std::vector<int> A_cpu(ARRAY_SIZE);
    std::copy(A.begin(), A.end(), A_cpu.begin());

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = vec_trans_kernel(q, A, b);

    // Wait for all work to finish.
    q.wait();

    vec_trans_cpu(A_cpu, b);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(A.begin(), A.end(), A_cpu.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failed\n";
      std::cout << "sum(fpga) = " << std::accumulate(A.begin(), A.end(), 0.0) << "\n";
      std::cout << "sum(cpu) = " << std::accumulate(A_cpu.begin(), A_cpu.end(), 0.0) << "\n";
    }
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

