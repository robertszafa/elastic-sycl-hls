#include <CL/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdlib.h>
#include <vector>
#include <random>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "store_queue.hpp"
#include "memory_utils.hpp"

#include "dt_common.h"

using namespace sycl;
using namespace fpga_tools;

using namespace sycl;


double delaunay_triangulation_kernel(queue &q, const std::vector<dt::Point2D<float>> &h_points,
                                     std::vector<dt::Triangle> &h_triangles, const int numPoints) {

  dt::Point2D<float> *points = toDevice(h_points.data(), h_points.size(), q);
  dt::Triangle *triangles = toDevice(h_triangles.data(), h_triangles.size(), q);

  dt::Edge *polygon = sycl::malloc_device<dt::Edge>(h_points.size()*3, q);

  auto event = q.submit([&](handler &hnd) {
    hnd.single_task<class MainKernel>([=]() [[intel::kernel_args_restrict]] {
      int numTriangles = 1;
      for (int iP = 0; iP < numPoints; ++iP) {
        // std::vector<EdgeType> polygon;
        int numEdgesInPolygon = 0;

        for (int iT = 0; iT < numTriangles; ++iT) {
          if (circumCircleContains(triangles[iT], points[iP], points)) {
            triangles[iT].isBad = true;
            polygon[numEdgesInPolygon++] = {triangles[iT].aIdx, triangles[iT].bIdx, false};
            polygon[numEdgesInPolygon++] = {triangles[iT].bIdx, triangles[iT].cIdx, false};
            polygon[numEdgesInPolygon++] = {triangles[iT].cIdx, triangles[iT].aIdx, false};
          }
        }

        // Delete bad triangles.
        int numGoodTriangles = 0;
        for (int iT = 0; iT < numTriangles; ++iT) {
          if (!triangles[iT].isBad)
            triangles[numGoodTriangles++] = triangles[iT];
        }

        // Mark bad polygons.
        for (int iE1 = 0; iE1 < numEdgesInPolygon; ++iE1) {
          for (int iE2 = iE1 + 1; iE2 < numEdgesInPolygon; ++iE2) {
            if (almost_equal(polygon[iE1], polygon[iE2])) {
              polygon[iE1].isBad = true;
              polygon[iE2].isBad = true;
            }
          }
        }

        // Add new triangles.
        for (int iE = 0; iE < numEdgesInPolygon; ++iE) {
          if (!polygon[iE].isBad) {
            triangles[numGoodTriangles++] = {polygon[iE].fromPoint, polygon[iE].toPoint, iP, false};
          }
        }

        numTriangles = numGoodTriangles;
      }

      // Remove triangles containing points from the supertriangle.
      for (int iT = 0; iT < numTriangles; ++iT) {
        if (containsPoint(points, triangles[iT], points[numPoints]) ||
            containsPoint(points, triangles[iT], points[numPoints + 1]) ||
            containsPoint(points, triangles[iT], points[numPoints + 2])) {
          triangles[iT].isBad = true;
        }
      }
    });
  });

  event.wait();
  q.copy(triangles, h_triangles.data(), h_triangles.size()).wait();

  sycl::free(points, q);
  sycl::free(polygon, q);
  sycl::free(triangles, q);

  auto start = event.get_profiling_info<info::event_profiling::command_start>();
  auto end = event.get_profiling_info<info::event_profiling::command_end>();
  double time_in_ms = static_cast<double>(end - start) / 1000000;

  return time_in_ms;
}

enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
template<typename T>
void init_data(std::vector<dt::Point2D<T>> &points, std::vector<dt::Triangle> &triangles,
               const int numPoints, const data_distribution distr, const int percentage) {
  T max = 1000.0, min = 50.0;
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution((int) min, (int) max);
  auto dice = std::bind (distribution, generator);

  // Determinate the super triangle
  T minX = max, minY = max, maxX = min, maxY = min;
  for (int i = 0; i < numPoints; ++i) {
    points[i] = {T(dice()), T(dice())};

    if (points[i].x < minX)
      minX = points[i].x;
    if (points[i].y < minY)
      minY = points[i].y;
    if (points[i].x > maxX)
      maxX = points[i].x;
    if (points[i].y > maxY)
      maxY = points[i].y;
  }

  const T dx = maxX - minX;
  const T dy = maxY - minY;
  const T deltaMax = std::max(dx, dy);
  const T midx = (minX + maxX) / 2;
  const T midy = (minY + maxY) / 2;

  points[numPoints] = {midx - 20 * deltaMax, midy - deltaMax};
  points[numPoints + 1] = {midx, midy + 20 * deltaMax};
  points[numPoints + 2] = {midx + 20 * deltaMax, midy - deltaMax};

  // Supertriangle for the initial condition
  triangles[0] = {numPoints, numPoints + 1, numPoints + 2};
  for (auto &t : triangles)
    t.isBad = false;
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
  int N = 20;
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
  // Create device selector for the device of your interest.
#if FPGA_EMULATOR
  // DPC++ extension: FPGA emulator selector on systems without FPGA card.
  ext::intel::fpga_emulator_selector d_selector;
#elif FPGA
  // DPC++ extension: FPGA selector on systems with FPGA card.
  ext::intel::fpga_selector d_selector;
#elif GPU
  gpu_selector d_selector;
#else
  // The default device selector will select the most performant device.
  default_selector d_selector;
#endif

  try {
    // Enable profiling.
    property_list properties{property::queue::enable_profiling()};
    queue q(d_selector, exception_handler, properties);

    // Print out the device information used for the kernel code.
    std::cout << "Running on device: " << q.get_device().get_info<info::device::name>() << "\n";

    std::vector<dt::Point2D<float>> points(N + 3); // +3 for the supertriangle
    std::vector<dt::Triangle> triangles(N*3 + 1);

    init_data(points, triangles, N, DATA_DISTR, PERCENTAGE);

    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;

    kernel_time = delaunay_triangulation_kernel(q, points, triangles, N);

    // Wait for all work to finish.
    q.wait();
    
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";
    
    int count = 0;
    for (auto &t : triangles)
      count += int(!(t.isBad));
    
    std::cout << "Num good triangles: " << count << "\n";
    
    auto stop = std::chrono::steady_clock::now();
    double total_time = (std::chrono::duration<double> (stop - start)).count() * 1000.0;
    // std::cout << "Total time (ms): " << total_time << "\n";
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }

  return 0;
}
