
class MainKernel_AGU_0;
#include "memory_utils.hpp"
#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <limits>
#include <math.h>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <sycl/ext/intel/fpga_extensions.hpp>
#include <vector>

#include "exception_handler.hpp"

// #include "LoadStoreQueueBRAM.hpp"
// #include "LoadStoreQueueBRAM_Muxes.hpp"
// #include "LoadStoreQueueBRAM_Acint.hpp"
// #include "LoadStoreQueueBRAM_Acint_regs.hpp"
#include "LoadStoreQueueBRAM_Acint_trunc.hpp"

using namespace sycl;

constexpr int UNROLL_FACTOR = 1;

constexpr int N = 1000;

// Forward declare kernel name.
class MainKernel;
double kernel_fpga(queue &q, const std::vector<int> &h_idx,
                        std::vector<int> &h_hist) {
  const int array_size = h_idx.size();
  int *idx = fpga_tools::toDevice(h_idx, q);
  auto *dramHist = fpga_tools::toDevice(h_hist, q);

  // LSQ_PIPES.
  using pipes_ld_req_0 = PipeArray<class pipes_ld_req_0_class, ld_req_lsq_bram_t, 4, 2>;
  using pipes_ld_val_0 = PipeArray<class pipes_ld_val_0_class, int, 4, 2>;
  using pipe_st_req_0 = PipeArray<class pipe_st_req_0_class, st_req_lsq_bram_t, 4, 2>;
  using pipe_st_val_0 = PipeArray<class pipe_st_val_0_class, tagged_val_lsq_bram_t<int>, 4, 2>;

  using pipe_end_lsq_signal_0 = pipe<class pipe_end_lsq_signal_0_class, bool, 1>;

  auto aguEvent = q.single_task<MainKernel_AGU_0>([=]() [[intel::kernel_args_restrict]] {
    uint tag = 0;

    for (int i = 0; i < array_size; i+=UNROLL_FACTOR) {
      pipes_ld_req_0:: template PipeAt<0>::write({idx[i], tag});

      tag++;
      pipe_st_req_0:: template PipeAt<0>::write({idx[i], tag });
    }

  });

  auto lsqEvent_0 =
      LoadStoreQueueBRAM<int, pipes_ld_req_0, pipes_ld_val_0, pipe_st_req_0,
                         pipe_st_val_0, pipe_end_lsq_signal_0, false, N, 1, 1, 4, 8>(q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < array_size; i+=UNROLL_FACTOR) {
      auto x = pipes_ld_val_0:: template PipeAt<0>::read();
      // tag++;
      pipe_st_val_0:: template PipeAt<0>::write({x + 1});
    }
    
    pipe_end_lsq_signal_0::write(1);
  });

  event.wait();
  lsqEvent_0.wait();
  aguEvent.wait();

  q.copy(dramHist, h_hist.data(), h_hist.size()).wait();
  sycl::free(idx, q);
  sycl::free(dramHist, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;
  return time_in_ms;
}


void histogram_cpu(const int *idx, int *hist, const int N) {
  for (int i = 0; i < N; i+=1) {
    auto x = hist[idx[i]];
    hist[idx[i]] = x + 1;
  }
}


void init_data(std::vector<int> &feature, std::vector<int> &hist,
               const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind(distribution, generator);

  int counter = 0;
  for (int i = 0; i < feature.size(); i++) {
    feature[i] = (dice() < percentage) ? 1: i;

    hist[i] = 0;
  }
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

    std::vector<int> feature(ARRAY_SIZE);
    std::vector<int> hist(ARRAY_SIZE);
    std::vector<int> hist_cpu(ARRAY_SIZE);

    init_data(feature, hist, PERCENTAGE);
    std::copy(hist.begin(), hist.end(), hist_cpu.begin());

    auto kernel_time = kernel_fpga(q, feature, hist);

    histogram_cpu(feature.data(), hist_cpu.data(), hist_cpu.size());

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

#ifdef TEST
    if (std::equal(hist.begin(), hist.end(), hist_cpu.begin()))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
#endif
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
