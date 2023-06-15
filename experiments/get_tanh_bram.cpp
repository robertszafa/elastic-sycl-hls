#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <sycl/sycl.hpp>
#include <vector>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "exception_handler.hpp"
#include "memory_utils.hpp"

using namespace sycl;
using namespace fpga_tools;

// Forward declare kernel name.
class MainKernel;

constexpr int kN = 1000;

double get_tanh_kernel(queue &q, const std::vector<int> &h_idx,
                       std::vector<int> &h_A) {
  const int N = h_A.size();

  int *idx = fpga_tools::toDevice(h_idx, q);
  int *A_dram = fpga_tools::toDevice(h_A, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int A[kN];

#ifdef TEST
    for (int i = 0; i < N; i++)
      A[i] = A_dram[i];
#endif

    int atanh[12] = {0x08C9, 0x0416, 0x0202, 0x0100, 0x0080, 0x0064,
                     0x0032, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001};
    int cosh[5] = {0x1000, 0x18B0, 0x3C31, 0xA115, 0x1B4EE};
    int sinh[5] = {0x0, 0x12CD, 0x3A07, 0xA049, 0x1B4A3};

    for (int i = 0; i < N; i++) {
      // Input angle
      // Result of tanh, sinh and cosh
      int result = 4096; // Saturation effect
      int beta = A[idx[i]];

      // Implement approximate range of the hyperbolic CORDIC block
      if (beta < 20480) {
        int x = 0x1351;
        int y = 0;
        int x_new;
        int index_trigo;
        int result_cosh, result_sinh;
        int outputcosh, outputsinh;

        if (beta >= 8192) {
          index_trigo = 4;
        } else if (beta >= 12288) {
          index_trigo = 3;
        } else if (beta >= 8192) {
          index_trigo = 2;
        } else if (beta >= 4096) {
          index_trigo = 1;
        } else {
          index_trigo = 0;
        }
        beta = beta - index_trigo * 4096;

        // Call to the hyperbolic CORDIC block
        #pragma unroll
        for (int k = 1; k <= 12; k++) {
          // force the 3k+1 th iteration to be repeated
          if (((k % 3) == 1) && (k != 1)) {
            #pragma unroll
            for (int j = 1; j <= 2; j++) {
              // beta<0 anti-clockwise rotation
              if (beta < 0) {
                x_new = x - (y >> k);
                y -= x >> k;
                beta += atanh[k - 1];
              }
              // beta>0 clockwise rotation
              else {
                x_new = x + (y >> k);
                y += (x >> k);
                beta -= atanh[k - 1];
              }
              x = x_new;
            }
          } else {
            if (beta < 0) {
              x_new = x - (y >> k);
              y -= x >> k;
              beta += atanh[k - 1];
            }
            // beta>0 clockwise rotation
            else {
              x_new = x + (y >> k);
              y += (x >> k);
              beta -= atanh[k - 1];
            }
            x = x_new;
          }
        }
        outputcosh = x;
        outputsinh = y;

        // Trigonometric rules application
        result_cosh =
            (sinh[index_trigo] * outputcosh + cosh[index_trigo] * outputsinh);
        result_sinh =
            (cosh[index_trigo] * outputcosh + sinh[index_trigo] * outputsinh) >>
            12;
        result = result_cosh / result_sinh;
      }

      // Central symmetry correction
      A[idx[i]] = result;
    }

#ifdef TEST
    for (int i = 0; i < N; i++)
      A_dram[i] = A[i];
#endif
  });

  event.wait();

  q.copy(A_dram, h_A.data(), h_A.size()).wait();

  sycl::free(idx, q);
  sycl::free(A_dram, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void get_tanh_cpu(const std::vector<int> &idx, std::vector<int> &A) {
  int atanh[12] = {0x08C9, 0x0416, 0x0202, 0x0100, 0x0080, 0x0064,
                   0x0032, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001};
  int cosh[5] = {0x1000, 0x18B0, 0x3C31, 0xA115, 0x1B4EE};
  int sinh[5] = {0x0, 0x12CD, 0x3A07, 0xA049, 0x1B4A3};

  for (int i = 0; i < kN; i++) {
    // Input angle
    // Result of tanh, sinh and cosh
    int result = 4096; // Saturation effect
    int beta = A[idx[i]];

    // Implement approximate range of the hyperbolic CORDIC block
    if (beta < 20480) {
      int x = 0x1351;
      int y = 0;
      int x_new;
      int index_trigo;
      int result_cosh, result_sinh;
      int outputcosh, outputsinh;

      if (beta >= 8192) {
        index_trigo = 4;
      } else if (beta >= 12288) {
        index_trigo = 3;
      } else if (beta >= 8192) {
        index_trigo = 2;
      } else if (beta >= 4096) {
        index_trigo = 1;
      } else {
        index_trigo = 0;
      }
      beta = beta - index_trigo * 4096;

      // Call to the hyperbolic CORDIC block
      #pragma unroll
      for (int k = 1; k <= 12; k++) {
        // force the 3k+1 th iteration to be repeated
        if (((k % 3) == 1) && (k != 1)) {
          #pragma unroll
          for (int j = 1; j <= 2; j++) {
            // beta<0 anti-clockwise rotation
            if (beta < 0) {
              x_new = x - (y >> k);
              y -= x >> k;
              beta += atanh[k - 1];
            }
            // beta>0 clockwise rotation
            else {
              x_new = x + (y >> k);
              y += (x >> k);
              beta -= atanh[k - 1];
            }
            x = x_new;
          }
        } else {
          if (beta < 0) {
            x_new = x - (y >> k);
            y -= x >> k;
            beta += atanh[k - 1];
          }
          // beta>0 clockwise rotation
          else {
            x_new = x + (y >> k);
            y += (x >> k);
            beta -= atanh[k - 1];
          }
          x = x_new;
        }
      }
      outputcosh = x;
      outputsinh = y;

      // Trigonometric rules application
      result_cosh =
          (sinh[index_trigo] * outputcosh + cosh[index_trigo] * outputsinh);
      result_sinh =
          (cosh[index_trigo] * outputcosh + sinh[index_trigo] * outputsinh) >>
          12;
      result = result_cosh / result_sinh;
    }

    // Central symmetry correction
    A[idx[i]] = result;
  }
}

void init_data(std::vector<int> &idxs, const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind(distribution, generator);

  for (int i = 0; i < idxs.size(); i++) {
    idxs[i] = (dice() < percentage) ? 1 : i;
  }
}

int main(int argc, char *argv[]) {
  int ARRAY_SIZE = 1000;
  int PERCENTAGE = 0;
  try {
    if (argc > 1) {
      ARRAY_SIZE = int(atoi(argv[1]));
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
#else // #if FPGA_EMULATOR
  auto d_selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif
  try {
    // Enable profiling.
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);

    std::vector<int> idxs(kN), h_A(kN, 0), cpu_A(kN, 0);
    init_data(idxs, PERCENTAGE);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    auto kernel_time = get_tanh_kernel(q, idxs, h_A);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

#ifdef TEST
    get_tanh_cpu(idxs, cpu_A);
    if (std::equal(cpu_A.begin(), cpu_A.end(), h_A.begin()))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
#endif

  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
