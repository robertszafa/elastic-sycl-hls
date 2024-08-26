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
constexpr int START_DIST = 1 << 29;

// Forward declare kernel name.
class MainKernel;

double kernel_fpga(queue &q, const std::vector<int> &h_adjList,
                   const std::vector<int> &h_numNeighboursLookup,
                   const std::vector<int> &h_cost, std::vector<int> &h_dist,
                   int numVertices, int startNode) {
  int *adjList = fpga_tools::toDevice(h_adjList, q);
  int *numNeighboursLookup = fpga_tools::toDevice(h_numNeighboursLookup, q);
  int *cost = fpga_tools::toDevice(h_cost, q);
  int *dist_dram = fpga_tools::toDevice(h_dist, q);

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
        // PRINTF("rear = %d\n", rear);
      } 

      if (rear == front) {
        bool succ3 = false;
        QueueIsEmptyPipe::read(succ3);
        if (succ3)
          break;
      }
    }

    // PRINTF("EXITING QUEUE KERNEL\nDONE %d NODES\n", front);
    // Sentinel value.
    CurrentNodePopPipe::write(-1);
  });

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int dist[MAX_VERTICES];
    for (int i = 0; i < numVertices; ++i)
      dist[i] = START_DIST; // inf

    dist[startNode] = 0;
    CurrentNodePushPipe::write(startNode);

    int qsize = 1;
    while (true) {
      int u = CurrentNodePopPipe::read();
      qsize--;
      if (u == -1)
        break;

      int pushedCount = 0;
      for (int i = 0; i < numNeighboursLookup[u]; ++i) {
        int v = adjList[u * numVertices + i];

        int dv = dist[v];
        int du = dist[u];
        
        if (du + cost[u * numVertices + v] < dv) {
          CurrentNodePushPipe::write(v);
          pushedCount++;
        }
        auto val = sycl::min(du + cost[u * numVertices + v], dv); 
        dist[v] = val;
      }
      qsize += pushedCount;

      // PRINTF("Qsize = %d\n", qsize);
      if (qsize == 0)
        QueueIsEmptyPipe::write(1);
    }

    // PRINTF("Exit main kernel\n");

    for (int i = 0; i < numVertices; ++i) {
      dist_dram[i] = dist[i];
    }
  });

  event.wait();
  queueEvent.wait();

  q.copy(dist_dram, h_dist.data(), h_dist.size()).wait();

  sycl::free(adjList, q);
  sycl::free(cost, q);
  sycl::free(numNeighboursLookup, q);
  sycl::free(dist_dram, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;
  return time_in_ms;
}

// Function to perform Breadth First Search on a graph
// represented using adjacency list
void sssp_cpu(const std::vector<int> &adjList,
              const std::vector<int> &numNeighboursLookup,
              const std::vector<int> &cost, std::vector<int> &dist,
              int numVertices, int startNode) {
  // Create a queue for bc
  std::queue<int> q;
  q.push(startNode);

  for (int i = 0; i < numVertices; ++i)  {
    dist[i] = START_DIST;
  }
  dist[startNode] = 0;

  while (!q.empty()) {
    int u = q.front();
    q.pop();

    for (int i = 0; i < numNeighboursLookup[u]; ++i) {
      int v = adjList[u * numVertices + i];

      auto du = dist[u];

      if (du + cost[u * numVertices + v] < dist[v]) {
        dist[v] = du + cost[u * numVertices + v];
        q.push(v);
      }
    }
  }
}

void init_data(std::vector<int> &G, std::vector<int> &cost, int numNodes) {
  // int cost[MAX][MAX],distance[MAX],pred[MAX];
  // int visited[MAX],count,mindistance,nextnode,i,j;
  // pred[] stores the predecessor of each node
  // count gives the number of nodes seen so far
  // create the cost matrix
  for (int i = 0; i < numNodes; i++) {
    for (int j = 0; j < numNodes; j++) {
      if (G[i * numNodes + j] == 0)
        cost[i * numNodes + j] = INT_MAX;
      else
        cost[i * numNodes + j] = G[i * numNodes + j];
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
      std::vector<int> G(numVertices * numVertices, 0);
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
        G[normalizedVertedId[fromNode] * numVertices + normalizedVertedId[toNode]] = 1;
      }
      ////////////////////////////////////////////////////////////////////////////
      ////// END Read graph
      ////////////////////////////////////////////////////////////////////////////

      // Mark all the vertices as not depths
      std::vector<int> cost(numVertices * numVertices, 0);
      std::vector<int> dist(numVertices, 0);
      std::vector<int> dist_cpu(numVertices, 0);

      init_data(G, cost, numVertices);

      property_list properties{property::queue::enable_profiling()};
      queue q(d_selector, exception_handler, properties);
      std::cout << "Running on device: "
                << q.get_device().get_info<info::device::name>() << "\n";

      // Perform bc traversal starting from vertex 0
      sssp_cpu(adjList, numNeighboursLookup, cost, dist_cpu, numVertices, 0);

      auto kernelTime = kernel_fpga(q, adjList, numNeighboursLookup, cost,
                                    dist, numVertices, 0);
      std::cout << "\nKernel time (ms): " << kernelTime << "\n";

      if (std::equal(dist.begin(), dist.end(), dist_cpu.begin())) {
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
