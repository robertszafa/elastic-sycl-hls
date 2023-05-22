#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <vector>
#include <random>

#include <sycl/ext/intel/fpga_extensions.hpp>


#include "memory_utils.hpp"

using namespace sycl;
using namespace fpga_tools;

// Forward declare kernel name.
class MainKernel;

constexpr int N = 1000;

double maximal_matching_kernel(queue &q, const std::vector<int> &h_edges, std::vector<int> &h_vertices,
                               int *h_out, const int num_edges) {
  const int *edges = fpga_tools::toDevice(h_edges, q);
  // int *vertices = fpga_tools::toDevice(h_vertices, q);
  int *out = fpga_tools::toDevice(h_out, 1, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    [[intel::singlepump]] 
    [[intel::max_replicates(1)]] 
    [[intel::numbanks(1)]] 
    int vertices[N];

    // #pragma unroll
    // for (int i = 0; i < 1000; ++i)
    //   vertices[i] = -1;

    int i = 0;
    int out_scalar = 0;

    while (i < num_edges) {
      int j = i * 2;

      int e1 = edges[j];
      int e2 = edges[j + 1];

      auto v1 = vertices[e1];
      auto v2 = vertices[e2];
      if (v1 == 0 && v2 == 0) {
        vertices[e1] = 1;
        vertices[e2] = 1;

        out_scalar = out_scalar + 1;
      }

      i = i + 1;
    }

    *out = out_scalar;
  });

  event.wait();
  // q.memcpy(h_vertices.data(), vertices, sizeof(h_vertices[0]) * h_vertices.size()).wait();
  q.memcpy(h_out, out, sizeof(h_out[0])).wait();

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<int> &edges, std::vector<int> &vertices, data_distribution distr, 
               const int num_edges, const uint percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind (distribution, generator);

  edges[0] = 0;
  edges[1] = 1;
  vertices[0] = -1;
  vertices[1] = -1;
  for (int i = 2; i < num_edges*2; i += 2) {
    if (distr == data_distribution::ALL_WAIT) {
      edges[0] = 0;
      edges[1] = 2;
      edges[i] = i-2;
      edges[i+1] = i;
    }
    else if (distr == data_distribution::NO_WAIT) {
      edges[i] = i;
      edges[i+1] = i+1;
    }
    else {
      edges[i] = (dice() <= percentage) ? edges[i-2] : i;
      edges[i+1] = (dice() <= percentage) ? edges[i-1] : i+1;
    }

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
  // defaulats
  uint NUM_EDGES = 64;
  auto DATA_DISTR = data_distribution::ALL_WAIT;
  uint PERCENTAGE = 5;
  try {
    if (argc > 1) {
      NUM_EDGES = uint(atoi(argv[1]));
      if (NUM_EDGES < 2) throw std::invalid_argument("At least 2 edges rq.");
    }
    if (argc > 2) {
      DATA_DISTR = data_distribution(atoi(argv[2]));
    }
    if (argc > 3) {
      PERCENTAGE = uint(atoi(argv[3]));
      std::cout << "Percentage is " << PERCENTAGE << "\n";
      if (PERCENTAGE < 0 || PERCENTAGE > 100) throw std::invalid_argument("Invalid percentage.");
    }
  }  catch (exception const &e) {
    std::cout << "Incorrect argv.\nUsage:\n";
    std::cout << "  ./mm [ARRAY_SIZE] [data_distribution (0/1/2)] [PERCENTAGE (only for data_distr 2)]\n";
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
    std::cout << "Running on device: " << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<int> edges(NUM_EDGES*2);
    std::vector<int> vertices(NUM_EDGES*2);

    init_data(edges, vertices, DATA_DISTR, NUM_EDGES, PERCENTAGE);

    std::vector<int> vertices_cpu(NUM_EDGES*2);
    std::copy(vertices.begin(), vertices.end(), vertices_cpu.begin());

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    int out = 0;

    kernel_time = maximal_matching_kernel(q, edges, vertices, &out, NUM_EDGES);

    // Wait for all work to finish.
    q.wait();

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    int out_cpu = maximal_matching_cpu(edges, vertices_cpu, NUM_EDGES);
    if (out == out_cpu) {
      std::cout << "Passed\n";
    }
    else {
      std::cout << "Failed\n";
      std::cout << "  out fpga = " <<  out << "\n";
      std::cout << "  out cpu = " <<  out_cpu << "\n";
    }
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

