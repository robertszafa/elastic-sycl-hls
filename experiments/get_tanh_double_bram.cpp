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

// Forward declare kernel name.
class MainKernel;

constexpr int kN = 1000;

using DATA_TYPE = int;

double get_tanh_double_kernel(queue &q, std::vector<DATA_TYPE> &h_A,
                              const std::vector<int> h_addr_in,
                              const std::vector<int> h_addr_out) {
  const uint array_size = h_A.size();

  auto* A_dram = fpga_tools::toDevice(h_A, q);
  int* addr_in = fpga_tools::toDevice(h_addr_in, q);
  int* addr_out = fpga_tools::toDevice(h_addr_out, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    DATA_TYPE A[kN];

#ifdef TEST
    for (int i = 0; i < kN; i++)
      A[i] = A_dram[i];
#endif

    for (int i = 0; i < array_size; i++) {
      auto beta = A[addr_in[i]];

      auto result = ((beta * beta + 19) * beta * beta + 3) * beta;

      A[addr_out[i]] = result;
    }

#ifdef TEST
    for (int i = 0; i < kN; i++)
      A_dram[i] = A[i];
#endif
  });

  event.wait();
  q.copy(A_dram, h_A.data(), h_A.size()).wait();

  sycl::free(A_dram, q);
  sycl::free(addr_in, q);
  sycl::free(addr_out, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void init_data(std::vector<DATA_TYPE> &A, std::vector<int> &addr_in,
               std::vector<int> &addr_out, const uint percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 99);
  auto dice = std::bind(distribution, generator);

  addr_in[0] = 0;
  addr_in[1] = 1;
  for (int i = 0; i < A.size(); i++) {
    A[i] = (i % 1000 == 0) ? 1 : 0;

    addr_in[i] = (dice() < percentage) ? addr_in[std::max(i - 1, 0)] : i;
    addr_out[i] = addr_in[i];
  }
}

void get_tanh_double_cpu(std::vector<DATA_TYPE> &A, const std::vector<int> addr_in,
                  const std::vector<int> addr_out) {
  int i;
  DATA_TYPE result, is_neg, beta;

  for (i = 0; i < A.size(); i++) {
    beta = A[addr_in[i]];

    result = ((beta*beta+19)*beta*beta+3)*beta;

    A[addr_out[i]] = result;
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
    std::cout << "Running on device: " << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<DATA_TYPE> A(ARRAY_SIZE);
    std::vector<int> addr_in(ARRAY_SIZE);
    std::vector<int> addr_out(ARRAY_SIZE);

    init_data(A, addr_in, addr_out,  PERCENTAGE);

    std::vector<DATA_TYPE> A_cpu(ARRAY_SIZE);
    std::copy(A.begin(), A.end(), A_cpu.begin());

    auto kernel_time = get_tanh_double_kernel(q, A, addr_in, addr_out);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

#ifdef TEST
    get_tanh_double_cpu(A_cpu, addr_in, addr_out);
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
