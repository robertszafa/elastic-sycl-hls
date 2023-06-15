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

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

constexpr int kM = 20;

double spmv_kernel(queue &q, std::vector<float> &h_matrix,
                   const std::vector<int> &h_row, const std::vector<int> &h_col,
                   const std::vector<float> &h_a, const int M) {

  float *matrix_dram = fpga_tools::toDevice(h_matrix, q);
  int *row = fpga_tools::toDevice(h_row, q);
  int *col = fpga_tools::toDevice(h_col, q);
  float *a = fpga_tools::toDevice(h_a, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    float matrix[kM * kM]; 

    #ifdef TEST
    for (int i=0; i < (M * M); ++i) 
      matrix[i] = matrix_dram[i];
    #endif

    for (int k = 1; k < M; k++) {
      for (int p = 0; p < M; p++) {
        matrix[k * M + row[p]] += a[p] * matrix[(k - 1) * kM + col[p]];
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

void spmv_cpu(std::vector<float> &matrix, const std::vector<int> &row, const std::vector<int> &col,
              std::vector<float> &a, const int M) {
  for (int k = 1; k < M; k++) {
    for (int p = 0; p < M; p++) {
      matrix[k * M + row[p]] += a[p] * matrix[(k - 1) * M + col[p]];
    }
  }
}

void init_data(std::vector<float> &matrix, std::vector<float> &a, std::vector<int> &col_index,
               std::vector<int> &row_ptr, const uint M, 
               const uint percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind(distribution, generator);

  for (int r = 0; r < M; ++r) {
    col_index[r] = (dice() < percentage) ? std::max(r - 1, 0) : r;
    row_ptr[r] = (dice() < percentage) ? std::max(r - 1, 0) : r;

    a[r] = 1.0f;
  }

  std::fill(matrix.begin(), matrix.end(), 1.0f);
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
#else  // #if FPGA_EMULATOR
  auto d_selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif
  try {
    // Enable profiling.
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: " << q.get_device().get_info<info::device::name>() << "\n";
    
    std::vector<float> matrix(kM * kM);
    std::vector<float> golden_matrix(kM * kM);
    std::vector<float> a(kM);

    std::vector<int> row_ptr(kM);
    std::vector<int> col_index(kM);

    init_data(matrix, a, col_index, row_ptr, kM,  PERCENTAGE);
    std::copy(matrix.begin(), matrix.end(), golden_matrix.begin());
    spmv_cpu(golden_matrix, row_ptr, col_index, a, kM);

    auto kernel_time = spmv_kernel(q, matrix, row_ptr, col_index, a, kM);

    std::cout << "Kernel time (ms): " << kernel_time << "\n";

    if (std::equal(matrix.begin(), matrix.end(), golden_matrix.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cerr << "Failed";
    }
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

