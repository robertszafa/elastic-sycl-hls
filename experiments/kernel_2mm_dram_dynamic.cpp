#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <vector>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "memory_utils.hpp"
#include "pipe_utils.hpp"
#include "exception_handler.hpp"

#include "TaggedStreamingMemory.hpp"

using namespace sycl;
using namespace fpga_tools;

// Forward declare kernel name.
class MainKernel0;
class MainKernel1;

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double kernel_2mm(queue &q, const int alpha, const int beta, const int NI,
                  const int NJ, const int NK, std::vector<int> &h_A,
                  std::vector<int> &h_B, std::vector<int> &h_C,
                  std::vector<int> &h_D) {

  int *A = fpga_tools::toDevice(h_A, q);
  int *B = fpga_tools::toDevice(h_B, q);
  int *C = fpga_tools::toDevice(h_C, q);
  int *D = fpga_tools::toDevice(h_D, q);

  std::vector<int> h_tmp(h_D.size(), 0);
  int* tmp = fpga_tools::toDevice(h_tmp, q);

  constexpr int NUM_LOADS = 2;
  constexpr int NUM_STORES = 1;
  constexpr int LOOP_DEPTH = 3;
  using LoadAddrPipes = PipeArray<class _LoadAddr_p, load_req_t<NUM_STORES, LOOP_DEPTH>, 16, NUM_LOADS>;
  using LoadValPipes = PipeArray<class _LoadVal_p, int, 16, NUM_LOADS>;
  using StoreAddrPipes = PipeArray<class _StoreAddr_p, store_req_t<LOOP_DEPTH>, 16, NUM_STORES>;
  using StoreValPipes = PipeArray<class _StoreVal_p, int, 16, NUM_STORES>;

  using LoadValA = sycl::pipe<class _LoadValA, int, 16>;
  auto sldA = q.single_task<class sldA>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) {
      for (int j = 0; j < NJ; j++) {
        for (int k = 0; k < NJ; ++k) {
          LoadValA::write(A[i * NI + k]);
        }
      }
    }
  });
  using LoadValB = sycl::pipe<class _LoadValB, int, 16>;
  auto sldB = q.single_task<class sldB>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) {
      for (int j = 0; j < NJ; j++) {
        for (int k = 0; k < NJ; ++k) {
          LoadValB::write(B[k * NK + j]);
        }
      }
    }
  });
  using LoadValC = sycl::pipe<class _LoadValC, int, 16>;
  auto sldC = q.single_task<class sldC>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) {
      for (int j = 0; j < NJ; j++) {
        for (int k = 0; k < NJ; ++k) {
          LoadValC::write(C[k * NK + j]);
        }
      }
    }
  });
  using LoadValD = sycl::pipe<class _LoadValD, int, 16>;
  auto sldD = q.single_task<class sldD>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) {
      for (int j = 0; j < NJ; j++) {
        // BurstCoalescedLSU
        LoadValD::write(D[i * NK + j]);
      }
    }
  });
  using StoreValD = sycl::pipe<class _StoreValD, int, 16>;
  auto sstD = q.single_task<class sstD>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) {
      for (int j = 0; j < NJ; j++) {
        // BurstCoalescedLSU
        D[i * NK + j] = StoreValD::read();
      }
    }
  });


  auto agu0 = q.single_task<class agu0>([=]() [[intel::kernel_args_restrict]] {
    load_req_t<NUM_STORES, LOOP_DEPTH> ld_req_1 {INVALID_ADDR};
    InitBundle(ld_req_1.sched, 0u);
    InitBundle(ld_req_1.posDepDist, false);

    store_req_t<LOOP_DEPTH> st_req_1 {INVALID_ADDR};
    InitBundle(st_req_1.sched, 0u);

    // [[intel::loop_coalesce(2)]]
    for (int i = 0; i < NI; i++) {
      ld_req_1.sched[0]++;
      st_req_1.sched[0]++;

      for (int j = 0; j < NJ; j++) {
        ld_req_1.sched[1]++;
        ld_req_1.addr++;
        ld_req_1.posDepDist[0] = ld_req_1.addr > st_req_1.addr;
        LoadAddrPipes::PipeAt<0>::write(ld_req_1);

        st_req_1.sched[1]++;
        st_req_1.addr = i * NI + j;
        StoreAddrPipes::PipeAt<0>::write(st_req_1);
      }
    }

    ld_req_1.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_1.sched, SCHED_SENTINEL);
    LoadAddrPipes::PipeAt<0>::write(ld_req_1);

    st_req_1.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req_1.sched, SCHED_SENTINEL);
    StoreAddrPipes::PipeAt<0>::write(st_req_1);
  });

  auto agu1 = q.single_task<class agu1>([=]() [[intel::kernel_args_restrict]] {
    load_req_t<NUM_STORES, LOOP_DEPTH> ld_req_1 {INVALID_ADDR};
    InitBundle(ld_req_1.sched, 0u);
    // InitBundle(ld_req_1.isMaxIter, false);
    InitBundle(ld_req_1.posDepDist, false);

    // [[intel::loop_coalesce(3)]]
    for (int i = 0; i < NI; i++) {
      ld_req_1.sched[0]++;
      // ld_req_1.isMaxIter[0] = ((i+1) == NI);

      for (int j = 0; j < NJ; j++) {
        ld_req_1.sched[1]++;
        // ld_req_1.isMaxIter[1] = ((j+1) == NJ);

        for (int k = 0; k < NJ; ++k) {
          ld_req_1.sched[2]++;
          // ld_req_1.isMaxIter[2] = ((k+1) == NJ);

          ld_req_1.addr = i * NI + k;
          LoadAddrPipes::PipeAt<1>::write(ld_req_1);
        }
      }
    }
    
    ld_req_1.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_1.sched, SCHED_SENTINEL);
    LoadAddrPipes::PipeAt<1>::write(ld_req_1);
  });

  auto memEvents =
      StreamingMemory<3, LoadAddrPipes, LoadValPipes, StoreAddrPipes,
                      StoreValPipes, NUM_LOADS, NUM_STORES, LOOP_DEPTH>(q, tmp);

  auto event0 = q.single_task<MainKernel0>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) {
      for (int j = 0; j < NJ; j++) {
        // int x = tmp[i * NI + j];
        int x = LoadValPipes::PipeAt<0>::read();
        for (int k = 0; k < NK; ++k) {
          x += alpha * LoadValA::read() * LoadValB::read();
        }
        // tmp[i * NI + j] = x;
        StoreValPipes::PipeAt<0>::write(x);
      }
    }
  });

  auto event1 = q.single_task<MainKernel1>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) {
      for (int j = 0; j < NJ; j++) {
        // int x = D[i * NI + j] * beta;
        int x = LoadValD::read() * beta;
        for (int k = 0; k < NJ; ++k) {
          // x += tmp[i * NI + k] * C[k * NK + j];
          x += LoadValPipes::PipeAt<1>::read() * LoadValC::read();
        }
        // D[i * NI + j] = x;
        StoreValD::write(x);
      }
    }
  });

  event0.wait();
  event1.wait();
  sstD.wait();

  for (auto &e : memEvents) e.wait();
  
  q.copy(D, h_D.data(), h_D.size()).wait();

  auto start = event0.get_profiling_info<info::event_profiling::command_start>();
  auto end = event1.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void kernel_2mm_cpu(const int alpha, const int beta, const int NI, const int NJ,
                    const int NK, const std::vector<int> &A,
                    const std::vector<int> &B, const std::vector<int> &C,
                    std::vector<int> &D) {
  std::vector<int> tmp(NI * NJ, 0);

  for (int i = 0; i < NI; i++) {
    for (int j = 0; j < NJ; j++) {
      int x = tmp[i * NI + j];
      for (int k = 0; k < NK; ++k) {
        x += alpha * A[i * NI + k] * B[k * NK + j];
      }
      tmp[i * NI + j] = x;
    }
  }

  for (int i = 0; i < NI; i++) {
    for (int j = 0; j < NJ; j++) {
      int x = D[i * NI + j] * beta;
      for (int k = 0; k < NJ; ++k) {
        x += tmp[i * NI + k] * C[k * NK + j];
      }
      D[i * NI + j] = x;
    }
  }
}

int main(int argc, char *argv[]) {
  int N = 10;
  try {
    if (argc > 1) {
      N = int(atoi(argv[1]));
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

    std::vector<int> A(S, 1);
    std::vector<int> B(S, 2); 
    std::vector<int> C(S, 3); 
    std::vector<int> D(S, 0); 
    std::vector<int> D_cpu(S, 0); 

    auto kernel_time = kernel_2mm(q, 2, 2, NI, NJ, NK, A, B, C, D);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    kernel_2mm_cpu(2, 2, NI, NJ, NK, A, B, C, D_cpu);

    if (std::equal(D.begin(), D.end(), D_cpu.begin()))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";

    for (int i = 0; i < S; ++i) {
      if (D[i] != D_cpu[i]) {
        std::cout << i << ": " << D[i] << " != " << D_cpu[i] << "\n";
      }
    }
    
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
