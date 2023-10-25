//Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
//Date        : Sun Jun 25 13:37:27 2023
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
    a_q0,
    a_q1,
    ap_clk,
    ap_ctrl_done,
    ap_ctrl_idle,
    ap_ctrl_ready,
    ap_ctrl_start,
    ap_rst,
    col_address0,
    col_address1,
    col_ce0,
    col_ce1,
    col_q0,
    col_q1,
    matrix_address0,
    matrix_address1,
    matrix_ce0,
    matrix_ce1,
    matrix_d0,
    matrix_d1,
    matrix_q0,
    matrix_q1,
    matrix_we0,
    matrix_we1,
    row_address0,
    row_address1,
    row_ce0,
    row_ce1,
    row_q0,
    row_q1);
  output [8:0]a_address0;
  output [8:0]a_address1;
  output a_ce0;
  output a_ce1;
  input [31:0]a_q0;
  input [31:0]a_q1;
  input ap_clk;
  output ap_ctrl_done;
  output ap_ctrl_idle;
  output ap_ctrl_ready;
  input ap_ctrl_start;
  input ap_rst;
  output [8:0]col_address0;
  output [8:0]col_address1;
  output col_ce0;
  output col_ce1;
  input [31:0]col_q0;
  input [31:0]col_q1;
  output [8:0]matrix_address0;
  output [8:0]matrix_address1;
  output matrix_ce0;
  output matrix_ce1;
  output [31:0]matrix_d0;
  output [31:0]matrix_d1;
  input [31:0]matrix_q0;
  input [31:0]matrix_q1;
  output matrix_we0;
  output matrix_we1;
  output [8:0]row_address0;
  output [8:0]row_address1;
  output row_ce0;
  output row_ce1;
  input [31:0]row_q0;
  input [31:0]row_q1;

  wire [8:0]a_address0;
  wire [8:0]a_address1;
  wire a_ce0;
  wire a_ce1;
  wire [31:0]a_q0;
  wire [31:0]a_q1;
  wire ap_clk;
  wire ap_ctrl_done;
  wire ap_ctrl_idle;
  wire ap_ctrl_ready;
  wire ap_ctrl_start;
  wire ap_rst;
  wire [8:0]col_address0;
  wire [8:0]col_address1;
  wire col_ce0;
  wire col_ce1;
  wire [31:0]col_q0;
  wire [31:0]col_q1;
  wire [8:0]matrix_address0;
  wire [8:0]matrix_address1;
  wire matrix_ce0;
  wire matrix_ce1;
  wire [31:0]matrix_d0;
  wire [31:0]matrix_d1;
  wire [31:0]matrix_q0;
  wire [31:0]matrix_q1;
  wire matrix_we0;
  wire matrix_we1;
  wire [8:0]row_address0;
  wire [8:0]row_address1;
  wire row_ce0;
  wire row_ce1;
  wire [31:0]row_q0;
  wire [31:0]row_q1;

  bd_0 bd_0_i
       (.a_address0(a_address0),
        .a_address1(a_address1),
        .a_ce0(a_ce0),
        .a_ce1(a_ce1),
        .a_q0(a_q0),
        .a_q1(a_q1),
        .ap_clk(ap_clk),
        .ap_ctrl_done(ap_ctrl_done),
        .ap_ctrl_idle(ap_ctrl_idle),
        .ap_ctrl_ready(ap_ctrl_ready),
        .ap_ctrl_start(ap_ctrl_start),
        .ap_rst(ap_rst),
        .col_address0(col_address0),
        .col_address1(col_address1),
        .col_ce0(col_ce0),
        .col_ce1(col_ce1),
        .col_q0(col_q0),
        .col_q1(col_q1),
        .matrix_address0(matrix_address0),
        .matrix_address1(matrix_address1),
        .matrix_ce0(matrix_ce0),
        .matrix_ce1(matrix_ce1),
        .matrix_d0(matrix_d0),
        .matrix_d1(matrix_d1),
        .matrix_q0(matrix_q0),
        .matrix_q1(matrix_q1),
        .matrix_we0(matrix_we0),
        .matrix_we1(matrix_we1),
        .row_address0(row_address0),
        .row_address1(row_address1),
        .row_ce0(row_ce0),
        .row_ce1(row_ce1),
        .row_q0(row_q0),
        .row_q1(row_q1));
endmodule
