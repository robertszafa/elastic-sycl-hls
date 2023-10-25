// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
// Date        : Sun Jun 25 13:38:15 2023
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
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix(matrix_ce0, matrix_we0, matrix_ce1, matrix_we1, 
  row_ce0, row_ce1, col_ce0, col_ce1, a_ce0, a_ce1, ap_clk, ap_rst, ap_start, ap_done, ap_idle, ap_ready, 
  matrix_address0, matrix_d0, matrix_q0, matrix_address1, matrix_d1, matrix_q1, row_address0, 
  row_q0, row_address1, row_q1, col_address0, col_q0, col_address1, col_q1, a_address0, a_q0, 
  a_address1, a_q1)
/* synthesis syn_black_box black_box_pad_pin="matrix_ce0,matrix_we0,matrix_ce1,matrix_we1,row_ce0,row_ce1,col_ce0,col_ce1,a_ce0,a_ce1,ap_clk,ap_rst,ap_start,ap_done,ap_idle,ap_ready,matrix_address0[8:0],matrix_d0[31:0],matrix_q0[31:0],matrix_address1[8:0],matrix_d1[31:0],matrix_q1[31:0],row_address0[8:0],row_q0[31:0],row_address1[8:0],row_q1[31:0],col_address0[8:0],col_q0[31:0],col_address1[8:0],col_q1[31:0],a_address0[8:0],a_q0[31:0],a_address1[8:0],a_q1[31:0]" */;
  output matrix_ce0;
  output matrix_we0;
  output matrix_ce1;
  output matrix_we1;
  output row_ce0;
  output row_ce1;
  output col_ce0;
  output col_ce1;
  output a_ce0;
  output a_ce1;
  input ap_clk;
  input ap_rst;
  input ap_start;
  output ap_done;
  output ap_idle;
  output ap_ready;
  output [8:0]matrix_address0;
  output [31:0]matrix_d0;
  input [31:0]matrix_q0;
  output [8:0]matrix_address1;
  output [31:0]matrix_d1;
  input [31:0]matrix_q1;
  output [8:0]row_address0;
  input [31:0]row_q0;
  output [8:0]row_address1;
  input [31:0]row_q1;
  output [8:0]col_address0;
  input [31:0]col_q0;
  output [8:0]col_address1;
  input [31:0]col_q1;
  output [8:0]a_address0;
  input [31:0]a_q0;
  output [8:0]a_address1;
  input [31:0]a_q1;
endmodule
