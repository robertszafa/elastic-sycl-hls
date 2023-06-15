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

double gsum_kernel(queue &q, const std::vector<double> &h_A,
                   const std::vector<double> &h_B, double *h_res) {
  auto *A = fpga_tools::toDevice(h_A, q);
  auto *B = fpga_tools::toDevice(h_B, q);
  auto *res = sycl::malloc_device<double>(1, q);

  const int N = h_A.size();

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    double d, s = 0.0;
    int i;

    for (i = 0; i < N; i++) {
      d = A[i] + B[i];
      if (d >= 0) {
        s += (((((d + 0.64) * d + 0.7) * d + 0.21) * d + 0.33) * d + 0.25) * d +
             0.125;
      }
    }

    *res = s;
  });

  event.wait();
  q.copy(res, h_res, 1).wait();

  sycl::free((void *)A, q);
  sycl::free((void *)B, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void gsum_cpu(const std::vector<double> &A, const std::vector<double> &B,
              double *res) {
  const int N = A.size();
  double d, s = 0.0;
  int i;

  for (i = 0; i < N; i++) {
    d = A[i] + B[i];
    if (d >= 0)
      s += (((((d + 0.64) * d + 0.7) * d + 0.21) * d + 0.33) * d + 0.25) * d +
           0.125;
  }

  *res = s;
}


void init_data(std::vector<double> &A, std::vector<double> &B,
                int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind(distribution, generator);

  for (int i = 0; i < A.size(); ++i) {
    if (dice() < percentage) {
      A[i] = 1.0;
      B[i] = 1.0;
    } else {
      A[i] = -1.0;
      B[i] = -1.0;
    }
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

    std::vector<double> A(ARRAY_SIZE);
    std::vector<double> B(ARRAY_SIZE);
    double res, res_cpu;

    init_data(A, B,  PERCENTAGE);

    auto kernel_time = gsum_kernel(q, A, B, &res);

    gsum_cpu(A, B, &res_cpu);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (res == res_cpu) {
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
