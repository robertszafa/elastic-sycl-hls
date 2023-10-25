//Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
//Date        : Fri Jun 23 11:55:51 2023
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
    edges_address0,
    edges_address1,
    edges_ce0,
    edges_ce1,
    edges_q0,
    edges_q1,
    vertices_address0,
    vertices_address1,
    vertices_ce0,
    vertices_ce1,
    vertices_d0,
    vertices_d1,
    vertices_q0,
    vertices_q1,
    vertices_we0,
    vertices_we1);
  input ap_clk;
  output ap_ctrl_done;
  output ap_ctrl_idle;
  output ap_ctrl_ready;
  input ap_ctrl_start;
  input ap_rst;
  output [10:0]edges_address0;
  output [10:0]edges_address1;
  output edges_ce0;
  output edges_ce1;
  input [31:0]edges_q0;
  input [31:0]edges_q1;
  output [10:0]vertices_address0;
  output [10:0]vertices_address1;
  output vertices_ce0;
  output vertices_ce1;
  output [31:0]vertices_d0;
  output [31:0]vertices_d1;
  input [31:0]vertices_q0;
  input [31:0]vertices_q1;
  output vertices_we0;
  output vertices_we1;

  wire ap_clk;
  wire ap_ctrl_done;
  wire ap_ctrl_idle;
  wire ap_ctrl_ready;
  wire ap_ctrl_start;
  wire ap_rst;
  wire [10:0]edges_address0;
  wire [10:0]edges_address1;
  wire edges_ce0;
  wire edges_ce1;
  wire [31:0]edges_q0;
  wire [31:0]edges_q1;
  wire [10:0]vertices_address0;
  wire [10:0]vertices_address1;
  wire vertices_ce0;
  wire vertices_ce1;
  wire [31:0]vertices_d0;
  wire [31:0]vertices_d1;
  wire [31:0]vertices_q0;
  wire [31:0]vertices_q1;
  wire vertices_we0;
  wire vertices_we1;

  bd_0 bd_0_i
       (.ap_clk(ap_clk),
        .ap_ctrl_done(ap_ctrl_done),
        .ap_ctrl_idle(ap_ctrl_idle),
        .ap_ctrl_ready(ap_ctrl_ready),
        .ap_ctrl_start(ap_ctrl_start),
        .ap_rst(ap_rst),
        .edges_address0(edges_address0),
        .edges_address1(edges_address1),
        .edges_ce0(edges_ce0),
        .edges_ce1(edges_ce1),
        .edges_q0(edges_q0),
        .edges_q1(edges_q1),
        .vertices_address0(vertices_address0),
        .vertices_address1(vertices_address1),
        .vertices_ce0(vertices_ce0),
        .vertices_ce1(vertices_ce1),
        .vertices_d0(vertices_d0),
        .vertices_d1(vertices_d1),
        .vertices_q0(vertices_q0),
        .vertices_q1(vertices_q1),
        .vertices_we0(vertices_we0),
        .vertices_we1(vertices_we1));
endmodule
