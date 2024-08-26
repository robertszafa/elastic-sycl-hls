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

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double adi_kernel(queue &sycl_q, std::vector<float> &h_u, std::vector<float> &h_v,
                     std::vector<float> &h_p, std::vector<float> &h_q,
                     const int N, const int TSTEPS) {
  auto *u = fpga_tools::toDevice(h_u, sycl_q);
  auto *v = fpga_tools::toDevice(h_v, sycl_q);
  auto *p = fpga_tools::toDevice(h_p, sycl_q);
  auto *q = fpga_tools::toDevice(h_q, sycl_q);

  auto event = sycl_q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    float DX = 1.0f / (float)N;
    float DY = 1.0f / (float)N;
    float DT = 1.0f / (float)TSTEPS;
    float B1 = 2.0f;
    float B2 = 1.0f;
    float mul1 = B1 * DT / (DX * DX);
    float mul2 = B2 * DT / (DY * DY);

    float a = -mul1 / 2.0f;
    float b = 1.0f + mul1;
    float c = a;
    float d = -mul2 / 2.0f;
    float e = 1.0f + mul2;
    float f = d;

    for (int t = 1; t <= TSTEPS; t++) {

      // Column Sweep
      for (int i = 1; i < N - 1; i++) {
        v[0 * N + i] = 1.0f;
        p[i * N + 0] = 0.0f;
        q[i * N + 0] = v[0 * N + i];

        for (int j = 1; j < N - 1; j++) {
          p[i * N + j] = -c / (a * p[i * N + j - 1] + b);
          q[i * N + j] =
              (-d * u[j * N + i - 1] + (1.0f + 2.0f * d) * u[j * N + i] -
               f * u[j * N + i + 1] - a * q[i * N + j - 1]) /
              (a * p[i * N + j - 1] + b);
        }

        v[N - 1 * N + i] = 1.0f;
        for (int j = N - 2; j >= 1; j--) {
          v[j * N + i] = p[i * N + j] * v[j + 1 * N + i] + q[i * N + j];
        }
      }


      // Row Sweep
      for (int i = 1; i < N - 1; i++) {
        u[i * N + 0] = 1.0f;
        p[i * N + 0] = 0.0f;
        q[i * N + 0] = u[i * N + 0];

        for (int j = 1; j < N - 1; j++) {
          p[i * N + j] = -f / (d * p[i * N + j - 1] + e);
          q[i * N + j] =
              (-a * v[i - 1 * N + j] + (1.0f + 2.0f * a) * v[i * N + j] -
               c * v[i + 1 * N + j] - d * q[i * N + j - 1]) /
              (d * p[i * N + j - 1] + e);
        }

        u[i * N + N - 1] = 1.0f;
        for (int j = N - 2; j >= 1; j--) {
          u[i * N + j] = p[i * N + j] * u[i * N + j + 1] + q[i * N + j];
        }
      }
    }
  });

  event.wait();
  sycl_q.copy(u, h_u.data(), h_u.size()).wait();
  sycl_q.copy(v, h_v.data(), h_v.size()).wait();
  sycl_q.copy(p, h_p.data(), h_p.size()).wait();
  sycl_q.copy(q, h_q.data(), h_q.size()).wait();

  sycl::free(u, sycl_q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void adi_cpu(std::vector<float> &u, std::vector<float> &v,
             std::vector<float> &p, std::vector<float> &q, const int N,
             const int TSTEPS) {
  float DX = 1.0f / (float)N;
  float DY = 1.0f / (float)N;
  float DT = 1.0f / (float)TSTEPS;
  float B1 = 2.0f;
  float B2 = 1.0f;
  float mul1 = B1 * DT / (DX * DX);
  float mul2 = B2 * DT / (DY * DY);

  float a = -mul1 / 2.0f;
  float b = 1.0f + mul1;
  float c = a;
  float d = -mul2 / 2.0f;
  float e = 1.0f + mul2;
  float f = d;

  for (int t = 1; t <= TSTEPS; t++) {

    // Column Sweep
    for (int i = 1; i < N - 1; i++) {
      v[0 * N + i] = 1.0f;
      p[i * N + 0] = 0.0f;
      q[i * N + 0] = v[0 * N + i];

      for (int j = 1; j < N - 1; j++) {
        p[i * N + j] = -c / (a * p[i * N + j - 1] + b);
        q[i * N + j] =
            (-d * u[j * N + i - 1] + (1.0f + 2.0f * d) * u[j * N + i] -
             f * u[j * N + i + 1] - a * q[i * N + j - 1]) /
            (a * p[i * N + j - 1] + b);
      }

      v[N - 1 * N + i] = 1.0f;
      for (int j = N - 2; j >= 1; j--) {
        v[j * N + i] = p[i * N + j] * v[j + 1 * N + i] + q[i * N + j];
      }
    }

    // Row Sweep
    for (int i = 1; i < N - 1; i++) {
      u[i * N + 0] = 1.0f;
      p[i * N + 0] = 0.0f;
      q[i * N + 0] = u[i * N + 0];

      for (int j = 1; j < N - 1; j++) {
        p[i * N + j] = -f / (d * p[i * N + j - 1] + e);
        q[i * N + j] =
            (-a * v[i - 1 * N + j] + (1.0f + 2.0f * a) * v[i * N + j] -
             c * v[i + 1 * N + j] - d * q[i * N + j - 1]) /
            (d * p[i * N + j - 1] + e);
      }

      u[i * N + N - 1] = 1.0f;
      for (int j = N - 2; j >= 1; j--) {
        u[i * N + j] = p[i * N + j] * u[i * N + j + 1] + q[i * N + j];
      }
    }
  }
}

int main(int argc, char *argv[]) {
  int N = 10;
  int TSTEPS = 2;
  try {
    if (argc > 1) {
      N = int(atoi(argv[1]));
    }
    if (argc > 2) {
      TSTEPS = int(atoi(argv[2]));
    }
  } catch (exception const &e) {
    std::cout << "Incorrect argv.\nUsage:\n"
              << "  ./executable [ARRAY_SIZE] [TSTEPS]\n";
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
    queue sycl_q(d_selector, exception_handler, properties);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << sycl_q.get_device().get_info<info::device::name>() << "\n";

    std::vector<float> u(N*N, 1.0f), u_cpu(N*N, 1.0f);
    std::vector<float> v(N*N, 1.0f), v_cpu(N*N, 1.0f);
    std::vector<float> p(N*N, 1.0f), p_cpu(N*N, 1.0f);
    std::vector<float> q(N*N, 1.0f), q_cpu(N*N, 1.0f);

    auto kernel_time = adi_kernel(sycl_q, u, v, p, q, N, TSTEPS);
    adi_cpu(u_cpu, v_cpu, p_cpu, q_cpu, N, TSTEPS);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    bool u_equal = std::equal(u.begin(), u.end(), u_cpu.begin());
    bool v_equal = std::equal(v.begin(), v.end(), v_cpu.begin());
    bool p_equal = std::equal(p.begin(), p.end(), p_cpu.begin());
    bool q_equal = std::equal(q.begin(), q.end(), q_cpu.begin());

    if (u_equal && v_equal && p_equal && q_equal)
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
