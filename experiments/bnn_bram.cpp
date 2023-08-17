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

using namespace sycl;

// Forward declare kernel name.
class MainKernel;

constexpr int kN = 100;

// Setting TEST will ensure test data is transfered from FPGA DRAM to to BRAM
// and back. This adds latency, so leave unset for the benchmarks.
#define TEST 0

double bnn_kernel(queue &q, const std::vector<int> &h_addr_in,
                   const std::vector<int> &h_addr_out,
                   std::vector<int> &h_data, const int N) {
  int *addr_in = fpga_tools::toDevice(h_addr_in, q);
  int* addr_out = fpga_tools::toDevice(h_addr_out, q);
  int* data_dram = fpga_tools::toDevice(h_data, q);

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int data[kN*kN];

    int w[kN*kN];
    int in[kN*kN];
    int mean[kN*kN];
    int alpha = 2;

    // Ignore for benchmarks.
    #if TEST
    for (int i=0; i<N*N; ++i)
      data[i] = data_dram[i];
    #endif

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        int x = i * N + j;
        int lut = in[x] ^ w[x];
        data[addr_in[x]] += lut * alpha;
      }
    }

    for (int k = 0; k < N; k++) {
      int y = (N-1) * N + k;
      int m = mean[y];
      int z = data[addr_out[y]];
      if (z > 0)
        z = z - m;
      else
        z = z + m;
      data[addr_out[y]] = z;
    }

    #if TEST
    for (int i=0; i<N*N; ++i)
      data_dram[i] = data[i];
    #endif 
  });

  event.wait();
  q.copy(data_dram, h_data.data(), h_data.size()).wait();

  sycl::free(addr_in, q);
  sycl::free(addr_out, q);
  sycl::free(data_dram, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void bnn_cpu(const std::vector<int> &addr_in, const std::vector<int> &addr_out,
             std::vector<int> &data) {
  std::vector<int> w(kN * kN, 0);
  std::vector<int> in(kN * kN, 0);
  std::vector<int> mean(kN * kN, 0);
  int alpha = 2;

  for (int i = 0; i < kN; i++) {
    for (int j = 0; j < kN; j++) {
      int x = i * kN + j;
      int lut = in[x] ^ w[x];
      data[addr_in[x]] += lut * alpha;
    }

    if (i == (kN - 1)) {
      int temp, m, k, y, z;

      for (int k = 0; k < kN; k++) {
        int y = i * kN + k;
        int m = mean[y];
        int temp = data[addr_out[y]];
        if (temp > 0)
          z = temp - m;
        else
          z = temp + m;
        data[addr_out[y]] = z;
      }
    }
  }
}

void init_data(std::vector<int> &h_addr_in, std::vector<int> &h_addr_out,
               std::vector<int> &h_data, const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 99);
  auto dice = std::bind(distribution, generator);

  for (int i = 0; i < h_addr_in.size(); i++) {
    h_addr_in[i] = (dice() < percentage) ? i%6 : i;
    h_addr_out[i] = h_addr_in[i];

    h_data[i] = 1;
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
#else  // #if FPGA_EMULATOR
  auto d_selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif

  try {
    // Enable profiling.
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);

    std::vector<int> addr_in(kN*kN), addr_out(kN*kN), h_data(kN*kN), cpu_data(kN*kN);
    init_data(addr_in, addr_out, h_data, PERCENTAGE);
    std::copy_n(h_data.begin(), kN*kN, cpu_data.begin());

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    auto kernel_time = bnn_kernel(q, addr_in, addr_out, h_data, kN);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    #if TEST
    bnn_cpu(addr_in, addr_out, cpu_data);
    if (std::equal(h_data.begin(), h_data.end(), cpu_data.begin()))
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
