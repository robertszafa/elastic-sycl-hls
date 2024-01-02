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

double jacobi1d_kernel(queue &q, std::vector<float> &h_A, 
                       std::vector<float> &h_B, int timeSteps) {
  const int kN = h_A.size();

  auto *A = fpga_tools::toDevice(h_A, q);
  auto *B = fpga_tools::toDevice(h_B, q);

  using pipe_done_B = pipe<class pipe_done_B_class, bool, 32>;

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {

    for (int t = 0; t < timeSteps; t++) {
      #pragma unroll 8
      for (int i = 1; i < kN - 1; i++) {
        B[i] = 0.33333f * (A[i - 1] + A[i] + A[i + 1]);
      }

      pipe_done_B::write(1);
    }
  });
  
  auto event2 = q.single_task<class MainKernel2>([=]() [[intel::kernel_args_restrict]] {

    for (int t = 0; t < timeSteps; t++) {
      pipe_done_B::read();

      #pragma unroll 8
      for (int i = 1; i < kN - 1; i++) {
        A[i] = 0.33333f * (B[i - 1] + B[i] + B[i + 1]);
      }
    }
  });

  event.wait();
  event2.wait();

  q.copy(A, h_A.data(), h_A.size()).wait();
  q.copy(B, h_B.data(), h_B.size()).wait();

  sycl::free(A, q);
  sycl::free(B, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void jacobi1d_cpu(std::vector<float> &A, std::vector<float> &B,
                  const int timeSteps) {
  const int kN = A.size();

  for (int t = 0; t < timeSteps; t++) {
    for (int i = 1; i < kN - 1; i++) {
      B[i] = 0.33333f * (A[i - 1] + A[i] + A[i + 1]);
    }

    for (int i = 1; i < kN - 1; i++) {
      A[i] = 0.33333f * (B[i - 1] + B[i] + B[i + 1]);
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

    std::vector<float> A(ARRAY_SIZE, 1);
    std::vector<float> A_cpu(ARRAY_SIZE, 1);
    std::vector<float> B(ARRAY_SIZE, 2);
    std::vector<float> B_cpu(ARRAY_SIZE, 2);

    const int timeSteps = 1;

    auto kernel_time = jacobi1d_kernel(q, A, B, timeSteps);
    jacobi1d_cpu(A_cpu, B_cpu, timeSteps);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    bool equalAs = std::equal(A.begin(), A.end(), A_cpu.begin());
    bool equalBs = std::equal(B.begin(), B.end(), B_cpu.begin());
    if (equalAs && equalBs)
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
