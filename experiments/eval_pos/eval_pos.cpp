#include <sycl/sycl.hpp>
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
class MainKernel;

double eval_pos_kernel(queue &q, const std::vector<int> &h_board,
                       const int color, const int pm, float *h_res) {
  auto *board = fpga_tools::toDevice(h_board, q);
  auto *res = sycl::malloc_device<float>(1, q);

  const int N = h_board.size();

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    float centralityValue = 0.0f;
    int b, benefitMagnitude, moveCount, temp;
    float temp_2;

    for (int i = 0; i < N; i++) {
      b = board[i];
      // Evaluates to -1 or 1, depending on piece/player color
      benefitMagnitude = ((b & 1) ^ color) * -2 + 1;

      temp = (b & 62) >> 1;
      if (temp == 9) {
        float a = 2.5f - i % 6;
        a = (a > 0) ? a : -a;
        float b = 2.5f - i / 6;
        b = (b > 0) ? b : -b;
        float c = (a > b) ? a : b;
        temp_2 = benefitMagnitude * c * pm;
        centralityValue -= temp_2;
      }
    }

    *res = centralityValue;
  });

  event.wait();
  q.copy(res, h_res, 1).wait();

  sycl::free((void *)board, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

void eval_pos_cpu(const std::vector<int> &board, const int color, const int pm,
                  float *res) {
  const int N = board.size();

  float centralityValue = 0.0f;
  int b, benefitMagnitude, moveCount, temp;

  for (int i = 0; i < N; i++) {
    b = board[i];
    // Evaluates to -1 or 1, depending on piece/player color
    benefitMagnitude = ((b & 1) ^ color) * -2 + 1;

    temp = (b & 62) >> 1;
    if (temp == 9) {
      float a = 2.5f - i % 6;
      a = (a > 0) ? a : -a;
      float b = 2.5f - i / 6;
      b = (b > 0) ? b : -b;
      float c = (a > b) ? a : b;
      centralityValue -= benefitMagnitude * c * pm;
    }
  }

  *res = centralityValue;
}

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<float> &a, std::vector<float> &r,
               const int array_size, data_distribution distr,
               const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind(distribution, generator);

  for (int i = 0; i < array_size; ++i) {
    if (distr == data_distribution::ALL_WAIT) {
      a[i] = 0.9f;
    } else if (distr == data_distribution::NO_WAIT) {
      a[i] = 1.1f;
    } else {
      a[i] = (dice() <= percentage) ? 0.9f : 1.1f;
    }

    r[i] = 0;
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
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<int> board(ARRAY_SIZE);
    float res, res_cpu;

    int color = 1;
    int pm = 1;
    for (int i = 0; i < board.size(); i++)
      board[i] = (i % 2 == 1) ? 18 : 1;

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = eval_pos_kernel(q, board, color, pm, &res);

    // Wait for all work to finish.
    q.wait();

    eval_pos_cpu(board, color, pm, &res_cpu);

    std::cout << "\nKernel time (ms): " << kernel_time << "\n";

    if (res == res_cpu) {
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
