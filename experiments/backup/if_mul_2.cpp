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

double if_mul_kernel(queue &q, const std::vector<int> &h_wet,
                     std::vector<float> &h_B) {
  const int array_size = h_wet.size();

  int *wet = fpga_tools::toDevice(h_wet, q);
  float *B = fpga_tools::toDevice(h_B, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    float etan = 0.0, t = 0.0;
    for (int i = 0; i < array_size; ++i) {
      if (wet[i] == 0) {
        t = 0.25 + etan * float(wet[i]) / 2.0;
        etan += t;
        if (etan > 100.0) {
          etan -= 0.1 + etan / 20.0;
        }
      } else if (wet[i] == 1) {
        etan /= 0.02;
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
  float etan = 0.0, t = 0.0;
  for (int i = 0; i < array_size; ++i) {
    if (wet[i] == 0) {
      t = 0.25 + etan * float(wet[i]) / 2.0;
      etan += t;
      if (etan > 100.0) {
        etan -= 0.1 + etan / 20.0;
      }
    } else if (wet[i] == 1) {
      etan /= 0.02;
    }
  }

  B[0] = etan;
}

void init_data(std::vector<int> &wet, std::vector<float> &B,
               const int array_size) {
  for (int i = 0; i < array_size; ++i) {
    if (i % 3 == 0)
      wet[i] = 0;
    else if (i % 3 == 1)
      wet[i] = 1;
    else
      wet[i] = 2;

    B[i] = i;
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

    std::vector<int> wet(ARRAY_SIZE);
    std::vector<float> B(ARRAY_SIZE);
    std::vector<float> B_cpu(ARRAY_SIZE);

    init_data(wet, B, ARRAY_SIZE);
    std::copy(B.begin(), B.end(), B_cpu.begin());

    auto kernel_time = if_mul_kernel(q, wet, B);

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
