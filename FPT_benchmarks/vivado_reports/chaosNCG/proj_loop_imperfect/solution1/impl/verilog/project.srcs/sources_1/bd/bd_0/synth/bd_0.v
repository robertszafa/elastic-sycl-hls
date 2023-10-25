//Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
//Date        : Fri Jun 23 13:44:43 2023
//Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
//Command     : generate_target bd_0.bd
//Design      : bd_0
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CORE_GENERATION_INFO = "bd_0,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=bd_0,x_ipVersion=1.00.a,x_ipLanguage=VERILOG,numBlks=1,numReposBlks=1,numNonXlnxBlks=0,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=1,numHdlrefBlks=0,numPkgbdBlks=0,bdsource=USER,synth_mode=OOC_per_IP}" *) (* HW_HANDOFF = "bd_0.hwdef" *) 
module bd_0
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
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.M_ADDRESS0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.M_ADDRESS0, LAYERED_METADATA undef" *) output [11:0]M_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.M_ADDRESS1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.M_ADDRESS1, LAYERED_METADATA undef" *) output [11:0]M_address1;
  output M_ce0;
  output M_ce1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.M_Q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.M_Q0, LAYERED_METADATA undef" *) input [31:0]M_q0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.M_Q1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.M_Q1, LAYERED_METADATA undef" *) input [31:0]M_q1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.AP_CLK CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.AP_CLK, ASSOCIATED_RESET ap_rst, CLK_DOMAIN bd_0_ap_clk_0, FREQ_HZ 250000000.0, INSERT_VIP 0, PHASE 0.000" *) input ap_clk;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl " *) output ap_ctrl_done;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl " *) output ap_ctrl_idle;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl " *) output ap_ctrl_ready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl " *) input ap_ctrl_start;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.AP_RST RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.AP_RST, INSERT_VIP 0, POLARITY ACTIVE_HIGH" *) input ap_rst;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.BUFFER_R_ADDRESS0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.BUFFER_R_ADDRESS0, LAYERED_METADATA undef" *) output [11:0]buffer_r_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.BUFFER_R_ADDRESS1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.BUFFER_R_ADDRESS1, LAYERED_METADATA undef" *) output [11:0]buffer_r_address1;
  output buffer_r_ce0;
  output buffer_r_ce1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.BUFFER_R_D0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.BUFFER_R_D0, LAYERED_METADATA undef" *) output [31:0]buffer_r_d0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.BUFFER_R_D1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.BUFFER_R_D1, LAYERED_METADATA undef" *) output [31:0]buffer_r_d1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.BUFFER_R_Q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.BUFFER_R_Q0, LAYERED_METADATA undef" *) input [31:0]buffer_r_q0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.BUFFER_R_Q1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.BUFFER_R_Q1, LAYERED_METADATA undef" *) input [31:0]buffer_r_q1;
  output buffer_r_we0;
  output buffer_r_we1;

  wire [31:0]M_q0_0_1;
  wire [31:0]M_q1_0_1;
  wire ap_clk_0_1;
  wire ap_ctrl_0_1_done;
  wire ap_ctrl_0_1_idle;
  wire ap_ctrl_0_1_ready;
  wire ap_ctrl_0_1_start;
  wire ap_rst_0_1;
  wire [31:0]buffer_r_q0_0_1;
  wire [31:0]buffer_r_q1_0_1;
  wire [11:0]hls_inst_M_address0;
  wire [11:0]hls_inst_M_address1;
  wire hls_inst_M_ce0;
  wire hls_inst_M_ce1;
  wire [11:0]hls_inst_buffer_r_address0;
  wire [11:0]hls_inst_buffer_r_address1;
  wire hls_inst_buffer_r_ce0;
  wire hls_inst_buffer_r_ce1;
  wire [31:0]hls_inst_buffer_r_d0;
  wire [31:0]hls_inst_buffer_r_d1;
  wire hls_inst_buffer_r_we0;
  wire hls_inst_buffer_r_we1;

  assign M_address0[11:0] = hls_inst_M_address0;
  assign M_address1[11:0] = hls_inst_M_address1;
  assign M_ce0 = hls_inst_M_ce0;
  assign M_ce1 = hls_inst_M_ce1;
  assign M_q0_0_1 = M_q0[31:0];
  assign M_q1_0_1 = M_q1[31:0];
  assign ap_clk_0_1 = ap_clk;
  assign ap_ctrl_0_1_start = ap_ctrl_start;
  assign ap_ctrl_done = ap_ctrl_0_1_done;
  assign ap_ctrl_idle = ap_ctrl_0_1_idle;
  assign ap_ctrl_ready = ap_ctrl_0_1_ready;
  assign ap_rst_0_1 = ap_rst;
  assign buffer_r_address0[11:0] = hls_inst_buffer_r_address0;
  assign buffer_r_address1[11:0] = hls_inst_buffer_r_address1;
  assign buffer_r_ce0 = hls_inst_buffer_r_ce0;
  assign buffer_r_ce1 = hls_inst_buffer_r_ce1;
  assign buffer_r_d0[31:0] = hls_inst_buffer_r_d0;
  assign buffer_r_d1[31:0] = hls_inst_buffer_r_d1;
  assign buffer_r_q0_0_1 = buffer_r_q0[31:0];
  assign buffer_r_q1_0_1 = buffer_r_q1[31:0];
  assign buffer_r_we0 = hls_inst_buffer_r_we0;
  assign buffer_r_we1 = hls_inst_buffer_r_we1;
  bd_0_hls_inst_0 hls_inst
       (.M_address0(hls_inst_M_address0),
        .M_address1(hls_inst_M_address1),
        .M_ce0(hls_inst_M_ce0),
        .M_ce1(hls_inst_M_ce1),
        .M_q0(M_q0_0_1),
        .M_q1(M_q1_0_1),
        .ap_clk(ap_clk_0_1),
        .ap_done(ap_ctrl_0_1_done),
        .ap_idle(ap_ctrl_0_1_idle),
        .ap_ready(ap_ctrl_0_1_ready),
        .ap_rst(ap_rst_0_1),
        .ap_start(ap_ctrl_0_1_start),
        .buffer_r_address0(hls_inst_buffer_r_address0),
        .buffer_r_address1(hls_inst_buffer_r_address1),
        .buffer_r_ce0(hls_inst_buffer_r_ce0),
        .buffer_r_ce1(hls_inst_buffer_r_ce1),
        .buffer_r_d0(hls_inst_buffer_r_d0),
        .buffer_r_d1(hls_inst_buffer_r_d1),
        .buffer_r_q0(buffer_r_q0_0_1),
        .buffer_r_q1(buffer_r_q1_0_1),
        .buffer_r_we0(hls_inst_buffer_r_we0),
        .buffer_r_we1(hls_inst_buffer_r_we1));
endmodule
