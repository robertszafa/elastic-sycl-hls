#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <math.h>
#include <numeric>
#include <queue>
#include <random>
#include <stdlib.h>
#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/sycl.hpp>
#include <vector>

#include "exception_handler.hpp"

#include "LoadStoreQueueBRAM.hpp"
#include "memory_utils.hpp"
#include "device_print.hpp"

using namespace sycl;

constexpr int MAX_VERTICES = 2000;

// Forward declare kernel name.
class MainKernel;
class MainKernel_AGU_0;

double kernel_fpga(queue &q, std::vector<int> &h_adjList,
                   std::vector<int> &h_numNeighboursLookup,
                   std::vector<int> &h_path_counts, std::vector<int> &h_depths,
                   int numVertices, int startNode) {
  int *adjList = fpga_tools::toDevice(h_adjList, q);
  int *numNeighboursLookup = fpga_tools::toDevice(h_numNeighboursLookup, q);
  int *depths_dram = fpga_tools::toDevice(h_depths, q);
  int *path_counts_dram = fpga_tools::toDevice(h_path_counts, q);

  using CurrentNodePopPipe = pipe<class _CurrentNodePopPipe, int, 16>;
  using CurrentNodePushPipe = pipe<class _CurrentNodePushPipe, int, 16>;

  using QueueIsEmptyPipe = pipe<class _QueueIsEmptyPipe, bool, 16>;

  auto queueEvent = q.single_task<class QueueKernel>([=]() [[intel::kernel_args_restrict]] {
    // Create a queue for bc
    int queue[MAX_VERTICES];
    int front = 0, rear = 0;
    queue[rear++] = startNode;

    [[intel::ivdep]]
    while (true) {
      // Dequeue a vertex from queue and print it
      if (front < rear) {
        int currentNode = queue[front];
        bool succ = false;
        CurrentNodePopPipe::write(currentNode, succ);
        if (succ) {
          front++;
        }
      }

      bool succ2 = false;
      int nextNode = CurrentNodePushPipe::read(succ2);
      if (succ2) {
        queue[rear++] = nextNode;
        // PRINTF("New push node %d\n", nextNode);
      } 

      if (rear == front) {
        bool succ3 = false;
        QueueIsEmptyPipe::read(succ3);
        // if (front == rear && succ3)
        if (succ3)
          break;
      }
    }

    // PRINTF("rear = %d\n", rear);
    // PRINTF("EXITING QUEUE KERNEL\nDONE %d NODES\n", front);
    // Sentinel value.
    CurrentNodePopPipe::write(-1);
  });

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int depths[MAX_VERTICES];
    for (int i = 0; i < numVertices; ++i) 
      depths[i] = -1;

    int path_counts[MAX_VERTICES];
    path_counts[startNode] = 1;

    int qsize = 1;
    while (true) {
      int u = CurrentNodePopPipe::read();
      qsize--;
      if (u == -1)
        break;

      // PRINTF("du: depths[%d] = %d\n", u, du);

      int pushedCount = 0;
      for (int i = 0; i < numNeighboursLookup[u]; ++i) {
        int v = adjList[u * numVertices + i];

        auto dv = depths[v];
        auto du = depths[u];
        
        if (dv == -1) {
          CurrentNodePushPipe::write(v);
          pushedCount++;
        }
        dv = du + 1;
        depths[v] = dv;

        // if (dv == du + 1) {
          auto path_count_u = path_counts[u];
          path_counts[v] += path_count_u;
        // }
      }
      qsize += pushedCount;

      // PRINTF("Qsize = %d\n", qsize);
      if (qsize == 0)
        QueueIsEmptyPipe::write(1);
    }

    // PRINTF("Exit main kernel\n"); 

    for (int i = 0; i < numVertices; ++i) {
      depths_dram[i] = int(depths[i]);
      path_counts_dram[i] = int(path_counts[i]);
    }
  });

  event.wait();
  queueEvent.wait();

  q.copy(depths_dram, h_depths.data(), h_depths.size()).wait();
  q.copy(path_counts_dram, h_path_counts.data(), h_path_counts.size()).wait();

  sycl::free(adjList, q);
  sycl::free(numNeighboursLookup, q);
  sycl::free(depths_dram, q);
  sycl::free(path_counts_dram, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;
  return time_in_ms;
}

// Function to perform Breadth First Search on a graph
// represented using adjacency list
void bc_cpu(std::vector<int> &adjList, std::vector<int> &numNeighboursLookup,
            std::vector<int> &path_counts, std::vector<int> &depths,
            int numVertices, int startNode) {
  // Create a queue for bc
  std::queue<int> q;
  q.push(startNode);

  path_counts[startNode] = 1;
  for (int i = 0; i < numVertices; ++i) 
    depths[i] = -1;

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    // auto du = depths[u];
    // auto path_count_u = path_counts[u];

    for (int i = 0; i < numNeighboursLookup[u]; ++i) {
      int v = adjList[u * numVertices + i];

      auto du = depths[u];
      auto path_count_u = path_counts[u];
      auto dv = depths[v];
      // printf("u: %d, v: %d, du: %d, du: %d\n", u, v, du, dv);
      if (dv == -1) {
        dv = du + 1;
        depths[v] = dv;
        q.push(v);
      }

      if (dv == du + 1) {
        path_counts[v] += path_count_u;
      }
    }
  }
}

// Function to add an edge to the graph
void addEdge(int u, int v, std::vector<int> &adjList,
             std::vector<int> &numNeighboursLookup, int numVertices) {
  adjList[u * numVertices + numNeighboursLookup[u]] = v;
  numNeighboursLookup[u]++;
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
#else
  auto d_selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif
  try {
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
#else
    auto d_selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif
    try {
      ////////////////////////////////////////////////////////////////////////////
      ////// Read graph
      ////////////////////////////////////////////////////////////////////////////
      int numVertices = 0, numEdges = 0;

      // Read the data set and get the number of nodes (n) and edges (e)
      // int n, e;
      const int MAX_LINE_LENGTH = 1000;
      char str[MAX_LINE_LENGTH];
      char ch = getc(fp);
      while (ch == '#') {
        fgets(str, MAX_LINE_LENGTH - 1, fp);
        // Debug: print title of the data set
        // printf("%s",str);
        sscanf(str, "%*s %d %*s %d", &numVertices,
               &numEdges); // number of nodes
        ch = getc(fp);
      }
      ungetc(ch, fp);

      // DEBUG: Print the number of nodes and edges, skip everything else
      printf("\nGraph data: Nodes: %d, Edges: %d \n\n", numVertices, numEdges);

      // Adjacency list representation of the graph
      std::vector<int> adjList(numVertices * numVertices, 0);
      std::vector<int> numNeighboursLookup(numVertices, 0);

      int fromNode, toNode;
      int nextNormalizedVertexId = 0;
      std::map<int, int> normalizedVertedId;
      while (!feof(fp)) {
        fscanf(fp, "%d%*s%d", &fromNode, &toNode);

        if (normalizedVertedId.find(fromNode) == normalizedVertedId.end())
          normalizedVertedId[fromNode] = nextNormalizedVertexId++;
        if (normalizedVertedId.find(toNode) == normalizedVertedId.end())
          normalizedVertedId[toNode] = nextNormalizedVertexId++;

        addEdge(normalizedVertedId[fromNode], normalizedVertedId[toNode],
                adjList, numNeighboursLookup, numVertices);
      }
      ////////////////////////////////////////////////////////////////////////////
      ////// END Read graph
      ////////////////////////////////////////////////////////////////////////////

      // Mark all the vertices as not depths
      std::vector<int> depths(numVertices, -1);
      std::vector<int> depths_cpu(numVertices, -1);
      std::vector<int> path_counts(numVertices, 0);
      std::vector<int> path_counts_cpu(numVertices, 0);

      property_list properties{property::queue::enable_profiling()};
      queue q(d_selector, exception_handler, properties);
      std::cout << "Running on device: "
                << q.get_device().get_info<info::device::name>() << "\n";

      // Perform bc traversal starting from vertex 0
      bc_cpu(adjList, numNeighboursLookup, path_counts_cpu, depths_cpu,
             numVertices, 0);

      auto kernelTime = kernel_fpga(q, adjList, numNeighboursLookup,
                                    path_counts, depths, numVertices, 0);
      std::cout << "\nKernel time (ms): " << kernelTime << "\n";

      if (std::equal(depths.begin(), depths.end(), depths_cpu.begin()) &&
          std::equal(path_counts.begin(), path_counts.end(), path_counts_cpu.begin())) {
        std::cout << "Passed\n";
      } else {
        std::cout << "Failed\n";
      }

    } catch (exception const &e) {
      std::cout << "An exception was caught.\n";
      std::terminate();
    }

    return 0;
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}