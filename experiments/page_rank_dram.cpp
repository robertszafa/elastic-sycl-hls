#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <vector>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "memory_utils.hpp"
#include "exception_handler.hpp"
#include "device_print.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

// Set the damping factor 'd'
constexpr float d = 0.85;


double page_rank_kernel(queue &q, const std::vector<int> &h_row_ptr,
                        const std::vector<int> &h_col_idx,
                        std::vector<float> &h_val, std::vector<float> &h_p,
                        const uint numNodes, const uint maxIters) {
  float *p_new = fpga_tools::toDevice(h_p, q);
  float *p = fpga_tools::toDevice(h_p, q);
  const int *row_ptr = fpga_tools::toDevice(h_row_ptr, q);
  const int *col_idx = fpga_tools::toDevice(h_col_idx, q);
  const float *val = fpga_tools::toDevice(h_val, q);

  std::vector<sycl::event> events;

  auto main_event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (uint iter = 0; iter < maxIters; ++iter) {

      // Initialize p_new as a vector of n 0.0 cells
      for (uint i = 0; i < numNodes; i++) {
        p_new[i] = 0.0f;
      }

      // Sparse PageRank algorithm using an adjacency matrix in CSR format.
      int curcol = 0;
      for (uint i = 0; i < numNodes; i++) {
        auto p_val = p[i];
        int rowel = row_ptr[i + 1] - row_ptr[i];
        int local_curcol = curcol;
        curcol += rowel;
        for (int j = 0; j < rowel; j++) {
          auto p_new_load = p_new[col_idx[local_curcol]];
          auto p_new_store = p_new_load + (val[local_curcol] * p_val);
          p_new[col_idx[local_curcol]] = p_new_store;

          local_curcol++;
        }
      }

      // Update p[]
      for (uint i = 0; i < numNodes; i++) {
        auto p_val = d * p_new[i] + (1.0f - d) / numNodes;
        p[i] = p_val; 
      }
    }
  });

  
  events.push_back(main_event);
  sycl::event::wait(events);
  q.copy(p, h_p.data(), h_p.size()).wait();

  sycl::free((void*) row_ptr, q);
  sycl::free((void*) col_idx, q);
  sycl::free((void*) val, q);
  sycl::free(p, q);
  sycl::free(p_new, q);

  double max_event_time = 0;
  for (auto &e : events) {
    auto start = e.get_profiling_info<info::event_profiling::command_start>();
    auto end = e.get_profiling_info<info::event_profiling::command_end>();
    double this_event_time = static_cast<double>(end - start) / 1000000;
    max_event_time = std::max(max_event_time, this_event_time);
  }

  return max_event_time;
}

void page_rank_cpu(const std::vector<int> &row_ptr,
                   const std::vector<int> &col_idx, std::vector<float> &val,
                   std::vector<float> &p, const int numNodes,
                   const int maxIters) {
  std::vector<float> p_new(p.size());

  for (int iter = 0; iter < maxIters; ++iter) {

    // Initialize p_new as a vector of n 0.0 cells
    for (int i = 0; i < numNodes; i++) {
      p_new[i] = 0.0f;
    }

    int rowel = 0;

    // Sparse PageRank algorithm using an adjacency matrix in CSR format.
    int curcol = 0;
    for (int i = 0; i < numNodes; i++) {
      rowel = row_ptr[i + 1] - row_ptr[i];
      for (int j = 0; j < rowel; j++) {
        p_new[col_idx[curcol]] += val[curcol] * p[i];
        curcol++;
      }
    }

    // Optional early termination: check if we have to stop
    // float error = 0.0;
    // for (int i = 0; i < numNodes; i++) {
    //   error = error + fabs(p_new[i] - p[i]);
    // }
    // if (error < 0.000001) {
    //   break;
    // }

    // Update p[]
    for (int i = 0; i < numNodes; i++) {
      p[i] = d * p_new[i] + (1.0f - d) / numNodes;
    }
  }
}

inline bool almost_equal(const float x, const float y) {
  const float ulpFloat = static_cast<float>(2);
  const float tolerance = 0.01f;

  return (fabsf(x - y) <= tolerance * fabsf(x + y) * ulpFloat) ||
         (fabsf(x - y) < std::numeric_limits<float>::min()) ||
         (std::isinf(x) && std::isinf(y)) || (std::isnan(x) && std::isnan(y));
}

int main(int argc, char *argv[]) {
  int maxIters = 10;
  FILE *fp;
  try {
    if (argc < 3) {
      throw std::invalid_argument("Need filename and max iters.");
    }

    char *filename = argv[1];
    // Open the data set
    // char filename[] = "/home/rob/git/elastic-sycl-hls/experiments/web-NotreDame.txt";
    if ((fp = fopen(filename, "r")) == NULL) {
      throw std::invalid_argument("Invalid filename.");
    }
    
    maxIters = int(atoi(argv[2]));
  } catch (exception const &e) {
    std::cout << "Incorrect argv.\nUsage: ./executable [maxIters]\n";
    std::terminate();
  }

  ////////////////////////////////////////////////////////////////////////////
  ////// INIT DATA
  ////////////////////////////////////////////////////////////////////////////
  int numNodes = 0, numEdges = 0;

  // Read the data set and get the number of nodes (n) and edges (e)
  // int n, e;
  char ch;
  char str[100];
  ch = getc(fp);
  while (ch == '#') {
    fgets(str, 100 - 1, fp);
    // Debug: print title of the data set
    // printf("%s",str);
    sscanf(str, "%*s %d %*s %d", &numNodes, &numEdges); // number of nodes
    ch = getc(fp);
  }
  ungetc(ch, fp);

  // DEBUG: Print the number of nodes and edges, skip everything else
  printf("\nGraph data: Nodes: %d, Edges: %d \n\n", numNodes, numEdges);

  auto sizeArr = std::max(numEdges + 1, numNodes + 1);
  std::vector<int> row_ptr(sizeArr, 0), col_idx(sizeArr, 0);
  std::vector<float> val(sizeArr, 0.0f), p(sizeArr, 0.0f);
  
  int fromnode, tonode;
  int cur_row = 0;
  int i = 0;
  // Elements for row
  int elrow = 0;
  // Cumulative numbers of elements
  int curel = 0;
  
  while(!feof(fp)){
    // fscanf(fp,"%d%d",&fromnode,&tonode);
    fscanf(fp, "%d%d", &tonode, &fromnode);
    
    if (tonode > cur_row) { // change the row
      curel = curel + elrow;
      for (int k = cur_row + 1; k <= tonode; k++) {
        row_ptr[k] = curel;
      }
      elrow = 0;
      cur_row = tonode;
    }
    val[i] = 1.0;
    col_idx[i] = fromnode;
    elrow++;
    i++;
  }
  row_ptr[cur_row+1] = curel + elrow - 1;
  ////////////////////////////////////////////////////////////////////////////
  ////// END INIT DATA
  ////////////////////////////////////////////////////////////////////////////



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

    std::vector<float> p_cpu(sizeArr, 0.0f);
    std::copy(p.begin(), p.end(), p_cpu.begin());

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    auto kernel_time = page_rank_kernel(q, row_ptr, col_idx, val, p, numNodes, maxIters);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    page_rank_cpu(row_ptr, col_idx, val, p_cpu, numNodes, maxIters);
    
    if (std::equal(p.begin(), p.end(), p_cpu.begin(), almost_equal)) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failed\n";

      for (int i = 0; i < numNodes; ++i) {
        if (!almost_equal(p[i], p_cpu[i])) {
          std::cout << i << ": fpga != cpu, " << p[i] << " != " << p_cpu[i] << "\n";
        }
      }
    }

  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
