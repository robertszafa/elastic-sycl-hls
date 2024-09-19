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
#include "device_print.hpp"

#include "pipe_utils.hpp"
#include "unrolled_loop.hpp"


using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double kernel_2mm(queue &q, const int alpha, const int beta, const uint NI,
                  const uint NJ, const uint NK, std::vector<int> &h_A,
                  std::vector<int> &h_B, std::vector<int> &h_C,
                  std::vector<int> &h_D) {

  int *A = fpga_tools::toDevice(h_A, q);
  int *B = fpga_tools::toDevice(h_B, q);
  int *C = fpga_tools::toDevice(h_C, q);
  int *D = fpga_tools::toDevice(h_D, q);

  std::vector zeroVec(h_D.size(), 0);
  int* tmp = fpga_tools::toDevice(zeroVec, q);

  std::vector<sycl::event> events;

  auto main_event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (uint i = 0; i < NI; i++) {
      for (uint j = 0; j < NJ; j++) {
        int x = tmp[i * NI + j];
        for (uint k = 0; k < NK; ++k) {
          x += alpha * A[i * NI + k] * B[k * NK + j];
        }
        tmp[i * NI + j] = x;
      }
    }

    for (uint i = 0; i < NI; i++) {
      for (uint j = 0; j < NJ; j++) {
        int x = D[i * NI + j] * beta;
        for (uint k = 0; k < NJ; ++k) {
          x += tmp[i * NI + k] * C[k * NK + j];
        }
        D[i * NI + j] = x;
      }
    }
  });

  
  events.push_back(main_event);
  sycl::event::wait(events);
  
  q.copy(D, h_D.data(), h_D.size()).wait();

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void kernel_2mm_cpu(const int alpha, const int beta, const int NI, const int NJ,
                    const int NK, const std::vector<int> &A,
                    const std::vector<int> &B, const std::vector<int> &C,
                    std::vector<int> &D) {
  std::vector<int> tmp(NI * NJ, 0);

  for (int i = 0; i < NI; i++) {
    for (int j = 0; j < NJ; j++) {
      int x = tmp[i * NI + j];
      for (int k = 0; k < NK; ++k) {
        x += alpha * A[i * NI + k] * B[k * NK + j];
      }
      tmp[i * NI + j] = x;
    }
  }

  for (int i = 0; i < NI; i++) {
    for (int j = 0; j < NJ; j++) {
      int x = D[i * NI + j] * beta;
      for (int k = 0; k < NJ; ++k) {
        x += tmp[i * NI + k] * C[k * NK + j];
      }
      D[i * NI + j] = x;
    }
  }
}

int main(int argc, char *argv[]) {
  int N = 10;
  try {
    if (argc > 1) 
      N = int(atoi(argv[1]));
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

    const int NI = N;
    const int NJ = N;
    const int NK = N;
    const int S = N*N;

    std::vector<int> A(S, 1);
    std::vector<int> B(S, 2); 
    std::vector<int> C(S, 3); 
    std::vector<int> D(S, 4); 
    std::vector<int> D_cpu(S, 4); 

    auto kernel_time = kernel_2mm(q, 2, 2, NI, NJ, NK, A, B, C, D);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    kernel_2mm_cpu(2, 2, NI, NJ, NK, A, B, C, D_cpu);

    if (std::equal(D.begin(), D.end(), D_cpu.begin()))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
    
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
