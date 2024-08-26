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

#include "device_print.hpp"
#include "exception_handler.hpp"

#include "LoadStoreQueueBRAM.hpp"
#include "memory_utils.hpp"

using namespace sycl;

constexpr int MAX_VERTICES = 2000;

// Forward declare kernel name.
class MainKernel;
class MainKernel_AGU_0;

double kernel_fpga(queue &q, std::vector<int> &h_adjList,
                   std::vector<int> &h_numNeighbours,
                   std::vector<int> &h_visited, int numVertices,
                   int startNode) {
  int *adjList = fpga_tools::toDevice(h_adjList, q);
  int *numNeighbours = fpga_tools::toDevice(h_numNeighbours, q);
  int *visited_dram = fpga_tools::toDevice(h_visited, q);


  using CurrentNodePopPipe = pipe<class _CurrentNodePopPipe, int, 32>;
  using CurrentNodePushPipe = pipe<class _CurrentNodePushPipe, int, 32>;

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
      } 

      if (rear == front) {
        bool succ3 = false;
        QueueIsEmptyPipe::read(succ3);
        // if (front == rear && succ3)
        if (succ3) {
          // PRINTF("rear = %d\n", rear);
          break;
        }
      }
    }

    // PRINTF("EXITING QUEUE KERNEL\nDONE %d NODES\n", front);
    // Sentinel value.
    PRINTF("rear = %d\n", rear);
    CurrentNodePopPipe::write(-1);
  });

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    bool visited[MAX_VERTICES];
    visited[startNode] = true;

    int qsize = 1;
    while (true) {
      int currentNode = CurrentNodePopPipe::read();
      qsize--;
      if (currentNode == -1)
        break;

      int pushedCount = 0;
      for (int i = 0; i < numNeighbours[currentNode]; ++i) {
        int neighbor = adjList[currentNode * numVertices + i];
        if (!visited[neighbor]) {
          CurrentNodePushPipe::write(neighbor);
          pushedCount++;
        }
        visited[neighbor] = 1; // outside
      }
      qsize += pushedCount;

      if (qsize == 0)
        QueueIsEmptyPipe::write(1);
    }

    for (int i = 0; i < numVertices; ++i)
      visited_dram[i] = int(visited[i]);
  });

  event.wait();
  queueEvent.wait();

  q.copy(visited_dram, h_visited.data(), h_visited.size()).wait();
  sycl::free(adjList, q);
  sycl::free(numNeighbours, q);
  sycl::free(visited_dram, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;
  return time_in_ms;
}

// Function to perform Breadth First Search on a graph
// represented using adjacency list
void bfs_cpu(std::vector<int> &adjList, std::vector<int> &numNeighbours,
             std::vector<int> &visited, int numVertices, int startNode) {
  // Create a queue for BFS
  std::queue<int> q;

  // Mark the current node as visited and enqueue it
  visited[startNode] = 1;
  q.push(startNode);

  // Iterate over the queue
  while (!q.empty()) {
    // Dequeue a vertex from queue and print it
    int currentNode = q.front();
    q.pop();

    // Get all adjacent vertices of the dequeued vertex
    // currentNode If an adjacent has not been visited,
    // then mark it visited and enqueue it
    // for (int neighbor : adjList[currentNode]) {
    for (int i = 0; i < numNeighbours[currentNode]; ++i) {
      int neighbor = adjList[currentNode * numVertices + i];
      if (visited[neighbor] == 0) {
        visited[neighbor] = 1;
        q.push(neighbor);
      }
    }
  }
}

// Function to add an edge to the graph
void addEdge(int u, int v, std::vector<int> &adjList,
             std::vector<int> &numNeighbours, int numVertices) {
  adjList[u * numVertices + numNeighbours[u]] = v;
  numNeighbours[u]++;
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
  ////////////////////////////////////////////////////////////////////////////
  ////// Read graph
  ////////////////////////////////////////////////////////////////////////////
  int numVertices = 0, numEdges = 0;

  // Read the data set and get the number of nodes (n) and edges (e)
  // int n, e;
  char str[1000];
  char ch = getc(fp);
  while (ch == '#') {
    fgets(str, 1000 - 1, fp);
    // Debug: print title of the data set
    // printf("%s",str);
    sscanf(str, "%*s %d %*s %d", &numVertices, &numEdges); // number of nodes
    ch = getc(fp);
  }
  ungetc(ch, fp);

  // DEBUG: Print the number of nodes and edges, skip everything else
  printf("\nGraph data: Nodes: %d, Edges: %d \n\n", numVertices, numEdges);

  // Adjacency list representation of the graph
  std::vector<int> adjList(numVertices * numVertices, -1);
  std::vector<int> numNeighbours(numVertices, 0);

  int fromNode, toNode;
  int nextNormalizedVertexId = 0;
  std::map<int, int> normalizedVertedId;
  while (!feof(fp)) {
    fscanf(fp, "%d%*s%d", &fromNode, &toNode);

    if (normalizedVertedId.find(fromNode) == normalizedVertedId.end()) 
      normalizedVertedId[fromNode] = nextNormalizedVertexId++;
    if (normalizedVertedId.find(toNode) == normalizedVertedId.end()) 
      normalizedVertedId[toNode] = nextNormalizedVertexId++;

    addEdge(normalizedVertedId[fromNode], normalizedVertedId[toNode], adjList,
            numNeighbours, numVertices);
  }
  ////////////////////////////////////////////////////////////////////////////
  ////// END Read graph
  ////////////////////////////////////////////////////////////////////////////

  // Mark all the vertices as not visited
  std::vector<int> visited(numVertices, 0);
  std::vector<int> visited_cpu(numVertices, 0);

  property_list properties{property::queue::enable_profiling()};
  queue q(d_selector, exception_handler, properties);
  std::cout << "Running on device: "
            << q.get_device().get_info<info::device::name>() << "\n";

  // Perform BFS traversal starting from vertex 0
  bfs_cpu(adjList, numNeighbours, visited_cpu, numVertices, 0);
  auto kernelTime =
      kernel_fpga(q, adjList, numNeighbours, visited, numVertices, 0);

  std::cout << "\nKernel time (ms): " << kernelTime << "\n";

  if (std::equal(visited.begin(), visited.end(), visited_cpu.begin())) {
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
