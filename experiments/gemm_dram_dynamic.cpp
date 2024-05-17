#include <algorithm>
#include <iostream>
#include <limits>
#include <math.h>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <sycl/sycl.hpp>
#include <vector>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "exception_handler.hpp"
#include "memory_utils.hpp"
#include "device_print.hpp"

#include "TaggedStreamingMemory.hpp"

using namespace sycl;
using namespace fpga_tools;

// Forward declare kernel name.
class MainKernel;

double gemm_kernel(queue &q, std::vector<float> &h_A, std::vector<float> &h_B,
                   std::vector<float> &h_C, const float alpha, const float beta,
                   const int NI, const int NJ, const int NK) {
  auto *A = fpga_tools::toDevice(h_A, q);
  auto *B = fpga_tools::toDevice(h_B, q);
  auto *C = fpga_tools::toDevice(h_C, q);


  constexpr int NUM_LOADS = 2;
  constexpr int NUM_STORES = 2;
  constexpr int LOOP_DEPTH = 3;
  using LoadAddrPipes = PipeArray<class _LoadAddr_p, load_req_t<NUM_STORES, LOOP_DEPTH>, 16, NUM_LOADS>;
  using LoadValPipes = PipeArray<class _LoadVal_p, float, 16, NUM_LOADS>;
  using StoreAddrPipes = PipeArray<class _StoreAddr_p, store_req_t<LOOP_DEPTH>, 16, NUM_STORES>;
  using StoreValPipes = PipeArray<class _StoreVal_p, float, 16, NUM_STORES>;

  // using TagPipe = PipeArray<class _TagPipe, tag_t, 16, NUM_AGUS, NUM_AGUS>;
  // using LoopTagPipe = PipeArray<class _TagAfterLoop_p_new, uint, 16, NUM_AGUS, NUM_AGUS>;
  
  using LoadValA0 = sycl::pipe<class LoadValA0Pipe, float, 16>;
  using LoadValB0 = sycl::pipe<class LoadValB0Pipe, float, 16>;
  q.single_task<class sldAB>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) { 
      for (int k = 0; k < NK; k++) {
        for (int j = 0; j < NJ; j++) {
          LoadValA0::write(A[i * NI + k]);
          LoadValB0::write(B[k * NK + j]);
        }
      }
    }
  });

  q.single_task<class AGU0>([=]() [[intel::kernel_args_restrict]] {
    load_req_t<NUM_STORES, LOOP_DEPTH> ld_req_0 {INVALID_ADDR};
    InitBundle(ld_req_0.sched, 0u);
    InitBundle(ld_req_0.posDepDist, false);
    InitBundle(ld_req_0.isMaxIter, false);
    store_req_t<LOOP_DEPTH> st_req_0 {INVALID_ADDR};
    InitBundle(st_req_0.sched, 0u);
    InitBundle(st_req_0.isMaxIter, false);

    for (int i = 0; i < NI; i++) { 
      st_req_0.sched[0]++;
      ld_req_0.sched[0]++;
      st_req_0.isMaxIter[0] = (i+1) == NI;
      ld_req_0.isMaxIter[0] = (i+1) == NI;

      for (int j = 0; j < NJ; j++) {
        st_req_0.sched[1]++;
        ld_req_0.sched[1]++;
        st_req_0.isMaxIter[1] = (j+1) == NJ;
        ld_req_0.isMaxIter[1] = (j+1) == NJ;

        ld_req_0.addr = i * NI + j;
        ld_req_0.posDepDist[0] = ld_req_0.addr > st_req_0.addr;
        LoadAddrPipes::PipeAt<0>::write(ld_req_0);

        st_req_0.addr = i * NI + j;
        StoreAddrPipes::PipeAt<0>::write(st_req_0);
      }
    }

    ld_req_0.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_0.sched, SCHED_SENTINEL);
    st_req_0.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req_0.sched, SCHED_SENTINEL);
    LoadAddrPipes::PipeAt<0>::write(ld_req_0);
    StoreAddrPipes::PipeAt<0>::write(st_req_0);
    // PRINTF("Done AGU0\n");
  });
  
  q.single_task<class AGU1>([=]() [[intel::kernel_args_restrict]] {
    load_req_t<NUM_STORES, LOOP_DEPTH> ld_req_1 {INVALID_ADDR};
    InitBundle(ld_req_1.sched, 0u);
    InitBundle(ld_req_1.posDepDist, false);
    InitBundle(ld_req_1.isMaxIter, false);
    store_req_t<LOOP_DEPTH> st_req_1 {INVALID_ADDR};
    InitBundle(st_req_1.sched, 0u);
    InitBundle(st_req_1.isMaxIter, false);

    for (int i = 0; i < NI; i++) { 
      st_req_1.sched[0]++;
      ld_req_1.sched[0]++;
      st_req_1.isMaxIter[0] = (i+1) == NI;
      ld_req_1.isMaxIter[0] = (i+1) == NI;

      for (int k = 0; k < NK; k++) {
        st_req_1.sched[1]++;
        ld_req_1.sched[1]++;
        st_req_1.isMaxIter[1] = (k+1) == NK;
        ld_req_1.isMaxIter[1] = (k+1) == NK;

        for (int j = 0; j < NJ; j++) {
          st_req_1.sched[2]++;
          ld_req_1.sched[2]++;
          st_req_1.isMaxIter[2] = (j+1) == NJ;
          ld_req_1.isMaxIter[2] = (j+1) == NJ;

          ld_req_1.addr = i * NI + j;
          ld_req_1.posDepDist[1] = ld_req_1.addr > st_req_1.addr;
          LoadAddrPipes::PipeAt<1>::write(ld_req_1);

          st_req_1.addr = i * NI + j;
          StoreAddrPipes::PipeAt<1>::write(st_req_1);
        }
      }
    }

    ld_req_1.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_1.sched, SCHED_SENTINEL);
    st_req_1.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req_1.sched, SCHED_SENTINEL);
    LoadAddrPipes::PipeAt<1>::write(ld_req_1);
    StoreAddrPipes::PipeAt<1>::write(st_req_1);
    // PRINTF("Done AGU1\n");
  });

  auto memEvents =
      StreamingMemory<0, LoadAddrPipes, LoadValPipes, StoreAddrPipes,
                      StoreValPipes, NUM_LOADS, NUM_STORES, LOOP_DEPTH>(q, C);

  auto eventL0 = q.single_task<class Loop0>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) { 
      for (int j = 0; j < NJ; j++) {
        StoreValPipes::PipeAt<0>::write(LoadValPipes::PipeAt<0>::read() * beta);
        // PRINTF("Done loop0 %d, %d\n", i, j);
      }
    }
  });

  auto eventL1 = q.single_task<class Loop1>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) { 
      for (int k = 0; k < NK; k++) {
        for (int j = 0; j < NJ; j++) {
          StoreValPipes::PipeAt<1>::write(
              LoadValPipes::PipeAt<1>::read() +
              (beta * LoadValA0::read() * LoadValB0::read()));

          // PRINTF("Done loop1 %d, %d, %d\n", i, j, k);
        }
      }
    }
  });

  eventL0.wait();
  eventL1.wait();
  for (auto &e : memEvents) e.wait();

  q.copy(C, h_C.data(), h_C.size()).wait();

  sycl::free(A, q);
  sycl::free(B, q);
  sycl::free(C, q);

  auto start = eventL0.get_profiling_info<info::event_profiling::command_start>();
  auto end = eventL1.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void gemm_cpu(std::vector<float> &A, std::vector<float> &B,
              std::vector<float> &C, const float alpha, const float beta,
              const int NI, const int NJ, const int NK) {
  // BLAS PARAMS
  // TRANSA = 'N'
  // TRANSB = 'N'
  //  => Form C := alpha*A*B + beta*C,
  // A is NIxNK
  // B is NKxNJ
  // C is NIxNJ
  // WrapId0 = 0;
  // WrapId1 = 0;
  for (int i = 0; i < NI; i++) {
    for (int j = 0; j < NJ; j++) {
      C[i * NI + j] *= beta;
    }

    for (int k = 0; k < NK; k++) {
      // WrapId1++;
      for (int j = 0; j < NJ; j++) {
        C[i * NI + j] += alpha * A[i * NI + k] * B[k * NK + j];
      }
    }
  }
}

inline bool almost_equal(const float x, const float y) {
  float ulpFloat = static_cast<float>(2);
  return fabsf(x - y) <=
             std::numeric_limits<float>::epsilon() * fabsf(x + y) * ulpFloat ||
         fabsf(x - y) < std::numeric_limits<float>::min();
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

    std::vector<float> A(N*N, 1);
    std::vector<float> A_cpu(N*N, 1);
    std::vector<float> B(N*N, 1);
    std::vector<float> B_cpu(N*N, 1);
    std::vector<float> C(N*N, 1);
    std::vector<float> C_cpu(N*N, 1);

    auto kernel_time = gemm_kernel(q, A, B, C, 1.0f, 1.0f, N, N, N);
    gemm_cpu(A_cpu, B_cpu, C_cpu, 1.0f, 1.0f, N, N, N);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(C.begin(), C.end(), C_cpu.begin(), almost_equal))
      std::cout << "Passed\n";
    else {
      std::cout << "Failed\n";

      for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
          if (C[i * N + j] != C_cpu[i * N + j]) {
            std::cout << "Mismatch in (" << i << ", " << j << ")\n";
          }
        }
      }
    }
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
