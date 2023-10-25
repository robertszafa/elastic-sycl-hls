//Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
//Date        : Thu Jul  6 15:26:11 2023
//Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
//Command     : generate_target bd_0_wrapper.bd
//Design      : bd_0_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module bd_0_wrapper
   (A_address0,
    A_ce0,
    A_q0,
    C_address0,
    C_ce0,
    C_q0,
    ap_clk,
    ap_ctrl_done,
    ap_ctrl_idle,
    ap_ctrl_ready,
    ap_ctrl_start,
    ap_rst,
    hist_address0,
    hist_ce0,
    hist_d0,
    hist_q0,
    hist_we0);
  output [9:0]A_address0;
  output A_ce0;
  input [31:0]A_q0;
  output [9:0]C_address0;
  output C_ce0;
  input [31:0]C_q0;
  input ap_clk;
  output ap_ctrl_done;
  output ap_ctrl_idle;
  output ap_ctrl_ready;
  input ap_ctrl_start;
  input ap_rst;
  output [9:0]hist_address0;
  output hist_ce0;
  output [31:0]hist_d0;
  input [31:0]hist_q0;
  output hist_we0;

  wire [9:0]A_address0;
  wire A_ce0;
  wire [31:0]A_q0;
  wire [9:0]C_address0;
  wire C_ce0;
  wire [31:0]C_q0;
  wire ap_clk;
  wire ap_ctrl_done;
  wire ap_ctrl_idle;
  wire ap_ctrl_ready;
  wire ap_ctrl_start;
  wire ap_rst;
  wire [9:0]hist_address0;
  wire hist_ce0;
  wire [31:0]hist_d0;
  wire [31:0]hist_q0;
  wire hist_we0;

  bd_0 bd_0_i
       (.A_address0(A_address0),
        .A_ce0(A_ce0),
        .A_q0(A_q0),
        .C_address0(C_address0),
        .C_ce0(C_ce0),
        .C_q0(C_q0),
        .ap_clk(ap_clk),
        .ap_ctrl_done(ap_ctrl_done),
        .ap_ctrl_idle(ap_ctrl_idle),
        .ap_ctrl_ready(ap_ctrl_ready),
        .ap_ctrl_start(ap_ctrl_start),
        .ap_rst(ap_rst),
        .hist_address0(hist_address0),
        .hist_ce0(hist_ce0),
        .hist_d0(hist_d0),
        .hist_q0(hist_q0),
        .hist_we0(hist_we0));
endmodule
