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

#include "pipe_utils.hpp"
#include "unrolled_loop.hpp"

#include "TaggedStreamingMemory_iters.hpp"


using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double test_kernel_war(queue &q, const int NI, const int NJ, const int NK, const int NUM_ITERS,
                       std::vector<int> &h_D) {

  int *D = fpga_tools::toDevice(h_D, q);

  std::vector zeroVec(h_D.size(), 0);
  int* tmp = fpga_tools::toDevice(zeroVec, q);

  using LoadAddrPipes = PipeArray<class _LoadAddrC, load_req_t<NUM_STORES, LOOP_DEPTH>, 16, NUM_LOADS>;
  using LoadValPipes = PipeArray<class _LoadValC, int, 16, NUM_LOADS>;

  using StoreAddrPipes = PipeArray<class _StoreAddrC, store_req_t<LOOP_DEPTH>, 16, NUM_STORES>;
  using StoreValPipes = PipeArray<class _StoreValC, int, 16, NUM_STORES>;

  q.single_task<class AGU0>([=]() [[intel::kernel_args_restrict]] {
    load_req_t<NUM_STORES, LOOP_DEPTH> ld_req_1 {INVALID_ADDR};
    InitBundle(ld_req_1.sched, 0u);
    InitBundle(ld_req_1.isMaxIter, false);
    InitBundle(ld_req_1.posDepDist, false);

    for (int iters = 0; iters < NUM_ITERS; iters++) {
      ld_req_1.sched[0]++;
      ld_req_1.isMaxIter[0] = ((iters+1) == NUM_ITERS);

      for (int j = 0; j < NI; j++) {
        ld_req_1.sched[1]++;
        ld_req_1.isMaxIter[1] = ((j+1) == NI);

        for (int i = 0; i < NI; i++) {
          ld_req_1.sched[2]++;
          ld_req_1.isMaxIter[2] = ((i+1) == NI);

          ld_req_1.addr = i;
          LoadAddrPipes::PipeAt<0>::write(ld_req_1);
        }
      }
    }

    ld_req_1.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_1.sched, SCHED_SENTINEL);
    InitBundle(ld_req_1.isMaxIter, true);
    LoadAddrPipes::PipeAt<0>::write(ld_req_1);

    PRINTF("** DONE AGU0\n";)
  });

  q.single_task<class AGU1>([=]() [[intel::kernel_args_restrict]] {
    store_req_t<LOOP_DEPTH> st_req_1 {INVALID_ADDR};
    InitBundle(st_req_1.sched, 0u);
    InitBundle(st_req_1.isMaxIter, false);

    for (int iters = 0; iters < NUM_ITERS; iters++) {
      st_req_1.sched[0]++;
      st_req_1.isMaxIter[0] = ((iters+1) == NUM_ITERS);

      for (int i = 10; i < NI; i++) {
        st_req_1.sched[1]++;
        st_req_1.isMaxIter[1] = ((iters+1) == NUM_ITERS);

        st_req_1.addr = i;
        StoreAddrPipes::PipeAt<0>::write(st_req_1);
      }
    }

    st_req_1.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req_1.sched, SCHED_SENTINEL);
    InitBundle(st_req_1.isMaxIter, true);
    StoreAddrPipes::PipeAt<0>::write(st_req_1);
  });

  auto memEvents = StreamingMemory<0, LoadAddrPipes, LoadValPipes,
                                   StoreAddrPipes, StoreValPipes, 
                                   NUM_LOADS, NUM_STORES, LOOP_DEPTH>(q, tmp);

  auto event = q.single_task<class MainKernel0>([=]() [[intel::kernel_args_restrict]] {
    for (int iters = 0; iters < NUM_ITERS; iters++) {

      for (int j = 0; j < NI; j++) {
        for (int i = 0; i < NI; i++) {
          D[i] = LoadValPipes::PipeAt<0>::read() + j;
        }
      }
    }
  });

  auto event2 = q.single_task<class MainKernel1>([=]() [[intel::kernel_args_restrict]] {
    for (int iters = 0; iters < NUM_ITERS; iters++) {
      for (int i = 10; i < NI; i++) {
        StoreValPipes::PipeAt<0>::write(iters);
      }
    }

  });

  event.wait();
  for (auto &e : memEvents) e.wait();
  
  q.copy(D, h_D.data(), h_D.size()).wait();

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void test_kernel_cpu(const int NI, const int NJ, const int NK,
                     const int NUM_ITERS, std::vector<int> &D) {
  std::vector tmp(D.size(), 0);

  for (int iters = 0; iters < NUM_ITERS; iters++) {

    for (int j = 0; j < NI; j++) {
      for (int i = 0; i < NI; i++) {
        D[i] = tmp[i] + j;
      }
    }

    for (int i = 10; i < NI; i++) {
      tmp[i] = iters;
    }
  }
}

int main(int argc, char *argv[]) {
  int N = 10;
  int NUM_ITERS = 1;
  try {
    if (argc > 1)
      N = int(atoi(argv[1]));
    if (argc > 2)
      NUM_ITERS = int(atoi(argv[2]));
  } catch (exception const &e) {
    std::cout << "Incorrect argv.\nUsage:\n"
              << "  ./executable [ARRAY_SIZE] [NUM_ITERS]\n";
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
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    const int NI = N;
    const int NJ = N;
    const int NK = N;
    const int S = N*N;

    std::vector<int> D(S, 4); 
    std::vector<int> D_cpu(S, 4); 

    auto kernel_time = test_kernel_war(q, NI, NJ, NK, NUM_ITERS, D);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    test_kernel_cpu(NI, NJ, NK, NUM_ITERS, D_cpu);

    if (std::equal(D.begin(), D.end(), D_cpu.begin()))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";

  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
