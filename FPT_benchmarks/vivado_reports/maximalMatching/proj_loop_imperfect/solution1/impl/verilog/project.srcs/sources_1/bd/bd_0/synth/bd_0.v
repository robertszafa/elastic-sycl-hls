//Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
//Date        : Fri Jun 23 11:55:50 2023
//Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
//Command     : generate_target bd_0.bd
//Design      : bd_0
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CORE_GENERATION_INFO = "bd_0,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=bd_0,x_ipVersion=1.00.a,x_ipLanguage=VERILOG,numBlks=1,numReposBlks=1,numNonXlnxBlks=0,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=1,numHdlrefBlks=0,numPkgbdBlks=0,bdsource=USER,synth_mode=OOC_per_IP}" *) (* HW_HANDOFF = "bd_0.hwdef" *) 
module bd_0
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
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.AP_CLK CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.AP_CLK, ASSOCIATED_RESET ap_rst, CLK_DOMAIN bd_0_ap_clk_0, FREQ_HZ 250000000.0, INSERT_VIP 0, PHASE 0.000" *) input ap_clk;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl " *) output ap_ctrl_done;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl " *) output ap_ctrl_idle;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl " *) output ap_ctrl_ready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl " *) input ap_ctrl_start;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.AP_RST RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.AP_RST, INSERT_VIP 0, POLARITY ACTIVE_HIGH" *) input ap_rst;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.EDGES_ADDRESS0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.EDGES_ADDRESS0, LAYERED_METADATA undef" *) output [10:0]edges_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.EDGES_ADDRESS1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.EDGES_ADDRESS1, LAYERED_METADATA undef" *) output [10:0]edges_address1;
  output edges_ce0;
  output edges_ce1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.EDGES_Q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.EDGES_Q0, LAYERED_METADATA undef" *) input [31:0]edges_q0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.EDGES_Q1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.EDGES_Q1, LAYERED_METADATA undef" *) input [31:0]edges_q1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.VERTICES_ADDRESS0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.VERTICES_ADDRESS0, LAYERED_METADATA undef" *) output [10:0]vertices_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.VERTICES_ADDRESS1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.VERTICES_ADDRESS1, LAYERED_METADATA undef" *) output [10:0]vertices_address1;
  output vertices_ce0;
  output vertices_ce1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.VERTICES_D0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.VERTICES_D0, LAYERED_METADATA undef" *) output [31:0]vertices_d0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.VERTICES_D1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.VERTICES_D1, LAYERED_METADATA undef" *) output [31:0]vertices_d1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.VERTICES_Q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.VERTICES_Q0, LAYERED_METADATA undef" *) input [31:0]vertices_q0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.VERTICES_Q1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.VERTICES_Q1, LAYERED_METADATA undef" *) input [31:0]vertices_q1;
  output vertices_we0;
  output vertices_we1;

  wire ap_clk_0_1;
  wire ap_ctrl_0_1_done;
  wire ap_ctrl_0_1_idle;
  wire ap_ctrl_0_1_ready;
  wire ap_ctrl_0_1_start;
  wire ap_rst_0_1;
  wire [31:0]edges_q0_0_1;
  wire [31:0]edges_q1_0_1;
  wire [10:0]hls_inst_edges_address0;
  wire [10:0]hls_inst_edges_address1;
  wire hls_inst_edges_ce0;
  wire hls_inst_edges_ce1;
  wire [10:0]hls_inst_vertices_address0;
  wire [10:0]hls_inst_vertices_address1;
  wire hls_inst_vertices_ce0;
  wire hls_inst_vertices_ce1;
  wire [31:0]hls_inst_vertices_d0;
  wire [31:0]hls_inst_vertices_d1;
  wire hls_inst_vertices_we0;
  wire hls_inst_vertices_we1;
  wire [31:0]vertices_q0_0_1;
  wire [31:0]vertices_q1_0_1;

  assign ap_clk_0_1 = ap_clk;
  assign ap_ctrl_0_1_start = ap_ctrl_start;
  assign ap_ctrl_done = ap_ctrl_0_1_done;
  assign ap_ctrl_idle = ap_ctrl_0_1_idle;
  assign ap_ctrl_ready = ap_ctrl_0_1_ready;
  assign ap_rst_0_1 = ap_rst;
  assign edges_address0[10:0] = hls_inst_edges_address0;
  assign edges_address1[10:0] = hls_inst_edges_address1;
  assign edges_ce0 = hls_inst_edges_ce0;
  assign edges_ce1 = hls_inst_edges_ce1;
  assign edges_q0_0_1 = edges_q0[31:0];
  assign edges_q1_0_1 = edges_q1[31:0];
  assign vertices_address0[10:0] = hls_inst_vertices_address0;
  assign vertices_address1[10:0] = hls_inst_vertices_address1;
  assign vertices_ce0 = hls_inst_vertices_ce0;
  assign vertices_ce1 = hls_inst_vertices_ce1;
  assign vertices_d0[31:0] = hls_inst_vertices_d0;
  assign vertices_d1[31:0] = hls_inst_vertices_d1;
  assign vertices_q0_0_1 = vertices_q0[31:0];
  assign vertices_q1_0_1 = vertices_q1[31:0];
  assign vertices_we0 = hls_inst_vertices_we0;
  assign vertices_we1 = hls_inst_vertices_we1;
  bd_0_hls_inst_0 hls_inst
       (.ap_clk(ap_clk_0_1),
        .ap_done(ap_ctrl_0_1_done),
        .ap_idle(ap_ctrl_0_1_idle),
        .ap_ready(ap_ctrl_0_1_ready),
        .ap_rst(ap_rst_0_1),
        .ap_start(ap_ctrl_0_1_start),
        .edges_address0(hls_inst_edges_address0),
        .edges_address1(hls_inst_edges_address1),
        .edges_ce0(hls_inst_edges_ce0),
        .edges_ce1(hls_inst_edges_ce1),
        .edges_q0(edges_q0_0_1),
        .edges_q1(edges_q1_0_1),
        .vertices_address0(hls_inst_vertices_address0),
        .vertices_address1(hls_inst_vertices_address1),
        .vertices_ce0(hls_inst_vertices_ce0),
        .vertices_ce1(hls_inst_vertices_ce1),
        .vertices_d0(hls_inst_vertices_d0),
        .vertices_d1(hls_inst_vertices_d1),
        .vertices_q0(vertices_q0_0_1),
        .vertices_q1(vertices_q1_0_1),
        .vertices_we0(hls_inst_vertices_we0),
        .vertices_we1(hls_inst_vertices_we1));
endmodule
