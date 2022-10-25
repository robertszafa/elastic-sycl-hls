#include <CL/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <vector>
#include <random>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "store_queue.hpp"
#include "memory_utils.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

double spmv_kernel(queue &q, std::vector<float> &h_matrix, const std::vector<int> &h_row,
                   const std::vector<int> &h_col, const std::vector<float> &h_a, const int M) {

  float *matrix = toDevice(h_matrix, q);
  int *row = toDevice(h_row, q);
  int *col = toDevice(h_col, q);
  float *a = toDevice(h_a, q);

  auto event = q.submit([&](handler &hnd) {
    hnd.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
      for (int k = 1; k < M; k++) {
        for (int p = 0; p < M; p++) {
          matrix[k * M + row[p]] += a[p] * matrix[(k - 1) * M + col[p]];

          // auto tmp = PipelinedLSU::load(sycl::device_ptr<float>(matrix + (k * M + row[p])));
          // tmp += a[p] * PipelinedLSU::load(sycl::device_ptr<float>(matrix + ((k - 1) * M + col[p])));
          // PipelinedLSU::store(sycl::device_ptr<float>(matrix + (k * M + row[p])), tmp);
        }
      }
    });
  });

  event.wait();
  q.memcpy(h_matrix.data(), matrix, sizeof(h_matrix[0]) * h_matrix.size()).wait();

  sycl::free(matrix, q);
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

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<float> &matrix, std::vector<float> &a, std::vector<int> &col_index,
               std::vector<int> &row_ptr, const uint M, const data_distribution distr,
               const uint percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind(distribution, generator);

  for (int r = 0; r < M; ++r) {
    if (distr == data_distribution::ALL_WAIT) {
      col_index[r] = r % 4;
      row_ptr[r] = r % 4;
    } else if (distr == data_distribution::NO_WAIT) {
      col_index[r] = r;
      row_ptr[r] = r;
    } else {
      col_index[r] = (dice() <= percentage) ? col_index[std::max(r - 1, 0)] : r;
      row_ptr[r] = (dice() <= percentage) ? row_ptr[std::max(r - 1, 0)] : r;
    }

    a[r] = 1.0;
  }

  std::fill(matrix.begin(), matrix.end(), 1.0);
}

// Create an exception handler for asynchronous SYCL exceptions
static auto exception_handler = [](sycl::exception_list e_list) {
  for (std::exception_ptr const &e : e_list) {
    try {
      std::rethrow_exception(e);
    } catch (std::exception const &e) {
#if _DEBUG
      std::cout << "Failure" << std::endl;
#endif
      std::terminate();
    }
  }
};

int main(int argc, char *argv[]) {
  // Get A_SIZE and forward/no-forward from args.
  uint M = 64;
  auto DATA_DISTR = data_distribution::ALL_WAIT;
  uint PERCENTAGE = 5;
  try {
    if (argc > 1) {
      M = uint(atoi(argv[1]));
    }
    if (argc > 2) {
      DATA_DISTR = data_distribution(atoi(argv[2]));
    }
    if (argc > 3) {
      PERCENTAGE = uint(atoi(argv[3]));
      std::cout << "Percentage is " << PERCENTAGE << "\n";
      if (PERCENTAGE < 0 || PERCENTAGE > 100) throw std::invalid_argument("Invalid percentage.");
    }
}  catch (exception const &e) {
    std::cout << "Incorrect argv.\nUsage:\n";
    std::cout << "  ./hist [ARRAY_SIZE] [data_distribution (0/1/2)] [PERCENTAGE (only for data_distr 2)]\n";
    std::cout << "    0 - all_wait, 1 - no_wait, 2 - PERCENTAGE wait\n";
    std::terminate();
  }

#if FPGA_EMULATOR
  ext::intel::fpga_emulator_selector d_selector;
#elif FPGA
  ext::intel::fpga_selector d_selector;
#else
  default_selector d_selector;
#endif
  try {
    // Enable profiling.
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: " << q.get_device().get_info<info::device::name>() << "\n";
    
    std::vector<float> matrix(M * M);
    std::vector<float> golden_matrix(M * M);
    std::vector<float> a(M);

    std::vector<int> row_ptr(M);
    std::vector<int> col_index(M);

    init_data(matrix, a, col_index, row_ptr, M, DATA_DISTR, PERCENTAGE);
    std::copy(matrix.begin(), matrix.end(), golden_matrix.begin());
    spmv_cpu(golden_matrix, row_ptr, col_index, a, M);

    auto kernel_time = spmv_kernel(q, matrix, row_ptr, col_index, a, M);

    // Wait for all work to finish.
    q.wait();

    std::cout << "Kernel time (ms): " << kernel_time << "\n";

    if (std::equal(matrix.begin(), matrix.end(), golden_matrix.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cerr << "Failed";
      std::cout << " sum(matrix) = " << std::accumulate(matrix.begin(), matrix.end(), 0.0) << "\n";
      std::cout << " sum(golden_matrix) = " << std::accumulate(golden_matrix.begin(), 
                                                               golden_matrix.end(), 0.0) << "\n";
    }
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

