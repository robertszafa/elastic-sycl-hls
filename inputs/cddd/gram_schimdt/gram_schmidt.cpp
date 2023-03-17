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

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<float> &a, std::vector<float> &r,
               data_distribution distr, int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind(distribution, generator);

  for (int i = 0; i < a.size(); ++i) {
    a[i] = rand() / 1000.0f;
    r[i] = rand() / 1000.0f;
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

template <class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
almost_equal(T x, T y) {
  std::ostringstream xSS, ySS;
  xSS << x;
  ySS << y;
  if (x == y || xSS.str() == ySS.str())
    return true;
  // the machine epsilon has to be scaled to the magnitude of the values used
  // and multiplied by the desired precision in ULPs (units in the last place)
  return std::fabs(x - y) <=
             std::numeric_limits<T>::epsilon() * std::fabs(x + y) * 2
         // unless the result is subnormal
         || std::fabs(x - y) < std::numeric_limits<T>::min();
}

int main(int argc, char *argv[]) {
  // Get A_SIZE and forward/no-forward from args.
  int N = 1024;
  auto DATA_DISTR = data_distribution::ALL_WAIT;
  int PERCENTAGE = 5;
  try {
    if (argc > 1) {
      N = int(atoi(argv[1]));
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

    std::vector<float> a(N * N);
    std::vector<float> r(N * N);
    std::vector<float> a_cpu(N * N);
    std::vector<float> r_cpu(N * N);

    init_data(a, r, DATA_DISTR, PERCENTAGE);

    std::copy(a.begin(), a.end(), a_cpu.begin());
    std::copy(r.begin(), r.end(), r_cpu.begin());

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = gram_schmidt_kernel(q, a, r, N);

    // Wait for all work to finish.
    q.wait();

    gram_schmidt_cpu(a_cpu, r_cpu, N);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    auto almost_pred = [](auto a, auto b) { return almost_equal(a, b); };
    if (std::equal(a.begin(), a.end(), a_cpu.begin(), almost_pred)) {
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
