#include <CL/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <vector>
#include <random>

#include <sycl/ext/intel/fpga_extensions.hpp>


#include "memory_utils.hpp"

using namespace sycl;
using namespace fpga_tools;

// Forward declare kernel name.
class MainKernel;

double bnn_kernel(queue &q, std::vector<int> &h_addr_in, std::vector<int> &h_addr_out,
                  std::vector<int> &h_in, std::vector<int> &h_data, std::vector<int> &h_mean, 
                  std::vector<int> &h_w, const int N, const int alpha) {

  int *addr_in = fpga_tools::toDevice(h_addr_in, q);
  int *addr_out = fpga_tools::toDevice(h_addr_out, q);
  int *in = fpga_tools::toDevice(h_in, q);
  int *data = fpga_tools::toDevice(h_data, q);
  int *mean = fpga_tools::toDevice(h_mean, q);
  int *w = fpga_tools::toDevice(h_w, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        int x = i * N + j;
        int lut = in[x] ^ w[x];
        data[addr_in[x]] += lut * alpha;
      }

      if (i == (N - 1)) {
        for (int k = 0; k < N; k++) {
          int y = i * N + k;
          int m = mean[y];
          int temp = data[addr_out[y]];
          int z;
          if (temp > 0)
            z = temp - m;
          else
            z = temp + m;
          data[addr_out[y]] = z;
        }
      }
    }
  });

  event.wait();
  q.copy(data, h_data.data(), h_data.size()).wait();

  sycl::free(addr_in, q);
  sycl::free(addr_out, q);
  sycl::free(in, q);
  sycl::free(data, q);
  sycl::free(mean, q);
  sycl::free(w, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void bnn_cpu(std::vector<int> &addr_in, std::vector<int> &addr_out, std::vector<int> &in,
             std::vector<int> &data, std::vector<int> &mean, std::vector<int> &w, int N, int alpha) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      int x = i * N + j;
      int lut = in[x] ^ w[x];
      data[addr_in[x]] += lut * alpha;
    }

    if (i == (N - 1)) {
      for (int k = 0; k < N; k++) {
        int y = i * N + k;
        int m = mean[y];
        int temp = data[addr_out[y]];
        int z;
        if (temp > 0)
          z = temp - m;
        else
          z = temp + m;
        data[addr_out[y]] = z;
      }
    }
  }
}

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<int> &addr_in, std::vector<int> &addr_out, std::vector<int> &in,
               std::vector<int> &data, std::vector<int> &mean, std::vector<int> &w, 
               const data_distribution distr, const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind (distribution, generator);

  int counter=0;
  for (int i = 0; i < addr_in.size(); i++) {
    if (distr == data_distribution::ALL_WAIT) {
      addr_in[i] = std::max(i-2, 0);
      addr_out[i] = i;
    }
    else if (distr == data_distribution::NO_WAIT) {
      addr_in[i] = i;
      addr_out[i] = i;
    }
    else {
      addr_in[i] = (dice() <= percentage) ? std::max(i-2, 0) : i;
    }

    in[i] = 1;
    data[i] = 0;
    mean[i] = 1;
    w[i] = 1;
  }
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
  int N = 10;
  auto DATA_DISTR = data_distribution::ALL_WAIT;
  int PERCENTAGE = 5;
  try {
    if (argc > 1) {
      N = int(atoi(argv[1]));
    }
    if (argc > 2) {
      DATA_DISTR = data_distribution(atoi(argv[2]));
    }
    if (argc > 3) {
      PERCENTAGE = int(atoi(argv[3]));
      std::cout << "Percentage is " << PERCENTAGE << "\n";
      if (PERCENTAGE < 0 || PERCENTAGE > 100)
        throw std::invalid_argument("Invalid percentage.");
    }
  } catch (exception const &e) {
    std::cout << "Incorrect argv.\nUsage:\n";
    std::cout << "  ./executable [ARRAY_SIZE] [data_distribution (0/1/2)] [PERCENTAGE (only for "
                 "data_distr 2)]\n";
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

    std::vector<int> addr_in(N*N) ;
    std::vector<int> addr_out(N*N); 
    std::vector<int> in(N*N);
    std::vector<int> data(N*N);
    std::vector<int> mean(N*N); 
    std::vector<int> w(N*N); 
    const int alpha = 2;

    init_data(addr_in, addr_out, in, data, mean, w, DATA_DISTR, PERCENTAGE);

    std::vector<int> data_cpu(N*N);
    std::copy(data.begin(), data.end(), data_cpu.begin());

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = bnn_kernel(q, addr_in, addr_out, in, data, mean, w, N, alpha);

    // Wait for all work to finish.
    q.wait();

    bnn_cpu(addr_in, addr_out, in, data_cpu, mean, w, N, alpha);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(data.begin(), data.end(), data_cpu.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failed\n";
      std::cout << "sum(fpga) = " << std::accumulate(data.begin(), data.end(), 0.0) << "\n";
      std::cout << "sum(cpu) = " << std::accumulate(data_cpu.begin(), data_cpu.end(), 0.0) << "\n";
    }
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}

