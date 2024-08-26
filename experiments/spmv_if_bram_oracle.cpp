#include <sycl/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <vector>
#include <random>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "memory_utils.hpp"
#include "exception_handler.hpp"
#include "device_print.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

constexpr int kM = 20;

// Setting TEST will ensure test data is transfered from FPGA DRAM to to BRAM
// and back. This adds latency, so leave unset for the benchmarks.
#define TEST 1

double spmv_kernel(queue &q, std::vector<int> &h_matrix,
                   const std::vector<int> &h_row, const std::vector<int> &h_col,
                   const std::vector<int> &h_a, const int M) {

  int *matrix_dram = fpga_tools::toDevice(h_matrix, q);
  int *row_dram = fpga_tools::toDevice(h_row, q);
  int *col_dram = fpga_tools::toDevice(h_col, q);
  int *a = fpga_tools::toDevice(h_a, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    [[intel::singlepump]] 
    [[intel::max_replicates(1)]] 
    [[intel::numbanks(1)]] 
    int matrix[kM * kM]; 

    [[intel::singlepump]] 
    [[intel::max_replicates(1)]] 
    [[intel::numbanks(1)]] 
    int row[kM * kM]; 

    [[intel::singlepump]] 
    [[intel::max_replicates(1)]] 
    [[intel::numbanks(1)]] 
    int col[kM * kM]; 

    #if TEST
    for (int i=0; i < (M * M); ++i) {
      matrix[i] = matrix_dram[i];
      row[i] = row_dram[i];
      col[i] = col_dram[i];
    }
    #endif

    int ptr = 0;
    for (int k = 1; k < M; k++) {
      for (int p = 0; p < M; p++) {
        int c = col[ptr];
        int r = row[ptr];

        auto col = matrix[c];
        // if (col != 0)
          matrix[r] += a[p] * matrix[c] + (col != 0);
        
        ptr++;
      }
    }

    #if TEST
    for (int i=0; i < (M * M); ++i) 
      matrix_dram[i] = matrix[i];
    #endif

  });

  event.wait();
  q.copy(matrix_dram, h_matrix.data(), h_matrix.size()).wait();

  sycl::free(matrix_dram, q);
  sycl::free(row_dram, q);
  sycl::free(col_dram, q);
  sycl::free(a, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void spmv_cpu(std::vector<int> &matrix, const std::vector<int> &row, const std::vector<int> &col,
              std::vector<int> &a, const int M) {
  int ptr = 0; 
  for (int k = 1; k < M; k++) {
    for (int p = 0; p < M; p++) {
      int c = col[ptr];
      int r = row[ptr];

      auto col = matrix[c];
      if (col != 0)
        matrix[r] += a[p] * col;
      
      ptr++;
    }
  }
}

void init_data(std::vector<int> &matrix, std::vector<int> &a,
               std::vector<int> &col_index, std::vector<int> &row_ptr,
               const uint M, const uint percentage_raw,
               const uint percentage_misspec) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(1, 99);
  auto dice = std::bind(distribution, generator);

  for (int i = 0; i < M*M; ++i) {
    // These indexes don't make sense algorithmically but it's easier to control
    // the % of data dependencies this way.
    col_index[i] = (dice() < percentage_raw) ? 1 : i;
    row_ptr[i] = col_index[i];
    matrix[i] = (dice() < percentage_misspec) ? 0 : 1;
  }
  std::fill(a.begin(), a.end(), 2);
}

int main(int argc, char *argv[]) {
  int ARRAY_SIZE = kM;
  int PERCENTAGE_RAW = 0;
  int PERCENTAGE_MISSPECULATION = 0;
  try {
    if (argc > 1) {
      ARRAY_SIZE = int(atoi(argv[1]));
    }
    if (argc > 2) {
      PERCENTAGE_RAW = int(atoi(argv[2]));
      if (PERCENTAGE_RAW < 0 || PERCENTAGE_RAW > 100)
        throw std::invalid_argument("Invalid percentage.");
    }
    if (argc > 3) {
      PERCENTAGE_MISSPECULATION = int(atoi(argv[3]));
      if (PERCENTAGE_MISSPECULATION < 0 || PERCENTAGE_MISSPECULATION > 100)
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
    
    std::vector<int> matrix(kM * kM);
    std::vector<int> golden_matrix(kM * kM);
    std::vector<int> a(kM);

    std::vector<int> row_ptr(kM*kM);
    std::vector<int> col_index(kM*kM);

    init_data(matrix, a, col_index, row_ptr, kM,  PERCENTAGE_RAW, PERCENTAGE_MISSPECULATION);
    std::copy(matrix.begin(), matrix.end(), golden_matrix.begin());
    spmv_cpu(golden_matrix, row_ptr, col_index, a, kM);

    auto kernel_time = spmv_kernel(q, matrix, row_ptr, col_index, a, kM);

    std::cout << "Kernel time (ms): " << kernel_time << "\n";

#if TEST
    if (std::equal(matrix.begin(), matrix.end(), golden_matrix.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cerr << "Failed\n";
    }
#endif
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

