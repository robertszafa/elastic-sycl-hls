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

// #include "StreamingMemory.hpp"
#include "TaggedStreamingMemorySingleKernelCache.hpp"

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
                  std::vector<int> &h_D, const int kDramStoreDelay) {

  int *A = fpga_tools::toDevice(h_A, q);
  int *B = fpga_tools::toDevice(h_B, q);
  int *C = fpga_tools::toDevice(h_C, q);
  int *D = fpga_tools::toDevice(h_D, q);

  std::vector zeroVec(h_D.size(), 0);
  int* tmp = fpga_tools::toDevice(zeroVec, q);

  using LoadAddrTmp = PipeArray<class _LoadAddrTmp0, addr_tag_mintag_t<1>, 64, 2>;
  using LoadValTmp = PipeArray<class _LoadValTmp0, int, 64, 2>;

  using StoreAddrTmp0 = PipeArray<class _StoreAddrTmp0, addr_tag_t, 64, 1>;
  using StoreValTmp0 = PipeArray<class _StoreValTmp0, int, 64, 1>;
  using StorePortAddrTmp0 = PipeArray<class _StoreAddrPortTmp0, addr_tag_t, 64, 1>;
  using StorePortValTmp0 = PipeArray<class _StoreValPortTmp0, int, 64, 1>;

  // using StorePortAddrPerLoad = PipeArray<class _StorePortAddrPerLoad, addr_tag_t, 64, 1, 2>;
  // using AdvanceStoreAddrTmp0 = PipeArray<class _AdvanceStoreAddrTmp0, addr_tag_t, 16, 1, 2>;
  // using StoreValTmp0Copy = pipe<class _StoreValTmp0Copy, int>;

  using EndSignalPipe = pipe<class _EndSignal, bool, 1>;
  
  using LoadValA = pipe<class _LoadValA, int, 16>;
  auto sldA = q.single_task<class sldA>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) {
      for (int j = 0; j < NJ; j++) {
        for (int k = 0; k < NJ; ++k) {
          LoadValA::write(A[i * NI + k]);
        }
      }
    }
  });
  using LoadValB = pipe<class _LoadValB, int, 16>;
  auto sldB = q.single_task<class sldB>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) {
      for (int j = 0; j < NJ; j++) {
        for (int k = 0; k < NJ; ++k) {
          LoadValB::write(B[k * NK + j]);
        }
      }
    }
  });
  using LoadValC = pipe<class _LoadValC, int, 16>;
  auto sldC = q.single_task<class sldC>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) {
      for (int j = 0; j < NJ; j++) {
        for (int k = 0; k < NJ; ++k) {
          LoadValC::write(C[k * NK + j]);
        }
      }
    }
  });
  using LoadValD = pipe<class _LoadValD, int, 16>;
  auto sldD = q.single_task<class sldD>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) {
      for (int j = 0; j < NJ; j++) {
        // BurstCoalescedLSU
        LoadValD::write(D[i * NK + j]);
      }
    }
  });
  using StoreValD = pipe<class _StoreValD, int, 64>;
  auto sstD = q.single_task<class sstD>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) {
      for (int j = 0; j < NJ; j++) {
        // BurstCoalescedLSU
        D[i * NK + j] = StoreValD::read();
      }
    }
  });


  auto agu0 = q.single_task<class agu0>([=]() [[intel::kernel_args_restrict]] {
    uint tag = 0;
    for (int i = 0; i < NI; i++) {
      int LoadAddr0 = INVALID_ADDR;
      int StoreAddr0 = INVALID_ADDR;
      for (int j = 0; j < NJ; j++) {
        LoadAddr0 = i * NI + j;
        LoadAddrTmp::PipeAt<0>::write({LoadAddr0, tag, {0u}, {LoadAddr0 > StoreAddr0}});

        tag++;
        StoreAddr0 = i * NI + j;
        StoreAddrTmp0::write({StoreAddr0, tag});
        StorePortAddrTmp0::write({StoreAddr0, tag});
        // StorePortAddrPerLoad::PipeAt<0, 0>::write({i * NI + j, tag});
        // StorePortAddrPerLoad::PipeAt<0, 1>::write({i * NI + j, tag});
        // AdvanceStoreAddrTmp0::PipeAt<0,0>::write({i * NI + j, tag});
      }
    }

    // LoadAddrTmp0::write({MAX_INT});
    // StoreAddrTmp0::write({MAX_INT, tag});
    // StoreAddrTmp0::PipeAt<0>::write({MAX_INT, MAX_INT});
    StorePortAddrTmp0::PipeAt<0>::write({MAX_INT, MAX_INT});
  });

  auto agu1 = q.single_task<class agu1>([=]() [[intel::kernel_args_restrict]] {
    uint tag = NI*NJ;
    for (int i = 0; i < NI; i++) {
      for (int j = 0; j < NJ; j++) {
        for (int k = 0; k < NJ; ++k) {
          LoadAddrTmp::PipeAt<1>::write({i * NI + k, tag, {0u}, {false}});
        }
      }
    }
    // LoadAddrTmp1::write({MAX_INT});
  });

  auto memEvents =
      StreamingMemory<0, EndSignalPipe, LoadAddrTmp, LoadValTmp,
                      StoreAddrTmp0, StoreValTmp0, StorePortAddrTmp0,
                      StorePortValTmp0, 2, 1>(q, tmp);

  auto event0 = q.single_task<MainKernel0>([=]() [[intel::kernel_args_restrict]] {
    for (uint i = 0; i < NI; i++) {
      for (uint j = 0; j < NJ; j++) {
        // PRINTF("Loop0 (i, j) = (%d, %d)\n", i, j);
        // int x = tmp[i * NI + j];
        int x = LoadValTmp::PipeAt<0>::read();
        for (uint k = 0; k < NK; ++k) {
          x += alpha * LoadValA::read() * LoadValB::read();
        }
        // tmp[i * NI + j] = x;
        // StoreAddrValTmp0::write({i * NI + j, x});
        StoreValTmp0::write(x);
        StorePortValTmp0::write(x);
        // StoreValTmp0Copy::write(x);
      }
    }
    // StoreAddrValTmp0::write({INVALID_ADDR});
    PRINTF("** Done loop 0\n");
  });

  auto event1 = q.single_task<MainKernel1>([=]() [[intel::kernel_args_restrict]] {
    for (uint i = 0; i < NI; i++) {
      for (uint j = 0; j < NJ; j++) {
        // PRINTF("Loop1 (i, j) = (%d, %d)\n", i, j);
        // int x = D[i * NI + j] * beta;
        int x = LoadValD::read() * beta;
        for (uint k = 0; k < NJ; ++k) {
          // x += tmp[i * NI + k] * C[k * NK + j];
          x += LoadValTmp::PipeAt<1>::read() * LoadValC::read();
        }
        // D[i * NI + j] = x;
        StoreValD::write(x);
      }
    }

    EndSignalPipe::write(true);
    PRINTF("** Done loop 1\n");
  });

  event0.wait();
  event1.wait();
  sstD.wait();
  for (auto &e : memEvents) {
    e.wait();
  }
  
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
  int dramStoreDelay = 1024;
  try {
    if (argc > 1) {
      N = int(atoi(argv[1]));
    }
    if (argc > 2) {
      dramStoreDelay = int(atoi(argv[2]));
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
    std::vector<int> D(S, 4); 
    std::vector<int> D_cpu(S, 4); 

    auto kernel_time = kernel_2mm(q, 2, 2, NI, NJ, NK, A, B, C, D, dramStoreDelay);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    kernel_2mm_cpu(2, 2, NI, NJ, NK, A, B, C, D_cpu);

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
