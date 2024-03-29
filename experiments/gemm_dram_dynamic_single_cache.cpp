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

// #include "StreamingMemory.hpp"
#include "TaggedStreamingMemorySingleKernelCache.hpp"
// #include "SingleStoreMonoStreamingMemory.hpp"
// #include "MonoStreamingMemory.hpp"
// #include "ComplicatedTaggedStreamingMemory.hpp"
// #include "BurstingStreamingMemory.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double gemm_kernel(queue &q, std::vector<float> &h_A, std::vector<float> &h_B,
                   std::vector<float> &h_C, const float alpha, const float beta,
                   const int NI, const int NJ, const int NK) {
  auto *A = fpga_tools::toDevice(h_A, q);
  auto *B = fpga_tools::toDevice(h_B, q);
  auto *C = fpga_tools::toDevice(h_C, q);

  using l0 = pipe<class _l0, bool, 16>;
  using l1 = pipe<class _l1, bool, 16>;
  // using i0 = pipe<class _i0, int, 16>;
  // using i1 = pipe<class _i1, int, 16>;

  using LoadAddrC = PipeArray<class _LoadAddrC, addr_tag_mintag_t<2>, 16, 2>;
  using LoadValC = PipeArray<class _LoadValC, float, 16, 2>;
  using StoreAddrC = PipeArray<class _StoreAddrC, addr_tag_t, 16, 2>;
  using StorePortAddrC = PipeArray<class _StorePortAddrC, addr_tag_t, 16, 2>;
  using StoreValC = PipeArray<class _StoreValC, float, 16, 2>;
  using StorePortValC = PipeArray<class _StorePortValC, float, 16, 2>;

  // using StoreAddrPerLoadC = PipeArray<class _StoreAddrPerLoadC, addr_tag_t, 16, 2, 2>;

  using MemEndSignal = pipe<class _MemEndSignal, bool, 1>;

  using LoadValA0 = pipe<class _LoadValA0, float, 16>;
  using LoadValB0 = pipe<class _LoadValB0, float, 16>;
  
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

  // using TagBeforeLoop = PipeArray<class _TagBefore, uint, 16, 2>;
  // using TagAfterLoop = PipeArray<class _TagAfter, uint, 16, 2>;
  q.single_task<class AGU0>([=]() [[intel::kernel_args_restrict]] {
    uint tag = 0;


    for (int i = 0; i < NI; i++) { 
      const uint tagBeforeLoop0 = tag;
      const uint tagBeforeLoop1 = (NJ >= tag) ? 0u : (tag - NJ);
      
      uint localTag = tag;

      int LoadAddr0 = INVALID_ADDR; 
      int StoreAddr0 = INVALID_ADDR; 
      for (int j = 0; j < NJ; j++) {
        LoadAddr0 = i * NI + j;
        const bool posDepDistFromSt0 = LoadAddr0 > StoreAddr0;
        LoadAddrC::PipeAt<0>::write({LoadAddr0,
                                     localTag,
                                     {tagBeforeLoop0, tagBeforeLoop1},
                                     {posDepDistFromSt0, false}});
        // LoadValC::PipeAt<0>::write(C[i*NI + j]);

        localTag++;
        StoreAddr0 = i * NI + j;
        StoreAddrC::PipeAt<0>::write({StoreAddr0, localTag});
        StorePortAddrC::PipeAt<0>::write({StoreAddr0, localTag});
      }

      tag += NJ + NK*NJ;
    }

    // StoreAddrC::PipeAt<0>::write({MAX_INT, MAX_INT});
    StorePortAddrC::PipeAt<0>::write({MAX_INT, MAX_INT});
    PRINTF("Done AGU0\n");
  });
  q.single_task<class AGU1>([=]() [[intel::kernel_args_restrict]] {
    uint tag = 0;
    for (int i = 0; i < NI; i++) { 

      tag += NJ;
      const uint tagBeforeLoop0 = tag - NJ;

      int LoadAddr1 = INVALID_ADDR; 
      int StoreAddr1 = INVALID_ADDR; 
      for (int k = 0; k < NK; k++) {

        const uint tagBeforeLoop1 = tag;
        for (int j = 0; j < NJ; j++) {
          LoadAddr1 = i * NI + j;
          const bool posDepDistFromSt1 = LoadAddr1 > StoreAddr1;
          LoadAddrC::PipeAt<1>::write({LoadAddr1,
                                       tag,
                                       {tagBeforeLoop0, tagBeforeLoop1},
                                       {false, posDepDistFromSt1}});

          tag++;
          StoreAddr1 = i * NI + j;
          StoreAddrC::PipeAt<1>::write({StoreAddr1, tag});
          StorePortAddrC::PipeAt<1>::write({StoreAddr1, tag});
        }

      }

    }

    // StoreAddrC::PipeAt<1>::write({MAX_INT, MAX_INT});
    StorePortAddrC::PipeAt<1>::write({MAX_INT, MAX_INT});
    PRINTF("** Done AGU1\n");
  });

  auto memEvents = StreamingMemory<0, MemEndSignal, LoadAddrC,
                                   LoadValC, StoreAddrC, StoreValC,
                                   StorePortAddrC, StorePortValC, 2, 2>(q, C);

  auto eventL0 = q.single_task<class Loop0>([=]() [[intel::kernel_args_restrict]] {
    while (l0::read()) {
      // auto i = i0::read();
      for (int j = 0; j < NJ; j++) {
        auto Val = LoadValC::PipeAt<0>::read() * beta;
        StoreValC::PipeAt<0>::write(Val);
        StorePortValC::PipeAt<0>::write(Val);
        // C[i * NI + j] *= beta;
      }
    }
    PRINTF("Done Loop0\n");
  });

  auto eventL1 = q.single_task<class Loop1>([=]() [[intel::kernel_args_restrict]] {
    while (l1::read()) {
      // auto i = i1::read();
      for (int k = 0; k < NK; k++) {
        for (int j = 0; j < NJ; j++) {
          // C[i * NI + j] += alpha * A[i * NI + k] * B[k * NK + j];
          auto Val = LoadValC::PipeAt<1>::read() +
                            (beta * LoadValA0::read() * LoadValB0::read());
          StoreValC::PipeAt<1>::write(Val);
          StorePortValC::PipeAt<1>::write(Val);
          // PRINTF("Done i=%d, k=%d, j=%d\n", i, k, j);
        }
      }
    }

    PRINTF("Done Loop1\n");
    MemEndSignal::write(0);
  });

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) { 
      l0::write(1);
      // i0::write(i);

      l1::write(1);
      // i1::write(i);
    }

    l0::write(0);
    l1::write(0);
  });

  for (auto &e : memEvents) e.wait();
  event.wait();
  q.copy(C, h_C.data(), h_C.size()).wait();

  sycl::free(A, q);
  sycl::free(B, q);
  sycl::free(C, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
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
  for (int i = 0; i < NI; i++) {
    for (int j = 0; j < NJ; j++) {
      C[i * NI + j] *= beta;
    }

    for (int k = 0; k < NK; k++) {
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
