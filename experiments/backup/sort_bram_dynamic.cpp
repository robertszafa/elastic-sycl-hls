#include "exception_handler.hpp"
#include "memory_utils.hpp"
#include "LoadStoreQueueBRAM.hpp"
#include "device_print.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/sycl.hpp>
#include <vector>
using namespace sycl;
class MainKernel;
constexpr int kN = 100;
double sort_kernel(queue &q, std::vector<int> &h_A) {
  const int N = h_A.size();
  int *A_dram = fpga_tools::toDevice(h_A, q);
  using pipes_ld_req_0 =
      PipeArray<class pipes_ld_req_0_class, ld_req_lsq_bram_t, 16, 2>;
  using pipes_st_req_0 =
      PipeArray<class pipes_st_req_0_class, st_req_lsq_bram_t, 16, 2>;
  using pipes_ld_val_0 = PipeArray<class pipes_ld_val_0_class, int, 16, 2>;
  
  using pipes_st_val_0 =
      PipeArray<class pipes_st_val_0_class, tagged_val_lsq_bram_t<int>, 16, 2>;
  // using pipes_st_val_0 =
  //     PipeArray<class pipes_st_val_0_class, int, 16, 2>;

  using pipe_end_lsq_signal_0 = pipe<class pipe_end_lsq_signal_0_class, bool>;
  
  using pred_pipe = pipe<class pred_pipe_0_class, bool, 32>;
  using dep_in_pipe =
      PipeArray<class dep_in_pipe_0_class, int, 32, 4>;

  auto lsqEvent_0 =
      LoadStoreQueueBRAM<int, pipes_ld_req_0, pipes_ld_val_0, pipes_st_req_0,
                         pipes_st_val_0, pipe_end_lsq_signal_0, true, 100, 2, 2, 4,
                         16>(q);


  auto event2 = q.single_task<class AGU>([=]() [[intel::kernel_args_restrict]] {
    uint tag = 0, ld_tag = 0;
    // while (pred_pipe::read()) {
      // auto j = dep_in_pipe::PipeAt<0>::read();
    for (int i = 0; i < N - 1; i++) {
      for (int j = 0; j < N - i - 1; j++) {

        pipes_ld_req_0::PipeAt<0>::write({j, tag, ld_tag});
        pipes_ld_req_0::PipeAt<1>::write({j + 1, tag, ld_tag + 1});
        // auto left = pipes_ld_val_0::PipeAt<0>::read();
        // auto right = pipes_ld_val_0::PipeAt<1>::read();

        ld_tag += 2;

        // if (left > right) {
        tag++;
        pipes_st_req_0::PipeAt<0>::write({j, tag});
        // pipes_st_val_0::PipeAt<0>::write({right, tag});

        tag++;
        pipes_st_req_0::PipeAt<1>::write({j + 1, tag});
        // pipes_st_val_0::PipeAt<1>::write({left, tag});
        // }
      }
    }

  });

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int i, j;
    uint tag = 0, ld_tag = 0;
    for (i = 0; i < N - 1; i++) {
      for (j = 0; j < N - i - 1; j++) {

      //   pipes_ld_req_0::PipeAt<0>::write({j, tag, ld_tag});
      //   pipes_ld_req_0::PipeAt<1>::write({j+1, tag, ld_tag+1});
        auto left = pipes_ld_val_0::PipeAt<0>::read();
        auto right = pipes_ld_val_0::PipeAt<1>::read();

      //   ld_tag++;
      //   // ld_tag += 2;
      //   // pipes_ld_req_0::PipeAt<0>::write({j, tag, ld_tag});
      //   // ld_tag++;

      //   // pipes_ld_req_0::PipeAt<1>::write({j+1, tag, ld_tag});
      //   // ld_tag++;

      //   // auto left = pipes_ld_val_0::PipeAt<0>::read();
      //   // auto right = pipes_ld_val_0::PipeAt<1>::read();
        if (left >= right) {
      //     pred_pipe::write(1);
      //     dep_in_pipe::PipeAt<0>::write(tag);
      //     dep_in_pipe::PipeAt<1>::write(left);
      //     dep_in_pipe::PipeAt<2>::write(right);
      //     dep_in_pipe::PipeAt<3>::write(j);

      //     tag += 2;
          tag++;
      //     // pipes_st_req_0::PipeAt<0>::write({j, tag});
          pipes_st_val_0::PipeAt<0>::write({right, tag, 1});

          tag++;
      //     // pipes_st_req_0::PipeAt<1>::write({j+1, tag});
          pipes_st_val_0::PipeAt<1>::write({left, tag, 1});
        } else {
          tag++;
          pipes_st_val_0::PipeAt<0>::write({-1, tag, 0});
          tag++;
          pipes_st_val_0::PipeAt<1>::write({-1, tag, 0});
        }
      }
    }

    // pred_pipe::write(0);
    pipe_end_lsq_signal_0::write(0);
  });

  lsqEvent_0.wait();
  event.wait();
  event2.wait();
  q.copy(A_dram, h_A.data(), h_A.size()).wait();
  sycl::free(A_dram, q);
  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;
  return time_in_ms;
}
int main(int argc, char *argv[]) {
  int ARRAY_SIZE = 1000;
  int PERCENTAGE = 0;
  try {
    if (argc > 1) {
      ARRAY_SIZE = int(atoi(argv[1]));
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
  std::cout << "SIMULATION:\n";
  auto d_selector = sycl::ext::intel::fpga_simulator_selector_v;
#elif FPGA_HW
  auto d_selector = sycl::ext::intel::fpga_selector_v;
#else
  auto d_selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif
  try {
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);
    auto device = q.get_device();
    std::cout << "Running on device: "
              << device.get_info<sycl::info::device::name>().c_str() << "\n";
    std::vector<int> arr(ARRAY_SIZE);
    std::vector<int> arr_cpu(ARRAY_SIZE);
    for (size_t i = 0; i < kN; ++i) {
      if (PERCENTAGE == 100)
        arr[i] = kN - i;
      else if (PERCENTAGE == 0)
        arr[i] = i;
      else
        arr[i] = rand();
    }
    std::copy_n(arr.begin(), kN, arr_cpu.begin());
    auto kernel_time = sort_kernel(q, arr);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";
    if (std::equal(arr.begin(), arr.end(), arr_cpu.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failed\n";
    }
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }
  return 0;
}