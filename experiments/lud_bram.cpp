#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <vector>
#include <random>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "memory_utils.hpp"
#include "exception_handler.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

constexpr int kN = 10;

#define TEST 0

double lud_cmp_kernel(queue &q, std::vector<float> &h_A) {

  float *dramA = fpga_tools::toDevice(h_A, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    float A[kN * kN];

    #if TEST
    for (int i = 0; i < kN*kN; i++) 
      A[i] = dramA[i];
    #endif

    for (int i = 0; i < kN; i++) {

      for (int j = 0; j < i; j++) {
        // [0, N) [0, i) 
        auto w = A[i * kN + j];
        for (int k = 0; k < j; k++) {
          //     [0, N) [0, j)   [0, j) [0, i) 
          w -= A[i * kN + k] * A[k * kN + j];
        }
        // rd: [0, i) [0, i) 
        // wr: [0, N) [0, i) 
        A[i * kN + j] = w / A[j * kN + j];
      }

      for (int l = i; l < kN; l++) {
        // [0, N) [i, N) 
        auto w = A[i * kN + l];
        for (int m = 0; m < i; m++) {
          //    [0, N) [0, i)   [0, i) [i, N) 
          w -= A[i * kN + m] * A[m * kN + l];
        }
        // [0, N) [i, N) 
        A[i * kN + l] = w;
      }

    }

    #if TEST
    for (int i = 0; i < kN*kN; i++) 
      dramA[i] = A[i];
    #endif

  });

  event.wait();
  q.copy(dramA, h_A.data(), h_A.size()).wait();

  sycl::free(dramA, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void lud_cmp_cpu(std::vector<float> &A) {
  for (int i = 0; i < kN; i++) {

    for (int j = 0; j < i; j++) {
      auto w = A[i * kN + j];
      for (int k = 0; k < j; k++) {
        w -= A[i * kN + k] * A[k * kN + j];
      }
      A[i * kN + j] = w / A[j * kN + j];
    }

    for (int l = i; l < kN; l++) {
      auto w = A[i * kN + l];
      for (int m = 0; m < i; m++) {
        w -= A[i * kN + m] * A[m * kN + l];
      }
      A[i * kN + l] = w;
    }

  }
}

inline bool almost_equal(const float x, const float y) {
  const float ulpFloat = static_cast<float>(2);
  const float tolerance = 0.0001f;
  return fabsf(x - y) <=
             tolerance * fabsf(x + y) * ulpFloat ||
         fabsf(x - y) < std::numeric_limits<float>::min();
}

int main(int argc, char *argv[]) {
  int N = kN;
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
    std::cout << "Running on device: " << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<float> A(N*N, 2.0f);
    std::vector<float> A_cpu(N*N, 2.0f);
    // std::iota(A.begin(), A.end(), 1.0f);
    // std::iota(A_cpu.begin(), A_cpu.end(), 1.0f);
    for (size_t i=0; i<kN*kN; ++i) {
      A[i] = float(rand() % 100);
      A_cpu[i] = A[i];
    }
    

    auto kernel_time = lud_cmp_kernel(q, A);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    lud_cmp_cpu(A_cpu);

    #if TEST
    if (std::equal(A.begin(), A.end(), A_cpu.begin(), almost_equal))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
    
    for (size_t i=0; i<kN*kN; ++i) {
      if (!almost_equal(A[i],A_cpu[i]))
        std::cout << i << ": " << A[i] << " != " << A_cpu[i] << "\n";
    }
    #endif
    
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

