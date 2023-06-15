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

double gram_schmidt_kernel(queue &q, std::vector<float> &h_a,
                           std::vector<float> &h_r, const int N) {
  auto *a = fpga_tools::toDevice(h_a, q);
  auto *r = fpga_tools::toDevice(h_r, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
  float tol = 0.1f;
  for (int i = 0; i < N; i++) {
    float sum = 0.0f;
    for (int j = 0; j < N; j++) {
      sum += a[i * N + j] * a[i * N + j];
    }
    sum -= 4.0f;
    sum = 0.0019f * ((sum - 8.0f) * sum + 16.0f) * sum + 2.0f;
    r[i * N + i] = sum;

    if (sum > tol) { // a_i = a_i/r_ii
      for (int j = 0; j < N; j++)
        a[i * N + j] = a[i * N + j] / sum;
    } else if (i == 0) { // set a[0] = [1 0 0 ... 0]^T
      for (int j = 0; j < N; j++)
        a[i * N + j] = (j == 0) ? 1.0f : 0.0f;
    } else { // need to choose a_i orthogonal to < a_1, ... a_{i-1} >
      for (int j = 0; j < N; j++)
        a[i * N + j] = -a[0 * N + i] * a[0 * N + j];

      a[i * N + i] += 1.0f;
      for (int j = 1; j < N; j++) {
        float d = a[j * N + i];
        for (int k = 0; k < N; k++)
          a[i * N + k] -= a[j * N + k] * d;
      }
      
      float anorm = 0.0f;
      for (int j = 0; j < N; j++)
        anorm += a[i * N + j] * a[i * N + j];
      for (int j = 0; j < N; j++)
        a[0 * N + i] = a[0 * N + i] / anorm;
    }

    for (int j = i + 1; j < N; j++) {
      float sum = 0.0f;
      for (int k = 0; k < N; k++)
        sum += a[i * N + k] * a[j * N + k]; // r_ij = a_i*a_j
      for (int k = 0; k < N; k++)
        a[j * N + k] -= a[i * N + k] * sum; // a_j -= r_ij a_i
      r[j * N + i] = sum;
    }
  }
  });

  event.wait();
  q.copy(r, h_r.data(), h_r.size()).wait();
  q.copy(a, h_a.data(), h_a.size()).wait();

  sycl::free((void *)a, q);
  sycl::free((void *)r, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void gram_schmidt_cpu(std::vector<float> &a, std::vector<float> &r,
                      const int N) {
  float tol = 0.1f;
  for (int i = 0; i < N; i++) {
    float sum = 0.0f;
    for (int j = 0; j < N; j++) {
      sum += a[i * N + j] * a[i * N + j];
    }
    sum -= 4.0f;
    sum = 0.0019f * ((sum - 8.0f) * sum + 16.0f) * sum + 2.0f;
    r[i * N + i] = sum;

    if (sum > tol) { // a_i = a_i/r_ii
      for (int j = 0; j < N; j++)
        a[i * N + j] = a[i * N + j] / sum;
    } else if (i == 0) { // set a[0] = [1 0 0 ... 0]^T
      for (int j = 0; j < N; j++)
        a[i * N + j] = (j == 0) ? 1.0f : 0.0f;
    } else { // need to choose a_i orthogonal to < a_1, ... a_{i-1} >
      for (int j = 0; j < N; j++)
        a[i * N + j] = -a[0 * N + i] * a[0 * N + j];

      a[i * N + i] += 1.0f;
      for (int j = 1; j < N; j++) {
        float d = a[j * N + i];
        for (int k = 0; k < N; k++)
          a[i * N + k] -= a[j * N + k] * d;
      }

      float anorm = 0.0f;
      for (int j = 0; j < N; j++)
        anorm += a[i * N + j] * a[i * N + j];
      for (int j = 0; j < N; j++)
        a[0 * N + i] = a[0 * N + i] / anorm;
    }

    for (int j = i + 1; j < N; j++) {
      float sum = 0.0f;
      for (int k = 0; k < N; k++)
        sum += a[i * N + k] * a[j * N + k]; // r_ij = a_i*a_j
      for (int k = 0; k < N; k++)
        a[j * N + k] -= a[i * N + k] * sum; // a_j -= r_ij a_i
      r[j * N + i] = sum;
    }
  }
}


void init_data(std::vector<float> &a, std::vector<float> &r,
                int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind(distribution, generator);

  for (int i = 0; i < a.size(); ++i) {
    a[i] = rand() / 1000.0f;
    r[i] = rand() / 1000.0f;
  }
}


int main(int argc, char *argv[]) {
  int N = 1000;
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

    std::vector<float> a(N * N);
    std::vector<float> r(N * N);
    std::vector<float> a_cpu(N * N);
    std::vector<float> r_cpu(N * N);

    init_data(a, r,  PERCENTAGE);

    std::copy(a.begin(), a.end(), a_cpu.begin());
    std::copy(r.begin(), r.end(), r_cpu.begin());

    auto kernel_time = gram_schmidt_kernel(q, a, r, N);

    gram_schmidt_cpu(a_cpu, r_cpu, N);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(a.begin(), a.end(), a_cpu.begin())) {
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
