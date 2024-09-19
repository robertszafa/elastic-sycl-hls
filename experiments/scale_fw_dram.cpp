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

double scale_fw_kernel(queue &q, std::vector<int> &h_vertices,
                   std::vector<int> &h_distance) {
  const int N = h_vertices.size();

  int *vertices = fpga_tools::toDevice(h_vertices, q);
  int *dist = fpga_tools::toDevice(h_distance, q);


  std::vector<sycl::event> events;

  auto main_event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        dist[i * N + j] *= 3;
      }
    }

    for (int k=0; k < N; ++k) {
      for (int i=0; i < N; ++i) {
        for (int j=0; j < N; ++j) {
          if (dist[i*N + j] > dist[i*N + k] + dist[k*N + j])
            dist[i*N + j] = dist[i*N + k] + dist[k*N + j];
        }
      }
    }
  });
  events.push_back(main_event);

  sycl::event::wait(events);
  
  q.copy(dist, h_distance.data(), h_distance.size()).wait();
  sycl::free(dist, q);
  sycl::free(vertices, q);

  double max_event_time = 0;
  for (auto &e : events) {
    auto start = e.get_profiling_info<info::event_profiling::command_start>();
    auto end = e.get_profiling_info<info::event_profiling::command_end>();
    double this_event_time = static_cast<double>(end - start) / 1000000;
    max_event_time = std::max(max_event_time, this_event_time);
  }

  return max_event_time;
}

void scale_fw_cpu(const std::vector<int> &vertices, std::vector<int> &dist) {
    const int N = vertices.size();

    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        dist[i * N + j] *= 3;
      }
    }

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
  int NUM_NODES = 1000;
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
    std::vector<int> distance_cpu(NUM_NODES * NUM_NODES);

    init_data(vertices, distance);
    std::copy_n(distance.data(), NUM_NODES*NUM_NODES, distance_cpu.data());

    auto kernel_time = scale_fw_kernel(q, vertices, distance);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    scale_fw_cpu(vertices, distance_cpu);
    if (std::equal(distance.begin(), distance.end(), distance_cpu.begin())) {
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

