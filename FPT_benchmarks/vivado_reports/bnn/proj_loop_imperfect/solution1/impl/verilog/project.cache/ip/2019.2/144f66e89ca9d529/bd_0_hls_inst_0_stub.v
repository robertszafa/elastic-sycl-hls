// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
// Date        : Sun Jun 25 11:56:46 2023
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
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix(data_ce0, data_we0, addr_in_ce0, addr_in_ce1, 
  addr_out_ce0, a_ce0, a_we0, a_ce1, a_we1, ap_clk, ap_rst, ap_start, ap_done, ap_idle, ap_ready, 
  data_address0, data_d0, data_q0, addr_in_address0, addr_in_q0, addr_in_address1, addr_in_q1, 
  addr_out_address0, addr_out_q0, a_address0, a_d0, a_q0, a_address1, a_d1, a_q1)
/* synthesis syn_black_box black_box_pad_pin="data_ce0,data_we0,addr_in_ce0,addr_in_ce1,addr_out_ce0,a_ce0,a_we0,a_ce1,a_we1,ap_clk,ap_rst,ap_start,ap_done,ap_idle,ap_ready,data_address0[13:0],data_d0[31:0],data_q0[31:0],addr_in_address0[13:0],addr_in_q0[31:0],addr_in_address1[13:0],addr_in_q1[31:0],addr_out_address0[13:0],addr_out_q0[31:0],a_address0[13:0],a_d0[31:0],a_q0[31:0],a_address1[13:0],a_d1[31:0],a_q1[31:0]" */;
  output data_ce0;
  output data_we0;
  output addr_in_ce0;
  output addr_in_ce1;
  output addr_out_ce0;
  output a_ce0;
  output a_we0;
  output a_ce1;
  output a_we1;
  input ap_clk;
  input ap_rst;
  input ap_start;
  output ap_done;
  output ap_idle;
  output ap_ready;
  output [13:0]data_address0;
  output [31:0]data_d0;
  input [31:0]data_q0;
  output [13:0]addr_in_address0;
  input [31:0]addr_in_q0;
  output [13:0]addr_in_address1;
  input [31:0]addr_in_q1;
  output [13:0]addr_out_address0;
  input [31:0]addr_out_q0;
  output [13:0]a_address0;
  output [31:0]a_d0;
  input [31:0]a_q0;
  output [13:0]a_address1;
  output [31:0]a_d1;
  input [31:0]a_q1;
endmodule
