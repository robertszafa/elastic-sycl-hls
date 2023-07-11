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

double floydWarshall_kernel(queue &q, std::vector<int> &h_vertices,
                   std::vector<int> &h_distance) {
  const int N = h_vertices.size();

  int *vertices = fpga_tools::toDevice(h_vertices, q);
  int *dist_dram = fpga_tools::toDevice(h_distance, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int dist[kN*kN];

#ifdef TEST
    for (int i=0; i < N*N; ++i)
      dist[i] = dist_dram[i];
#endif

    for (int k=0; k < kN; ++k) {
      for (int i=0; i < kN; ++i) {
        for (int j=0; j < kN; ++j) {
          if (dist[i*kN + j] > dist[i*kN + k] + dist[k*kN + j])
            dist[i*kN + j] = dist[i*kN + k] + dist[k*kN + j];
        }
      }
    }

#ifdef TEST
    for (int i=0; i < N*N; ++i)
      dist_dram[i] = dist[i];
#endif
  });

  event.wait();
  q.copy(dist_dram, h_distance.data(), h_distance.size()).wait();

  sycl::free(dist_dram, q);
  sycl::free(vertices, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void floydWarshall_cpu(const std::vector<int> &vertices, std::vector<int> &dist) {
    const int N = vertices.size();

    for (int k=0; k < N; ++k) {
      for (int i=0; i < N; ++i) {
        for (int j=0; j < N; ++j) {
          if (dist[i*N + j] > dist[i*N + k] + dist[k*N + j])
            dist[i*N + j] = dist[i*N + k] + dist[k*N + j];
        }
      }

    }
}

void init_data(const std::vector<int> &vertices, std::vector<int> &dist) {
  const int N = vertices.size();

  // Create adjencency matrix
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      dist[i*N + j] = i==j ? 0 : rand() % N;
    }
  }
}

int main(int argc, char *argv[]) {
  int NUM_NODES = 100;
  int PERCENTAGE = 0;
  try {
    if (argc > 1) {
      NUM_NODES = int(atoi(argv[1]));
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

    std::vector<int> vertices(NUM_NODES);
    std::vector<int> distance(NUM_NODES * NUM_NODES);
    std::vector<int> distance_cpu(NUM_NODES);

    init_data(vertices, distance);
    std::copy_n(distance.data(), NUM_NODES, distance_cpu.data());

    auto kernel_time = floydWarshall_kernel(q, vertices, distance);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

#ifdef TEST
    floydWarshall_cpu(vertices, distance_cpu);
    if (std::equal(distance.begin(), distance.end(), distance_cpu.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failed\n";
    }
#endif

  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

