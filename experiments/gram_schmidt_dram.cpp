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
                           std::vector<float> &h_r, std::vector<float> &h_p,
                           const int N) {
  auto *a = fpga_tools::toDevice(h_a, q);
  auto *r = fpga_tools::toDevice(h_r, q);
  auto *p = fpga_tools::toDevice(h_p, q);

  std::vector<std::pair<sycl::event, bool>> events; // {event, measureTimeBool} pairs

  auto main_event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (uint k = 0; k < N; k++) {
      float nrm = 0.0f;
      for (uint i = 0; i < N; i++)
        nrm += a[i] * a[i];
      nrm = 0.0019f * ((nrm - 8.0f) * nrm + 16.0f) * nrm + 2.0f;

      r[k] = nrm;
      for (uint i = 0; i < N; i++)
        p[i] = a[i] / r[i];

      for (uint j = k + 1; j < N; j++) {
        r[j] = 0.0f;
        for (uint i = 0; i < N; i++)
          r[j] += p[i] * a[i];

        for (uint i = 0; i < N; i++)
          a[i] += p[i] * r[i];
      }
    }
  });

  
  events.push_back({main_event, true});
  for (auto &kv : events) kv.first.wait();
  q.copy(r, h_r.data(), h_r.size()).wait();
  q.copy(a, h_a.data(), h_a.size()).wait();
  q.copy(p, h_p.data(), h_p.size()).wait();

  sycl::free((void *)a, q);
  sycl::free((void *)r, q);

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

void gram_schmidt_cpu(std::vector<float> &a, std::vector<float> &r,
                      std::vector<float> &p, const int N) {
  for (uint k = 0; k < N; k++) {
    float nrm = 0.0f;
    for (uint i = 0; i < N; i++)
      nrm += a[i] * a[i];
    nrm = 0.0019f * ((nrm - 8.0f) * nrm + 16.0f) * nrm + 2.0f;

    r[k] = nrm;
    for (uint i = 0; i < N; i++)
      p[i] = a[i] / r[i];

    for (uint j = k + 1; j < N; j++) {
      r[j] = 0.0f;
      for (uint i = 0; i < N; i++)
        r[j] += p[i] * a[i];

      for (uint i = 0; i < N; i++)
        a[i] += -p[i] * r[i];
    }
  }
}

void init_data(std::vector<float> &a, std::vector<float> &r,
                int percentage) {
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
    std::vector<float> p(N * N, 1);
    std::vector<float> p_cpu(N * N, 1);

    init_data(a, r,  PERCENTAGE);

    std::copy(a.begin(), a.end(), a_cpu.begin());
    std::copy(r.begin(), r.end(), r_cpu.begin());

    auto kernel_time = gram_schmidt_kernel(q, a, r, p, N);

    gram_schmidt_cpu(a_cpu, r_cpu, p_cpu, N);

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
