/*
Robert Szafarczyk, Glasgow, 2023

A simple N-to-1 Mux template.
*/

#ifndef __MULTIPLEXOR_LSQ_HPP__
#define __MULTIPLEXOR_LSQ_HPP__

#include <CL/sycl.hpp>
#include <stdlib.h>
#include <type_traits>
#include <sycl/ext/intel/fpga_extensions.hpp>

#include "pipe_utils.hpp"
#include "tuple.hpp"
#include "unrolled_loop.hpp"

// To get the lsq_request_t type.
#include "LoadStoreQueue.hpp"

using namespace sycl;
using namespace fpga_tools;

/// {value, tag} pair (analogous to the lsq_request_t {address, tag} pair).
template <typename T>
struct tagged_val_t { T first; int second; };

/// Each {pipe_in} non-blocking read gives a {first, second} pair, where second
/// is the tag. The tags start at 1 and increase with a step of 1. Using the
/// second member of each pipe_in, write out the incoming values in-tag-order to
/// {pipe_out}. Stop the kernel once the final tag read in from the
/// {end_signal_pipe} is reached.
template <typename pipes_in, typename pipe_out, typename end_signal_pipe,
          typename pipe_type, int mux_width, int II>
event Multiplexor(queue &q) {
  auto event = q.single_task<pipe_out>([=]() [[intel::kernel_args_restrict]] {
    /// The expected next tag, always increasing by one.
    int next_tag = 1;
    /// How many out writes have been done.
    int num_done = 0;
    /// How many out writes we need to do altogether.
    int num_todo = INT_MAX;
    /// End signal turns true once we read in the actual {num_todo}.
    bool end_signal = false;

    NTuple<pipe_type, mux_width> in_val_tp;
    /// We start with none in pipe reads being successful.
    NTuple<bool, mux_width> read_succ_tp;
    UnrolledLoop<mux_width>([&](auto k) {
      read_succ_tp. template get<k>() = false;
    });

    [[intel::initiation_interval(II)]] 
    while (!end_signal || num_done < num_todo) {
      bool have_val_in = false;
      pipe_type val_to_write;

      // For each in pipe, try to read. There are 3 cases:
      //   1. Success, and it has the tag we're looking for. Then set
      //   {have_val_in} true.
      //   2. Success, but not the tag we're looking for. {have_val_in} stays
      //   false. {read_succ} stays false until {next_tag} reaches this tag.
      //   3. Fail. Do nothing.
      UnrolledLoop<mux_width>([&](auto k) {
        auto& read_succ = read_succ_tp. template get<k>();
        auto& in_val = in_val_tp. template get<k>();

        if (!read_succ) 
          in_val = pipes_in:: template PipeAt<k>::read(read_succ);

        if (!have_val_in && read_succ && in_val.second == next_tag) {
          have_val_in = true;
          read_succ = false;
          val_to_write = in_val;
          next_tag++;
        }
      });

      if (!end_signal)
        num_todo = end_signal_pipe::read(end_signal);
      
      if (have_val_in) {
        // We can discard the tag for a store value, not for an LSQ request. 
        if constexpr (std::is_same_v<pipe_type, request_lsq_t>) 
          pipe_out::write(val_to_write);
        else 
          pipe_out::write(val_to_write.first);

        num_done++;
      }
    }
  });

  return event;
}

#endif