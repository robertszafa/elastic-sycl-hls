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

#include "StreamingMemory.hpp"


using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double test_kernel_war(queue &q, const int NI, const int NJ, const int NK,
                       std::vector<int> &h_D) {

  int *D = fpga_tools::toDevice(h_D, q);

  constexpr uint NUM_STORES = 2;
  constexpr uint NUM_LOADS = 1;
  constexpr uint NUM_LOOPS = 3;

  using LoadAddrPipes = PipeArray<class _LoadAddrC, ld_req_t<2>, 16, 1>;
  using LoadValPipes = PipeArray<class _LoadValC, int, 16, 1>;

  using StoreAddrPipes =
      PipeArray<class _StoreAddrC, st_req_t<NUM_STORES, NUM_LOADS>, 16, 2>;
  using StoreValPipes = PipeArray<class _StoreValC, int, 16, 2>;

  using MemEndSignal = pipe<class _MemEndSignal, bool, 1>;

  using TagBeforeLoop = PipeArray<class _TagBeforeLoop, uint, 16, 2, 2>;
  using TagAfterLoop = PipeArray<class _TagAfterLoop, uint, 16, 2, 2>;
  q.single_task<class AGU0>([=]() [[intel::kernel_args_restrict]] {
    uint tag = 0u;

    tag_t tagBeforeLoop[NUM_LOOPS];
    InitBundle(tagBeforeLoop, 0u);

    tagBeforeLoop[0] = tag;
    for (int iters = 0; iters < NI; iters++) {
      TagBeforeLoop::PipeAt<0, 1>::write(tag);
      TagAfterLoop::PipeAt<0, 1>::write(tag + NI);

      tagBeforeLoop[1] = tag;
      for (int i = 0; i < NI; i++) {
        tag++;
        StoreAddrPipes::PipeAt<0>::write(
            {i, tag, {tagBeforeLoop[1], tagBeforeLoop[2]}, {tagBeforeLoop[2]}});
      }

      tagBeforeLoop[2] = TagBeforeLoop::PipeAt<1, 0>::read();
      tag = TagAfterLoop::PipeAt<1, 0>::read();
    }
  
    StoreAddrPipes::PipeAt<0>::write({MAX_INT, MAX_INT});
  });

  q.single_task<class AGU1>([=]() [[intel::kernel_args_restrict]] {
    addr_t storeAddr[NUM_STORES];
    InitBundle(storeAddr, 0u);

    uint tag = 0u;

    tag_t tagBeforeLoop[NUM_LOOPS];
    InitBundle(tagBeforeLoop, 0u);

    tagBeforeLoop[0] = tag;
    for (int iters = 0; iters < NI; iters++) {
      tagBeforeLoop[1] = TagBeforeLoop::PipeAt<0, 1>::read();
      tag = TagAfterLoop::PipeAt<0, 1>::read();

      tagBeforeLoop[2] = tag;
      TagBeforeLoop::PipeAt<1, 0>::write(tag);
      TagAfterLoop::PipeAt<1, 0>::write(tag + NI);
      for (int i = 0; i < NI; i++) {
        LoadAddrPipes::PipeAt<0>::write({i,
                                         tag,
                                         {tagBeforeLoop[0], tagBeforeLoop[2]},
                                         {tagBeforeLoop[1], tagBeforeLoop[2]},
                                         {false, i > storeAddr[1]}});

        tag++;
        storeAddr[1] = i;
        StoreAddrPipes::PipeAt<1>::write(
            {i, tag, {tagBeforeLoop[1], tagBeforeLoop[2]}, {tagBeforeLoop[2]}});
      }
    }

    StoreAddrPipes::PipeAt<1>::write({MAX_INT, MAX_INT});
  });

  auto memEvents = StreamingMemory<0, LoadAddrPipes, LoadValPipes,
                                   StoreAddrPipes, StoreValPipes, 
                                   MemEndSignal, 1, 2>(q, D);

  auto event = q.single_task<class MainKernel0>([=]() [[intel::kernel_args_restrict]] {
    for (int iters = 0; iters < NI; iters++) {

      for (int i = 0; i < NI; i++) {
        // D[i] = iters;
        StoreValPipes::PipeAt<0>::write(iters);
      }
    }
  });

  auto event2 = q.single_task<class MainKernel1>([=]() [[intel::kernel_args_restrict]] {
    for (int iters = 0; iters < NI; iters++) {
      for (int i = 0; i < NI; i++) {
        // D[i] += 1;
        StoreValPipes::PipeAt<1>::write(LoadValPipes::PipeAt<0>::read() + 1);
      }
    }

    MemEndSignal::write(0);
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
                     std::vector<int> &D) {
    for (int iters = 0; iters < NI; iters++) {

      for (int i = 0; i < NI; i++) {
        D[i] = iters;
      }

      for (int i = 0; i < NI; i++) {
        D[i] += 1;
      }

    }
}

int main(int argc, char *argv[]) {
  int N = 10;
  try {
    if (argc > 1)
      N = int(atoi(argv[1]));
  } catch (exception const &e) {
    std::cout << "Incorrect argv.\nUsage:\n"
              << "  ./executable [ARRAY_SIZE] [PERCENTAGE (% of iterations "
                 "with dependencies.)]\n";
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

    auto kernel_time = test_kernel_war(q, NI, NJ, NK, D);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    test_kernel_cpu(NI, NJ, NK, D_cpu);

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
