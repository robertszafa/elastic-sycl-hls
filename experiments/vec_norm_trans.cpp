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

double vec_norm_trans_kernel(queue &q, const std::vector<float> &h_a,
                             std::vector<float> &h_r, const int N) {
  float *a = fpga_tools::toDevice(h_a, q);
  float *r = fpga_tools::toDevice(h_r, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    float weight = 0.0f;
    for (int i = 0; i < N; i++) {
      float d = a[i];
      if (d < 1.0f)
        weight = ((d * d + 19.52381f) * d + 3.704762f) * d + 0.73f * weight;
    }

    for (int i = 0; i < N; i++) {
      float d = a[i] / weight;
      r[i] = r[i] + d;
    }
  });

  event.wait();
  q.copy(r, h_r.data(), h_r.size()).wait();

  sycl::free(a, q);
  sycl::free(r, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void vec_norm_trans_cpu(const std::vector<float> &a, std::vector<float> &r,
                        const int N) {
  float weight = 0.0f;
  for (int i = 0; i < N; i++) {
    float d = a[i];
    if (d < 1.0f)
      weight = ((d * d + 19.52381f) * d + 3.704762f) * d + 0.73f * weight;
  }

  for (int i = 0; i < N; i++) {
    float d = a[i] / weight;
    r[i] = r[i] + d;
  }
}


void init_data(std::vector<float> &a, std::vector<float> &r,
               const int array_size, 
               const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 99);
  auto dice = std::bind(distribution, generator);

  for (int i = 0; i < array_size; ++i) {
    a[i] = (dice() < percentage) ? 0.9f : 1.1f;

    r[i] = 0;
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

    std::vector<float> a(ARRAY_SIZE);
    std::vector<float> r(ARRAY_SIZE);
    std::vector<float> r_cpu(ARRAY_SIZE);

    init_data(a, r, ARRAY_SIZE,  PERCENTAGE);
    std::copy(r.begin(), r.end(), r_cpu.begin());

    auto kernel_time = vec_norm_trans_kernel(q, a, r, ARRAY_SIZE);

    vec_norm_trans_cpu(a, r_cpu, ARRAY_SIZE);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(r.begin(), r.end(), r_cpu.begin())) {
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
