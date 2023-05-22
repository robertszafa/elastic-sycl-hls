/*
Robert Szafarczyk, Glasgow, 2022

Memory disambiguation kernel for C/C++/OpenCL/SYCL based HLS.
Store queue with early execution of loads when all preceding stores have
calculated their addresses.
*/

#ifndef __LOAD_STORE_QUEUE_BRAM_HPP__
#define __LOAD_STORE_QUEUE_BRAM_HPP__

#include <sycl/sycl.hpp>

#include <sycl/ext/intel/ac_types/ac_int.hpp>
#include <sycl/ext/intel/fpga_extensions.hpp>

#include "constexpr_math.hpp"
#include "device_print.hpp"
#include "pipe_utils.hpp"
#include "tuple.hpp"
#include "unrolled_loop.hpp"

using namespace sycl;
using namespace fpga_tools;

// All our kernels don't have aliasing memory and don't need OpenCL ids.
#define KERNEL_PRAGMAS [[intel::kernel_args_restrict]] [[intel::max_global_work_dim(0)]]

template <typename T>
struct tagged_val_lsq_bram_t { T value; uint tag; };

struct st_req_lsq_bram_t { int addr; uint tag; };
struct ld_req_lsq_bram_t { int addr; uint tag; uint ld_tag; };


/// Always evaluates to true. Intended for creating artificial dependencies.
template <typename T>
[[clang::optnone]] bool mk_dependency(T dep_src) {
  volatile int tmp = reinterpret_cast<int8_t &>(dep_src);
  return !((tmp << 1 == tmp) && (tmp != 0));
}

template <typename value_t, typename ld_req_pipes, typename ld_val_pipes,
          typename st_req_pipes, typename st_val_pipes, typename end_signal_pipe,
          int ARRAY_SIZE, int NUM_LDS, int NUM_STS, int LD_Q_SIZE = 4, int ST_Q_SIZE = 8>
event LoadStoreQueueBRAM(queue &q) {
  assert(LD_Q_SIZE >= 2 && "Load queue size must be at least 2.");
  assert(ST_Q_SIZE >= 2 && "Store queue size must be at least 2.");
  
  constexpr int MUX_BUFFER_SIZE = 32;

  // Multiple stores are muxed when coming into the LSQ.
  using st_req_pipe = pipe<class st_req_pipe_lsq_class, st_req_lsq_bram_t, MUX_BUFFER_SIZE>;
  using st_req_mux_end_signal = pipe<class st_req_mux_end_signal_class, bool>;
  using st_val_pipe = pipe<class st_val_pipe_lsq_class, value_t, MUX_BUFFER_SIZE>;
  using st_val_mux_end_signal = pipe<class st_val_mux_end_signal_class, bool>;
  if constexpr (NUM_STS > 1) {
    q.single_task<class StoreReqMux>([=]() KERNEL_PRAGMAS {
      /// The expected next tag, always increasing by one.
      uint next_tag = 1;
      /// End signal turns true once we read in the actual {num_todo}.
      bool end_signal = false;

      NTuple<st_req_lsq_bram_t, NUM_STS> st_req_tp;
      /// We start with none in pipe reads being successful.
      NTuple<bool, NUM_STS> read_succ_tp;

      [[intel::initiation_interval(1)]] 
      [[intel::speculated_iterations(0)]]
      while (!end_signal) {
        st_req_mux_end_signal::read(end_signal);

        // For each in pipe, try to read. There are 3 cases:
        //   1. Success, and it has the tag we're looking for. Then set
        //   {have_val_in} true.
        //   2. Success, but not the tag we're looking for. {have_val_in} stays
        //   false. {read_succ} stays false until {next_tag} reaches this tag.
        //   3. Fail. Do nothing.
        UnrolledLoop<NUM_STS>([&](auto k) {
          auto& read_succ = read_succ_tp. template get<k>();
          auto& st_req = st_req_tp. template get<k>();
          if (!read_succ) 
            st_req = st_req_pipes:: template PipeAt<k>::read(read_succ);
        });

        bool is_req_valid = false;
        st_req_lsq_bram_t req_to_write;
        UnrolledLoop<NUM_STS>([&](auto k) {
          auto& read_succ = read_succ_tp. template get<k>();
          auto& st_req = st_req_tp. template get<k>();
          // Only one will match.
          if (read_succ && st_req.tag == next_tag) {
            is_req_valid = true;
            read_succ = false;
            req_to_write = st_req;
          }
        });
        
        if (is_req_valid) {
          st_req_pipe::write(req_to_write);
          next_tag++;
        }
      }
    });

    q.single_task<class StoreValMux>([=]() KERNEL_PRAGMAS {
      /// The expected next tag, always increasing by one.
      uint next_tag = 1;
      /// End signal turns true once we read in the actual {num_todo}.
      bool end_signal = false;

      NTuple<tagged_val_lsq_bram_t<value_t>, NUM_STS> st_val_tp;
      /// We start with none in pipe reads being successful.
      NTuple<bool, NUM_STS> read_succ_tp;

      [[intel::initiation_interval(1)]] 
      [[intel::speculated_iterations(0)]]
      while (!end_signal) {
        st_val_mux_end_signal::read(end_signal);

        // For each in pipe, try to read. There are 3 cases:
        //   1. Success, and it has the tag we're looking for. Then set
        //   {have_val_in} true.
        //   2. Success, but not the tag we're looking for. {have_val_in} stays
        //   false. {read_succ} stays false until {next_tag} reaches this tag.
        //   3. Fail. Do nothing.
        UnrolledLoop<NUM_STS>([&](auto k) {
          auto& read_succ = read_succ_tp. template get<k>();
          auto& st_val = st_val_tp. template get<k>();
          if (!read_succ) 
            st_val = st_val_pipes:: template PipeAt<k>::read(read_succ);
        });

        bool is_req_valid = false;
        value_t val_to_write;
        UnrolledLoop<NUM_STS>([&](auto k) {
          auto& read_succ = read_succ_tp. template get<k>();
          auto& st_val = st_val_tp. template get<k>();
          // Only one will match.
          if (read_succ && st_val.tag == next_tag) {
            is_req_valid = true;
            read_succ = false;
            val_to_write = st_val;
          }
        });
        
        if (is_req_valid) {
          st_val_pipe::write(val_to_write);
          next_tag++;
        }
      }
    });
  } // End Muxes for Incoming Stores

  // Multiple loads are also muxed when coming into the LSQ because we have only
  // one load port in BRAM.
  using ld_req_pipe = pipe<class ld_req_pipe_lsq_class, ld_req_lsq_bram_t, MUX_BUFFER_SIZE>;
  using ld_req_mux_end_signal = pipe<class ld_req_mux_end_signal_class, bool>;
  using ld_val_pipe = pipe<class ld_val_pipe_lsq_class, value_t, MUX_BUFFER_SIZE>;
  using ld_val_mux_end_signal = pipe<class ld_val_mux_end_signal_class, bool>;

  using ld_demux_sel_pipe = pipe<class ld_demux_sel_pipe_class, int, MUX_BUFFER_SIZE>;
  using ld_demux_pred_pipe = pipe<class ld_demux_pred_pipe_class, bool, MUX_BUFFER_SIZE>;
  if constexpr (NUM_LDS > 1) {
    q.single_task<class LoadReqMux>([=]() KERNEL_PRAGMAS {
      /// The expected next tag, always increasing by one. Loads start at 0.
      uint next_ld_tag = 0;
      /// End signal turns true once we read in the actual {num_todo}.
      bool end_signal = false;

      NTuple<ld_req_lsq_bram_t, NUM_LDS> ld_req_tp;
      NTuple<bool, NUM_LDS> read_succ_tp;

      [[intel::initiation_interval(1)]] 
      [[intel::speculated_iterations(0)]]
      while (!end_signal) {
        ld_req_mux_end_signal::read(end_signal);

        UnrolledLoop<NUM_LDS>([&](auto k) {
          auto& read_succ = read_succ_tp. template get<k>();
          auto& ld_req = ld_req_tp. template get<k>();
          if (!read_succ) 
            ld_req = ld_req_pipes:: template PipeAt<k>::read(read_succ);
        });

        bool is_req_valid = false;
        ld_req_lsq_bram_t req_to_write;
        int next_port;
        UnrolledLoop<NUM_LDS>([&](auto k) {
          auto& read_succ = read_succ_tp. template get<k>();
          auto& ld_req = ld_req_tp. template get<k>();
          // Only one will match.
          if (read_succ && ld_req.ld_tag == next_ld_tag) {
            is_req_valid = true;
            next_port = k;
            read_succ = false;
            req_to_write = ld_req;
          }
        });
        
        if (is_req_valid) {
          ld_req_pipe::write(req_to_write);
          ld_demux_pred_pipe::write(1);
          ld_demux_sel_pipe::write(next_port);
          next_ld_tag++;
        }
      }
      ld_demux_pred_pipe::write(0);
    });
    
    q.single_task<class LoadValDemux>([=]() KERNEL_PRAGMAS {
      [[intel::initiation_interval(1)]] 
      [[intel::speculated_iterations(0)]]
      while (ld_demux_pred_pipe::read()) {
        auto nextPort = ld_demux_sel_pipe::read();
        auto nextVal = ld_val_pipe::read();

        UnrolledLoop<NUM_LDS>([&](auto k) {
          if (nextPort == k) 
            ld_val_pipes:: template PipeAt<k>::write(nextVal);
        });
      }
    });
  }

  auto lsqEvent = q.submit([&](handler &hnd) {
    hnd.single_task<class LSQ>([=]() KERNEL_PRAGMAS {
      // Dual port memory, capable of 1 rd and 1 wr per cycle.
      [[intel::singlepump]] 
      [[intel::max_replicates(1)]] 
      [[intel::numbanks(1)]] 
      value_t DATA[ARRAY_SIZE];

      // Registers for store logic.
      [[intel::fpga_register]] int st_alloc_addr[ST_Q_SIZE];
      [[intel::fpga_register]] bool st_alloc_addr_valid[ST_Q_SIZE];
      [[intel::fpga_register]] uint st_alloc_tag[ST_Q_SIZE];
      uint last_st_req_tag = 0;

      // Registers for load logic. 
      enum LD_STATE { WAIT, SEARCH, RETURN };
      [[intel::fpga_register]] LD_STATE ld_state[LD_Q_SIZE];
      [[intel::fpga_register]] int ld_addr[LD_Q_SIZE];
      [[intel::fpga_register]] uint ld_tag[LD_Q_SIZE];
      [[intel::fpga_register]] bool ld_got_val[LD_Q_SIZE];
      [[intel::fpga_register]] bool ld_no_conflict[LD_Q_SIZE];
      value_t ld_return_val;
      value_t ld_memory_val;

      // When to stop the LSQ.
      bool end_signal = false;

      [[intel::ivdep]]
      [[intel::initiation_interval(1)]] 
      [[intel::speculated_iterations(0)]]
      while (!end_signal) {
        end_signal_pipe::read(end_signal);

        // Load from memory
        ld_memory_val = DATA[ld_addr[0]];

        // Store to memory (one cycle after load).
        bool st_val_valid = false;
        value_t st_val;
        if (st_alloc_addr_valid[0]) {
          if constexpr (NUM_STS > 1)
            st_val = st_val_pipe::read(st_val_valid);
          else
            st_val = st_val_pipes:: template PipeAt<0>::read(st_val_valid);
          if (st_val_valid) {
            // An artificial dependency between the load and the store to force
            // the compiler to schedule the store after the load.
            if (mk_dependency(ld_memory_val)) // always true
              DATA[st_alloc_addr[0]] = st_val;
          }
        }

        /////////////////////////////////////////////////// Load logic
        value_t bypass_val = st_val;
        bool is_bypass = (st_val_valid && st_alloc_addr[0] == ld_addr[0] &&
                          st_alloc_tag[0] == ld_tag[0]);

        if (ld_state[0] == LD_STATE::SEARCH && (is_bypass || ld_no_conflict[0])) {
          if (!ld_got_val[0]) {
            ld_return_val = is_bypass ? bypass_val : ld_memory_val;
            ld_got_val[0] = true;
          }

          bool pipe_succ = false;
          if constexpr (NUM_LDS > 1) 
            ld_val_pipe::write(ld_return_val, pipe_succ);
          else
            ld_val_pipes:: template PipeAt<0>::write(ld_return_val, pipe_succ);
          if (pipe_succ) 
            ld_state[0] = LD_STATE::WAIT;
        }

        if (ld_state[LD_Q_SIZE-1] == LD_STATE::WAIT) {
          bool ld_req_valid = false;
          ld_req_lsq_bram_t ld_req;
          if constexpr (NUM_LDS > 1)
            ld_req = ld_req_pipe::read(ld_req_valid);
          else
            ld_req = ld_req_pipes::template PipeAt<0>::read(ld_req_valid);
          if (ld_req_valid) {
            ld_addr[LD_Q_SIZE - 1] = ld_req.addr;
            ld_tag[LD_Q_SIZE - 1] = ld_req.tag;
            ld_state[LD_Q_SIZE - 1] = LD_STATE::SEARCH;
          }
          ld_got_val[LD_Q_SIZE-1] = false;
          ld_no_conflict[LD_Q_SIZE-1] = false;
        }

        // Only check first two entries.
        UnrolledLoop<2>([&](auto iLd) {
          if (ld_tag[iLd] <= last_st_req_tag) {
            bool match[ST_Q_SIZE];
            #pragma unroll
            for (int i = 0; i < ST_Q_SIZE; ++i) {
              match[i] = (st_alloc_addr[i] == ld_addr[iLd] && 
                          st_alloc_tag[i] <= ld_tag[iLd]);
            }

            bool ld_wait = false;
            #pragma unroll
            for (int i=0; i<ST_Q_SIZE; ++i)
              ld_wait |= match[i];

            if (!ld_wait) 
              ld_no_conflict[iLd] = true;
          }
        });

        if (ld_state[0] == LD_STATE::WAIT) {
          UnrolledLoop<LD_Q_SIZE - 1>([&](auto i) {
            ld_state[i] = ld_state[i + 1];
            ld_addr[i] = ld_addr[i + 1];
            ld_tag[i] = ld_tag[i + 1];
            ld_no_conflict[i] = ld_no_conflict[i + 1];
            ld_got_val[i] = ld_got_val[i + 1];
          });
          ld_state[LD_Q_SIZE-1] = LD_STATE::WAIT;
          ld_got_val[LD_Q_SIZE-1] = false;
          ld_no_conflict[LD_Q_SIZE-1] = false;
        }
        /////////////////////////////////////////////////// End Load Logic

        /// Store Request Logic and Register Shift
        if (st_val_valid || !st_alloc_addr_valid[0]) {
          bool st_req_valid = false;
          st_req_lsq_bram_t st_req;
          if constexpr (NUM_STS > 1)
            st_req = st_req_pipe::read(st_req_valid);
          else
            st_req = st_req_pipes:: template PipeAt<0>::read(st_req_valid);
          if (st_req_valid) 
            last_st_req_tag++;

          UnrolledLoop<ST_Q_SIZE - 1>([&](auto i) {
            st_alloc_addr[i] = st_alloc_addr[i + 1];
            st_alloc_addr_valid[i] = st_alloc_addr_valid[i + 1];
            st_alloc_tag[i] = st_alloc_tag[i + 1];
          });
          st_alloc_addr_valid[ST_Q_SIZE - 1] = st_req_valid;
          st_alloc_addr[ST_Q_SIZE - 1] = st_req.addr;
          st_alloc_tag[ST_Q_SIZE - 1] = st_req.tag;
        }
        /// End Store Request Logic and Register Shift

      } // End main loop

      if constexpr (NUM_STS > 1) {
        st_req_mux_end_signal::write(0);
        st_val_mux_end_signal::write(0);
      }

      if constexpr (NUM_LDS > 1) 
        ld_req_mux_end_signal::write(0);

    });
  });

  return lsqEvent;
}

#endif // __LOAD_STORE_QUEUE_BRAM_HPP__
