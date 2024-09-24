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

double spmv_kernel(queue &q, std::vector<int> &h_matrix,
                   const std::vector<int> &h_row, const std::vector<int> &h_col,
                   const std::vector<int> &h_a, const int M) {

  int *matrix = fpga_tools::toDevice(h_matrix, q);
  int *row = fpga_tools::toDevice(h_row, q);
  int *col = fpga_tools::toDevice(h_col, q);
  int *a = fpga_tools::toDevice(h_a, q);

  std::vector<std::pair<sycl::event, bool>> events; // {event, measureTimeBool} pairs

  auto main_event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int k = 1; k < M; k += 2) {
      for (int p = 0; p < M; p++) {
        matrix[k*M + row[k*M + p]] += a[p] * matrix[(k-1)*M + col[k*M + p]];
      }
      
      if ((k+1) < M) {
        for (int p = 0; p < M; p++) {
          matrix[(k+1)*M + row[(k+1)*M + p]] += a[p] * matrix[k*M + col[(k+1)*M + p]];
        }
      }
    }
  });

    
  events.push_back({main_event, true});
  for (auto &kv : events) kv.first.wait();
  q.copy(matrix, h_matrix.data(), h_matrix.size()).wait();

  sycl::free(matrix, q);
  sycl::free(row, q);
  sycl::free(col, q);
  sycl::free(a, q);

  double max_event_time = 0;
  for (auto &[e, toMeasure] : events) {
    if (toMeasure) {
      auto start = e.get_profiling_info<info::event_profiling::command_start>();
      auto end = e.get_profiling_info<info::event_profiling::command_end>();
      double this_event_time = static_cast<double>(end - start) / 1000000;
      max_event_time = std::max(max_event_time, this_event_time);
    }
  }

  return max_event_time;
}

void spmv_cpu(std::vector<int> &matrix, const std::vector<int> &row, const std::vector<int> &col,
              std::vector<int> &a, const int M) {
  for (int k = 1; k < M; k += 2) {
    for (int p = 0; p < M; p++) {
      matrix[k*M + row[k*M + p]] += a[p] * matrix[(k-1)*M + col[k*M + p]];
    }
    
    if ((k+1) < M) {
      for (int p = 0; p < M; p++) {
        matrix[(k+1)*M + row[(k+1)*M + p]] += a[p] * matrix[k*M + col[(k+1)*M + p]];
      }
    }
  }
}

void init_data(std::vector<int> &matrix, std::vector<int> &a,
               std::vector<int> &col_index, std::vector<int> &row_ptr,
               const uint M, const uint percentage) {
  for (int i = 0; i < M*M; ++i) {
    // These indexes don't make sense algorithmically but it's easier to control
    // the % of data dependencies this way.
    col_index[i] = rand() % M;
    row_ptr[i] = rand() % M;
  }
  std::fill(matrix.begin(), matrix.end(), 1);
  std::fill(a.begin(), a.end(), 1);

  std::sort(col_index.begin(), col_index.end());
  std::sort(row_ptr.begin(), row_ptr.end());
}

int main(int argc, char *argv[]) {
  int M = 1000;
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
    std::cout << "Running on device: " << q.get_device().get_info<info::device::name>() << "\n";
    
    std::vector<int> matrix(M * M);
    std::vector<int> golden_matrix(M * M);
    std::vector<int> a(M);

    std::vector<int> row_ptr(M*M);
    std::vector<int> col_index(M*M);

    init_data(matrix, a, col_index, row_ptr, M,  PERCENTAGE);
    std::copy(matrix.begin(), matrix.end(), golden_matrix.begin());
    spmv_cpu(golden_matrix, row_ptr, col_index, a, M);

    auto kernel_time = spmv_kernel(q, matrix, row_ptr, col_index, a, M);
    std::cout << "Kernel time (ms): " << kernel_time << "\n";

    if (std::equal(matrix.begin(), matrix.end(), golden_matrix.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cerr << "Failed\n";
    }
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
