#include <map>
#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <vector>
#include <random>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "device_print.hpp"
#include "memory_utils.hpp"
#include "exception_handler.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

constexpr int MAX_NUM_NODES = 8192;

#define TEST 1

double sssp_kernel(queue &q, const std::vector<int> &h_G,
                   std::vector<int> &h_cost, std::vector<int> &h_distance,
                   std::vector<int> &h_visited, std::vector<int> &h_pred,
                   int numNodes, int startNode) {

  int *G = fpga_tools::toDevice(h_G, q);
  int *cost = fpga_tools::toDevice(h_cost, q);
  int *distance_dram = fpga_tools::toDevice(h_distance, q);
  int *visited_dram = fpga_tools::toDevice(h_visited, q);
  int *pred = fpga_tools::toDevice(h_pred, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    [[intel::singlepump]] 
    [[intel::max_replicates(1)]] 
    [[intel::numbanks(1)]] 
    int distance[MAX_NUM_NODES];

    [[intel::singlepump]] 
    [[intel::max_replicates(1)]] 
    [[intel::numbanks(1)]] 
    int visited[MAX_NUM_NODES];
    #if TEST
    for (int i = 0; i < numNodes; ++i) {
      visited[i] = 0;
      distance[i] = distance_dram[i];
    }
    #endif

    int count = 1;
    while (count < numNodes - 1) {
      int mindist = INT_MAX;
      int nextnode = 0;
      // nextnode gives the node at minimum distance
      for (int i = 0; i < numNodes; i++) {
        if (distance[i] < mindist && !visited[i]) {
          mindist = distance[i];
          nextnode = i;
        }
      }

      // check if a better path exists through nextnode
      visited[nextnode] = 1;

      for (int i = 0; i < numNodes; i++) {
        if ((mindist + cost[nextnode * numNodes + i]) < distance[i] &&
            !visited[i]) {
          distance[i] = mindist + cost[nextnode * numNodes + i];
          pred[i] = nextnode; // force visited load
        }
      }
      count++;
      // PRINTF("count = %d\n", count);
    }

    #if TEST
    for (int i = 0; i < numNodes; ++i) {
      distance_dram[i] = distance[i];
    }
    #endif
  });

  event.wait();
  q.copy(distance_dram, h_distance.data(), h_distance.size()).wait();

  sycl::free(G, q);
  sycl::free(cost, q);
  sycl::free(distance_dram, q);
  sycl::free(visited_dram, q);
  sycl::free(pred, q);


  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void sssp_cpu(const std::vector<int> &G, std::vector<int> &cost,
                   std::vector<int> &distance, std::vector<int> &visited,
                   std::vector<int> &pred, int numNodes, int startNode) {
  int count = 1;
  while (count < numNodes - 1) {
    int mindistance = INT_MAX;
    int nextnode = 0;
    // nextnode gives the node at minimum distance
    for (int i = 0; i < numNodes; i++) {
      bool isvis = visited[i];
      if (distance[i] < mindistance && !isvis) {
        mindistance = distance[i];
        nextnode = i;
      }
    }

    // check if a better path exists through nextnode
    visited[nextnode] = 1;
    for (int i = 0; i < numNodes; i++) {
        bool isvis = visited[i];
      if (mindistance + cost[nextnode * numNodes + i] < distance[i] && !isvis) {
        distance[i] = mindistance + cost[nextnode * numNodes + i];
        pred[i] = nextnode;
      }
    }
    count++;
  }
}

void init_data(std::vector<int> &G, std::vector<int> &cost,
               std::vector<int> &distance, std::vector<int> &visited,
               std::vector<int> &pred, int numNodes, int startNode) {
  // int cost[MAX][MAX],distance[MAX],pred[MAX];
  // int visited[MAX],count,mindistance,nextnode,i,j;
  // pred[] stores the predecessor of each node
  // count gives the number of nodes seen so far
  // create the cost matrix
  for (int i = 0; i < numNodes; i++) {
    for (int j = 0; j < numNodes; j++) {
      if (G[i * numNodes + j] == 0)
        cost[i*numNodes + j] = INT_MAX;
      else
        cost[i*numNodes + j] = G[i*numNodes + j];
    }
  }
  // initialize pred[],distance[] and visited[]
  for (int i = 0; i < numNodes; i++) {
    distance[i] = cost[startNode *numNodes + i];
    pred[i] = startNode;
    visited[i] = 0;
  }
  distance[startNode] = 0;
  visited[startNode] = 1;
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
#else  // #if FPGA_EMULATOR
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
      std::vector<int> cost(numNodes * numNodes, 0);
      std::vector<int> distance(numNodes, 0);
      std::vector<int> visited(numNodes, 0);
      std::vector<int> pred(numNodes, 0);
      int startNode = 0;

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


        G[normalizedVertedId[fromNode] * numNodes + normalizedVertedId[toNode]] = 1;
        // addEdge(normalizedVertedId[fromNode], normalizedVertedId[toNode],
        //         adjList, numNeighboursLookup, numNodes);
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


    init_data(G, cost, distance, visited, pred, numNodes, startNode);
    std::vector<int> distance_cpu(numNodes);
    std::copy_n(distance.data(), numNodes, distance_cpu.data());

    auto kernel_time =
        sssp_kernel(q, G, cost, distance, visited, pred, numNodes, startNode);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    sssp_cpu(G, cost, distance_cpu, visited, pred, numNodes, startNode);
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

