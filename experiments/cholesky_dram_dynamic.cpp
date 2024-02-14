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
// #include "StreamingMemory.hpp"
#include "TaggedStreamingMemory.hpp"

#include "device_print.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double cholesky_kernel(queue &q, std::vector<float> &h_A, const int N) {
  auto *A = fpga_tools::toDevice(h_A, q);

  using LoadAddrA0 = pipe<class _LoadAddrA0, addr_tag_t, 16>;
  using LoadValA0 = pipe<class _LoadValA0, float, 16>;
  using StoreAddrA0 = pipe<class _StoreAddrA0, addr_tag_t, 16>;
  using StoreValA0 = pipe<class _StoreValA0, float, 16>;

  using LoadAddrA1 = pipe<class _LoadAddrA1, addr_tag_t, 16>;
  using LoadValA1 = pipe<class _LoadValA1, float, 16>;
  using StoreAddrA1 = pipe<class _StoreAddrA1, addr_tag_t, 16>;
  using StoreValA1 = pipe<class _StoreValA1, float, 16>;

  using LoadAddrA2 = pipe<class _LoadAddrA2, addr_tag_t, 16>;
  using LoadValA2 = pipe<class _LoadValA2, float, 16>;
  using LoadAddrA3 = pipe<class _LoadAddrA3, addr_tag_t, 16>;
  using LoadValA3 = pipe<class _LoadValA3, float, 16>;
  using LoadAddrA4 = pipe<class _LoadAddrA4, addr_tag_t, 16>;
  using LoadValA4 = pipe<class _LoadValA4, float, 16>;
  using LoadAddrA5 = pipe<class _LoadAddrA5, addr_tag_t, 16>;
  using LoadValA5 = pipe<class _LoadValA5, float, 16>;

  // using l00 = pipe<class _l00, bool, 16>;
  // using l01 = pipe<class _l01, bool, 16>;
  // using l000 = pipe<class _l000, bool, 16>;
  // using l01_i = pipe<class _l01_i, int, 16>;
  // using l00_i = pipe<class _l00_i, int, 16>;
  // using l000_j = pipe<class _l000_j, int, 16>;
  // using l000_a = pipe<class _l000_a, float, 16>;
  // using a_out = pipe<class _a_out, float, 16>;

  auto agu0 = q.single_task<class agu0>([=]() [[intel::kernel_args_restrict]] {
    int tag = 0;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < i; j++) {
        LoadAddrA0::write({i * N + j, tag + j});

        for (int k = 0; k < j; k++) {
          LoadAddrA1::write({i * N + k, tag + j});
          LoadAddrA2::write({j * N + k, tag + j});
        }

        LoadAddrA3::write({j * N + j, tag + j});
        // tag++;
        // PRINTF("st0 tag = %d\n", tag);
        StoreAddrA0::write({i * N + j, tag + j + 1});
      }

      tag += i + 1;
    }

    LoadAddrA0::write({INVALID_ADDR});
    LoadAddrA1::write({INVALID_ADDR});
    LoadAddrA2::write({INVALID_ADDR});
    LoadAddrA3::write({INVALID_ADDR});
    StoreAddrA0::write({INVALID_ADDR});
    PRINTF("Done agu0\n");
  });

  auto agu1 = q.single_task<class agu1>([=]() [[intel::kernel_args_restrict]] {
    int tag = 0;
    for (int i = 0; i < N; i++) {
      // tag += i;

      LoadAddrA4::write({i * N + i, tag + i});
      for (int k = 0; k < i; k++) {
        LoadAddrA5::write({i * N + k, tag + i});
      }

      tag += i + 1;
      // PRINTF("st1 tag = %d\n", tag);
      StoreAddrA1::write({i * N + i, tag});
    }

    LoadAddrA4::write({INVALID_ADDR});
    LoadAddrA5::write({INVALID_ADDR});
    StoreAddrA1::write({INVALID_ADDR});
    PRINTF("Done agu1\n");
  });

  // using InSld0 = PipeArray<class _InSld0, addr_tag_val_t<float>, 16, 2>;
  using InSld1 = PipeArray<class _InSld1, addr_tag_val_t<float>, 16, 2>;
  using InSld2 = PipeArray<class _InSld2, addr_tag_val_t<float>, 16, 2>;
  using InSld3 = PipeArray<class _InSld3, addr_tag_val_t<float>, 16, 2>;
  // using InSld4 = PipeArray<class _InSld4, addr_tag_val_t<float>, 16, 2>;
  using InSld5 = PipeArray<class _InSld5, addr_tag_val_t<float>, 16, 2>;
  // using InSst1 = PipeArray<class _InSst1, addr_tag_val_t<float>, 16, 1>;

  // auto sldA0 = StreamingLoad<0, LoadAddrA0, LoadValA0, InSld0, 2>(q, A, 0);
  auto sldA0 = StreamingLoad<0, LoadAddrA0, LoadValA0>(q, A, 0);
  auto sldA1 = StreamingLoad<1, LoadAddrA1, LoadValA1, InSld1, 2>(q, A, 0);
  auto sldA2 = StreamingLoad<2, LoadAddrA2, LoadValA2, InSld2, 2>(q, A, 0);
  auto sldA3 = StreamingLoad<3, LoadAddrA3, LoadValA3, InSld3, 2>(q, A, 0);
  // auto sldA4 = StreamingLoad<4, LoadAddrA4, LoadValA4, InSld4, 2>(q, A, 0);
  auto sldA4 = StreamingLoad<4, LoadAddrA4, LoadValA4>(q, A, 0);
  auto sldA5 = StreamingLoad<5, LoadAddrA5, LoadValA5, InSld5, 2>(q, A, 0);

  using OutSstC0 =
      PipeDuplicator<class _OutSstC0, addr_tag_val_t<float>, 
                    //  InSld0::PipeAt<0>,
                     InSld1::PipeAt<0>, 
                     InSld2::PipeAt<0>, 
                     InSld3::PipeAt<0>,
                    //  InSld4::PipeAt<0>, 
                     InSld5::PipeAt<0>>;
                    //  InSst1::PipeAt<0>>;

  using OutSstC1 =
      PipeDuplicator<class _OutSstC1, addr_tag_val_t<float>, 
                    //  InSld0::PipeAt<1>,
                     InSld1::PipeAt<1>, 
                     InSld2::PipeAt<1>, 
                     InSld3::PipeAt<1>,
                    //  InSld4::PipeAt<1>, 
                     InSld5::PipeAt<1>>;

  auto sstA0 = StreamingStore<0, StoreAddrA0, StoreValA0, NoPipe, 0, OutSstC0, 4>(q, A);
  // auto sstA1 = StreamingStore<1, StoreAddrA1, StoreValA1, InSst1, 1, OutSstC1, 6>(q, A, 1);
  auto sstA1 = StreamingStore<1, StoreAddrA1, StoreValA1, NoPipe, 0, OutSstC1, 4>(q, A);

  auto event0 = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < i; j++) {
        auto a = LoadValA0::read();

        for (int k = 0; k < j; k++) {
          a -= LoadValA1::read() * LoadValA2::read();
        }

        StoreValA0::write(a / LoadValA3::read());
      }
    }

    PRINTF("Done Loop0\n");
  });
  
  auto event1 = q.single_task<class MainKernel1>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < N; i++) {
      auto a = LoadValA4::read();
      for (int k = 0; k < i; k++) {
        auto ldA5 = LoadValA5::read();
        a -= (ldA5*ldA5);
      }
      
      StoreValA1::write(a);
    }

    PRINTF("Done Loop1\n");
  });

  event0.wait();
  event1.wait();
  sstA0.wait();
  sstA1.wait();
  q.copy(A, h_A.data(), h_A.size()).wait();

  sycl::free(A, q);

  auto start = event0.get_profiling_info<info::event_profiling::command_start>();
  auto end = event0.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void cholesky_cpu(std::vector<float> &A, const int N) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < i; j++) {
      auto a = A[i * N + j];
      for (int k = 0; k < j; k++) {
        a -= A[i * N + k] * A[j * N + k];
      }
      A[i * N + j] = a / A[j * N + j];
    }

    auto a = A[i * N + i];
    for (int k = 0; k < i; k++) {
      a -= A[i * N + k] * A[i * N + k];
    }
    A[i * N + i] = a; // sycl::sqrt(a)
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

    std::vector<float> A(N*N);
    std::vector<float> A_cpu(N*N);
    for (int i=0; i<N*N; ++i) {
      A[i] = rand() % 100;
      A_cpu[i] = A[i];
    }
    // std::iota(A.begin(), A.end(), 1);
    // std::iota(A_cpu.begin(), A_cpu.end(), 1);

    auto kernel_time = cholesky_kernel(q, A, N);
    cholesky_cpu(A_cpu, N);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(A.begin(), A.end(), A_cpu.begin(), almost_equal))
      std::cout << "Passed\n";
    else {
      std::cout << "Failed\n";
      for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
          if (A[i * N + j] != A_cpu[i * N + j]) {
            std::cout << "Mismatch in (" << i << ", " << j << ")\t" << A[i * N + j] << " != " << A_cpu[i * N + j] << "\n";
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
