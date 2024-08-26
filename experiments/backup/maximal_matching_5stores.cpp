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
using namespace fpga_tools;

// Forward declare kernel name.
class MainKernel;

double maximal_matching_kernel(queue &q, const std::vector<int> &h_edges, std::vector<int> &h_vertices,
                               int *h_out, const int num_edges) {
  const int *edges = fpga_tools::toDevice(h_edges, q);
  int *vertices = fpga_tools::toDevice(h_vertices, q);
  int *out = fpga_tools::toDevice(h_out, 1, q);

  const int num_vertices = h_vertices.size();

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int i = 0;
    int out_scalar = 0;

    while (i < num_edges) {
      int j = i * 2;

      int e1 = edges[j];
      int e2 = edges[j + 1];

      if ((vertices[e1] < 0) && (vertices[e2] < 0)) { 
        vertices[e1] = e2;
        vertices[e2] = e1;
        vertices[max(e1 - 2, 0)] = e2;
        vertices[max(e2 - 2, 0)] = e1;
        vertices[max(e2 - 1, 0)] = e1;

        out_scalar = out_scalar + 1;
      }

      i = i + 1;
    }

    *out = out_scalar;
  });

  event.wait();
  q.memcpy(h_vertices.data(), vertices, sizeof(h_vertices[0]) * h_vertices.size()).wait();
  q.memcpy(h_out, out, sizeof(h_out[0])).wait();

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}


void maximal_matching_cpu(const int *idx, const int *weight, float *hist, const int N) {
  for (int i = 0; i < N; ++i) {
    auto wt = weight[i];
    auto idx_scalar = idx[i];
    auto x = hist[idx_scalar];

    if (wt > 0) {
      hist[idx_scalar] = x + 10.0;
    }
    else if (wt == 0) {
      if (idx_scalar == 1)
        hist[idx_scalar] = x * 2.0;
      else
        hist[idx_scalar] = x - 20.0;
    }
    else {
      hist[idx_scalar] = x - 10.0; 
    }
  }
}

void init_data(std::vector<int> &edges, std::vector<int> &vertices, 
               const int num_edges, const uint percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 99);
  auto dice = std::bind (distribution, generator);

  edges[0] = 0;
  edges[1] = 1;
  vertices[0] = -1;
  vertices[1] = -1;
  for (int i = 2; i < num_edges*2; i += 2) {
    edges[i] = (dice() < percentage) ? edges[i-2] : i;
    edges[i+1] = (dice() < percentage) ? edges[i-1] : i+1;

    vertices[i] = -1;
    vertices[i+1] = -1;
  }
}

int maximal_matching_cpu(const std::vector<int> &edges, std::vector<int> &vertices, const int num_edges) {
  int i = 0;
  int out = 0;

  while (i < num_edges) {

    int j = i * 2;

    int u = edges[j];
    int v = edges[j + 1];

    int vertex_u = vertices[u];
    int vertex_v = vertices[v];
    
    if (vertex_u < 0 && vertex_v < 0) {
      vertices[u] = v;
      vertices[v] = u;

      out = out + 1;
    }

    i = i + 1;
  }

  return out;
}



int main(int argc, char *argv[]) {
  int NUM_EDGES = 1000;
  int PERCENTAGE = 0;
  try {
    if (argc > 1) {
      NUM_EDGES = int(atoi(argv[1]));
      if (NUM_EDGES < 2) throw std::invalid_argument("At least 2 edges rq.");
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

    std::vector<int> edges(NUM_EDGES*2);
    std::vector<int> vertices(NUM_EDGES*2);
    int out = 0;

    init_data(edges, vertices,  NUM_EDGES, PERCENTAGE);

    std::vector<int> vertices_cpu(NUM_EDGES*2);
    std::copy(vertices.begin(), vertices.end(), vertices_cpu.begin());

    auto kernel_time = maximal_matching_kernel(q, edges, vertices, &out, NUM_EDGES);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    int out_cpu = maximal_matching_cpu(edges, vertices_cpu, NUM_EDGES);
    if (out == out_cpu) {
      std::cout << "Passed\n";
    }
    else {
      std::cout << "Failed\n";
    }
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

