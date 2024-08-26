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
#include "device_print.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double doitgen_triple_kernel(queue &q, std::vector<float> &h_A,
                             std::vector<float> &h_sum,
                             const std::vector<float> &h_w) {
  auto *A = fpga_tools::toDevice(h_A, q);
  auto *sum = fpga_tools::toDevice(h_sum, q);
  auto *w = fpga_tools::toDevice(h_w, q);

  const int N = h_A.size();

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int idx[1000];

    for (int i = 0; i < N; i++) {
      float s = 0;
      for (int j = i; j < N; j++) {
        float a = A[j];
        float wt = w[i * N + j];
        if (a > 0.0) {
          s += (a * wt * s) * s;
          A[idx[i]] = a + 1; // speculated store
        }
      }
      sum[i] = s;
    }
    
    for (int i = 0; i < N; i++) {
      float q = sum[i];
      A[i] = A[i] + q * q * q;
    }
  });

  event.wait();
  q.copy(sum, h_sum.data(), h_sum.size()).wait();

  sycl::free((void *)A, q);
  sycl::free((void *)sum, q);
  sycl::free((void *)w, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void doitgen_triple_cpu(std::vector<float> &A, std::vector<float> &sum,
                        const std::vector<float> &w) {
  const int N = A.size();

  for (int i = 0; i < N; i++) {
    float s = 0;
    for (int j = i; j < N; j++) {
      float a = A[j];
      float wt = w[i * N + j];
      if (a > 0.0) {
        s += (a * wt * s) * a;
      }
    }
    sum[i] = s;
  }
  for (int i = 0; i < N; i++) {
    float q = sum[i];
    A[i] = A[i] + q * q * q;
  }
}

void init_data(std::vector<float> &A, std::vector<float> &sum,
               std::vector<float> &w, int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(1, 99);
  auto dice = std::bind(distribution, generator);

  for (int i = 0; i < A.size(); ++i) {
    A[i] = (dice() < percentage) ? 1.0f : -1.0f;
    sum[i] = 0.0;
    w[i] = rand();
  }
}


int main(int argc, char *argv[]) {
  int N = 10;
  int PERCENTAGE = 0;
  try {
    if (argc > 1) {
      N = int(atoi(argv[1]));
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

    std::vector<float> A(N);
    std::vector<float> sum(N);
    std::vector<float> w(N*N);
    std::vector<float> A_cpu(N);
    std::vector<float> sum_cpu(N);

    init_data(A, sum, w, PERCENTAGE);

    std::copy(A.begin(), A.end(), A_cpu.begin());
    std::copy(sum.begin(), sum.end(), sum_cpu.begin());

    
    auto kernel_time = doitgen_triple_kernel(q, A, sum, w);

    doitgen_triple_cpu(A_cpu, sum_cpu, w);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(sum.begin(), sum.end(), sum_cpu.begin())) {
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
