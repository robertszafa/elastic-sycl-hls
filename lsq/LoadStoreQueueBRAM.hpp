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
#ifndef KERNEL_PRAGMAS
#define KERNEL_PRAGMAS [[intel::kernel_args_restrict]] [[intel::max_global_work_dim(0)]]
#endif

using addr_bram_t = int;

constexpr addr_bram_t INVALID_BRAM_ADDR = -1;

template <typename T>
struct tagged_val_lsq_bram_t { T value; uint tag; bool valid; };

struct st_req_lsq_bram_t { addr_bram_t addr; uint tag; };
struct ld_req_lsq_bram_t { addr_bram_t addr; uint tag; uint ld_tag; };


/// Always evaluates to true. Intended for creating artificial dependencies.
template <typename T>
[[clang::optnone]] bool mk_dependency(T dep_src) {
  volatile int tmp = reinterpret_cast<int8_t &>(dep_src);
  return !((tmp << 1 == tmp) && (tmp != 0));
}

template <typename value_t, typename ld_req_pipes, typename ld_val_pipes,
          typename st_req_pipes, typename st_val_pipes, typename end_signal_pipe,
          bool USE_SPECULATION, int ARRAY_SIZE, int NUM_LDS, int NUM_STS, 
          int LD_Q_SIZE = 4, int ST_Q_SIZE = 8>
[[clang::optnone]] event LoadStoreQueueBRAM(queue &q) {
  assert(LD_Q_SIZE >= 2 && "Load queue size must be at least 2.");
  assert(ST_Q_SIZE >= 2 && "Store queue size must be at least 2.");
  
  auto lsqEvent = q.submit([&](handler &hnd) {
    hnd.single_task<class LSQ>([=]() KERNEL_PRAGMAS {
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
      for (int i=0; i<ST_Q_SIZE; ++i) 
        st_alloc_addr[i] = INVALID_BRAM_ADDR;

      // Registers for load logic. 
      // enum LD_STATE { WAIT, SEARCH, RETURN };
      [[intel::fpga_register]] addr_bram_t ld_addr[LD_Q_SIZE];
      [[intel::fpga_register]] uint ld_tag[LD_Q_SIZE];
      [[intel::fpga_register]] bool ld_addr_valid[LD_Q_SIZE];
      [[intel::fpga_register]] bool ld_val_valid[LD_Q_SIZE];
      [[intel::fpga_register]] bool ld_is_safe[LD_Q_SIZE];
      [[intel::fpga_register]] uint ld_port[LD_Q_SIZE]; // used if NUM_LDS>1
      // Only the head of load queue will have values.
      value_t ld_val;
      value_t ld_memory_val;

      // When to stop the LSQ.
      bool end_signal = false;
      
      // Used if NUM_STS > 1. st_reqs and st_vals are muxed based on tag.
      st_req_lsq_bram_t st_req_read[NUM_STS];
      bool st_req_read_succ[NUM_STS];
      uint next_st_req_tag = 1;
      tagged_val_lsq_bram_t<value_t> st_val_read[NUM_STS];
      bool st_val_read_succ[NUM_STS];
      uint next_st_val_tag = 1;

      // Used if NUM_LDS > 1. ld_req is muxed based on the ld_tag. For a ld_req
      // read from port 'k', the later ld_val will also be written to port 'k'.
      ld_req_lsq_bram_t ld_req_read[NUM_LDS];
      bool ld_req_read_succ[NUM_LDS];
      uint next_ld_tag = 0;

      [[intel::ivdep(DATA)]]
      [[intel::initiation_interval(1)]] 
      [[intel::speculated_iterations(0)]]
      while (!end_signal) {
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
          if constexpr (NUM_STS == 1) {
            auto _rd = st_val_pipes::template PipeAt<0>::read(st_val_arrived);
            st_val = _rd.value;
            // Optional support for speculative address allocations. 
            // If enabled, then the store value needs a valid bit to commit.
            if constexpr (USE_SPECULATION) 
              st_val_valid = _rd.valid; 
            else
              st_val_valid = st_val_arrived;
          } else { 
            // Store value mux.
            UnrolledLoop<NUM_STS>([&](auto k) {
              if (!st_val_read_succ[k])
                st_val_read[k] =
                    st_val_pipes::template PipeAt<k>::read(st_val_read_succ[k]);
            });
            UnrolledLoop<NUM_STS>([&](auto k) {
              if (st_val_read_succ[k] && st_val_read[k].tag == next_st_val_tag) {
                st_val_read_succ[k] = false;
                st_val_arrived = true;
                st_val = st_val_read[k].value;
                if constexpr (USE_SPECULATION) 
                  st_val_valid = st_val_read[k].valid;
                else
                  st_val_valid = true;
              }
            });

            if (st_val_arrived) 
              next_st_val_tag++;
          }

          if (st_val_valid) {
            // This is true for any value. Forces the scheduler to exec the 
            // store after the load.
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
          if constexpr (NUM_LDS == 1) {
            ld_val_pipes:: template PipeAt<0>::write(ld_val, pipe_succ);
          } else {
            // Write back to the correct port.
            UnrolledLoop<NUM_LDS>([&](auto k) {
              if (ld_port[0] == k)  
                ld_val_pipes:: template PipeAt<k>::write(ld_val, pipe_succ);
            });
          }

          if (pipe_succ) {
            ld_addr_valid[0] = false;
            ld_val_valid[0] = false;
            ld_port[0] = -1;
          }
        }
        /* End Rule writing the ld_return value to a pipe. */

        /* Rule for reading a load requst from a pipe. */
        if (!ld_addr_valid[LD_Q_SIZE-1]) {
          bool ld_req_valid = false;
          ld_req_lsq_bram_t ld_req;
          uint next_ld_port;

          if constexpr (NUM_LDS == 1) {
            ld_req = ld_req_pipes::template PipeAt<0>::read(ld_req_valid);
          } else { // Ld_req mux. 
            UnrolledLoop<NUM_LDS>([&](auto k) {
              if (!ld_req_read_succ[k])
                ld_req_read[k] =
                    ld_req_pipes::template PipeAt<k>::read(ld_req_read_succ[k]);
            });
            UnrolledLoop<NUM_LDS>([&](auto k) {
              if (ld_req_read_succ[k] && ld_req_read[k].ld_tag == next_ld_tag) {
                ld_req_read_succ[k] = false;
                ld_req = ld_req_read[k];
                ld_req_valid = true;
                // Record where this load request came from.
                next_ld_port = k;
              }
            });
          }

          if (ld_req_valid) {
            ld_addr[LD_Q_SIZE - 1] = ld_req.addr;
            ld_tag[LD_Q_SIZE - 1] = ld_req.tag;
            ld_addr_valid[LD_Q_SIZE - 1] = true;
            ld_port[LD_Q_SIZE - 1] = next_ld_port;
            next_ld_tag++;
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
            for (int i=0; i<ST_Q_SIZE; ++i)
              ld_wait |= match[i];

            // Once a load is marked as safe, it cannot go back to being unsafe.
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
          ld_addr[LD_Q_SIZE-1] = 0;
          ld_tag[LD_Q_SIZE-1] = 0;
          ld_addr_valid[LD_Q_SIZE-1] = false;
          ld_val_valid[LD_Q_SIZE-1] = false;
          ld_is_safe[LD_Q_SIZE-1] = false;
          ld_port[LD_Q_SIZE-1] = 0;
        }
        /* End Rule for load queue shift register shift.*/

        /* Rule for store request pipe read and store_q shift register shift.*/
        if (st_val_arrived || !st_alloc_addr_valid[0]) {
          uint next_tag;
          addr_bram_t next_addr = INVALID_BRAM_ADDR;
          bool st_req_valid = false;
          st_req_lsq_bram_t st_req;

          if constexpr (NUM_STS == 1) {
            st_req = st_req_pipes:: template PipeAt<0>::read(st_req_valid);
          } else { // Store request mux.
            UnrolledLoop<NUM_STS>([&](auto k) {
              if (!st_req_read_succ[k])
                st_req_read[k] =
                    st_req_pipes::template PipeAt<k>::read(st_req_read_succ[k]);
            });
            UnrolledLoop<NUM_STS>([&](auto k) {
              if (st_req_read_succ[k] && st_req_read[k].tag == next_st_req_tag) {
                st_req_read_succ[k] = false;
                st_req = st_req_read[k];
                st_req_valid = true;
              }
            });
          }

          if (st_req_valid) {
            next_addr = st_req.addr;
            next_tag = st_req.tag;
            next_st_req_tag++;
            last_st_req_tag++;
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
    });
  });

  return lsqEvent;
}

#endif // __LOAD_STORE_QUEUE_BRAM_HPP__
