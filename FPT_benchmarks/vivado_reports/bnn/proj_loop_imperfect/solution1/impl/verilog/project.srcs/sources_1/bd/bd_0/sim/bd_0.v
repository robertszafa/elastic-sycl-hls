//Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
//Date        : Sun Jun 25 11:54:22 2023
//Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
//Command     : generate_target bd_0.bd
//Design      : bd_0
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CORE_GENERATION_INFO = "bd_0,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=bd_0,x_ipVersion=1.00.a,x_ipLanguage=VERILOG,numBlks=1,numReposBlks=1,numNonXlnxBlks=0,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=1,numHdlrefBlks=0,numPkgbdBlks=0,bdsource=USER,synth_mode=OOC_per_IP}" *) (* HW_HANDOFF = "bd_0.hwdef" *) 
module bd_0
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
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.A_ADDRESS0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.A_ADDRESS0, LAYERED_METADATA undef" *) output [13:0]a_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.A_ADDRESS1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.A_ADDRESS1, LAYERED_METADATA undef" *) output [13:0]a_address1;
  output a_ce0;
  output a_ce1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.A_D0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.A_D0, LAYERED_METADATA undef" *) output [31:0]a_d0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.A_D1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.A_D1, LAYERED_METADATA undef" *) output [31:0]a_d1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.A_Q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.A_Q0, LAYERED_METADATA undef" *) input [31:0]a_q0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.A_Q1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.A_Q1, LAYERED_METADATA undef" *) input [31:0]a_q1;
  output a_we0;
  output a_we1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.ADDR_IN_ADDRESS0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.ADDR_IN_ADDRESS0, LAYERED_METADATA undef" *) output [13:0]addr_in_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.ADDR_IN_ADDRESS1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.ADDR_IN_ADDRESS1, LAYERED_METADATA undef" *) output [13:0]addr_in_address1;
  output addr_in_ce0;
  output addr_in_ce1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.ADDR_IN_Q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.ADDR_IN_Q0, LAYERED_METADATA undef" *) input [31:0]addr_in_q0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.ADDR_IN_Q1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.ADDR_IN_Q1, LAYERED_METADATA undef" *) input [31:0]addr_in_q1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.ADDR_OUT_ADDRESS0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.ADDR_OUT_ADDRESS0, LAYERED_METADATA undef" *) output [13:0]addr_out_address0;
  output addr_out_ce0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.ADDR_OUT_Q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.ADDR_OUT_Q0, LAYERED_METADATA undef" *) input [31:0]addr_out_q0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.AP_CLK CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.AP_CLK, ASSOCIATED_RESET ap_rst, CLK_DOMAIN bd_0_ap_clk_0, FREQ_HZ 250000000.0, INSERT_VIP 0, PHASE 0.000" *) input ap_clk;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl " *) output ap_ctrl_done;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl " *) output ap_ctrl_idle;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl " *) output ap_ctrl_ready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl " *) input ap_ctrl_start;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.AP_RST RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.AP_RST, INSERT_VIP 0, POLARITY ACTIVE_HIGH" *) input ap_rst;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.DATA_ADDRESS0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.DATA_ADDRESS0, LAYERED_METADATA undef" *) output [13:0]data_address0;
  output data_ce0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.DATA_D0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.DATA_D0, LAYERED_METADATA undef" *) output [31:0]data_d0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 DATA.DATA_Q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME DATA.DATA_Q0, LAYERED_METADATA undef" *) input [31:0]data_q0;
  output data_we0;

  wire [31:0]a_q0_0_1;
  wire [31:0]a_q1_0_1;
  wire [31:0]addr_in_q0_0_1;
  wire [31:0]addr_in_q1_0_1;
  wire [31:0]addr_out_q0_0_1;
  wire ap_clk_0_1;
  wire ap_ctrl_0_1_done;
  wire ap_ctrl_0_1_idle;
  wire ap_ctrl_0_1_ready;
  wire ap_ctrl_0_1_start;
  wire ap_rst_0_1;
  wire [31:0]data_q0_0_1;
  wire [13:0]hls_inst_a_address0;
  wire [13:0]hls_inst_a_address1;
  wire hls_inst_a_ce0;
  wire hls_inst_a_ce1;
  wire [31:0]hls_inst_a_d0;
  wire [31:0]hls_inst_a_d1;
  wire hls_inst_a_we0;
  wire hls_inst_a_we1;
  wire [13:0]hls_inst_addr_in_address0;
  wire [13:0]hls_inst_addr_in_address1;
  wire hls_inst_addr_in_ce0;
  wire hls_inst_addr_in_ce1;
  wire [13:0]hls_inst_addr_out_address0;
  wire hls_inst_addr_out_ce0;
  wire [13:0]hls_inst_data_address0;
  wire hls_inst_data_ce0;
  wire [31:0]hls_inst_data_d0;
  wire hls_inst_data_we0;

  assign a_address0[13:0] = hls_inst_a_address0;
  assign a_address1[13:0] = hls_inst_a_address1;
  assign a_ce0 = hls_inst_a_ce0;
  assign a_ce1 = hls_inst_a_ce1;
  assign a_d0[31:0] = hls_inst_a_d0;
  assign a_d1[31:0] = hls_inst_a_d1;
  assign a_q0_0_1 = a_q0[31:0];
  assign a_q1_0_1 = a_q1[31:0];
  assign a_we0 = hls_inst_a_we0;
  assign a_we1 = hls_inst_a_we1;
  assign addr_in_address0[13:0] = hls_inst_addr_in_address0;
  assign addr_in_address1[13:0] = hls_inst_addr_in_address1;
  assign addr_in_ce0 = hls_inst_addr_in_ce0;
  assign addr_in_ce1 = hls_inst_addr_in_ce1;
  assign addr_in_q0_0_1 = addr_in_q0[31:0];
  assign addr_in_q1_0_1 = addr_in_q1[31:0];
  assign addr_out_address0[13:0] = hls_inst_addr_out_address0;
  assign addr_out_ce0 = hls_inst_addr_out_ce0;
  assign addr_out_q0_0_1 = addr_out_q0[31:0];
  assign ap_clk_0_1 = ap_clk;
  assign ap_ctrl_0_1_start = ap_ctrl_start;
  assign ap_ctrl_done = ap_ctrl_0_1_done;
  assign ap_ctrl_idle = ap_ctrl_0_1_idle;
  assign ap_ctrl_ready = ap_ctrl_0_1_ready;
  assign ap_rst_0_1 = ap_rst;
  assign data_address0[13:0] = hls_inst_data_address0;
  assign data_ce0 = hls_inst_data_ce0;
  assign data_d0[31:0] = hls_inst_data_d0;
  assign data_q0_0_1 = data_q0[31:0];
  assign data_we0 = hls_inst_data_we0;
  bd_0_hls_inst_0 hls_inst
       (.a_address0(hls_inst_a_address0),
        .a_address1(hls_inst_a_address1),
        .a_ce0(hls_inst_a_ce0),
        .a_ce1(hls_inst_a_ce1),
        .a_d0(hls_inst_a_d0),
        .a_d1(hls_inst_a_d1),
        .a_q0(a_q0_0_1),
        .a_q1(a_q1_0_1),
        .a_we0(hls_inst_a_we0),
        .a_we1(hls_inst_a_we1),
        .addr_in_address0(hls_inst_addr_in_address0),
        .addr_in_address1(hls_inst_addr_in_address1),
        .addr_in_ce0(hls_inst_addr_in_ce0),
        .addr_in_ce1(hls_inst_addr_in_ce1),
        .addr_in_q0(addr_in_q0_0_1),
        .addr_in_q1(addr_in_q1_0_1),
        .addr_out_address0(hls_inst_addr_out_address0),
        .addr_out_ce0(hls_inst_addr_out_ce0),
        .addr_out_q0(addr_out_q0_0_1),
        .ap_clk(ap_clk_0_1),
        .ap_done(ap_ctrl_0_1_done),
        .ap_idle(ap_ctrl_0_1_idle),
        .ap_ready(ap_ctrl_0_1_ready),
        .ap_rst(ap_rst_0_1),
        .ap_start(ap_ctrl_0_1_start),
        .data_address0(hls_inst_data_address0),
        .data_ce0(hls_inst_data_ce0),
        .data_d0(hls_inst_data_d0),
        .data_q0(data_q0_0_1),
        .data_we0(hls_inst_data_we0));
endmodule
