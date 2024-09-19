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

#define max(a, b) a > b ? a : b
#define min(a, b) a < b ? a : b

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double fft_conv_kernel(queue &q, const std::vector<float> &h_h,
                          const std::vector<float> &h_x1, const std::vector<float> &h_x2,
                          std::vector<float> &h_y1, 
                          std::vector<float> &h_y2) {
  const uint N = h_x1.size();
  const uint lenH = h_h.size();
  const uint lenConv = lenH + N - 1;

  auto *h = fpga_tools::toDevice(h_h, q);
  auto *x1 = fpga_tools::toDevice(h_x1, q);
  auto *x2 = fpga_tools::toDevice(h_x2, q);
  auto *y1 = fpga_tools::toDevice(h_y1, q);
  auto *y2 = fpga_tools::toDevice(h_y2, q);

  std::vector<sycl::event> events;

  auto main_event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    const float pi = -3.14159265358979323846;

    for (unsigned int step = 1; step < N; step <<= 1) {
      const unsigned int jump = step << 1;
      const float step_d = (float)step;
      float twiddle_re = 1.0;
      float twiddle_im = 0.0;
      for (unsigned int group = 0; group < step; group++) {
        // PRINTF("\n");
        for (unsigned int pair = group; pair < N; pair += jump) {
          const unsigned int match = pair + step;

          const auto x1_match = x1[match];
          const auto x2_match = x2[match];
          const auto x1_pair = x1[pair];
          const auto x2_pair = x2[pair];

          const float product_re =
              twiddle_re * x1_match - twiddle_im * x2_match;
          const float product_im =
              twiddle_im * x1_match + twiddle_re * x2_match;

          x1[match] = x1_pair - product_re;
          x2[match] = x2_pair - product_im;

          x1[pair] = x1_pair + product_re;
          x2[pair] = x2_pair + product_im;
          // PRINTF("%d, ", pair);
          // PRINTF("%d, ", match);
          // PRINTF("%d, ", match == pair);
        }

        float angle = pi * ((float)group + 1) / step_d;
        twiddle_re = cos(angle);
        twiddle_im = sin(angle);
      }
    }

    for (uint i = lenH-1; i < lenConv; i++) {
      // if (i < lenH-1)
      //   continue;

      uint x_start = i - lenH + 1;
      uint h_start = min(i, lenH - 1);

      float y1_tmp = y1[i];
      float y2_tmp = y2[i];
      // PRINTF("\n-----------------\n");
      for (uint j = x_start; j < N; j++) {
        // PRINTF("%d, ", j);
        y1_tmp += h[h_start] * x1[j];
        y2_tmp += h[h_start] * x2[j];
        h_start--;
      }
      y1[i] = y1_tmp;
      y2[i] = y2_tmp;
    }
  });

  
  events.push_back(main_event);
  sycl::event::wait(events);
  q.copy(y1, h_y1.data(), h_y1.size()).wait();
  q.copy(y2, h_y2.data(), h_y2.size()).wait();

  sycl::free(h, q);
  sycl::free(x1, q);
  sycl::free(x2, q);
  sycl::free(y1, q);
  sycl::free(y2, q);

  double max_event_time = 0;
  for (auto &e : events) {
    auto start = e.get_profiling_info<info::event_profiling::command_start>();
    auto end = e.get_profiling_info<info::event_profiling::command_end>();
    double this_event_time = static_cast<double>(end - start) / 1000000;
    max_event_time = max(max_event_time, this_event_time);
  }

  return max_event_time;
}

void fft_conv_cpu(const std::vector<float> &h, std::vector<float> &x1,
                  std::vector<float> &x2, std::vector<float> &y1,
                  std::vector<float> &y2) {
  const uint N = x1.size();
  const uint lenH = h.size();
  const uint lenConv = lenH + N - 1;

  const float pi = -3.14159265358979323846;

  for (unsigned int step = 1; step < N; step <<= 1) {
    const unsigned int jump = step << 1;
    const float step_d = (float)step;
    float twiddle_re = 1.0;
    float twiddle_im = 0.0;
    for (unsigned int group = 0; group < step; group++) {
      for (unsigned int pair = group; pair < N; pair += jump) {
        const unsigned int match = pair + step;
        const float product_re =
            twiddle_re * x1[match] - twiddle_im * x2[match];
        const float product_im =
            twiddle_im * x1[match] + twiddle_re * x2[match];
        x1[match] = x1[pair] - product_re;
        x2[match] = x2[pair] - product_im;
        x1[pair] += product_re;
        x2[pair] += product_im;
      }

      float angle = pi * ((float)group + 1) / step_d;
      twiddle_re = cos(angle);
      twiddle_im = sin(angle);
    }
  }

  for (uint i = 0; i < lenConv; i++) {
    uint x_start = max(0, i - lenH + 1);
    uint x_end = min(i + 1, N);
    uint h_start = min(i, lenH - 1);

    for (uint j = x_start; j < x_end; j++) {
      y1[i] += h[h_start] * x1[j];
      y2[i] += h[h_start] * x2[j];
      h_start--;
    }
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

    std::vector<float> h(N, 1);
    std::vector<float> x1(N, 1);
    std::vector<float> x2(N, 1);
    std::vector<float> y1(N + N, 0);
    std::vector<float> y2(N + N, 0);

    std::vector<float> x1_cpu(N, 1);
    std::vector<float> x2_cpu(N, 1);
    std::vector<float> y1_cpu(N + N, 0);
    std::vector<float> y2_cpu(N + N, 0);

    auto kernel_time = fft_conv_kernel(q, h, x1, x2, y1, y2);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";
    fft_conv_cpu(h, x1_cpu, x2_cpu, y1_cpu, y2_cpu);


    if (std::equal(y1.begin(), y1.end(), y1_cpu.begin()) &&
        std::equal(y2.begin(), y2.end(), y2_cpu.begin()))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
