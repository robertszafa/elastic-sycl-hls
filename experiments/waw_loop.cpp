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

double test_kernel_waw(queue &q, std::vector<int> &h_D, std::vector<int> &h_D2,
                       std::vector<int> &h_idx, std::vector<int> &h_idx2,
                       const int NUM_ITERS) {

  const int N = h_D.size();
  const int M = h_D.size();

  int *idx = fpga_tools::toDevice(h_idx, q);
  int *idx2 = fpga_tools::toDevice(h_idx2, q);
  int *D = fpga_tools::toDevice(h_D, q);
  int *D2 = fpga_tools::toDevice(h_D2, q);

  std::vector<std::pair<sycl::event, bool>> events; // {event, measureTimeBool} pairs

  auto main_event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (uint i = 0; i < N; i++) {
      D[idx[i]] = N + i;
    }

    for (uint i = 0; i < M; i++) {
      D[idx2[i]] = N + N + i;
      D2[idx2[i]] = D[idx2[i]]; // Need at least one read to use our StreamingMemory IP. 
    }

  });

  
  events.push_back({main_event, true});
  for (auto &kv : events) kv.first.wait();
  
  q.copy(D, h_D.data(), h_D.size()).wait();
  q.copy(D2, h_D2.data(), h_D2.size()).wait();

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

void test_kernel_cpu(std::vector<int> &D, std::vector<int> &D2,
                     std::vector<int> &idx, std::vector<int> &idx2,
                     const int NUM_ITERS) {
  const int N = D.size();
  const int M = D2.size();

  for (uint i = 0; i < N; i++) {
    D[idx[i]] = N + i;
  }

  for (uint i = 0; i < M; i++) {
    D[idx2[i]] = N + N + i;
    D2[idx2[i]] = D[idx2[i]]; // Need at least one read to use our StreamingMemory IP. 
  }
}

int main(int argc, char *argv[]) {
  int N = 10;
  int NUM_ITERS = 1;
  try {
    if (argc > 1)
      N = int(atoi(argv[1]));
    if (argc > 2)
      NUM_ITERS = int(atoi(argv[2]));
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

    std::vector<int> D(N, 4); 
    std::vector<int> D2(N, 4); 
    std::vector<int> D_cpu(N, 4); 
    std::vector<int> D2_cpu(N, 4); 
    std::vector<int> idx(N, 0); 
    std::vector<int> idx2(N, 4); 

    for (size_t i = 0; i < N; ++i) {
      idx[i] = rand() % N;
    }
    std::sort(idx.begin(), idx.end());
    std::copy(idx.begin(), idx.end(), idx2.begin());

    auto kernel_time = test_kernel_waw(q, D, D2, idx, idx2, NUM_ITERS);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    test_kernel_cpu(D_cpu, D2_cpu, idx, idx2, NUM_ITERS);

    if (std::equal(D.begin(), D.end(), D_cpu.begin()) &&
        std::equal(D2.begin(), D2.end(), D2_cpu.begin())) {
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
