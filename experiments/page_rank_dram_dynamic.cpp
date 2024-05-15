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

#include "TaggedStreamingMemory.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

// Set the damping factor 'd'
constexpr float d = 0.85;


double page_rank_kernel(queue &q, const std::vector<int> &h_row_ptr,
                        const std::vector<int> &h_col_idx,
                        std::vector<float> &h_val, std::vector<float> &h_p,
                        const int numNodes, const int maxIters) {
  const int *row_ptr = fpga_tools::toDevice(h_row_ptr, q);
  const int *col_idx = fpga_tools::toDevice(h_col_idx, q);
  const float *val = fpga_tools::toDevice(h_val, q);
  float *p = fpga_tools::toDevice(h_p, q);
  float *p_new = fpga_tools::toDevice(h_p, q);
  
  constexpr int LOOP_DEPTH = 3;

  constexpr int NUM_LOADS_p_new = 2;
  constexpr int NUM_STORES_p_new = 2;
  constexpr int LOOP_DEPTH_p_new = 3;
  using LoadAddrPipes_p_new = PipeArray<class _LoadAddr_p_new, load_req_t<NUM_STORES_p_new, LOOP_DEPTH_p_new>, 16, NUM_LOADS_p_new>;
  using LoadValPipes_p_new = PipeArray<class _LoadVal_p_new, float, 16, NUM_LOADS_p_new>;
  using StoreAddrPipes_p_new = PipeArray<class _StoreAddr_p_new, store_req_t<LOOP_DEPTH_p_new>, 16, NUM_STORES_p_new>;
  using StoreValPipes_p_new = PipeArray<class _StoreVal_p_new, float, 16, NUM_STORES_p_new>;
  
  constexpr int NUM_LOADS_p = 1;
  constexpr int NUM_STORES_p = 1;
  constexpr int LOOP_DEPTH_p = 3;
  using LoadAddrPipes_p = PipeArray<class _LoadAddr_p, load_req_t<NUM_STORES_p, LOOP_DEPTH_p>, 16, NUM_LOADS_p>;
  using LoadValPipes_p = PipeArray<class _LoadVal_p, float, 16, NUM_LOADS_p>;
  using StoreAddrPipes_p = PipeArray<class _StoreAddr_p, store_req_t<LOOP_DEPTH_p>, 16, NUM_STORES_p>;
  using StoreValPipes_p = PipeArray<class _StoreVal_p, float, 16, NUM_STORES_p>;

  // using LoopPredPipe = PipeArray<class _LoopPredPipe, bool, 16, LOOP_DEPTH_p, LOOP_DEPTH_p>;

  q.single_task<class AGU_l0>([=]() [[intel::kernel_args_restrict]] {
    store_req_t<LOOP_DEPTH> st_req {INVALID_ADDR};
    InitBundle(st_req.sched, 0u);

    for (int iter = 0; iter < maxIters; ++iter) {
      st_req.sched[0]++;

      for (int i = 0; i < numNodes; i++) {
        st_req.sched[1]++;

        st_req.addr = i;
        StoreAddrPipes_p_new::PipeAt<0>::write(st_req);
      }
    }

    st_req.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req.sched, SCHED_SENTINEL);
    StoreAddrPipes_p_new::PipeAt<0>::write(st_req);

    // PRINTF("** DONE AGU 0\n");
  });

  q.single_task<class AGU_l1>([=]() [[intel::kernel_args_restrict]] {
    store_req_t<LOOP_DEPTH> st_req_p_new {INVALID_ADDR};
    InitBundle(st_req_p_new.sched, 0u);

    load_req_t<NUM_STORES_p_new, LOOP_DEPTH> ld_req_p_new {INVALID_ADDR};
    InitBundle(ld_req_p_new.sched, 0u);
    InitBundle(ld_req_p_new.posDepDist, false);

    load_req_t<NUM_STORES_p, LOOP_DEPTH> ld_req_p {INVALID_ADDR};
    InitBundle(ld_req_p.sched, 0u);
    InitBundle(ld_req_p.posDepDist, false);

    for (int iter = 0; iter < maxIters; ++iter) {
      st_req_p_new.sched[0]++;
      ld_req_p_new.sched[0]++;
      ld_req_p.sched[0]++;

      int rowel = 0;
      int curcol = 0;

      for (int i = 0; i < numNodes; i++) {
        st_req_p_new.sched[1]++;
        ld_req_p_new.sched[1]++;
        ld_req_p.sched[1]++;

        rowel = row_ptr[i + 1] - row_ptr[i];

        int local_curcol = curcol;
        int local_i = i;
        int local_rowel = rowel;
        for (int j = 0; j < local_rowel; j++) {
          
          st_req_p_new.sched[2]++;
          ld_req_p_new.sched[2]++;
          ld_req_p.sched[2]++;

          ld_req_p.addr = local_i;
          LoadAddrPipes_p::PipeAt<0>::write(ld_req_p);

          ld_req_p_new.addr = col_idx[local_curcol];
          ld_req_p_new.posDepDist[1] = ld_req_p_new.addr > st_req_p_new.addr;
          LoadAddrPipes_p_new::PipeAt<0>::write(ld_req_p_new);

          st_req_p_new.addr = col_idx[local_curcol];
          StoreAddrPipes_p_new::PipeAt<1>::write(st_req_p_new);

          local_curcol++;
        }

        curcol += local_rowel;
      }
    }

    ld_req_p.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_p.sched, SCHED_SENTINEL);
    LoadAddrPipes_p::PipeAt<0>::write(ld_req_p);

    ld_req_p_new.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_p_new.sched, SCHED_SENTINEL);
    LoadAddrPipes_p_new::PipeAt<0>::write(ld_req_p_new);

    st_req_p_new.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req_p_new.sched, SCHED_SENTINEL);
    StoreAddrPipes_p_new::PipeAt<1>::write(st_req_p_new);

    // PRINTF("** DONE AGU 1\n");
  });

  q.single_task<class AGU_l2>([=]() [[intel::kernel_args_restrict]] {
    store_req_t<LOOP_DEPTH> st_req_p {INVALID_ADDR};
    InitBundle(st_req_p.sched, 0u);

    load_req_t<NUM_STORES_p_new, LOOP_DEPTH> ld_req_p_new {INVALID_ADDR};
    InitBundle(ld_req_p_new.sched, 0u);
    InitBundle(ld_req_p_new.posDepDist, false);

    for (int iter = 0; iter < maxIters; ++iter) {
      st_req_p.sched[0]++;
      ld_req_p_new.sched[0]++;


      for (int i = 0; i < numNodes; i++) {
        st_req_p.sched[1]++;
        st_req_p.sched[1]++;
        ld_req_p_new.sched[1]++;

        ld_req_p_new.addr = i;
        LoadAddrPipes_p_new::PipeAt<1>::write(ld_req_p_new);

        st_req_p.addr = i;
        StoreAddrPipes_p::PipeAt<0>::write(st_req_p);
      }
    }

    ld_req_p_new.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_p_new.sched, SCHED_SENTINEL);
    LoadAddrPipes_p_new::PipeAt<1>::write(ld_req_p_new);

    st_req_p.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req_p.sched, SCHED_SENTINEL);
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
    for (int iter = 0; iter < maxIters; ++iter) {
      for (int i = 0; i < numNodes; i++) {
        StoreValPipes_p_new::PipeAt<0>::write(0.0f);
      }
    }

    // PRINTF("** DONE MainKernel0\n");
  });

  auto event2 = q.single_task<class MainKernel1>([=]() [[intel::kernel_args_restrict]] {
    for (int iter = 0; iter < maxIters; ++iter) {
      int curcol = 0;
      for (int i = 0; i < numNodes; i++) {

        int rowel = row_ptr[i + 1] - row_ptr[i];
        int local_curcol = curcol;
        for (int j = 0; j < rowel; j++) {
          
          auto LoadVal_p = LoadValPipes_p::PipeAt<0>::read();
          // PRINTF("ld0 (%d, %d, %d), p[%d] = %f\n", iter, i, j, i, LoadVal_p);

          auto LoadVal_p_new = LoadValPipes_p_new::PipeAt<0>::read();
          // PRINTF("ld1 (%d, %d, %d), p_new[%d] = %f\n", iter, i, j,
                //  col_idx[local_curcol], LoadVal_p_new);

          auto StoreVal_p_new = LoadVal_p_new + val[local_curcol] * LoadVal_p;
          // PRINTF("st1 (%d, %d, %d), p_new[%d] = %f\n", iter, i, j,
                //  col_idx[local_curcol], StoreVal_p_new);

          StoreValPipes_p_new::PipeAt<1>::write(StoreVal_p_new);

          local_curcol++;
        }
        curcol += rowel;
      }
    }

    // PRINTF("** DONE MainKernel1\n");
  });

  auto event3 = q.single_task<class MainKernel2>([=]() [[intel::kernel_args_restrict]] {
    for (int iter = 0; iter < maxIters; ++iter) {
      for (int i = 0; i < numNodes; i++) {
        auto LoadVal_p_new = LoadValPipes_p_new::PipeAt<1>::read();
        // PRINTF("ld2 (%d, %d), p_new[%d] = %f\n", iter, i, i, LoadVal_p_new);

        auto valP = d * LoadVal_p_new + (1.0f - d) / numNodes;
        // PRINTF("st2 (%d, %d), p[%d] = %f\n", iter, i, i, valP);
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

inline bool almost_equal(const float x, const float y) {
  const float ulpFloat = static_cast<float>(2);
  const float tolerance = 0.01f;
  return fabsf(x - y) <=
             tolerance * fabsf(x + y) * ulpFloat ||
         fabsf(x - y) < std::numeric_limits<float>::min();
}

void page_rank_cpu(const std::vector<int> &row_ptr,
                   const std::vector<int> &col_idx, std::vector<float> &val,
                   std::vector<float> &p, const int numNodes,
                   const int maxIters) {
  std::vector<float> p_new(p.size());

  for (int iter = 0; iter < maxIters; ++iter) {

    // Initialize p_new as a vector of n 0.0 cells
    for (int i = 0; i < numNodes; i++) {
      p_new[i] = 0.0f; // id = iter
    }

    int rowel = 0;
    int curcol = 0;

    // Sparse PageRank algorithm using an adjacency matrix in CSR format.
    for (int i = 0; i < numNodes; i++) {
      rowel = row_ptr[i + 1] - row_ptr[i];
      for (int j = 0; j < rowel; j++) {
        p_new[col_idx[curcol]] += val[curcol] * p[i]; // id = i = (numNodes-1)
        curcol++;
      }
    }

    for (int i = 0; i < numNodes; i++) {
      p[i] = d * p_new[i] + (1.0f - d) / numNodes;
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
