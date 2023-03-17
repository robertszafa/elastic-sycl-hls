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

double doitgen_triple_kernel(queue &q, std::vector<float> &h_A,
                             std::vector<float> &h_sum,
                             const std::vector<float> &h_w) {
  auto *A = fpga_tools::toDevice(h_A, q);
  auto *sum = fpga_tools::toDevice(h_sum, q);
  auto *w = fpga_tools::toDevice(h_w, q);

  const int N = h_A.size();

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < N; i++) {
      float s = 0;
      for (int j = 0; j < N; j++) {
        float a = A[j];
        float wt = w[i * N + j];
        if (a > 0.0) {
          s += (a * wt + wt) * a;
        }
      }
      sum[i] = s;
    }
    
    // Either implement decoupling whole loops, or comment this loop out.
    for (int i = 0; i < N - 1; i++) {
      float q = sum[i];
      A[i + 1] = A[i] + q * q * q;
    }
  });

  event.wait();
  q.copy(sum, h_sum.data(), h_sum.size()).wait();

  sycl::free((void *)A, q);
  sycl::free((void *)sum, q);
  sycl::free((void *)w, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void doitgen_triple_cpu(std::vector<float> &A, std::vector<float> &sum,
                        const std::vector<float> &w) {
  const int N = A.size();

  for (int i = 0; i < N; i++) {
    float s = 0;
    for (int j = 0; j < N; j++) {
      float a = A[j];
      float wt = w[i * N + j];
      if (a > 0.0) {
        s += (a * wt + wt) * a;
      }
    }
    sum[i] = s;
  }
  for (int i = 0; i < N - 1; i++) {
    float q = sum[i];
    A[i + 1] = A[i] + q * q * q;
  }
}

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<float> &A, std::vector<float> &sum, std::vector<float> &w,
               data_distribution distr, int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind(distribution, generator);

  for (int i = 0; i < A.size(); ++i) {
    if (distr == data_distribution::ALL_WAIT) {
      A[i] = 1.0;
    } else if (distr == data_distribution::NO_WAIT) {
      A[i] = -1;
    } else {
      A[i] = (dice() <= percentage) ? 1 : 0;
    }

    sum[i] = 0.0;
    w[i] = rand();
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

    std::vector<float> A(N);
    std::vector<float> sum(N);
    std::vector<float> w(N*N);
    std::vector<float> A_cpu(N);
    std::vector<float> sum_cpu(N);

    init_data(A, sum, w, DATA_DISTR, PERCENTAGE);

    std::copy(A.begin(), A.end(), A_cpu.begin());
    std::copy(sum.begin(), sum.end(), sum_cpu.begin());

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = doitgen_triple_kernel(q, A, sum, w);

    // Wait for all work to finish.
    q.wait();

    doitgen_triple_cpu(A_cpu, sum_cpu, w);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    auto almost_pred = [] (auto a, auto b) { return almost_equal(a, b); };
    if (std::equal(sum.begin(), sum.end(), sum_cpu.begin(), almost_pred)) {
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
