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
  

  constexpr uint NUM_STORES_p_new = 2;
  constexpr uint NUM_LOADS_p_new = 2;

  constexpr uint NUM_STORES_p = 1;
  constexpr uint NUM_LOADS_p = 1;

  constexpr uint NUM_LOOPS = 5;
  constexpr uint NUM_AGUS = 3;

  using LoadAddrPipes_p_new =
      PipeArray<class _LoadAddr_p_new, load_req_t<NUM_STORES_p_new>, 16,
                NUM_LOADS_p_new>;
  using LoadValPipes_p_new =
      PipeArray<class _LoadVal_p_new, float, 16, NUM_LOADS_p_new>;
  using StoreAddrPipes_p_new =
      PipeArray<class _StoreAddr_p_new,
                store_req_t<NUM_LOADS_p_new, NUM_LOADS_p_new>, 16,
                NUM_STORES_p_new>;
  using StoreValPipes_p_new =
      PipeArray<class _StoreVal_p_new, float, 16, NUM_STORES_p_new>;

  using LoadAddrPipes_p = PipeArray<class _LoadAddr_p, load_req_t<NUM_LOADS_p>, 16, NUM_LOADS_p>;
  using LoadValPipes_p = PipeArray<class _LoadVal_p, float, 16, NUM_LOADS_p>;
  using StoreAddrPipes_p = PipeArray<class _StoreAddr_p, store_req_t<NUM_STORES_p, NUM_LOADS_p>, 16, NUM_STORES_p>;
  using StoreValPipes_p = PipeArray<class _StoreVal_p, float, 16, NUM_STORES_p>;

  // using MemEndSignal = pipe<class _MemEndSignal, bool, 1>;
  // using MemEndSignal2 = pipe<class _MemEndSignal_2, bool, 1>;

  // 2 stores * (3-1) AGUs.
  using StoreTagPipe_p = PipeArray<class _TagPipe_p, tag_t, 16, 2, NUM_AGUS, NUM_AGUS>;
  using LoadTagPipe_p = PipeArray<class _LoadTagPipe_p, tag_t, 16, 2,  NUM_AGUS, NUM_AGUS>;

  using StoreTagPipe_p_new = PipeArray<class _TagPipe_p_new, tag_t, 16, 2, NUM_AGUS, NUM_AGUS>;
  using LoadTagPipe_p_new = PipeArray<class _LoadTagPipe_p_new, tag_t, 16, 2,  NUM_AGUS, NUM_AGUS>;

  using LoopPredPipe = PipeArray<class _Loop0Pipe, bool, 16, 3, 2>;

  q.single_task<class AGU_l0>([=]() [[intel::kernel_args_restrict]] {
    addr_t LoadAddr = INVALID_ADDR, StoreAddr = INVALID_ADDR;

    uint storeTag_p = 0u;
    uint loadTag_p = 0u;
    tag_t storeTagBeforeLoop_p[NUM_LOOPS];
    InitBundle(storeTagBeforeLoop_p, 0u);
    tag_t loadTagBeforeLoop_p[NUM_LOOPS];
    InitBundle(loadTagBeforeLoop_p, 0u);

    uint storeTag_p_new = 0u;
    uint loadTag_p_new = 0u;
    tag_t storeTagBeforeLoop_p_new[NUM_LOOPS];
    InitBundle(storeTagBeforeLoop_p_new, 0u);
    tag_t loadTagBeforeLoop_p_new[NUM_LOOPS];
    InitBundle(loadTagBeforeLoop_p_new, 0u);


    // for (int iter = 0; iter < maxIters; ++iter) {
    while(LoopPredPipe::PipeAt<0, 0>::read()) {

      StoreTagPipe_p_new::PipeAt<0, 0, 1>::write(storeTag_p_new);
      StoreTagPipe_p_new::PipeAt<1, 0, 1>::write(storeTag_p_new + numNodes);

      storeTagBeforeLoop_p_new[1] = storeTag_p_new;
      loadTagBeforeLoop_p_new[1] = loadTag_p_new;
      for (int i = 0; i < numNodes; i++) {
        storeTag_p_new++
        StoreAddrPipes_p_new::PipeAt<0>::write({i, storeTag_p_new, storeTagBeforeLoop_p_new[1],
                                                {0u, storeTagBeforeLoop_p_new[3]},
                                                {loopTags[2], loopTags[3]}});
      }

      tag_p_new = TagPipe_p_new::PipeAt<1, 0>::read();
      loopTag = LoopTagPipe::PipeAt<1, 0>::read();
      loopTags[2] = loopTag;

      tag_p_new = TagPipe_p_new::PipeAt<2, 0>::read();
      loopTag = LoopTagPipe::PipeAt<2, 0>::read();
      loopTags[3] = loopTag;
    }

    StoreAddrPipes_p_new::PipeAt<0>::write({MAX_INT, MAX_INT, MAX_INT});

    // TagAfterStoreLoopPipes_p_new::PipeAt<1, 0>::read();

    PRINTF("** DONE AGU 0\n");
  });

  q.single_task<class AGU_l1>([=]() [[intel::kernel_args_restrict]] {
    addr_t StoreAddr1 = INVALID_ADDR;

    uint tag_p_new = 0u;
    uint tag_p = 0u;

    uint loopTag = 0u;
    tag_t loopTags[NUM_LOOPS];
    InitBundle(loopTags, 0u);

    while(LoopPredPipe::PipeAt<1, 0>::read()) {
      int rowel = 0;
      int curcol = 0;

      loopTag = LoopTagPipe::PipeAt<0, 1>::read();
      tag_p_new = TagPipe_p_new::PipeAt<0, 1>::read();
      loopTags[1] = loopTag;

      for (int i = 0; i < numNodes; i++) {
        rowel = row_ptr[i + 1] - row_ptr[i];

        int local_curcol = curcol;
        
        for (int j = 0; j < rowel; j++) {
          if (j == 0) {
            loopTag++;
          }

          LoadAddrPipes_p::PipeAt<0>::write(
              {i, tag_p, loopTag, {loopTags[3]}, {false}});

          int LoadAddr0 = col_idx[local_curcol];
          LoadAddrPipes_p_new::PipeAt<0>::write(
              {LoadAddr0,
               tag_p_new,
               loopTag,
               {loopTags[1], loopTags[2]},
               {false, LoadAddr0 > StoreAddr1}});

          tag_p_new++;
          StoreAddr1 = col_idx[local_curcol];
          StoreAddrPipes_p_new::PipeAt<1>::write(
              {StoreAddr1,
               tag_p_new,
               loopTag,
               {loopTags[1], loopTags[2]},
               {loopTags[2], loopTags[3]}});

          local_curcol++;
        }
        loopTags[2] = loopTag;

        curcol += rowel;
      }
      // PRINTF("FINAL loop tag=%d\n", loopTag);

      LoopTagPipe::PipeAt<1, 0>::write(loopTag);
      LoopTagPipe::PipeAt<1, 2>::write(loopTag);

      TagPipe_p_new::PipeAt<1, 0>::write(tag_p_new);
      TagPipe_p_new::PipeAt<1, 2>::write(tag_p_new);

      TagPipe_p::PipeAt<1, 2>::write(tag_p);

      loopTag = LoopTagPipe::PipeAt<2, 1>::read();
      loopTags[3] = loopTag;

      tag_p_new = TagPipe_p_new::PipeAt<2, 1>::read();

      tag_p = TagPipe_p::PipeAt<2, 1>::read();
    }

    LoadAddrPipes_p::PipeAt<0>::write({MAX_INT, 0, MAX_INT, {MAX_INT}});
    LoadAddrPipes_p_new::PipeAt<0>::write({MAX_INT, 0, MAX_INT, {MAX_INT, MAX_INT}});
    StoreAddrPipes_p_new::PipeAt<1>::write({MAX_INT, MAX_INT, MAX_INT});

    // minTagStore0_p = TagBeforeStoreLoopPipes_p::PipeAt<2, 1>::read();
    // tag2 = TagAfterStoreLoopPipes_p::PipeAt<2, 1>::read();
    PRINTF("** DONE AGU 1\n");
  });

  q.single_task<class AGU_l2>([=]() [[intel::kernel_args_restrict]] {
    uint tag_p_new = 0u;
    uint tag_p = 0u;

    uint loopTag = 0u;
    tag_t loopTags[NUM_LOOPS];
    InitBundle(loopTags, 0u);

    // for (int iter = 0; iter < maxIters; ++iter) {
    while(LoopPredPipe::PipeAt<2, 0>::read()) {
      loopTag = LoopTagPipe::PipeAt<0, 2>::read();
      tag_p_new = TagPipe_p_new::PipeAt<0, 2>::read();
      loopTags[1] = loopTag;

      loopTag = LoopTagPipe::PipeAt<1, 2>::read();
      tag_p_new = TagPipe_p_new::PipeAt<1, 2>::read();
      tag_p = TagPipe_p::PipeAt<1, 2>::read();
      loopTags[2] = loopTag;

      TagPipe_p_new::PipeAt<2, 0>::write(tag_p_new);
      TagPipe_p_new::PipeAt<2, 1>::write(tag_p_new);
      TagPipe_p::PipeAt<2, 1>::write(tag_p + numNodes);

      loopTag++;
      LoopTagPipe::PipeAt<2, 0>::write(loopTag);
      LoopTagPipe::PipeAt<2, 1>::write(loopTag);
      for (int i = 0; i < numNodes; i++) {
        // tag++;
        int LoadAddr0 = i;
        LoadAddrPipes_p_new::PipeAt<1>::write({LoadAddr0,
                                               tag_p_new,
                                               loopTag,
                                               {loopTags[1], loopTags[2]},
                                               {false, false}});
        
        tag_p++;
        StoreAddrPipes_p::PipeAt<0>::write(
            {i, tag_p, loopTag, {0u}, {loopTags[2]}});
      }
      loopTags[3] = loopTag;
    }

    LoadAddrPipes_p_new::PipeAt<1>::write({MAX_INT, 0u, MAX_INT, {MAX_INT, MAX_INT}});
    StoreAddrPipes_p::PipeAt<0>::write({MAX_INT, MAX_INT, MAX_INT});
    PRINTF("** DONE AGU 2\n");
  });


  auto memEvents = StreamingMemory<0, LoadAddrPipes_p_new, LoadValPipes_p_new,
                                   StoreAddrPipes_p_new, StoreValPipes_p_new, 
                                   2, 2>(q, p_new);

  auto memEvents2 = StreamingMemory<1, LoadAddrPipes_p, LoadValPipes_p,
                                   StoreAddrPipes_p, StoreValPipes_p, 
                                   1, 1>(q, p);


  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int iter = 0; iter < maxIters; ++iter) {
      LoopPredPipe::write(true);

      // LoopPredPipe::PipeAt<0, 0>::write(true);
      // LoopPredPipe::PipeAt<0, 1>::write(true);

      // LoopPredPipe::PipeAt<1, 0>::write(true);
      // LoopPredPipe::PipeAt<1, 1>::write(true);

      // LoopPredPipe::PipeAt<2, 0>::write(true);
      // LoopPredPipe::PipeAt<2, 1>::write(true);
    }

    LoopPredPipe::write(false);
  });

  q.single_task<class MainKernel0>([=]() [[intel::kernel_args_restrict]] {
    [[maybe_unused]] int iter = 0;
    while (LoopPredPipe::PipeAt<0, 1>::read()) {
      // PRINTF("MainKernel0 iter %d\n", iter);
      for (int i = 0; i < numNodes; i++) {
        // PRINTF("(%d, %d) st0 p_new %f\n", iter, i, 0.0f);
        StoreValPipes_p_new::PipeAt<0>::write(0.0f);
      }
      iter++;
    }

    PRINTF("** DONE MainKernel0\n");
  });

  q.single_task<class MainKernel1>([=]() [[intel::kernel_args_restrict]] {
    [[maybe_unused]] int iter = 0;
    while (LoopPredPipe::PipeAt<1, 1>::read()) {
      // PRINTF("MainKernel1 iter %d\n", iter);

      // Sparse PageRank algorithm using an adjacency matrix in CSR format.
      int curcol = 0;
      for (int i = 0; i < numNodes; i++) {
        // PRINTF("MainKernle1 i = %d\n", i);
        // PRINTF("MainKernel1 iter, i %d, %d\n", iter, i);

        int rowel = row_ptr[i + 1] - row_ptr[i];
        int local_curcol = curcol;
        for (int j = 0; j < rowel; j++) {
          
          auto LoadVal_p = LoadValPipes_p::PipeAt<0>::read();
          auto LoadVal_p_new = LoadValPipes_p_new::PipeAt<0>::read();

          auto StoreVal_p_new = LoadVal_p_new + val[local_curcol] * LoadVal_p;

          StoreValPipes_p_new::PipeAt<1>::write(StoreVal_p_new);

          // PRINTF("(%d, %d, %d) ld0 p_new %f\n"
          //        "(%d, %d, %d) ld0 p %f\n"
          //        "(%d, %d, %d) st1 p_new %f\n",
          //        iter, i, j, LoadVal_p_new, iter, i, j, LoadVal_p, iter, i, j,
          //        StoreVal_p_new);

          local_curcol++;
        }
        curcol += rowel;
      }

      iter++;
    }

    PRINTF("** DONE MainKernel1\n");
  });

  auto event3 = q.single_task<class MainKernel2>([=]() [[intel::kernel_args_restrict]] {
    [[maybe_unused]] int iter = 0;
    while (LoopPredPipe::PipeAt<2, 1>::read()) {
      // PRINTF("MainKernel2 iter %d\n", iter);
      
      // Update p[]
      for (int i = 0; i < numNodes; i++) {
        auto LoadVal_p_new = LoadValPipes_p_new::PipeAt<1>::read();
        // PRINTF("(%d, %d) ld1 p_new %f\n", iter, i, LoadVal_p_new);

        auto valP = d * LoadVal_p_new + (1.0f - d) / numNodes;
        // PRINTF("(%d, %d) st0 p %f\n", iter, i, valP);
        StoreValPipes_p::PipeAt<0>::write(valP);
      }

      iter++;
    }

    PRINTF("** DONE MainKernel2\n");

    // MemEndSignal::write(0);
    // MemEndSignal2::write(0);
  });

  event.wait();
  event3.wait();

  for (auto &e : memEvents) e.wait();
  for (auto &e2 : memEvents2) e2.wait();

  q.copy(p, h_p.data(), h_p.size()).wait();

  sycl::free((void*) row_ptr, q);
  sycl::free((void*) col_idx, q);
  sycl::free((void*) val, q);
  sycl::free(p, q);
  sycl::free(p_new, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
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

  std::vector<int> row_ptr(numNodes + 1, 0), col_idx(numEdges, 0);
  std::vector<float> val(numEdges, 0.0f), p(numNodes, 0.0f);
  
  int fromnode, tonode;
  int cur_row = 0;
  int i = 0;
  // Elements for row
  int elrow = 0;
  // Cumulative numbers of elements
  int curel = 0;
  
  while(!feof(fp)){
    fscanf(fp,"%d%d",&fromnode,&tonode);
    
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

    std::vector<float> p_cpu(numNodes, 0.0f);
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
