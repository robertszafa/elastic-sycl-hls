#include <algorithm>
#include <iostream>
#include <limits>
#include <math.h>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <sycl/sycl.hpp>
#include <vector>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "exception_handler.hpp"
#include "memory_utils.hpp"
#include "device_print.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double gemver_kernel(queue &q, std::vector<float> &h_a, std::vector<float> &h_x,
                   std::vector<float> &h_y, std::vector<float> &h_z,
                   std::vector<float> &h_w, std::vector<float> &h_u1,
                   std::vector<float> &h_v1, std::vector<float> &h_u2,
                   std::vector<float> &h_v2, const float alpha,
                   const float beta, const uint N) {
  auto *a = fpga_tools::toDevice(h_a, q);
  auto *x = fpga_tools::toDevice(h_x, q);
  auto *y = fpga_tools::toDevice(h_y, q);
  auto *z = fpga_tools::toDevice(h_z, q);
  auto *w = fpga_tools::toDevice(h_w, q);

  auto *u1 = fpga_tools::toDevice(h_u1, q);
  auto *u2 = fpga_tools::toDevice(h_u2, q);
  auto *v1 = fpga_tools::toDevice(h_v1, q);
  auto *v2 = fpga_tools::toDevice(h_v2, q);

  std::vector<std::pair<sycl::event, bool>> events;

  auto main_event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (unsigned i = 0; i < N; i++)
      for (unsigned j = 0; j < N; j++)
        a[i*N + j] = a[i*N + j] + u1[i] * v1[j] + u2[i] * v2[j];

    for (unsigned i = 0; i < N; i++) {
      int tmp = x[i];
      for (unsigned j = 0; j < N; j++)
        tmp += beta * a[j*N + i] * y[j];
      x[i] = tmp;
    }

    for (unsigned i = 0; i < N; i++)
      x[i] = x[i] + z[i];

    for (unsigned i = 0; i < N; i++) {
      int tmp = w[i];
      for (unsigned j = 0; j < N; j++) {
        tmp += alpha * a[i*N + j] * x[j];
      }
      w[i] = tmp;
    }
  });

  
  events.push_back({main_event, true});
  for (auto &kv : events) kv.first.wait();

  q.copy(a, h_a.data(), h_a.size()).wait();
  q.copy(x, h_x.data(), h_x.size()).wait();
  q.copy(w, h_w.data(), h_w.size()).wait();

  sycl::free(a, q);
  sycl::free(x, q);
  sycl::free(y, q);
  sycl::free(z, q);
  sycl::free(w, q);
  sycl::free(u1, q);
  sycl::free(u2, q);
  sycl::free(v1, q);
  sycl::free(v2, q);

  double max_event_time = 0;
  for (auto &[e, toMeasure] : events) {
    if (toMeasure) {
      auto start = e.get_profiling_info<info::event_profiling::command_start>();
      auto end = e.get_profiling_info<info::event_profiling::command_end>();
      double this_event_time = static_cast<double>(end - start) / 1000000;
      max_event_time = std::max(max_event_time, this_event_time);
    }
  }

  return max_event_time;
}

void gemver_cpu(std::vector<float> &a, std::vector<float> &x,
              std::vector<float> &y, std::vector<float> &z,
              std::vector<float> &w, std::vector<float> &u1,
              std::vector<float> &v1, std::vector<float> &u2,
              std::vector<float> &v2, const float alpha, const float beta,
              const uint N) {
  for (unsigned i = 0; i < N; i++)
    for (unsigned j = 0; j < N; j++)
      a[i * N + j] = a[i * N + j] + u1[i] * v1[j] + u2[i] * v2[j];

  for (unsigned i = 0; i < N; i++) {
    int tmp = x[i];
    for (unsigned j = 0; j < N; j++)
      tmp += beta * a[j * N + i] * y[j];
    x[i] = tmp;
  }

  for (unsigned i = 0; i < N; i++)
    x[i] = x[i] + z[i];

  for (unsigned i = 0; i < N; i++) {
    int tmp = w[i];
    for (unsigned j = 0; j < N; j++) {
      tmp += alpha * a[i * N + j] * x[j];
    }
    w[i] = tmp;
  }
}

int main(int argc, char *argv[]) {
  int N = 10;
  try {
    if (argc > 1) {
      N = int(atoi(argv[1]));
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
#else // #if FPGA_EMULATOR
  auto d_selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif

  try {
    // Enable profiling.
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<float> a(N*N, 1.0f);
    std::vector<float> x(N, 1.0f);
    std::vector<float> y(N, 1.0f);
    std::vector<float> z(N, 1.0f);
    std::vector<float> w(N, 1.0f);
    std::vector<float> u1(N, 1.0f);
    std::vector<float> v1(N, 1.0f);
    std::vector<float> u2(N, 1.0f);
    std::vector<float> v2(N, 1.0f);
    const float alpha = 1.0f;
    const float beta = 1.0f;

    auto kernel_time =
        gemver_kernel(q, a, x, y, z, w, u1, v1, u2, v2, alpha, beta, N);

    std::vector<float> a_cpu(N*N, 1.0f);
    std::vector<float> x_cpu(N, 1.0f);
    std::vector<float> y_cpu(N, 1.0f);
    std::vector<float> z_cpu(N, 1.0f);
    std::vector<float> w_cpu(N, 1.0f);
    std::vector<float> u1_cpu(N, 1.0f);
    std::vector<float> v1_cpu(N, 1.0f);
    std::vector<float> u2_cpu(N, 1.0f);
    std::vector<float> v2_cpu(N, 1.0f);
    gemver_cpu(a_cpu, x_cpu, y_cpu, z_cpu, w_cpu, u1_cpu, v1_cpu, u2_cpu,
               v2_cpu, alpha, beta, N);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(a.begin(), a.end(), a_cpu.begin()) &&
        std::equal(x.begin(), x.end(), x_cpu.begin()) &&
        std::equal(w.begin(), w.end(), w_cpu.begin())) {
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
