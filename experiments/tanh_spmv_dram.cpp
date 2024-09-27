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

double tanh_spmv_kernel(queue &q, std::vector<float> &h_matrix,
                        std::vector<float> &h_a, std::vector<float> &h_y,
                        const std::vector<int> &h_row,
                        const std::vector<int> &h_col, const uint nz) {

  const uint N = h_a.size();
  const uint t = 2;

  float *matrix = fpga_tools::toDevice(h_matrix, q);
  float *a = fpga_tools::toDevice(h_a, q);
  float *y = fpga_tools::toDevice(h_y, q);
  int *row = fpga_tools::toDevice(h_row, q);
  int *col = fpga_tools::toDevice(h_col, q);

  int *test = fpga_tools::toDevice(h_col, q);

  std::vector<std::pair<sycl::event, bool>> events; // {event, measureTimeBool} pairs

  auto main_event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (uint i = 0; i < N; i++) {
      float beta = a[i];
      if (beta <= 1.0f) // saturation
        a[i] = ((beta * beta + 19.5f) * beta * beta + 3.7f) * beta;
    }

    // spmv using COO
    for (uint i = 0; i < nz ; i++) {
      y[row[i]] += a[row[i]] * matrix[col[i]];
    }  

    for (uint i = 0; i < t ; i++)
      test[i] = y[i];
  });

    
  events.push_back({main_event, true});
  for (auto &kv : events) kv.first.wait();
  q.copy(y, h_y.data(), h_y.size()).wait();

  sycl::free(matrix, q);
  sycl::free(row, q);
  sycl::free(col, q);
  sycl::free(a, q);
  sycl::free(y, q);

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

void tanh_spmv_cpu(std::vector<float> &matrix, std::vector<float> &a,
                   std::vector<float> &y, const std::vector<int> &row,
                   const std::vector<int> &col, const int nz) {
  const uint N = a.size();
  for (uint i = 0; i < N; i++) {
    float beta = a[i];
    if (beta <= 1.0f) // saturation
      a[i] = ((beta * beta + 19.5f) * beta * beta + 3.7f) * beta;
  }

  // spmv using COO
  for (uint i = 0; i < nz ; i++) {
    y[row[i]] += a[row[i]] * matrix[col[i]];
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
  int N = 1000;
  float PERCENTAGE_DENSE = 0.2f;
  try {
    if (argc > 1) {
      N = int(atoi(argv[1]));
    }
    if (argc > 2) {
      PERCENTAGE_DENSE = float(atof(argv[2]));
      if (PERCENTAGE_DENSE < 0.0f || PERCENTAGE_DENSE > 1.0f)
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
#else  // #if FPGA_EMULATOR
  auto d_selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif

  try {
    // Enable profiling.
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);
    auto device = q.get_device();

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << device.get_info<sycl::info::device::name>().c_str() << "\n";

    std::vector<float> arr(N, 1);
    std::vector<float> arr_cpu(N, 1);
    std::vector<float> y(N, 0);
    std::vector<float> y_cpu(N, 0);
    std::vector<float> matrix(N*N, 1);
    std::vector<float> matrix_cpu(N*N, 1);
    
    // const int NZ = N*N;
    const int NZ = int(double(N * N) * double(PERCENTAGE_DENSE));
    std::vector<int> row(NZ);
    std::vector<int> col(NZ);
    for (size_t i = 0; i < NZ; ++i) {
      row[i] = i % N;
      col[i] = i % (N*N);
    }

    std::sort(row.begin(), row.end());

    auto kernel_time = tanh_spmv_kernel(q, matrix, arr, y, row, col, NZ);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    tanh_spmv_cpu(matrix_cpu, arr_cpu, y_cpu, row, col, NZ);

    if (std::equal(y.begin(), y.end(), y_cpu.begin())) {
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
