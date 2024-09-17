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

// No need to test if result correct since this kernel will not be transformed,
// because the static scheduler achieves II=1.
double correlation_kernel(queue &q, const int M, const int N,
                          std::vector<float> &dataHost,
                          std::vector<float> &corrHost,
                          std::vector<float> &meanHost,
                          std::vector<float> &stddevHost) {

  auto *data = fpga_tools::toDevice(dataHost, q);
  auto *corr = fpga_tools::toDevice(corrHost, q);
  auto *mean = fpga_tools::toDevice(meanHost, q);
  auto *stddev = fpga_tools::toDevice(stddevHost, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    const float eps = 0.1f;
    const float float_n = float(N);

    for (int i = 0; i < N; i++) {
      float res = 0.0f;
      for (int j = 0; j < M; j++) {
        res += data[i * N + j];
      }
      res /= float_n;
      
      mean[i] = res;
    }

    for (int i = 0; i < N; i++) {
      auto mean_i = mean[i];
      float res = 0.0f;
      for (int j = 0; j < M; j++) {
        auto tmp = data[i * N + j] - mean_i;
        res += (tmp) * (tmp);
      }
      res /= float_n;
      res = sqrtf(res);
      res = res <= eps ? 1.0f : res;

      stddev[i] = res;
    }

    /* Center and reduce the column vectors. */
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
        auto res = data[i * N + j] - mean[i];
        res /= float_n * stddev[i];
        data[i * N + j] = res;
      }
    }

    /* Calculate the m * m correlation matrix. */
    for (int i = 0; i < M - 1; i++) {
      for (int j = i + 1; j < M; j++) {
        float res = 0.0f;
        for (int k = 0; k < N; k++) {
          res += (data[i * N + k] * data[i * N + k]);
        }

        corr[i * N + j] = res;
      }
    }
  });

  event.wait();

  q.copy(corr, corrHost.data(), corrHost.size()).wait();
  q.copy(data, dataHost.data(), dataHost.size()).wait();
  q.copy(mean, meanHost.data(), meanHost.size()).wait();
  q.copy(stddev, stddevHost.data(), stddevHost.size()).wait();

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void correlation_cpu(const int M, const int N, std::vector<float> &data,
                     std::vector<float> &corr, std::vector<float> &mean,
                     std::vector<float> &stddev) {
  const float eps = 0.1f;
  const float float_n = float(N);

  for (int i = 0; i < N; i++) {
    float res = 0.0f;
    for (int j = 0; j < M; j++) {
      res += data[i * N + j];
    }
    res /= float_n;

    mean[i] = res;
  }

  for (int i = 0; i < N; i++) {
    auto mean_i = mean[i];
    float res = 0.0f;
    for (int j = 0; j < M; j++) {
      auto tmp = data[i * N + j] - mean_i;
      res += (tmp) * (tmp);
    }
    res /= float_n;
    res = sqrtf(res);
    res = res <= eps ? 1.0f : res;

    stddev[i] = res;
  }

  /* Center and reduce the column vectors. */
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      auto res = data[i * N + j] - mean[i];
      res /= float_n * stddev[i];
      data[i * N + j] = res;
    }
  }

  /* Calculate the m * m correlation matrix. */
  for (int i = 0; i < M - 1; i++) {
    corr[i * N + i] = 1.0f;

    for (int j = i + 1; j < M; j++) {
      float res = 0.0f;
      for (int k = 0; k < N; k++) {
        res += (data[i * N + k] * data[i * N + k]);
      }

      corr[i * N + j] = res;
    }
  }
}

int main(int argc, char *argv[]) {
  int M = 10;
  int N = 10;
  try {
    if (argc > 1) {
      M = int(atoi(argv[1]));
    }
    if (argc > 2) {
      N = int(atoi(argv[2]));
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

    std::vector<float> data(M*N, 1), dataCpu(M*N, 1);
    std::vector<float> corr(M*N, 1), corrCpu(M*N, 1);
    std::vector<float> mean(M, 1), meanCpu(M, 1);
    std::vector<float> stddev(M*N, 1), stddevCpu(M*N, 1);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    auto kernel_time = correlation_kernel(q, M, N, data, corr, mean, stddev);

    correlation_cpu(M, N, dataCpu, corrCpu, meanCpu, stddevCpu);

    bool dataEqual = std::equal(data.begin(), data.end(), dataCpu.begin());
    bool corrEqual = std::equal(corr.begin(), corr.end(), corrCpu.begin());
    bool meanEqual = std::equal(mean.begin(), mean.end(), meanCpu.begin());
    bool stddevEqual =
        std::equal(stddev.begin(), stddev.end(), stddevCpu.begin());

    if (dataEqual && corrEqual && meanEqual && stddevEqual)
      std::cout << "Passed\n";
    else
     std::cout << "Failed\n";

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
