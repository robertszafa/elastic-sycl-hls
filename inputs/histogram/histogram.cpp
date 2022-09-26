#include <CL/sycl.hpp>
#include <algorithm>
#include <numeric>
#include <sycl/ext/intel/fpga_extensions.hpp>
#include <vector>

#include "memory_utils.hpp"

using namespace sycl;

class MainKernel;
double histogram_kernel(queue &q, const std::vector<int> &h_idx, std::vector<float> &h_hist) {
  const int array_size = h_idx.size();

  int *idx = fpga_tools::toDevice(h_idx, q);
  float *hist = fpga_tools::toDevice(h_hist, q);

  auto event = q.submit([&](handler &hnd) {
    hnd.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
      for (int i = 0; i < array_size; ++i) {
        auto idx_scalar = idx[i];
        float x = hist[idx_scalar];
        hist[idx_scalar] = x + 10.0f;
      }
    });
  });

  event.wait();
  q.copy(hist, h_hist.data(), h_hist.size()).wait();

  sycl::free(idx, q);
  sycl::free(hist, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void histogram_cpu(const int *idx, float *hist, const int N) {
  for (int i = 0; i < N; ++i) {
    auto idx_scalar = idx[i];
    float x = hist[idx_scalar];
    hist[idx_scalar] = x + 10.0f;
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

    int ARRAY_SIZE = 16;

    std::vector<int> feature(ARRAY_SIZE);
    std::vector<float> hist(ARRAY_SIZE);
    std::vector<float> hist_cpu(ARRAY_SIZE);
    std::iota(feature.begin(), feature.end(), 0);
    std::fill(hist.begin(), hist.end(), 0.0);
    std::fill(hist_cpu.begin(), hist_cpu.end(), 0.0);

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = histogram_kernel(q, feature, hist);

    // Wait for all work to finish.
    q.wait();

    histogram_cpu(feature.data(), hist_cpu.data(), hist_cpu.size());

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(hist.begin(), hist.end(), hist_cpu.begin())) {
      std::cout << "Passed\n";
    } else {
      std::cout << "Failse\n";
      std::cout << " sum(hist) = " << std::accumulate(hist.begin(), hist.end(), 0.0) << "\n";
      std::cout << " sum(hist_cpu) = " << std::accumulate(hist_cpu.begin(), hist_cpu.end(), 0.0)
                << "\n";
    }
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}