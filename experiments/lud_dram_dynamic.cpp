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

#include "TaggedStreamingMemorySingleKernelCache.hpp"

using namespace sycl;
using namespace fpga_tools;

// Forward declare kernel name.
class MainKernel;
class MainKernel2;

double lud_cmp_kernel(queue &q, std::vector<float> &h_A, const int kN) {
  float *A = fpga_tools::toDevice(h_A, q);

  using LoadAddrPipes = PipeArray<class _LoadAddrC, addr_tag_mintag_t<2>, 16, 7>;
  using LoadValPipes = PipeArray<class _LoadValC, float, 16, 7>;
  using StoreAddrPipes = PipeArray<class _StoreAddrC, addr_tag_t, 16, 2>;
  using StoreValPipes = PipeArray<class _StoreValC, float, 16, 2>;

  using MemEndSignal = pipe<class _MemEndSignal, bool, 1>;

  q.single_task<class AGU0>([=]() [[intel::kernel_args_restrict]] {
    uint tag = 0;
    uint tagBeforeLoop1 = 0, lastTagBeforeLoop1 = 0;
    
    uint lastTagBeforeLoop3 = 0;

    int LoadAddr[7];
    InitBundle(LoadAddr, INVALID_ADDR);
    int StoreAddr[2];
    InitBundle(StoreAddr, INVALID_ADDR);

    for (int i = 0; i < kN; i++) {
      lastTagBeforeLoop1 = tagBeforeLoop1;
      tagBeforeLoop1 = tag;

      uint localTag = tag;
      for (int j = 0; j < i; j++) {
        LoadAddr[0] = i * kN + j;
        LoadAddrPipes::PipeAt<0>::write(
            {LoadAddr[0],
             localTag,
             tagBeforeLoop1,
             {lastTagBeforeLoop1, lastTagBeforeLoop3},
            //  {false, false}});
             {LoadAddr[0] > StoreAddr[0], false}});

        for (int k = 0; k < j; k++) {
          LoadAddr[1] = i * kN + k;
          LoadAddrPipes::PipeAt<1>::write(
              {LoadAddr[1],
              localTag,
              tagBeforeLoop1,
              {lastTagBeforeLoop1, lastTagBeforeLoop3},
              // {false, false}});
              {LoadAddr[1] > StoreAddr[0], false}});
          
          LoadAddr[2] = k * kN + j;
          LoadAddrPipes::PipeAt<2>::write(
              {LoadAddr[2],
              localTag,
              tagBeforeLoop1,
              {lastTagBeforeLoop1, lastTagBeforeLoop3},
              // {false, false}});
              {LoadAddr[2] > StoreAddr[0], false}});
        }

        LoadAddr[3] = j * kN + j;
        LoadAddrPipes::PipeAt<3>::write(
            {LoadAddr[3],
             localTag,
             tagBeforeLoop1,
             {lastTagBeforeLoop1, lastTagBeforeLoop3},
            //  {false, false}});
             {LoadAddr[3] > StoreAddr[0], false}});

        localTag++;
        StoreAddr[0] = i * kN + j;
        StoreAddrPipes::PipeAt<0>::write({StoreAddr[0], localTag});
      }

      tag = localTag;
      lastTagBeforeLoop3 = tag;

      tag += (kN - i);
    }

    StoreAddrPipes::PipeAt<0>::write({MAX_INT, MAX_INT});
  });

  
  q.single_task<class AGU1>([=]() [[intel::kernel_args_restrict]] {
    uint tag = 0;
    uint tagBeforeLoop1 = 0, lastTagBeforeLoop1 = 0;
    uint tagBeforeLoop3 = 0, lastTagBeforeLoop3 = 0;

    int LoadAddr[7];
    InitBundle(LoadAddr, INVALID_ADDR);
    int StoreAddr[2];
    InitBundle(StoreAddr, INVALID_ADDR);

    for (int i = 0; i < kN; i++) { // L0 
      tag += i;

      lastTagBeforeLoop3 = tagBeforeLoop3;
      tagBeforeLoop3 = tag;

      uint localTag = tag;
      for (int l = i; l < kN; l++) { // L3
        LoadAddr[4] = i * kN + l;
        LoadAddrPipes::PipeAt<4>::write(
            {LoadAddr[4],
             localTag,
             tagBeforeLoop3,
             {lastTagBeforeLoop1, lastTagBeforeLoop3},
            //  {false, false}});
             {false, LoadAddr[4] > StoreAddr[1]}});

        for (int m = 0; m < i; m++) { // L4
          LoadAddr[5] = i * kN + m;
          LoadAddrPipes::PipeAt<5>::write(
              {LoadAddr[5],
              localTag,
              tagBeforeLoop3,
              {lastTagBeforeLoop1, lastTagBeforeLoop3},
              // {false, false}});
              {false, LoadAddr[5] > StoreAddr[1]}});

          LoadAddr[6] = m * kN + l;
          LoadAddrPipes::PipeAt<6>::write(
              {LoadAddr[6],
              localTag,
              tagBeforeLoop3,
              {lastTagBeforeLoop1, lastTagBeforeLoop3},
              // {false, false}});
              {false, LoadAddr[6] > StoreAddr[1]}});
        }
        
        localTag++;
        StoreAddr[1] = i * kN + l;
        StoreAddrPipes::PipeAt<1>::write({StoreAddr[1], localTag});
      }

      tag = localTag;
      lastTagBeforeLoop1 = tag;
    }

    StoreAddrPipes::PipeAt<1>::write({MAX_INT, MAX_INT});
  });

  auto memEvents = StreamingMemory<0, MemEndSignal, LoadAddrPipes, LoadValPipes,
                                   StoreAddrPipes, StoreValPipes, 7, 2>(q, A);

  auto event0 = q.single_task<class MainKernel0>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < kN; i++) { // L0 
      for (int j = 0; j < i; j++) { // L1
        auto w = LoadValPipes::PipeAt<0>::read();
        for (int k = 0; k < j; k++) { // L2
          w -= LoadValPipes::PipeAt<1>::read() * LoadValPipes::PipeAt<2>::read();
        }
        StoreValPipes::PipeAt<0>::write(w / LoadValPipes::PipeAt<3>::read());
      }
    }
  });

  auto event1 = q.single_task<class MainKernel1>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < kN; i++) { // L0 
      for (int l = i; l < kN; l++) { // L3
        auto w = LoadValPipes::PipeAt<4>::read();
        for (int m = 0; m < i; m++) { // L4
          w -= LoadValPipes::PipeAt<5>::read() * LoadValPipes::PipeAt<6>::read();
        }
        StoreValPipes::PipeAt<1>::write(w);
      }
    }

    MemEndSignal::write(true);
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
      auto w = A[i * kN + j];
      for (int k = 0; k < j; k++) {
        w -= A[i * kN + k] * A[k * kN + j];
      }
      A[i * kN + j] = w / A[j * kN + j];
    }

    for (int l = i; l < kN; l++) {
      auto w = A[i * kN + l];
      for (int m = 0; m < i; m++) {
        w -= A[i * kN + m] * A[m * kN + l];
      }
      A[i * kN + l] = w;
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

