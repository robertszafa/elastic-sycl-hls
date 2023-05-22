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

// Forward declare kernel name.
class MainKernel;

double sssp_kernel(queue & q, const std::vector<int> &h_G, std::vector<int> &h_cost,
                   std::vector<int> &h_distance, std::vector<int> &h_visited,
                   std::vector<int> &h_pred, int numNodes, int startNode) {

  int *G = fpga_tools::toDevice(h_G, q);
  int *cost = fpga_tools::toDevice(h_cost, q);
  int *distance = fpga_tools::toDevice(h_distance, q);
  // int *visited = fpga_tools::toDevice(h_visited, q);
  int *pred = fpga_tools::toDevice(h_pred, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {

  [[intel::singlepump]] 
  [[intel::max_replicates(1)]] 
  [[intel::numbanks(1)]] 
    int visited[40];

    int count = 1;
    while (count < numNodes - 1) {
      int mindist = INT_MAX;
      int nextnode = 0;
      // nextnode gives the node at minimum distance
      for (int i = 0; i < numNodes; i++) {
        bool isvis = visited[i];
        if (!isvis && distance[i] < mindist) {
          mindist = distance[i];
          nextnode = i;
        }
      }

      // check if a better path exists through nextnode
      visited[nextnode] = 1;

      for (int i = 0; i < numNodes; i++) {
        bool isvis = visited[i];
        if (!isvis && mindist + cost[nextnode * numNodes + i] < distance[i]) {
          distance[i] = mindist + cost[nextnode * numNodes + i];
          pred[i] = nextnode;
        }
      }
      count++;
    }
  });

  event.wait();
  q.copy(distance, h_distance.data(), h_distance.size()).wait();

  sycl::free(G, q);
  sycl::free(cost, q);
  sycl::free(distance, q);
  // sycl::free(visited, q);
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

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<int> &G, std::vector<int> &cost, std::vector<int> &distance,
               std::vector<int> &visited, std::vector<int> &pred, int numNodes, int startNode) {
  
  // Create adjencency matrix
  for (int i = 0; i < numNodes; i++) {
    for (int j = 0; j < numNodes; j++) {
      G[i * numNodes + j] = 0;
      if (i != j && (i + j) % 2 == 0) {
        G[i * numNodes + j] = 1;
      }
    }
  }

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
  int NUM_NODES = 200;

  auto DATA_DISTR = data_distribution::ALL_WAIT;
  int PERCENTAGE = 5;
  try {
    if (argc > 1) {
      NUM_NODES = int(atoi(argv[1]));
    }
    if (argc > 2) {
      DATA_DISTR = data_distribution(atoi(argv[2]));
    }
    if (argc > 3) {
      PERCENTAGE = int(atoi(argv[3]));
      std::cout << "Percentage is " << PERCENTAGE << "\n";
      if (PERCENTAGE < 0 || PERCENTAGE > 100)
        throw std::invalid_argument("Invalid percentage.");
    }
  } catch (exception const &e) {
    std::cout << "Incorrect argv.\nUsage:\n";
    std::cout << "  ./executable [num_items] [data_distribution (0/1/2)] [PERCENTAGE (only for "
                 "data_distr 2)]\n";
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

    std::vector<int> G(NUM_NODES * NUM_NODES);
    std::vector<int> cost(NUM_NODES * NUM_NODES);
    std::vector<int> distance(NUM_NODES);
    std::vector<int> visited(NUM_NODES);
    std::vector<int> pred(NUM_NODES);
    int startNode = 0;

    init_data(G, cost, distance, visited, pred, NUM_NODES, startNode);
    std::vector<int> distance_cpu(NUM_NODES);
    std::copy_n(distance.data(), NUM_NODES, distance_cpu.data());

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = sssp_kernel(q, G, cost, distance, visited, pred, NUM_NODES, startNode);

    // Wait for all work to finish.
    q.wait();

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    sssp_cpu(G, cost, distance_cpu, visited, pred, NUM_NODES, startNode);
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

