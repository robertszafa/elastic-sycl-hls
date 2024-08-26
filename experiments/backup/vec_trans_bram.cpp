#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <vector>
#include <random>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "memory_utils.hpp"
#include "exception_handler.hpp"

using namespace sycl;
using namespace fpga_tools;

// Forward declare kernel name.
class MainKernel;

constexpr int kN = 1000;

// Setting TEST will ensure test data is transfered from FPGA DRAM to to BRAM
// and back. This adds latency, so leave unset for the benchmarks.
#define TEST 0

double vec_trans_kernel(queue &q, std::vector<int> &h_A, const std::vector<int> &h_b) {

  const int N = h_A.size();

  int *A_dram = fpga_tools::toDevice(h_A, q);
  int *b = fpga_tools::toDevice(h_b, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int A[kN];

    #if TEST
    for (int i=0; i < kN; ++i) 
      A[i] = A_dram[i];
    #endif

    for (int i = 0; i < N; i++) {
      int d = A[i];
      A[b[i]] =
          (((((((d + 112) * d + 23) * d + 36) * d + 82) * d + 127) * d + 2) *
               d +
           20) *
              d +
          100;
    }

    #if TEST
    for (int i=0; i < kN; ++i) 
      A_dram[i] = A[i];
    #endif

  });

  event.wait();
  q.copy(A_dram, h_A.data(), h_A.size()).wait();

  sycl::free(A_dram, q);
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
    A[b[i]] =
        (((((((d + 112) * d + 23) * d + 36) * d + 82) * d + 127) * d + 2) * d +
         20) *
            d +
        100;
  }
}

void init_data(std::vector<int> &A, std::vector<int> &b,  
               const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 99);
  auto dice = std::bind (distribution, generator);

  for (size_t i = 0; i < A.size(); i++) {
    b[i] = (dice() < percentage) ? std::min(i+1, A.size()-1) : i;

    A[i] = i % 50-25;
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

    std::vector<int> A(ARRAY_SIZE);
    std::vector<int> b(ARRAY_SIZE); 

    init_data(A, b,  PERCENTAGE);

    std::vector<int> A_cpu(ARRAY_SIZE);
    std::copy(A.begin(), A.end(), A_cpu.begin());

    auto kernel_time = vec_trans_kernel(q, A, b);

    vec_trans_cpu(A_cpu, b);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";
    
    #if TEST
    if (std::equal(A.begin(), A.end(), A_cpu.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failed\n";
    }
    #endif
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

