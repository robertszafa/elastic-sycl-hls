#include <algorithm>
#include <iostream>
#include <limits>
#include <math.h>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <sycl/sycl.hpp>
#include <vector>
#include <functional>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "exception_handler.hpp"
#include "memory_utils.hpp"

#include "pipe_utils.hpp"
#include "unrolled_loop.hpp"

#include "StreamingMemory.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;
class MainKernel2;


double simple_loop_fusion_kernel(queue &q, std::vector<float> &h_A,
                       std::vector<float> &h_B, const int timeSteps) {
  const int kN = h_A.size();

  auto *A = fpga_tools::toDevice(h_A, q);
  auto *B = fpga_tools::toDevice(h_B, q);

  using signal_B = pipe<class pipe_done_B_0_class, addr_val_sched_t<float>, 64>;
  using signal_A = pipe<class pipe_loaded_A0_class, addr_val_sched_t<float>, 64>;

  using pipe_loadB0 = pipe<class pipe_B_0_class, float, 64>;
  using pipe_storeB0 = pipe<class pipe_B_store_class, float, 64>;
  using pipe_loadA0 = pipe<class pipe_A_0_class, float, 64>;
  using pipe_storeA0 = pipe<class pipe_A_store_class, float, 64>;

  using LoadAddrB = pipe<class _LoadAddrB_class, addr_sched_t, 64>;
  using StoreAddrB = pipe<class _StoreAddrB_class, addr_sched_t, 64>;
  using LoadAddrA = pipe<class _LoadAddrA_class, addr_sched_t, 64>;
  using StoreAddrA = pipe<class _StoreAddrA_class, addr_sched_t, 64>;
  

  /*
    B
  */
  auto loadAddressB = q.single_task<class ldAddressB>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 0; i < kN; i++) {
        LoadAddrB::write({i, t});
      }
    }
    LoadAddrB::write({-1});
  });
  auto storeAddressB = q.single_task<class stAddressB>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 0; i < kN; i++) {
        StoreAddrB::write({i, t});
      }
    }
    StoreAddrB::write({-1});
  });
  auto sstB = StreamingStore<StoreAddrB, pipe_storeB0, signal_B, NoPipe>(q, B);
  auto sldB0 = StreamingLoad<LoadAddrB, pipe_loadB0, NoPipe, signal_B>(q, B);

  /*
    A
  */
  auto loadAddressA = q.single_task<class loadAddressA>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 0; i < kN; i++) {
        LoadAddrA::write({i, t});
      }
    }
    LoadAddrA::write({-1});
  });
  auto storeAddressA = q.single_task<class storeAddressA>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 0; i < kN; i++) {
        StoreAddrA::write({i, t});
      }
    }
    StoreAddrA::write({-1});
  });
  auto sldA = StreamingLoad<LoadAddrA, pipe_loadA0, signal_A, NoPipe>(q, A);
  auto sstA = StreamingStore<StoreAddrA, pipe_storeA0, NoPipe, signal_A>(q, A);

  
 /*
  Compute
 */
 auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 0; i < kN; i++) {
        pipe_storeB0::write(0.33333f * pipe_loadA0::read());
        // pipe_storeB0::write(0.33333f * A[i]);
      }
    }
  });
  auto event2 = q.single_task<MainKernel2>([=]() [[intel::kernel_args_restrict]] {
    for (int t = 0; t < timeSteps; t++) {
      for (int i = 0; i < kN; i++) {
        pipe_storeA0::write(0.33333f * pipe_loadB0::read());
        // A[i] = 0.33333f * pipe_loadB0::read();
      }
    }
  });


  event.wait();
  event2.wait();

  sldB0.wait();
  sstB.wait();

  sstA.wait();
  sldA.wait();

  q.copy(A, h_A.data(), h_A.size()).wait();
  q.copy(B, h_B.data(), h_B.size()).wait();

  sycl::free(A, q);
  sycl::free(B, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void simple_loop_fusion_cpu(std::vector<float> &A, std::vector<float> &B,
                  const int timeSteps) {
  const int kN = A.size();

  for (int t = 0; t < timeSteps; t++) {
    for (int i = 0; i < kN; i++) {
      B[i] = 0.33333f * A[i];
    }

    for (int i = 0; i < kN; i++) {
      A[i] = 0.33333f * B[i];
    }
  }
}

int main(int argc, char *argv[]) {
  int ARRAY_SIZE = 1000;
  int TIME_STEPS = 1;
  try {
    if (argc > 1) {
      ARRAY_SIZE = int(atoi(argv[1]));
    }
    if (argc > 2) {
      TIME_STEPS = int(atoi(argv[2]));
      if (TIME_STEPS < 0)
        throw std::invalid_argument("Invalid time steps.");
    }
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
#else // #if FPGA_EMULATOR
  auto d_selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif

  try {
    // Enable profiling.
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<float> A(ARRAY_SIZE, 1);
    std::vector<float> A_cpu(ARRAY_SIZE, 1);
    std::vector<float> B(ARRAY_SIZE, 2);
    std::vector<float> B_cpu(ARRAY_SIZE, 2);

    auto kernel_time = simple_loop_fusion_kernel(q, A, B, TIME_STEPS);
    simple_loop_fusion_cpu(A_cpu, B_cpu, TIME_STEPS);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    bool equalAs = std::equal(A.begin(), A.end(), A_cpu.begin());
    bool equalBs = std::equal(B.begin(), B.end(), B_cpu.begin());
    if (equalAs && equalBs)
      std::cout << "Passed\n";
    else {
      std::cout << "Failed\n";
      std::cout << "equalAs: " << equalAs << ", equalBs: " << equalBs << "\n";
      std::cout << "First 32 in A equal: "
                << std::equal(A.begin(), A.begin() + 64, A_cpu.begin()) << "\n";
    }

  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
