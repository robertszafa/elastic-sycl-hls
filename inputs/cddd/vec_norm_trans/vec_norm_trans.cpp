#include <CL/sycl.hpp>
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

double vec_norm_trans_kernel(queue &q, const std::vector<float> h_a,
                             std::vector<float> h_r, const int N) {
  const float *a = fpga_tools::toDevice(h_a, q);
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

  sycl::free(r, q);
  sycl::free((void *)a, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void vec_norm_trans_cpu(const std::vector<float> a, std::vector<float> r,
                        const int N) {
  float weight = 0.0f;
  for (int i = 0; i < N; i++) {
    float d = a[i];
    if (d < 1.0f)
      weight = ((d * d + 19.52381f) * d + 3.704762f) * d + 0.73f * weight;
  }

  for (int i = 0; i < N; i++) {
    float d = a[i] / weight;
    r[i + 4] = r[i] + d;
  }
}

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<float> &a, std::vector<float> &r,
               const int array_size, data_distribution distr,
               const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind(distribution, generator);

  for (int i = 0; i < array_size; ++i) {
    if (distr == data_distribution::ALL_WAIT) {
      a[i] = 0.9f;
    } else if (distr == data_distribution::NO_WAIT) {
      a[i] = 1.1f;
    } else {
      a[i] = (dice() <= percentage) ? 0.9f : 1.1f;
    }

    r[i] = 0;
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

#if FPGA_EMULATOR
  ext::intel::fpga_emulator_selector d_selector;
#elif FPGA
  ext::intel::fpga_selector d_selector;
#else
  default_selector d_selector;
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

    init_data(a, r, ARRAY_SIZE, DATA_DISTR, PERCENTAGE);
    std::copy(r.begin(), r.end(), r_cpu.begin());

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = vec_norm_trans_kernel(q, a, r, ARRAY_SIZE);

    // Wait for all work to finish.
    q.wait();

    vec_norm_trans_cpu(a, r, ARRAY_SIZE);

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
