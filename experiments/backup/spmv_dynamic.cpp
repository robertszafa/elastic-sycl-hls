#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <vector>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "memory_utils.hpp"
#include "exception_handler.hpp"
#include "device_print.hpp"

#include "LoadStoreQueueBRAM.hpp"

using namespace sycl;

using TYPE = int;

constexpr int kM = 20;

// Forward declare kernel name.
class MainKernel;
class MainKernel_AGU_0;


double spmv_kernel(queue &q, std::vector<TYPE> &h_matrix,
                   const std::vector<int> &h_row, const std::vector<int> &h_col,
                   const std::vector<TYPE> &h_a, const int M) {

  TYPE *matrix = fpga_tools::toDevice(h_matrix, q);
  int *row = fpga_tools::toDevice(h_row, q);
  int *col = fpga_tools::toDevice(h_col, q);
  TYPE *a = fpga_tools::toDevice(h_a, q);

  // LSQ_PIPES.
  using pipes_ld_req_0 = PipeArray<class class_pipes_ld_req_0, ld_req_lsq_bram_t, 64, 2>;
  using pipes_ld_val_0 = PipeArray<class class_pipes_ld_val_0, TYPE, 64, 2>;

  using pipes_st_req_0 = PipeArray<class class_pipe_st_req_0, st_req_lsq_bram_t, 32, 2>;
  using pipes_st_val_0 = PipeArray<class class_pipe_st_val_0, TYPE, 32, 2>;
  // using pipes_st_val_0 = PipeArray<class class_pipe_st_val_0, tagged_val_lsq_bram_t<TYPE>, 32, 2>;

  using pipe_end_lsq_signal_0 = pipe<class class_pipe_end_lsq_signal_0, bool, 2>;

  q.single_task<MainKernel_AGU_0>([=]() [[intel::kernel_args_restrict]] {
    uint tag = 0;
    uint ld_tag = 0;
    int ptr = 0;
    for (int k = 1; k < M; k++) {
      for (int p = 0; p < M; p++) {
        pipes_ld_req_0::PipeAt<0>::write({k * M + row[ptr], tag, ld_tag});
        ld_tag += 1;
        pipes_ld_req_0::PipeAt<1>::write({(k - 1) * M + col[ptr], tag, ld_tag});
        ld_tag += 1;

        tag += 1;
        pipes_st_req_0::PipeAt<0>::write({k * M + row[ptr], tag});


        ptr++;
      }
    }
  });

  auto lsqEvent_0 =
      LoadStoreQueueBRAM<TYPE, pipes_ld_req_0, pipes_ld_val_0, pipes_st_req_0,
                         pipes_st_val_0, pipe_end_lsq_signal_0, kM*kM, 2, 1, 2, 32>(q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    uint tag = 0;
    for (int k = 1; k < M; k++) {
      for (int p = 0; p < M; p++) {
        pipes_st_val_0::PipeAt<0>::write(pipes_ld_val_0::PipeAt<0>::read() +
                              (pipes_ld_val_0::PipeAt<1>::read() * a[k]));
      }
    }
    pipe_end_lsq_signal_0::write(tag);
  });

  event.wait();
  lsqEvent_0.wait();
  // q.memcpy(h_matrix.data(), matrix, sizeof(h_matrix[0]) * h_matrix.size())
  //     .wait();

  sycl::free(matrix, q);
  sycl::free(row, q);
  sycl::free(col, q);
  sycl::free(a, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void spmv_cpu(std::vector<TYPE> &matrix, const std::vector<int> &row,
              const std::vector<int> &col, std::vector<TYPE> &a, const int M) {
  int ptr = 0; 
  for (int k = 1; k < M; k++) {
    for (int p = 0; p < M; p++) {
      matrix[row[ptr]] += a[p] * matrix[col[ptr]];
      ptr++;
    }
  }
}

void init_data(std::vector<TYPE> &matrix, std::vector<TYPE> &a,
               std::vector<int> &col_index, std::vector<int> &row_ptr,
               const uint M, 
               const uint percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(1, 99);
  auto dice = std::bind(distribution, generator);

  for (int i = 0; i < M*M; ++i) {
    // These indexes don't make sense algorithmically but it's easier to control
    // the % of data dependencies this way.
    col_index[i] = (dice() < percentage) ? 1 : i;
    row_ptr[i] = col_index[i];
  }
  std::fill(matrix.begin(), matrix.end(), 1);
  std::fill(a.begin(), a.end(), 1);
}




int main(int argc, char *argv[]) {
  int M = kM;
  int PERCENTAGE = 0;
  try {
    if (argc > 1) {
      M = int(atoi(argv[1]));
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
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<TYPE> matrix(M * M);
    std::vector<TYPE> golden_matrix(M * M);
    std::vector<TYPE> a(M);

    std::vector<int> row_ptr(M*M);
    std::vector<int> col_index(M*M);

    init_data(matrix, a, col_index, row_ptr, M,  PERCENTAGE);
    std::copy(matrix.begin(), matrix.end(), golden_matrix.begin());
    spmv_cpu(golden_matrix, row_ptr, col_index, a, M);

    auto kernel_time = spmv_kernel(q, matrix, row_ptr, col_index, a, M);

    std::cout << "Kernel time (ms): " << kernel_time << "\n";

  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
