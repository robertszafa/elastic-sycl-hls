#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <vector>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "memory_utils.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double if_mul_kernel(queue &q, const std::vector<int> &h_wet,
                     std::vector<float> &h_B) {
  const int array_size = h_wet.size();

  int *wet = fpga_tools::toDevice(h_wet, q);
  float *B = fpga_tools::toDevice(h_B, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    float etan = 0.0, t = 0.0;
    // II=35
    for (int i = 0; i < array_size; ++i) {
      if (wet[i] > 0) {
        // 35 cycles of stall
        t = 0.25 + etan * float(wet[i]) / 2.0;
        etan += t;
      }
    }

    B[0] = etan;
  });

  event.wait();
  q.copy(B, h_B.data(), h_B.size()).wait();

  sycl::free(B, q);
  sycl::free(wet, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void if_mul_cpu(const std::vector<int> &wet, std::vector<float> &B) {
  const int array_size = wet.size();
  float etan, t = 0.0;
  // II=35
  for (int i = 0; i < array_size; ++i) {
    if (wet[i] > 0) {
      // 35 cycles of stall
      t = 0.25 + etan * float(wet[i]) / 2.0;
      etan += t;
    }
  }

  B[0] = etan;
}

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<int> &wet, std::vector<float> &B,
               const int array_size, const data_distribution distr,
               const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind (distribution, generator);

  for (int i = 0; i < array_size; ++i) {
    if (distr == data_distribution::ALL_WAIT) 
      wet[i] = 1;
    else if (distr == data_distribution::NO_WAIT) 
      wet[i] = 0;
    else 
      wet[i] = (dice() <= percentage) ? 1 : 0;

    B[i] = i;
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
  int ARRAY_SIZE = 1024;
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
    std::cout << "  ./executable [ARRAY_SIZE] [data_distribution (0/1/2)] "
                 "[PERCENTAGE (only for "
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
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<int> wet(ARRAY_SIZE);
    std::vector<float> B(ARRAY_SIZE);
    std::vector<float> B_cpu(ARRAY_SIZE);

    init_data(wet, B, ARRAY_SIZE, DATA_DISTR, PERCENTAGE);
    std::copy(B.begin(), B.end(), B_cpu.begin());

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = if_mul_kernel(q, wet, B);

    // Wait for all work to finish.
    q.wait();

    if_mul_cpu(wet, B_cpu);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(B.begin(), B.end(), B_cpu.begin())) {
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
