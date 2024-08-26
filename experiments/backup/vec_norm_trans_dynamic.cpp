class MainKernel_PE_0;
#include "exception_handler.hpp"
#include "memory_utils.hpp"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/sycl.hpp>
#include <vector>
using namespace sycl;
class MainKernel;
double vec_norm_trans_kernel(queue &q, const std::vector<float> &h_a,
                             std::vector<float> &h_r, const int N) {
  float *a = fpga_tools::toDevice(h_a, q);
  float *r = fpga_tools::toDevice(h_r, q);
  using pipe_pe_0_dep_in_0 = pipe<class pipe_pe_0_dep_in_0_class, float>;
  using pipe_pe_0_dep_in_1 = pipe<class pipe_pe_0_dep_in_1_class, float>;
  using pipe_pe_0_dep_out_0 = pipe<class pipe_pe_0_dep_out_0_class, float>;
  using pipe_pe_0_pred = pipe<class pipe_pe_0_pred_class, bool>;
  auto event_MainKernel_PE_0 =
      q.single_task<MainKernel_PE_0>([=]() [[intel::kernel_args_restrict]] {
        float weight = pipe_pe_0_dep_in_0::read();
        while (pipe_pe_0_pred::read()) {
          float d = pipe_pe_0_dep_in_1::read();
          weight = ((d * d + 19.52381f) * d + 3.704762f) * d + 0.73f * weight;
          pipe_pe_0_dep_out_0::write(weight);
        }
      });
  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    float weight = 0.0f;
    pipe_pe_0_dep_in_0::write(weight);
    for (int i = 0; i < N; i++) {
      float d = a[i];
      if (d < 1.0f) {
        pipe_pe_0_pred::write(1);
        pipe_pe_0_dep_in_1::write(d);
        weight = pipe_pe_0_dep_out_0::read();
      }
    }
    pipe_pe_0_pred::write(0);
    for (int i = 0; i < N; i++) {
      float d = a[i] / weight;
      r[i] = r[i] + d;
    }
  });
  event_MainKernel_PE_0.wait();

  event.wait();
  q.copy(r, h_r.data(), h_r.size()).wait();
  sycl::free(a, q);
  sycl::free(r, q);
  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;
  return time_in_ms;
}
void vec_norm_trans_cpu(const std::vector<float> &a, std::vector<float> &r,
                        const int N) {
  float weight = 0.0f;
  for (int i = 0; i < N; i++) {
    float d = a[i];
    if (d < 1.0f)
      weight = ((d * d + 19.52381f) * d + 3.704762f) * d + 0.73f * weight;
  }
  for (int i = 0; i < N; i++) {
    float d = a[i] / weight;
    r[i] = r[i] + d;
  }
}

void init_data(std::vector<float> &a, std::vector<float> &r,
               const int array_size, 
               const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind(distribution, generator);
  for (int i = 0; i < array_size; ++i) {
    if (percentage == 100) {
      a[i] = 0.9f;
    } else if (percentage == 0) {
      a[i] = 1.1f;
    } else {
      a[i] = (dice() <= percentage) ? 0.9f : 1.1f;
    }
    r[i] = 0;
  }
}
template <class T> bool almost_equal(T x, T y) {
  std::ostringstream xSS, ySS;
  xSS << x;
  ySS << y;
  if (x == y || xSS.str() == ySS.str())
    return true;
  return std::fabs(x - y) <=
             std::numeric_limits<T>::epsilon() * std::fabs(x + y) * 2 ||
         std::fabs(x - y) < std::numeric_limits<T>::min();
}
int main(int argc, char *argv[]) {
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
#else
  auto d_selector = sycl::ext::intel::fpga_emulator_selector_v;
#endif
  try {
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);
    std::cout << "Running on device: "
              << q.get_device().get_info<info::device::name>() << "\n";
    std::vector<float> a(ARRAY_SIZE);
    std::vector<float> r(ARRAY_SIZE);
    std::vector<float> r_cpu(ARRAY_SIZE);
    init_data(a, r, ARRAY_SIZE,  PERCENTAGE);
    std::copy(r.begin(), r.end(), r_cpu.begin());
        auto kernel_time = vec_norm_trans_kernel(q, a, r, ARRAY_SIZE);
    vec_norm_trans_cpu(a, r_cpu, ARRAY_SIZE);
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";
    if (std::equal(r.begin(), r.end(), r_cpu.begin(), almost_equal<float>)) {
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