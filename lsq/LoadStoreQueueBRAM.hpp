/*
Memory disambiguation kernel for C/C++/OpenCL/SYCL based HLS.
Store queue with early execution of loads when all preceding stores have
calculated their addresses.
*/

#ifndef __LOAD_STORE_QUEUE_BRAM_HPP__
#define __LOAD_STORE_QUEUE_BRAM_HPP__

#include <sycl/sycl.hpp>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "constexpr_math.hpp"
#include "pipe_utils.hpp"
#include "unrolled_loop.hpp"
#include "tuple.hpp"

#include "device_print.hpp"

using namespace sycl;
using namespace fpga_tools;

// All our kernels don't have aliasing memory and don't need OpenCL ids.
#ifndef KERNEL_PRAGMAS
#define KERNEL_PRAGMAS [[intel::kernel_args_restrict]] [[intel::max_global_work_dim(0)]]
#endif

using addr_bram_t = int;

constexpr addr_bram_t INVALID_BRAM_ADDR = -1;

template <typename T>
struct tagged_val_lsq_bram_t { T value; uint tag; bool valid; };

struct st_req_lsq_bram_t { addr_bram_t addr; uint tag; };
struct ld_req_lsq_bram_t { 
  addr_bram_t addr; uint tag;
  // A load tag is used as a selected in LoadReqInMux. After the mux, the field is
  // changed to the port to which the load value should be returned.
  uint port_ldtag_union;
};

template <typename LSQ_ID>
class LSQ_BRAM;

template <typename LSQ_ID>
class LoadReqInMux;

template <typename LSQ_ID>
class StoreReqInMux;

template <typename LSQ_ID>
class StoreValInMux;

// Applaying [[optnone]] to StreamingMemory doesn't apply the attribute to 
// nested lambdas, so apply the attribute to a range of source code.
#pragma clang attribute push (__attribute__((optnone)), apply_to=function)

/// Always evaluates to true. Intended for creating artificial dependencies.
template <typename T>
[[clang::optnone]] bool mk_dependency(T dep_src) {
  volatile int tmp = reinterpret_cast<int8_t &>(dep_src);
  return !((tmp << 1 == tmp) && (tmp != 0));
}

template <typename value_t, typename ld_req_pipes, typename ld_val_pipes,
          typename st_req_pipes, typename st_val_pipes,
          typename end_signal_pipe, bool USE_SPECULATION, uint ARRAY_SIZE,
          uint NUM_LDS, uint NUM_ST_REQ, uint NUM_ST_VAL, uint LD_Q_SIZE = 4,
          uint ST_Q_SIZE = 8>
[[clang::optnone]] event LoadStoreQueueBRAM(queue &q) {
  assert(LD_Q_SIZE >= 2 && "Load queue size must be at least 2.");
  assert(ST_Q_SIZE >= 2 && "Store queue size must be at least 2.");

  // Beyond 3, the we would need an additional pipeline stage in the LSQ.
  constexpr int MAX_INLINED_ST_VAL_MUX = 3;
  constexpr bool USE_ST_VAL_MUX_KERNEL = NUM_ST_VAL > MAX_INLINED_ST_VAL_MUX;
  constexpr bool USE_ST_VAL_MUX_INLINED = !USE_ST_VAL_MUX_KERNEL && 
                                           NUM_ST_VAL > 1;
  constexpr bool USE_ST_REQ_MUX_KERNEL = NUM_ST_REQ > 1;
  constexpr bool USE_LD_MUX = NUM_LDS > 1;

  // Multiple store requests/values are muxed when coming into the LSQ.
  using st_req_pipe = pipe<class st_req_pipe_mux_, st_req_lsq_bram_t, NUM_ST_REQ>;
  using st_req_mux_end_signal = pipe<class st_req_mux_end_signal_, bool>;
  if constexpr (USE_ST_REQ_MUX_KERNEL) {
    q.single_task<StoreReqInMux<end_signal_pipe>>([=]() KERNEL_PRAGMAS {
      // The expected next tag, always increasing by one.
      uint next_tag = 1;

      NTuple<st_req_lsq_bram_t, NUM_ST_REQ> st_req_tuple;
      NTuple<bool, NUM_ST_REQ> read_succ_tuple;
      UnrolledLoop<NUM_ST_REQ>([&](auto k){
        st_req_tuple.template get<k>() = st_req_lsq_bram_t{0, 0u};
        read_succ_tuple.template get<k>() = false;
      });

      bool end_signal = false;
      [[intel::initiation_interval(1)]] 
      [[intel::speculated_iterations(0)]]
      while (!end_signal) {
        st_req_mux_end_signal::read(end_signal);

        // Choose one, based on tag.
        bool is_req_valid = false;
        st_req_lsq_bram_t req_to_write;
        UnrolledLoop<NUM_ST_REQ>([&](auto k) {
          auto& read_succ = read_succ_tuple. template get<k>();
          auto& st_req = st_req_tuple. template get<k>();
          // Only one will match.
          if (read_succ) { 
            if (st_req.tag == next_tag) {
              is_req_valid = true;
              read_succ = false;
              req_to_write = st_req;
            }
          } else {
            st_req = st_req_pipes:: template PipeAt<k>::read(read_succ);
          }
        });

        if (is_req_valid) {
          st_req_pipe::write(req_to_write);
          next_tag++;
        }
      }
    });
  }

  using st_val_pipe = pipe<class st_val_pipe_mux_, tagged_val_lsq_bram_t<value_t>, NUM_ST_VAL>;
  using st_val_mux_end_signal = pipe<class st_val_mux_end_signal_, bool>;
  if constexpr (USE_ST_VAL_MUX_KERNEL) {
    q.single_task<StoreValInMux<end_signal_pipe>>([=]() KERNEL_PRAGMAS {
      // The expected next tag, always increasing by one.
      uint next_tag = 1;

      NTuple<tagged_val_lsq_bram_t<value_t>, NUM_ST_VAL> st_val_tuple;
      NTuple<bool, NUM_ST_VAL> read_succ_tuple;
      UnrolledLoop<NUM_ST_VAL>([&](auto k) {
        st_val_tuple.template get<k>() = tagged_val_lsq_bram_t<value_t>{0, 0u};
        read_succ_tuple.template get<k>() = false;
      });

      bool end_signal = false;
      [[intel::initiation_interval(1)]] 
      [[intel::speculated_iterations(0)]]
      while (!end_signal) {
        st_val_mux_end_signal::read(end_signal);

        // Choose one, based on tag.
        bool is_val_valid = false;
        tagged_val_lsq_bram_t<value_t> val_to_write;
        UnrolledLoop<NUM_ST_VAL>([&](auto k) {
          auto &read_succ = read_succ_tuple.template get<k>();
          auto &st_val = st_val_tuple.template get<k>();
          // Only one will match.
          if (read_succ) {
            if (st_val.tag == next_tag) {
              is_val_valid = true;
              read_succ = false;
              val_to_write = st_val;
            }
          } else {
            st_val = st_val_pipes::template PipeAt<k>::read(read_succ);
          }
        });

        if (is_val_valid) {
          st_val_pipe::write(val_to_write);
          next_tag++;
        }
      }
    });
  } 

  // Multiple loads are also muxed when coming into the BRAM LSQ.
  using ld_req_pipe = pipe<class ld_req_pipe_mux_, ld_req_lsq_bram_t, NUM_LDS>;
  using ld_req_mux_end_signal = pipe<class ld_req_mux_end_signal_, bool>;
  if constexpr (USE_LD_MUX) {
    q.single_task<LoadReqInMux<end_signal_pipe>>([=]() KERNEL_PRAGMAS {
      // The expected next tag, always increasing by one. Loads start at 0.
      uint next_ld_tag = 0;

      NTuple<ld_req_lsq_bram_t, NUM_LDS> ld_req_tuple;
      NTuple<bool, NUM_LDS> read_succ_tuple;
      UnrolledLoop<NUM_LDS>([&](auto k) {
        read_succ_tuple.template get<k>() = false;
        ld_req_tuple.template get<k>() = ld_req_lsq_bram_t{0, 0u, 0u};
      });

      bool end_signal = false;
      [[intel::initiation_interval(1)]] 
      [[intel::speculated_iterations(0)]]
      while (!end_signal) {
        ld_req_mux_end_signal::read(end_signal);

        // Choose one, based on tag.
        bool is_req_valid = false;
        ld_req_lsq_bram_t req_to_write;
        UnrolledLoop<NUM_LDS>([&](auto k) {
          auto& read_succ = read_succ_tuple. template get<k>();
          auto& ld_req = ld_req_tuple. template get<k>();
          // Only one will match.
          if (read_succ) { 
            if (ld_req.port_ldtag_union == next_ld_tag) {
              is_req_valid = true;
              read_succ = false;
              req_to_write = ld_req;
              // Attach port to this request. ld_tag can be discarded here.
              req_to_write.port_ldtag_union = k;
            }
          } else {
            ld_req = ld_req_pipes:: template PipeAt<k>::read(read_succ);
          }
        });

        if (is_req_valid) {
          ld_req_pipe::write(req_to_write);
          next_ld_tag++;
        }
      }
    });
  }
  
  auto lsqEvent = q.submit([&](handler &hnd) {
    hnd.single_task<LSQ_BRAM<end_signal_pipe>>([=]() KERNEL_PRAGMAS {
      // Dual port memory, capable of 1 rd and 1 wr per cycle.
      [[intel::singlepump]] 
      [[intel::max_replicates(1)]] 
      [[intel::numbanks(1)]] 
      value_t DATA[ARRAY_SIZE];

      // Registers for store logic.
      [[intel::fpga_register]] addr_bram_t st_alloc_addr[ST_Q_SIZE];
      [[intel::fpga_register]] bool st_alloc_addr_valid[ST_Q_SIZE];
      [[intel::fpga_register]] uint st_alloc_tag[ST_Q_SIZE];
      uint last_st_req_tag = 0;
      // Initialize store allocation queue with invalid entries.
      #pragma unroll
      for (int i = 0; i < ST_Q_SIZE; ++i) {
        st_alloc_addr[i] = INVALID_BRAM_ADDR;
        st_alloc_tag[i] = 0u;
        st_alloc_addr_valid[i] = false;
      }

      // Registers for load logic. 
      [[intel::fpga_register]] addr_bram_t ld_addr[LD_Q_SIZE];
      [[intel::fpga_register]] uint ld_tag[LD_Q_SIZE];
      [[intel::fpga_register]] bool ld_addr_valid[LD_Q_SIZE];
      [[intel::fpga_register]] bool ld_val_valid[LD_Q_SIZE];
      [[intel::fpga_register]] bool ld_is_safe[LD_Q_SIZE];
      [[intel::fpga_register]] uint ld_port[LD_Q_SIZE]; // used if NUM_LDS>1
      #pragma unroll
      for (int i = 0; i < LD_Q_SIZE; ++i) {
        ld_addr[i] = 0;
        ld_tag[i] = 0u;
        ld_addr_valid[i] = false;
        ld_val_valid[i] = false;
        ld_is_safe[i] = false;
        ld_port[i] = 0u;
      }

      // Only the head of load queue will have values.
      value_t ld_val;
      value_t ld_memory_val;

      // When to stop the LSQ.
      bool end_signal = false;
      
      // Used if NUM_STS > 1. st_reqs and st_vals are muxed based on tag.
      tagged_val_lsq_bram_t<value_t> st_val_read[NUM_ST_VAL];
      bool st_val_read_succ[NUM_ST_VAL];
      uint next_st_val_tag = 1;
      #pragma unroll
      for (int i = 0; i < NUM_ST_VAL; ++i) st_val_read_succ[i] = false;

      // Used if NUM_LDS > 1. ld_req is muxed based on the ld_tag. For a ld_req
      // read from port 'k', the later ld_val will also be written to port 'k'.
      ld_req_lsq_bram_t ld_req_read[NUM_LDS];
      bool ld_req_read_succ[NUM_LDS];

      [[maybe_unused]] uint cycle = 0; // used for debug
      /// To count misspeculation percentage in benchmarks
      [[maybe_unused]] uint count_all_stores = 0; 
      [[maybe_unused]] uint count_misspec_stores = 0; 

      [[intel::ivdep(DATA)]]
      [[intel::initiation_interval(1)]] 
      [[intel::speculated_iterations(0)]]
      while (!end_signal) { // no stalls in loop, II=1
        // cycle++;

        // Only listen for end signal if store queue is empty.
        if (!st_alloc_addr_valid[0]) 
          end_signal_pipe::read(end_signal);
        
        // Load from memory on every cycle. Value is discarded if not needed.
        // If a load and store happen on the same cycle (possible since this is
        // a pipelined loop), then the load gets the OLD_DATA (before store).
        // This is configured by a parameter in the altera RAM IP.
        ld_memory_val = DATA[ld_addr[0]];

        /* Rule for storing to memory. */
        // Store to memory if a value for a valid allocation arrives.
        // The store is scheduled one cycle after the load (achieved by
        // introducing an artificial constrol dependency between the loaded
        // value and the store to force the scheduler.)
        bool st_val_arrived = false, st_val_valid = false;
        value_t st_val;
        if (st_alloc_addr_valid[0]) {
          if constexpr (USE_ST_VAL_MUX_INLINED) {
            // Store value mux based on tag.
            UnrolledLoop<NUM_ST_VAL>([&](auto k) {
              if (st_val_read_succ[k]) {
                if (st_val_read[k].tag == next_st_val_tag) {
                  st_val_read_succ[k] = false;
                  st_val_arrived = true;
                  st_val = st_val_read[k].value;
                  if constexpr (USE_SPECULATION) 
                    st_val_valid = st_val_read[k].valid;
                  else
                    st_val_valid = true;
                }
              } else {
                st_val_read[k] =
                    st_val_pipes::template PipeAt<k>::read(st_val_read_succ[k]);
              }
            });

            if (st_val_arrived) 
              next_st_val_tag++;
          } else {
            tagged_val_lsq_bram_t<value_t> _rd;
            if constexpr (USE_ST_VAL_MUX_KERNEL) 
              _rd = st_val_pipe::read(st_val_arrived);
            else
              _rd = st_val_pipes::template PipeAt<0>::read(st_val_arrived);

            st_val = _rd.value;
            if constexpr (USE_SPECULATION)
              st_val_valid = _rd.valid;
            else
              st_val_valid = true;
          }

          // Optional support for speculative address allocations. 
          // If enabled, then the store value needs a valid bit to commit.
          if constexpr (USE_SPECULATION) 
            st_val_valid = st_val_arrived && st_val_valid; 
          else
            st_val_valid = st_val_arrived;

          if (st_val_arrived) {
            count_all_stores++;
            if (!st_val_valid) {
              count_misspec_stores++;
            }
          }

          if (st_val_valid) {
            // Force the scheduler to exec the store one cycle after the load.
            if (mk_dependency(ld_memory_val)) 
              DATA[st_alloc_addr[0]] = st_val;
          } 
        }
        /* End Rule for storing to memory. */

        /* Rule for returning load value to client. */
        value_t bypass_val = st_val;
        // We only need a bypass for the st value that arrives on this cycle.
        // Any earlier st value would have already been commited to memory.
        bool is_bypass = (st_val_valid && st_alloc_addr[0] == ld_addr[0] &&
                          st_alloc_tag[0] == ld_tag[0]);
        if (ld_addr_valid[0] && (is_bypass || ld_is_safe[0])) {
          if (!ld_val_valid[0]) {
            ld_val = is_bypass ? bypass_val : ld_memory_val;
            ld_val_valid[0] = true;
          }

          bool pipe_succ = false;
          if constexpr (USE_LD_MUX) {
            // Write back to the correct port.
            UnrolledLoop<NUM_LDS>([&](auto k) {
              if (ld_port[0] == k)  
                ld_val_pipes:: template PipeAt<k>::write(ld_val, pipe_succ);
            });
          } else {
            ld_val_pipes:: template PipeAt<0>::write(ld_val, pipe_succ);
          }

          if (pipe_succ) {
            ld_addr_valid[0] = false;
            ld_val_valid[0] = false;
            ld_port[0] = NUM_LDS + 1;
          }
        }
        /* End Rule writing the ld_return value to a pipe. */

        /* Rule for reading a load requst from a pipe. */
        if (!ld_addr_valid[LD_Q_SIZE-1]) {
          bool ld_req_valid = false;
          ld_req_lsq_bram_t ld_req;
          if constexpr (USE_LD_MUX)
            ld_req = ld_req_pipe::read(ld_req_valid);
          else
            ld_req = ld_req_pipes::template PipeAt<0>::read(ld_req_valid);

          if (ld_req_valid) {
            ld_addr[LD_Q_SIZE - 1] = ld_req.addr;
            ld_tag[LD_Q_SIZE - 1] = ld_req.tag;
            ld_addr_valid[LD_Q_SIZE - 1] = true;
            ld_port[LD_Q_SIZE - 1] = ld_req.port_ldtag_union;
          }
        }
        /* End Rule for reading a load requst from a pipe. */

        /* Rule for setting ld_is_safe flag (only first two entries).*/
        UnrolledLoop<2>([&](auto iLd) {
          // Only if ld_addr is valid, and if previous store addresses arrived.
          if (ld_addr_valid[iLd] && ld_tag[iLd] <= last_st_req_tag) {
            bool match[ST_Q_SIZE];
            #pragma unroll
            for (int i = 0; i < ST_Q_SIZE; ++i) {
              match[i] = (st_alloc_addr[i] == ld_addr[iLd] && 
                          st_alloc_tag[i] <= ld_tag[iLd]);
            }

            bool ld_wait = false;
            #pragma unroll
            for (int i = 0; i < ST_Q_SIZE; ++i)
              ld_wait |= match[i];

            // Once a load is marked as safe, it cannot go back to unsafe.
            if (!ld_wait) 
              ld_is_safe[iLd] = true;
          }
        });
        /* End Rule for setting ld_is_safe flag.*/

        // /* Rule for load queue shift.*/
        if (!ld_addr_valid[0]) {
          UnrolledLoop<LD_Q_SIZE - 1>([&](auto i) {
            ld_addr[i] = ld_addr[i + 1];
            ld_tag[i] = ld_tag[i + 1];
            ld_addr_valid[i] = ld_addr_valid[i + 1];
            ld_val_valid[i] = ld_val_valid[i + 1];
            ld_is_safe[i] = ld_is_safe[i + 1];
            ld_port[i] = ld_port[i + 1];
          });
          ld_addr[LD_Q_SIZE - 1] = 0;
          ld_tag[LD_Q_SIZE - 1] = 0u;
          ld_addr_valid[LD_Q_SIZE - 1] = false;
          ld_val_valid[LD_Q_SIZE - 1] = false;
          ld_is_safe[LD_Q_SIZE - 1] = false;
          ld_port[LD_Q_SIZE - 1] = NUM_LDS + 1;
        }
        /* End Rule for load queue shift register shift.*/

        /* Rule for store request pipe read and store_q shift register shift.*/
        if (st_val_arrived || !st_alloc_addr_valid[0]) {
          uint next_tag;
          addr_bram_t next_addr = INVALID_BRAM_ADDR;
          bool st_req_valid = false;
          st_req_lsq_bram_t st_req;

          if constexpr (USE_ST_REQ_MUX_KERNEL)
            st_req = st_req_pipe::read(st_req_valid);
          else
            st_req = st_req_pipes:: template PipeAt<0>::read(st_req_valid);

          if (st_req_valid) {
            next_addr = st_req.addr;
            next_tag = st_req.tag;
            last_st_req_tag = st_req.tag;
          }

          #pragma unroll
          for (int i = 0; i < ST_Q_SIZE-1; ++i) {
            st_alloc_addr[i] = st_alloc_addr[i + 1];
            st_alloc_addr_valid[i] = st_alloc_addr_valid[i + 1];
            st_alloc_tag[i] = st_alloc_tag[i + 1];
          }
          st_alloc_addr[ST_Q_SIZE - 1] = next_addr;
          st_alloc_addr_valid[ST_Q_SIZE - 1] = st_req_valid;
          st_alloc_tag[ST_Q_SIZE - 1] = next_tag;
        }
        /* End Rule for store request pipe read and store_q shift register.*/

      } // End main loop
    
      // Terminate muxes.
      if constexpr (USE_ST_REQ_MUX_KERNEL) 
        st_req_mux_end_signal::write(0);
      if constexpr (USE_ST_VAL_MUX_KERNEL) 
        st_val_mux_end_signal::write(0);
      if constexpr (USE_LD_MUX) 
        ld_req_mux_end_signal::write(0);
      
      /** To count misspeculation percentage in benchmarks: */
      // count_all_stores -= 1000; # subtract num memory transfer stores. 
      // PRINTF("\n==========\n%d / %d = %f\n==========\n", 
      //        count_misspec_stores, count_all_stores, 
      //        float(count_misspec_stores)/float(count_all_stores));
      
    });
  });

  return lsqEvent;
}

#pragma clang attribute pop

#endif // __LOAD_STORE_QUEUE_BRAM_HPP__
