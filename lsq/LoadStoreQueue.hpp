/*
Robert Szafarczyk, Glasgow, 2022

Memory disambiguation kernel for C/C++/OpenCL/SYCL based HLS.
Store queue with early execution of loads when all preceding stores have calculated their addresses.
*/

#ifndef __LOAD_STORE_QUEUE_HPP__
#define __LOAD_STORE_QUEUE_HPP__

#include <CL/sycl.hpp>

#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/ext/intel/ac_types/ac_int.hpp>

#include "pipe_utils.hpp"
#include "tuple.hpp"
#include "unrolled_loop.hpp"
#include "constexpr_math.hpp"


using namespace sycl;
using namespace fpga_tools;


// The default PipelinedLSU will start a load/store immediately.
using PipelinedLSU = ext::intel::lsu<>;
// TODO: The store latency should be customized for each application 
// (e.g. saturated bandwidth might increase the latency).
constexpr int kLatencyPipelinedLSU = 104; // Arria 10

/// Used for {address, tag} pairs.
struct request_lsq_t { int64_t first; int second; };

template <typename value_t, typename ld_req_pipes, typename ld_val_pipes,
          typename st_req_pipe, typename st_val_pipe, typename end_signal_pipe,
          int num_lds, int QUEUE_SIZE = 8>
event LoadStoreQueue(queue &q) {
  // Setting the II to the number of store_q entries ensures that we are not
  // increasing the critical path of the resulting circuit by too much.
  constexpr int LSQ_II = QUEUE_SIZE;
  // Use minimum number of bits for store_q iterator.
  constexpr int kQueueLoopIterBitSize = BitsForMaxValue<QUEUE_SIZE+1>();
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
      // How many store requests were read.
      int i_store_req = 0;
      // How many store values were read.
      int i_store_val = 0;
      // Total number of stores to commit (supplied by the end_signal).
      int total_req_stores = 0;
      // Pointers into the store_entries buffer. Tail: values, head: addresses.
      storeq_idx_t stq_tail = 0;
      storeq_idx_t stq_head = 0;
      // The tag of the latest read store request. 
      int tag_store = 0;

      // Scalar values for the load logic (one per load, expanded at compile).
      NTuple<value_t, num_lds> val_load_tp;
      NTuple<request_lsq_t, num_lds> req_load_tp;
      NTuple<int64_t, num_lds> address_load_tp;
      NTuple<int, num_lds> tag_load_tp;
      NTuple<bool, num_lds> consumer_load_succ_tp;
      NTuple<bool, num_lds> is_load_waiting_tp;
      NTuple<bool, num_lds> is_load_rq_finished_tp;
      UnrolledLoop<num_lds>([&](auto k) {
        consumer_load_succ_tp. template get<k>() = true;
        tag_load_tp. template get<k>() = 0;
        is_load_waiting_tp. template get<k>() = false;
        is_load_rq_finished_tp. template get<k>() = true;
      });


      // ivdep (ignore mem dependencies): The logic guarantees dependencies are honoured.
      [[intel::initiation_interval(LSQ_II)]] 
      [[intel::ivdep]] 
      while (!end_signal || i_store_val < total_req_stores) {
        /* Start Load Logic */
        // All loads can proceed in parallel. The below unrolls the template PipeArray/NTuple. 
        UnrolledLoop<num_lds>([&](auto k) {
          // Use shorter names.
          auto& val_load = val_load_tp. template get<k>();
          auto& req_load = req_load_tp. template get<k>();
          auto& address_load = address_load_tp. template get<k>();
          auto& tag_load = tag_load_tp. template get<k>();
          auto& consumer_pipe_succ = consumer_load_succ_tp. template get<k>();
          auto& is_load_waiting = is_load_waiting_tp. template get<k>();
          auto& is_load_req_finished = is_load_rq_finished_tp. template get<k>();

          // Check for new ld requests, only once the prev one was completed.
          if (is_load_req_finished) {
            bool req_load_pipe_succ = false;
            req_load = ld_req_pipes:: template PipeAt<k>::read(req_load_pipe_succ);

            if (req_load_pipe_succ) {
              is_load_req_finished = false;
              address_load = req_load.first;
              tag_load = req_load.second;
            }
          }

          if (!is_load_req_finished) {
            // If the load tag sequence has overtaken the store tags, then we cannot possibly
            // disambiguate -- need to wait for more store idxs to arrive. 
            is_load_waiting = (tag_load > tag_store);
            int max_tag = -1;

            #pragma unroll
            for (storeq_idx_t i = 0; i < QUEUE_SIZE; ++i) {
              auto st_entry = store_entries[i];
              if (st_entry.address == address_load && // If found store with same idx as ld,
                  st_entry.tag <= tag_load &&         // make sure the store occured before the ld,
                  st_entry.tag > max_tag) {           // and it is the youngest that did so. 
                is_load_waiting |= st_entry.waiting_for_val;
                val_load = store_entries_val[i];
                max_tag = st_entry.tag;
              }
            }

            // If true, this means that the requested idx is not in the store queue.  
            // Else, val_load would have been assigned in the search loop above.
            if (!is_load_waiting && max_tag == -1) 
              val_load = PipelinedLSU::load(device_ptr<value_t>((value_t*) address_load));
            
            // Setting consumer_load_succ=false forces a write to load consumer pipe.
            consumer_pipe_succ = is_load_waiting;
          }

          if (!consumer_pipe_succ) {
            // The ld. req. is deemed finished once the consumer pipe has been successfully written.
            ld_val_pipes:: template PipeAt<k>::write(val_load, consumer_pipe_succ);
            is_load_req_finished = consumer_pipe_succ;
          }
        }); 
        /* End Load Logic */
      

        /* Start Store Logic */
        bool is_space_in_stq = (store_entries[stq_head].address == 0);
        #pragma unroll
        for (storeq_idx_t i = 0; i < QUEUE_SIZE; ++i) {
          // Invalidate entry if count will go to 0 on this iteration.
          // On every iteration, decrement counter for stores in-flight.
          if (store_entries[i].countdown < int16_t(1) && !store_entries[i].waiting_for_val) 
            store_entries[i].address = 0;
          else 
            store_entries[i].countdown--;
        }

        // If store_q not full, check for new store_idx requests.
        if (is_space_in_stq) {
          bool req_store_pipe_succ = false;
          request_lsq_t req_store = st_req_pipe::read(req_store_pipe_succ);

          if (req_store_pipe_succ) {
            auto address_store = req_store.first;
            tag_store = req_store.second;
            store_entries[stq_head] = {address_store, tag_store, true};
            stq_head = (stq_head+1) % QUEUE_SIZE;
            i_store_req++;
          }
        }

        // Only check for store values, once their corresponding index has been received.
        if (i_store_req > i_store_val) {
          bool val_store_pipe_succ = false;
          value_t val_store = st_val_pipe::read(val_store_pipe_succ);

          if (val_store_pipe_succ) {
            store_entries_val[stq_tail] = val_store;
            store_entries[stq_tail].waiting_for_val = false;
            PipelinedLSU::store(device_ptr<value_t>((value_t*) store_entries[stq_tail].address), 
                                val_store);
            store_entries[stq_tail].countdown = int16_t(kLatencyPipelinedLSU / LSQ_II + 1);
            
            i_store_val++;
            stq_tail = (stq_tail + 1) % QUEUE_SIZE;
          }
        }
        /* End Store Logic */

        // The end signal supplies the total number of stores sent to the store queue.
        if (!end_signal) {
          total_req_stores = end_signal_pipe::read(end_signal);
        }
      }
    });
  });

  return event;
}

#endif
