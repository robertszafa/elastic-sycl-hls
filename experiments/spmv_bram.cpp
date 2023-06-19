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

double spmv_kernel(queue &q, std::vector<int> &h_matrix,
                   const std::vector<int> &h_row, const std::vector<int> &h_col,
                   const std::vector<int> &h_a, const int M) {

  int *matrix_dram = fpga_tools::toDevice(h_matrix, q);
  int *row = fpga_tools::toDevice(h_row, q);
  int *col = fpga_tools::toDevice(h_col, q);
  int *a = fpga_tools::toDevice(h_a, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int matrix[kM * kM]; 

    #ifdef TEST
    for (int i=0; i < (M * M); ++i) 
      matrix[i] = matrix_dram[i];
    #endif

    int ptr = 0; 
    for (int k = 1; k < kM; k++) {
      for (int p = 0; p < kM; p++) {
        matrix[row[ptr]] += a[p] * matrix[col[ptr]];
        ptr++;
      }
    }

    #ifdef TEST
    for (int i=0; i < (M * M); ++i) 
      matrix_dram[i] = matrix[i];
    #endif

  });

  event.wait();
  q.memcpy(h_matrix.data(), matrix_dram, sizeof(h_matrix[0]) * h_matrix.size()).wait();

  sycl::free(matrix_dram, q);
  sycl::free(row, q);
  sycl::free(col, q);
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
      matrix[row[ptr]] += a[p] * matrix[col[ptr]];
      ptr++;
    }
  }
}

void init_data(std::vector<int> &matrix, std::vector<int> &a,
               std::vector<int> &col_index, std::vector<int> &row_ptr,
               const uint M, const uint percentage) {
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
  int ARRAY_SIZE = kM;
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

    init_data(matrix, a, col_index, row_ptr, kM,  PERCENTAGE);
    std::copy(matrix.begin(), matrix.end(), golden_matrix.begin());
    spmv_cpu(golden_matrix, row_ptr, col_index, a, kM);

    auto kernel_time = spmv_kernel(q, matrix, row_ptr, col_index, a, kM);

    std::cout << "Kernel time (ms): " << kernel_time << "\n";

#ifdef TEST
    if (std::equal(matrix.begin(), matrix.end(), golden_matrix.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cerr << "Failed";
    }
#endif
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

