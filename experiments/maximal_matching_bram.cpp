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

constexpr int kN = 1000;

double maximal_matching_kernel(queue &q, const std::vector<int> &h_edges,
                               std::vector<int> &h_vertices, int *h_out,
                               std::vector<int> &h_is_true,
                               const int N) {
  const int *edges = fpga_tools::toDevice(h_edges, q);
  int *vertices_dram = fpga_tools::toDevice(h_vertices, q);
  int *is_true = fpga_tools::toDevice(h_is_true, q);
  int *out = fpga_tools::toDevice(h_out, 1, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int vertices[kN*2];

    #ifdef TEST
    for (int i = 0; i < N*2; ++i)
      vertices[i] = vertices_dram[i];
    #endif

    int i = 0;
    int out_scalar = 0;

    while (i < N) {
      int j = i * 2;

      int e1 = edges[j];
      int e2 = edges[j + 1];

      auto v1 = vertices[e1];
      auto v2 = vertices[e2];

      if (v1 == 0 && v2 == 0) {
        vertices[e1] = e2;
        vertices[e2] = e1;

        out_scalar = out_scalar + 1;
      } 

      i = i + 1;
    }

    *out = out_scalar;
  });

  event.wait();
  q.memcpy(h_out, out, sizeof(h_out[0])).wait();

  sycl::free(vertices_dram, q);
  sycl::free(is_true, q);
  sycl::free((void*) edges, q);
  sycl::free(out, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

int maximal_matching_cpu(const std::vector<int> &edges,
                         std::vector<int> &vertices,
                         std::vector<int> &is_true, const int num_edges) {
  int i = 0;
  int out = 0;

  while (i < num_edges) {

    int j = i * 2;

    int e1 = edges[j];
    int e2 = edges[j + 1];

    auto v1 = vertices[e1];
    auto v2 = vertices[e2];
    
    // if (vertex_u < 0 && vertex_v < 0) {
    if (is_true[i]) {
      vertices[e1] = v2;
      vertices[e2] = v1;

      out = out + 1;
    }

    i = i + 1;
  }

  return out;
}

void init_data(std::vector<int> &edges, std::vector<int> &vertices,
               std::vector<int> &is_true, const int num_edges,
               const uint percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 99);
  auto dice = std::bind (distribution, generator);

  edges[0] = 0;
  edges[1] = 1;
  vertices[0] = -1;
  vertices[1] = -1;
  for (int i = 2; i < num_edges*2; i += 2) {
    is_true[i] =  (dice() < percentage) ? 1 : 0;

    edges[i] = (dice() < percentage) ? edges[i-2] : i;
    edges[i+1] = (dice() < percentage) ? edges[i-1] : i+1;

    vertices[i] = -1;
    vertices[i+1] = -1;
  }
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

    std::vector<int> edges(kN*2);
    std::vector<int> vertices(kN*2);
    std::vector<int> is_true(kN*2);
    int out = 0;

    init_data(edges, vertices, is_true, kN, PERCENTAGE);

    std::vector<int> vertices_cpu(kN*2);
    std::copy(vertices.begin(), vertices.end(), vertices_cpu.begin());

    auto kernel_time = maximal_matching_kernel(q, edges, vertices, &out, is_true, kN);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    #ifdef TEST
    int out_cpu = maximal_matching_cpu(edges, vertices_cpu, is_true, NUM_EDGES);
    if (out == out_cpu) {
      std::cout << "Passed\n";
    }
    else {
      std::cout << "Failed\n";
      std::cout << "  out fpga = " <<  out << "\n";
      std::cout << "  out cpu = " <<  out_cpu << "\n";
    }
    #endif
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

