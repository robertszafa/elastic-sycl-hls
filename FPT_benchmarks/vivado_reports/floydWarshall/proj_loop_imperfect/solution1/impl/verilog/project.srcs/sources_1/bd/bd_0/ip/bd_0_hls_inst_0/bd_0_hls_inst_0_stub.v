// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
// Date        : Wed Jul  5 16:28:03 2023
// Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
// Command     : write_verilog -force -mode synth_stub
//               /home/dynamatic/floydWarshall/proj_loop_imperfect/solution1/impl/verilog/project.srcs/sources_1/bd/bd_0/ip/bd_0_hls_inst_0/bd_0_hls_inst_0_stub.v
// Design      : bd_0_hls_inst_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7k160tfbg484-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "loop_imperfect,Vivado 2019.2" *)
module bd_0_hls_inst_0(dist_ce0, dist_ce1, dist_we1, ap_clk, ap_rst, 
  ap_start, ap_done, ap_idle, ap_ready, dist_address0, dist_q0, dist_address1, dist_d1, dist_q1)
/* synthesis syn_black_box black_box_pad_pin="dist_ce0,dist_ce1,dist_we1,ap_clk,ap_rst,ap_start,ap_done,ap_idle,ap_ready,dist_address0[6:0],dist_q0[31:0],dist_address1[6:0],dist_d1[31:0],dist_q1[31:0]" */;
  output dist_ce0;
  output dist_ce1;
  output dist_we1;
  input ap_clk;
  input ap_rst;
  input ap_start;
  output ap_done;
  output ap_idle;
  output ap_ready;
  output [6:0]dist_address0;
  input [31:0]dist_q0;
  output [6:0]dist_address1;
  output [31:0]dist_d1;
  input [31:0]dist_q1;
endmodule
