//Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
//Date        : Tue Oct 24 09:48:21 2023
//Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
//Command     : generate_target bd_0_wrapper.bd
//Design      : bd_0_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module bd_0_wrapper
   (A_address0,
    A_address1,
    A_ce0,
    A_ce1,
    A_d0,
    A_d1,
    A_q0,
    A_q1,
    A_we0,
    A_we1,
    ap_clk,
    ap_ctrl_done,
    ap_ctrl_idle,
    ap_ctrl_ready,
    ap_ctrl_start,
    ap_rst);
  output [5:0]A_address0;
  output [5:0]A_address1;
  output A_ce0;
  output A_ce1;
  output [31:0]A_d0;
  output [31:0]A_d1;
  input [31:0]A_q0;
  input [31:0]A_q1;
  output A_we0;
  output A_we1;
  input ap_clk;
  output ap_ctrl_done;
  output ap_ctrl_idle;
  output ap_ctrl_ready;
  input ap_ctrl_start;
  input ap_rst;

  wire [5:0]A_address0;
  wire [5:0]A_address1;
  wire A_ce0;
  wire A_ce1;
  wire [31:0]A_d0;
  wire [31:0]A_d1;
  wire [31:0]A_q0;
  wire [31:0]A_q1;
  wire A_we0;
  wire A_we1;
  wire ap_clk;
  wire ap_ctrl_done;
  wire ap_ctrl_idle;
  wire ap_ctrl_ready;
  wire ap_ctrl_start;
  wire ap_rst;

  bd_0 bd_0_i
       (.A_address0(A_address0),
        .A_address1(A_address1),
        .A_ce0(A_ce0),
        .A_ce1(A_ce1),
        .A_d0(A_d0),
        .A_d1(A_d1),
        .A_q0(A_q0),
        .A_q1(A_q1),
        .A_we0(A_we0),
        .A_we1(A_we1),
        .ap_clk(ap_clk),
        .ap_ctrl_done(ap_ctrl_done),
        .ap_ctrl_idle(ap_ctrl_idle),
        .ap_ctrl_ready(ap_ctrl_ready),
        .ap_ctrl_start(ap_ctrl_start),
        .ap_rst(ap_rst));
endmodule
