/*
Robert Szafarczyk, Glasgow, 2022

Load Store Queue kernel for SYCL HLS. The LSQ is for off-chip memory.
*/

#ifndef __LOAD_STORE_QUEUE_DRAM_HPP__
#define __LOAD_STORE_QUEUE_DRAM_HPP__

#include <sycl/sycl.hpp>

#include <sycl/ext/intel/fpga_extensions.hpp>
#include <sycl/ext/intel/ac_types/ac_int.hpp>

#include "pipe_utils.hpp"
#include "tuple.hpp"
#include "unrolled_loop.hpp"
#include "constexpr_math.hpp"
#include "device_print.hpp"

using namespace sycl;
using namespace fpga_tools;

// All our kernels don't have aliasing memory and don't need OpenCL ids.
#define KERNEL_PRAGMAS [[intel::kernel_args_restrict]] [[intel::max_global_work_dim(0)]] 

// Use the simplest load/store units for DRAM (no bursts, no caches, no prefetching).
using PipelinedLSU = sycl::ext::intel::lsu<>;

/// Max number of cycles between issuing store to DRAM memory controller and commit.
constexpr int STORE_LATENCY_DRAM = 8; 

/// Represents pointer bits.
using addr_dram_t = int64_t;
constexpr addr_dram_t INVALID_ADDRESS = -1; 

struct req_lsq_dram_t { addr_dram_t addr; uint tag; };

template <typename T>
struct addr_dram_val_pair_t { addr_dram_t addr; T value; };

template <typename T>
struct tagged_val_lsq_dram_t { T value; uint tag; };

// To generate unique kernel names.
template <int id>
class LoadPort;
template <int id>
class LoadMux;


template <typename value_t, typename ld_req_pipes, typename ld_val_pipes,
          typename st_req_pipes, typename st_val_pipes, typename end_signal_pipe,
          int NUM_LDS, int NUM_STS, int LD_Q_SIZE = 4, int ST_Q_SIZE = 8>
[[clang::optnone]] event LoadStoreQueueDRAM(queue &q) {
  assert(LD_Q_SIZE >= 2 && "Load queue size must be at least 2.");
  assert(ST_Q_SIZE >= 2 && "Store queue size must be at least 2.");

  // Pipes to connect LSQ to ld/st ports and to ld value mux.
  using pred_st_port_pipe = pipe<class pred_st_port_pipe_class, bool, STORE_LATENCY_DRAM>;
  using st_port_val_pipe = pipe<class st_port_val_pipe_class, addr_dram_val_pair_t<value_t>, STORE_LATENCY_DRAM>;

  // Each load gets its own load port and load mux, thus the use of PipeArrays.
  using pred_ld_port_pipes = PipeArray<class pred_ld_port_pipe_class, bool, STORE_LATENCY_DRAM, NUM_LDS>;
  using ld_port_addr_pipes = PipeArray<class ld_port_addr_pipe_class, addr_dram_t, STORE_LATENCY_DRAM, NUM_LDS>;
  using pred_ld_mux_pipes = PipeArray<class pred_ld_mux_pipe_class, bool, STORE_LATENCY_DRAM, NUM_LDS>;
  using ld_mux_sel_pipes = PipeArray<class ld_mux_sel_pipe_class, bool, STORE_LATENCY_DRAM, NUM_LDS>;
  using ld_mux_from_memory_val_pipes = PipeArray<class ld_mux_from_memory_val_class, value_t, STORE_LATENCY_DRAM, NUM_LDS>;
  using ld_mux_from_bypass_val_pipes = PipeArray<class ld_mux_from_bypass_val_class, value_t, STORE_LATENCY_DRAM, NUM_LDS>;

  /// Store port kernel
  auto storePortEvent = q.submit([&](handler &hnd) {
    hnd.single_task<class StorePort>([=]()  {
      [[intel::speculated_iterations(0)]]
      while (pred_st_port_pipe::read()) {
        addr_dram_val_pair_t<value_t> addr_val = st_port_val_pipe::read();
        auto st_addr_dram_typed =
            sycl::ext::intel::device_ptr<value_t>((value_t *)addr_val.addr);
        PipelinedLSU::store(st_addr_dram_typed, addr_val.value);
      }
    });
  });

  /// Compile time unroll of load port and load mux kernels.
  UnrolledLoop<NUM_LDS>([&](auto iLd) {
    q.submit([&](handler &hnd) {
      hnd.single_task<LoadPort<iLd>>([=]() KERNEL_PRAGMAS {
        [[intel::speculated_iterations(0)]]
        while (pred_ld_port_pipes:: template PipeAt<iLd>::read()) {
          addr_dram_t ld_addr = ld_port_addr_pipes:: template PipeAt<iLd>::read();
          auto ld_addr_dram_typed =
              sycl::ext::intel::device_ptr<value_t>((value_t *)ld_addr);
          value_t ld_val = PipelinedLSU::load(ld_addr_dram_typed);
          ld_mux_from_memory_val_pipes:: template PipeAt<iLd>::write(ld_val);
        }
      });
    });

    q.submit([&](handler &hnd) {
      hnd.single_task<LoadMux<iLd>>([=]() KERNEL_PRAGMAS {
        [[intel::speculated_iterations(0)]]
        while (pred_ld_mux_pipes:: template PipeAt<iLd>::read()) {
          value_t val;
          if (ld_mux_sel_pipes:: template PipeAt<iLd>::read()) 
            val = ld_mux_from_bypass_val_pipes:: template PipeAt<iLd>::read();
          else
            val = ld_mux_from_memory_val_pipes:: template PipeAt<iLd>::read();
          
          ld_val_pipes:: template PipeAt<iLd>::write(val);
        }
      });
    });
  }); // End load port and load mux kernels

  /// Load store queue kernel.
  auto lsqEvent = q.submit([&](handler &hnd) {
    hnd.single_task<class LSQ>([=]() KERNEL_PRAGMAS {
      // Registers for store logic.
      [[intel::fpga_register]] addr_dram_t st_alloc_addr[ST_Q_SIZE];
      [[intel::fpga_register]] bool st_alloc_addr_valid[ST_Q_SIZE];
      [[intel::fpga_register]] uint st_alloc_tag[ST_Q_SIZE];
      [[intel::fpga_register]] addr_dram_t st_commit_addr[STORE_LATENCY_DRAM];
      [[intel::fpga_register]] value_t st_commit_value[STORE_LATENCY_DRAM];
      uint last_st_req_tag = 0;
      #pragma unroll
      for (int i = 0; i < STORE_LATENCY_DRAM; ++i) 
        st_commit_addr[i] = INVALID_ADDRESS;

      // Registers for load logic. 
      enum LD_STATE { WAIT_FOR_REQ, SEARCH, RETURN };
      [[intel::fpga_register]] LD_STATE ld_state[NUM_LDS][LD_Q_SIZE];
      [[intel::fpga_register]] addr_dram_t ld_addr[NUM_LDS][LD_Q_SIZE];
      [[intel::fpga_register]] uint ld_tag[NUM_LDS][LD_Q_SIZE];
      [[intel::fpga_register]] bool ld_no_conflict[NUM_LDS][LD_Q_SIZE];
      [[intel::fpga_register]] value_t bypass_val[NUM_LDS][LD_Q_SIZE];
      [[intel::fpga_register]] bool is_bypass[NUM_LDS][LD_Q_SIZE];

      // Signal to stop the LSQ.
      bool end_signal = false;

      // Used if NUM_STS > 1. st_reqs and st_vals are muxed based on tag.
      req_lsq_dram_t st_req_read[NUM_STS];
      bool st_req_read_succ[NUM_STS];
      uint next_st_req_tag = 1;
      tagged_val_lsq_dram_t<value_t> st_val_read[NUM_STS];
      bool st_val_read_succ[NUM_STS];
      uint next_st_val_tag = 1;

      [[intel::ivdep]] 
      [[intel::initiation_interval(1)]] 
      [[intel::speculated_iterations(0)]]
      while (!end_signal) {
        // We can only stop if the commit queue has cleared.
        if (!st_alloc_addr_valid[0]) 
          end_signal_pipe::read(end_signal);

        /************************* Start Store Logic **************************/
        // If top of store allocation queue has a valid address, then wait for
        // value. If value arrives, pass it to the store port and put it at the
        // end of the store commit queue.
        bool st_value_valid = false;
        value_t st_value;
        auto next_st_commit_addr = INVALID_ADDRESS;
        if (st_alloc_addr_valid[0]) {
          if constexpr (NUM_STS == 1) {
            st_value = st_val_pipes:: template PipeAt<0>::read(st_value_valid);
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
                st_value = st_val_read[k].value;
                st_value_valid = true;
              }
            });
          }

          if (st_value_valid) {
            pred_st_port_pipe::write(1);
            st_port_val_pipe::write({st_alloc_addr[0], st_value});
            next_st_commit_addr = st_alloc_addr[0];
            next_st_val_tag++;
          }
        }

        // Shift commit queue on every cycles, unless no space is required.
        if (st_value_valid || st_commit_addr[0] == INVALID_ADDRESS) {
          #pragma unroll
          for (int i = 0; i < STORE_LATENCY_DRAM-1; ++i) {
            st_commit_addr[i] = st_commit_addr[i+1];
            st_commit_value[i] = st_commit_value[i+1];
          }
          st_commit_addr[STORE_LATENCY_DRAM-1] = next_st_commit_addr;
          st_commit_value[STORE_LATENCY_DRAM-1] = st_value;
        }

        // Accept new store allocations, unless no space in allocation queue.
        if (st_value_valid || !st_alloc_addr_valid[0]) {
          bool st_req_valid = false;
          auto next_st_alloc_addr = INVALID_ADDRESS;
          req_lsq_dram_t st_req;

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
            next_st_alloc_addr = st_req.addr;
            last_st_req_tag++;
            next_st_req_tag++;
          }

          #pragma unroll
          for (int i = 0; i < ST_Q_SIZE-1; ++i) {
            st_alloc_addr[i] = st_alloc_addr[i + 1];
            st_alloc_addr_valid[i] = st_alloc_addr_valid[i + 1];
            st_alloc_tag[i] = st_alloc_tag[i + 1];
          }
          st_alloc_addr[ST_Q_SIZE - 1] = next_st_alloc_addr;
          st_alloc_addr_valid[ST_Q_SIZE - 1] = st_req_valid;
          st_alloc_tag[ST_Q_SIZE - 1] = st_req.tag;
        }
        /************************** End Store Logic ***************************/


        /************************* Start Load Logic **************************/
        // All loads proceed in parallel.
        UnrolledLoop<NUM_LDS>([&](auto iLd) {
          if (ld_state[iLd][0] == LD_STATE::SEARCH && ld_no_conflict[iLd][0]) {
            // Inform ld MUX to expect value from bypass pipe or ld port.
            pred_ld_mux_pipes:: template PipeAt<iLd>::write(1);
            ld_mux_sel_pipes:: template PipeAt<iLd>::write(is_bypass[iLd][0]);
            if (is_bypass[iLd][0]) {
              // If bypass, then send value directly to mux.
              ld_mux_from_bypass_val_pipes::template PipeAt<iLd>::write(
                  bypass_val[iLd][0]);
            } else {
              // Otherwise, issue load request to load port.
              pred_ld_port_pipes:: template PipeAt<iLd>::write(1);
              ld_port_addr_pipes:: template PipeAt<iLd>::write(ld_addr[iLd][0]);
            }

            ld_state[iLd][0] = LD_STATE::WAIT_FOR_REQ;
          }

          // Wait for load allocation if there is space in the queue.
          if (ld_state[iLd][LD_Q_SIZE-1] == LD_STATE::WAIT_FOR_REQ) {
            bool pipe_succ = false;
            auto ld_req = ld_req_pipes:: template PipeAt<iLd>::read(pipe_succ);
            if (pipe_succ) {
              ld_addr[iLd][LD_Q_SIZE - 1] = ld_req.addr;
              ld_tag[iLd][LD_Q_SIZE - 1] = ld_req.tag;
              ld_state[iLd][LD_Q_SIZE - 1] = LD_STATE::SEARCH;
            }
            ld_no_conflict[iLd][LD_Q_SIZE-1] = false;
          }

          // Check the first 2 entries in the ld alloc queue for store conflicts.
          UnrolledLoop<2>([&](auto iAlloc) {
            if (ld_tag[iLd][iAlloc] <= last_st_req_tag) {
              // Pipeline stage before the OR-reduction.
              bool match[ST_Q_SIZE];
              bool ld_wait = false;
              #pragma unroll
              for (int i = 0; i < ST_Q_SIZE; ++i) {
                match[i] = (st_alloc_addr[i] == ld_addr[iLd][iAlloc] &&
                            st_alloc_tag[i] <= ld_tag[iLd][iAlloc]);
              }

              #pragma unroll
              for (int i = 0; i < ST_Q_SIZE; ++i) 
                ld_wait |= match[i];

              if (!ld_wait)
                ld_no_conflict[iLd][iAlloc] = true;
            }

            // Check iif we can possibly forward a value from the store commit
            // queue. 'is_bypass' is only valid when 'no_conflict' is true, so
            // check for bypass on every invocation.
            is_bypass[iLd][iAlloc] = false;
            #pragma unroll
            for (int i = 0; i < STORE_LATENCY_DRAM; ++i) {
              // If multiple matches, then the most recent one wins. No need to
              // check the tags since all stores in the commit queue by
              // definition come before this load in program order.
              if (st_commit_addr[i] == ld_addr[iLd][iAlloc]) {
                is_bypass[iLd][iAlloc] = true;
                bypass_val[iLd][iAlloc] = st_commit_value[i];
              }
            }
          });

          // Shift ld alloc queue whenever the top alloc has no valid request.
          if (ld_state[iLd][0] == LD_STATE::WAIT_FOR_REQ) {
            UnrolledLoop<LD_Q_SIZE - 1>([&](auto i) {
              ld_state[iLd][i] = ld_state[iLd][i + 1];
              ld_addr[iLd][i] = ld_addr[iLd][i + 1];
              ld_tag[iLd][i] = ld_tag[iLd][i + 1];
              ld_no_conflict[iLd][i] = ld_no_conflict[iLd][i + 1];
              is_bypass[iLd][i] = is_bypass[iLd][i+1];
              bypass_val[iLd][i] = bypass_val[iLd][i+1];
            });
            ld_state[iLd][LD_Q_SIZE-1] = LD_STATE::WAIT_FOR_REQ;
            ld_no_conflict[iLd][LD_Q_SIZE-1] = false;
          }
        });
        /************************** End Load Logic ***************************/

      } // End LSQ pipelined loop.

      // Send end signals to ld/st ports and the load_value/store_req muxes.
      UnrolledLoop<NUM_LDS>([&](auto iLd) {
        pred_ld_port_pipes:: template PipeAt<iLd>::write(0);
        pred_ld_mux_pipes:: template PipeAt<iLd>::write(0);
      });

      pred_st_port_pipe::write(0);

      // if constexpr (NUM_STS > 1) {
      //   st_req_mux_end_signal::write(0);
      //   st_val_mux_end_signal::write(0);
      // }

    });
  }); // End LSQ kernel
  

  // The client might want to wait for the kernels to finish. 
  // Store port will be last so wait for it.
  return storePortEvent;
}



#endif