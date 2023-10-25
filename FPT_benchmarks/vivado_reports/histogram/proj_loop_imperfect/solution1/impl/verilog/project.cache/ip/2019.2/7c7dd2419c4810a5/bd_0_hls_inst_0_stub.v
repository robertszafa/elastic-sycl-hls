// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
// Date        : Mon Jun 19 13:13:20 2023
// Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
// Command     : write_verilog -force -mode synth_stub -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
//               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ bd_0_hls_inst_0_stub.v
// Design      : bd_0_hls_inst_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7k160tfbg484-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "loop_imperfect,Vivado 2019.2" *)
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix(A_ce0, hist_ce0, hist_we0, C_ce0, ap_clk, ap_rst, 
  ap_start, ap_done, ap_idle, ap_ready, A_address0, A_q0, hist_address0, hist_d0, hist_q0, 
  C_address0, C_q0)
/* synthesis syn_black_box black_box_pad_pin="A_ce0,hist_ce0,hist_we0,C_ce0,ap_clk,ap_rst,ap_start,ap_done,ap_idle,ap_ready,A_address0[9:0],A_q0[31:0],hist_address0[9:0],hist_d0[31:0],hist_q0[31:0],C_address0[9:0],C_q0[31:0]" */;
  output A_ce0;
  output hist_ce0;
  output hist_we0;
  output C_ce0;
  input ap_clk;
  input ap_rst;
  input ap_start;
  output ap_done;
  output ap_idle;
  output ap_ready;
  output [9:0]A_address0;
  input [31:0]A_q0;
  output [9:0]hist_address0;
  output [31:0]hist_d0;
  input [31:0]hist_q0;
  output [9:0]C_address0;
  input [31:0]C_q0;
endmodule
