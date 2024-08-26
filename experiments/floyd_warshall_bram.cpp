#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <sycl/sycl.hpp>
#include <vector>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "exception_handler.hpp"
#include "memory_utils.hpp"

#include "device_print.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

constexpr int MAX_NODES = 512;

// Setting TEST will ensure test data is transfered from FPGA DRAM to to BRAM
// and back. This adds latency, so leave unset for the benchmarks.
#define TEST 1

double floydWarshall_kernel(queue &q, std::vector<int> &h_vertices,
                            std::vector<int> &h_distance) {
  const int numNodes = h_vertices.size();

  int *vertices = fpga_tools::toDevice(h_vertices, q);
  int *dist_dram = fpga_tools::toDevice(h_distance, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int dist[MAX_NODES * MAX_NODES];

#if TEST
    for (int i = 0; i < numNodes * numNodes; ++i)
      dist[i] = dist_dram[i];
#endif

    for (int k = 0; k < numNodes; ++k) {
      for (int i = 0; i < numNodes; ++i) {
        for (int j = 0; j < numNodes; ++j) {
          auto dist_i2j = dist[i * numNodes + j];
          auto dist_i2k = dist[i * numNodes + k];
          auto dist_k2j = dist[k * numNodes + j];

          if (dist_i2j > dist_i2k + dist_k2j) {
            dist[i * numNodes + j] = dist_i2k + dist_k2j;
          }
        }
      }
    }

#if TEST
    for (int i = 0; i < numNodes * numNodes; ++i)
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

void floydWarshall_cpu(const std::vector<int> &vertices,
                       std::vector<int> &dist) {
  const int numNodes = vertices.size();

  for (int k = 0; k < numNodes; ++k) {
    for (int i = 0; i < numNodes; ++i) {
      for (int j = 0; j < numNodes; ++j) {
        if (dist[i * numNodes + j] >
            (dist[i * numNodes + k] + dist[k * numNodes + j])) {
          dist[i * numNodes + j] =
              dist[i * numNodes + k] + dist[k * numNodes + j];
        }
      }
    }
  }
}

void init_data(const std::vector<int> &G, const std::vector<int> &vertices,
               std::vector<int> &dist) {
  const int numNodes = vertices.size();

  for (int i = 0; i < numNodes; i++) {
    for (int j = 0; j < numNodes; j++) {
      if (G[i * numNodes + j] == 0)
        dist[i * numNodes + j] = INT_MAX;
      else
        dist[i * numNodes + j] = G[i * numNodes + j];
    }
  }
}

int main(int argc, char *argv[]) {
  FILE *fp;
  try {
    if (argc < 2) {
      throw std::invalid_argument("Need graph filename.");
    }

    char *filename = argv[1];
    if ((fp = fopen(filename, "r")) == NULL) {
      throw std::invalid_argument("Invalid filename.");
    }
  } catch (exception const &e) {
    std::cout << "Incorrect argv.\nUsage: ./executable [maxIters]\n";
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
    ////////////////////////////////////////////////////////////////////////////
    ////// Read graph
    ////////////////////////////////////////////////////////////////////////////
    int numNodes = 0, numEdges = 0;

    // Read the data set and get the number of nodes (n) and edges (e)
    // int n, e;
    const int MAX_LINE_LENGTH = 1000;
    char str[MAX_LINE_LENGTH];
    char ch = getc(fp);
    while (ch == '#') {
      fgets(str, MAX_LINE_LENGTH - 1, fp);
      // Debug: print title of the data set
      // printf("%s",str);
      sscanf(str, "%*s %d %*s %d", &numNodes,
             &numEdges); // number of nodes
      ch = getc(fp);
    }
    ungetc(ch, fp);

    // DEBUG: Print the number of nodes and edges, skip everything else
    printf("\nGraph data: Nodes: %d, Edges: %d \n\n", numNodes, numEdges);

    // Adjacency list representation of the graph
    std::vector<int> G(numNodes * numNodes, 0);
    std::vector<int> distance(numNodes*numNodes, 0);
    std::vector<int> distance_cpu(numNodes*numNodes, 0);
    std::vector<int> vertices(numNodes, 0);

    int fromNode, toNode;
    int nextNormalizedVertexId = 0;
    // Use ids from 0 to numNodes-1
    std::map<int, int> normalizedVertedId;
    while (!feof(fp)) {
      fscanf(fp, "%d%*s%d", &fromNode, &toNode);

      if (normalizedVertedId.find(fromNode) == normalizedVertedId.end())
        normalizedVertedId[fromNode] = nextNormalizedVertexId++;
      if (normalizedVertedId.find(toNode) == normalizedVertedId.end())
        normalizedVertedId[toNode] = nextNormalizedVertexId++;

      G[normalizedVertedId[fromNode] * numNodes + normalizedVertedId[toNode]] =
          1;
    }
    ////////////////////////////////////////////////////////////////////////////
    ////// END Read graph
    ////////////////////////////////////////////////////////////////////////////

    // Enable profiling.
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    init_data(G, vertices, distance);
    std::copy_n(distance.data(), numNodes * numNodes, distance_cpu.data());

    floydWarshall_cpu(vertices, distance_cpu);
    auto kernel_time = floydWarshall_kernel(q, vertices, distance);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";
#if TEST
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
