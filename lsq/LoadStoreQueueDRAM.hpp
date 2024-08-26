/*
Load Store Queue kernel for SYCL HLS. The LSQ is for off-chip memory.
*/

#ifndef __LOAD_STORE_QUEUE_DRAM_HPP__
#define __LOAD_STORE_QUEUE_DRAM_HPP__

#include <sycl/sycl.hpp>

#include <sycl/ext/intel/fpga_extensions.hpp>

#include "constexpr_math.hpp"
#include "pipe_utils.hpp"
#include "unrolled_loop.hpp"
#include "tuple.hpp"

using namespace sycl;
using namespace fpga_tools;

// All our kernels don't have aliasing memory and don't need OpenCL ids.
#define KERNEL_PRAGMAS [[intel::kernel_args_restrict]] [[intel::max_global_work_dim(0)]] 

// Use the simplest load/store units for DRAM (no bursts, no caches, no prefetching).
using PipelinedLSU = sycl::ext::intel::lsu<>;

/// Max number of cycles between issuing store to DRAM memory controller and commit.
constexpr int LSU_STORE_LATENCY = 8; 
constexpr int LSU_LOAD_LATENCY = 8; 

/// Represents pointer bits.
using addr_dram_t = int64_t;
constexpr addr_dram_t INVALID_ADDRESS = -1; 

struct req_lsq_dram_t { addr_dram_t addr; uint tag; };

template <typename T>
struct addr_dram_val_pair_t { addr_dram_t addr; T value; };

template <typename T>
struct tagged_val_lsq_dram_t { T value; uint tag; bool valid; };

// To generate unique kernel names for multiple load ports across multiple LSQs.
template <typename LSQ_ID, int PORT_NUM>
class LoadPort;
template <typename LSQ_ID, int MUX_NUM>
class LoadMux;
template <typename LSQ_ID>
class StorePort;
template <typename LSQ_ID>
class LSQ_DRAM;
template <typename LSQ_ID>
class StoreReqMux;

// Applaying [[optnone]] to StreamingMemory doesn't apply the attribute to 
// nested lambdas, so apply the attribute to a range of source code.
#pragma clang attribute push (__attribute__((optnone)), apply_to=function)

template <typename value_t, typename ld_req_pipes, typename ld_val_pipes,
          typename st_req_pipes, typename st_val_pipes,
          typename end_signal_pipe, bool USE_SPECULATION, uint NUM_LDS,
          uint NUM_ST_REQ, uint NUM_ST_VAL, uint LD_Q_SIZE = 4,
          uint ST_Q_SIZE = 8>
[[clang::optnone]] event LoadStoreQueueDRAM(queue &q) {
  assert(LD_Q_SIZE >= 2 && "Load queue size must be at least 2.");
  assert(ST_Q_SIZE >= 2 && "Store queue size must be at least 2.");

  // Pipes to connect LSQ to ld/st ports and to ld value mux.
  using pred_st_port_pipe = pipe<class pred_st_port_pipe_class, bool, NUM_ST_REQ>;
  using st_port_val_pipe = pipe<class st_port_val_pipe_class, addr_dram_val_pair_t<value_t>, NUM_ST_REQ>;

  // Each load gets its own load port and load mux, thus the use of PipeArrays.
  using pred_ld_port_pipes = PipeArray<class pred_ld_port_pipe_class, bool, LSU_LOAD_LATENCY, NUM_LDS>;
  using ld_port_addr_pipes = PipeArray<class ld_port_addr_pipe_class, addr_dram_t, LSU_LOAD_LATENCY, NUM_LDS>;
  using pred_ld_mux_pipes = PipeArray<class pred_ld_mux_pipe_class, bool, LSU_LOAD_LATENCY, NUM_LDS>;
  using ld_mux_sel_pipes = PipeArray<class ld_mux_sel_pipe_class, bool, LSU_LOAD_LATENCY, NUM_LDS>;
  using ld_mux_from_memory_val_pipes = PipeArray<class ld_mux_from_memory_val_class, value_t, LSU_LOAD_LATENCY, NUM_LDS>;
  using ld_mux_from_bypass_val_pipes = PipeArray<class ld_mux_from_bypass_val_class, value_t, LSU_LOAD_LATENCY, NUM_LDS>;

  /// Store port kernel
  auto storePortEvent = q.submit([&](handler &hnd) {
    hnd.single_task<StorePort<end_signal_pipe>>([=]() KERNEL_PRAGMAS {
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
      hnd.single_task<LoadPort<end_signal_pipe, iLd>>([=]() KERNEL_PRAGMAS {
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
      hnd.single_task<LoadMux<end_signal_pipe, iLd>>([=]() KERNEL_PRAGMAS {
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

  // Multiple store requests are muxed when coming into the LSQ.
  using st_req_pipe = pipe<class st_req_pipe_mux_, req_lsq_dram_t, NUM_ST_REQ>;
  using st_req_mux_end_signal = pipe<class st_req_mux_end_signal_, bool>;
  if constexpr (NUM_ST_REQ > 1) {
    q.single_task<StoreReqMux<end_signal_pipe>>([=]() KERNEL_PRAGMAS {
      // The expected next tag, always increasing by one.
      uint next_tag = 1;

      NTuple<req_lsq_dram_t, NUM_ST_REQ> st_req_tuple;
      NTuple<bool, NUM_ST_REQ> read_succ_tuple;

      bool end_signal = false;
      [[intel::initiation_interval(1)]] 
      [[intel::speculated_iterations(0)]]
      while (!end_signal) {
        st_req_mux_end_signal::read(end_signal);

        // Choose one, based on tag.
        bool is_req_valid = false;
        req_lsq_dram_t req_to_write;
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

  /// Load store queue kernel.
  auto lsqEvent = q.submit([&](handler &hnd) {
    hnd.single_task<LSQ_DRAM<end_signal_pipe>>([=]() KERNEL_PRAGMAS {
      // Registers for store logic.
      [[intel::fpga_register]] addr_dram_t st_alloc_addr[ST_Q_SIZE];
      [[intel::fpga_register]] bool st_alloc_addr_valid[ST_Q_SIZE];
      [[intel::fpga_register]] uint st_alloc_tag[ST_Q_SIZE];
      [[intel::fpga_register]] addr_dram_t st_commit_addr[LSU_STORE_LATENCY];
      [[intel::fpga_register]] value_t st_commit_value[LSU_STORE_LATENCY];
      uint last_st_req_tag = 0;
      #pragma unroll
      for (int i = 0; i < LSU_STORE_LATENCY; ++i) 
        st_commit_addr[i] = INVALID_ADDRESS;

      // Registers for load logic. 
      enum LD_STATE { WAIT_FOR_REQ, SEARCH, RETURN };
      [[intel::fpga_register]] LD_STATE ld_state[NUM_LDS][LD_Q_SIZE];
      [[intel::fpga_register]] addr_dram_t ld_addr[NUM_LDS][LD_Q_SIZE];
      [[intel::fpga_register]] uint ld_tag[NUM_LDS][LD_Q_SIZE];
      [[intel::fpga_register]] bool ld_is_safe[NUM_LDS][LD_Q_SIZE];
      [[intel::fpga_register]] value_t bypass_val[NUM_LDS][LD_Q_SIZE];
      [[intel::fpga_register]] bool is_bypass[NUM_LDS][LD_Q_SIZE];

      // Signal to stop the LSQ.
      bool end_signal = false;

      // Used if NUM_STS > 1. st_vals are muxed based on tag.
      tagged_val_lsq_dram_t<value_t> st_val_read[NUM_ST_VAL];
      bool st_val_read_succ[NUM_ST_VAL];
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
        bool st_val_arrived = false, st_val_valid = false;
        value_t st_val;
        auto next_st_commit_addr = INVALID_ADDRESS;
        if (st_alloc_addr_valid[0]) {
          if constexpr (NUM_ST_REQ == 1) {
            auto _rd = st_val_pipes::template PipeAt<0>::read(st_val_arrived);
            st_val = _rd.value;
            // Optional support for speculative address allocations. 
            // If enabled, then the store value needs a valid bit to commit.
            if constexpr (USE_SPECULATION) {
              if (st_val_arrived)
                st_val_valid = _rd.valid; 
            } else {
              st_val_valid = st_val_arrived;
            }
          } else {
            // Store value mux.
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
          }

          if (st_val_valid) {
            pred_st_port_pipe::write(1);
            st_port_val_pipe::write({st_alloc_addr[0], st_val});
            next_st_commit_addr = st_alloc_addr[0];
          }
        }

        // Shift commit queue on every cycle, unless no space is required. This 
        // keeps the values in the queue longer than needed, increasing re-use.
        if (st_val_valid || st_commit_addr[0] == INVALID_ADDRESS) {
          #pragma unroll
          for (int i = 0; i < LSU_STORE_LATENCY-1; ++i) {
            st_commit_addr[i] = st_commit_addr[i+1];
            st_commit_value[i] = st_commit_value[i+1];
          }
          st_commit_addr[LSU_STORE_LATENCY-1] = next_st_commit_addr;
          st_commit_value[LSU_STORE_LATENCY-1] = st_val;
        }

        // Accept new store allocations, unless no space in allocation queue.
        if (st_val_arrived || !st_alloc_addr_valid[0]) {
          bool st_req_valid = false;
          auto next_st_alloc_addr = INVALID_ADDRESS;
          req_lsq_dram_t st_req;

          if constexpr (NUM_ST_REQ == 1)
            st_req = st_req_pipes:: template PipeAt<0>::read(st_req_valid);
          else 
            st_req = st_req_pipe::read(st_req_valid);

          if (st_req_valid) {
            next_st_alloc_addr = st_req.addr;
            last_st_req_tag++;
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
        UnrolledLoop<NUM_LDS>([&](auto iPort) {
          if (ld_state[iPort][0] == LD_STATE::SEARCH && ld_is_safe[iPort][0]) {
            // Inform ld MUX to expect value from bypass pipe or ld port.
            pred_ld_mux_pipes:: template PipeAt<iPort>::write(1);
            ld_mux_sel_pipes:: template PipeAt<iPort>::write(is_bypass[iPort][0]);
            if (is_bypass[iPort][0]) {
              // If bypass, then send value directly to mux.
              ld_mux_from_bypass_val_pipes::template PipeAt<iPort>::write(
                  bypass_val[iPort][0]);
            } else {
              // Otherwise, issue load request to load port.
              pred_ld_port_pipes:: template PipeAt<iPort>::write(1);
              ld_port_addr_pipes:: template PipeAt<iPort>::write(ld_addr[iPort][0]);
            }

            ld_state[iPort][0] = LD_STATE::WAIT_FOR_REQ;
          }

          // Wait for load allocation if there is space in the queue.
          if (ld_state[iPort][LD_Q_SIZE-1] == LD_STATE::WAIT_FOR_REQ) {
            bool pipe_succ = false;
            auto ld_req = ld_req_pipes:: template PipeAt<iPort>::read(pipe_succ);
            if (pipe_succ) {
              ld_addr[iPort][LD_Q_SIZE - 1] = ld_req.addr;
              ld_tag[iPort][LD_Q_SIZE - 1] = ld_req.tag;
              ld_state[iPort][LD_Q_SIZE - 1] = LD_STATE::SEARCH;
            }
            ld_is_safe[iPort][LD_Q_SIZE-1] = false;
          }

          // Check the first 2 entries in the ld alloc queue for store conflicts.
          UnrolledLoop<2>([&](auto iLd) {
            if (ld_tag[iPort][iLd] <= last_st_req_tag) {
              bool match[ST_Q_SIZE];
              #pragma unroll
              for (int i = 0; i < ST_Q_SIZE; ++i) {
                match[i] = (st_alloc_addr[i] == ld_addr[iPort][iLd] && 
                            st_alloc_tag[i] <= ld_tag[iPort][iLd]);
              }

              bool ld_wait = false;
              #pragma unroll
              for (int i=0; i<ST_Q_SIZE; ++i)
                ld_wait |= match[i];

              // Once a load is marked as safe, it cannot go back to unsafe.
              if (!ld_wait) 
                ld_is_safe[iPort][iLd] = true;
            }

            // Check if we can possibly forward a value from the store commit
            // queue. 'is_bypass' is only valid when 'no_conflict' is true, so
            // check for bypass on every invocation.
            is_bypass[iPort][iLd] = false;
            #pragma unroll
            for (int i = 0; i < LSU_STORE_LATENCY; ++i) {
              // If multiple matches, then the most recent one wins. No need to
              // check the tags since all stores in the commit queue by
              // definition come before this load in program order.
              if (st_commit_addr[i] == ld_addr[iPort][iLd]) {
                is_bypass[iPort][iLd] = true;
                bypass_val[iPort][iLd] = st_commit_value[i];
              }
            }
          });

          // Shift ld alloc queue whenever the top alloc has no valid request.
          if (ld_state[iPort][0] == LD_STATE::WAIT_FOR_REQ) {
            UnrolledLoop<LD_Q_SIZE - 1>([&](auto i) {
              ld_state[iPort][i] = ld_state[iPort][i + 1];
              ld_addr[iPort][i] = ld_addr[iPort][i + 1];
              ld_tag[iPort][i] = ld_tag[iPort][i + 1];
              ld_is_safe[iPort][i] = ld_is_safe[iPort][i + 1];
              is_bypass[iPort][i] = is_bypass[iPort][i+1];
              bypass_val[iPort][i] = bypass_val[iPort][i+1];
            });
            ld_state[iPort][LD_Q_SIZE-1] = LD_STATE::WAIT_FOR_REQ;
            ld_is_safe[iPort][LD_Q_SIZE-1] = false;
          }
        });
        /************************** End Load Logic ***************************/

      } // End LSQ pipelined loop.

      // Send end signals to ld/st ports.
      UnrolledLoop<NUM_LDS>([&](auto iLd) {
        pred_ld_port_pipes:: template PipeAt<iLd>::write(0);
        pred_ld_mux_pipes:: template PipeAt<iLd>::write(0);
      });
      pred_st_port_pipe::write(0);

      if constexpr (NUM_ST_REQ > 1) 
        st_req_mux_end_signal::write(0);

    });
  }); // End LSQ kernel
  

  // Return the event for the last kernel to finish.
  return storePortEvent;
}

#pragma clang attribute pop

#endif