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
using namespace fpga_tools;

using DATA_TYPE = float;

// Forward declare kernel name.
class MainKernel;

constexpr int N = 1000;

double filter_sum_kernel(queue &q, const std::vector<int> &h_idxs,
                   std::vector<DATA_TYPE> &h_A) {
  int *idxs = fpga_tools::toDevice(h_idxs, q);
  auto *A_dram = fpga_tools::toDevice(h_A, q);
  auto *B = fpga_tools::toDevice(h_A, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    DATA_TYPE A[N];
    DATA_TYPE d = DATA_TYPE(0), s = DATA_TYPE(0);

    for (int i = 0; i < N; i++) 
      A[i] = A_dram[i];


    for (int i = 0; i < N; i++) {
      d = A_dram[i] + B[i];
      if (idxs[i] > 0) {
        s += (s * s + DATA_TYPE(0.7));
        A[idxs[i]] = s + d;
      }
    }

    for (int i = 0; i < N; i++) 
      A_dram[i] = A[i];
  });

  event.wait();
  q.copy(A_dram, h_A.data(), h_A.size()).wait();

  sycl::free(idxs, q);
  sycl::free(A_dram, q);
  sycl::free(B, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void filter_sum_cpu(const std::vector<int> &idxs, std::vector<DATA_TYPE> &A) {
  std::vector<DATA_TYPE> B(A.size());
  std::copy(A.begin(), A.end(), B.begin());

  DATA_TYPE d = DATA_TYPE(0), s = DATA_TYPE(0);
    for (int i = 0; i < N; i++) {
      d = A[i] + B[i];
      if (idxs[i] > 0) {
        s += (s * s + DATA_TYPE(0.7));
        A[idxs[i]] = s + d;
      }
    }
}

void init_data(std::vector<int> &idxs, const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(1, 99);
  auto dice = std::bind (distribution, generator);

  for (int i = 0; i < idxs.size(); i++) {
    idxs[i] = (dice() < percentage) ? i : 0;
  }
}

int main(int argc, char *argv[]) {
  int ARRAY_SIZE = 10;
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

    std::vector<int> idxs(ARRAY_SIZE);
    std::vector<DATA_TYPE> h_A(ARRAY_SIZE), cpu_A(ARRAY_SIZE);
    std::fill_n(h_A.begin(), ARRAY_SIZE, DATA_TYPE(0));
    std::fill_n(cpu_A.begin(), ARRAY_SIZE, DATA_TYPE(0));
    init_data(idxs, PERCENTAGE);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    auto kernel_time = filter_sum_kernel(q, idxs, h_A);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    filter_sum_cpu(idxs, cpu_A);

    if (std::equal(h_A.begin(), h_A.end(), cpu_A.begin()))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
