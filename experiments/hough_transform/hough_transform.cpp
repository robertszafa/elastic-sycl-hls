#include <sycl/sycl.hpp>
#include <algorithm>
#include <cstdint>
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

constexpr int k_width = 360;
constexpr int k_height = 360;

#ifndef M_PI
  #define M_PI 3.1415927
#endif



double hough_transform_kernel(queue q, const std::vector<uint8_t> h_d, int w,
                              int h, int s, int bpp, std::vector<uint8_t> h_ht_out) {

  const uint8_t *d = fpga_tools::toDevice(h_d, q);
  // Scalars
  // auto *w = fpga_tools::toDevice(h_w, 1, q);
  // auto *h = fpga_tools::toDevice(h_h, 1, q);
  // auto *s = fpga_tools::toDevice(h_s, 1, q);

  uint8_t *ht_dram = fpga_tools::toDevice(h_ht_out, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int rho, theta, y, x, W = w, H = h;
    int th = sqrt(W * W + H * H) / 2.0;
    int tw = 360;

    uint8_t ht[k_height * k_width * 4];

    for (rho = 0; rho < th; rho++) {
      for (theta = 0; theta < tw /*720*/; theta++) {
        double C = sycl::cos(M_PI * ((double) theta) / 180.0);
        double S = sycl::sin(M_PI * ((double) theta) / 180.0);
        uint32_t totalred = 0;
        uint32_t totalgreen = 0;
        uint32_t totalblue = 0;
        uint32_t totalpix = 0;
        if (theta < 45 || (theta > 135 && theta < 225) || theta > 315) {
          for (y = 0; y < H; y++) {
            double dx = W / 2.0 + (rho - (H / 2.0 - y) * S) / C;
            x = floor(dx + .5);
            if (dx > 0 && dx < W && x == W) {
              totalpix++;
              totalred += d[s * y + bpp * x + (2 % bpp)];
              totalgreen += d[s * y + bpp * x + (1 % bpp)];
              totalblue += d[s * y + bpp * x + (0 % bpp)];
            }
          }
        } else {
          for (x = 0; x < W; x++) {
            double dy = H / 2.0 - (rho - (x - W / 2.0) * C) / S;
            y = floor(dy + .5);
            if (dy > 0 && dy < H && y != H) {
              totalpix++;
              totalred += d[s * y + bpp * x + (2 % bpp)];
              totalgreen += d[s * y + bpp * x + (1 % bpp)];
              totalblue += d[s * y + bpp * x + (0 % bpp)];
            }
          }
        }
        if (totalpix > 0) {
          double dp = totalpix;
          ht[4 * tw * (rho % th) + 4 * (theta % tw) + 2] = (int)(totalred / dp) & 0xff;
          ht[4 * tw * (rho % th) + 4 * (theta % tw) + 1] = (int)(totalgreen / dp) & 0xff;
          ht[4 * tw * (rho % th) + 4 * (theta % tw) + 0] = (int)(totalblue / dp) & 0xff;
        }
      }
    }

    for (int i=0; i<k_height * k_width * 4; ++i)
      ht_dram[i] = ht[i];

  });

  event.wait();
  q.memcpy(h_ht_out.data(), ht_dram, sizeof(h_ht_out[0]) * h_ht_out.size()).wait();

  sycl::free((void*) d, q);
  sycl::free(ht_dram, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void hough_transform_cpu(const uint8_t *d, int w, int h, int s, int bpp, uint8_t *ht) {
    int rho, theta, y, x, W = w, H = h;
    int th = sqrt(W * W + H * H) / 2.0;
    int tw = 360;

    for (rho = 0; rho < th; rho++) {
      for (theta = 0; theta < tw /*720*/; theta++) {
        double C = cos(M_PI * ((double) theta) / 180.0);
        double S = sin(M_PI * ((double) theta) / 180.0);
        uint32_t totalred = 0;
        uint32_t totalgreen = 0;
        uint32_t totalblue = 0;
        uint32_t totalpix = 0;
        if (theta < 45 || (theta > 135 && theta < 225) || theta > 315) {
          for (y = 0; y < H; y++) {
            double dx = W / 2.0 + (rho - (H / 2.0 - y) * S) / C;
            if (dx < 0 || dx >= W)
              continue;
            x = floor(dx + .5);
            if (x == W)
              continue;
            totalpix++;
            totalred += d[s * y + bpp * x + (2 % bpp)];
            totalgreen += d[s * y + bpp * x + (1 % bpp)];
            totalblue += d[s * y + bpp * x + (0 % bpp)];
          }
        } else {
          for (x = 0; x < W; x++) {
            double dy = H / 2.0 - (rho - (x - W / 2.0) * C) / S;
            if (dy < 0 || dy >= H)
              continue;
            y = floor(dy + .5);
            if (y == H)
              continue;
            totalpix++;
            totalred += d[s * y + bpp * x + (2 % bpp)];
            totalgreen += d[s * y + bpp * x + (1 % bpp)];
            totalblue += d[s * y + bpp * x + (0 % bpp)];
          }
        }
        if (totalpix > 0) {
          double dp = totalpix;
          ht[4 * tw * (rho % th) + 4 * (theta % tw) + 2] = (int)(totalred / dp) & 0xff;
          ht[4 * tw * (rho % th) + 4 * (theta % tw) + 1] = (int)(totalgreen / dp) & 0xff;
          ht[4 * tw * (rho % th) + 4 * (theta % tw) + 0] = (int)(totalblue / dp) & 0xff;
        }
      }
    }

    // *h = th; // sqrt(W*W+H*H)/2
    // *w = tw; // 360
    // *s = 4 * tw;
}

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<uint8_t> &d) {
  for (int i = 0; i < d.size(); ++i) {
    d[i] = uint8_t(i);
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
  int ARRAY_SIZE = k_height * k_width;
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

    constexpr int bpp = 8;

    std::vector<uint8_t> d(ARRAY_SIZE);
    int h = k_height;
    int w = k_width;
    int s = k_width;
    int cpu_h = k_height;
    int cpu_w = k_width;
    int cpu_s = k_width;
    std::vector<uint8_t> cpu_ht(k_height * k_width * 4, 0);
    std::vector<uint8_t> fpga_ht(k_height * k_width * 4, 0);

    init_data(d);

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = hough_transform_kernel(q, d, h, w, s, bpp, fpga_ht);

    // Wait for all work to finish.
    q.wait();

    hough_transform_cpu(d.data(), cpu_h, cpu_w, cpu_s, bpp, cpu_ht.data());

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    // std::cout << cpu_h << "/" << h << "\n"
    //           << cpu_w << "/" << w << "\n"
    //           << cpu_s << "/" << s << "\n";
    // if (cpu_h == h && cpu_w == w && cpu_s == s) {
    if (std::equal(fpga_ht.begin(), fpga_ht.end(), cpu_ht.begin())) {
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
