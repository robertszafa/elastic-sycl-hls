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

#include "StreamingMemory.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

// Set the damping factor 'd' 
constexpr int d = 2;


double page_rank_kernel(queue &q, const std::vector<int> &h_row_ptr,
                        const std::vector<int> &h_col_idx,
                        std::vector<int> &h_val, std::vector<int> &h_p,
                        const int numNodes, const int maxIters) {
  const int *row_ptr = fpga_tools::toDevice(h_row_ptr, q);
  const int *col_idx = fpga_tools::toDevice(h_col_idx, q);
  const int *val = fpga_tools::toDevice(h_val, q);
  int *p = fpga_tools::toDevice(h_p, q);
  int *p_new = fpga_tools::toDevice(h_p, q);
  
  constexpr int LOOP_DEPTH = 3;

  constexpr int NUM_LOADS_p_new = 2;
  constexpr int NUM_STORES_p_new = 2;
  constexpr int LOOP_DEPTH_p_new = 3;
  using LoadAddrPipes_p_new = PipeArray<class _LoadAddr_p_new, ld_req_t<NUM_STORES_p_new, LOOP_DEPTH_p_new>, 16, NUM_LOADS_p_new>;
  using LoadValPipes_p_new = PipeArray<class _LoadVal_p_new, int, 16, NUM_LOADS_p_new>;
  using StoreAddrPipes_p_new = PipeArray<class _StoreAddr_p_new, st_req_t<LOOP_DEPTH_p_new>, 16, NUM_STORES_p_new>;
  using StoreValPipes_p_new = PipeArray<class _StoreVal_p_new, int, 16, NUM_STORES_p_new>;
  
  constexpr int NUM_LOADS_p = 1;
  constexpr int NUM_STORES_p = 1;
  constexpr int LOOP_DEPTH_p = 3;
  using LoadAddrPipes_p = PipeArray<class _LoadAddr_p, ld_req_t<NUM_STORES_p, LOOP_DEPTH_p>, 16, NUM_LOADS_p>;
  using LoadValPipes_p = PipeArray<class _LoadVal_p, int, 16, NUM_LOADS_p>;
  using StoreAddrPipes_p = PipeArray<class _StoreAddr_p, st_req_t<LOOP_DEPTH_p>, 16, NUM_STORES_p>;
  using StoreValPipes_p = PipeArray<class _StoreVal_p, int, 16, NUM_STORES_p>;

  // using LoopPredPipe = PipeArray<class _LoopPredPipe, bool, 16, LOOP_DEPTH_p, LOOP_DEPTH_p>;

  q.single_task<class AGU_l0>([=]() [[intel::kernel_args_restrict]] {
    st_req_t<LOOP_DEPTH> st_req {0u};
    InitBundle(st_req.sched, 0u);
    InitBundle(st_req.isMaxIter, false);

    for (uint iter = 0; iter < maxIters; ++iter) {
      st_req.sched[0]++;
      st_req.isMaxIter[0] = (iter + 1) == maxIters;

      for (uint i = 0; i < numNodes; i++) {
        st_req.sched[1]++;
        st_req.isMaxIter[1] = (i + 1) == numNodes;

        st_req.addr = i;
        StoreAddrPipes_p_new::PipeAt<0>::write(st_req);
      }
    }

    st_req.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req.sched, SCHED_SENTINEL);
    InitBundle(st_req.isMaxIter, true);
    StoreAddrPipes_p_new::PipeAt<0>::write(st_req);

    // PRINTF("** DONE AGU 0\n");
  });

  q.single_task<class AGU_l1>([=]() [[intel::kernel_args_restrict]] {
    st_req_t<LOOP_DEPTH> st_req_p_new {0u};
    InitBundle(st_req_p_new.sched, 0u);
    InitBundle(st_req_p_new.isMaxIter, false);

    ld_req_t<NUM_STORES_p_new, LOOP_DEPTH> ld_req_p_new {0u};
    InitBundle(ld_req_p_new.sched, 0u);
    InitBundle(ld_req_p_new.posDepDist, false);
    InitBundle(ld_req_p_new.isMaxIter, false);

    ld_req_t<NUM_STORES_p, LOOP_DEPTH> ld_req_p {0u};
    InitBundle(ld_req_p.sched, 0u);
    InitBundle(ld_req_p.posDepDist, false);
    InitBundle(ld_req_p.isMaxIter, false);

    for (uint iter = 0; iter < maxIters; ++iter) {
      st_req_p_new.sched[0]++;
      st_req_p_new.isMaxIter[0] = (iter + 1) == maxIters;

      ld_req_p_new.sched[0]++;
      ld_req_p_new.isMaxIter[0] = (iter + 1) == maxIters;

      ld_req_p.sched[0]++;
      ld_req_p.isMaxIter[0] = (iter + 1) == maxIters;

      int rowel = 0;
      int curcol = 0;

      for (uint i = 0; i < numNodes; i++) {
        st_req_p_new.sched[1]++;
        st_req_p_new.isMaxIter[1] = (i+1) == numNodes;

        ld_req_p_new.sched[1]++;
        ld_req_p_new.isMaxIter[1] = (i+1) == numNodes;

        ld_req_p.sched[1]++;
        ld_req_p.isMaxIter[1] = (i+1) == numNodes;

        rowel = row_ptr[i + 1] - row_ptr[i];

        for (int j = 0; j < rowel; j++) {
          st_req_p_new.sched[2]++;
          st_req_p_new.isMaxIter[2] = (j+1) == rowel;

          ld_req_p_new.sched[2]++;
          ld_req_p_new.isMaxIter[2] = (j+1) == rowel;

          ld_req_p.sched[2]++;
          ld_req_p.isMaxIter[2] = (j+1) == rowel;

          ld_req_p.addr = i;
          LoadAddrPipes_p::PipeAt<0>::write(ld_req_p);

          ld_req_p_new.addr = col_idx[curcol];
          ld_req_p_new.posDepDist[1] = ld_req_p_new.addr > st_req_p_new.addr;
          LoadAddrPipes_p_new::PipeAt<0>::write(ld_req_p_new);

          st_req_p_new.addr = col_idx[curcol];
          StoreAddrPipes_p_new::PipeAt<1>::write(st_req_p_new);

          curcol++;
        }
      }
    }

    ld_req_p.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_p.sched, SCHED_SENTINEL);
    InitBundle(ld_req_p.isMaxIter, true);
    InitBundle(ld_req_p.posDepDist, true);
    LoadAddrPipes_p::PipeAt<0>::write(ld_req_p);

    ld_req_p_new.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_p_new.sched, SCHED_SENTINEL);
    InitBundle(ld_req_p_new.isMaxIter, true);
    InitBundle(ld_req_p_new.posDepDist, true);
    LoadAddrPipes_p_new::PipeAt<0>::write(ld_req_p_new);

    st_req_p_new.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req_p_new.sched, SCHED_SENTINEL);
    InitBundle(st_req_p_new.isMaxIter, true);
    StoreAddrPipes_p_new::PipeAt<1>::write(st_req_p_new);

    // PRINTF("** DONE AGU 1\n");
  });

  q.single_task<class AGU_l2>([=]() [[intel::kernel_args_restrict]] {
    st_req_t<LOOP_DEPTH> st_req_p {0u};
    InitBundle(st_req_p.sched, 0u);
    InitBundle(st_req_p.isMaxIter, false);

    ld_req_t<NUM_STORES_p_new, LOOP_DEPTH> ld_req_p_new {0u};
    InitBundle(ld_req_p_new.sched, 0u);
    InitBundle(ld_req_p_new.posDepDist, false);
    InitBundle(ld_req_p_new.isMaxIter, false);

    for (uint iter = 0; iter < maxIters; ++iter) {
      st_req_p.sched[0]++;
      st_req_p.isMaxIter[0] = (iter + 1) == maxIters;

      ld_req_p_new.sched[0]++;
      ld_req_p_new.isMaxIter[0] = (iter + 1) == maxIters;

      for (uint i = 0; i < numNodes; i++) {
        st_req_p.sched[1]++;
        st_req_p.isMaxIter[1] = (i+1) == numNodes;

        ld_req_p_new.sched[1]++;
        ld_req_p_new.isMaxIter[1] = (i+1) == numNodes;

        ld_req_p_new.addr = i;
        LoadAddrPipes_p_new::PipeAt<1>::write(ld_req_p_new);

        st_req_p.addr = i;
        StoreAddrPipes_p::PipeAt<0>::write(st_req_p);
      }
    }

    ld_req_p_new.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_p_new.sched, SCHED_SENTINEL);
    InitBundle(ld_req_p_new.isMaxIter, true);
    InitBundle(ld_req_p_new.posDepDist, true);
    LoadAddrPipes_p_new::PipeAt<1>::write(ld_req_p_new);

    st_req_p.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req_p.sched, SCHED_SENTINEL);
    InitBundle(st_req_p.isMaxIter, true);
    StoreAddrPipes_p::PipeAt<0>::write(st_req_p);

    // PRINTF("** DONE AGU 1\n");
  });

  auto memEvents =
      StreamingMemory<4, LoadAddrPipes_p_new, LoadValPipes_p_new,
                      StoreAddrPipes_p_new, StoreValPipes_p_new,
                      NUM_LOADS_p_new, NUM_STORES_p_new, LOOP_DEPTH_p_new>(
          q, p_new);

  auto memEvents2 =
      StreamingMemory<5, LoadAddrPipes_p, LoadValPipes_p, StoreAddrPipes_p,
                      StoreValPipes_p, NUM_LOADS_p, NUM_STORES_p, LOOP_DEPTH_p>(
          q, p);

  auto event1 = q.single_task<class MainKernel0>([=]() [[intel::kernel_args_restrict]] {
    for (uint iter = 0; iter < maxIters; ++iter) {
      for (uint i = 0; i < numNodes; i++) {
        StoreValPipes_p_new::PipeAt<0>::write(0);
      }
    }

    // PRINTF("** DONE MainKernel0\n");
  });

  auto event2 = q.single_task<class MainKernel1>([=]() [[intel::kernel_args_restrict]] {
    for (uint iter = 0; iter < maxIters; ++iter) {
      int curcol = 0;
      for (uint i = 0; i < numNodes; i++) {

        int rowel = row_ptr[i + 1] - row_ptr[i];
        for (int j = 0; j < rowel; j++) {
          auto LoadVal_p = LoadValPipes_p::PipeAt<0>::read();
          auto LoadVal_p_new = LoadValPipes_p_new::PipeAt<0>::read();
          auto StoreVal_p_new = LoadVal_p_new + val[curcol] * LoadVal_p;
          StoreValPipes_p_new::PipeAt<1>::write(StoreVal_p_new);

          curcol++;
        }
      }
    }

    // PRINTF("** DONE MainKernel1\n");
  });

  auto event3 = q.single_task<class MainKernel2>([=]() [[intel::kernel_args_restrict]] {
    for (uint iter = 0; iter < maxIters; ++iter) {
      for (uint i = 0; i < numNodes; i++) {
        auto LoadVal_p_new = LoadValPipes_p_new::PipeAt<1>::read();

        // p[i] = d * p_new[i] + (100 - d) / numNodes;
        // auto valP = d * LoadVal_p_new + (100 - d) / numNodes;
        auto valP = d * LoadVal_p_new + d*2;
        StoreValPipes_p::PipeAt<0>::write(valP);
      }
    }

    // PRINTF("** DONE MainKernel2\n");
  });

  event1.wait();
  event2.wait();
  event3.wait();

  for (auto &e : memEvents) e.wait();
  for (auto &e2 : memEvents2) e2.wait();

  q.copy(p, h_p.data(), h_p.size()).wait();

  sycl::free((void*) row_ptr, q);
  sycl::free((void*) col_idx, q);
  sycl::free((void*) val, q);
  sycl::free(p, q);
  sycl::free(p_new, q);

  auto start = event1.get_profiling_info<info::event_profiling::command_start>();
  auto end = event3.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void page_rank_cpu(const std::vector<int> &row_ptr,
                   const std::vector<int> &col_idx, std::vector<int> &val,
                   std::vector<int> &p, const int numNodes,
                   const int maxIters) {
  std::vector<int> p_new(p.size());

  for (int iter = 0; iter < maxIters; ++iter) {

    // Initialize p_new as a vector of n 0.0 cells
    for (int i = 0; i < numNodes; i++) {
      p_new[i] = 0; // id = iter
    }

    int rowel = 0;
    int curcol = 0;

    // Sparse PageRank algorithm using an adjacency matrix in CSR format.
    for (int i = 0; i < numNodes; i++) {
      rowel = row_ptr[i + 1] - row_ptr[i];
      // std::cout << "Mem5 LdPort0 a" << i << " val=" << p[i] << "\n";
      // if (rowel < 0)
      //   std::cout << "i = " << i << " is negative rowel = " << rowel  << "\n";
      for (int j = 0; j < rowel; j++) {
        // std::cout << "Mem4 LdPort0 a" << col_idx[curcol] << " val=" << p_new[col_idx[curcol]] << "\n";
        p_new[col_idx[curcol]] += val[curcol] * p[i]; // id = i = (numNodes-1)
        // std::cout << "Mem4 St1 a" << col_idx[curcol] << " val=" << p_new[col_idx[curcol]] << "\n";
        curcol++;
      }
    }

    for (int i = 0; i < numNodes; i++) {
      // p[i] = d * p_new[i] + (100 - d) / numNodes;
      // std::cout << "Mem5 LdPort1 a" << i << " val=" << p_new[i] << "\n";
      p[i] = d * p_new[i] + d*2;
      // std::cout << "Mem5 St0 a" << i << " val=" << p[i] << "\n";
    }
  }

}

int main(int argc, char *argv[]) {
  int maxIters = 10;
  FILE *fp;
  try {
    if (argc < 2) {
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
  std::vector<int> val(sizeArr, 0), p(sizeArr, 0);
  
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

    std::vector<int> p_cpu(sizeArr, 0);
    std::copy(p.begin(), p.end(), p_cpu.begin());

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    auto kernel_time = page_rank_kernel(q, row_ptr, col_idx, val, p, numNodes, maxIters);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    page_rank_cpu(row_ptr, col_idx, val, p_cpu, numNodes, maxIters);
    
    if (std::equal(p.begin(), p.end(), p_cpu.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failed\n";

      for (int i = 0; i < numNodes; ++i) {
        if (p[i] != p_cpu[i]) {
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
