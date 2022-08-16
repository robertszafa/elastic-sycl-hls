#include "CL/sycl/access/access.hpp"
#include "CL/sycl/builtins.hpp"
#include "CL/sycl/properties/accessor_properties.hpp"
#include <CL/sycl.hpp>
#include <iostream>
#include <vector>

#include <sycl/ext/intel/fpga_extensions.hpp>

using namespace sycl;

class MyKernelClassName;

void histogram_kernel(queue &q, const std::vector<int> &idx, std::vector<int> &hist) {
  const uint array_size = idx.size();

  buffer idx_buf(idx);
  buffer hist_buf(hist);

  q.submit([&](handler &hnd) {
    accessor idx(idx_buf, hnd, read_only);
    accessor hist(hist_buf, hnd, write_only);

    hnd.single_task<MyKernelClassName>([=]() [[intel::kernel_args_restrict]] {
      for (int i = 0; i < array_size; ++i) {
        // int idx_scalar  = idx[i];
        int idx_scalar  = i;
        uint x = hist[idx_scalar];
        hist[idx_scalar] = x + 3;
      }
    });
  });
}
