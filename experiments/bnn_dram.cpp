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

double bnn_kernel(queue &q, const std::vector<int> &h_addr_in,
                  const std::vector<int> &h_addr_out,
                  const std::vector<int> &h_w, const std::vector<int> &h_in,
                  const std::vector<int> &h_mean, std::vector<int> &h_data,
                  const int N) {
  int* data = fpga_tools::toDevice(h_data, q);
  int *addr_in = fpga_tools::toDevice(h_addr_in, q);
  int* addr_out = fpga_tools::toDevice(h_addr_out, q);
  int* w = fpga_tools::toDevice(h_w, q);
  int* in = fpga_tools::toDevice(h_in, q);
  int* mean = fpga_tools::toDevice(h_mean, q);

  std::vector<std::pair<sycl::event, bool>> events; // {event, measureTimeBool} pairs

  auto main_event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    int alpha = 2;

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

  });

  
  events.push_back({main_event, true});
  for (auto &kv : events) kv.first.wait();
  q.copy(data, h_data.data(), h_data.size()).wait();

  sycl::free(addr_in, q);
  sycl::free(addr_out, q);
  sycl::free(data, q);
  sycl::free(w, q);
  sycl::free(in, q);
  sycl::free(mean, q);

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

void bnn_cpu(const std::vector<int> &addr_in, const std::vector<int> &addr_out,
             const std::vector<int> &w, const std::vector<int> &in,
             const std::vector<int> &mean, std::vector<int> &data,
             const int N) {
  int alpha = 2;

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

void init_data(std::vector<int> &addr_in, std::vector<int> &addr_out,
               std::vector<int> &w, std::vector<int> &in,
               std::vector<int> &mean, std::vector<int> &data,
               const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 99);
  auto dice = std::bind(distribution, generator);

  for (int i = 0; i < addr_in.size(); i++) {
    addr_in[i] = (dice() < percentage) ? 1 : i;
    addr_out[i] = addr_in[i];
    data[i] = 1;
    w[i] = 1;
    in[i] = 1;
    mean[i] = 1;
  }
}

int main(int argc, char *argv[]) {
  int N = 100;
  int PERCENTAGE = 0;
  try {
    if (argc > 1) {
      N = int(atoi(argv[1]));
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

    std::vector<int> addr_in(N*N), addr_out(N*N), h_data(N*N), cpu_data(N*N), w(N*N), in(N*N), mean(N*N);
    init_data(addr_in, addr_out, w, in, mean, h_data, PERCENTAGE);
    std::copy_n(h_data.begin(), N*N, cpu_data.begin());

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    auto kernel_time = bnn_kernel(q, addr_in, addr_out, w, in, mean, h_data, N);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    bnn_cpu(addr_in, addr_out, w, in, mean, cpu_data, N);
    if (std::equal(h_data.begin(), h_data.end(), cpu_data.begin()))
      std::cout << "Passed\n";
    else
      std::cout << "Failed\n";
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
