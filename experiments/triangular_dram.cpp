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
#include "device_print.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double triangular_kernel(queue &q, std::vector<float> &h_A,
                         std::vector<float> &h_x) {
  const uint N = h_x.size();

  auto *A = fpga_tools::toDevice(h_A, q);
  auto *x = fpga_tools::toDevice(h_x, q);

  std::vector<std::pair<sycl::event, bool>> events; // {event, measureTimeBool} pairs

  auto main_event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (uint i = 0; i < N; i++) {
      for (uint k = 0; k < i; k++) {
        A[k * N + N] -= A[k * N + i] * x[i];
      }
    }
  });

  
  events.push_back({main_event, true});
  for (auto &kv : events) kv.first.wait();
  q.copy(A, h_A.data(), h_A.size()).wait();

  sycl::free(A, q);
  sycl::free(x, q);

  double max_event_time = 0;
  for (auto &[e, toMeasure] : events) {
    if (toMeasure) {
      auto start = e.get_profiling_info<info::event_profiling::command_start>();
      auto end = e.get_profiling_info<info::event_profiling::command_end>();
      double this_event_time = static_cast<double>(end - start) / 1000000;
      max_event_time = std::max(max_event_time, this_event_time);
    }
  }

  return max_event_time;
}

void triangular_cpu(std::vector<float> &A, std::vector<float> &x) {
  const uint N = x.size();

  for (uint i = 0; i < N; i++) {
    for (uint k = 0; k < i; k++) {
      A[k * N + N] -= A[k * N + i] * x[i];
    }
  }
}

int main(int argc, char *argv[]) {
  int N = 10;
  try {
    if (argc > 1) {
      N = int(atoi(argv[1]));
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

    std::vector<float> A(N*N, 1);
    std::vector<float> A_cpu(N*N, 1);
    std::vector<float> x(N, 1);
    std::vector<float> x_cpu(N, 1);

    auto kernel_time = triangular_kernel(q, A, x);
    triangular_cpu(A_cpu, x_cpu);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(A.begin(), A.end(), A_cpu.begin()))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
