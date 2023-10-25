//Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
//Date        : Wed Jul  5 16:26:52 2023
//Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
//Command     : generate_target bd_0_wrapper.bd
//Design      : bd_0_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module bd_0_wrapper
   (ap_clk,
    ap_ctrl_done,
    ap_ctrl_idle,
    ap_ctrl_ready,
    ap_ctrl_start,
    ap_rst,
    dist_address0,
    dist_address1,
    dist_ce0,
    dist_ce1,
    dist_d1,
    dist_q0,
    dist_q1,
    dist_we1);
  input ap_clk;
  output ap_ctrl_done;
  output ap_ctrl_idle;
  output ap_ctrl_ready;
  input ap_ctrl_start;
  input ap_rst;
  output [6:0]dist_address0;
  output [6:0]dist_address1;
  output dist_ce0;
  output dist_ce1;
  output [31:0]dist_d1;
  input [31:0]dist_q0;
  input [31:0]dist_q1;
  output dist_we1;

  wire ap_clk;
  wire ap_ctrl_done;
  wire ap_ctrl_idle;
  wire ap_ctrl_ready;
  wire ap_ctrl_start;
  wire ap_rst;
  wire [6:0]dist_address0;
  wire [6:0]dist_address1;
  wire dist_ce0;
  wire dist_ce1;
  wire [31:0]dist_d1;
  wire [31:0]dist_q0;
  wire [31:0]dist_q1;
  wire dist_we1;

  bd_0 bd_0_i
       (.ap_clk(ap_clk),
        .ap_ctrl_done(ap_ctrl_done),
        .ap_ctrl_idle(ap_ctrl_idle),
        .ap_ctrl_ready(ap_ctrl_ready),
        .ap_ctrl_start(ap_ctrl_start),
        .ap_rst(ap_rst),
        .dist_address0(dist_address0),
        .dist_address1(dist_address1),
        .dist_ce0(dist_ce0),
        .dist_ce1(dist_ce1),
        .dist_d1(dist_d1),
        .dist_q0(dist_q0),
        .dist_q1(dist_q1),
        .dist_we1(dist_we1));
endmodule
