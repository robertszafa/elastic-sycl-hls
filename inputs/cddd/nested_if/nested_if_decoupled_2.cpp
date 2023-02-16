#include <CL/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <vector>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "memory_utils.hpp"

using namespace sycl;

// Forward declare kernel name.
class MainKernelWhichIsVeryVeryVeryVeryVeryVeryLong;

double nested_if_kernel(queue &q, const std::vector<int> &h_wet,
                        std::vector<float> &h_B) {
  const int array_size = h_wet.size();

  int *wet = fpga_tools::toDevice(h_wet, q);
  float *B = fpga_tools::toDevice(h_B, q);

  using etan_in_1_pipe = pipe<class etan_in_1_pipe_class, float>;
  using wet_in_1_pipe = pipe<class wet_in_1_pipe_class, float>;
  using etan_out_1_pipe = pipe<class etan_out_1_pipe_class, float>;
  using kernel_1_pred_pipe = pipe<class kernel_1_pred_pipe_class, bool>;

  using etan_in_2_pipe = pipe<class etan_in_2_pipe_class, float>;
  using etan_out_2_pipe = pipe<class etan_out_2_pipe_class, float>;
  using kernel_2_pred_pipe = pipe<class kernel_2_pred_pipe_class, bool>;

  auto event = q.submit([&](handler &hnd) {
    hnd.single_task<MainKernelWhichIsVeryVeryVeryVeryVeryVeryLong>([=]() [[intel::kernel_args_restrict]] {
      float etan = 0.0, t = 0.0;
      // II=35
      for (int i = 0; i < array_size; ++i) {
        if (wet[i] > 0) {
          // 35 cycles of stall
          kernel_1_pred_pipe::write(1);
          etan_in_1_pipe::write(etan);
          wet_in_1_pipe::write(wet[i]);
          etan = etan_out_1_pipe::read();
          // t = 0.25 + etan * float(wet[i]) / 2.0;
          // etan += t;
          if (etan > 100.0) {
            // 22 cycles of stall
            kernel_2_pred_pipe::write(1);
            etan_in_2_pipe::write(etan);
            etan = etan_out_2_pipe::read();
            // etan -= 0.1 + etan / 20.0;
            // etan /= 30.0;
          }
        }
      }
      kernel_1_pred_pipe::write(0);
      kernel_2_pred_pipe::write(0);

      B[0] = etan;
    });
  });

  q.submit([&](handler &hnd) {
    hnd.single_task<class Decoupled1>([=]() [[intel::kernel_args_restrict]] {
      while (kernel_1_pred_pipe::read()) {
        auto etan = etan_in_1_pipe::read();
        auto wet = wet_in_1_pipe::read();

        float t = 0.25 + etan * float(wet) / 2.0;
        etan += t;

        etan_out_1_pipe::write(etan);
      }
    });
  });

  q.submit([&](handler &hnd) {
    hnd.single_task<class Decoupled2>([=]() [[intel::kernel_args_restrict]] {
      while (kernel_2_pred_pipe::read()) {
        auto etan = etan_in_2_pipe::read();

        etan -= 0.1 + etan / 20.0;
        etan /= 30.0;

        etan_out_2_pipe::write(etan);
      }
    });
  });

  event.wait();
  q.copy(B, h_B.data(), h_B.size()).wait();

  sycl::free(B, q);
  sycl::free(wet, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void nested_if_cpu(const std::vector<int> &wet, std::vector<float> &B) {
  const int array_size = wet.size();
  float etan, t = 0.0;
  // II=35
  for (int i = 0; i < array_size; ++i) {
    if (wet[i] > 0) {
      // 35 cycles of stall
      t = 0.25 + etan * float(wet[i]) / 2.0;
      etan += t;
      if (etan > 100.0) {
        // 24 cycles of stall
        etan -= 0.1 + etan / 20.0;
        etan /= 30.0;
      }
    }
  }

  B[0] = etan;
}

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<int> &wet, std::vector<float> &B,
               const int array_size) {
  for (int i = 0; i < array_size; ++i) {
    if (i % 2 == 0)
      wet[i] = 1;
    else
      wet[i] = 0;

    B[i] = i;
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
  int ARRAY_SIZE = 1024;
  auto DATA_DISTR = data_distribution::ALL_WAIT;
  int PERCENTAGE = 5;
  try {
    if (argc > 1) {
      ARRAY_SIZE = int(atoi(argv[1]));
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
    std::cout << "  ./executable [ARRAY_SIZE] [data_distribution (0/1/2)] "
                 "[PERCENTAGE (only for "
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
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<int> wet(ARRAY_SIZE);
    std::vector<float> B(ARRAY_SIZE);
    std::vector<float> B_cpu(ARRAY_SIZE);

    init_data(wet, B, ARRAY_SIZE);
    std::copy(B.begin(), B.end(), B_cpu.begin());

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = nested_if_kernel(q, wet, B);

    // Wait for all work to finish.
    q.wait();

    nested_if_cpu(wet, B_cpu);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (std::equal(B.begin(), B.end(), B_cpu.begin())) {
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
