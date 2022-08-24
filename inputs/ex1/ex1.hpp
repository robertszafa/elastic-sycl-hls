#include <CL/sycl.hpp>
// #include <sycl/ext/intel/fpga_extensions.hpp>
#include <vector>


using namespace sycl;

class MyKernelClassName;
class SecondKernel;

void histogram_kernel(queue &q, const std::vector<int> h_idx, std::vector<float> h_hist) {
  const uint array_size = h_idx.size();

  int *idx = device_ptr<int>(malloc_device<int>(h_idx.size(), q));
  q.memcpy(idx, h_idx.data(), sizeof(int)*h_idx.size()).wait();

  float *hist = device_ptr<float>(malloc_device<float>(h_hist.size(), q));
  q.memcpy(hist, h_hist.data(), sizeof(float)*h_hist.size()).wait();

  // q.single_task<SecondKernel>([=]() [[intel::kernel_args_restrict]] {
  //   for (int i = 0; i < 22; ++i) {
  //     idx[i] = 9;
  //   }
  // });

  q.single_task<MyKernelClassName>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < 64; ++i) {
      auto idx_scalar  = idx[i];
      // auto idx_scalar  = i;
      float x = hist[idx_scalar];
      hist[idx_scalar] = x + 3.0;
    }
  }).wait();

  q.memcpy(h_hist.data(), hist, sizeof(h_hist[0])*h_hist.size()).wait();
}

