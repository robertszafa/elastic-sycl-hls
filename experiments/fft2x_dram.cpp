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

double fft_conv_kernel(queue &q, std::vector<float> &h_x1,
                       std::vector<float> &h_x2) {
  const uint N = h_x1.size();
  auto *x1 = fpga_tools::toDevice(h_x1, q);
  auto *x2 = fpga_tools::toDevice(h_x2, q);

  std::vector<std::pair<sycl::event, bool>> events; // {event, measureTimeBool} pairs

  auto main_event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (unsigned int step = 1; step < N; step <<= 1) {
      const unsigned int jump = step << 1;
      for (unsigned int group = 0; group < step; group += 2) {

        // Wrong but same perf characteristic as using precomputed twiddles.
        auto twiddle_re = 1.0f;
        auto twiddle_im = 1.0f;
        for (unsigned int pair = group; pair < N; pair += jump) {
          const unsigned int match = pair + step;

          const auto x1_match = x1[match];
          const auto x2_match = x2[match];
          const auto x1_pair = x1[pair];
          const auto x2_pair = x2[pair];

          const float product_re = twiddle_re * x1_match - twiddle_im * x2_match;
          const float product_im = twiddle_im * x1_match + twiddle_re * x2_match;

          x1[match] = x1_pair - product_re;
          x2[match] = x2_pair - product_im;

          x1[pair] = x1_pair + product_re;
          x2[pair] = x2_pair + product_im;
        }

        for (unsigned int pair = group + 1; pair < N; pair += jump) {
          if (group + 1 < step) {
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
          }
        }
      }
    }

  });

  
  events.push_back({main_event, true});
  for (auto &kv : events) kv.first.wait();
  q.copy(x1, h_x1.data(), h_x1.size()).wait();
  q.copy(x2, h_x2.data(), h_x2.size()).wait();

  sycl::free(x1, q);
  sycl::free(x2, q);

  double max_event_time = 0;
  for (auto &[e, toMeasure] : events) {
    if (toMeasure) {
      auto start = e.get_profiling_info<info::event_profiling::command_start>();
      auto end = e.get_profiling_info<info::event_profiling::command_end>();
      double this_event_time = static_cast<double>(end - start) / 1000000;
      max_event_time = max(max_event_time, this_event_time);
    }
  }

  return max_event_time;
}

void fft_conv_cpu(std::vector<float> &x1, std::vector<float> &x2) {
  const uint N = x1.size();
  for (unsigned int step = 1; step < N; step <<= 1) {
    const unsigned int jump = step << 1;
    for (unsigned int group = 0; group < step; group += 2) {

      // Wrong but same perf characteristic as using precomputed twiddles.
      auto twiddle_re = 1.0f;
      auto twiddle_im = 1.0f;
      for (unsigned int pair = group; pair < N; pair += jump) {
        const unsigned int match = pair + step;

        const auto x1_match = x1[match];
        const auto x2_match = x2[match];
        const auto x1_pair = x1[pair];
        const auto x2_pair = x2[pair];

        const float product_re = twiddle_re * x1_match - twiddle_im * x2_match;
        const float product_im = twiddle_im * x1_match + twiddle_re * x2_match;

        x1[match] = x1_pair - product_re;
        x2[match] = x2_pair - product_im;

        x1[pair] = x1_pair + product_re;
        x2[pair] = x2_pair + product_im;
      }

      for (unsigned int pair = group + 1; pair < N; pair += jump) {
        if (group + 1 < step) {
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
        }
      }
    }
  }
}

void getTwiddles(const int N, std::vector<float> &twiddles_re,
                 std::vector<float> &twiddles_im) {
  const float pi = -3.14159265358979323846;
  for (unsigned int step = 1; step < N; step <<= 1) {
    const float step_d = (float)step;
    float twiddle_re = 1.0;
    float twiddle_im = 0.0;
    for (unsigned int group = 0; group < step; group++) {
      float angle = pi * ((float)group + 1) / step_d;
      twiddle_re = cos(angle);
      twiddle_im = sin(angle);
      twiddles_re.push_back(twiddle_re);
      twiddles_im.push_back(twiddle_im);
    }
  }
}

inline bool almost_equal(const float x, const float y) {
  float ulpFloat = static_cast<float>(2);
  return fabsf(x - y) <=
             std::numeric_limits<float>::epsilon() * fabsf(x + y) * ulpFloat ||
         fabsf(x - y) < std::numeric_limits<float>::min();
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

    std::vector<float> x1(N, 1);
    std::vector<float> x2(N, 1);
    std::vector<float> x1_cpu(N, 1);
    std::vector<float> x2_cpu(N, 1);
    for (int i = 0; i < N; ++i) {
      x1[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      x2[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      x1_cpu[i] = x1[i];
      x2_cpu[i] = x2[i];
    }

    std::vector<float> twiddles_im, twiddles_re;
    getTwiddles(N, twiddles_re, twiddles_im);

    auto kernel_time = fft_conv_kernel(q, x1, x2);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";
    fft_conv_cpu(x1_cpu, x2_cpu);

    if (std::equal(x1.begin(), x1.end(), x1_cpu.begin(), almost_equal) &&
        std::equal(x2.begin(), x2.end(), x2_cpu.begin(), almost_equal)) {
      std::cout << "\nPassed\n";
    } else {
      std::cout << "\nFailed\n";
      for (int i=0; i<N; ++i) {
        if (x1[i] != x1_cpu[i]) {
          std::cout << i << ": " << x1[i] << " != " << x1_cpu[i] << "\n";
        }
      }
    }
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
