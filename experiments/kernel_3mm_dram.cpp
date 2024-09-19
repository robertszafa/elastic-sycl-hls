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

double kernel_3mm(queue &q, const int alpha, const int beta, const int NI,
                  const int NJ, const int NK, const int NL, const int NM,
                  std::vector<int> &h_A, std::vector<int> &h_B,
                  std::vector<int> &h_C, std::vector<int> &h_D,
                  std::vector<int> &h_E, std::vector<int> &h_F,
                  std::vector<int> &h_G) {

  int *A = fpga_tools::toDevice(h_A, q);
  int *B = fpga_tools::toDevice(h_B, q);
  int *C = fpga_tools::toDevice(h_C, q);
  int *D = fpga_tools::toDevice(h_D, q);
  int *E = fpga_tools::toDevice(h_E, q);
  int *F = fpga_tools::toDevice(h_F, q);
  int *G = fpga_tools::toDevice(h_G, q);

  std::vector<std::pair<sycl::event, bool>> events; // {event, measureTimeBool} pairs

  auto main_event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (unsigned i = 0; i < NI; i++) {
      for (unsigned j = 0; j < NJ; j++) {
        for (unsigned k = 0; k < NK; ++k)
          E[i * NJ + j] += A[i * NI + k] * B[k * NK + j];
      }
    }

    for (unsigned j = 0; j < NJ; j++) {
      for (unsigned l = 0; l < NL; l++) {
        for (unsigned m = 0; m < NM; ++m)
          F[j * NJ + l] += C[j * NJ + m] * D[m * NM + l];
      }
    }

    for (unsigned i = 0; i < NI; i++) {
      for (unsigned l = 0; l < NL; l++) {
        for (unsigned j = 0; j < NJ; ++j)
          G[i * NI + l] += E[i * NI + j] * F[j * NJ + l];
      }
    }
  });

  
  events.push_back({main_event, true});
  for (auto &kv : events) kv.first.wait();
  
  q.copy(G, h_G.data(), h_G.size()).wait();

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

void kernel_3mm_cpu(const int alpha, const int beta, const int NI, const int NJ,
                    const int NK, const int NL, const int NM,
                    std::vector<int> &A, std::vector<int> &B,
                    std::vector<int> &C, std::vector<int> &D,
                    std::vector<int> &E, std::vector<int> &F,
                    std::vector<int> &G) {
  for (unsigned i = 0; i < NI; i++) {
    for (unsigned j = 0; j < NJ; j++) {
      for (unsigned k = 0; k < NK; ++k)
        E[i * NJ + j] += A[i * NI + k] * B[k * NK + j];
    }
  }

  for (unsigned j = 0; j < NJ; j++) {
    for (unsigned l = 0; l < NL; l++) {
      for (unsigned m = 0; m < NM; ++m)
        F[j * NJ + l] += C[j * NJ + m] * D[m * NM + l];
    }
  }

  for (unsigned i = 0; i < NI; i++) {
    for (unsigned l = 0; l < NL; l++) {
      for (unsigned j = 0; j < NJ; ++j)
        G[i * NI + l] += E[i * NI + j] * F[j * NJ + l];
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

    // const int NI = N;
    // const int NJ = N;
    // const int NK = N;
    const int S = N*N;

    std::vector<int> A(S, 1), A_cpu(S, 1);
    std::vector<int> B(S, 2), B_cpu(S, 2); 
    std::vector<int> C(S, 3), C_cpu(S, 3); 
    std::vector<int> D(S, 4), D_cpu(S, 4); 
    std::vector<int> E(S, 4), E_cpu(S, 4); 
    std::vector<int> F(S, 4), F_cpu(S, 4); 
    std::vector<int> G(S, 4), G_cpu(S, 4); 

    auto kernel_time = kernel_3mm(q, 2, 2, N, N, N, N, N, A, B, C, D, E, F, G);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    kernel_3mm_cpu(2, 2, N, N, N, N, N, A_cpu, B_cpu, C_cpu, D_cpu, E_cpu,
                   F_cpu, G_cpu);

    if (std::equal(G.begin(), G.end(), G_cpu.begin()))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
    
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
