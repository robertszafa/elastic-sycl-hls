#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <vector>
#include <random>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "memory_utils.hpp"
#include "pipe_utils.hpp"
#include "exception_handler.hpp"

// #include "TaggedStreamingMemorySingleKernelCache.hpp"
#include "TaggedStreamingMemory.hpp"

using namespace sycl;
using namespace fpga_tools;

// Forward declare kernel name.
class MainKernel;
class MainKernel2;

double lud_cmp_kernel(queue &q, std::vector<float> &h_A, const int kN) {
  float *A = fpga_tools::toDevice(h_A, q);

  constexpr uint NUM_STORES = 2;
  constexpr uint NUM_LOADS = 7;
  constexpr uint LOOP_DEPTH = 3;

  using LoadAddrPipes = PipeArray<class _LoadAddr_p, load_req_t<NUM_STORES, LOOP_DEPTH>, 16, NUM_LOADS>;
  using LoadValPipes = PipeArray<class _LoadVal_p, float, 16, NUM_LOADS>;
  using StoreAddrPipes = PipeArray<class _StoreAddr_p, store_req_t<LOOP_DEPTH>, 16, NUM_STORES>;
  using StoreValPipes = PipeArray<class _StoreVal_p, float, 16, NUM_STORES>;

  q.single_task<class AGU_ij>([=]() [[intel::kernel_args_restrict]] {
    load_req_t<NUM_STORES, LOOP_DEPTH> ld_req_1 {INVALID_ADDR};
    InitBundle(ld_req_1.sched, 0u);
    InitBundle(ld_req_1.posDepDist, false);

    load_req_t<NUM_STORES, LOOP_DEPTH> ld_req_2 {INVALID_ADDR};
    InitBundle(ld_req_2.sched, 0u);
    InitBundle(ld_req_2.posDepDist, false);

    store_req_t<LOOP_DEPTH> st_req_1 {INVALID_ADDR};
    InitBundle(st_req_1.sched, 0u);

    for (int i = 0; i < kN; i++) {
      ld_req_1.sched[0]++;
      ld_req_2.sched[0]++;
      st_req_1.sched[0]++;

      for (int j = 0; j < i; j++) {
        ld_req_1.sched[1]++;
        ld_req_2.sched[1]++;
        st_req_1.sched[1]++;

        ld_req_1.addr = i * kN + j;
        ld_req_1.posDepDist[0] = ld_req_1.addr > st_req_1.addr;
        LoadAddrPipes::PipeAt<0>::write(ld_req_1);

        ld_req_2.addr = j * kN + j;
        ld_req_2.posDepDist[0] = ld_req_2.addr > st_req_1.addr;
        LoadAddrPipes::PipeAt<3>::write(ld_req_2);

        st_req_1.addr = i * kN + j;
        StoreAddrPipes::PipeAt<0>::write(st_req_1);
      }
    }

    ld_req_1.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_1.sched, SCHED_SENTINEL);
    LoadAddrPipes::PipeAt<0>::write(ld_req_1);
    
    ld_req_2.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_2.sched, SCHED_SENTINEL);
    LoadAddrPipes::PipeAt<3>::write(ld_req_2);

    st_req_1.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req_1.sched, SCHED_SENTINEL);
    StoreAddrPipes::PipeAt<0>::write(st_req_1);
  });

  q.single_task<class AGU_ijk>([=]() [[intel::kernel_args_restrict]] {
    load_req_t<NUM_STORES, LOOP_DEPTH> ld_req_1 {INVALID_ADDR};
    InitBundle(ld_req_1.sched, 0u);
    InitBundle(ld_req_1.posDepDist, false);

    load_req_t<NUM_STORES, LOOP_DEPTH> ld_req_2 {INVALID_ADDR};
    InitBundle(ld_req_2.sched, 0u);
    InitBundle(ld_req_2.posDepDist, false);

    for (int i = 0; i < kN; i++) {
      ld_req_1.sched[0]++;
      ld_req_2.sched[0]++;

      for (int j = 0; j < i; j++) {
        ld_req_1.sched[1]++;
        ld_req_2.sched[1]++;

        for (int k = 0; k < j; k++) {
          ld_req_1.sched[2]++;
          ld_req_2.sched[2]++;

          ld_req_1.addr = i * kN + k;
          LoadAddrPipes::PipeAt<1>::write(ld_req_1);

          ld_req_2.addr = k * kN + j;
          LoadAddrPipes::PipeAt<2>::write(ld_req_2);
        }
      }
    }

    ld_req_1.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_1.sched, SCHED_SENTINEL);
    LoadAddrPipes::PipeAt<1>::write(ld_req_1);

    ld_req_2.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_2.sched, SCHED_SENTINEL);
    LoadAddrPipes::PipeAt<2>::write(ld_req_2);
  });

  q.single_task<class AGU_il>([=]() [[intel::kernel_args_restrict]] {
    load_req_t<NUM_STORES, LOOP_DEPTH> ld_req_1 {INVALID_ADDR};
    InitBundle(ld_req_1.sched, 0u);
    InitBundle(ld_req_1.posDepDist, false);

    store_req_t<LOOP_DEPTH> st_req_1 {INVALID_ADDR};
    InitBundle(st_req_1.sched, 0u);

    for (int i = 0; i < kN; i++) {
      ld_req_1.sched[0]++;
      st_req_1.sched[0]++;

      for (int l = i; l < kN; l++) {
        ld_req_1.sched[1]++;
        st_req_1.sched[1]++;

        ld_req_1.addr = i * kN + l;
        ld_req_1.posDepDist[1] = ld_req_1.addr > st_req_1.addr;
        LoadAddrPipes::PipeAt<4>::write(ld_req_1);

        st_req_1.addr = i * kN + l;
        StoreAddrPipes::PipeAt<1>::write(st_req_1);
      }
    }

    ld_req_1.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_1.sched, SCHED_SENTINEL);
    LoadAddrPipes::PipeAt<4>::write(ld_req_1);

    st_req_1.addr = STORE_ADDR_SENTINEL;
    InitBundle(st_req_1.sched, SCHED_SENTINEL);
    StoreAddrPipes::PipeAt<1>::write(st_req_1);
  });

  q.single_task<class AGU_ilm>([=]() [[intel::kernel_args_restrict]] {
    load_req_t<NUM_STORES, LOOP_DEPTH> ld_req_1 {INVALID_ADDR};
    InitBundle(ld_req_1.sched, 0u);
    InitBundle(ld_req_1.posDepDist, false);

    load_req_t<NUM_STORES, LOOP_DEPTH> ld_req_2 {INVALID_ADDR};
    InitBundle(ld_req_2.sched, 0u);
    InitBundle(ld_req_2.posDepDist, false);

    for (int i = 0; i < kN; i++) {
      ld_req_1.sched[0]++;
      ld_req_2.sched[0]++;

      for (int l = i; l < kN; l++) {
        ld_req_1.sched[1]++;
        ld_req_2.sched[1]++;

        for (int m = 0; m < i; m++) {
          ld_req_1.sched[2]++;
          ld_req_2.sched[2]++;

          ld_req_1.addr = i * kN + m;
          LoadAddrPipes::PipeAt<5>::write(ld_req_1);

          ld_req_2.addr = m * kN + l;
          LoadAddrPipes::PipeAt<6>::write(ld_req_2);
        }
      }
    }

    ld_req_1.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_1.sched, SCHED_SENTINEL);
    LoadAddrPipes::PipeAt<5>::write(ld_req_1);

    ld_req_2.addr = LOAD_ADDR_SENTINEL;
    InitBundle(ld_req_2.sched, SCHED_SENTINEL);
    LoadAddrPipes::PipeAt<6>::write(ld_req_2);
  });


  auto memEvents = StreamingMemory<6, LoadAddrPipes, LoadValPipes,
                                   StoreAddrPipes, StoreValPipes, 
                                   NUM_LOADS, NUM_STORES, LOOP_DEPTH>(q, A);

  auto event0 = q.single_task<class MainKernel0>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < kN; i++) { // L0 
      for (int j = 0; j < i; j++) { // L1
        auto w = LoadValPipes::PipeAt<0>::read();
        // PRINTF("Done2 iter i,j = %d,%d\n", i,j);
        for (int k = 0; k < j; k++) { // L2
          w -= LoadValPipes::PipeAt<1>::read() * LoadValPipes::PipeAt<2>::read();
        }
        
        auto StVal = w / LoadValPipes::PipeAt<3>::read();
        StoreValPipes::PipeAt<0>::write(StVal);
        // StorePortValPipes::PipeAt<0>::write(StVal);
      }
    }
  });

  auto event1 = q.single_task<class MainKernel1>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < kN; i++) { // L0 
      // PRINTF("i = %d, ", i);
      for (int l = i; l < kN; l++) { // L3
        auto w = LoadValPipes::PipeAt<4>::read();
        for (int m = 0; m < i; m++) { // L4
          w -= LoadValPipes::PipeAt<5>::read() * LoadValPipes::PipeAt<6>::read();
        }
        StoreValPipes::PipeAt<1>::write(w);
        // StorePortValPipes::PipeAt<1>::write(w);
      }
    }
  });

  event0.wait();
  event1.wait();
  for (auto &e : memEvents) e.wait();
  q.copy(A, h_A.data(), h_A.size()).wait();

  sycl::free(A, q);

  auto start = event0.get_profiling_info<info::event_profiling::command_start>();
  auto end = event1.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void lud_cmp_cpu(std::vector<float> &A, const int kN) {
  for (int i = 0; i < kN; i++) {

    for (int j = 0; j < i; j++) {
      // i, j
      auto w = A[i * kN + j]; // no wrap

      for (int k = 0; k < j; k++) {
        // i, j, k
        w -= A[i * kN + k] * // wraps at k (id = j)
             A[k * kN + j]; // wraps at k (id = j)
      }

      // i, j
      A[i * kN + j] = // no wrap
          w / A[j * kN + j]; // wraps at j (id = i)
    }

    for (int l = i; l < kN; l++) {
      auto w = A[i * kN + l]; // no wrap
      for (int m = 0; m < i; m++) {
        w -= A[i * kN + m] * // wraps at m (id = l)
             A[m * kN + l]; // wraps at m (id = l)
      }
      A[i * kN + l] = w; // no wrap
    }

  }
}

inline bool almost_equal(const float x, const float y) {
  const float ulpFloat = static_cast<float>(2);
  const float tolerance = 0.01f;
  return fabsf(x - y) <=
             tolerance * fabsf(x + y) * ulpFloat ||
         fabsf(x - y) < std::numeric_limits<float>::min();
}

int main(int argc, char *argv[]) {
  int N = 10;
  int PERCENTAGE = 0;
  try {
    if (argc > 1) {
      N = int(atoi(argv[1]));
    }
    if (argc > 2) {
      PERCENTAGE = int(atoi(argv[2]));
      
      if (PERCENTAGE < 0 || PERCENTAGE > 100)
        throw std::invalid_argument("Invalid percentage.");
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
    std::cout << "Running on device: " << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<float> A(N*N, 2.0f);
    std::vector<float> A_cpu(N*N, 2.0f);
    // std::iota(A.begin(), A.end(), 1.0f);
    // std::iota(A_cpu.begin(), A_cpu.end(), 1.0f);
    for (size_t i=0; i<N*N; ++i) {
      A[i] = float(rand() % 100);
      A_cpu[i] = A[i];
    }
    

    auto kernel_time = lud_cmp_kernel(q, A, N);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    lud_cmp_cpu(A_cpu, N);

    if (std::equal(A.begin(), A.end(), A_cpu.begin(), almost_equal))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";

    
    for (size_t i=0; i<N*N; ++i) {
      if (!almost_equal(A[i],A_cpu[i]))
        std::cout << i << ": " << A[i] << " != " << A_cpu[i] << "\n";
    }

  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

