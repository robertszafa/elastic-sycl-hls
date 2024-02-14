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
#include "TaggedStreamingMemory.hpp"
// #include "ComplicatedTaggedStreamingMemory.hpp"
// #include "BurstingStreamingMemory.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

struct this_sched_t {
  int a; int b; int c; int d; 
};
bool operator==(const this_sched_t &LHS, const this_sched_t &RHS) {
  return LHS.a == RHS.a && LHS.b == RHS.b && LHS.c == RHS.c && LHS.d == RHS.d;
}

bool operator>(const this_sched_t &LHS, const this_sched_t &RHS) {
  if (LHS.a > RHS.a) {
    return true;
  } else if (LHS.a == RHS.a) {
    if (LHS.b > RHS.b) {
      return true;
    } else if (LHS.b == RHS.b) {
      if (LHS.c > RHS.c) {
        return true;
      } else if (LHS.c == RHS.c) {
        return LHS.d > RHS.d;
      }
    }
  }

  return false;
}
bool operator>=(const this_sched_t &LHS, const this_sched_t &RHS) {
  if (LHS.a > RHS.a) {
    return true;
  } else if (LHS.a == RHS.a) {
    if (LHS.b > RHS.b) {
      return true;
    } else if (LHS.b == RHS.b) {
      if (LHS.c > RHS.c) {
        return true;
      } else if (LHS.c == RHS.c) {
        return LHS.d >= RHS.d;
      }
    }
  }

  return false;
}
bool operator<(const this_sched_t &LHS, const this_sched_t &RHS) {
  if (LHS.a < RHS.a) {
    return true;
  } else if (LHS.a == RHS.a) {
    if (LHS.b < RHS.b) {
      return true;
    } else if (LHS.b == RHS.b) {
      if (LHS.c < RHS.c) {
        return true;
      } else if (LHS.c == RHS.c) {
        return LHS.d < RHS.d;
      }
    }
  }

  return false;
}

bool operator<=(const this_sched_t &LHS, const this_sched_t &RHS) {
  if (LHS.a < RHS.a) {
    return true;
  } else if (LHS.a == RHS.a) {
    if (LHS.b < RHS.b) {
      return true;
    } else if (LHS.b == RHS.b) {
      if (LHS.c < RHS.c) {
        return true;
      } else if (LHS.c == RHS.c) {
        return LHS.d <= RHS.d;
      }
    }
  }

  return false;
}
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

  using LoadAddrC0 = pipe<class _LoadAddrC0, addr_tag_t, 16>;
  using LoadValC0 = pipe<class _LoadValC0, float, 16>;
  using StoreAddrC0 = pipe<class _StoreAddrC0, addr_tag_t, 16>;
  using StoreValC0 = pipe<class _StoreValC0, float, 16>;

  using LoadAddrC1 = pipe<class _LoadAddrC1, addr_tag_t, 16>;
  using LoadValC1 = pipe<class _LoadValC1, float, 16>;
  using StoreAddrC1 = pipe<class _StoreAddrC1, addr_tag_t, 16>;
  using StoreValC1 = pipe<class _StoreValC1, float, 16>;

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

  q.single_task<class AGU0>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < NI; i++) { 
      for (int j = 0; j < NJ; j++) {
        // PRINTF("Tag st0 = %d\n", (j + 1 + i*NJ*1 + i*NJ*NK));
        LoadAddrC0::write({i * NI + j, (j + 0 + i*NJ*1 + i*NJ*NK)});
        StoreAddrC0::write({i * NI + j, (j + 1 + i*NJ*1 + i*NJ*NK)});
      }
    }

    LoadAddrC0::write({INVALID_ADDR});
    StoreAddrC0::write({INVALID_ADDR});
  });
  
  q.single_task<class AGU1>([=]() [[intel::kernel_args_restrict]] {
    // For each gate, send its range and dependence distance.
    // LoadAddrC1::write({NJ, NJ-1});
    // LoadAddrC1::write({NJ*NK, NJ-1});

    for (int i = 0; i < NI; i++) { 
      for (int k = 0; k < NK; k++) {
        for (int j = 0; j < NJ; j++) {
          // PRINTF("Tag st 1 = %d\n", (i*(NJ + NK*NJ) + k*NK + NJ + j + 1));

          LoadAddrC1::write({i * NI + j, (i*(NJ + NK*NJ) + k*NK + NJ + j + 0)});
          StoreAddrC1::write({i * NI + j, (i*(NJ + NK*NJ) + k*NK + NJ + j + 1)});
        }
      }
    }

    LoadAddrC1::write({INVALID_ADDR});
    StoreAddrC1::write({INVALID_ADDR});
  });


  using InSldC1 = PipeArray<class _InSldC1, addr_tag_val_t<float>, 16, 2>;

  // using InSst1 = PipeArray<class _InSstC1, addr_tag_val_t<float>, 16, 1>;
  // using OutSstC0 = PipeDuplicator<class _OutSstC0, addr_tag_val_t<float>,
  //                                 InSldC1::PipeAt<0>
  //                                 , InSst1::PipeAt<0>
  //                                 >;
  // using OutSstC1 = PipeDuplicator<class _OutSstC1, addr_tag_val_t<float>,
  //                                 InSldC1::PipeAt<1>
  //                                 >;

  auto sldC0 = StreamingLoad<0, LoadAddrC0, LoadValC0>(q, C);
  // auto sstC0 = StreamingStore<0, StoreAddrC0, StoreValC0, NoPipe, 0, OutSstC0, 1>(q, C);
  auto sstC0 = StreamingStore<0, StoreAddrC0, StoreValC0, NoPipe, 0, InSldC1::PipeAt<0>, 1>(q, C);

  auto sldC1 = StreamingLoad<1, LoadAddrC1, LoadValC1, InSldC1, 2>(q, C, NJ-1);
  // auto sstC1 = StreamingStore<1, StoreAddrC1, StoreValC1, InSst1, 1, OutSstC1, 1>(q, C, NJ); // for st->st deps, one greater dep distance
  auto sstC1 = StreamingStore<1, StoreAddrC1, StoreValC1, NoPipe, 0, InSldC1::PipeAt<1>, 1>(q, C);

  auto eventL0 = q.single_task<class Loop0>([=]() [[intel::kernel_args_restrict]] {
    while (l0::read()) {
      // auto i = i0::read();
      for (int j = 0; j < NJ; j++) {
        StoreValC0::write(LoadValC0::read() * beta);
        // C[i * NI + j] *= beta;
      }
    }
    // PRINTF("Done Loop0\n");
  });

  auto eventL1 = q.single_task<class Loop1>([=]() [[intel::kernel_args_restrict]] {
    while (l1::read()) {
      // auto i = i1::read();
      for (int k = 0; k < NK; k++) {
        for (int j = 0; j < NJ; j++) {
          // C[i * NI + j] += alpha * A[i * NI + k] * B[k * NK + j];
          StoreValC1::write(LoadValC1::read() +
                            (beta * LoadValA0::read() * LoadValB0::read()));
          // PRINTF("Done i=%d, k=%d, j=%d\n", i, k, j);
        }
      }
    }

    // PRINTF("Done Loop1\n");
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

  event.wait();
  sstC0.wait();
  sstC1.wait();
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
