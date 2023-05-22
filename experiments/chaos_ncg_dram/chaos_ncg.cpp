#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <vector>
#include <random>

#include <sycl/ext/intel/fpga_extensions.hpp>


#include "memory_utils.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double chaos_ncg_kernel(queue &q, int I, int bo, int X, int Y, int params0, int params1, 
                        const std::vector<int> &h_M, std::vector<int> &h_buffer) {
  int *M = fpga_tools::toDevice(h_M, q);
  int *buffer = fpga_tools::toDevice(h_buffer, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int bound = 2 * bo;
    int p0 = params0;
    int p1 = params1;

    for (int i = 0; i < bound; i += 2) {
      int a = M[I + i - 2], b = M[I + i + 2];
      int b0 = buffer[a], b1 = buffer[b];
      p0 ^= b0, p1 ^= b1;
      p1 ^= (Y << (p0 % 17) | Y >> (16 - p0 % 17));
      p0 ^= p1;
      p1 += (X & p0);
      int out0 = b0 - p1;
      p0 += (X << (p1 % 17) | X >> (16 - p1 % 17));
      p1 += p0;
      p0 += (Y & p1);
      int out1 = b1 + p0;
      int pp0 = p0;
      int pp1 = p1;
      buffer[a] = out0;
      p0 = pp0;
      p1 = pp1;
      buffer[b] = out1;
    }
  });

  event.wait();
  q.copy(buffer, h_buffer.data(), h_buffer.size()).wait();

  sycl::free(buffer, q);
  sycl::free(M, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}


enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };

void chaos_ncg_cpu(int I, int bo, int X, int Y, int params0, int params1, 
                   const std::vector<int> &M, std::vector<int> &buffer) {
  int bound = 2 * bo;
  int p0 = params0;
  int p1 = params1;

  for (int i = 0; i < bound; i += 2) {
    int a = M[I + i - 2], b = M[I + i + 2];
    int b0 = buffer[a], b1 = buffer[b];
    p0 ^= b0, p1 ^= b1;
    p1 ^= (Y << (p0 % 17) | Y >> (16 - p0 % 17));
    p0 ^= p1;
    p1 += (X & p0);
    int out0 = b0 - p1;
    p0 += (X << (p1 % 17) | X >> (16 - p1 % 17));
    p1 += p0;
    p0 += (Y & p1);
    int out1 = b1 + p0;
    int pp0 = p0;
    int pp1 = p1;
    buffer[a] = out0;
    p0 = pp0;
    p1 = pp1;
    buffer[b] = out1;
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
  // defaulats
  uint BO = 100;
  auto DATA_DISTR = data_distribution::ALL_WAIT;
  uint PERCENTAGE = 5;
  try {
    if (argc > 1) {
      BO = uint(atoi(argv[1]));
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

    std::vector<int> M(BO*3);
    std::vector<int> buffer(BO*3);

    // init_data - no easy way to control ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT
    int I = 2, X = 250, Y = 250;
    for (int i = 0; i < BO*3; i++) {
      if (DATA_DISTR == data_distribution::NO_WAIT)
        M[i] = i % BO;
      else
        M[i] = i % 4;

      buffer[i] = rand() % BO;
    }
    // init_data

    std::vector<int> buffer_cpu(BO*3);
    std::copy(buffer.begin(), buffer.end(), buffer_cpu.begin());

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = chaos_ncg_kernel(q, I, BO, X, Y, 127, 41, M, buffer);

    // Wait for all work to finish.
    q.wait();

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    chaos_ncg_cpu(I, BO, X, Y, 127, 41, M, buffer_cpu);
    if (std::equal(buffer.data(), buffer.data() + BO*2, buffer_cpu.data())) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failed";
      std::cout << " sum(fpga) = " << std::accumulate(buffer.data(), buffer.data() + BO*2, 0) << "\n";
      std::cout << " sum(cpu) = " << std::accumulate(buffer_cpu.data(), buffer_cpu.data() + BO*2, 0) << "\n";
    }
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

