/*
Robert Szafarczyk, Glasgow, 2022

Memory disambiguation kernel for C/C++/OpenCL/SYCL based HLS.
Store queue with early execution of loads when all preceding stores have
calculated their addresses.
*/

#ifndef __STORE_QUEUE_HPP__
  #define __STORE_QUEUE_HPP__

  #include <CL/sycl.hpp>

  #include <sycl/ext/intel/ac_types/ac_int.hpp>
  #include <sycl/ext/intel/fpga_extensions.hpp>

  #include "constexpr_math.hpp"
  #include "pipe_utils.hpp"
  #include "tuple.hpp"
  #include "unrolled_loop.hpp"

using namespace sycl;
using namespace fpga_tools;

  #ifdef __SYCL_DEVICE_ONLY__
    #define CL_CONSTANT __attribute__((opencl_constant))
  #else
    #define CL_CONSTANT
  #endif
  #define PRINTF(format, ...)                                                  \
    {                                                                          \
      static const CL_CONSTANT char _format[] = format;                        \
      sycl::ext::oneapi::experimental::printf(_format, ##__VA_ARGS__);         \
    }

// The default PipelinedLSU will start a load/store immediately, which the
// memory disambiguation logic relies upon. A BurstCoalescedLSU would instead of
// waiting for more requests to arrive for a coalesced access. (Use
// sycl::ext::intel::experimental::lsu<> if want latency control).
using PipelinedLSU = ext::intel::lsu<>;
constexpr int kLatencyPipelinedLSU = 7;

/// Used for {address, tag} pairs.
struct request_lsq_t {
  int64_t first;
  int second;
};

template <typename value_t, typename ld_idx_pipes, typename ld_val_pipes,
          typename st_idx_pipe, typename st_val_pipe, typename end_signal_pipe,
          int num_lds, int QUEUE_SIZE = 8>
event LoadStoreQueue(queue &q) {
  // Use minimum number of bits for store_q iterator.
  constexpr int kQueueLoopIterBitSize =
      fpga_tools::BitsForMaxValue<QUEUE_SIZE + 1>();
  using storeq_idx_t = ac_int<kQueueLoopIterBitSize, false>;

  struct store_entry {
    int64_t address;
    int tag;
    bool waiting_for_val;
    int16_t countdown;
  };

  auto event = q.submit([&](handler &hnd) {
    hnd.single_task<end_signal_pipe>([=]() [[intel::kernel_args_restrict]] {
      /// The store queue is a circular buffer.
      [[intel::fpga_register]] store_entry store_entries[QUEUE_SIZE];
      [[intel::fpga_register]] value_t store_entries_val[QUEUE_SIZE];

  // Start with no valid entries in store queue.
  #pragma unroll
      for (uint i = 0; i < QUEUE_SIZE; ++i)
        store_entries[i] = {0};

      // The below are variables kept around across iterations.
      bool end_signal = false;
      // How many store (valid) indexes were read from st_idx pipe.
      int i_store_idx = 0;
      // How many store values were accepted from st_val pipe.
      int i_store_val = 0;
      // Total number of stores to commit (supplied by the end_signal).
      int total_req_stores = 0;
      // Pointers into the store_entries circular buffer. Tail is for values,
      // Head for idxs.
      storeq_idx_t stq_tail = 0;
      storeq_idx_t stq_head = 0;
      int tag_store = 0;

      // Scalar book-keeping values for the load logic (one per load, NTuple
      // expanded at compile).
      NTuple<value_t, num_lds> val_load_tp;
      NTuple<request_lsq_t, num_lds> idx_tag_pair_load_tp;
      NTuple<int64_t, num_lds> address_load_tp;
      NTuple<int, num_lds> tag_load_tp;
      NTuple<bool, num_lds> consumer_load_succ_tp;
      NTuple<bool, num_lds> is_load_waiting_tp;
      NTuple<bool, num_lds> is_load_rq_finished_tp;
      UnrolledLoop<num_lds>([&](auto k) {
        consumer_load_succ_tp.template get<k>() = true;
        tag_load_tp.template get<k>() = 0;
        is_load_waiting_tp.template get<k>() = false;
        is_load_rq_finished_tp.template get<k>() = true;
      });

      // Setting Inititation Interval to the number of store_q entries ensures
      // that we are not increasing the critical path of the resulting circuit
      // by too much, or not at all in most cases. ivdep (ignore mem
      // dependencies): The logic guarantees dependencies are honoured.
      [[intel::initiation_interval(II_MAKE)]] [[intel::ivdep]] while (
          !end_signal || i_store_val < total_req_stores) {
        /* Start Load Logic */
        // All loads can proceed in parallel. The below unrolls the template
        // PipeArray/NTuple.
        UnrolledLoop<num_lds>([&](auto k) {
          // Use shorter names.
          auto &val_load = val_load_tp.template get<k>();
          auto &idx_tag_pair_load = idx_tag_pair_load_tp.template get<k>();
          auto &address_load = address_load_tp.template get<k>();
          auto &tag_load = tag_load_tp.template get<k>();
          auto &consumer_pipe_succ = consumer_load_succ_tp.template get<k>();
          auto &is_load_waiting = is_load_waiting_tp.template get<k>();
          auto &is_load_rq_finished = is_load_rq_finished_tp.template get<k>();

          // Check for new ld requests, only once the prev one was completed.
          if (is_load_rq_finished) {
            bool idx_load_pipe_succ = false;
            idx_tag_pair_load =
                ld_idx_pipes::template PipeAt<k>::read(idx_load_pipe_succ);

            if (idx_load_pipe_succ) {
              is_load_rq_finished = false;
              address_load = idx_tag_pair_load.first;
              tag_load = idx_tag_pair_load.second;
            }
          }

          if (!is_load_rq_finished) {
            // If the load tag sequence has overtaken the store tags, then we
            // cannot possibly disambiguate -- need to wait for more store idxs
            // to arrive.
            is_load_waiting = (tag_load > tag_store);
            int max_tag = -1;

  #pragma unroll
            for (storeq_idx_t i = 0; i < QUEUE_SIZE; ++i) {
              auto st_entry = store_entries[i];
              if (st_entry.address ==
                      address_load && // If found store with same idx as ld,
                  st_entry.tag <=
                      tag_load && // make sure the store occured before the ld,
                  st_entry.tag >
                      max_tag) { // and it is the youngest that did so.
                is_load_waiting |= st_entry.waiting_for_val;
                val_load = store_entries_val[i];
                max_tag = st_entry.tag;
              }
            }

            // If true, this means that the requested idx is not in the store
            // queue. Else, val_load would have been assigned in the search loop
            // above.
            if (!is_load_waiting && max_tag == -1)
              val_load = PipelinedLSU::load(
                  device_ptr<value_t>((value_t *)address_load));

            // Setting consumer_load_succ=false forces a write to load consumer
            // pipe.
            consumer_pipe_succ = is_load_waiting;
          }

          if (!consumer_pipe_succ) {
            // The ld. req. is deemed finished once the consumer pipe has been
            // successfully written.
            ld_val_pipes::template PipeAt<k>::write(val_load,
                                                    consumer_pipe_succ);
            is_load_rq_finished = consumer_pipe_succ;
          }
        });
        /* End Load Logic */

        /* Start Store Logic */
        bool is_space_in_stq = (store_entries[stq_head].address == 0);
  #pragma unroll
        for (storeq_idx_t i = 0; i < QUEUE_SIZE; ++i) {
          // Invalidate idx if count WILL GO to 0 on this iteration.
          // On every iteration, decrement counter for stores in-flight.
          if (store_entries[i].countdown < int16_t(1) &&
              !store_entries[i].waiting_for_val)
            store_entries[i].address = 0;
          else
            store_entries[i].countdown--;
        }

        // If store_q not full, check for new store_idx requests.
        if (is_space_in_stq) {
          bool idx_store_pipe_succ = false;
          request_lsq_t idx_tag_pair_store =
              st_idx_pipe::read(idx_store_pipe_succ);

          if (idx_store_pipe_succ) {
            auto address_store = idx_tag_pair_store.first;
            tag_store = idx_tag_pair_store.second;

            store_entries[stq_head] = {address_store, tag_store, true};
            stq_head = (stq_head + 1) % QUEUE_SIZE;
            i_store_idx++;
          }
        }

        // Only check for store values, once their corresponding index has been
        // received.
        if (i_store_idx > i_store_val) {
          bool val_store_pipe_succ = false;
          value_t val_store = st_val_pipe::read(val_store_pipe_succ);

          if (val_store_pipe_succ) {
            store_entries_val[stq_tail] = val_store;
            store_entries[stq_tail].waiting_for_val = false;
            PipelinedLSU::store(
                device_ptr<value_t>((value_t *)store_entries[stq_tail].address),
                val_store);
            store_entries[stq_tail].countdown = int16_t(kLatencyPipelinedLSU);

            i_store_val++;
            stq_tail = (stq_tail + 1) % QUEUE_SIZE;
          }
        }
        /* End Store Logic */

        // The end signal supplies the total number of stores sent to the store
        // queue.
        if (!end_signal)
          total_req_stores = end_signal_pipe::read(end_signal);
      }
    });
  });

  return event;
}

#endif
class MainKernel_AGU_0;
#include "memory_utils.hpp"
#include <CL/sycl.hpp>
#include <algorithm>
#include <iostream>
#include <limits>
#include <math.h>
#include <numeric>
#include <random>
#include <stdlib.h>
#include <sycl/ext/intel/fpga_extensions.hpp>
#include <vector>
using namespace sycl;
// Forward declare kernel name.
class MainKernel;
double histogram_kernel(queue &q, const std::vector<int> &h_idx,
                        std::vector<float> &h_hist) {
  const int array_size = h_idx.size();
  int *idx = fpga_tools::toDevice(h_idx, q);
  float *hist = fpga_tools::toDevice(h_hist, q);
  using pipes_ld_req_0 =
      PipeArray<class pipes_ld_req_0_class, request_lsq_t, 64, 1>;
  using pipe_st_req_0 = pipe<class pipe_st_req_0_class, request_lsq_t, 64>;
  using pipes_ld_val_0 = PipeArray<class pipes_ld_val_0_class, float, 64, 1>;
  using pipe_st_val_0 = pipe<class pipe_st_val_0_class, float, 64>;
  using pipe_end_lsq_signal_0 = pipe<class pipe_end_lsq_signal_0_class, int>;

  q.single_task<MainKernel_AGU_0>([=]() [[intel::kernel_args_restrict]] {
    int tag = 0;
    for (int i = 0; i < array_size; ++i) {
      auto idx_scalar = idx[i];
      pipes_ld_req_0::PipeAt<0>::write({int64_t(hist + idx_scalar), tag});
      tag++;
      pipe_st_req_0::write({int64_t(hist + idx_scalar), tag});
    }

    pipe_end_lsq_signal_0::write(tag);
  });

  auto event = q.single_task<MainKernel>([=]() [[intel::kernel_args_restrict]] {
    for (int i = 0; i < array_size; ++i) {
      auto idx_scalar = idx[i];
      auto x = pipes_ld_val_0::PipeAt<0>::read();
      pipe_st_val_0::write(x + 10.0);
    }
  });

  auto lsqEvent_0 =
      LoadStoreQueue<float, pipes_ld_req_0, pipes_ld_val_0, pipe_st_req_0,
                     pipe_st_val_0, pipe_end_lsq_signal_0, 1, 8>(q);

  lsqEvent_0.wait();
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
    auto x = hist[idx_scalar];
    hist[idx_scalar] = x + 10.0;
  }
}
enum data_distribution { ALL_WAIT, NO_WAIT, PERCENTAGE_WAIT };
void init_data(std::vector<int> &feature, std::vector<float> &hist,
               const data_distribution distr, const int percentage) {
  std::default_random_engine generator;
  std::uniform_int_distribution<int> distribution(0, 100);
  auto dice = std::bind(distribution, generator);
  int counter = 0;
  for (int i = 0; i < feature.size(); i++) {
    if (distr == data_distribution::ALL_WAIT) {
      feature[i] = (feature.size() >= 4) ? i % 4 : 0;
    } else if (distr == data_distribution::NO_WAIT) {
      feature[i] = i;
    } else {
      feature[i] = (dice() <= percentage) ? feature[std::max(i - 1, 0)] : i;
    }
    hist[i] = 0.0;
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
  int ARRAY_SIZE = 64;
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
    std::vector<int> feature(ARRAY_SIZE);
    std::vector<float> hist(ARRAY_SIZE);
    std::vector<float> hist_cpu(ARRAY_SIZE);
    init_data(feature, hist, DATA_DISTR, PERCENTAGE);
    std::copy(hist.begin(), hist.end(), hist_cpu.begin());
    auto start = std::chrono::steady_clock::now();
    double kernel_time = 0;
    kernel_time = histogram_kernel(q, feature, hist);
    // Wait for all work to finish.
    q.wait();
    histogram_cpu(feature.data(), hist_cpu.data(), hist_cpu.size());
    std::cout << "\nKernel time (ms): " << kernel_time << "\n";
  } catch (exception const &e) {
    std::cout << "An exception was caught.\n";
    std::terminate();
  }
  return 0;
}