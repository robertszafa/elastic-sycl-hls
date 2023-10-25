//Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
//Date        : Fri Jun 23 13:44:44 2023
//Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
//Command     : generate_target bd_0_wrapper.bd
//Design      : bd_0_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module bd_0_wrapper
   (M_address0,
    M_address1,
    M_ce0,
    M_ce1,
    M_q0,
    M_q1,
    ap_clk,
    ap_ctrl_done,
    ap_ctrl_idle,
    ap_ctrl_ready,
    ap_ctrl_start,
    ap_rst,
    buffer_r_address0,
    buffer_r_address1,
    buffer_r_ce0,
    buffer_r_ce1,
    buffer_r_d0,
    buffer_r_d1,
    buffer_r_q0,
    buffer_r_q1,
    buffer_r_we0,
    buffer_r_we1);
  output [11:0]M_address0;
  output [11:0]M_address1;
  output M_ce0;
  output M_ce1;
  input [31:0]M_q0;
  input [31:0]M_q1;
  input ap_clk;
  output ap_ctrl_done;
  output ap_ctrl_idle;
  output ap_ctrl_ready;
  input ap_ctrl_start;
  input ap_rst;
  output [11:0]buffer_r_address0;
  output [11:0]buffer_r_address1;
  output buffer_r_ce0;
  output buffer_r_ce1;
  output [31:0]buffer_r_d0;
  output [31:0]buffer_r_d1;
  input [31:0]buffer_r_q0;
  input [31:0]buffer_r_q1;
  output buffer_r_we0;
  output buffer_r_we1;

  wire [11:0]M_address0;
  wire [11:0]M_address1;
  wire M_ce0;
  wire M_ce1;
  wire [31:0]M_q0;
  wire [31:0]M_q1;
  wire ap_clk;
  wire ap_ctrl_done;
  wire ap_ctrl_idle;
  wire ap_ctrl_ready;
  wire ap_ctrl_start;
  wire ap_rst;
  wire [11:0]buffer_r_address0;
  wire [11:0]buffer_r_address1;
  wire buffer_r_ce0;
  wire buffer_r_ce1;
  wire [31:0]buffer_r_d0;
  wire [31:0]buffer_r_d1;
  wire [31:0]buffer_r_q0;
  wire [31:0]buffer_r_q1;
  wire buffer_r_we0;
  wire buffer_r_we1;

  bd_0 bd_0_i
       (.M_address0(M_address0),
        .M_address1(M_address1),
        .M_ce0(M_ce0),
        .M_ce1(M_ce1),
        .M_q0(M_q0),
        .M_q1(M_q1),
        .ap_clk(ap_clk),
        .ap_ctrl_done(ap_ctrl_done),
        .ap_ctrl_idle(ap_ctrl_idle),
        .ap_ctrl_ready(ap_ctrl_ready),
        .ap_ctrl_start(ap_ctrl_start),
        .ap_rst(ap_rst),
        .buffer_r_address0(buffer_r_address0),
        .buffer_r_address1(buffer_r_address1),
        .buffer_r_ce0(buffer_r_ce0),
        .buffer_r_ce1(buffer_r_ce1),
        .buffer_r_d0(buffer_r_d0),
        .buffer_r_d1(buffer_r_d1),
        .buffer_r_q0(buffer_r_q0),
        .buffer_r_q1(buffer_r_q1),
        .buffer_r_we0(buffer_r_we0),
        .buffer_r_we1(buffer_r_we1));
endmodule
