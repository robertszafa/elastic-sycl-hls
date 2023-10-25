//Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
//Date        : Sun Jun 25 11:54:22 2023
//Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
//Command     : generate_target bd_0_wrapper.bd
//Design      : bd_0_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module bd_0_wrapper
   (a_address0,
    a_address1,
    a_ce0,
    a_ce1,
    a_d0,
    a_d1,
    a_q0,
    a_q1,
    a_we0,
    a_we1,
    addr_in_address0,
    addr_in_address1,
    addr_in_ce0,
    addr_in_ce1,
    addr_in_q0,
    addr_in_q1,
    addr_out_address0,
    addr_out_ce0,
    addr_out_q0,
    ap_clk,
    ap_ctrl_done,
    ap_ctrl_idle,
    ap_ctrl_ready,
    ap_ctrl_start,
    ap_rst,
    data_address0,
    data_ce0,
    data_d0,
    data_q0,
    data_we0);
  output [13:0]a_address0;
  output [13:0]a_address1;
  output a_ce0;
  output a_ce1;
  output [31:0]a_d0;
  output [31:0]a_d1;
  input [31:0]a_q0;
  input [31:0]a_q1;
  output a_we0;
  output a_we1;
  output [13:0]addr_in_address0;
  output [13:0]addr_in_address1;
  output addr_in_ce0;
  output addr_in_ce1;
  input [31:0]addr_in_q0;
  input [31:0]addr_in_q1;
  output [13:0]addr_out_address0;
  output addr_out_ce0;
  input [31:0]addr_out_q0;
  input ap_clk;
  output ap_ctrl_done;
  output ap_ctrl_idle;
  output ap_ctrl_ready;
  input ap_ctrl_start;
  input ap_rst;
  output [13:0]data_address0;
  output data_ce0;
  output [31:0]data_d0;
  input [31:0]data_q0;
  output data_we0;

  wire [13:0]a_address0;
  wire [13:0]a_address1;
  wire a_ce0;
  wire a_ce1;
  wire [31:0]a_d0;
  wire [31:0]a_d1;
  wire [31:0]a_q0;
  wire [31:0]a_q1;
  wire a_we0;
  wire a_we1;
  wire [13:0]addr_in_address0;
  wire [13:0]addr_in_address1;
  wire addr_in_ce0;
  wire addr_in_ce1;
  wire [31:0]addr_in_q0;
  wire [31:0]addr_in_q1;
  wire [13:0]addr_out_address0;
  wire addr_out_ce0;
  wire [31:0]addr_out_q0;
  wire ap_clk;
  wire ap_ctrl_done;
  wire ap_ctrl_idle;
  wire ap_ctrl_ready;
  wire ap_ctrl_start;
  wire ap_rst;
  wire [13:0]data_address0;
  wire data_ce0;
  wire [31:0]data_d0;
  wire [31:0]data_q0;
  wire data_we0;

  bd_0 bd_0_i
       (.a_address0(a_address0),
        .a_address1(a_address1),
        .a_ce0(a_ce0),
        .a_ce1(a_ce1),
        .a_d0(a_d0),
        .a_d1(a_d1),
        .a_q0(a_q0),
        .a_q1(a_q1),
        .a_we0(a_we0),
        .a_we1(a_we1),
        .addr_in_address0(addr_in_address0),
        .addr_in_address1(addr_in_address1),
        .addr_in_ce0(addr_in_ce0),
        .addr_in_ce1(addr_in_ce1),
        .addr_in_q0(addr_in_q0),
        .addr_in_q1(addr_in_q1),
        .addr_out_address0(addr_out_address0),
        .addr_out_ce0(addr_out_ce0),
        .addr_out_q0(addr_out_q0),
        .ap_clk(ap_clk),
        .ap_ctrl_done(ap_ctrl_done),
        .ap_ctrl_idle(ap_ctrl_idle),
        .ap_ctrl_ready(ap_ctrl_ready),
        .ap_ctrl_start(ap_ctrl_start),
        .ap_rst(ap_rst),
        .data_address0(data_address0),
        .data_ce0(data_ce0),
        .data_d0(data_d0),
        .data_q0(data_q0),
        .data_we0(data_we0));
endmodule
