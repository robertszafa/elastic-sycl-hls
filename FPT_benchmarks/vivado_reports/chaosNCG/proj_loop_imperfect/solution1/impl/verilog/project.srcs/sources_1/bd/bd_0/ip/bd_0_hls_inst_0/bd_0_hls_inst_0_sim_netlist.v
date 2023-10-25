// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
// Date        : Fri Jun 23 13:46:46 2023
// Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
// Command     : write_verilog -force -mode funcsim
//               /home/dynamatic/chaosNCG/proj_loop_imperfect/solution1/impl/verilog/project.srcs/sources_1/bd/bd_0/ip/bd_0_hls_inst_0/bd_0_hls_inst_0_sim_netlist.v
// Design      : bd_0_hls_inst_0
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7k160tfbg484-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CHECK_LICENSE_TYPE = "bd_0_hls_inst_0,loop_imperfect,{}" *) (* DowngradeIPIdentifiedWarnings = "yes" *) (* IP_DEFINITION_SOURCE = "HLS" *) 
(* X_CORE_INFO = "loop_imperfect,Vivado 2019.2" *) (* hls_module = "yes" *) 
(* NotValidForBitStream *)
module bd_0_hls_inst_0
   (buffer_r_ce0,
    buffer_r_we0,
    buffer_r_ce1,
    buffer_r_we1,
    M_ce0,
    M_ce1,
    ap_clk,
    ap_rst,
    ap_start,
    ap_done,
    ap_idle,
    ap_ready,
    buffer_r_address0,
    buffer_r_d0,
    buffer_r_q0,
    buffer_r_address1,
    buffer_r_d1,
    buffer_r_q1,
    M_address0,
    M_q0,
    M_address1,
    M_q1);
  output buffer_r_ce0;
  output buffer_r_we0;
  output buffer_r_ce1;
  output buffer_r_we1;
  output M_ce0;
  output M_ce1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 ap_clk CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ap_clk, ASSOCIATED_RESET ap_rst, FREQ_HZ 250000000.0, PHASE 0.000, CLK_DOMAIN bd_0_ap_clk_0, INSERT_VIP 0" *) input ap_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 ap_rst RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ap_rst, POLARITY ACTIVE_HIGH, INSERT_VIP 0" *) input ap_rst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl start" *) input ap_start;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl done" *) output ap_done;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl idle" *) output ap_idle;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl ready" *) output ap_ready;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 buffer_r_address0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME buffer_r_address0, LAYERED_METADATA undef" *) output [11:0]buffer_r_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 buffer_r_d0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME buffer_r_d0, LAYERED_METADATA undef" *) output [31:0]buffer_r_d0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 buffer_r_q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME buffer_r_q0, LAYERED_METADATA undef" *) input [31:0]buffer_r_q0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 buffer_r_address1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME buffer_r_address1, LAYERED_METADATA undef" *) output [11:0]buffer_r_address1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 buffer_r_d1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME buffer_r_d1, LAYERED_METADATA undef" *) output [31:0]buffer_r_d1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 buffer_r_q1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME buffer_r_q1, LAYERED_METADATA undef" *) input [31:0]buffer_r_q1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 M_address0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M_address0, LAYERED_METADATA undef" *) output [11:0]M_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 M_q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M_q0, LAYERED_METADATA undef" *) input [31:0]M_q0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 M_address1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M_address1, LAYERED_METADATA undef" *) output [11:0]M_address1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 M_q1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME M_q1, LAYERED_METADATA undef" *) input [31:0]M_q1;

  wire [11:0]M_address0;
  wire [11:0]M_address1;
  wire M_ce0;
  wire M_ce1;
  wire [31:0]M_q0;
  wire [31:0]M_q1;
  wire ap_clk;
  wire ap_done;
  wire ap_idle;
  wire ap_ready;
  wire ap_rst;
  wire ap_start;
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

  (* ap_ST_fsm_state1 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000000000001" *) 
  (* ap_ST_fsm_state10 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000001000000000" *) 
  (* ap_ST_fsm_state11 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000010000000000" *) 
  (* ap_ST_fsm_state12 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000100000000000" *) 
  (* ap_ST_fsm_state13 = "83'b00000000000000000000000000000000000000000000000000000000000000000000001000000000000" *) 
  (* ap_ST_fsm_state14 = "83'b00000000000000000000000000000000000000000000000000000000000000000000010000000000000" *) 
  (* ap_ST_fsm_state15 = "83'b00000000000000000000000000000000000000000000000000000000000000000000100000000000000" *) 
  (* ap_ST_fsm_state16 = "83'b00000000000000000000000000000000000000000000000000000000000000000001000000000000000" *) 
  (* ap_ST_fsm_state17 = "83'b00000000000000000000000000000000000000000000000000000000000000000010000000000000000" *) 
  (* ap_ST_fsm_state18 = "83'b00000000000000000000000000000000000000000000000000000000000000000100000000000000000" *) 
  (* ap_ST_fsm_state19 = "83'b00000000000000000000000000000000000000000000000000000000000000001000000000000000000" *) 
  (* ap_ST_fsm_state2 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000000000010" *) 
  (* ap_ST_fsm_state20 = "83'b00000000000000000000000000000000000000000000000000000000000000010000000000000000000" *) 
  (* ap_ST_fsm_state21 = "83'b00000000000000000000000000000000000000000000000000000000000000100000000000000000000" *) 
  (* ap_ST_fsm_state22 = "83'b00000000000000000000000000000000000000000000000000000000000001000000000000000000000" *) 
  (* ap_ST_fsm_state23 = "83'b00000000000000000000000000000000000000000000000000000000000010000000000000000000000" *) 
  (* ap_ST_fsm_state24 = "83'b00000000000000000000000000000000000000000000000000000000000100000000000000000000000" *) 
  (* ap_ST_fsm_state25 = "83'b00000000000000000000000000000000000000000000000000000000001000000000000000000000000" *) 
  (* ap_ST_fsm_state26 = "83'b00000000000000000000000000000000000000000000000000000000010000000000000000000000000" *) 
  (* ap_ST_fsm_state27 = "83'b00000000000000000000000000000000000000000000000000000000100000000000000000000000000" *) 
  (* ap_ST_fsm_state28 = "83'b00000000000000000000000000000000000000000000000000000001000000000000000000000000000" *) 
  (* ap_ST_fsm_state29 = "83'b00000000000000000000000000000000000000000000000000000010000000000000000000000000000" *) 
  (* ap_ST_fsm_state3 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000000000100" *) 
  (* ap_ST_fsm_state30 = "83'b00000000000000000000000000000000000000000000000000000100000000000000000000000000000" *) 
  (* ap_ST_fsm_state31 = "83'b00000000000000000000000000000000000000000000000000001000000000000000000000000000000" *) 
  (* ap_ST_fsm_state32 = "83'b00000000000000000000000000000000000000000000000000010000000000000000000000000000000" *) 
  (* ap_ST_fsm_state33 = "83'b00000000000000000000000000000000000000000000000000100000000000000000000000000000000" *) 
  (* ap_ST_fsm_state34 = "83'b00000000000000000000000000000000000000000000000001000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state35 = "83'b00000000000000000000000000000000000000000000000010000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state36 = "83'b00000000000000000000000000000000000000000000000100000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state37 = "83'b00000000000000000000000000000000000000000000001000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state38 = "83'b00000000000000000000000000000000000000000000010000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state39 = "83'b00000000000000000000000000000000000000000000100000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state4 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000000001000" *) 
  (* ap_ST_fsm_state40 = "83'b00000000000000000000000000000000000000000001000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state41 = "83'b00000000000000000000000000000000000000000010000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state42 = "83'b00000000000000000000000000000000000000000100000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state43 = "83'b00000000000000000000000000000000000000001000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state44 = "83'b00000000000000000000000000000000000000010000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state45 = "83'b00000000000000000000000000000000000000100000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state46 = "83'b00000000000000000000000000000000000001000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state47 = "83'b00000000000000000000000000000000000010000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state48 = "83'b00000000000000000000000000000000000100000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state49 = "83'b00000000000000000000000000000000001000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state5 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000000010000" *) 
  (* ap_ST_fsm_state50 = "83'b00000000000000000000000000000000010000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state51 = "83'b00000000000000000000000000000000100000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state52 = "83'b00000000000000000000000000000001000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state53 = "83'b00000000000000000000000000000010000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state54 = "83'b00000000000000000000000000000100000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state55 = "83'b00000000000000000000000000001000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state56 = "83'b00000000000000000000000000010000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state57 = "83'b00000000000000000000000000100000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state58 = "83'b00000000000000000000000001000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state59 = "83'b00000000000000000000000010000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state6 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000000100000" *) 
  (* ap_ST_fsm_state60 = "83'b00000000000000000000000100000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state61 = "83'b00000000000000000000001000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state62 = "83'b00000000000000000000010000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state63 = "83'b00000000000000000000100000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state64 = "83'b00000000000000000001000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state65 = "83'b00000000000000000010000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state66 = "83'b00000000000000000100000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state67 = "83'b00000000000000001000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state68 = "83'b00000000000000010000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state69 = "83'b00000000000000100000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state7 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000001000000" *) 
  (* ap_ST_fsm_state70 = "83'b00000000000001000000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state71 = "83'b00000000000010000000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state72 = "83'b00000000000100000000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state73 = "83'b00000000001000000000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state74 = "83'b00000000010000000000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state75 = "83'b00000000100000000000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state76 = "83'b00000001000000000000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state77 = "83'b00000010000000000000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state78 = "83'b00000100000000000000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state79 = "83'b00001000000000000000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state8 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000010000000" *) 
  (* ap_ST_fsm_state80 = "83'b00010000000000000000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state81 = "83'b00100000000000000000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state82 = "83'b01000000000000000000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state83 = "83'b10000000000000000000000000000000000000000000000000000000000000000000000000000000000" *) 
  (* ap_ST_fsm_state9 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000100000000" *) 
  bd_0_hls_inst_0_loop_imperfect inst
       (.M_address0(M_address0),
        .M_address1(M_address1),
        .M_ce0(M_ce0),
        .M_ce1(M_ce1),
        .M_q0(M_q0),
        .M_q1(M_q1),
        .ap_clk(ap_clk),
        .ap_done(ap_done),
        .ap_idle(ap_idle),
        .ap_ready(ap_ready),
        .ap_rst(ap_rst),
        .ap_start(ap_start),
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

(* ORIG_REF_NAME = "loop_imperfect" *) (* ap_ST_fsm_state1 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000000000001" *) (* ap_ST_fsm_state10 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000001000000000" *) 
(* ap_ST_fsm_state11 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000010000000000" *) (* ap_ST_fsm_state12 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000100000000000" *) (* ap_ST_fsm_state13 = "83'b00000000000000000000000000000000000000000000000000000000000000000000001000000000000" *) 
(* ap_ST_fsm_state14 = "83'b00000000000000000000000000000000000000000000000000000000000000000000010000000000000" *) (* ap_ST_fsm_state15 = "83'b00000000000000000000000000000000000000000000000000000000000000000000100000000000000" *) (* ap_ST_fsm_state16 = "83'b00000000000000000000000000000000000000000000000000000000000000000001000000000000000" *) 
(* ap_ST_fsm_state17 = "83'b00000000000000000000000000000000000000000000000000000000000000000010000000000000000" *) (* ap_ST_fsm_state18 = "83'b00000000000000000000000000000000000000000000000000000000000000000100000000000000000" *) (* ap_ST_fsm_state19 = "83'b00000000000000000000000000000000000000000000000000000000000000001000000000000000000" *) 
(* ap_ST_fsm_state2 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000000000010" *) (* ap_ST_fsm_state20 = "83'b00000000000000000000000000000000000000000000000000000000000000010000000000000000000" *) (* ap_ST_fsm_state21 = "83'b00000000000000000000000000000000000000000000000000000000000000100000000000000000000" *) 
(* ap_ST_fsm_state22 = "83'b00000000000000000000000000000000000000000000000000000000000001000000000000000000000" *) (* ap_ST_fsm_state23 = "83'b00000000000000000000000000000000000000000000000000000000000010000000000000000000000" *) (* ap_ST_fsm_state24 = "83'b00000000000000000000000000000000000000000000000000000000000100000000000000000000000" *) 
(* ap_ST_fsm_state25 = "83'b00000000000000000000000000000000000000000000000000000000001000000000000000000000000" *) (* ap_ST_fsm_state26 = "83'b00000000000000000000000000000000000000000000000000000000010000000000000000000000000" *) (* ap_ST_fsm_state27 = "83'b00000000000000000000000000000000000000000000000000000000100000000000000000000000000" *) 
(* ap_ST_fsm_state28 = "83'b00000000000000000000000000000000000000000000000000000001000000000000000000000000000" *) (* ap_ST_fsm_state29 = "83'b00000000000000000000000000000000000000000000000000000010000000000000000000000000000" *) (* ap_ST_fsm_state3 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000000000100" *) 
(* ap_ST_fsm_state30 = "83'b00000000000000000000000000000000000000000000000000000100000000000000000000000000000" *) (* ap_ST_fsm_state31 = "83'b00000000000000000000000000000000000000000000000000001000000000000000000000000000000" *) (* ap_ST_fsm_state32 = "83'b00000000000000000000000000000000000000000000000000010000000000000000000000000000000" *) 
(* ap_ST_fsm_state33 = "83'b00000000000000000000000000000000000000000000000000100000000000000000000000000000000" *) (* ap_ST_fsm_state34 = "83'b00000000000000000000000000000000000000000000000001000000000000000000000000000000000" *) (* ap_ST_fsm_state35 = "83'b00000000000000000000000000000000000000000000000010000000000000000000000000000000000" *) 
(* ap_ST_fsm_state36 = "83'b00000000000000000000000000000000000000000000000100000000000000000000000000000000000" *) (* ap_ST_fsm_state37 = "83'b00000000000000000000000000000000000000000000001000000000000000000000000000000000000" *) (* ap_ST_fsm_state38 = "83'b00000000000000000000000000000000000000000000010000000000000000000000000000000000000" *) 
(* ap_ST_fsm_state39 = "83'b00000000000000000000000000000000000000000000100000000000000000000000000000000000000" *) (* ap_ST_fsm_state4 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000000001000" *) (* ap_ST_fsm_state40 = "83'b00000000000000000000000000000000000000000001000000000000000000000000000000000000000" *) 
(* ap_ST_fsm_state41 = "83'b00000000000000000000000000000000000000000010000000000000000000000000000000000000000" *) (* ap_ST_fsm_state42 = "83'b00000000000000000000000000000000000000000100000000000000000000000000000000000000000" *) (* ap_ST_fsm_state43 = "83'b00000000000000000000000000000000000000001000000000000000000000000000000000000000000" *) 
(* ap_ST_fsm_state44 = "83'b00000000000000000000000000000000000000010000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state45 = "83'b00000000000000000000000000000000000000100000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state46 = "83'b00000000000000000000000000000000000001000000000000000000000000000000000000000000000" *) 
(* ap_ST_fsm_state47 = "83'b00000000000000000000000000000000000010000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state48 = "83'b00000000000000000000000000000000000100000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state49 = "83'b00000000000000000000000000000000001000000000000000000000000000000000000000000000000" *) 
(* ap_ST_fsm_state5 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000000010000" *) (* ap_ST_fsm_state50 = "83'b00000000000000000000000000000000010000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state51 = "83'b00000000000000000000000000000000100000000000000000000000000000000000000000000000000" *) 
(* ap_ST_fsm_state52 = "83'b00000000000000000000000000000001000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state53 = "83'b00000000000000000000000000000010000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state54 = "83'b00000000000000000000000000000100000000000000000000000000000000000000000000000000000" *) 
(* ap_ST_fsm_state55 = "83'b00000000000000000000000000001000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state56 = "83'b00000000000000000000000000010000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state57 = "83'b00000000000000000000000000100000000000000000000000000000000000000000000000000000000" *) 
(* ap_ST_fsm_state58 = "83'b00000000000000000000000001000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state59 = "83'b00000000000000000000000010000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state6 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000000100000" *) 
(* ap_ST_fsm_state60 = "83'b00000000000000000000000100000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state61 = "83'b00000000000000000000001000000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state62 = "83'b00000000000000000000010000000000000000000000000000000000000000000000000000000000000" *) 
(* ap_ST_fsm_state63 = "83'b00000000000000000000100000000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state64 = "83'b00000000000000000001000000000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state65 = "83'b00000000000000000010000000000000000000000000000000000000000000000000000000000000000" *) 
(* ap_ST_fsm_state66 = "83'b00000000000000000100000000000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state67 = "83'b00000000000000001000000000000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state68 = "83'b00000000000000010000000000000000000000000000000000000000000000000000000000000000000" *) 
(* ap_ST_fsm_state69 = "83'b00000000000000100000000000000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state7 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000001000000" *) (* ap_ST_fsm_state70 = "83'b00000000000001000000000000000000000000000000000000000000000000000000000000000000000" *) 
(* ap_ST_fsm_state71 = "83'b00000000000010000000000000000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state72 = "83'b00000000000100000000000000000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state73 = "83'b00000000001000000000000000000000000000000000000000000000000000000000000000000000000" *) 
(* ap_ST_fsm_state74 = "83'b00000000010000000000000000000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state75 = "83'b00000000100000000000000000000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state76 = "83'b00000001000000000000000000000000000000000000000000000000000000000000000000000000000" *) 
(* ap_ST_fsm_state77 = "83'b00000010000000000000000000000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state78 = "83'b00000100000000000000000000000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state79 = "83'b00001000000000000000000000000000000000000000000000000000000000000000000000000000000" *) 
(* ap_ST_fsm_state8 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000010000000" *) (* ap_ST_fsm_state80 = "83'b00010000000000000000000000000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state81 = "83'b00100000000000000000000000000000000000000000000000000000000000000000000000000000000" *) 
(* ap_ST_fsm_state82 = "83'b01000000000000000000000000000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state83 = "83'b10000000000000000000000000000000000000000000000000000000000000000000000000000000000" *) (* ap_ST_fsm_state9 = "83'b00000000000000000000000000000000000000000000000000000000000000000000000000100000000" *) 
(* hls_module = "yes" *) 
module bd_0_hls_inst_0_loop_imperfect
   (ap_clk,
    ap_rst,
    ap_start,
    ap_done,
    ap_idle,
    ap_ready,
    buffer_r_address0,
    buffer_r_ce0,
    buffer_r_we0,
    buffer_r_d0,
    buffer_r_q0,
    buffer_r_address1,
    buffer_r_ce1,
    buffer_r_we1,
    buffer_r_d1,
    buffer_r_q1,
    M_address0,
    M_ce0,
    M_q0,
    M_address1,
    M_ce1,
    M_q1);
  input ap_clk;
  input ap_rst;
  input ap_start;
  output ap_done;
  output ap_idle;
  output ap_ready;
  output [11:0]buffer_r_address0;
  output buffer_r_ce0;
  output buffer_r_we0;
  output [31:0]buffer_r_d0;
  input [31:0]buffer_r_q0;
  output [11:0]buffer_r_address1;
  output buffer_r_ce1;
  output buffer_r_we1;
  output [31:0]buffer_r_d1;
  input [31:0]buffer_r_q1;
  output [11:0]M_address0;
  output M_ce0;
  input [31:0]M_q0;
  output [11:0]M_address1;
  output M_ce1;
  input [31:0]M_q1;

  wire \<const0> ;
  wire [10:0]\^M_address0 ;
  wire [10:0]\^M_address1 ;
  wire M_ce0;
  wire M_ce1;
  wire [31:0]M_q0;
  wire [31:0]M_q1;
  wire [10:2]add_ln101_fu_134_p2;
  wire \add_ln101_reg_305[10]_i_3_n_1 ;
  wire \ap_CS_fsm[0]_i_10_n_1 ;
  wire \ap_CS_fsm[0]_i_11_n_1 ;
  wire \ap_CS_fsm[0]_i_12_n_1 ;
  wire \ap_CS_fsm[0]_i_13_n_1 ;
  wire \ap_CS_fsm[0]_i_14_n_1 ;
  wire \ap_CS_fsm[0]_i_15_n_1 ;
  wire \ap_CS_fsm[0]_i_16_n_1 ;
  wire \ap_CS_fsm[0]_i_17_n_1 ;
  wire \ap_CS_fsm[0]_i_18_n_1 ;
  wire \ap_CS_fsm[0]_i_2_n_1 ;
  wire \ap_CS_fsm[0]_i_3_n_1 ;
  wire \ap_CS_fsm[0]_i_4_n_1 ;
  wire \ap_CS_fsm[0]_i_5_n_1 ;
  wire \ap_CS_fsm[0]_i_6_n_1 ;
  wire \ap_CS_fsm[0]_i_7_n_1 ;
  wire \ap_CS_fsm[0]_i_8_n_1 ;
  wire \ap_CS_fsm[0]_i_9_n_1 ;
  wire \ap_CS_fsm_reg_n_1_[0] ;
  wire \ap_CS_fsm_reg_n_1_[10] ;
  wire \ap_CS_fsm_reg_n_1_[11] ;
  wire \ap_CS_fsm_reg_n_1_[12] ;
  wire \ap_CS_fsm_reg_n_1_[13] ;
  wire \ap_CS_fsm_reg_n_1_[14] ;
  wire \ap_CS_fsm_reg_n_1_[15] ;
  wire \ap_CS_fsm_reg_n_1_[16] ;
  wire \ap_CS_fsm_reg_n_1_[17] ;
  wire \ap_CS_fsm_reg_n_1_[18] ;
  wire \ap_CS_fsm_reg_n_1_[19] ;
  wire \ap_CS_fsm_reg_n_1_[20] ;
  wire \ap_CS_fsm_reg_n_1_[21] ;
  wire \ap_CS_fsm_reg_n_1_[22] ;
  wire \ap_CS_fsm_reg_n_1_[23] ;
  wire \ap_CS_fsm_reg_n_1_[24] ;
  wire \ap_CS_fsm_reg_n_1_[25] ;
  wire \ap_CS_fsm_reg_n_1_[26] ;
  wire \ap_CS_fsm_reg_n_1_[27] ;
  wire \ap_CS_fsm_reg_n_1_[28] ;
  wire \ap_CS_fsm_reg_n_1_[29] ;
  wire \ap_CS_fsm_reg_n_1_[30] ;
  wire \ap_CS_fsm_reg_n_1_[31] ;
  wire \ap_CS_fsm_reg_n_1_[32] ;
  wire \ap_CS_fsm_reg_n_1_[33] ;
  wire \ap_CS_fsm_reg_n_1_[34] ;
  wire \ap_CS_fsm_reg_n_1_[35] ;
  wire \ap_CS_fsm_reg_n_1_[36] ;
  wire \ap_CS_fsm_reg_n_1_[37] ;
  wire \ap_CS_fsm_reg_n_1_[38] ;
  wire \ap_CS_fsm_reg_n_1_[39] ;
  wire \ap_CS_fsm_reg_n_1_[40] ;
  wire \ap_CS_fsm_reg_n_1_[45] ;
  wire \ap_CS_fsm_reg_n_1_[46] ;
  wire \ap_CS_fsm_reg_n_1_[47] ;
  wire \ap_CS_fsm_reg_n_1_[48] ;
  wire \ap_CS_fsm_reg_n_1_[49] ;
  wire \ap_CS_fsm_reg_n_1_[50] ;
  wire \ap_CS_fsm_reg_n_1_[51] ;
  wire \ap_CS_fsm_reg_n_1_[52] ;
  wire \ap_CS_fsm_reg_n_1_[53] ;
  wire \ap_CS_fsm_reg_n_1_[54] ;
  wire \ap_CS_fsm_reg_n_1_[55] ;
  wire \ap_CS_fsm_reg_n_1_[56] ;
  wire \ap_CS_fsm_reg_n_1_[57] ;
  wire \ap_CS_fsm_reg_n_1_[58] ;
  wire \ap_CS_fsm_reg_n_1_[59] ;
  wire \ap_CS_fsm_reg_n_1_[60] ;
  wire \ap_CS_fsm_reg_n_1_[61] ;
  wire \ap_CS_fsm_reg_n_1_[62] ;
  wire \ap_CS_fsm_reg_n_1_[63] ;
  wire \ap_CS_fsm_reg_n_1_[64] ;
  wire \ap_CS_fsm_reg_n_1_[65] ;
  wire \ap_CS_fsm_reg_n_1_[66] ;
  wire \ap_CS_fsm_reg_n_1_[67] ;
  wire \ap_CS_fsm_reg_n_1_[68] ;
  wire \ap_CS_fsm_reg_n_1_[69] ;
  wire \ap_CS_fsm_reg_n_1_[70] ;
  wire \ap_CS_fsm_reg_n_1_[71] ;
  wire \ap_CS_fsm_reg_n_1_[72] ;
  wire \ap_CS_fsm_reg_n_1_[73] ;
  wire \ap_CS_fsm_reg_n_1_[74] ;
  wire \ap_CS_fsm_reg_n_1_[75] ;
  wire \ap_CS_fsm_reg_n_1_[76] ;
  wire \ap_CS_fsm_reg_n_1_[77] ;
  wire \ap_CS_fsm_reg_n_1_[78] ;
  wire \ap_CS_fsm_reg_n_1_[7] ;
  wire \ap_CS_fsm_reg_n_1_[8] ;
  wire \ap_CS_fsm_reg_n_1_[9] ;
  wire ap_CS_fsm_state2;
  wire ap_CS_fsm_state4;
  wire ap_CS_fsm_state42;
  wire ap_CS_fsm_state43;
  wire ap_CS_fsm_state44;
  wire ap_CS_fsm_state5;
  wire ap_CS_fsm_state6;
  wire ap_CS_fsm_state80;
  wire ap_CS_fsm_state81;
  wire ap_CS_fsm_state82;
  wire [2:0]ap_NS_fsm;
  wire ap_clk;
  wire ap_idle;
  wire ap_ready;
  wire ap_ready_INST_0_i_1_n_1;
  wire ap_rst;
  wire ap_start;
  wire [31:0]b0_1_fu_159_p2;
  wire [31:0]b0_1_reg_341;
  wire [31:0]b0_2_fu_214_p2;
  wire \b0_2_reg_372[1]_i_2_n_1 ;
  wire [31:0]b0_reg_330;
  wire [31:0]b1_1_fu_219_p2;
  wire [31:0]b1_1_reg_377;
  wire [31:0]b1_2_fu_292_p2;
  wire \b1_2_reg_409[11]_i_2_n_1 ;
  wire \b1_2_reg_409[11]_i_3_n_1 ;
  wire \b1_2_reg_409[11]_i_4_n_1 ;
  wire \b1_2_reg_409[11]_i_5_n_1 ;
  wire \b1_2_reg_409[15]_i_2_n_1 ;
  wire \b1_2_reg_409[15]_i_3_n_1 ;
  wire \b1_2_reg_409[15]_i_4_n_1 ;
  wire \b1_2_reg_409[15]_i_5_n_1 ;
  wire \b1_2_reg_409[19]_i_2_n_1 ;
  wire \b1_2_reg_409[19]_i_3_n_1 ;
  wire \b1_2_reg_409[19]_i_4_n_1 ;
  wire \b1_2_reg_409[19]_i_5_n_1 ;
  wire \b1_2_reg_409[23]_i_2_n_1 ;
  wire \b1_2_reg_409[23]_i_3_n_1 ;
  wire \b1_2_reg_409[23]_i_4_n_1 ;
  wire \b1_2_reg_409[23]_i_5_n_1 ;
  wire \b1_2_reg_409[27]_i_2_n_1 ;
  wire \b1_2_reg_409[27]_i_3_n_1 ;
  wire \b1_2_reg_409[27]_i_4_n_1 ;
  wire \b1_2_reg_409[27]_i_5_n_1 ;
  wire \b1_2_reg_409[31]_i_2_n_1 ;
  wire \b1_2_reg_409[31]_i_3_n_1 ;
  wire \b1_2_reg_409[31]_i_4_n_1 ;
  wire \b1_2_reg_409[31]_i_5_n_1 ;
  wire \b1_2_reg_409[3]_i_2_n_1 ;
  wire \b1_2_reg_409[3]_i_3_n_1 ;
  wire \b1_2_reg_409[3]_i_4_n_1 ;
  wire \b1_2_reg_409[3]_i_5_n_1 ;
  wire \b1_2_reg_409[7]_i_2_n_1 ;
  wire \b1_2_reg_409[7]_i_3_n_1 ;
  wire \b1_2_reg_409[7]_i_4_n_1 ;
  wire \b1_2_reg_409[7]_i_5_n_1 ;
  wire \b1_2_reg_409_reg[11]_i_1_n_1 ;
  wire \b1_2_reg_409_reg[11]_i_1_n_2 ;
  wire \b1_2_reg_409_reg[11]_i_1_n_3 ;
  wire \b1_2_reg_409_reg[11]_i_1_n_4 ;
  wire \b1_2_reg_409_reg[15]_i_1_n_1 ;
  wire \b1_2_reg_409_reg[15]_i_1_n_2 ;
  wire \b1_2_reg_409_reg[15]_i_1_n_3 ;
  wire \b1_2_reg_409_reg[15]_i_1_n_4 ;
  wire \b1_2_reg_409_reg[19]_i_1_n_1 ;
  wire \b1_2_reg_409_reg[19]_i_1_n_2 ;
  wire \b1_2_reg_409_reg[19]_i_1_n_3 ;
  wire \b1_2_reg_409_reg[19]_i_1_n_4 ;
  wire \b1_2_reg_409_reg[23]_i_1_n_1 ;
  wire \b1_2_reg_409_reg[23]_i_1_n_2 ;
  wire \b1_2_reg_409_reg[23]_i_1_n_3 ;
  wire \b1_2_reg_409_reg[23]_i_1_n_4 ;
  wire \b1_2_reg_409_reg[27]_i_1_n_1 ;
  wire \b1_2_reg_409_reg[27]_i_1_n_2 ;
  wire \b1_2_reg_409_reg[27]_i_1_n_3 ;
  wire \b1_2_reg_409_reg[27]_i_1_n_4 ;
  wire \b1_2_reg_409_reg[31]_i_1_n_2 ;
  wire \b1_2_reg_409_reg[31]_i_1_n_3 ;
  wire \b1_2_reg_409_reg[31]_i_1_n_4 ;
  wire \b1_2_reg_409_reg[3]_i_1_n_1 ;
  wire \b1_2_reg_409_reg[3]_i_1_n_2 ;
  wire \b1_2_reg_409_reg[3]_i_1_n_3 ;
  wire \b1_2_reg_409_reg[3]_i_1_n_4 ;
  wire \b1_2_reg_409_reg[7]_i_1_n_1 ;
  wire \b1_2_reg_409_reg[7]_i_1_n_2 ;
  wire \b1_2_reg_409_reg[7]_i_1_n_3 ;
  wire \b1_2_reg_409_reg[7]_i_1_n_4 ;
  wire [31:0]b1_reg_336;
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
  wire grp_fu_163_ap_start;
  wire i_0_reg_111;
  wire \i_0_reg_111_reg_n_1_[0] ;
  wire [10:1]i_fu_249_p2;
  wire [10:0]i_reg_404;
  wire \i_reg_404[10]_i_2_n_1 ;
  wire loop_imperfect_srbkb_U1_n_1;
  wire loop_imperfect_srbkb_U2_n_1;
  wire loop_imperfect_srbkb_U2_n_10;
  wire loop_imperfect_srbkb_U2_n_11;
  wire loop_imperfect_srbkb_U2_n_12;
  wire loop_imperfect_srbkb_U2_n_13;
  wire loop_imperfect_srbkb_U2_n_14;
  wire loop_imperfect_srbkb_U2_n_15;
  wire loop_imperfect_srbkb_U2_n_16;
  wire loop_imperfect_srbkb_U2_n_17;
  wire loop_imperfect_srbkb_U2_n_18;
  wire loop_imperfect_srbkb_U2_n_19;
  wire loop_imperfect_srbkb_U2_n_2;
  wire loop_imperfect_srbkb_U2_n_20;
  wire loop_imperfect_srbkb_U2_n_21;
  wire loop_imperfect_srbkb_U2_n_22;
  wire loop_imperfect_srbkb_U2_n_23;
  wire loop_imperfect_srbkb_U2_n_24;
  wire loop_imperfect_srbkb_U2_n_25;
  wire loop_imperfect_srbkb_U2_n_26;
  wire loop_imperfect_srbkb_U2_n_27;
  wire loop_imperfect_srbkb_U2_n_28;
  wire loop_imperfect_srbkb_U2_n_29;
  wire loop_imperfect_srbkb_U2_n_3;
  wire loop_imperfect_srbkb_U2_n_30;
  wire loop_imperfect_srbkb_U2_n_31;
  wire loop_imperfect_srbkb_U2_n_32;
  wire loop_imperfect_srbkb_U2_n_4;
  wire loop_imperfect_srbkb_U2_n_5;
  wire loop_imperfect_srbkb_U2_n_6;
  wire loop_imperfect_srbkb_U2_n_7;
  wire loop_imperfect_srbkb_U2_n_8;
  wire loop_imperfect_srbkb_U2_n_9;
  wire [0:0]lshr_ln105_fu_193_p2;
  wire [31:6]or_ln_fu_207_p3;
  wire [10:10]p_0_in;
  wire \reg_123[0]_i_1_n_1 ;
  wire \reg_123[10]_i_1_n_1 ;
  wire \reg_123[11]_i_1_n_1 ;
  wire \reg_123[11]_i_2_n_1 ;
  wire \reg_123[1]_i_1_n_1 ;
  wire \reg_123[2]_i_1_n_1 ;
  wire \reg_123[3]_i_1_n_1 ;
  wire \reg_123[4]_i_1_n_1 ;
  wire \reg_123[5]_i_1_n_1 ;
  wire \reg_123[6]_i_1_n_1 ;
  wire \reg_123[7]_i_1_n_1 ;
  wire \reg_123[8]_i_1_n_1 ;
  wire \reg_123[9]_i_1_n_1 ;
  wire [31:0]remd;
  wire [5:1]shl_ln105_reg_357;
  wire \shl_ln105_reg_357[1]_i_1_n_1 ;
  wire \shl_ln105_reg_357[2]_i_1_n_1 ;
  wire \shl_ln105_reg_357[3]_i_1_n_1 ;
  wire \shl_ln105_reg_357[4]_i_1_n_1 ;
  wire \shl_ln105_reg_357[4]_i_2_n_1 ;
  wire \shl_ln105_reg_357[5]_i_10_n_1 ;
  wire \shl_ln105_reg_357[5]_i_1_n_1 ;
  wire \shl_ln105_reg_357[5]_i_2_n_1 ;
  wire \shl_ln105_reg_357[5]_i_3_n_1 ;
  wire \shl_ln105_reg_357[5]_i_4_n_1 ;
  wire \shl_ln105_reg_357[5]_i_5_n_1 ;
  wire \shl_ln105_reg_357[5]_i_6_n_1 ;
  wire \shl_ln105_reg_357[5]_i_7_n_1 ;
  wire \shl_ln105_reg_357[5]_i_8_n_1 ;
  wire \shl_ln105_reg_357[5]_i_9_n_1 ;
  wire [24:0]shl_ln108_fu_233_p2;
  wire \shl_ln108_reg_393[0]_i_2_n_1 ;
  wire \shl_ln108_reg_393[15]_i_1_n_1 ;
  wire \shl_ln108_reg_393[16]_i_2_n_1 ;
  wire \shl_ln108_reg_393[16]_i_3_n_1 ;
  wire \shl_ln108_reg_393[16]_i_4_n_1 ;
  wire \shl_ln108_reg_393[23]_i_1_n_1 ;
  wire \shl_ln108_reg_393[24]_i_2_n_1 ;
  wire \shl_ln108_reg_393[24]_i_3_n_1 ;
  wire \shl_ln108_reg_393[24]_i_4_n_1 ;
  wire \shl_ln108_reg_393[24]_i_5_n_1 ;
  wire \shl_ln108_reg_393[24]_i_6_n_1 ;
  wire \shl_ln108_reg_393[24]_i_7_n_1 ;
  wire \shl_ln108_reg_393[24]_i_8_n_1 ;
  wire \shl_ln108_reg_393[24]_i_9_n_1 ;
  wire \shl_ln108_reg_393[25]_i_1_n_1 ;
  wire \shl_ln108_reg_393[26]_i_1_n_1 ;
  wire \shl_ln108_reg_393[27]_i_1_n_1 ;
  wire \shl_ln108_reg_393[28]_i_1_n_1 ;
  wire \shl_ln108_reg_393[29]_i_1_n_1 ;
  wire \shl_ln108_reg_393[30]_i_1_n_1 ;
  wire \shl_ln108_reg_393[31]_i_1_n_1 ;
  wire \shl_ln108_reg_393[31]_i_2_n_1 ;
  wire \shl_ln108_reg_393[7]_i_1_n_1 ;
  wire \shl_ln108_reg_393_reg_n_1_[0] ;
  wire \shl_ln108_reg_393_reg_n_1_[10] ;
  wire \shl_ln108_reg_393_reg_n_1_[11] ;
  wire \shl_ln108_reg_393_reg_n_1_[12] ;
  wire \shl_ln108_reg_393_reg_n_1_[13] ;
  wire \shl_ln108_reg_393_reg_n_1_[14] ;
  wire \shl_ln108_reg_393_reg_n_1_[15] ;
  wire \shl_ln108_reg_393_reg_n_1_[16] ;
  wire \shl_ln108_reg_393_reg_n_1_[17] ;
  wire \shl_ln108_reg_393_reg_n_1_[18] ;
  wire \shl_ln108_reg_393_reg_n_1_[19] ;
  wire \shl_ln108_reg_393_reg_n_1_[1] ;
  wire \shl_ln108_reg_393_reg_n_1_[20] ;
  wire \shl_ln108_reg_393_reg_n_1_[21] ;
  wire \shl_ln108_reg_393_reg_n_1_[22] ;
  wire \shl_ln108_reg_393_reg_n_1_[23] ;
  wire \shl_ln108_reg_393_reg_n_1_[24] ;
  wire \shl_ln108_reg_393_reg_n_1_[25] ;
  wire \shl_ln108_reg_393_reg_n_1_[26] ;
  wire \shl_ln108_reg_393_reg_n_1_[27] ;
  wire \shl_ln108_reg_393_reg_n_1_[28] ;
  wire \shl_ln108_reg_393_reg_n_1_[29] ;
  wire \shl_ln108_reg_393_reg_n_1_[2] ;
  wire \shl_ln108_reg_393_reg_n_1_[30] ;
  wire \shl_ln108_reg_393_reg_n_1_[31] ;
  wire \shl_ln108_reg_393_reg_n_1_[3] ;
  wire \shl_ln108_reg_393_reg_n_1_[4] ;
  wire \shl_ln108_reg_393_reg_n_1_[5] ;
  wire \shl_ln108_reg_393_reg_n_1_[6] ;
  wire \shl_ln108_reg_393_reg_n_1_[7] ;
  wire \shl_ln108_reg_393_reg_n_1_[8] ;
  wire \shl_ln108_reg_393_reg_n_1_[9] ;
  wire [31:0]srem_ln105_reg_347;
  wire [31:6]srem_ln108_reg_383;
  wire [5:1]sub_ln105_fu_178_p2;
  wire [5:0]sub_ln105_reg_362;
  wire \tmp_reg_367[0]_i_1_n_1 ;
  wire \tmp_reg_367[10]_i_2_n_1 ;
  wire \tmp_reg_367[1]_i_1_n_1 ;
  wire \tmp_reg_367[25]_i_1_n_1 ;
  wire \tmp_reg_367[2]_i_1_n_1 ;
  wire \tmp_reg_367[3]_i_1_n_1 ;
  wire \tmp_reg_367[4]_i_1_n_1 ;
  wire \tmp_reg_367[5]_i_1_n_1 ;
  wire \tmp_reg_367[6]_i_1_n_1 ;
  wire \tmp_reg_367[6]_i_2_n_1 ;
  wire \tmp_reg_367[7]_i_1_n_1 ;
  wire \tmp_reg_367[8]_i_1_n_1 ;
  wire \tmp_reg_367[9]_i_1_n_1 ;
  wire \tmp_reg_367[9]_i_2_n_1 ;
  wire [5:0]trunc_ln108_reg_388;
  wire [3:3]\NLW_b1_2_reg_409_reg[31]_i_1_CO_UNCONNECTED ;

  assign M_address0[11] = \<const0> ;
  assign M_address0[10:0] = \^M_address0 [10:0];
  assign M_address1[11] = \<const0> ;
  assign M_address1[10:0] = \^M_address1 [10:0];
  assign ap_done = ap_ready;
  GND GND
       (.G(\<const0> ));
  (* SOFT_HLUTNM = "soft_lutpair86" *) 
  LUT2 #(
    .INIT(4'hE)) 
    \M_address0[0]_INST_0 
       (.I0(\i_0_reg_111_reg_n_1_[0] ),
        .I1(ap_CS_fsm_state81),
        .O(\^M_address0 [0]));
  (* SOFT_HLUTNM = "soft_lutpair86" *) 
  LUT2 #(
    .INIT(4'hE)) 
    M_ce0_INST_0
       (.I0(ap_CS_fsm_state81),
        .I1(M_ce1),
        .O(M_ce0));
  LUT5 #(
    .INIT(32'hABFF0000)) 
    \add_ln101_reg_305[10]_i_1 
       (.I0(ap_ready_INST_0_i_1_n_1),
        .I1(\^M_address0 [5]),
        .I2(\^M_address0 [4]),
        .I3(\^M_address0 [10]),
        .I4(ap_CS_fsm_state2),
        .O(ap_NS_fsm[2]));
  LUT6 #(
    .INIT(64'hBFFFFFFF40000000)) 
    \add_ln101_reg_305[10]_i_2 
       (.I0(\add_ln101_reg_305[10]_i_3_n_1 ),
        .I1(\^M_address0 [8]),
        .I2(\^M_address0 [6]),
        .I3(\^M_address0 [7]),
        .I4(\^M_address0 [9]),
        .I5(\^M_address0 [10]),
        .O(add_ln101_fu_134_p2[10]));
  (* SOFT_HLUTNM = "soft_lutpair36" *) 
  LUT4 #(
    .INIT(16'h7FFF)) 
    \add_ln101_reg_305[10]_i_3 
       (.I0(\^M_address0 [4]),
        .I1(\^M_address0 [2]),
        .I2(\^M_address0 [3]),
        .I3(\^M_address0 [5]),
        .O(\add_ln101_reg_305[10]_i_3_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair54" *) 
  LUT1 #(
    .INIT(2'h1)) 
    \add_ln101_reg_305[2]_i_1 
       (.I0(\^M_address0 [2]),
        .O(add_ln101_fu_134_p2[2]));
  (* SOFT_HLUTNM = "soft_lutpair54" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln101_reg_305[3]_i_1 
       (.I0(\^M_address0 [2]),
        .I1(\^M_address0 [3]),
        .O(add_ln101_fu_134_p2[3]));
  (* SOFT_HLUTNM = "soft_lutpair38" *) 
  LUT3 #(
    .INIT(8'h78)) 
    \add_ln101_reg_305[4]_i_1 
       (.I0(\^M_address0 [3]),
        .I1(\^M_address0 [2]),
        .I2(\^M_address0 [4]),
        .O(add_ln101_fu_134_p2[4]));
  (* SOFT_HLUTNM = "soft_lutpair38" *) 
  LUT4 #(
    .INIT(16'h7F80)) 
    \add_ln101_reg_305[5]_i_1 
       (.I0(\^M_address0 [4]),
        .I1(\^M_address0 [2]),
        .I2(\^M_address0 [3]),
        .I3(\^M_address0 [5]),
        .O(add_ln101_fu_134_p2[5]));
  (* SOFT_HLUTNM = "soft_lutpair36" *) 
  LUT5 #(
    .INIT(32'h7FFF8000)) 
    \add_ln101_reg_305[6]_i_1 
       (.I0(\^M_address0 [5]),
        .I1(\^M_address0 [3]),
        .I2(\^M_address0 [2]),
        .I3(\^M_address0 [4]),
        .I4(\^M_address0 [6]),
        .O(add_ln101_fu_134_p2[6]));
  LUT6 #(
    .INIT(64'h7FFFFFFF80000000)) 
    \add_ln101_reg_305[7]_i_1 
       (.I0(\^M_address0 [6]),
        .I1(\^M_address0 [4]),
        .I2(\^M_address0 [2]),
        .I3(\^M_address0 [3]),
        .I4(\^M_address0 [5]),
        .I5(\^M_address0 [7]),
        .O(add_ln101_fu_134_p2[7]));
  (* SOFT_HLUTNM = "soft_lutpair34" *) 
  LUT4 #(
    .INIT(16'hBF40)) 
    \add_ln101_reg_305[8]_i_1 
       (.I0(\add_ln101_reg_305[10]_i_3_n_1 ),
        .I1(\^M_address0 [6]),
        .I2(\^M_address0 [7]),
        .I3(\^M_address0 [8]),
        .O(add_ln101_fu_134_p2[8]));
  (* SOFT_HLUTNM = "soft_lutpair34" *) 
  LUT5 #(
    .INIT(32'hBFFF4000)) 
    \add_ln101_reg_305[9]_i_1 
       (.I0(\add_ln101_reg_305[10]_i_3_n_1 ),
        .I1(\^M_address0 [7]),
        .I2(\^M_address0 [6]),
        .I3(\^M_address0 [8]),
        .I4(\^M_address0 [9]),
        .O(add_ln101_fu_134_p2[9]));
  FDRE \add_ln101_reg_305_reg[0] 
       (.C(ap_clk),
        .CE(ap_NS_fsm[2]),
        .D(\i_0_reg_111_reg_n_1_[0] ),
        .Q(\^M_address1 [0]),
        .R(1'b0));
  FDRE \add_ln101_reg_305_reg[10] 
       (.C(ap_clk),
        .CE(ap_NS_fsm[2]),
        .D(add_ln101_fu_134_p2[10]),
        .Q(\^M_address1 [10]),
        .R(1'b0));
  FDRE \add_ln101_reg_305_reg[1] 
       (.C(ap_clk),
        .CE(ap_NS_fsm[2]),
        .D(\^M_address0 [1]),
        .Q(\^M_address1 [1]),
        .R(1'b0));
  FDRE \add_ln101_reg_305_reg[2] 
       (.C(ap_clk),
        .CE(ap_NS_fsm[2]),
        .D(add_ln101_fu_134_p2[2]),
        .Q(\^M_address1 [2]),
        .R(1'b0));
  FDRE \add_ln101_reg_305_reg[3] 
       (.C(ap_clk),
        .CE(ap_NS_fsm[2]),
        .D(add_ln101_fu_134_p2[3]),
        .Q(\^M_address1 [3]),
        .R(1'b0));
  FDRE \add_ln101_reg_305_reg[4] 
       (.C(ap_clk),
        .CE(ap_NS_fsm[2]),
        .D(add_ln101_fu_134_p2[4]),
        .Q(\^M_address1 [4]),
        .R(1'b0));
  FDRE \add_ln101_reg_305_reg[5] 
       (.C(ap_clk),
        .CE(ap_NS_fsm[2]),
        .D(add_ln101_fu_134_p2[5]),
        .Q(\^M_address1 [5]),
        .R(1'b0));
  FDRE \add_ln101_reg_305_reg[6] 
       (.C(ap_clk),
        .CE(ap_NS_fsm[2]),
        .D(add_ln101_fu_134_p2[6]),
        .Q(\^M_address1 [6]),
        .R(1'b0));
  FDRE \add_ln101_reg_305_reg[7] 
       (.C(ap_clk),
        .CE(ap_NS_fsm[2]),
        .D(add_ln101_fu_134_p2[7]),
        .Q(\^M_address1 [7]),
        .R(1'b0));
  FDRE \add_ln101_reg_305_reg[8] 
       (.C(ap_clk),
        .CE(ap_NS_fsm[2]),
        .D(add_ln101_fu_134_p2[8]),
        .Q(\^M_address1 [8]),
        .R(1'b0));
  FDRE \add_ln101_reg_305_reg[9] 
       (.C(ap_clk),
        .CE(ap_NS_fsm[2]),
        .D(add_ln101_fu_134_p2[9]),
        .Q(\^M_address1 [9]),
        .R(1'b0));
  LUT6 #(
    .INIT(64'hEAAAAAAAAAAAAAAA)) 
    \ap_CS_fsm[0]_i_1 
       (.I0(ap_idle),
        .I1(\ap_CS_fsm[0]_i_2_n_1 ),
        .I2(\ap_CS_fsm[0]_i_3_n_1 ),
        .I3(\ap_CS_fsm[0]_i_4_n_1 ),
        .I4(\ap_CS_fsm[0]_i_5_n_1 ),
        .I5(\ap_CS_fsm[0]_i_6_n_1 ),
        .O(ap_NS_fsm[0]));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \ap_CS_fsm[0]_i_10 
       (.I0(\ap_CS_fsm_reg_n_1_[7] ),
        .I1(\ap_CS_fsm_reg_n_1_[8] ),
        .I2(ap_CS_fsm_state6),
        .I3(grp_fu_163_ap_start),
        .I4(\ap_CS_fsm_reg_n_1_[10] ),
        .I5(\ap_CS_fsm_reg_n_1_[9] ),
        .O(\ap_CS_fsm[0]_i_10_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \ap_CS_fsm[0]_i_11 
       (.I0(\ap_CS_fsm_reg_n_1_[13] ),
        .I1(\ap_CS_fsm_reg_n_1_[14] ),
        .I2(\ap_CS_fsm_reg_n_1_[11] ),
        .I3(\ap_CS_fsm_reg_n_1_[12] ),
        .I4(\ap_CS_fsm_reg_n_1_[16] ),
        .I5(\ap_CS_fsm_reg_n_1_[15] ),
        .O(\ap_CS_fsm[0]_i_11_n_1 ));
  LUT5 #(
    .INIT(32'h00000004)) 
    \ap_CS_fsm[0]_i_12 
       (.I0(\ap_CS_fsm_reg_n_1_[0] ),
        .I1(ap_CS_fsm_state2),
        .I2(M_ce1),
        .I3(ap_CS_fsm_state5),
        .I4(ap_CS_fsm_state4),
        .O(\ap_CS_fsm[0]_i_12_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \ap_CS_fsm[0]_i_13 
       (.I0(\ap_CS_fsm_reg_n_1_[55] ),
        .I1(\ap_CS_fsm_reg_n_1_[56] ),
        .I2(\ap_CS_fsm_reg_n_1_[53] ),
        .I3(\ap_CS_fsm_reg_n_1_[54] ),
        .I4(\ap_CS_fsm_reg_n_1_[58] ),
        .I5(\ap_CS_fsm_reg_n_1_[57] ),
        .O(\ap_CS_fsm[0]_i_13_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \ap_CS_fsm[0]_i_14 
       (.I0(\ap_CS_fsm_reg_n_1_[49] ),
        .I1(\ap_CS_fsm_reg_n_1_[50] ),
        .I2(\ap_CS_fsm_reg_n_1_[47] ),
        .I3(\ap_CS_fsm_reg_n_1_[48] ),
        .I4(\ap_CS_fsm_reg_n_1_[52] ),
        .I5(\ap_CS_fsm_reg_n_1_[51] ),
        .O(\ap_CS_fsm[0]_i_14_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \ap_CS_fsm[0]_i_15 
       (.I0(\ap_CS_fsm_reg_n_1_[67] ),
        .I1(\ap_CS_fsm_reg_n_1_[68] ),
        .I2(\ap_CS_fsm_reg_n_1_[65] ),
        .I3(\ap_CS_fsm_reg_n_1_[66] ),
        .I4(\ap_CS_fsm_reg_n_1_[70] ),
        .I5(\ap_CS_fsm_reg_n_1_[69] ),
        .O(\ap_CS_fsm[0]_i_15_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \ap_CS_fsm[0]_i_16 
       (.I0(\ap_CS_fsm_reg_n_1_[61] ),
        .I1(\ap_CS_fsm_reg_n_1_[62] ),
        .I2(\ap_CS_fsm_reg_n_1_[59] ),
        .I3(\ap_CS_fsm_reg_n_1_[60] ),
        .I4(\ap_CS_fsm_reg_n_1_[64] ),
        .I5(\ap_CS_fsm_reg_n_1_[63] ),
        .O(\ap_CS_fsm[0]_i_16_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \ap_CS_fsm[0]_i_17 
       (.I0(\ap_CS_fsm_reg_n_1_[37] ),
        .I1(\ap_CS_fsm_reg_n_1_[38] ),
        .I2(\ap_CS_fsm_reg_n_1_[35] ),
        .I3(\ap_CS_fsm_reg_n_1_[36] ),
        .I4(\ap_CS_fsm_reg_n_1_[40] ),
        .I5(\ap_CS_fsm_reg_n_1_[39] ),
        .O(\ap_CS_fsm[0]_i_17_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \ap_CS_fsm[0]_i_18 
       (.I0(ap_CS_fsm_state44),
        .I1(buffer_r_we0),
        .I2(ap_CS_fsm_state42),
        .I3(ap_CS_fsm_state43),
        .I4(\ap_CS_fsm_reg_n_1_[46] ),
        .I5(\ap_CS_fsm_reg_n_1_[45] ),
        .O(\ap_CS_fsm[0]_i_18_n_1 ));
  LUT5 #(
    .INIT(32'h80000000)) 
    \ap_CS_fsm[0]_i_2 
       (.I0(\ap_CS_fsm[0]_i_7_n_1 ),
        .I1(\ap_CS_fsm[0]_i_8_n_1 ),
        .I2(\ap_CS_fsm[0]_i_9_n_1 ),
        .I3(\ap_CS_fsm[0]_i_10_n_1 ),
        .I4(\ap_CS_fsm[0]_i_11_n_1 ),
        .O(\ap_CS_fsm[0]_i_2_n_1 ));
  LUT5 #(
    .INIT(32'h22200000)) 
    \ap_CS_fsm[0]_i_3 
       (.I0(\ap_CS_fsm[0]_i_12_n_1 ),
        .I1(ap_ready_INST_0_i_1_n_1),
        .I2(\^M_address0 [5]),
        .I3(\^M_address0 [4]),
        .I4(\^M_address0 [10]),
        .O(\ap_CS_fsm[0]_i_3_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \ap_CS_fsm[0]_i_4 
       (.I0(\ap_CS_fsm_reg_n_1_[73] ),
        .I1(\ap_CS_fsm_reg_n_1_[74] ),
        .I2(\ap_CS_fsm_reg_n_1_[71] ),
        .I3(\ap_CS_fsm_reg_n_1_[72] ),
        .I4(\ap_CS_fsm_reg_n_1_[76] ),
        .I5(\ap_CS_fsm_reg_n_1_[75] ),
        .O(\ap_CS_fsm[0]_i_4_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \ap_CS_fsm[0]_i_5 
       (.I0(ap_CS_fsm_state80),
        .I1(ap_CS_fsm_state81),
        .I2(\ap_CS_fsm_reg_n_1_[77] ),
        .I3(\ap_CS_fsm_reg_n_1_[78] ),
        .I4(buffer_r_we1),
        .I5(ap_CS_fsm_state82),
        .O(\ap_CS_fsm[0]_i_5_n_1 ));
  LUT6 #(
    .INIT(64'h8000000000000000)) 
    \ap_CS_fsm[0]_i_6 
       (.I0(\ap_CS_fsm[0]_i_13_n_1 ),
        .I1(\ap_CS_fsm[0]_i_14_n_1 ),
        .I2(\ap_CS_fsm[0]_i_15_n_1 ),
        .I3(\ap_CS_fsm[0]_i_16_n_1 ),
        .I4(\ap_CS_fsm[0]_i_17_n_1 ),
        .I5(\ap_CS_fsm[0]_i_18_n_1 ),
        .O(\ap_CS_fsm[0]_i_6_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \ap_CS_fsm[0]_i_7 
       (.I0(\ap_CS_fsm_reg_n_1_[31] ),
        .I1(\ap_CS_fsm_reg_n_1_[32] ),
        .I2(\ap_CS_fsm_reg_n_1_[29] ),
        .I3(\ap_CS_fsm_reg_n_1_[30] ),
        .I4(\ap_CS_fsm_reg_n_1_[34] ),
        .I5(\ap_CS_fsm_reg_n_1_[33] ),
        .O(\ap_CS_fsm[0]_i_7_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \ap_CS_fsm[0]_i_8 
       (.I0(\ap_CS_fsm_reg_n_1_[25] ),
        .I1(\ap_CS_fsm_reg_n_1_[26] ),
        .I2(\ap_CS_fsm_reg_n_1_[23] ),
        .I3(\ap_CS_fsm_reg_n_1_[24] ),
        .I4(\ap_CS_fsm_reg_n_1_[28] ),
        .I5(\ap_CS_fsm_reg_n_1_[27] ),
        .O(\ap_CS_fsm[0]_i_8_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \ap_CS_fsm[0]_i_9 
       (.I0(\ap_CS_fsm_reg_n_1_[19] ),
        .I1(\ap_CS_fsm_reg_n_1_[20] ),
        .I2(\ap_CS_fsm_reg_n_1_[17] ),
        .I3(\ap_CS_fsm_reg_n_1_[18] ),
        .I4(\ap_CS_fsm_reg_n_1_[22] ),
        .I5(\ap_CS_fsm_reg_n_1_[21] ),
        .O(\ap_CS_fsm[0]_i_9_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair41" *) 
  LUT3 #(
    .INIT(8'hF8)) 
    \ap_CS_fsm[1]_i_1 
       (.I0(\ap_CS_fsm_reg_n_1_[0] ),
        .I1(ap_start),
        .I2(buffer_r_we1),
        .O(ap_NS_fsm[1]));
  (* FSM_ENCODING = "none" *) 
  FDSE #(
    .INIT(1'b1)) 
    \ap_CS_fsm_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_NS_fsm[0]),
        .Q(\ap_CS_fsm_reg_n_1_[0] ),
        .S(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[10] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[9] ),
        .Q(\ap_CS_fsm_reg_n_1_[10] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[11] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[10] ),
        .Q(\ap_CS_fsm_reg_n_1_[11] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[12] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[11] ),
        .Q(\ap_CS_fsm_reg_n_1_[12] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[13] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[12] ),
        .Q(\ap_CS_fsm_reg_n_1_[13] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[14] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[13] ),
        .Q(\ap_CS_fsm_reg_n_1_[14] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[15] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[14] ),
        .Q(\ap_CS_fsm_reg_n_1_[15] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[16] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[15] ),
        .Q(\ap_CS_fsm_reg_n_1_[16] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[17] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[16] ),
        .Q(\ap_CS_fsm_reg_n_1_[17] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[18] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[17] ),
        .Q(\ap_CS_fsm_reg_n_1_[18] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[19] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[18] ),
        .Q(\ap_CS_fsm_reg_n_1_[19] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[1] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_NS_fsm[1]),
        .Q(ap_CS_fsm_state2),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[20] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[19] ),
        .Q(\ap_CS_fsm_reg_n_1_[20] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[21] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[20] ),
        .Q(\ap_CS_fsm_reg_n_1_[21] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[22] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[21] ),
        .Q(\ap_CS_fsm_reg_n_1_[22] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[23] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[22] ),
        .Q(\ap_CS_fsm_reg_n_1_[23] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[24] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[23] ),
        .Q(\ap_CS_fsm_reg_n_1_[24] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[25] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[24] ),
        .Q(\ap_CS_fsm_reg_n_1_[25] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[26] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[25] ),
        .Q(\ap_CS_fsm_reg_n_1_[26] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[27] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[26] ),
        .Q(\ap_CS_fsm_reg_n_1_[27] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[28] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[27] ),
        .Q(\ap_CS_fsm_reg_n_1_[28] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[29] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[28] ),
        .Q(\ap_CS_fsm_reg_n_1_[29] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_NS_fsm[2]),
        .Q(M_ce1),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[30] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[29] ),
        .Q(\ap_CS_fsm_reg_n_1_[30] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[31] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[30] ),
        .Q(\ap_CS_fsm_reg_n_1_[31] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[32] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[31] ),
        .Q(\ap_CS_fsm_reg_n_1_[32] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[33] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[32] ),
        .Q(\ap_CS_fsm_reg_n_1_[33] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[34] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[33] ),
        .Q(\ap_CS_fsm_reg_n_1_[34] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[35] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[34] ),
        .Q(\ap_CS_fsm_reg_n_1_[35] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[36] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[35] ),
        .Q(\ap_CS_fsm_reg_n_1_[36] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[37] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[36] ),
        .Q(\ap_CS_fsm_reg_n_1_[37] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[38] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[37] ),
        .Q(\ap_CS_fsm_reg_n_1_[38] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[39] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[38] ),
        .Q(\ap_CS_fsm_reg_n_1_[39] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[3] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(M_ce1),
        .Q(ap_CS_fsm_state4),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[40] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[39] ),
        .Q(\ap_CS_fsm_reg_n_1_[40] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[41] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[40] ),
        .Q(ap_CS_fsm_state42),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[42] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_CS_fsm_state42),
        .Q(ap_CS_fsm_state43),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[43] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_CS_fsm_state43),
        .Q(ap_CS_fsm_state44),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[44] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_CS_fsm_state44),
        .Q(buffer_r_we0),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[45] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buffer_r_we0),
        .Q(\ap_CS_fsm_reg_n_1_[45] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[46] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[45] ),
        .Q(\ap_CS_fsm_reg_n_1_[46] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[47] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[46] ),
        .Q(\ap_CS_fsm_reg_n_1_[47] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[48] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[47] ),
        .Q(\ap_CS_fsm_reg_n_1_[48] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[49] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[48] ),
        .Q(\ap_CS_fsm_reg_n_1_[49] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[4] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_CS_fsm_state4),
        .Q(ap_CS_fsm_state5),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[50] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[49] ),
        .Q(\ap_CS_fsm_reg_n_1_[50] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[51] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[50] ),
        .Q(\ap_CS_fsm_reg_n_1_[51] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[52] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[51] ),
        .Q(\ap_CS_fsm_reg_n_1_[52] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[53] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[52] ),
        .Q(\ap_CS_fsm_reg_n_1_[53] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[54] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[53] ),
        .Q(\ap_CS_fsm_reg_n_1_[54] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[55] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[54] ),
        .Q(\ap_CS_fsm_reg_n_1_[55] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[56] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[55] ),
        .Q(\ap_CS_fsm_reg_n_1_[56] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[57] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[56] ),
        .Q(\ap_CS_fsm_reg_n_1_[57] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[58] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[57] ),
        .Q(\ap_CS_fsm_reg_n_1_[58] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[59] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[58] ),
        .Q(\ap_CS_fsm_reg_n_1_[59] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[5] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_CS_fsm_state5),
        .Q(ap_CS_fsm_state6),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[60] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[59] ),
        .Q(\ap_CS_fsm_reg_n_1_[60] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[61] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[60] ),
        .Q(\ap_CS_fsm_reg_n_1_[61] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[62] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[61] ),
        .Q(\ap_CS_fsm_reg_n_1_[62] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[63] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[62] ),
        .Q(\ap_CS_fsm_reg_n_1_[63] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[64] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[63] ),
        .Q(\ap_CS_fsm_reg_n_1_[64] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[65] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[64] ),
        .Q(\ap_CS_fsm_reg_n_1_[65] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[66] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[65] ),
        .Q(\ap_CS_fsm_reg_n_1_[66] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[67] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[66] ),
        .Q(\ap_CS_fsm_reg_n_1_[67] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[68] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[67] ),
        .Q(\ap_CS_fsm_reg_n_1_[68] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[69] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[68] ),
        .Q(\ap_CS_fsm_reg_n_1_[69] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[6] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_CS_fsm_state6),
        .Q(grp_fu_163_ap_start),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[70] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[69] ),
        .Q(\ap_CS_fsm_reg_n_1_[70] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[71] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[70] ),
        .Q(\ap_CS_fsm_reg_n_1_[71] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[72] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[71] ),
        .Q(\ap_CS_fsm_reg_n_1_[72] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[73] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[72] ),
        .Q(\ap_CS_fsm_reg_n_1_[73] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[74] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[73] ),
        .Q(\ap_CS_fsm_reg_n_1_[74] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[75] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[74] ),
        .Q(\ap_CS_fsm_reg_n_1_[75] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[76] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[75] ),
        .Q(\ap_CS_fsm_reg_n_1_[76] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[77] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[76] ),
        .Q(\ap_CS_fsm_reg_n_1_[77] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[78] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[77] ),
        .Q(\ap_CS_fsm_reg_n_1_[78] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[79] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[78] ),
        .Q(ap_CS_fsm_state80),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[7] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(grp_fu_163_ap_start),
        .Q(\ap_CS_fsm_reg_n_1_[7] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[80] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_CS_fsm_state80),
        .Q(ap_CS_fsm_state81),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[81] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_CS_fsm_state81),
        .Q(ap_CS_fsm_state82),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[82] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_CS_fsm_state82),
        .Q(buffer_r_we1),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[8] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[7] ),
        .Q(\ap_CS_fsm_reg_n_1_[8] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[9] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[8] ),
        .Q(\ap_CS_fsm_reg_n_1_[9] ),
        .R(ap_rst));
  (* SOFT_HLUTNM = "soft_lutpair41" *) 
  LUT2 #(
    .INIT(4'h2)) 
    ap_idle_INST_0
       (.I0(\ap_CS_fsm_reg_n_1_[0] ),
        .I1(ap_start),
        .O(ap_idle));
  LUT5 #(
    .INIT(32'h22200000)) 
    ap_ready_INST_0
       (.I0(ap_CS_fsm_state2),
        .I1(ap_ready_INST_0_i_1_n_1),
        .I2(\^M_address0 [5]),
        .I3(\^M_address0 [4]),
        .I4(\^M_address0 [10]),
        .O(ap_ready));
  LUT4 #(
    .INIT(16'h7FFF)) 
    ap_ready_INST_0_i_1
       (.I0(\^M_address0 [8]),
        .I1(\^M_address0 [6]),
        .I2(\^M_address0 [7]),
        .I3(\^M_address0 [9]),
        .O(ap_ready_INST_0_i_1_n_1));
  FDRE \b0_1_reg_341_reg[0] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[0]),
        .Q(b0_1_reg_341[0]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[10] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[10]),
        .Q(b0_1_reg_341[10]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[11] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[11]),
        .Q(b0_1_reg_341[11]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[12] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[12]),
        .Q(b0_1_reg_341[12]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[13] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[13]),
        .Q(b0_1_reg_341[13]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[14] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[14]),
        .Q(b0_1_reg_341[14]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[15] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[15]),
        .Q(b0_1_reg_341[15]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[16] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[16]),
        .Q(b0_1_reg_341[16]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[17] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[17]),
        .Q(b0_1_reg_341[17]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[18] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[18]),
        .Q(b0_1_reg_341[18]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[19] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[19]),
        .Q(b0_1_reg_341[19]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[1] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[1]),
        .Q(b0_1_reg_341[1]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[20] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[20]),
        .Q(b0_1_reg_341[20]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[21] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[21]),
        .Q(b0_1_reg_341[21]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[22] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[22]),
        .Q(b0_1_reg_341[22]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[23] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[23]),
        .Q(b0_1_reg_341[23]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[24] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[24]),
        .Q(b0_1_reg_341[24]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[25] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[25]),
        .Q(b0_1_reg_341[25]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[26] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[26]),
        .Q(b0_1_reg_341[26]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[27] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[27]),
        .Q(b0_1_reg_341[27]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[28] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[28]),
        .Q(b0_1_reg_341[28]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[29] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[29]),
        .Q(b0_1_reg_341[29]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[2] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[2]),
        .Q(b0_1_reg_341[2]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[30] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[30]),
        .Q(b0_1_reg_341[30]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[31] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[31]),
        .Q(b0_1_reg_341[31]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[3] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[3]),
        .Q(b0_1_reg_341[3]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[4] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[4]),
        .Q(b0_1_reg_341[4]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[5] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[5]),
        .Q(b0_1_reg_341[5]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[6] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[6]),
        .Q(b0_1_reg_341[6]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[7] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[7]),
        .Q(b0_1_reg_341[7]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[8] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[8]),
        .Q(b0_1_reg_341[8]),
        .R(1'b0));
  FDRE \b0_1_reg_341_reg[9] 
       (.C(ap_clk),
        .CE(grp_fu_163_ap_start),
        .D(b0_1_fu_159_p2[9]),
        .Q(b0_1_reg_341[9]),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair52" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[0]_i_1 
       (.I0(b0_1_reg_341[0]),
        .I1(lshr_ln105_fu_193_p2),
        .O(b0_2_fu_214_p2[0]));
  LUT6 #(
    .INIT(64'h0000000000000002)) 
    \b0_2_reg_372[0]_i_2 
       (.I0(sub_ln105_reg_362[0]),
        .I1(sub_ln105_reg_362[3]),
        .I2(sub_ln105_reg_362[2]),
        .I3(sub_ln105_reg_362[5]),
        .I4(sub_ln105_reg_362[4]),
        .I5(sub_ln105_reg_362[1]),
        .O(lshr_ln105_fu_193_p2));
  (* SOFT_HLUTNM = "soft_lutpair63" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[10]_i_1 
       (.I0(b0_1_reg_341[10]),
        .I1(or_ln_fu_207_p3[10]),
        .O(b0_2_fu_214_p2[10]));
  (* SOFT_HLUTNM = "soft_lutpair64" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[11]_i_1 
       (.I0(b0_1_reg_341[11]),
        .I1(or_ln_fu_207_p3[11]),
        .O(b0_2_fu_214_p2[11]));
  (* SOFT_HLUTNM = "soft_lutpair65" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[12]_i_1 
       (.I0(b0_1_reg_341[12]),
        .I1(or_ln_fu_207_p3[12]),
        .O(b0_2_fu_214_p2[12]));
  (* SOFT_HLUTNM = "soft_lutpair66" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[13]_i_1 
       (.I0(b0_1_reg_341[13]),
        .I1(or_ln_fu_207_p3[13]),
        .O(b0_2_fu_214_p2[13]));
  (* SOFT_HLUTNM = "soft_lutpair67" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[14]_i_1 
       (.I0(b0_1_reg_341[14]),
        .I1(or_ln_fu_207_p3[14]),
        .O(b0_2_fu_214_p2[14]));
  (* SOFT_HLUTNM = "soft_lutpair68" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[15]_i_1 
       (.I0(b0_1_reg_341[15]),
        .I1(or_ln_fu_207_p3[15]),
        .O(b0_2_fu_214_p2[15]));
  (* SOFT_HLUTNM = "soft_lutpair69" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[16]_i_1 
       (.I0(b0_1_reg_341[16]),
        .I1(or_ln_fu_207_p3[16]),
        .O(b0_2_fu_214_p2[16]));
  (* SOFT_HLUTNM = "soft_lutpair70" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[17]_i_1 
       (.I0(b0_1_reg_341[17]),
        .I1(or_ln_fu_207_p3[17]),
        .O(b0_2_fu_214_p2[17]));
  (* SOFT_HLUTNM = "soft_lutpair71" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[18]_i_1 
       (.I0(b0_1_reg_341[18]),
        .I1(or_ln_fu_207_p3[18]),
        .O(b0_2_fu_214_p2[18]));
  (* SOFT_HLUTNM = "soft_lutpair72" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[19]_i_1 
       (.I0(b0_1_reg_341[19]),
        .I1(or_ln_fu_207_p3[19]),
        .O(b0_2_fu_214_p2[19]));
  (* SOFT_HLUTNM = "soft_lutpair37" *) 
  LUT4 #(
    .INIT(16'h55A6)) 
    \b0_2_reg_372[1]_i_1 
       (.I0(b0_1_reg_341[1]),
        .I1(\b0_2_reg_372[1]_i_2_n_1 ),
        .I2(sub_ln105_reg_362[0]),
        .I3(shl_ln105_reg_357[1]),
        .O(b0_2_fu_214_p2[1]));
  LUT5 #(
    .INIT(32'h00000001)) 
    \b0_2_reg_372[1]_i_2 
       (.I0(sub_ln105_reg_362[1]),
        .I1(sub_ln105_reg_362[4]),
        .I2(sub_ln105_reg_362[5]),
        .I3(sub_ln105_reg_362[2]),
        .I4(sub_ln105_reg_362[3]),
        .O(\b0_2_reg_372[1]_i_2_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair83" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[20]_i_1 
       (.I0(b0_1_reg_341[20]),
        .I1(or_ln_fu_207_p3[20]),
        .O(b0_2_fu_214_p2[20]));
  (* SOFT_HLUTNM = "soft_lutpair73" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[21]_i_1 
       (.I0(b0_1_reg_341[21]),
        .I1(or_ln_fu_207_p3[21]),
        .O(b0_2_fu_214_p2[21]));
  (* SOFT_HLUTNM = "soft_lutpair74" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[22]_i_1 
       (.I0(b0_1_reg_341[22]),
        .I1(or_ln_fu_207_p3[22]),
        .O(b0_2_fu_214_p2[22]));
  (* SOFT_HLUTNM = "soft_lutpair75" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[23]_i_1 
       (.I0(b0_1_reg_341[23]),
        .I1(or_ln_fu_207_p3[23]),
        .O(b0_2_fu_214_p2[23]));
  (* SOFT_HLUTNM = "soft_lutpair55" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[24]_i_1 
       (.I0(b0_1_reg_341[24]),
        .I1(or_ln_fu_207_p3[24]),
        .O(b0_2_fu_214_p2[24]));
  (* SOFT_HLUTNM = "soft_lutpair76" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[25]_i_1 
       (.I0(b0_1_reg_341[25]),
        .I1(or_ln_fu_207_p3[25]),
        .O(b0_2_fu_214_p2[25]));
  (* SOFT_HLUTNM = "soft_lutpair77" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[26]_i_1 
       (.I0(b0_1_reg_341[26]),
        .I1(or_ln_fu_207_p3[26]),
        .O(b0_2_fu_214_p2[26]));
  (* SOFT_HLUTNM = "soft_lutpair78" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[27]_i_1 
       (.I0(b0_1_reg_341[27]),
        .I1(or_ln_fu_207_p3[27]),
        .O(b0_2_fu_214_p2[27]));
  (* SOFT_HLUTNM = "soft_lutpair79" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[28]_i_1 
       (.I0(b0_1_reg_341[28]),
        .I1(or_ln_fu_207_p3[28]),
        .O(b0_2_fu_214_p2[28]));
  (* SOFT_HLUTNM = "soft_lutpair80" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[29]_i_1 
       (.I0(b0_1_reg_341[29]),
        .I1(or_ln_fu_207_p3[29]),
        .O(b0_2_fu_214_p2[29]));
  (* SOFT_HLUTNM = "soft_lutpair51" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[2]_i_1 
       (.I0(b0_1_reg_341[2]),
        .I1(shl_ln105_reg_357[2]),
        .O(b0_2_fu_214_p2[2]));
  (* SOFT_HLUTNM = "soft_lutpair81" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[30]_i_1 
       (.I0(b0_1_reg_341[30]),
        .I1(or_ln_fu_207_p3[30]),
        .O(b0_2_fu_214_p2[30]));
  (* SOFT_HLUTNM = "soft_lutpair82" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[31]_i_1 
       (.I0(b0_1_reg_341[31]),
        .I1(or_ln_fu_207_p3[31]),
        .O(b0_2_fu_214_p2[31]));
  (* SOFT_HLUTNM = "soft_lutpair56" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[3]_i_1 
       (.I0(b0_1_reg_341[3]),
        .I1(shl_ln105_reg_357[3]),
        .O(b0_2_fu_214_p2[3]));
  (* SOFT_HLUTNM = "soft_lutpair57" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[4]_i_1 
       (.I0(b0_1_reg_341[4]),
        .I1(shl_ln105_reg_357[4]),
        .O(b0_2_fu_214_p2[4]));
  (* SOFT_HLUTNM = "soft_lutpair58" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[5]_i_1 
       (.I0(b0_1_reg_341[5]),
        .I1(shl_ln105_reg_357[5]),
        .O(b0_2_fu_214_p2[5]));
  (* SOFT_HLUTNM = "soft_lutpair59" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[6]_i_1 
       (.I0(b0_1_reg_341[6]),
        .I1(or_ln_fu_207_p3[6]),
        .O(b0_2_fu_214_p2[6]));
  (* SOFT_HLUTNM = "soft_lutpair60" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[7]_i_1 
       (.I0(b0_1_reg_341[7]),
        .I1(or_ln_fu_207_p3[7]),
        .O(b0_2_fu_214_p2[7]));
  (* SOFT_HLUTNM = "soft_lutpair61" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[8]_i_1 
       (.I0(b0_1_reg_341[8]),
        .I1(or_ln_fu_207_p3[8]),
        .O(b0_2_fu_214_p2[8]));
  (* SOFT_HLUTNM = "soft_lutpair62" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b0_2_reg_372[9]_i_1 
       (.I0(b0_1_reg_341[9]),
        .I1(or_ln_fu_207_p3[9]),
        .O(b0_2_fu_214_p2[9]));
  FDRE \b0_2_reg_372_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[0]),
        .Q(buffer_r_d0[0]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[10]),
        .Q(buffer_r_d0[10]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[11] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[11]),
        .Q(buffer_r_d0[11]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[12] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[12]),
        .Q(buffer_r_d0[12]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[13] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[13]),
        .Q(buffer_r_d0[13]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[14] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[14]),
        .Q(buffer_r_d0[14]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[15] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[15]),
        .Q(buffer_r_d0[15]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[16] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[16]),
        .Q(buffer_r_d0[16]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[17] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[17]),
        .Q(buffer_r_d0[17]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[18] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[18]),
        .Q(buffer_r_d0[18]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[19] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[19]),
        .Q(buffer_r_d0[19]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[1]),
        .Q(buffer_r_d0[1]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[20] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[20]),
        .Q(buffer_r_d0[20]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[21] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[21]),
        .Q(buffer_r_d0[21]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[22] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[22]),
        .Q(buffer_r_d0[22]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[23] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[23]),
        .Q(buffer_r_d0[23]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[24] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[24]),
        .Q(buffer_r_d0[24]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[25] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[25]),
        .Q(buffer_r_d0[25]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[26] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[26]),
        .Q(buffer_r_d0[26]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[27] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[27]),
        .Q(buffer_r_d0[27]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[28] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[28]),
        .Q(buffer_r_d0[28]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[29] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[29]),
        .Q(buffer_r_d0[29]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[2]),
        .Q(buffer_r_d0[2]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[30] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[30]),
        .Q(buffer_r_d0[30]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[31] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[31]),
        .Q(buffer_r_d0[31]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[3]),
        .Q(buffer_r_d0[3]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[4]),
        .Q(buffer_r_d0[4]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[5]),
        .Q(buffer_r_d0[5]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[6]),
        .Q(buffer_r_d0[6]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[7]),
        .Q(buffer_r_d0[7]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[8]),
        .Q(buffer_r_d0[8]),
        .R(1'b0));
  FDRE \b0_2_reg_372_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b0_2_fu_214_p2[9]),
        .Q(buffer_r_d0[9]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[0]),
        .Q(b0_reg_330[0]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[10]),
        .Q(b0_reg_330[10]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[11] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[11]),
        .Q(b0_reg_330[11]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[12] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[12]),
        .Q(b0_reg_330[12]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[13] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[13]),
        .Q(b0_reg_330[13]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[14] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[14]),
        .Q(b0_reg_330[14]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[15] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[15]),
        .Q(b0_reg_330[15]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[16] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[16]),
        .Q(b0_reg_330[16]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[17] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[17]),
        .Q(b0_reg_330[17]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[18] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[18]),
        .Q(b0_reg_330[18]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[19] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[19]),
        .Q(b0_reg_330[19]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[1]),
        .Q(b0_reg_330[1]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[20] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[20]),
        .Q(b0_reg_330[20]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[21] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[21]),
        .Q(b0_reg_330[21]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[22] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[22]),
        .Q(b0_reg_330[22]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[23] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[23]),
        .Q(b0_reg_330[23]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[24] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[24]),
        .Q(b0_reg_330[24]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[25] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[25]),
        .Q(b0_reg_330[25]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[26] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[26]),
        .Q(b0_reg_330[26]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[27] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[27]),
        .Q(b0_reg_330[27]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[28] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[28]),
        .Q(b0_reg_330[28]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[29] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[29]),
        .Q(b0_reg_330[29]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[2]),
        .Q(b0_reg_330[2]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[30] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[30]),
        .Q(b0_reg_330[30]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[31] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[31]),
        .Q(b0_reg_330[31]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[3]),
        .Q(b0_reg_330[3]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[4]),
        .Q(b0_reg_330[4]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[5]),
        .Q(b0_reg_330[5]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[6]),
        .Q(b0_reg_330[6]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[7]),
        .Q(b0_reg_330[7]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[8]),
        .Q(b0_reg_330[8]),
        .R(1'b0));
  FDRE \b0_reg_330_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q0[9]),
        .Q(b0_reg_330[9]),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair52" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[0]_i_1 
       (.I0(b0_reg_330[0]),
        .I1(lshr_ln105_fu_193_p2),
        .O(b1_1_fu_219_p2[0]));
  (* SOFT_HLUTNM = "soft_lutpair63" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[10]_i_1 
       (.I0(b0_reg_330[10]),
        .I1(or_ln_fu_207_p3[10]),
        .O(b1_1_fu_219_p2[10]));
  (* SOFT_HLUTNM = "soft_lutpair64" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[11]_i_1 
       (.I0(b0_reg_330[11]),
        .I1(or_ln_fu_207_p3[11]),
        .O(b1_1_fu_219_p2[11]));
  (* SOFT_HLUTNM = "soft_lutpair65" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[12]_i_1 
       (.I0(b0_reg_330[12]),
        .I1(or_ln_fu_207_p3[12]),
        .O(b1_1_fu_219_p2[12]));
  (* SOFT_HLUTNM = "soft_lutpair66" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[13]_i_1 
       (.I0(b0_reg_330[13]),
        .I1(or_ln_fu_207_p3[13]),
        .O(b1_1_fu_219_p2[13]));
  (* SOFT_HLUTNM = "soft_lutpair67" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[14]_i_1 
       (.I0(b0_reg_330[14]),
        .I1(or_ln_fu_207_p3[14]),
        .O(b1_1_fu_219_p2[14]));
  (* SOFT_HLUTNM = "soft_lutpair68" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[15]_i_1 
       (.I0(b0_reg_330[15]),
        .I1(or_ln_fu_207_p3[15]),
        .O(b1_1_fu_219_p2[15]));
  (* SOFT_HLUTNM = "soft_lutpair69" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[16]_i_1 
       (.I0(b0_reg_330[16]),
        .I1(or_ln_fu_207_p3[16]),
        .O(b1_1_fu_219_p2[16]));
  (* SOFT_HLUTNM = "soft_lutpair70" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[17]_i_1 
       (.I0(b0_reg_330[17]),
        .I1(or_ln_fu_207_p3[17]),
        .O(b1_1_fu_219_p2[17]));
  (* SOFT_HLUTNM = "soft_lutpair71" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[18]_i_1 
       (.I0(b0_reg_330[18]),
        .I1(or_ln_fu_207_p3[18]),
        .O(b1_1_fu_219_p2[18]));
  (* SOFT_HLUTNM = "soft_lutpair72" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[19]_i_1 
       (.I0(b0_reg_330[19]),
        .I1(or_ln_fu_207_p3[19]),
        .O(b1_1_fu_219_p2[19]));
  (* SOFT_HLUTNM = "soft_lutpair37" *) 
  LUT4 #(
    .INIT(16'h55A6)) 
    \b1_1_reg_377[1]_i_1 
       (.I0(b0_reg_330[1]),
        .I1(\b0_2_reg_372[1]_i_2_n_1 ),
        .I2(sub_ln105_reg_362[0]),
        .I3(shl_ln105_reg_357[1]),
        .O(b1_1_fu_219_p2[1]));
  (* SOFT_HLUTNM = "soft_lutpair83" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[20]_i_1 
       (.I0(b0_reg_330[20]),
        .I1(or_ln_fu_207_p3[20]),
        .O(b1_1_fu_219_p2[20]));
  (* SOFT_HLUTNM = "soft_lutpair73" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[21]_i_1 
       (.I0(b0_reg_330[21]),
        .I1(or_ln_fu_207_p3[21]),
        .O(b1_1_fu_219_p2[21]));
  (* SOFT_HLUTNM = "soft_lutpair74" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[22]_i_1 
       (.I0(b0_reg_330[22]),
        .I1(or_ln_fu_207_p3[22]),
        .O(b1_1_fu_219_p2[22]));
  (* SOFT_HLUTNM = "soft_lutpair75" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[23]_i_1 
       (.I0(b0_reg_330[23]),
        .I1(or_ln_fu_207_p3[23]),
        .O(b1_1_fu_219_p2[23]));
  (* SOFT_HLUTNM = "soft_lutpair55" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[24]_i_1 
       (.I0(b0_reg_330[24]),
        .I1(or_ln_fu_207_p3[24]),
        .O(b1_1_fu_219_p2[24]));
  (* SOFT_HLUTNM = "soft_lutpair76" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[25]_i_1 
       (.I0(b0_reg_330[25]),
        .I1(or_ln_fu_207_p3[25]),
        .O(b1_1_fu_219_p2[25]));
  (* SOFT_HLUTNM = "soft_lutpair77" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[26]_i_1 
       (.I0(b0_reg_330[26]),
        .I1(or_ln_fu_207_p3[26]),
        .O(b1_1_fu_219_p2[26]));
  (* SOFT_HLUTNM = "soft_lutpair78" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[27]_i_1 
       (.I0(b0_reg_330[27]),
        .I1(or_ln_fu_207_p3[27]),
        .O(b1_1_fu_219_p2[27]));
  (* SOFT_HLUTNM = "soft_lutpair79" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[28]_i_1 
       (.I0(b0_reg_330[28]),
        .I1(or_ln_fu_207_p3[28]),
        .O(b1_1_fu_219_p2[28]));
  (* SOFT_HLUTNM = "soft_lutpair80" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[29]_i_1 
       (.I0(b0_reg_330[29]),
        .I1(or_ln_fu_207_p3[29]),
        .O(b1_1_fu_219_p2[29]));
  (* SOFT_HLUTNM = "soft_lutpair51" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[2]_i_1 
       (.I0(b0_reg_330[2]),
        .I1(shl_ln105_reg_357[2]),
        .O(b1_1_fu_219_p2[2]));
  (* SOFT_HLUTNM = "soft_lutpair81" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[30]_i_1 
       (.I0(b0_reg_330[30]),
        .I1(or_ln_fu_207_p3[30]),
        .O(b1_1_fu_219_p2[30]));
  (* SOFT_HLUTNM = "soft_lutpair82" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[31]_i_1 
       (.I0(b0_reg_330[31]),
        .I1(or_ln_fu_207_p3[31]),
        .O(b1_1_fu_219_p2[31]));
  (* SOFT_HLUTNM = "soft_lutpair56" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[3]_i_1 
       (.I0(b0_reg_330[3]),
        .I1(shl_ln105_reg_357[3]),
        .O(b1_1_fu_219_p2[3]));
  (* SOFT_HLUTNM = "soft_lutpair57" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[4]_i_1 
       (.I0(b0_reg_330[4]),
        .I1(shl_ln105_reg_357[4]),
        .O(b1_1_fu_219_p2[4]));
  (* SOFT_HLUTNM = "soft_lutpair58" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[5]_i_1 
       (.I0(b0_reg_330[5]),
        .I1(shl_ln105_reg_357[5]),
        .O(b1_1_fu_219_p2[5]));
  (* SOFT_HLUTNM = "soft_lutpair59" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[6]_i_1 
       (.I0(b0_reg_330[6]),
        .I1(or_ln_fu_207_p3[6]),
        .O(b1_1_fu_219_p2[6]));
  (* SOFT_HLUTNM = "soft_lutpair60" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[7]_i_1 
       (.I0(b0_reg_330[7]),
        .I1(or_ln_fu_207_p3[7]),
        .O(b1_1_fu_219_p2[7]));
  (* SOFT_HLUTNM = "soft_lutpair61" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[8]_i_1 
       (.I0(b0_reg_330[8]),
        .I1(or_ln_fu_207_p3[8]),
        .O(b1_1_fu_219_p2[8]));
  (* SOFT_HLUTNM = "soft_lutpair62" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \b1_1_reg_377[9]_i_1 
       (.I0(b0_reg_330[9]),
        .I1(or_ln_fu_207_p3[9]),
        .O(b1_1_fu_219_p2[9]));
  FDRE \b1_1_reg_377_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[0]),
        .Q(b1_1_reg_377[0]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[10]),
        .Q(b1_1_reg_377[10]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[11] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[11]),
        .Q(b1_1_reg_377[11]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[12] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[12]),
        .Q(b1_1_reg_377[12]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[13] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[13]),
        .Q(b1_1_reg_377[13]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[14] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[14]),
        .Q(b1_1_reg_377[14]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[15] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[15]),
        .Q(b1_1_reg_377[15]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[16] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[16]),
        .Q(b1_1_reg_377[16]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[17] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[17]),
        .Q(b1_1_reg_377[17]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[18] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[18]),
        .Q(b1_1_reg_377[18]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[19] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[19]),
        .Q(b1_1_reg_377[19]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[1]),
        .Q(b1_1_reg_377[1]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[20] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[20]),
        .Q(b1_1_reg_377[20]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[21] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[21]),
        .Q(b1_1_reg_377[21]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[22] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[22]),
        .Q(b1_1_reg_377[22]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[23] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[23]),
        .Q(b1_1_reg_377[23]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[24] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[24]),
        .Q(b1_1_reg_377[24]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[25] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[25]),
        .Q(b1_1_reg_377[25]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[26] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[26]),
        .Q(b1_1_reg_377[26]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[27] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[27]),
        .Q(b1_1_reg_377[27]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[28] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[28]),
        .Q(b1_1_reg_377[28]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[29] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[29]),
        .Q(b1_1_reg_377[29]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[2]),
        .Q(b1_1_reg_377[2]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[30] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[30]),
        .Q(b1_1_reg_377[30]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[31] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[31]),
        .Q(b1_1_reg_377[31]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[3]),
        .Q(b1_1_reg_377[3]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[4]),
        .Q(b1_1_reg_377[4]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[5]),
        .Q(b1_1_reg_377[5]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[6]),
        .Q(b1_1_reg_377[6]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[7]),
        .Q(b1_1_reg_377[7]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[8]),
        .Q(b1_1_reg_377[8]),
        .R(1'b0));
  FDRE \b1_1_reg_377_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state44),
        .D(b1_1_fu_219_p2[9]),
        .Q(b1_1_reg_377[9]),
        .R(1'b0));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[11]_i_2 
       (.I0(\shl_ln108_reg_393_reg_n_1_[11] ),
        .I1(b1_1_reg_377[11]),
        .O(\b1_2_reg_409[11]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[11]_i_3 
       (.I0(\shl_ln108_reg_393_reg_n_1_[10] ),
        .I1(b1_1_reg_377[10]),
        .O(\b1_2_reg_409[11]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[11]_i_4 
       (.I0(\shl_ln108_reg_393_reg_n_1_[9] ),
        .I1(b1_1_reg_377[9]),
        .O(\b1_2_reg_409[11]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[11]_i_5 
       (.I0(\shl_ln108_reg_393_reg_n_1_[8] ),
        .I1(b1_1_reg_377[8]),
        .O(\b1_2_reg_409[11]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[15]_i_2 
       (.I0(\shl_ln108_reg_393_reg_n_1_[15] ),
        .I1(b1_1_reg_377[15]),
        .O(\b1_2_reg_409[15]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[15]_i_3 
       (.I0(\shl_ln108_reg_393_reg_n_1_[14] ),
        .I1(b1_1_reg_377[14]),
        .O(\b1_2_reg_409[15]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[15]_i_4 
       (.I0(\shl_ln108_reg_393_reg_n_1_[13] ),
        .I1(b1_1_reg_377[13]),
        .O(\b1_2_reg_409[15]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[15]_i_5 
       (.I0(\shl_ln108_reg_393_reg_n_1_[12] ),
        .I1(b1_1_reg_377[12]),
        .O(\b1_2_reg_409[15]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[19]_i_2 
       (.I0(\shl_ln108_reg_393_reg_n_1_[19] ),
        .I1(b1_1_reg_377[19]),
        .O(\b1_2_reg_409[19]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[19]_i_3 
       (.I0(\shl_ln108_reg_393_reg_n_1_[18] ),
        .I1(b1_1_reg_377[18]),
        .O(\b1_2_reg_409[19]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[19]_i_4 
       (.I0(\shl_ln108_reg_393_reg_n_1_[17] ),
        .I1(b1_1_reg_377[17]),
        .O(\b1_2_reg_409[19]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[19]_i_5 
       (.I0(\shl_ln108_reg_393_reg_n_1_[16] ),
        .I1(b1_1_reg_377[16]),
        .O(\b1_2_reg_409[19]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[23]_i_2 
       (.I0(\shl_ln108_reg_393_reg_n_1_[23] ),
        .I1(b1_1_reg_377[23]),
        .O(\b1_2_reg_409[23]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[23]_i_3 
       (.I0(\shl_ln108_reg_393_reg_n_1_[22] ),
        .I1(b1_1_reg_377[22]),
        .O(\b1_2_reg_409[23]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[23]_i_4 
       (.I0(\shl_ln108_reg_393_reg_n_1_[21] ),
        .I1(b1_1_reg_377[21]),
        .O(\b1_2_reg_409[23]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[23]_i_5 
       (.I0(\shl_ln108_reg_393_reg_n_1_[20] ),
        .I1(b1_1_reg_377[20]),
        .O(\b1_2_reg_409[23]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[27]_i_2 
       (.I0(\shl_ln108_reg_393_reg_n_1_[27] ),
        .I1(b1_1_reg_377[27]),
        .O(\b1_2_reg_409[27]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[27]_i_3 
       (.I0(\shl_ln108_reg_393_reg_n_1_[26] ),
        .I1(b1_1_reg_377[26]),
        .O(\b1_2_reg_409[27]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[27]_i_4 
       (.I0(\shl_ln108_reg_393_reg_n_1_[25] ),
        .I1(b1_1_reg_377[25]),
        .O(\b1_2_reg_409[27]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[27]_i_5 
       (.I0(\shl_ln108_reg_393_reg_n_1_[24] ),
        .I1(b1_1_reg_377[24]),
        .O(\b1_2_reg_409[27]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[31]_i_2 
       (.I0(b1_1_reg_377[31]),
        .I1(\shl_ln108_reg_393_reg_n_1_[31] ),
        .O(\b1_2_reg_409[31]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[31]_i_3 
       (.I0(\shl_ln108_reg_393_reg_n_1_[30] ),
        .I1(b1_1_reg_377[30]),
        .O(\b1_2_reg_409[31]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[31]_i_4 
       (.I0(\shl_ln108_reg_393_reg_n_1_[29] ),
        .I1(b1_1_reg_377[29]),
        .O(\b1_2_reg_409[31]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[31]_i_5 
       (.I0(\shl_ln108_reg_393_reg_n_1_[28] ),
        .I1(b1_1_reg_377[28]),
        .O(\b1_2_reg_409[31]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[3]_i_2 
       (.I0(\shl_ln108_reg_393_reg_n_1_[3] ),
        .I1(b1_1_reg_377[3]),
        .O(\b1_2_reg_409[3]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[3]_i_3 
       (.I0(\shl_ln108_reg_393_reg_n_1_[2] ),
        .I1(b1_1_reg_377[2]),
        .O(\b1_2_reg_409[3]_i_3_n_1 ));
  LUT5 #(
    .INIT(32'h5554AAAB)) 
    \b1_2_reg_409[3]_i_4 
       (.I0(\shl_ln108_reg_393_reg_n_1_[1] ),
        .I1(trunc_ln108_reg_388[5]),
        .I2(trunc_ln108_reg_388[0]),
        .I3(\shl_ln108_reg_393[16]_i_2_n_1 ),
        .I4(b1_1_reg_377[1]),
        .O(\b1_2_reg_409[3]_i_4_n_1 ));
  LUT4 #(
    .INIT(16'h54AB)) 
    \b1_2_reg_409[3]_i_5 
       (.I0(\shl_ln108_reg_393_reg_n_1_[0] ),
        .I1(trunc_ln108_reg_388[5]),
        .I2(\shl_ln108_reg_393[16]_i_2_n_1 ),
        .I3(b1_1_reg_377[0]),
        .O(\b1_2_reg_409[3]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[7]_i_2 
       (.I0(\shl_ln108_reg_393_reg_n_1_[7] ),
        .I1(b1_1_reg_377[7]),
        .O(\b1_2_reg_409[7]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[7]_i_3 
       (.I0(\shl_ln108_reg_393_reg_n_1_[6] ),
        .I1(b1_1_reg_377[6]),
        .O(\b1_2_reg_409[7]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[7]_i_4 
       (.I0(\shl_ln108_reg_393_reg_n_1_[5] ),
        .I1(b1_1_reg_377[5]),
        .O(\b1_2_reg_409[7]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \b1_2_reg_409[7]_i_5 
       (.I0(\shl_ln108_reg_393_reg_n_1_[4] ),
        .I1(b1_1_reg_377[4]),
        .O(\b1_2_reg_409[7]_i_5_n_1 ));
  FDRE \b1_2_reg_409_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[0]),
        .Q(buffer_r_d1[0]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[10]),
        .Q(buffer_r_d1[10]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[11] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[11]),
        .Q(buffer_r_d1[11]),
        .R(1'b0));
  CARRY4 \b1_2_reg_409_reg[11]_i_1 
       (.CI(\b1_2_reg_409_reg[7]_i_1_n_1 ),
        .CO({\b1_2_reg_409_reg[11]_i_1_n_1 ,\b1_2_reg_409_reg[11]_i_1_n_2 ,\b1_2_reg_409_reg[11]_i_1_n_3 ,\b1_2_reg_409_reg[11]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({\shl_ln108_reg_393_reg_n_1_[11] ,\shl_ln108_reg_393_reg_n_1_[10] ,\shl_ln108_reg_393_reg_n_1_[9] ,\shl_ln108_reg_393_reg_n_1_[8] }),
        .O(b1_2_fu_292_p2[11:8]),
        .S({\b1_2_reg_409[11]_i_2_n_1 ,\b1_2_reg_409[11]_i_3_n_1 ,\b1_2_reg_409[11]_i_4_n_1 ,\b1_2_reg_409[11]_i_5_n_1 }));
  FDRE \b1_2_reg_409_reg[12] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[12]),
        .Q(buffer_r_d1[12]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[13] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[13]),
        .Q(buffer_r_d1[13]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[14] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[14]),
        .Q(buffer_r_d1[14]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[15] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[15]),
        .Q(buffer_r_d1[15]),
        .R(1'b0));
  CARRY4 \b1_2_reg_409_reg[15]_i_1 
       (.CI(\b1_2_reg_409_reg[11]_i_1_n_1 ),
        .CO({\b1_2_reg_409_reg[15]_i_1_n_1 ,\b1_2_reg_409_reg[15]_i_1_n_2 ,\b1_2_reg_409_reg[15]_i_1_n_3 ,\b1_2_reg_409_reg[15]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({\shl_ln108_reg_393_reg_n_1_[15] ,\shl_ln108_reg_393_reg_n_1_[14] ,\shl_ln108_reg_393_reg_n_1_[13] ,\shl_ln108_reg_393_reg_n_1_[12] }),
        .O(b1_2_fu_292_p2[15:12]),
        .S({\b1_2_reg_409[15]_i_2_n_1 ,\b1_2_reg_409[15]_i_3_n_1 ,\b1_2_reg_409[15]_i_4_n_1 ,\b1_2_reg_409[15]_i_5_n_1 }));
  FDRE \b1_2_reg_409_reg[16] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[16]),
        .Q(buffer_r_d1[16]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[17] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[17]),
        .Q(buffer_r_d1[17]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[18] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[18]),
        .Q(buffer_r_d1[18]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[19] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[19]),
        .Q(buffer_r_d1[19]),
        .R(1'b0));
  CARRY4 \b1_2_reg_409_reg[19]_i_1 
       (.CI(\b1_2_reg_409_reg[15]_i_1_n_1 ),
        .CO({\b1_2_reg_409_reg[19]_i_1_n_1 ,\b1_2_reg_409_reg[19]_i_1_n_2 ,\b1_2_reg_409_reg[19]_i_1_n_3 ,\b1_2_reg_409_reg[19]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({\shl_ln108_reg_393_reg_n_1_[19] ,\shl_ln108_reg_393_reg_n_1_[18] ,\shl_ln108_reg_393_reg_n_1_[17] ,\shl_ln108_reg_393_reg_n_1_[16] }),
        .O(b1_2_fu_292_p2[19:16]),
        .S({\b1_2_reg_409[19]_i_2_n_1 ,\b1_2_reg_409[19]_i_3_n_1 ,\b1_2_reg_409[19]_i_4_n_1 ,\b1_2_reg_409[19]_i_5_n_1 }));
  FDRE \b1_2_reg_409_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[1]),
        .Q(buffer_r_d1[1]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[20] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[20]),
        .Q(buffer_r_d1[20]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[21] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[21]),
        .Q(buffer_r_d1[21]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[22] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[22]),
        .Q(buffer_r_d1[22]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[23] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[23]),
        .Q(buffer_r_d1[23]),
        .R(1'b0));
  CARRY4 \b1_2_reg_409_reg[23]_i_1 
       (.CI(\b1_2_reg_409_reg[19]_i_1_n_1 ),
        .CO({\b1_2_reg_409_reg[23]_i_1_n_1 ,\b1_2_reg_409_reg[23]_i_1_n_2 ,\b1_2_reg_409_reg[23]_i_1_n_3 ,\b1_2_reg_409_reg[23]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({\shl_ln108_reg_393_reg_n_1_[23] ,\shl_ln108_reg_393_reg_n_1_[22] ,\shl_ln108_reg_393_reg_n_1_[21] ,\shl_ln108_reg_393_reg_n_1_[20] }),
        .O(b1_2_fu_292_p2[23:20]),
        .S({\b1_2_reg_409[23]_i_2_n_1 ,\b1_2_reg_409[23]_i_3_n_1 ,\b1_2_reg_409[23]_i_4_n_1 ,\b1_2_reg_409[23]_i_5_n_1 }));
  FDRE \b1_2_reg_409_reg[24] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[24]),
        .Q(buffer_r_d1[24]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[25] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[25]),
        .Q(buffer_r_d1[25]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[26] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[26]),
        .Q(buffer_r_d1[26]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[27] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[27]),
        .Q(buffer_r_d1[27]),
        .R(1'b0));
  CARRY4 \b1_2_reg_409_reg[27]_i_1 
       (.CI(\b1_2_reg_409_reg[23]_i_1_n_1 ),
        .CO({\b1_2_reg_409_reg[27]_i_1_n_1 ,\b1_2_reg_409_reg[27]_i_1_n_2 ,\b1_2_reg_409_reg[27]_i_1_n_3 ,\b1_2_reg_409_reg[27]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({\shl_ln108_reg_393_reg_n_1_[27] ,\shl_ln108_reg_393_reg_n_1_[26] ,\shl_ln108_reg_393_reg_n_1_[25] ,\shl_ln108_reg_393_reg_n_1_[24] }),
        .O(b1_2_fu_292_p2[27:24]),
        .S({\b1_2_reg_409[27]_i_2_n_1 ,\b1_2_reg_409[27]_i_3_n_1 ,\b1_2_reg_409[27]_i_4_n_1 ,\b1_2_reg_409[27]_i_5_n_1 }));
  FDRE \b1_2_reg_409_reg[28] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[28]),
        .Q(buffer_r_d1[28]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[29] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[29]),
        .Q(buffer_r_d1[29]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[2]),
        .Q(buffer_r_d1[2]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[30] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[30]),
        .Q(buffer_r_d1[30]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[31] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[31]),
        .Q(buffer_r_d1[31]),
        .R(1'b0));
  CARRY4 \b1_2_reg_409_reg[31]_i_1 
       (.CI(\b1_2_reg_409_reg[27]_i_1_n_1 ),
        .CO({\NLW_b1_2_reg_409_reg[31]_i_1_CO_UNCONNECTED [3],\b1_2_reg_409_reg[31]_i_1_n_2 ,\b1_2_reg_409_reg[31]_i_1_n_3 ,\b1_2_reg_409_reg[31]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,\shl_ln108_reg_393_reg_n_1_[30] ,\shl_ln108_reg_393_reg_n_1_[29] ,\shl_ln108_reg_393_reg_n_1_[28] }),
        .O(b1_2_fu_292_p2[31:28]),
        .S({\b1_2_reg_409[31]_i_2_n_1 ,\b1_2_reg_409[31]_i_3_n_1 ,\b1_2_reg_409[31]_i_4_n_1 ,\b1_2_reg_409[31]_i_5_n_1 }));
  FDRE \b1_2_reg_409_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[3]),
        .Q(buffer_r_d1[3]),
        .R(1'b0));
  CARRY4 \b1_2_reg_409_reg[3]_i_1 
       (.CI(1'b0),
        .CO({\b1_2_reg_409_reg[3]_i_1_n_1 ,\b1_2_reg_409_reg[3]_i_1_n_2 ,\b1_2_reg_409_reg[3]_i_1_n_3 ,\b1_2_reg_409_reg[3]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({\shl_ln108_reg_393_reg_n_1_[3] ,\shl_ln108_reg_393_reg_n_1_[2] ,b1_1_reg_377[1:0]}),
        .O(b1_2_fu_292_p2[3:0]),
        .S({\b1_2_reg_409[3]_i_2_n_1 ,\b1_2_reg_409[3]_i_3_n_1 ,\b1_2_reg_409[3]_i_4_n_1 ,\b1_2_reg_409[3]_i_5_n_1 }));
  FDRE \b1_2_reg_409_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[4]),
        .Q(buffer_r_d1[4]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[5]),
        .Q(buffer_r_d1[5]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[6]),
        .Q(buffer_r_d1[6]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[7]),
        .Q(buffer_r_d1[7]),
        .R(1'b0));
  CARRY4 \b1_2_reg_409_reg[7]_i_1 
       (.CI(\b1_2_reg_409_reg[3]_i_1_n_1 ),
        .CO({\b1_2_reg_409_reg[7]_i_1_n_1 ,\b1_2_reg_409_reg[7]_i_1_n_2 ,\b1_2_reg_409_reg[7]_i_1_n_3 ,\b1_2_reg_409_reg[7]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({\shl_ln108_reg_393_reg_n_1_[7] ,\shl_ln108_reg_393_reg_n_1_[6] ,\shl_ln108_reg_393_reg_n_1_[5] ,\shl_ln108_reg_393_reg_n_1_[4] }),
        .O(b1_2_fu_292_p2[7:4]),
        .S({\b1_2_reg_409[7]_i_2_n_1 ,\b1_2_reg_409[7]_i_3_n_1 ,\b1_2_reg_409[7]_i_4_n_1 ,\b1_2_reg_409[7]_i_5_n_1 }));
  FDRE \b1_2_reg_409_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[8]),
        .Q(buffer_r_d1[8]),
        .R(1'b0));
  FDRE \b1_2_reg_409_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state82),
        .D(b1_2_fu_292_p2[9]),
        .Q(buffer_r_d1[9]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[0]),
        .Q(b1_reg_336[0]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[10]),
        .Q(b1_reg_336[10]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[11] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[11]),
        .Q(b1_reg_336[11]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[12] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[12]),
        .Q(b1_reg_336[12]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[13] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[13]),
        .Q(b1_reg_336[13]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[14] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[14]),
        .Q(b1_reg_336[14]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[15] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[15]),
        .Q(b1_reg_336[15]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[16] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[16]),
        .Q(b1_reg_336[16]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[17] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[17]),
        .Q(b1_reg_336[17]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[18] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[18]),
        .Q(b1_reg_336[18]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[19] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[19]),
        .Q(b1_reg_336[19]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[1]),
        .Q(b1_reg_336[1]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[20] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[20]),
        .Q(b1_reg_336[20]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[21] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[21]),
        .Q(b1_reg_336[21]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[22] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[22]),
        .Q(b1_reg_336[22]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[23] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[23]),
        .Q(b1_reg_336[23]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[24] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[24]),
        .Q(b1_reg_336[24]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[25] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[25]),
        .Q(b1_reg_336[25]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[26] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[26]),
        .Q(b1_reg_336[26]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[27] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[27]),
        .Q(b1_reg_336[27]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[28] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[28]),
        .Q(b1_reg_336[28]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[29] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[29]),
        .Q(b1_reg_336[29]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[2]),
        .Q(b1_reg_336[2]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[30] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[30]),
        .Q(b1_reg_336[30]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[31] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[31]),
        .Q(b1_reg_336[31]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[3]),
        .Q(b1_reg_336[3]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[4]),
        .Q(b1_reg_336[4]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[5]),
        .Q(b1_reg_336[5]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[6]),
        .Q(b1_reg_336[6]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[7]),
        .Q(b1_reg_336[7]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[8]),
        .Q(b1_reg_336[8]),
        .R(1'b0));
  FDRE \b1_reg_336_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(buffer_r_q1[9]),
        .Q(b1_reg_336[9]),
        .R(1'b0));
  FDRE \buffer_addr_reg_320_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(M_q0[0]),
        .Q(buffer_r_address0[0]),
        .R(1'b0));
  FDRE \buffer_addr_reg_320_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(M_q0[10]),
        .Q(buffer_r_address0[10]),
        .R(1'b0));
  FDRE \buffer_addr_reg_320_reg[11] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(M_q0[11]),
        .Q(buffer_r_address0[11]),
        .R(1'b0));
  FDRE \buffer_addr_reg_320_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(M_q0[1]),
        .Q(buffer_r_address0[1]),
        .R(1'b0));
  FDRE \buffer_addr_reg_320_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(M_q0[2]),
        .Q(buffer_r_address0[2]),
        .R(1'b0));
  FDRE \buffer_addr_reg_320_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(M_q0[3]),
        .Q(buffer_r_address0[3]),
        .R(1'b0));
  FDRE \buffer_addr_reg_320_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(M_q0[4]),
        .Q(buffer_r_address0[4]),
        .R(1'b0));
  FDRE \buffer_addr_reg_320_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(M_q0[5]),
        .Q(buffer_r_address0[5]),
        .R(1'b0));
  FDRE \buffer_addr_reg_320_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(M_q0[6]),
        .Q(buffer_r_address0[6]),
        .R(1'b0));
  FDRE \buffer_addr_reg_320_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(M_q0[7]),
        .Q(buffer_r_address0[7]),
        .R(1'b0));
  FDRE \buffer_addr_reg_320_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(M_q0[8]),
        .Q(buffer_r_address0[8]),
        .R(1'b0));
  FDRE \buffer_addr_reg_320_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(M_q0[9]),
        .Q(buffer_r_address0[9]),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair87" *) 
  LUT2 #(
    .INIT(4'hE)) 
    buffer_r_ce0_INST_0
       (.I0(buffer_r_we0),
        .I1(ap_CS_fsm_state5),
        .O(buffer_r_ce0));
  (* SOFT_HLUTNM = "soft_lutpair87" *) 
  LUT2 #(
    .INIT(4'hE)) 
    buffer_r_ce1_INST_0
       (.I0(buffer_r_we1),
        .I1(ap_CS_fsm_state5),
        .O(buffer_r_ce1));
  LUT3 #(
    .INIT(8'h40)) 
    \i_0_reg_111[10]_i_1 
       (.I0(buffer_r_we1),
        .I1(\ap_CS_fsm_reg_n_1_[0] ),
        .I2(ap_start),
        .O(i_0_reg_111));
  FDRE \i_0_reg_111_reg[0] 
       (.C(ap_clk),
        .CE(buffer_r_we1),
        .D(i_reg_404[0]),
        .Q(\i_0_reg_111_reg_n_1_[0] ),
        .R(i_0_reg_111));
  FDRE \i_0_reg_111_reg[10] 
       (.C(ap_clk),
        .CE(buffer_r_we1),
        .D(i_reg_404[10]),
        .Q(\^M_address0 [10]),
        .R(i_0_reg_111));
  FDRE \i_0_reg_111_reg[1] 
       (.C(ap_clk),
        .CE(buffer_r_we1),
        .D(i_reg_404[1]),
        .Q(\^M_address0 [1]),
        .R(i_0_reg_111));
  FDRE \i_0_reg_111_reg[2] 
       (.C(ap_clk),
        .CE(buffer_r_we1),
        .D(i_reg_404[2]),
        .Q(\^M_address0 [2]),
        .R(i_0_reg_111));
  FDRE \i_0_reg_111_reg[3] 
       (.C(ap_clk),
        .CE(buffer_r_we1),
        .D(i_reg_404[3]),
        .Q(\^M_address0 [3]),
        .R(i_0_reg_111));
  FDRE \i_0_reg_111_reg[4] 
       (.C(ap_clk),
        .CE(buffer_r_we1),
        .D(i_reg_404[4]),
        .Q(\^M_address0 [4]),
        .R(i_0_reg_111));
  FDRE \i_0_reg_111_reg[5] 
       (.C(ap_clk),
        .CE(buffer_r_we1),
        .D(i_reg_404[5]),
        .Q(\^M_address0 [5]),
        .R(i_0_reg_111));
  FDRE \i_0_reg_111_reg[6] 
       (.C(ap_clk),
        .CE(buffer_r_we1),
        .D(i_reg_404[6]),
        .Q(\^M_address0 [6]),
        .R(i_0_reg_111));
  FDRE \i_0_reg_111_reg[7] 
       (.C(ap_clk),
        .CE(buffer_r_we1),
        .D(i_reg_404[7]),
        .Q(\^M_address0 [7]),
        .R(i_0_reg_111));
  FDRE \i_0_reg_111_reg[8] 
       (.C(ap_clk),
        .CE(buffer_r_we1),
        .D(i_reg_404[8]),
        .Q(\^M_address0 [8]),
        .R(i_0_reg_111));
  FDRE \i_0_reg_111_reg[9] 
       (.C(ap_clk),
        .CE(buffer_r_we1),
        .D(i_reg_404[9]),
        .Q(\^M_address0 [9]),
        .R(i_0_reg_111));
  LUT6 #(
    .INIT(64'hBFFFFFFF40000000)) 
    \i_reg_404[10]_i_1 
       (.I0(\i_reg_404[10]_i_2_n_1 ),
        .I1(\^M_address0 [8]),
        .I2(\^M_address0 [6]),
        .I3(\^M_address0 [7]),
        .I4(\^M_address0 [9]),
        .I5(\^M_address0 [10]),
        .O(i_fu_249_p2[10]));
  (* SOFT_HLUTNM = "soft_lutpair32" *) 
  LUT5 #(
    .INIT(32'h7FFFFFFF)) 
    \i_reg_404[10]_i_2 
       (.I0(\^M_address0 [5]),
        .I1(\^M_address0 [3]),
        .I2(\^M_address0 [2]),
        .I3(\^M_address0 [4]),
        .I4(\^M_address0 [1]),
        .O(\i_reg_404[10]_i_2_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair53" *) 
  LUT1 #(
    .INIT(2'h1)) 
    \i_reg_404[1]_i_1 
       (.I0(\^M_address0 [1]),
        .O(i_fu_249_p2[1]));
  (* SOFT_HLUTNM = "soft_lutpair53" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \i_reg_404[2]_i_1 
       (.I0(\^M_address0 [1]),
        .I1(\^M_address0 [2]),
        .O(i_fu_249_p2[2]));
  (* SOFT_HLUTNM = "soft_lutpair39" *) 
  LUT3 #(
    .INIT(8'h78)) 
    \i_reg_404[3]_i_1 
       (.I0(\^M_address0 [2]),
        .I1(\^M_address0 [1]),
        .I2(\^M_address0 [3]),
        .O(i_fu_249_p2[3]));
  (* SOFT_HLUTNM = "soft_lutpair39" *) 
  LUT4 #(
    .INIT(16'h7F80)) 
    \i_reg_404[4]_i_1 
       (.I0(\^M_address0 [1]),
        .I1(\^M_address0 [2]),
        .I2(\^M_address0 [3]),
        .I3(\^M_address0 [4]),
        .O(i_fu_249_p2[4]));
  (* SOFT_HLUTNM = "soft_lutpair32" *) 
  LUT5 #(
    .INIT(32'h7FFF8000)) 
    \i_reg_404[5]_i_1 
       (.I0(\^M_address0 [1]),
        .I1(\^M_address0 [3]),
        .I2(\^M_address0 [2]),
        .I3(\^M_address0 [4]),
        .I4(\^M_address0 [5]),
        .O(i_fu_249_p2[5]));
  LUT6 #(
    .INIT(64'h7FFFFFFF80000000)) 
    \i_reg_404[6]_i_1 
       (.I0(\^M_address0 [1]),
        .I1(\^M_address0 [4]),
        .I2(\^M_address0 [2]),
        .I3(\^M_address0 [3]),
        .I4(\^M_address0 [5]),
        .I5(\^M_address0 [6]),
        .O(i_fu_249_p2[6]));
  (* SOFT_HLUTNM = "soft_lutpair33" *) 
  LUT4 #(
    .INIT(16'hDF20)) 
    \i_reg_404[7]_i_1 
       (.I0(\^M_address0 [6]),
        .I1(\add_ln101_reg_305[10]_i_3_n_1 ),
        .I2(\^M_address0 [1]),
        .I3(\^M_address0 [7]),
        .O(i_fu_249_p2[7]));
  (* SOFT_HLUTNM = "soft_lutpair33" *) 
  LUT5 #(
    .INIT(32'hF7FF0800)) 
    \i_reg_404[8]_i_1 
       (.I0(\^M_address0 [6]),
        .I1(\^M_address0 [7]),
        .I2(\add_ln101_reg_305[10]_i_3_n_1 ),
        .I3(\^M_address0 [1]),
        .I4(\^M_address0 [8]),
        .O(i_fu_249_p2[8]));
  LUT6 #(
    .INIT(64'hFF7FFFFF00800000)) 
    \i_reg_404[9]_i_1 
       (.I0(\^M_address0 [7]),
        .I1(\^M_address0 [6]),
        .I2(\^M_address0 [8]),
        .I3(\add_ln101_reg_305[10]_i_3_n_1 ),
        .I4(\^M_address0 [1]),
        .I5(\^M_address0 [9]),
        .O(i_fu_249_p2[9]));
  FDRE \i_reg_404_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\i_0_reg_111_reg_n_1_[0] ),
        .Q(i_reg_404[0]),
        .R(1'b0));
  FDRE \i_reg_404_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(i_fu_249_p2[10]),
        .Q(i_reg_404[10]),
        .R(1'b0));
  FDRE \i_reg_404_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(i_fu_249_p2[1]),
        .Q(i_reg_404[1]),
        .R(1'b0));
  FDRE \i_reg_404_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(i_fu_249_p2[2]),
        .Q(i_reg_404[2]),
        .R(1'b0));
  FDRE \i_reg_404_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(i_fu_249_p2[3]),
        .Q(i_reg_404[3]),
        .R(1'b0));
  FDRE \i_reg_404_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(i_fu_249_p2[4]),
        .Q(i_reg_404[4]),
        .R(1'b0));
  FDRE \i_reg_404_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(i_fu_249_p2[5]),
        .Q(i_reg_404[5]),
        .R(1'b0));
  FDRE \i_reg_404_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(i_fu_249_p2[6]),
        .Q(i_reg_404[6]),
        .R(1'b0));
  FDRE \i_reg_404_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(i_fu_249_p2[7]),
        .Q(i_reg_404[7]),
        .R(1'b0));
  FDRE \i_reg_404_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(i_fu_249_p2[8]),
        .Q(i_reg_404[8]),
        .R(1'b0));
  FDRE \i_reg_404_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(i_fu_249_p2[9]),
        .Q(i_reg_404[9]),
        .R(1'b0));
  bd_0_hls_inst_0_loop_imperfect_srbkb loop_imperfect_srbkb_U1
       (.D(b0_1_fu_159_p2),
        .Q(grp_fu_163_ap_start),
        .ap_clk(ap_clk),
        .ap_rst(ap_rst),
        .\dividend0_reg[31] (b0_reg_330),
        .\dividend0_reg[31]_0 (b1_reg_336),
        .r_stage_reg_r_29(loop_imperfect_srbkb_U1_n_1),
        .\remd_reg[31] (remd));
  bd_0_hls_inst_0_loop_imperfect_srbkb_0 loop_imperfect_srbkb_U2
       (.Q(buffer_r_we0),
        .ap_clk(ap_clk),
        .ap_rst(ap_rst),
        .\dividend0_reg[31] (b1_1_reg_377),
        .\r_stage_reg[32] (loop_imperfect_srbkb_U1_n_1),
        .\remd_reg[31] ({loop_imperfect_srbkb_U2_n_1,loop_imperfect_srbkb_U2_n_2,loop_imperfect_srbkb_U2_n_3,loop_imperfect_srbkb_U2_n_4,loop_imperfect_srbkb_U2_n_5,loop_imperfect_srbkb_U2_n_6,loop_imperfect_srbkb_U2_n_7,loop_imperfect_srbkb_U2_n_8,loop_imperfect_srbkb_U2_n_9,loop_imperfect_srbkb_U2_n_10,loop_imperfect_srbkb_U2_n_11,loop_imperfect_srbkb_U2_n_12,loop_imperfect_srbkb_U2_n_13,loop_imperfect_srbkb_U2_n_14,loop_imperfect_srbkb_U2_n_15,loop_imperfect_srbkb_U2_n_16,loop_imperfect_srbkb_U2_n_17,loop_imperfect_srbkb_U2_n_18,loop_imperfect_srbkb_U2_n_19,loop_imperfect_srbkb_U2_n_20,loop_imperfect_srbkb_U2_n_21,loop_imperfect_srbkb_U2_n_22,loop_imperfect_srbkb_U2_n_23,loop_imperfect_srbkb_U2_n_24,loop_imperfect_srbkb_U2_n_25,loop_imperfect_srbkb_U2_n_26,loop_imperfect_srbkb_U2_n_27,loop_imperfect_srbkb_U2_n_28,loop_imperfect_srbkb_U2_n_29,loop_imperfect_srbkb_U2_n_30,loop_imperfect_srbkb_U2_n_31,loop_imperfect_srbkb_U2_n_32}));
  (* SOFT_HLUTNM = "soft_lutpair48" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \reg_123[0]_i_1 
       (.I0(M_q0[0]),
        .I1(ap_CS_fsm_state82),
        .I2(M_q1[0]),
        .O(\reg_123[0]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair45" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \reg_123[10]_i_1 
       (.I0(M_q0[10]),
        .I1(ap_CS_fsm_state82),
        .I2(M_q1[10]),
        .O(\reg_123[10]_i_1_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \reg_123[11]_i_1 
       (.I0(ap_CS_fsm_state82),
        .I1(ap_CS_fsm_state4),
        .O(\reg_123[11]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair43" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \reg_123[11]_i_2 
       (.I0(M_q0[11]),
        .I1(ap_CS_fsm_state82),
        .I2(M_q1[11]),
        .O(\reg_123[11]_i_2_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair49" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \reg_123[1]_i_1 
       (.I0(M_q0[1]),
        .I1(ap_CS_fsm_state82),
        .I2(M_q1[1]),
        .O(\reg_123[1]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair49" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \reg_123[2]_i_1 
       (.I0(M_q0[2]),
        .I1(ap_CS_fsm_state82),
        .I2(M_q1[2]),
        .O(\reg_123[2]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair48" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \reg_123[3]_i_1 
       (.I0(M_q0[3]),
        .I1(ap_CS_fsm_state82),
        .I2(M_q1[3]),
        .O(\reg_123[3]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair47" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \reg_123[4]_i_1 
       (.I0(M_q0[4]),
        .I1(ap_CS_fsm_state82),
        .I2(M_q1[4]),
        .O(\reg_123[4]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair47" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \reg_123[5]_i_1 
       (.I0(M_q0[5]),
        .I1(ap_CS_fsm_state82),
        .I2(M_q1[5]),
        .O(\reg_123[5]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair46" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \reg_123[6]_i_1 
       (.I0(M_q0[6]),
        .I1(ap_CS_fsm_state82),
        .I2(M_q1[6]),
        .O(\reg_123[6]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair45" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \reg_123[7]_i_1 
       (.I0(M_q0[7]),
        .I1(ap_CS_fsm_state82),
        .I2(M_q1[7]),
        .O(\reg_123[7]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair43" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \reg_123[8]_i_1 
       (.I0(M_q0[8]),
        .I1(ap_CS_fsm_state82),
        .I2(M_q1[8]),
        .O(\reg_123[8]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair46" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \reg_123[9]_i_1 
       (.I0(M_q0[9]),
        .I1(ap_CS_fsm_state82),
        .I2(M_q1[9]),
        .O(\reg_123[9]_i_1_n_1 ));
  FDRE \reg_123_reg[0] 
       (.C(ap_clk),
        .CE(\reg_123[11]_i_1_n_1 ),
        .D(\reg_123[0]_i_1_n_1 ),
        .Q(buffer_r_address1[0]),
        .R(1'b0));
  FDRE \reg_123_reg[10] 
       (.C(ap_clk),
        .CE(\reg_123[11]_i_1_n_1 ),
        .D(\reg_123[10]_i_1_n_1 ),
        .Q(buffer_r_address1[10]),
        .R(1'b0));
  FDRE \reg_123_reg[11] 
       (.C(ap_clk),
        .CE(\reg_123[11]_i_1_n_1 ),
        .D(\reg_123[11]_i_2_n_1 ),
        .Q(buffer_r_address1[11]),
        .R(1'b0));
  FDRE \reg_123_reg[1] 
       (.C(ap_clk),
        .CE(\reg_123[11]_i_1_n_1 ),
        .D(\reg_123[1]_i_1_n_1 ),
        .Q(buffer_r_address1[1]),
        .R(1'b0));
  FDRE \reg_123_reg[2] 
       (.C(ap_clk),
        .CE(\reg_123[11]_i_1_n_1 ),
        .D(\reg_123[2]_i_1_n_1 ),
        .Q(buffer_r_address1[2]),
        .R(1'b0));
  FDRE \reg_123_reg[3] 
       (.C(ap_clk),
        .CE(\reg_123[11]_i_1_n_1 ),
        .D(\reg_123[3]_i_1_n_1 ),
        .Q(buffer_r_address1[3]),
        .R(1'b0));
  FDRE \reg_123_reg[4] 
       (.C(ap_clk),
        .CE(\reg_123[11]_i_1_n_1 ),
        .D(\reg_123[4]_i_1_n_1 ),
        .Q(buffer_r_address1[4]),
        .R(1'b0));
  FDRE \reg_123_reg[5] 
       (.C(ap_clk),
        .CE(\reg_123[11]_i_1_n_1 ),
        .D(\reg_123[5]_i_1_n_1 ),
        .Q(buffer_r_address1[5]),
        .R(1'b0));
  FDRE \reg_123_reg[6] 
       (.C(ap_clk),
        .CE(\reg_123[11]_i_1_n_1 ),
        .D(\reg_123[6]_i_1_n_1 ),
        .Q(buffer_r_address1[6]),
        .R(1'b0));
  FDRE \reg_123_reg[7] 
       (.C(ap_clk),
        .CE(\reg_123[11]_i_1_n_1 ),
        .D(\reg_123[7]_i_1_n_1 ),
        .Q(buffer_r_address1[7]),
        .R(1'b0));
  FDRE \reg_123_reg[8] 
       (.C(ap_clk),
        .CE(\reg_123[11]_i_1_n_1 ),
        .D(\reg_123[8]_i_1_n_1 ),
        .Q(buffer_r_address1[8]),
        .R(1'b0));
  FDRE \reg_123_reg[9] 
       (.C(ap_clk),
        .CE(\reg_123[11]_i_1_n_1 ),
        .D(\reg_123[9]_i_1_n_1 ),
        .Q(buffer_r_address1[9]),
        .R(1'b0));
  LUT6 #(
    .INIT(64'h0000000000000080)) 
    \shl_ln105_reg_357[1]_i_1 
       (.I0(\shl_ln105_reg_357[4]_i_2_n_1 ),
        .I1(\shl_ln105_reg_357[5]_i_4_n_1 ),
        .I2(\shl_ln105_reg_357[5]_i_5_n_1 ),
        .I3(\shl_ln105_reg_357[5]_i_6_n_1 ),
        .I4(srem_ln105_reg_347[0]),
        .I5(srem_ln105_reg_347[1]),
        .O(\shl_ln105_reg_357[1]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000800000)) 
    \shl_ln105_reg_357[2]_i_1 
       (.I0(\shl_ln105_reg_357[4]_i_2_n_1 ),
        .I1(\shl_ln105_reg_357[5]_i_4_n_1 ),
        .I2(\shl_ln105_reg_357[5]_i_5_n_1 ),
        .I3(\shl_ln105_reg_357[5]_i_6_n_1 ),
        .I4(srem_ln105_reg_347[0]),
        .I5(srem_ln105_reg_347[1]),
        .O(\shl_ln105_reg_357[2]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000800000)) 
    \shl_ln105_reg_357[3]_i_1 
       (.I0(\shl_ln105_reg_357[4]_i_2_n_1 ),
        .I1(\shl_ln105_reg_357[5]_i_4_n_1 ),
        .I2(\shl_ln105_reg_357[5]_i_5_n_1 ),
        .I3(\shl_ln105_reg_357[5]_i_6_n_1 ),
        .I4(srem_ln105_reg_347[1]),
        .I5(srem_ln105_reg_347[0]),
        .O(\shl_ln105_reg_357[3]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h0080000000000000)) 
    \shl_ln105_reg_357[4]_i_1 
       (.I0(\shl_ln105_reg_357[4]_i_2_n_1 ),
        .I1(\shl_ln105_reg_357[5]_i_4_n_1 ),
        .I2(\shl_ln105_reg_357[5]_i_5_n_1 ),
        .I3(\shl_ln105_reg_357[5]_i_6_n_1 ),
        .I4(srem_ln105_reg_347[0]),
        .I5(srem_ln105_reg_347[1]),
        .O(\shl_ln105_reg_357[4]_i_1_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \shl_ln105_reg_357[4]_i_2 
       (.I0(srem_ln105_reg_347[3]),
        .I1(srem_ln105_reg_347[2]),
        .O(\shl_ln105_reg_357[4]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h8)) 
    \shl_ln105_reg_357[5]_i_1 
       (.I0(srem_ln105_reg_347[4]),
        .I1(ap_CS_fsm_state43),
        .O(\shl_ln105_reg_357[5]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'hFFFE)) 
    \shl_ln105_reg_357[5]_i_10 
       (.I0(srem_ln105_reg_347[12]),
        .I1(srem_ln105_reg_347[15]),
        .I2(srem_ln105_reg_347[10]),
        .I3(srem_ln105_reg_347[13]),
        .O(\shl_ln105_reg_357[5]_i_10_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000080)) 
    \shl_ln105_reg_357[5]_i_2 
       (.I0(\shl_ln105_reg_357[5]_i_3_n_1 ),
        .I1(\shl_ln105_reg_357[5]_i_4_n_1 ),
        .I2(\shl_ln105_reg_357[5]_i_5_n_1 ),
        .I3(\shl_ln105_reg_357[5]_i_6_n_1 ),
        .I4(srem_ln105_reg_347[0]),
        .I5(srem_ln105_reg_347[1]),
        .O(\shl_ln105_reg_357[5]_i_2_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair50" *) 
  LUT2 #(
    .INIT(4'h2)) 
    \shl_ln105_reg_357[5]_i_3 
       (.I0(srem_ln105_reg_347[2]),
        .I1(srem_ln105_reg_347[3]),
        .O(\shl_ln105_reg_357[5]_i_3_n_1 ));
  LUT5 #(
    .INIT(32'h00010000)) 
    \shl_ln105_reg_357[5]_i_4 
       (.I0(srem_ln105_reg_347[28]),
        .I1(srem_ln105_reg_347[29]),
        .I2(srem_ln105_reg_347[30]),
        .I3(srem_ln105_reg_347[31]),
        .I4(\shl_ln105_reg_357[5]_i_7_n_1 ),
        .O(\shl_ln105_reg_357[5]_i_4_n_1 ));
  LUT5 #(
    .INIT(32'h00010000)) 
    \shl_ln105_reg_357[5]_i_5 
       (.I0(srem_ln105_reg_347[18]),
        .I1(srem_ln105_reg_347[19]),
        .I2(srem_ln105_reg_347[16]),
        .I3(srem_ln105_reg_347[17]),
        .I4(\shl_ln105_reg_357[5]_i_8_n_1 ),
        .O(\shl_ln105_reg_357[5]_i_5_n_1 ));
  LUT5 #(
    .INIT(32'hFFFFFFFE)) 
    \shl_ln105_reg_357[5]_i_6 
       (.I0(srem_ln105_reg_347[7]),
        .I1(srem_ln105_reg_347[6]),
        .I2(srem_ln105_reg_347[14]),
        .I3(\shl_ln105_reg_357[5]_i_9_n_1 ),
        .I4(\shl_ln105_reg_357[5]_i_10_n_1 ),
        .O(\shl_ln105_reg_357[5]_i_6_n_1 ));
  LUT4 #(
    .INIT(16'h0001)) 
    \shl_ln105_reg_357[5]_i_7 
       (.I0(srem_ln105_reg_347[27]),
        .I1(srem_ln105_reg_347[26]),
        .I2(srem_ln105_reg_347[25]),
        .I3(srem_ln105_reg_347[24]),
        .O(\shl_ln105_reg_357[5]_i_7_n_1 ));
  LUT4 #(
    .INIT(16'h0001)) 
    \shl_ln105_reg_357[5]_i_8 
       (.I0(srem_ln105_reg_347[23]),
        .I1(srem_ln105_reg_347[22]),
        .I2(srem_ln105_reg_347[21]),
        .I3(srem_ln105_reg_347[20]),
        .O(\shl_ln105_reg_357[5]_i_8_n_1 ));
  LUT4 #(
    .INIT(16'hFFFE)) 
    \shl_ln105_reg_357[5]_i_9 
       (.I0(srem_ln105_reg_347[8]),
        .I1(srem_ln105_reg_347[11]),
        .I2(srem_ln105_reg_347[5]),
        .I3(srem_ln105_reg_347[9]),
        .O(\shl_ln105_reg_357[5]_i_9_n_1 ));
  FDRE \shl_ln105_reg_357_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\shl_ln105_reg_357[1]_i_1_n_1 ),
        .Q(shl_ln105_reg_357[1]),
        .R(\shl_ln105_reg_357[5]_i_1_n_1 ));
  FDRE \shl_ln105_reg_357_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\shl_ln105_reg_357[2]_i_1_n_1 ),
        .Q(shl_ln105_reg_357[2]),
        .R(\shl_ln105_reg_357[5]_i_1_n_1 ));
  FDRE \shl_ln105_reg_357_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\shl_ln105_reg_357[3]_i_1_n_1 ),
        .Q(shl_ln105_reg_357[3]),
        .R(\shl_ln105_reg_357[5]_i_1_n_1 ));
  FDRE \shl_ln105_reg_357_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\shl_ln105_reg_357[4]_i_1_n_1 ),
        .Q(shl_ln105_reg_357[4]),
        .R(\shl_ln105_reg_357[5]_i_1_n_1 ));
  FDRE \shl_ln105_reg_357_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\shl_ln105_reg_357[5]_i_2_n_1 ),
        .Q(shl_ln105_reg_357[5]),
        .R(\shl_ln105_reg_357[5]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000002)) 
    \shl_ln108_reg_393[0]_i_1 
       (.I0(\shl_ln108_reg_393[0]_i_2_n_1 ),
        .I1(\shl_ln108_reg_393[16]_i_3_n_1 ),
        .I2(\shl_ln108_reg_393[16]_i_4_n_1 ),
        .I3(srem_ln108_reg_383[22]),
        .I4(srem_ln108_reg_383[17]),
        .I5(\shl_ln108_reg_393[24]_i_5_n_1 ),
        .O(shl_ln108_fu_233_p2[0]));
  (* SOFT_HLUTNM = "soft_lutpair30" *) 
  LUT5 #(
    .INIT(32'h00000001)) 
    \shl_ln108_reg_393[0]_i_2 
       (.I0(trunc_ln108_reg_388[0]),
        .I1(trunc_ln108_reg_388[1]),
        .I2(trunc_ln108_reg_388[2]),
        .I3(trunc_ln108_reg_388[4]),
        .I4(trunc_ln108_reg_388[3]),
        .O(\shl_ln108_reg_393[0]_i_2_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair84" *) 
  LUT2 #(
    .INIT(4'h2)) 
    \shl_ln108_reg_393[15]_i_1 
       (.I0(trunc_ln108_reg_388[3]),
        .I1(trunc_ln108_reg_388[4]),
        .O(\shl_ln108_reg_393[15]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \shl_ln108_reg_393[16]_i_1 
       (.I0(\shl_ln108_reg_393[16]_i_2_n_1 ),
        .I1(\shl_ln108_reg_393[16]_i_3_n_1 ),
        .I2(\shl_ln108_reg_393[16]_i_4_n_1 ),
        .I3(srem_ln108_reg_383[22]),
        .I4(srem_ln108_reg_383[17]),
        .I5(\shl_ln108_reg_393[24]_i_5_n_1 ),
        .O(shl_ln108_fu_233_p2[16]));
  (* SOFT_HLUTNM = "soft_lutpair30" *) 
  LUT5 #(
    .INIT(32'hFF7FFEFF)) 
    \shl_ln108_reg_393[16]_i_2 
       (.I0(trunc_ln108_reg_388[1]),
        .I1(trunc_ln108_reg_388[0]),
        .I2(trunc_ln108_reg_388[2]),
        .I3(trunc_ln108_reg_388[4]),
        .I4(trunc_ln108_reg_388[3]),
        .O(\shl_ln108_reg_393[16]_i_2_n_1 ));
  LUT4 #(
    .INIT(16'hFFFE)) 
    \shl_ln108_reg_393[16]_i_3 
       (.I0(srem_ln108_reg_383[11]),
        .I1(srem_ln108_reg_383[31]),
        .I2(srem_ln108_reg_383[20]),
        .I3(srem_ln108_reg_383[23]),
        .O(\shl_ln108_reg_393[16]_i_3_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair31" *) 
  LUT5 #(
    .INIT(32'hFFFFFFFE)) 
    \shl_ln108_reg_393[16]_i_4 
       (.I0(srem_ln108_reg_383[19]),
        .I1(srem_ln108_reg_383[26]),
        .I2(srem_ln108_reg_383[28]),
        .I3(\shl_ln108_reg_393[24]_i_7_n_1 ),
        .I4(\shl_ln108_reg_393[24]_i_8_n_1 ),
        .O(\shl_ln108_reg_393[16]_i_4_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair84" *) 
  LUT2 #(
    .INIT(4'h2)) 
    \shl_ln108_reg_393[23]_i_1 
       (.I0(trunc_ln108_reg_388[4]),
        .I1(trunc_ln108_reg_388[3]),
        .O(\shl_ln108_reg_393[23]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h000000000000202A)) 
    \shl_ln108_reg_393[24]_i_1 
       (.I0(trunc_ln108_reg_388[4]),
        .I1(\shl_ln108_reg_393[24]_i_2_n_1 ),
        .I2(trunc_ln108_reg_388[3]),
        .I3(\shl_ln108_reg_393[24]_i_3_n_1 ),
        .I4(\shl_ln108_reg_393[24]_i_4_n_1 ),
        .I5(\shl_ln108_reg_393[24]_i_5_n_1 ),
        .O(shl_ln108_fu_233_p2[24]));
  (* SOFT_HLUTNM = "soft_lutpair44" *) 
  LUT3 #(
    .INIT(8'hFE)) 
    \shl_ln108_reg_393[24]_i_2 
       (.I0(trunc_ln108_reg_388[2]),
        .I1(trunc_ln108_reg_388[1]),
        .I2(trunc_ln108_reg_388[0]),
        .O(\shl_ln108_reg_393[24]_i_2_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair44" *) 
  LUT3 #(
    .INIT(8'h7F)) 
    \shl_ln108_reg_393[24]_i_3 
       (.I0(trunc_ln108_reg_388[1]),
        .I1(trunc_ln108_reg_388[0]),
        .I2(trunc_ln108_reg_388[2]),
        .O(\shl_ln108_reg_393[24]_i_3_n_1 ));
  LUT6 #(
    .INIT(64'hFFFFFFFFFFFFFFFE)) 
    \shl_ln108_reg_393[24]_i_4 
       (.I0(\shl_ln108_reg_393[16]_i_3_n_1 ),
        .I1(\shl_ln108_reg_393[24]_i_6_n_1 ),
        .I2(\shl_ln108_reg_393[24]_i_7_n_1 ),
        .I3(\shl_ln108_reg_393[24]_i_8_n_1 ),
        .I4(srem_ln108_reg_383[22]),
        .I5(srem_ln108_reg_383[17]),
        .O(\shl_ln108_reg_393[24]_i_4_n_1 ));
  LUT5 #(
    .INIT(32'hFFFFFFFE)) 
    \shl_ln108_reg_393[24]_i_5 
       (.I0(\shl_ln108_reg_393[24]_i_9_n_1 ),
        .I1(srem_ln108_reg_383[21]),
        .I2(srem_ln108_reg_383[10]),
        .I3(srem_ln108_reg_383[9]),
        .I4(srem_ln108_reg_383[8]),
        .O(\shl_ln108_reg_393[24]_i_5_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair31" *) 
  LUT3 #(
    .INIT(8'hFE)) 
    \shl_ln108_reg_393[24]_i_6 
       (.I0(srem_ln108_reg_383[19]),
        .I1(srem_ln108_reg_383[26]),
        .I2(srem_ln108_reg_383[28]),
        .O(\shl_ln108_reg_393[24]_i_6_n_1 ));
  LUT4 #(
    .INIT(16'hFFFE)) 
    \shl_ln108_reg_393[24]_i_7 
       (.I0(srem_ln108_reg_383[16]),
        .I1(srem_ln108_reg_383[29]),
        .I2(srem_ln108_reg_383[24]),
        .I3(srem_ln108_reg_383[12]),
        .O(\shl_ln108_reg_393[24]_i_7_n_1 ));
  LUT4 #(
    .INIT(16'hFFFE)) 
    \shl_ln108_reg_393[24]_i_8 
       (.I0(srem_ln108_reg_383[25]),
        .I1(srem_ln108_reg_383[15]),
        .I2(trunc_ln108_reg_388[5]),
        .I3(srem_ln108_reg_383[14]),
        .O(\shl_ln108_reg_393[24]_i_8_n_1 ));
  LUT6 #(
    .INIT(64'hFFFFFFFFFFFFFFFE)) 
    \shl_ln108_reg_393[24]_i_9 
       (.I0(srem_ln108_reg_383[18]),
        .I1(srem_ln108_reg_383[7]),
        .I2(srem_ln108_reg_383[27]),
        .I3(srem_ln108_reg_383[6]),
        .I4(srem_ln108_reg_383[13]),
        .I5(srem_ln108_reg_383[30]),
        .O(\shl_ln108_reg_393[24]_i_9_n_1 ));
  LUT5 #(
    .INIT(32'hFFFE0000)) 
    \shl_ln108_reg_393[25]_i_1 
       (.I0(trunc_ln108_reg_388[2]),
        .I1(trunc_ln108_reg_388[1]),
        .I2(\shl_ln108_reg_393[24]_i_4_n_1 ),
        .I3(\shl_ln108_reg_393[24]_i_5_n_1 ),
        .I4(ap_CS_fsm_state81),
        .O(\shl_ln108_reg_393[25]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'hFFFFFFF900000000)) 
    \shl_ln108_reg_393[26]_i_1 
       (.I0(trunc_ln108_reg_388[1]),
        .I1(trunc_ln108_reg_388[0]),
        .I2(\shl_ln108_reg_393[24]_i_4_n_1 ),
        .I3(\shl_ln108_reg_393[24]_i_5_n_1 ),
        .I4(trunc_ln108_reg_388[2]),
        .I5(ap_CS_fsm_state81),
        .O(\shl_ln108_reg_393[26]_i_1_n_1 ));
  LUT5 #(
    .INIT(32'hFEFF0000)) 
    \shl_ln108_reg_393[27]_i_1 
       (.I0(\shl_ln108_reg_393[24]_i_5_n_1 ),
        .I1(\shl_ln108_reg_393[24]_i_4_n_1 ),
        .I2(trunc_ln108_reg_388[2]),
        .I3(trunc_ln108_reg_388[1]),
        .I4(ap_CS_fsm_state81),
        .O(\shl_ln108_reg_393[27]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'hFFFFFFE700000000)) 
    \shl_ln108_reg_393[28]_i_1 
       (.I0(trunc_ln108_reg_388[1]),
        .I1(trunc_ln108_reg_388[0]),
        .I2(trunc_ln108_reg_388[2]),
        .I3(\shl_ln108_reg_393[24]_i_4_n_1 ),
        .I4(\shl_ln108_reg_393[24]_i_5_n_1 ),
        .I5(ap_CS_fsm_state81),
        .O(\shl_ln108_reg_393[28]_i_1_n_1 ));
  LUT5 #(
    .INIT(32'hFFFD0000)) 
    \shl_ln108_reg_393[29]_i_1 
       (.I0(trunc_ln108_reg_388[2]),
        .I1(trunc_ln108_reg_388[1]),
        .I2(\shl_ln108_reg_393[24]_i_4_n_1 ),
        .I3(\shl_ln108_reg_393[24]_i_5_n_1 ),
        .I4(ap_CS_fsm_state81),
        .O(\shl_ln108_reg_393[29]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'hFFFFFFD700000000)) 
    \shl_ln108_reg_393[30]_i_1 
       (.I0(trunc_ln108_reg_388[2]),
        .I1(trunc_ln108_reg_388[1]),
        .I2(trunc_ln108_reg_388[0]),
        .I3(\shl_ln108_reg_393[24]_i_4_n_1 ),
        .I4(\shl_ln108_reg_393[24]_i_5_n_1 ),
        .I5(ap_CS_fsm_state81),
        .O(\shl_ln108_reg_393[30]_i_1_n_1 ));
  LUT5 #(
    .INIT(32'hEFFF0000)) 
    \shl_ln108_reg_393[31]_i_1 
       (.I0(\shl_ln108_reg_393[24]_i_5_n_1 ),
        .I1(\shl_ln108_reg_393[24]_i_4_n_1 ),
        .I2(trunc_ln108_reg_388[2]),
        .I3(trunc_ln108_reg_388[1]),
        .I4(ap_CS_fsm_state81),
        .O(\shl_ln108_reg_393[31]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair85" *) 
  LUT2 #(
    .INIT(4'h8)) 
    \shl_ln108_reg_393[31]_i_2 
       (.I0(trunc_ln108_reg_388[3]),
        .I1(trunc_ln108_reg_388[4]),
        .O(\shl_ln108_reg_393[31]_i_2_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair85" *) 
  LUT2 #(
    .INIT(4'h1)) 
    \shl_ln108_reg_393[7]_i_1 
       (.I0(trunc_ln108_reg_388[4]),
        .I1(trunc_ln108_reg_388[3]),
        .O(\shl_ln108_reg_393[7]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000001015)) 
    \shl_ln108_reg_393[8]_i_1 
       (.I0(trunc_ln108_reg_388[4]),
        .I1(\shl_ln108_reg_393[24]_i_2_n_1 ),
        .I2(trunc_ln108_reg_388[3]),
        .I3(\shl_ln108_reg_393[24]_i_3_n_1 ),
        .I4(\shl_ln108_reg_393[24]_i_4_n_1 ),
        .I5(\shl_ln108_reg_393[24]_i_5_n_1 ),
        .O(shl_ln108_fu_233_p2[8]));
  FDRE \shl_ln108_reg_393_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(shl_ln108_fu_233_p2[0]),
        .Q(\shl_ln108_reg_393_reg_n_1_[0] ),
        .R(1'b0));
  FDRE \shl_ln108_reg_393_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[15]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[10] ),
        .R(\shl_ln108_reg_393[26]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[11] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[15]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[11] ),
        .R(\shl_ln108_reg_393[27]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[12] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[15]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[12] ),
        .R(\shl_ln108_reg_393[28]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[13] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[15]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[13] ),
        .R(\shl_ln108_reg_393[29]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[14] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[15]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[14] ),
        .R(\shl_ln108_reg_393[30]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[15] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[15]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[15] ),
        .R(\shl_ln108_reg_393[31]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[16] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(shl_ln108_fu_233_p2[16]),
        .Q(\shl_ln108_reg_393_reg_n_1_[16] ),
        .R(1'b0));
  FDRE \shl_ln108_reg_393_reg[17] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[23]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[17] ),
        .R(\shl_ln108_reg_393[25]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[18] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[23]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[18] ),
        .R(\shl_ln108_reg_393[26]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[19] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[23]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[19] ),
        .R(\shl_ln108_reg_393[27]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[7]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[1] ),
        .R(\shl_ln108_reg_393[25]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[20] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[23]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[20] ),
        .R(\shl_ln108_reg_393[28]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[21] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[23]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[21] ),
        .R(\shl_ln108_reg_393[29]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[22] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[23]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[22] ),
        .R(\shl_ln108_reg_393[30]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[23] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[23]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[23] ),
        .R(\shl_ln108_reg_393[31]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[24] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(shl_ln108_fu_233_p2[24]),
        .Q(\shl_ln108_reg_393_reg_n_1_[24] ),
        .R(1'b0));
  FDRE \shl_ln108_reg_393_reg[25] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[31]_i_2_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[25] ),
        .R(\shl_ln108_reg_393[25]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[26] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[31]_i_2_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[26] ),
        .R(\shl_ln108_reg_393[26]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[27] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[31]_i_2_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[27] ),
        .R(\shl_ln108_reg_393[27]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[28] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[31]_i_2_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[28] ),
        .R(\shl_ln108_reg_393[28]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[29] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[31]_i_2_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[29] ),
        .R(\shl_ln108_reg_393[29]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[7]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[2] ),
        .R(\shl_ln108_reg_393[26]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[30] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[31]_i_2_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[30] ),
        .R(\shl_ln108_reg_393[30]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[31] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[31]_i_2_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[31] ),
        .R(\shl_ln108_reg_393[31]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[7]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[3] ),
        .R(\shl_ln108_reg_393[27]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[7]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[4] ),
        .R(\shl_ln108_reg_393[28]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[7]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[5] ),
        .R(\shl_ln108_reg_393[29]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[7]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[6] ),
        .R(\shl_ln108_reg_393[30]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[7]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[7] ),
        .R(\shl_ln108_reg_393[31]_i_1_n_1 ));
  FDRE \shl_ln108_reg_393_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(shl_ln108_fu_233_p2[8]),
        .Q(\shl_ln108_reg_393_reg_n_1_[8] ),
        .R(1'b0));
  FDRE \shl_ln108_reg_393_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state81),
        .D(\shl_ln108_reg_393[15]_i_1_n_1 ),
        .Q(\shl_ln108_reg_393_reg_n_1_[9] ),
        .R(\shl_ln108_reg_393[25]_i_1_n_1 ));
  FDRE \srem_ln105_reg_347_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[0]),
        .Q(srem_ln105_reg_347[0]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[10]),
        .Q(srem_ln105_reg_347[10]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[11] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[11]),
        .Q(srem_ln105_reg_347[11]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[12] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[12]),
        .Q(srem_ln105_reg_347[12]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[13] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[13]),
        .Q(srem_ln105_reg_347[13]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[14] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[14]),
        .Q(srem_ln105_reg_347[14]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[15] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[15]),
        .Q(srem_ln105_reg_347[15]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[16] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[16]),
        .Q(srem_ln105_reg_347[16]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[17] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[17]),
        .Q(srem_ln105_reg_347[17]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[18] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[18]),
        .Q(srem_ln105_reg_347[18]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[19] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[19]),
        .Q(srem_ln105_reg_347[19]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[1]),
        .Q(srem_ln105_reg_347[1]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[20] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[20]),
        .Q(srem_ln105_reg_347[20]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[21] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[21]),
        .Q(srem_ln105_reg_347[21]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[22] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[22]),
        .Q(srem_ln105_reg_347[22]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[23] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[23]),
        .Q(srem_ln105_reg_347[23]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[24] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[24]),
        .Q(srem_ln105_reg_347[24]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[25] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[25]),
        .Q(srem_ln105_reg_347[25]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[26] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[26]),
        .Q(srem_ln105_reg_347[26]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[27] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[27]),
        .Q(srem_ln105_reg_347[27]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[28] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[28]),
        .Q(srem_ln105_reg_347[28]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[29] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[29]),
        .Q(srem_ln105_reg_347[29]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[2]),
        .Q(srem_ln105_reg_347[2]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[30] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[30]),
        .Q(srem_ln105_reg_347[30]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[31] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[31]),
        .Q(srem_ln105_reg_347[31]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[3]),
        .Q(srem_ln105_reg_347[3]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[4]),
        .Q(srem_ln105_reg_347[4]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[5]),
        .Q(srem_ln105_reg_347[5]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[6]),
        .Q(srem_ln105_reg_347[6]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[7]),
        .Q(srem_ln105_reg_347[7]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[8]),
        .Q(srem_ln105_reg_347[8]),
        .R(1'b0));
  FDRE \srem_ln105_reg_347_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state42),
        .D(remd[9]),
        .Q(srem_ln105_reg_347[9]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_22),
        .Q(srem_ln108_reg_383[10]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[11] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_21),
        .Q(srem_ln108_reg_383[11]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[12] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_20),
        .Q(srem_ln108_reg_383[12]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[13] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_19),
        .Q(srem_ln108_reg_383[13]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[14] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_18),
        .Q(srem_ln108_reg_383[14]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[15] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_17),
        .Q(srem_ln108_reg_383[15]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[16] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_16),
        .Q(srem_ln108_reg_383[16]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[17] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_15),
        .Q(srem_ln108_reg_383[17]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[18] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_14),
        .Q(srem_ln108_reg_383[18]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[19] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_13),
        .Q(srem_ln108_reg_383[19]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[20] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_12),
        .Q(srem_ln108_reg_383[20]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[21] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_11),
        .Q(srem_ln108_reg_383[21]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[22] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_10),
        .Q(srem_ln108_reg_383[22]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[23] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_9),
        .Q(srem_ln108_reg_383[23]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[24] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_8),
        .Q(srem_ln108_reg_383[24]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[25] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_7),
        .Q(srem_ln108_reg_383[25]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[26] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_6),
        .Q(srem_ln108_reg_383[26]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[27] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_5),
        .Q(srem_ln108_reg_383[27]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[28] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_4),
        .Q(srem_ln108_reg_383[28]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[29] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_3),
        .Q(srem_ln108_reg_383[29]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[30] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_2),
        .Q(srem_ln108_reg_383[30]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[31] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_1),
        .Q(srem_ln108_reg_383[31]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_26),
        .Q(srem_ln108_reg_383[6]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_25),
        .Q(srem_ln108_reg_383[7]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_24),
        .Q(srem_ln108_reg_383[8]),
        .R(1'b0));
  FDRE \srem_ln108_reg_383_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_23),
        .Q(srem_ln108_reg_383[9]),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair42" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \sub_ln105_reg_362[1]_i_1 
       (.I0(srem_ln105_reg_347[0]),
        .I1(srem_ln105_reg_347[1]),
        .O(sub_ln105_fu_178_p2[1]));
  (* SOFT_HLUTNM = "soft_lutpair42" *) 
  LUT3 #(
    .INIT(8'h1E)) 
    \sub_ln105_reg_362[2]_i_1 
       (.I0(srem_ln105_reg_347[1]),
        .I1(srem_ln105_reg_347[0]),
        .I2(srem_ln105_reg_347[2]),
        .O(sub_ln105_fu_178_p2[2]));
  (* SOFT_HLUTNM = "soft_lutpair35" *) 
  LUT4 #(
    .INIT(16'h01FE)) 
    \sub_ln105_reg_362[3]_i_1 
       (.I0(srem_ln105_reg_347[2]),
        .I1(srem_ln105_reg_347[0]),
        .I2(srem_ln105_reg_347[1]),
        .I3(srem_ln105_reg_347[3]),
        .O(sub_ln105_fu_178_p2[3]));
  (* SOFT_HLUTNM = "soft_lutpair35" *) 
  LUT5 #(
    .INIT(32'hFFFE0001)) 
    \sub_ln105_reg_362[4]_i_1 
       (.I0(srem_ln105_reg_347[3]),
        .I1(srem_ln105_reg_347[1]),
        .I2(srem_ln105_reg_347[0]),
        .I3(srem_ln105_reg_347[2]),
        .I4(srem_ln105_reg_347[4]),
        .O(sub_ln105_fu_178_p2[4]));
  LUT6 #(
    .INIT(64'h55555557AAAAAAA8)) 
    \sub_ln105_reg_362[5]_i_1 
       (.I0(srem_ln105_reg_347[4]),
        .I1(srem_ln105_reg_347[2]),
        .I2(srem_ln105_reg_347[0]),
        .I3(srem_ln105_reg_347[1]),
        .I4(srem_ln105_reg_347[3]),
        .I5(srem_ln105_reg_347[5]),
        .O(sub_ln105_fu_178_p2[5]));
  FDRE \sub_ln105_reg_362_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(srem_ln105_reg_347[0]),
        .Q(sub_ln105_reg_362[0]),
        .R(1'b0));
  FDRE \sub_ln105_reg_362_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(sub_ln105_fu_178_p2[1]),
        .Q(sub_ln105_reg_362[1]),
        .R(1'b0));
  FDRE \sub_ln105_reg_362_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(sub_ln105_fu_178_p2[2]),
        .Q(sub_ln105_reg_362[2]),
        .R(1'b0));
  FDRE \sub_ln105_reg_362_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(sub_ln105_fu_178_p2[3]),
        .Q(sub_ln105_reg_362[3]),
        .R(1'b0));
  FDRE \sub_ln105_reg_362_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(sub_ln105_fu_178_p2[4]),
        .Q(sub_ln105_reg_362[4]),
        .R(1'b0));
  FDRE \sub_ln105_reg_362_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(sub_ln105_fu_178_p2[5]),
        .Q(sub_ln105_reg_362[5]),
        .R(1'b0));
  LUT6 #(
    .INIT(64'h0000000000800000)) 
    \tmp_reg_367[0]_i_1 
       (.I0(\shl_ln105_reg_357[5]_i_3_n_1 ),
        .I1(\shl_ln105_reg_357[5]_i_4_n_1 ),
        .I2(\shl_ln105_reg_357[5]_i_5_n_1 ),
        .I3(\shl_ln105_reg_357[5]_i_6_n_1 ),
        .I4(srem_ln105_reg_347[0]),
        .I5(srem_ln105_reg_347[1]),
        .O(\tmp_reg_367[0]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h0080000000000000)) 
    \tmp_reg_367[10]_i_1 
       (.I0(\tmp_reg_367[10]_i_2_n_1 ),
        .I1(\shl_ln105_reg_357[5]_i_4_n_1 ),
        .I2(\shl_ln105_reg_357[5]_i_5_n_1 ),
        .I3(\shl_ln105_reg_357[5]_i_6_n_1 ),
        .I4(srem_ln105_reg_347[0]),
        .I5(srem_ln105_reg_347[1]),
        .O(p_0_in));
  (* SOFT_HLUTNM = "soft_lutpair40" *) 
  LUT3 #(
    .INIT(8'h08)) 
    \tmp_reg_367[10]_i_2 
       (.I0(srem_ln105_reg_347[2]),
        .I1(srem_ln105_reg_347[3]),
        .I2(srem_ln105_reg_347[4]),
        .O(\tmp_reg_367[10]_i_2_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000800000)) 
    \tmp_reg_367[1]_i_1 
       (.I0(\shl_ln105_reg_357[5]_i_3_n_1 ),
        .I1(\shl_ln105_reg_357[5]_i_4_n_1 ),
        .I2(\shl_ln105_reg_357[5]_i_5_n_1 ),
        .I3(\shl_ln105_reg_357[5]_i_6_n_1 ),
        .I4(srem_ln105_reg_347[1]),
        .I5(srem_ln105_reg_347[0]),
        .O(\tmp_reg_367[1]_i_1_n_1 ));
  LUT2 #(
    .INIT(4'h2)) 
    \tmp_reg_367[25]_i_1 
       (.I0(ap_CS_fsm_state43),
        .I1(srem_ln105_reg_347[4]),
        .O(\tmp_reg_367[25]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h0080000000000000)) 
    \tmp_reg_367[2]_i_1 
       (.I0(\shl_ln105_reg_357[5]_i_3_n_1 ),
        .I1(\shl_ln105_reg_357[5]_i_4_n_1 ),
        .I2(\shl_ln105_reg_357[5]_i_5_n_1 ),
        .I3(\shl_ln105_reg_357[5]_i_6_n_1 ),
        .I4(srem_ln105_reg_347[0]),
        .I5(srem_ln105_reg_347[1]),
        .O(\tmp_reg_367[2]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000080)) 
    \tmp_reg_367[3]_i_1 
       (.I0(\tmp_reg_367[6]_i_2_n_1 ),
        .I1(\shl_ln105_reg_357[5]_i_4_n_1 ),
        .I2(\shl_ln105_reg_357[5]_i_5_n_1 ),
        .I3(\shl_ln105_reg_357[5]_i_6_n_1 ),
        .I4(srem_ln105_reg_347[0]),
        .I5(srem_ln105_reg_347[1]),
        .O(\tmp_reg_367[3]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000800000)) 
    \tmp_reg_367[4]_i_1 
       (.I0(\tmp_reg_367[6]_i_2_n_1 ),
        .I1(\shl_ln105_reg_357[5]_i_4_n_1 ),
        .I2(\shl_ln105_reg_357[5]_i_5_n_1 ),
        .I3(\shl_ln105_reg_357[5]_i_6_n_1 ),
        .I4(srem_ln105_reg_347[0]),
        .I5(srem_ln105_reg_347[1]),
        .O(\tmp_reg_367[4]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000800000)) 
    \tmp_reg_367[5]_i_1 
       (.I0(\tmp_reg_367[6]_i_2_n_1 ),
        .I1(\shl_ln105_reg_357[5]_i_4_n_1 ),
        .I2(\shl_ln105_reg_357[5]_i_5_n_1 ),
        .I3(\shl_ln105_reg_357[5]_i_6_n_1 ),
        .I4(srem_ln105_reg_347[1]),
        .I5(srem_ln105_reg_347[0]),
        .O(\tmp_reg_367[5]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h0080000000000000)) 
    \tmp_reg_367[6]_i_1 
       (.I0(\tmp_reg_367[6]_i_2_n_1 ),
        .I1(\shl_ln105_reg_357[5]_i_4_n_1 ),
        .I2(\shl_ln105_reg_357[5]_i_5_n_1 ),
        .I3(\shl_ln105_reg_357[5]_i_6_n_1 ),
        .I4(srem_ln105_reg_347[0]),
        .I5(srem_ln105_reg_347[1]),
        .O(\tmp_reg_367[6]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair50" *) 
  LUT2 #(
    .INIT(4'h2)) 
    \tmp_reg_367[6]_i_2 
       (.I0(srem_ln105_reg_347[3]),
        .I1(srem_ln105_reg_347[2]),
        .O(\tmp_reg_367[6]_i_2_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000000080)) 
    \tmp_reg_367[7]_i_1 
       (.I0(\tmp_reg_367[9]_i_2_n_1 ),
        .I1(\shl_ln105_reg_357[5]_i_4_n_1 ),
        .I2(\shl_ln105_reg_357[5]_i_5_n_1 ),
        .I3(\shl_ln105_reg_357[5]_i_6_n_1 ),
        .I4(srem_ln105_reg_347[0]),
        .I5(srem_ln105_reg_347[1]),
        .O(\tmp_reg_367[7]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000800000)) 
    \tmp_reg_367[8]_i_1 
       (.I0(\tmp_reg_367[9]_i_2_n_1 ),
        .I1(\shl_ln105_reg_357[5]_i_4_n_1 ),
        .I2(\shl_ln105_reg_357[5]_i_5_n_1 ),
        .I3(\shl_ln105_reg_357[5]_i_6_n_1 ),
        .I4(srem_ln105_reg_347[0]),
        .I5(srem_ln105_reg_347[1]),
        .O(\tmp_reg_367[8]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h0000000000800000)) 
    \tmp_reg_367[9]_i_1 
       (.I0(\tmp_reg_367[9]_i_2_n_1 ),
        .I1(\shl_ln105_reg_357[5]_i_4_n_1 ),
        .I2(\shl_ln105_reg_357[5]_i_5_n_1 ),
        .I3(\shl_ln105_reg_357[5]_i_6_n_1 ),
        .I4(srem_ln105_reg_347[1]),
        .I5(srem_ln105_reg_347[0]),
        .O(\tmp_reg_367[9]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair40" *) 
  LUT2 #(
    .INIT(4'h8)) 
    \tmp_reg_367[9]_i_2 
       (.I0(srem_ln105_reg_347[3]),
        .I1(srem_ln105_reg_347[2]),
        .O(\tmp_reg_367[9]_i_2_n_1 ));
  FDRE \tmp_reg_367_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[0]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[6]),
        .R(\shl_ln105_reg_357[5]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(p_0_in),
        .Q(or_ln_fu_207_p3[16]),
        .R(1'b0));
  FDRE \tmp_reg_367_reg[11] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\shl_ln105_reg_357[1]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[17]),
        .R(\tmp_reg_367[25]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[12] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\shl_ln105_reg_357[2]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[18]),
        .R(\tmp_reg_367[25]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[13] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\shl_ln105_reg_357[3]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[19]),
        .R(\tmp_reg_367[25]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[14] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\shl_ln105_reg_357[4]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[20]),
        .R(\tmp_reg_367[25]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[15] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\shl_ln105_reg_357[5]_i_2_n_1 ),
        .Q(or_ln_fu_207_p3[21]),
        .R(\tmp_reg_367[25]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[16] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[0]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[22]),
        .R(\tmp_reg_367[25]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[17] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[1]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[23]),
        .R(\tmp_reg_367[25]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[18] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[2]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[24]),
        .R(\tmp_reg_367[25]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[19] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[3]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[25]),
        .R(\tmp_reg_367[25]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[1]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[7]),
        .R(\shl_ln105_reg_357[5]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[20] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[4]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[26]),
        .R(\tmp_reg_367[25]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[21] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[5]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[27]),
        .R(\tmp_reg_367[25]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[22] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[6]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[28]),
        .R(\tmp_reg_367[25]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[23] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[7]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[29]),
        .R(\tmp_reg_367[25]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[24] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[8]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[30]),
        .R(\tmp_reg_367[25]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[25] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[9]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[31]),
        .R(\tmp_reg_367[25]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[2]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[8]),
        .R(\shl_ln105_reg_357[5]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[3]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[9]),
        .R(\shl_ln105_reg_357[5]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[4]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[10]),
        .R(\shl_ln105_reg_357[5]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[5]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[11]),
        .R(\shl_ln105_reg_357[5]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[6]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[12]),
        .R(\shl_ln105_reg_357[5]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[7]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[13]),
        .R(\shl_ln105_reg_357[5]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[8]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[14]),
        .R(\shl_ln105_reg_357[5]_i_1_n_1 ));
  FDRE \tmp_reg_367_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state43),
        .D(\tmp_reg_367[9]_i_1_n_1 ),
        .Q(or_ln_fu_207_p3[15]),
        .R(\shl_ln105_reg_357[5]_i_1_n_1 ));
  FDRE \trunc_ln108_reg_388_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_32),
        .Q(trunc_ln108_reg_388[0]),
        .R(1'b0));
  FDRE \trunc_ln108_reg_388_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_31),
        .Q(trunc_ln108_reg_388[1]),
        .R(1'b0));
  FDRE \trunc_ln108_reg_388_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_30),
        .Q(trunc_ln108_reg_388[2]),
        .R(1'b0));
  FDRE \trunc_ln108_reg_388_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_29),
        .Q(trunc_ln108_reg_388[3]),
        .R(1'b0));
  FDRE \trunc_ln108_reg_388_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_28),
        .Q(trunc_ln108_reg_388[4]),
        .R(1'b0));
  FDRE \trunc_ln108_reg_388_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state80),
        .D(loop_imperfect_srbkb_U2_n_27),
        .Q(trunc_ln108_reg_388[5]),
        .R(1'b0));
endmodule

(* ORIG_REF_NAME = "loop_imperfect_srbkb" *) 
module bd_0_hls_inst_0_loop_imperfect_srbkb
   (r_stage_reg_r_29,
    D,
    \remd_reg[31] ,
    Q,
    ap_clk,
    ap_rst,
    \dividend0_reg[31] ,
    \dividend0_reg[31]_0 );
  output r_stage_reg_r_29;
  output [31:0]D;
  output [31:0]\remd_reg[31] ;
  input [0:0]Q;
  input ap_clk;
  input ap_rst;
  input [31:0]\dividend0_reg[31] ;
  input [31:0]\dividend0_reg[31]_0 ;

  wire [31:0]D;
  wire [0:0]Q;
  wire ap_clk;
  wire ap_rst;
  wire [31:0]\dividend0_reg[31] ;
  wire [31:0]\dividend0_reg[31]_0 ;
  wire r_stage_reg_r_29;
  wire [31:0]\remd_reg[31] ;

  bd_0_hls_inst_0_loop_imperfect_srbkb_div_1 loop_imperfect_srbkb_div_U
       (.D(D),
        .Q(Q),
        .ap_clk(ap_clk),
        .ap_rst(ap_rst),
        .\dividend0_reg[31]_0 (\dividend0_reg[31] ),
        .\dividend0_reg[31]_1 (\dividend0_reg[31]_0 ),
        .r_stage_reg_r_29(r_stage_reg_r_29),
        .\remd_reg[31]_0 (\remd_reg[31] ));
endmodule

(* ORIG_REF_NAME = "loop_imperfect_srbkb" *) 
module bd_0_hls_inst_0_loop_imperfect_srbkb_0
   (\remd_reg[31] ,
    Q,
    ap_clk,
    ap_rst,
    \r_stage_reg[32] ,
    \dividend0_reg[31] );
  output [31:0]\remd_reg[31] ;
  input [0:0]Q;
  input ap_clk;
  input ap_rst;
  input \r_stage_reg[32] ;
  input [31:0]\dividend0_reg[31] ;

  wire [0:0]Q;
  wire ap_clk;
  wire ap_rst;
  wire [31:0]\dividend0_reg[31] ;
  wire \r_stage_reg[32] ;
  wire [31:0]\remd_reg[31] ;

  bd_0_hls_inst_0_loop_imperfect_srbkb_div loop_imperfect_srbkb_div_U
       (.Q(Q),
        .ap_clk(ap_clk),
        .ap_rst(ap_rst),
        .\dividend0_reg[31]_0 (\dividend0_reg[31] ),
        .\r_stage_reg[32] (\r_stage_reg[32] ),
        .\remd_reg[31]_0 (\remd_reg[31] ));
endmodule

(* ORIG_REF_NAME = "loop_imperfect_srbkb_div" *) 
module bd_0_hls_inst_0_loop_imperfect_srbkb_div
   (\remd_reg[31]_0 ,
    Q,
    ap_clk,
    ap_rst,
    \r_stage_reg[32] ,
    \dividend0_reg[31]_0 );
  output [31:0]\remd_reg[31]_0 ;
  input [0:0]Q;
  input ap_clk;
  input ap_rst;
  input \r_stage_reg[32] ;
  input [31:0]\dividend0_reg[31]_0 ;

  wire [0:0]Q;
  wire ap_clk;
  wire ap_rst;
  wire \dividend0[10]_i_1_n_1 ;
  wire \dividend0[11]_i_1_n_1 ;
  wire \dividend0[12]_i_1_n_1 ;
  wire \dividend0[12]_i_3_n_1 ;
  wire \dividend0[12]_i_4_n_1 ;
  wire \dividend0[12]_i_5_n_1 ;
  wire \dividend0[12]_i_6_n_1 ;
  wire \dividend0[13]_i_1_n_1 ;
  wire \dividend0[14]_i_1_n_1 ;
  wire \dividend0[15]_i_1_n_1 ;
  wire \dividend0[16]_i_1_n_1 ;
  wire \dividend0[16]_i_3_n_1 ;
  wire \dividend0[16]_i_4_n_1 ;
  wire \dividend0[16]_i_5_n_1 ;
  wire \dividend0[16]_i_6_n_1 ;
  wire \dividend0[17]_i_1_n_1 ;
  wire \dividend0[18]_i_1_n_1 ;
  wire \dividend0[19]_i_1_n_1 ;
  wire \dividend0[1]_i_1_n_1 ;
  wire \dividend0[20]_i_1_n_1 ;
  wire \dividend0[20]_i_3_n_1 ;
  wire \dividend0[20]_i_4_n_1 ;
  wire \dividend0[20]_i_5_n_1 ;
  wire \dividend0[20]_i_6_n_1 ;
  wire \dividend0[21]_i_1_n_1 ;
  wire \dividend0[22]_i_1_n_1 ;
  wire \dividend0[23]_i_1_n_1 ;
  wire \dividend0[24]_i_1_n_1 ;
  wire \dividend0[24]_i_3_n_1 ;
  wire \dividend0[24]_i_4_n_1 ;
  wire \dividend0[24]_i_5_n_1 ;
  wire \dividend0[24]_i_6_n_1 ;
  wire \dividend0[25]_i_1_n_1 ;
  wire \dividend0[26]_i_1_n_1 ;
  wire \dividend0[27]_i_1_n_1 ;
  wire \dividend0[28]_i_1_n_1 ;
  wire \dividend0[28]_i_3_n_1 ;
  wire \dividend0[28]_i_4_n_1 ;
  wire \dividend0[28]_i_5_n_1 ;
  wire \dividend0[28]_i_6_n_1 ;
  wire \dividend0[29]_i_1_n_1 ;
  wire \dividend0[2]_i_1_n_1 ;
  wire \dividend0[30]_i_1_n_1 ;
  wire \dividend0[31]_i_1_n_1 ;
  wire \dividend0[31]_i_3_n_1 ;
  wire \dividend0[31]_i_4_n_1 ;
  wire \dividend0[31]_i_5_n_1 ;
  wire \dividend0[3]_i_1_n_1 ;
  wire \dividend0[4]_i_1_n_1 ;
  wire \dividend0[4]_i_3_n_1 ;
  wire \dividend0[4]_i_4_n_1 ;
  wire \dividend0[4]_i_5_n_1 ;
  wire \dividend0[4]_i_6_n_1 ;
  wire \dividend0[4]_i_7_n_1 ;
  wire \dividend0[5]_i_1_n_1 ;
  wire \dividend0[6]_i_1_n_1 ;
  wire \dividend0[7]_i_1_n_1 ;
  wire \dividend0[8]_i_1_n_1 ;
  wire \dividend0[8]_i_3_n_1 ;
  wire \dividend0[8]_i_4_n_1 ;
  wire \dividend0[8]_i_5_n_1 ;
  wire \dividend0[8]_i_6_n_1 ;
  wire \dividend0[9]_i_1_n_1 ;
  wire \dividend0_reg[12]_i_2__0_n_1 ;
  wire \dividend0_reg[12]_i_2__0_n_2 ;
  wire \dividend0_reg[12]_i_2__0_n_3 ;
  wire \dividend0_reg[12]_i_2__0_n_4 ;
  wire \dividend0_reg[12]_i_2__0_n_5 ;
  wire \dividend0_reg[12]_i_2__0_n_6 ;
  wire \dividend0_reg[12]_i_2__0_n_7 ;
  wire \dividend0_reg[12]_i_2__0_n_8 ;
  wire \dividend0_reg[16]_i_2__0_n_1 ;
  wire \dividend0_reg[16]_i_2__0_n_2 ;
  wire \dividend0_reg[16]_i_2__0_n_3 ;
  wire \dividend0_reg[16]_i_2__0_n_4 ;
  wire \dividend0_reg[16]_i_2__0_n_5 ;
  wire \dividend0_reg[16]_i_2__0_n_6 ;
  wire \dividend0_reg[16]_i_2__0_n_7 ;
  wire \dividend0_reg[16]_i_2__0_n_8 ;
  wire \dividend0_reg[20]_i_2__0_n_1 ;
  wire \dividend0_reg[20]_i_2__0_n_2 ;
  wire \dividend0_reg[20]_i_2__0_n_3 ;
  wire \dividend0_reg[20]_i_2__0_n_4 ;
  wire \dividend0_reg[20]_i_2__0_n_5 ;
  wire \dividend0_reg[20]_i_2__0_n_6 ;
  wire \dividend0_reg[20]_i_2__0_n_7 ;
  wire \dividend0_reg[20]_i_2__0_n_8 ;
  wire \dividend0_reg[24]_i_2__0_n_1 ;
  wire \dividend0_reg[24]_i_2__0_n_2 ;
  wire \dividend0_reg[24]_i_2__0_n_3 ;
  wire \dividend0_reg[24]_i_2__0_n_4 ;
  wire \dividend0_reg[24]_i_2__0_n_5 ;
  wire \dividend0_reg[24]_i_2__0_n_6 ;
  wire \dividend0_reg[24]_i_2__0_n_7 ;
  wire \dividend0_reg[24]_i_2__0_n_8 ;
  wire \dividend0_reg[28]_i_2__0_n_1 ;
  wire \dividend0_reg[28]_i_2__0_n_2 ;
  wire \dividend0_reg[28]_i_2__0_n_3 ;
  wire \dividend0_reg[28]_i_2__0_n_4 ;
  wire \dividend0_reg[28]_i_2__0_n_5 ;
  wire \dividend0_reg[28]_i_2__0_n_6 ;
  wire \dividend0_reg[28]_i_2__0_n_7 ;
  wire \dividend0_reg[28]_i_2__0_n_8 ;
  wire [31:0]\dividend0_reg[31]_0 ;
  wire \dividend0_reg[31]_i_2__0_n_3 ;
  wire \dividend0_reg[31]_i_2__0_n_4 ;
  wire \dividend0_reg[31]_i_2__0_n_6 ;
  wire \dividend0_reg[31]_i_2__0_n_7 ;
  wire \dividend0_reg[31]_i_2__0_n_8 ;
  wire \dividend0_reg[4]_i_2__0_n_1 ;
  wire \dividend0_reg[4]_i_2__0_n_2 ;
  wire \dividend0_reg[4]_i_2__0_n_3 ;
  wire \dividend0_reg[4]_i_2__0_n_4 ;
  wire \dividend0_reg[4]_i_2__0_n_5 ;
  wire \dividend0_reg[4]_i_2__0_n_6 ;
  wire \dividend0_reg[4]_i_2__0_n_7 ;
  wire \dividend0_reg[4]_i_2__0_n_8 ;
  wire \dividend0_reg[8]_i_2__0_n_1 ;
  wire \dividend0_reg[8]_i_2__0_n_2 ;
  wire \dividend0_reg[8]_i_2__0_n_3 ;
  wire \dividend0_reg[8]_i_2__0_n_4 ;
  wire \dividend0_reg[8]_i_2__0_n_5 ;
  wire \dividend0_reg[8]_i_2__0_n_6 ;
  wire \dividend0_reg[8]_i_2__0_n_7 ;
  wire \dividend0_reg[8]_i_2__0_n_8 ;
  wire \dividend0_reg_n_1_[0] ;
  wire \dividend0_reg_n_1_[10] ;
  wire \dividend0_reg_n_1_[11] ;
  wire \dividend0_reg_n_1_[12] ;
  wire \dividend0_reg_n_1_[13] ;
  wire \dividend0_reg_n_1_[14] ;
  wire \dividend0_reg_n_1_[15] ;
  wire \dividend0_reg_n_1_[16] ;
  wire \dividend0_reg_n_1_[17] ;
  wire \dividend0_reg_n_1_[18] ;
  wire \dividend0_reg_n_1_[19] ;
  wire \dividend0_reg_n_1_[1] ;
  wire \dividend0_reg_n_1_[20] ;
  wire \dividend0_reg_n_1_[21] ;
  wire \dividend0_reg_n_1_[22] ;
  wire \dividend0_reg_n_1_[23] ;
  wire \dividend0_reg_n_1_[24] ;
  wire \dividend0_reg_n_1_[25] ;
  wire \dividend0_reg_n_1_[26] ;
  wire \dividend0_reg_n_1_[27] ;
  wire \dividend0_reg_n_1_[28] ;
  wire \dividend0_reg_n_1_[29] ;
  wire \dividend0_reg_n_1_[2] ;
  wire \dividend0_reg_n_1_[30] ;
  wire \dividend0_reg_n_1_[3] ;
  wire \dividend0_reg_n_1_[4] ;
  wire \dividend0_reg_n_1_[5] ;
  wire \dividend0_reg_n_1_[6] ;
  wire \dividend0_reg_n_1_[7] ;
  wire \dividend0_reg_n_1_[8] ;
  wire \dividend0_reg_n_1_[9] ;
  wire loop_imperfect_srbkb_div_u_0_n_1;
  wire loop_imperfect_srbkb_div_u_0_n_10;
  wire loop_imperfect_srbkb_div_u_0_n_11;
  wire loop_imperfect_srbkb_div_u_0_n_12;
  wire loop_imperfect_srbkb_div_u_0_n_13;
  wire loop_imperfect_srbkb_div_u_0_n_14;
  wire loop_imperfect_srbkb_div_u_0_n_15;
  wire loop_imperfect_srbkb_div_u_0_n_16;
  wire loop_imperfect_srbkb_div_u_0_n_17;
  wire loop_imperfect_srbkb_div_u_0_n_18;
  wire loop_imperfect_srbkb_div_u_0_n_19;
  wire loop_imperfect_srbkb_div_u_0_n_2;
  wire loop_imperfect_srbkb_div_u_0_n_20;
  wire loop_imperfect_srbkb_div_u_0_n_21;
  wire loop_imperfect_srbkb_div_u_0_n_22;
  wire loop_imperfect_srbkb_div_u_0_n_23;
  wire loop_imperfect_srbkb_div_u_0_n_24;
  wire loop_imperfect_srbkb_div_u_0_n_25;
  wire loop_imperfect_srbkb_div_u_0_n_26;
  wire loop_imperfect_srbkb_div_u_0_n_27;
  wire loop_imperfect_srbkb_div_u_0_n_28;
  wire loop_imperfect_srbkb_div_u_0_n_29;
  wire loop_imperfect_srbkb_div_u_0_n_3;
  wire loop_imperfect_srbkb_div_u_0_n_30;
  wire loop_imperfect_srbkb_div_u_0_n_31;
  wire loop_imperfect_srbkb_div_u_0_n_32;
  wire loop_imperfect_srbkb_div_u_0_n_33;
  wire loop_imperfect_srbkb_div_u_0_n_4;
  wire loop_imperfect_srbkb_div_u_0_n_5;
  wire loop_imperfect_srbkb_div_u_0_n_6;
  wire loop_imperfect_srbkb_div_u_0_n_7;
  wire loop_imperfect_srbkb_div_u_0_n_8;
  wire loop_imperfect_srbkb_div_u_0_n_9;
  wire p_1_in;
  wire \r_stage_reg[32] ;
  wire [31:0]\remd_reg[31]_0 ;
  wire start0_reg_n_1;
  wire [3:2]\NLW_dividend0_reg[31]_i_2__0_CO_UNCONNECTED ;
  wire [3:3]\NLW_dividend0_reg[31]_i_2__0_O_UNCONNECTED ;

  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[10]_i_1 
       (.I0(\dividend0_reg[12]_i_2__0_n_7 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[10] ),
        .O(\dividend0[10]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[11]_i_1 
       (.I0(\dividend0_reg[12]_i_2__0_n_6 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[11] ),
        .O(\dividend0[11]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[12]_i_1 
       (.I0(\dividend0_reg[12]_i_2__0_n_5 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[12] ),
        .O(\dividend0[12]_i_1_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[12]_i_3 
       (.I0(\dividend0_reg_n_1_[12] ),
        .O(\dividend0[12]_i_3_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[12]_i_4 
       (.I0(\dividend0_reg_n_1_[11] ),
        .O(\dividend0[12]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[12]_i_5 
       (.I0(\dividend0_reg_n_1_[10] ),
        .O(\dividend0[12]_i_5_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[12]_i_6 
       (.I0(\dividend0_reg_n_1_[9] ),
        .O(\dividend0[12]_i_6_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[13]_i_1 
       (.I0(\dividend0_reg[16]_i_2__0_n_8 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[13] ),
        .O(\dividend0[13]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[14]_i_1 
       (.I0(\dividend0_reg[16]_i_2__0_n_7 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[14] ),
        .O(\dividend0[14]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[15]_i_1 
       (.I0(\dividend0_reg[16]_i_2__0_n_6 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[15] ),
        .O(\dividend0[15]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[16]_i_1 
       (.I0(\dividend0_reg[16]_i_2__0_n_5 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[16] ),
        .O(\dividend0[16]_i_1_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[16]_i_3 
       (.I0(\dividend0_reg_n_1_[16] ),
        .O(\dividend0[16]_i_3_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[16]_i_4 
       (.I0(\dividend0_reg_n_1_[15] ),
        .O(\dividend0[16]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[16]_i_5 
       (.I0(\dividend0_reg_n_1_[14] ),
        .O(\dividend0[16]_i_5_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[16]_i_6 
       (.I0(\dividend0_reg_n_1_[13] ),
        .O(\dividend0[16]_i_6_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[17]_i_1 
       (.I0(\dividend0_reg[20]_i_2__0_n_8 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[17] ),
        .O(\dividend0[17]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[18]_i_1 
       (.I0(\dividend0_reg[20]_i_2__0_n_7 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[18] ),
        .O(\dividend0[18]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[19]_i_1 
       (.I0(\dividend0_reg[20]_i_2__0_n_6 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[19] ),
        .O(\dividend0[19]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[1]_i_1 
       (.I0(\dividend0_reg[4]_i_2__0_n_8 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[1] ),
        .O(\dividend0[1]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[20]_i_1 
       (.I0(\dividend0_reg[20]_i_2__0_n_5 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[20] ),
        .O(\dividend0[20]_i_1_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[20]_i_3 
       (.I0(\dividend0_reg_n_1_[20] ),
        .O(\dividend0[20]_i_3_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[20]_i_4 
       (.I0(\dividend0_reg_n_1_[19] ),
        .O(\dividend0[20]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[20]_i_5 
       (.I0(\dividend0_reg_n_1_[18] ),
        .O(\dividend0[20]_i_5_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[20]_i_6 
       (.I0(\dividend0_reg_n_1_[17] ),
        .O(\dividend0[20]_i_6_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[21]_i_1 
       (.I0(\dividend0_reg[24]_i_2__0_n_8 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[21] ),
        .O(\dividend0[21]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[22]_i_1 
       (.I0(\dividend0_reg[24]_i_2__0_n_7 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[22] ),
        .O(\dividend0[22]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[23]_i_1 
       (.I0(\dividend0_reg[24]_i_2__0_n_6 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[23] ),
        .O(\dividend0[23]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[24]_i_1 
       (.I0(\dividend0_reg[24]_i_2__0_n_5 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[24] ),
        .O(\dividend0[24]_i_1_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[24]_i_3 
       (.I0(\dividend0_reg_n_1_[24] ),
        .O(\dividend0[24]_i_3_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[24]_i_4 
       (.I0(\dividend0_reg_n_1_[23] ),
        .O(\dividend0[24]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[24]_i_5 
       (.I0(\dividend0_reg_n_1_[22] ),
        .O(\dividend0[24]_i_5_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[24]_i_6 
       (.I0(\dividend0_reg_n_1_[21] ),
        .O(\dividend0[24]_i_6_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[25]_i_1 
       (.I0(\dividend0_reg[28]_i_2__0_n_8 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[25] ),
        .O(\dividend0[25]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[26]_i_1 
       (.I0(\dividend0_reg[28]_i_2__0_n_7 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[26] ),
        .O(\dividend0[26]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[27]_i_1 
       (.I0(\dividend0_reg[28]_i_2__0_n_6 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[27] ),
        .O(\dividend0[27]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[28]_i_1 
       (.I0(\dividend0_reg[28]_i_2__0_n_5 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[28] ),
        .O(\dividend0[28]_i_1_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[28]_i_3 
       (.I0(\dividend0_reg_n_1_[28] ),
        .O(\dividend0[28]_i_3_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[28]_i_4 
       (.I0(\dividend0_reg_n_1_[27] ),
        .O(\dividend0[28]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[28]_i_5 
       (.I0(\dividend0_reg_n_1_[26] ),
        .O(\dividend0[28]_i_5_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[28]_i_6 
       (.I0(\dividend0_reg_n_1_[25] ),
        .O(\dividend0[28]_i_6_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[29]_i_1 
       (.I0(\dividend0_reg[31]_i_2__0_n_8 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[29] ),
        .O(\dividend0[29]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[2]_i_1 
       (.I0(\dividend0_reg[4]_i_2__0_n_7 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[2] ),
        .O(\dividend0[2]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[30]_i_1 
       (.I0(\dividend0_reg[31]_i_2__0_n_7 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[30] ),
        .O(\dividend0[30]_i_1_n_1 ));
  LUT2 #(
    .INIT(4'h8)) 
    \dividend0[31]_i_1 
       (.I0(p_1_in),
        .I1(\dividend0_reg[31]_i_2__0_n_6 ),
        .O(\dividend0[31]_i_1_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[31]_i_3 
       (.I0(p_1_in),
        .O(\dividend0[31]_i_3_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[31]_i_4 
       (.I0(\dividend0_reg_n_1_[30] ),
        .O(\dividend0[31]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[31]_i_5 
       (.I0(\dividend0_reg_n_1_[29] ),
        .O(\dividend0[31]_i_5_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[3]_i_1 
       (.I0(\dividend0_reg[4]_i_2__0_n_6 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[3] ),
        .O(\dividend0[3]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[4]_i_1 
       (.I0(\dividend0_reg[4]_i_2__0_n_5 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[4] ),
        .O(\dividend0[4]_i_1_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[4]_i_3 
       (.I0(\dividend0_reg_n_1_[0] ),
        .O(\dividend0[4]_i_3_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[4]_i_4 
       (.I0(\dividend0_reg_n_1_[4] ),
        .O(\dividend0[4]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[4]_i_5 
       (.I0(\dividend0_reg_n_1_[3] ),
        .O(\dividend0[4]_i_5_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[4]_i_6 
       (.I0(\dividend0_reg_n_1_[2] ),
        .O(\dividend0[4]_i_6_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[4]_i_7 
       (.I0(\dividend0_reg_n_1_[1] ),
        .O(\dividend0[4]_i_7_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[5]_i_1 
       (.I0(\dividend0_reg[8]_i_2__0_n_8 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[5] ),
        .O(\dividend0[5]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[6]_i_1 
       (.I0(\dividend0_reg[8]_i_2__0_n_7 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[6] ),
        .O(\dividend0[6]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[7]_i_1 
       (.I0(\dividend0_reg[8]_i_2__0_n_6 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[7] ),
        .O(\dividend0[7]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[8]_i_1 
       (.I0(\dividend0_reg[8]_i_2__0_n_5 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[8] ),
        .O(\dividend0[8]_i_1_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[8]_i_3 
       (.I0(\dividend0_reg_n_1_[8] ),
        .O(\dividend0[8]_i_3_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[8]_i_4 
       (.I0(\dividend0_reg_n_1_[7] ),
        .O(\dividend0[8]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[8]_i_5 
       (.I0(\dividend0_reg_n_1_[6] ),
        .O(\dividend0[8]_i_5_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[8]_i_6 
       (.I0(\dividend0_reg_n_1_[5] ),
        .O(\dividend0[8]_i_6_n_1 ));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[9]_i_1 
       (.I0(\dividend0_reg[12]_i_2__0_n_8 ),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[9] ),
        .O(\dividend0[9]_i_1_n_1 ));
  FDRE \dividend0_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [0]),
        .Q(\dividend0_reg_n_1_[0] ),
        .R(1'b0));
  FDRE \dividend0_reg[10] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [10]),
        .Q(\dividend0_reg_n_1_[10] ),
        .R(1'b0));
  FDRE \dividend0_reg[11] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [11]),
        .Q(\dividend0_reg_n_1_[11] ),
        .R(1'b0));
  FDRE \dividend0_reg[12] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [12]),
        .Q(\dividend0_reg_n_1_[12] ),
        .R(1'b0));
  CARRY4 \dividend0_reg[12]_i_2__0 
       (.CI(\dividend0_reg[8]_i_2__0_n_1 ),
        .CO({\dividend0_reg[12]_i_2__0_n_1 ,\dividend0_reg[12]_i_2__0_n_2 ,\dividend0_reg[12]_i_2__0_n_3 ,\dividend0_reg[12]_i_2__0_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\dividend0_reg[12]_i_2__0_n_5 ,\dividend0_reg[12]_i_2__0_n_6 ,\dividend0_reg[12]_i_2__0_n_7 ,\dividend0_reg[12]_i_2__0_n_8 }),
        .S({\dividend0[12]_i_3_n_1 ,\dividend0[12]_i_4_n_1 ,\dividend0[12]_i_5_n_1 ,\dividend0[12]_i_6_n_1 }));
  FDRE \dividend0_reg[13] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [13]),
        .Q(\dividend0_reg_n_1_[13] ),
        .R(1'b0));
  FDRE \dividend0_reg[14] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [14]),
        .Q(\dividend0_reg_n_1_[14] ),
        .R(1'b0));
  FDRE \dividend0_reg[15] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [15]),
        .Q(\dividend0_reg_n_1_[15] ),
        .R(1'b0));
  FDRE \dividend0_reg[16] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [16]),
        .Q(\dividend0_reg_n_1_[16] ),
        .R(1'b0));
  CARRY4 \dividend0_reg[16]_i_2__0 
       (.CI(\dividend0_reg[12]_i_2__0_n_1 ),
        .CO({\dividend0_reg[16]_i_2__0_n_1 ,\dividend0_reg[16]_i_2__0_n_2 ,\dividend0_reg[16]_i_2__0_n_3 ,\dividend0_reg[16]_i_2__0_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\dividend0_reg[16]_i_2__0_n_5 ,\dividend0_reg[16]_i_2__0_n_6 ,\dividend0_reg[16]_i_2__0_n_7 ,\dividend0_reg[16]_i_2__0_n_8 }),
        .S({\dividend0[16]_i_3_n_1 ,\dividend0[16]_i_4_n_1 ,\dividend0[16]_i_5_n_1 ,\dividend0[16]_i_6_n_1 }));
  FDRE \dividend0_reg[17] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [17]),
        .Q(\dividend0_reg_n_1_[17] ),
        .R(1'b0));
  FDRE \dividend0_reg[18] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [18]),
        .Q(\dividend0_reg_n_1_[18] ),
        .R(1'b0));
  FDRE \dividend0_reg[19] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [19]),
        .Q(\dividend0_reg_n_1_[19] ),
        .R(1'b0));
  FDRE \dividend0_reg[1] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [1]),
        .Q(\dividend0_reg_n_1_[1] ),
        .R(1'b0));
  FDRE \dividend0_reg[20] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [20]),
        .Q(\dividend0_reg_n_1_[20] ),
        .R(1'b0));
  CARRY4 \dividend0_reg[20]_i_2__0 
       (.CI(\dividend0_reg[16]_i_2__0_n_1 ),
        .CO({\dividend0_reg[20]_i_2__0_n_1 ,\dividend0_reg[20]_i_2__0_n_2 ,\dividend0_reg[20]_i_2__0_n_3 ,\dividend0_reg[20]_i_2__0_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\dividend0_reg[20]_i_2__0_n_5 ,\dividend0_reg[20]_i_2__0_n_6 ,\dividend0_reg[20]_i_2__0_n_7 ,\dividend0_reg[20]_i_2__0_n_8 }),
        .S({\dividend0[20]_i_3_n_1 ,\dividend0[20]_i_4_n_1 ,\dividend0[20]_i_5_n_1 ,\dividend0[20]_i_6_n_1 }));
  FDRE \dividend0_reg[21] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [21]),
        .Q(\dividend0_reg_n_1_[21] ),
        .R(1'b0));
  FDRE \dividend0_reg[22] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [22]),
        .Q(\dividend0_reg_n_1_[22] ),
        .R(1'b0));
  FDRE \dividend0_reg[23] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [23]),
        .Q(\dividend0_reg_n_1_[23] ),
        .R(1'b0));
  FDRE \dividend0_reg[24] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [24]),
        .Q(\dividend0_reg_n_1_[24] ),
        .R(1'b0));
  CARRY4 \dividend0_reg[24]_i_2__0 
       (.CI(\dividend0_reg[20]_i_2__0_n_1 ),
        .CO({\dividend0_reg[24]_i_2__0_n_1 ,\dividend0_reg[24]_i_2__0_n_2 ,\dividend0_reg[24]_i_2__0_n_3 ,\dividend0_reg[24]_i_2__0_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\dividend0_reg[24]_i_2__0_n_5 ,\dividend0_reg[24]_i_2__0_n_6 ,\dividend0_reg[24]_i_2__0_n_7 ,\dividend0_reg[24]_i_2__0_n_8 }),
        .S({\dividend0[24]_i_3_n_1 ,\dividend0[24]_i_4_n_1 ,\dividend0[24]_i_5_n_1 ,\dividend0[24]_i_6_n_1 }));
  FDRE \dividend0_reg[25] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [25]),
        .Q(\dividend0_reg_n_1_[25] ),
        .R(1'b0));
  FDRE \dividend0_reg[26] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [26]),
        .Q(\dividend0_reg_n_1_[26] ),
        .R(1'b0));
  FDRE \dividend0_reg[27] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [27]),
        .Q(\dividend0_reg_n_1_[27] ),
        .R(1'b0));
  FDRE \dividend0_reg[28] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [28]),
        .Q(\dividend0_reg_n_1_[28] ),
        .R(1'b0));
  CARRY4 \dividend0_reg[28]_i_2__0 
       (.CI(\dividend0_reg[24]_i_2__0_n_1 ),
        .CO({\dividend0_reg[28]_i_2__0_n_1 ,\dividend0_reg[28]_i_2__0_n_2 ,\dividend0_reg[28]_i_2__0_n_3 ,\dividend0_reg[28]_i_2__0_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\dividend0_reg[28]_i_2__0_n_5 ,\dividend0_reg[28]_i_2__0_n_6 ,\dividend0_reg[28]_i_2__0_n_7 ,\dividend0_reg[28]_i_2__0_n_8 }),
        .S({\dividend0[28]_i_3_n_1 ,\dividend0[28]_i_4_n_1 ,\dividend0[28]_i_5_n_1 ,\dividend0[28]_i_6_n_1 }));
  FDRE \dividend0_reg[29] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [29]),
        .Q(\dividend0_reg_n_1_[29] ),
        .R(1'b0));
  FDRE \dividend0_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [2]),
        .Q(\dividend0_reg_n_1_[2] ),
        .R(1'b0));
  FDRE \dividend0_reg[30] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [30]),
        .Q(\dividend0_reg_n_1_[30] ),
        .R(1'b0));
  FDRE \dividend0_reg[31] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [31]),
        .Q(p_1_in),
        .R(1'b0));
  CARRY4 \dividend0_reg[31]_i_2__0 
       (.CI(\dividend0_reg[28]_i_2__0_n_1 ),
        .CO({\NLW_dividend0_reg[31]_i_2__0_CO_UNCONNECTED [3:2],\dividend0_reg[31]_i_2__0_n_3 ,\dividend0_reg[31]_i_2__0_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\NLW_dividend0_reg[31]_i_2__0_O_UNCONNECTED [3],\dividend0_reg[31]_i_2__0_n_6 ,\dividend0_reg[31]_i_2__0_n_7 ,\dividend0_reg[31]_i_2__0_n_8 }),
        .S({1'b0,\dividend0[31]_i_3_n_1 ,\dividend0[31]_i_4_n_1 ,\dividend0[31]_i_5_n_1 }));
  FDRE \dividend0_reg[3] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [3]),
        .Q(\dividend0_reg_n_1_[3] ),
        .R(1'b0));
  FDRE \dividend0_reg[4] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [4]),
        .Q(\dividend0_reg_n_1_[4] ),
        .R(1'b0));
  CARRY4 \dividend0_reg[4]_i_2__0 
       (.CI(1'b0),
        .CO({\dividend0_reg[4]_i_2__0_n_1 ,\dividend0_reg[4]_i_2__0_n_2 ,\dividend0_reg[4]_i_2__0_n_3 ,\dividend0_reg[4]_i_2__0_n_4 }),
        .CYINIT(\dividend0[4]_i_3_n_1 ),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\dividend0_reg[4]_i_2__0_n_5 ,\dividend0_reg[4]_i_2__0_n_6 ,\dividend0_reg[4]_i_2__0_n_7 ,\dividend0_reg[4]_i_2__0_n_8 }),
        .S({\dividend0[4]_i_4_n_1 ,\dividend0[4]_i_5_n_1 ,\dividend0[4]_i_6_n_1 ,\dividend0[4]_i_7_n_1 }));
  FDRE \dividend0_reg[5] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [5]),
        .Q(\dividend0_reg_n_1_[5] ),
        .R(1'b0));
  FDRE \dividend0_reg[6] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [6]),
        .Q(\dividend0_reg_n_1_[6] ),
        .R(1'b0));
  FDRE \dividend0_reg[7] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [7]),
        .Q(\dividend0_reg_n_1_[7] ),
        .R(1'b0));
  FDRE \dividend0_reg[8] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [8]),
        .Q(\dividend0_reg_n_1_[8] ),
        .R(1'b0));
  CARRY4 \dividend0_reg[8]_i_2__0 
       (.CI(\dividend0_reg[4]_i_2__0_n_1 ),
        .CO({\dividend0_reg[8]_i_2__0_n_1 ,\dividend0_reg[8]_i_2__0_n_2 ,\dividend0_reg[8]_i_2__0_n_3 ,\dividend0_reg[8]_i_2__0_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\dividend0_reg[8]_i_2__0_n_5 ,\dividend0_reg[8]_i_2__0_n_6 ,\dividend0_reg[8]_i_2__0_n_7 ,\dividend0_reg[8]_i_2__0_n_8 }),
        .S({\dividend0[8]_i_3_n_1 ,\dividend0[8]_i_4_n_1 ,\dividend0[8]_i_5_n_1 ,\dividend0[8]_i_6_n_1 }));
  FDRE \dividend0_reg[9] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend0_reg[31]_0 [9]),
        .Q(\dividend0_reg_n_1_[9] ),
        .R(1'b0));
  bd_0_hls_inst_0_loop_imperfect_srbkb_div_u loop_imperfect_srbkb_div_u_0
       (.D({\dividend0[31]_i_1_n_1 ,\dividend0[30]_i_1_n_1 ,\dividend0[29]_i_1_n_1 ,\dividend0[28]_i_1_n_1 ,\dividend0[27]_i_1_n_1 ,\dividend0[26]_i_1_n_1 ,\dividend0[25]_i_1_n_1 ,\dividend0[24]_i_1_n_1 ,\dividend0[23]_i_1_n_1 ,\dividend0[22]_i_1_n_1 ,\dividend0[21]_i_1_n_1 ,\dividend0[20]_i_1_n_1 ,\dividend0[19]_i_1_n_1 ,\dividend0[18]_i_1_n_1 ,\dividend0[17]_i_1_n_1 ,\dividend0[16]_i_1_n_1 ,\dividend0[15]_i_1_n_1 ,\dividend0[14]_i_1_n_1 ,\dividend0[13]_i_1_n_1 ,\dividend0[12]_i_1_n_1 ,\dividend0[11]_i_1_n_1 ,\dividend0[10]_i_1_n_1 ,\dividend0[9]_i_1_n_1 ,\dividend0[8]_i_1_n_1 ,\dividend0[7]_i_1_n_1 ,\dividend0[6]_i_1_n_1 ,\dividend0[5]_i_1_n_1 ,\dividend0[4]_i_1_n_1 ,\dividend0[3]_i_1_n_1 ,\dividend0[2]_i_1_n_1 ,\dividend0[1]_i_1_n_1 }),
        .E(start0_reg_n_1),
        .O9({loop_imperfect_srbkb_div_u_0_n_2,loop_imperfect_srbkb_div_u_0_n_3,loop_imperfect_srbkb_div_u_0_n_4,loop_imperfect_srbkb_div_u_0_n_5,loop_imperfect_srbkb_div_u_0_n_6,loop_imperfect_srbkb_div_u_0_n_7,loop_imperfect_srbkb_div_u_0_n_8,loop_imperfect_srbkb_div_u_0_n_9,loop_imperfect_srbkb_div_u_0_n_10,loop_imperfect_srbkb_div_u_0_n_11,loop_imperfect_srbkb_div_u_0_n_12,loop_imperfect_srbkb_div_u_0_n_13,loop_imperfect_srbkb_div_u_0_n_14,loop_imperfect_srbkb_div_u_0_n_15,loop_imperfect_srbkb_div_u_0_n_16,loop_imperfect_srbkb_div_u_0_n_17,loop_imperfect_srbkb_div_u_0_n_18,loop_imperfect_srbkb_div_u_0_n_19,loop_imperfect_srbkb_div_u_0_n_20,loop_imperfect_srbkb_div_u_0_n_21,loop_imperfect_srbkb_div_u_0_n_22,loop_imperfect_srbkb_div_u_0_n_23,loop_imperfect_srbkb_div_u_0_n_24,loop_imperfect_srbkb_div_u_0_n_25,loop_imperfect_srbkb_div_u_0_n_26,loop_imperfect_srbkb_div_u_0_n_27,loop_imperfect_srbkb_div_u_0_n_28,loop_imperfect_srbkb_div_u_0_n_29,loop_imperfect_srbkb_div_u_0_n_30,loop_imperfect_srbkb_div_u_0_n_31,loop_imperfect_srbkb_div_u_0_n_32,loop_imperfect_srbkb_div_u_0_n_33}),
        .Q({p_1_in,\dividend0_reg_n_1_[0] }),
        .ap_clk(ap_clk),
        .ap_rst(ap_rst),
        .\r_stage_reg[32]_0 (loop_imperfect_srbkb_div_u_0_n_1),
        .\r_stage_reg[32]_1 (\r_stage_reg[32] ));
  FDRE \remd_reg[0] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_33),
        .Q(\remd_reg[31]_0 [0]),
        .R(1'b0));
  FDRE \remd_reg[10] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_23),
        .Q(\remd_reg[31]_0 [10]),
        .R(1'b0));
  FDRE \remd_reg[11] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_22),
        .Q(\remd_reg[31]_0 [11]),
        .R(1'b0));
  FDRE \remd_reg[12] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_21),
        .Q(\remd_reg[31]_0 [12]),
        .R(1'b0));
  FDRE \remd_reg[13] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_20),
        .Q(\remd_reg[31]_0 [13]),
        .R(1'b0));
  FDRE \remd_reg[14] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_19),
        .Q(\remd_reg[31]_0 [14]),
        .R(1'b0));
  FDRE \remd_reg[15] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_18),
        .Q(\remd_reg[31]_0 [15]),
        .R(1'b0));
  FDRE \remd_reg[16] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_17),
        .Q(\remd_reg[31]_0 [16]),
        .R(1'b0));
  FDRE \remd_reg[17] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_16),
        .Q(\remd_reg[31]_0 [17]),
        .R(1'b0));
  FDRE \remd_reg[18] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_15),
        .Q(\remd_reg[31]_0 [18]),
        .R(1'b0));
  FDRE \remd_reg[19] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_14),
        .Q(\remd_reg[31]_0 [19]),
        .R(1'b0));
  FDRE \remd_reg[1] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_32),
        .Q(\remd_reg[31]_0 [1]),
        .R(1'b0));
  FDRE \remd_reg[20] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_13),
        .Q(\remd_reg[31]_0 [20]),
        .R(1'b0));
  FDRE \remd_reg[21] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_12),
        .Q(\remd_reg[31]_0 [21]),
        .R(1'b0));
  FDRE \remd_reg[22] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_11),
        .Q(\remd_reg[31]_0 [22]),
        .R(1'b0));
  FDRE \remd_reg[23] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_10),
        .Q(\remd_reg[31]_0 [23]),
        .R(1'b0));
  FDRE \remd_reg[24] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_9),
        .Q(\remd_reg[31]_0 [24]),
        .R(1'b0));
  FDRE \remd_reg[25] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_8),
        .Q(\remd_reg[31]_0 [25]),
        .R(1'b0));
  FDRE \remd_reg[26] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_7),
        .Q(\remd_reg[31]_0 [26]),
        .R(1'b0));
  FDRE \remd_reg[27] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_6),
        .Q(\remd_reg[31]_0 [27]),
        .R(1'b0));
  FDRE \remd_reg[28] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_5),
        .Q(\remd_reg[31]_0 [28]),
        .R(1'b0));
  FDRE \remd_reg[29] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_4),
        .Q(\remd_reg[31]_0 [29]),
        .R(1'b0));
  FDRE \remd_reg[2] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_31),
        .Q(\remd_reg[31]_0 [2]),
        .R(1'b0));
  FDRE \remd_reg[30] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_3),
        .Q(\remd_reg[31]_0 [30]),
        .R(1'b0));
  FDRE \remd_reg[31] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_2),
        .Q(\remd_reg[31]_0 [31]),
        .R(1'b0));
  FDRE \remd_reg[3] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_30),
        .Q(\remd_reg[31]_0 [3]),
        .R(1'b0));
  FDRE \remd_reg[4] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_29),
        .Q(\remd_reg[31]_0 [4]),
        .R(1'b0));
  FDRE \remd_reg[5] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_28),
        .Q(\remd_reg[31]_0 [5]),
        .R(1'b0));
  FDRE \remd_reg[6] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_27),
        .Q(\remd_reg[31]_0 [6]),
        .R(1'b0));
  FDRE \remd_reg[7] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_26),
        .Q(\remd_reg[31]_0 [7]),
        .R(1'b0));
  FDRE \remd_reg[8] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_25),
        .Q(\remd_reg[31]_0 [8]),
        .R(1'b0));
  FDRE \remd_reg[9] 
       (.C(ap_clk),
        .CE(loop_imperfect_srbkb_div_u_0_n_1),
        .D(loop_imperfect_srbkb_div_u_0_n_24),
        .Q(\remd_reg[31]_0 [9]),
        .R(1'b0));
  FDRE #(
    .INIT(1'b0)) 
    start0_reg
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q),
        .Q(start0_reg_n_1),
        .R(1'b0));
endmodule

(* ORIG_REF_NAME = "loop_imperfect_srbkb_div" *) 
module bd_0_hls_inst_0_loop_imperfect_srbkb_div_1
   (r_stage_reg_r_29,
    D,
    \remd_reg[31]_0 ,
    Q,
    ap_clk,
    ap_rst,
    \dividend0_reg[31]_0 ,
    \dividend0_reg[31]_1 );
  output r_stage_reg_r_29;
  output [31:0]D;
  output [31:0]\remd_reg[31]_0 ;
  input [0:0]Q;
  input ap_clk;
  input ap_rst;
  input [31:0]\dividend0_reg[31]_0 ;
  input [31:0]\dividend0_reg[31]_1 ;

  wire [31:0]D;
  wire [0:0]Q;
  wire ap_clk;
  wire ap_rst;
  wire \dividend0[12]_i_3_n_1 ;
  wire \dividend0[12]_i_4_n_1 ;
  wire \dividend0[12]_i_5_n_1 ;
  wire \dividend0[12]_i_6_n_1 ;
  wire \dividend0[16]_i_3_n_1 ;
  wire \dividend0[16]_i_4_n_1 ;
  wire \dividend0[16]_i_5_n_1 ;
  wire \dividend0[16]_i_6_n_1 ;
  wire \dividend0[20]_i_3_n_1 ;
  wire \dividend0[20]_i_4_n_1 ;
  wire \dividend0[20]_i_5_n_1 ;
  wire \dividend0[20]_i_6_n_1 ;
  wire \dividend0[24]_i_3_n_1 ;
  wire \dividend0[24]_i_4_n_1 ;
  wire \dividend0[24]_i_5_n_1 ;
  wire \dividend0[24]_i_6_n_1 ;
  wire \dividend0[28]_i_3_n_1 ;
  wire \dividend0[28]_i_4_n_1 ;
  wire \dividend0[28]_i_5_n_1 ;
  wire \dividend0[28]_i_6_n_1 ;
  wire \dividend0[31]_i_3_n_1 ;
  wire \dividend0[31]_i_4_n_1 ;
  wire \dividend0[31]_i_5_n_1 ;
  wire \dividend0[4]_i_3_n_1 ;
  wire \dividend0[4]_i_4_n_1 ;
  wire \dividend0[4]_i_5_n_1 ;
  wire \dividend0[4]_i_6_n_1 ;
  wire \dividend0[4]_i_7_n_1 ;
  wire \dividend0[8]_i_3_n_1 ;
  wire \dividend0[8]_i_4_n_1 ;
  wire \dividend0[8]_i_5_n_1 ;
  wire \dividend0[8]_i_6_n_1 ;
  wire \dividend0_reg[12]_i_2_n_1 ;
  wire \dividend0_reg[12]_i_2_n_2 ;
  wire \dividend0_reg[12]_i_2_n_3 ;
  wire \dividend0_reg[12]_i_2_n_4 ;
  wire \dividend0_reg[16]_i_2_n_1 ;
  wire \dividend0_reg[16]_i_2_n_2 ;
  wire \dividend0_reg[16]_i_2_n_3 ;
  wire \dividend0_reg[16]_i_2_n_4 ;
  wire \dividend0_reg[20]_i_2_n_1 ;
  wire \dividend0_reg[20]_i_2_n_2 ;
  wire \dividend0_reg[20]_i_2_n_3 ;
  wire \dividend0_reg[20]_i_2_n_4 ;
  wire \dividend0_reg[24]_i_2_n_1 ;
  wire \dividend0_reg[24]_i_2_n_2 ;
  wire \dividend0_reg[24]_i_2_n_3 ;
  wire \dividend0_reg[24]_i_2_n_4 ;
  wire \dividend0_reg[28]_i_2_n_1 ;
  wire \dividend0_reg[28]_i_2_n_2 ;
  wire \dividend0_reg[28]_i_2_n_3 ;
  wire \dividend0_reg[28]_i_2_n_4 ;
  wire [31:0]\dividend0_reg[31]_0 ;
  wire [31:0]\dividend0_reg[31]_1 ;
  wire \dividend0_reg[31]_i_2_n_3 ;
  wire \dividend0_reg[31]_i_2_n_4 ;
  wire \dividend0_reg[4]_i_2_n_1 ;
  wire \dividend0_reg[4]_i_2_n_2 ;
  wire \dividend0_reg[4]_i_2_n_3 ;
  wire \dividend0_reg[4]_i_2_n_4 ;
  wire \dividend0_reg[8]_i_2_n_1 ;
  wire \dividend0_reg[8]_i_2_n_2 ;
  wire \dividend0_reg[8]_i_2_n_3 ;
  wire \dividend0_reg[8]_i_2_n_4 ;
  wire \dividend0_reg_n_1_[0] ;
  wire \dividend0_reg_n_1_[10] ;
  wire \dividend0_reg_n_1_[11] ;
  wire \dividend0_reg_n_1_[12] ;
  wire \dividend0_reg_n_1_[13] ;
  wire \dividend0_reg_n_1_[14] ;
  wire \dividend0_reg_n_1_[15] ;
  wire \dividend0_reg_n_1_[16] ;
  wire \dividend0_reg_n_1_[17] ;
  wire \dividend0_reg_n_1_[18] ;
  wire \dividend0_reg_n_1_[19] ;
  wire \dividend0_reg_n_1_[1] ;
  wire \dividend0_reg_n_1_[20] ;
  wire \dividend0_reg_n_1_[21] ;
  wire \dividend0_reg_n_1_[22] ;
  wire \dividend0_reg_n_1_[23] ;
  wire \dividend0_reg_n_1_[24] ;
  wire \dividend0_reg_n_1_[25] ;
  wire \dividend0_reg_n_1_[26] ;
  wire \dividend0_reg_n_1_[27] ;
  wire \dividend0_reg_n_1_[28] ;
  wire \dividend0_reg_n_1_[29] ;
  wire \dividend0_reg_n_1_[2] ;
  wire \dividend0_reg_n_1_[30] ;
  wire \dividend0_reg_n_1_[3] ;
  wire \dividend0_reg_n_1_[4] ;
  wire \dividend0_reg_n_1_[5] ;
  wire \dividend0_reg_n_1_[6] ;
  wire \dividend0_reg_n_1_[7] ;
  wire \dividend0_reg_n_1_[8] ;
  wire \dividend0_reg_n_1_[9] ;
  wire [31:1]dividend_u;
  wire [31:1]dividend_u0;
  wire done0;
  wire loop_imperfect_srbkb_div_u_0_n_10;
  wire loop_imperfect_srbkb_div_u_0_n_11;
  wire loop_imperfect_srbkb_div_u_0_n_12;
  wire loop_imperfect_srbkb_div_u_0_n_13;
  wire loop_imperfect_srbkb_div_u_0_n_14;
  wire loop_imperfect_srbkb_div_u_0_n_15;
  wire loop_imperfect_srbkb_div_u_0_n_16;
  wire loop_imperfect_srbkb_div_u_0_n_17;
  wire loop_imperfect_srbkb_div_u_0_n_18;
  wire loop_imperfect_srbkb_div_u_0_n_19;
  wire loop_imperfect_srbkb_div_u_0_n_20;
  wire loop_imperfect_srbkb_div_u_0_n_21;
  wire loop_imperfect_srbkb_div_u_0_n_22;
  wire loop_imperfect_srbkb_div_u_0_n_23;
  wire loop_imperfect_srbkb_div_u_0_n_24;
  wire loop_imperfect_srbkb_div_u_0_n_25;
  wire loop_imperfect_srbkb_div_u_0_n_26;
  wire loop_imperfect_srbkb_div_u_0_n_27;
  wire loop_imperfect_srbkb_div_u_0_n_28;
  wire loop_imperfect_srbkb_div_u_0_n_29;
  wire loop_imperfect_srbkb_div_u_0_n_3;
  wire loop_imperfect_srbkb_div_u_0_n_30;
  wire loop_imperfect_srbkb_div_u_0_n_31;
  wire loop_imperfect_srbkb_div_u_0_n_32;
  wire loop_imperfect_srbkb_div_u_0_n_33;
  wire loop_imperfect_srbkb_div_u_0_n_34;
  wire loop_imperfect_srbkb_div_u_0_n_4;
  wire loop_imperfect_srbkb_div_u_0_n_5;
  wire loop_imperfect_srbkb_div_u_0_n_6;
  wire loop_imperfect_srbkb_div_u_0_n_7;
  wire loop_imperfect_srbkb_div_u_0_n_8;
  wire loop_imperfect_srbkb_div_u_0_n_9;
  wire p_1_in;
  wire r_stage_reg_r_29;
  wire [31:0]\remd_reg[31]_0 ;
  wire start0;
  wire [3:2]\NLW_dividend0_reg[31]_i_2_CO_UNCONNECTED ;
  wire [3:3]\NLW_dividend0_reg[31]_i_2_O_UNCONNECTED ;

  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[0]_i_1 
       (.I0(\dividend0_reg[31]_0 [0]),
        .I1(\dividend0_reg[31]_1 [0]),
        .O(D[0]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[10]_i_1 
       (.I0(\dividend0_reg[31]_0 [10]),
        .I1(\dividend0_reg[31]_1 [10]),
        .O(D[10]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[10]_i_1__0 
       (.I0(dividend_u0[10]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[10] ),
        .O(dividend_u[10]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[11]_i_1 
       (.I0(\dividend0_reg[31]_0 [11]),
        .I1(\dividend0_reg[31]_1 [11]),
        .O(D[11]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[11]_i_1__0 
       (.I0(dividend_u0[11]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[11] ),
        .O(dividend_u[11]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[12]_i_1 
       (.I0(\dividend0_reg[31]_0 [12]),
        .I1(\dividend0_reg[31]_1 [12]),
        .O(D[12]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[12]_i_1__0 
       (.I0(dividend_u0[12]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[12] ),
        .O(dividend_u[12]));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[12]_i_3 
       (.I0(\dividend0_reg_n_1_[12] ),
        .O(\dividend0[12]_i_3_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[12]_i_4 
       (.I0(\dividend0_reg_n_1_[11] ),
        .O(\dividend0[12]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[12]_i_5 
       (.I0(\dividend0_reg_n_1_[10] ),
        .O(\dividend0[12]_i_5_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[12]_i_6 
       (.I0(\dividend0_reg_n_1_[9] ),
        .O(\dividend0[12]_i_6_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[13]_i_1 
       (.I0(\dividend0_reg[31]_0 [13]),
        .I1(\dividend0_reg[31]_1 [13]),
        .O(D[13]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[13]_i_1__0 
       (.I0(dividend_u0[13]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[13] ),
        .O(dividend_u[13]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[14]_i_1 
       (.I0(\dividend0_reg[31]_0 [14]),
        .I1(\dividend0_reg[31]_1 [14]),
        .O(D[14]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[14]_i_1__0 
       (.I0(dividend_u0[14]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[14] ),
        .O(dividend_u[14]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[15]_i_1 
       (.I0(\dividend0_reg[31]_0 [15]),
        .I1(\dividend0_reg[31]_1 [15]),
        .O(D[15]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[15]_i_1__0 
       (.I0(dividend_u0[15]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[15] ),
        .O(dividend_u[15]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[16]_i_1 
       (.I0(\dividend0_reg[31]_0 [16]),
        .I1(\dividend0_reg[31]_1 [16]),
        .O(D[16]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[16]_i_1__0 
       (.I0(dividend_u0[16]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[16] ),
        .O(dividend_u[16]));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[16]_i_3 
       (.I0(\dividend0_reg_n_1_[16] ),
        .O(\dividend0[16]_i_3_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[16]_i_4 
       (.I0(\dividend0_reg_n_1_[15] ),
        .O(\dividend0[16]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[16]_i_5 
       (.I0(\dividend0_reg_n_1_[14] ),
        .O(\dividend0[16]_i_5_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[16]_i_6 
       (.I0(\dividend0_reg_n_1_[13] ),
        .O(\dividend0[16]_i_6_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[17]_i_1 
       (.I0(\dividend0_reg[31]_0 [17]),
        .I1(\dividend0_reg[31]_1 [17]),
        .O(D[17]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[17]_i_1__0 
       (.I0(dividend_u0[17]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[17] ),
        .O(dividend_u[17]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[18]_i_1 
       (.I0(\dividend0_reg[31]_0 [18]),
        .I1(\dividend0_reg[31]_1 [18]),
        .O(D[18]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[18]_i_1__0 
       (.I0(dividend_u0[18]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[18] ),
        .O(dividend_u[18]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[19]_i_1 
       (.I0(\dividend0_reg[31]_0 [19]),
        .I1(\dividend0_reg[31]_1 [19]),
        .O(D[19]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[19]_i_1__0 
       (.I0(dividend_u0[19]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[19] ),
        .O(dividend_u[19]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[1]_i_1 
       (.I0(\dividend0_reg[31]_0 [1]),
        .I1(\dividend0_reg[31]_1 [1]),
        .O(D[1]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[1]_i_1__0 
       (.I0(dividend_u0[1]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[1] ),
        .O(dividend_u[1]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[20]_i_1 
       (.I0(\dividend0_reg[31]_0 [20]),
        .I1(\dividend0_reg[31]_1 [20]),
        .O(D[20]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[20]_i_1__0 
       (.I0(dividend_u0[20]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[20] ),
        .O(dividend_u[20]));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[20]_i_3 
       (.I0(\dividend0_reg_n_1_[20] ),
        .O(\dividend0[20]_i_3_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[20]_i_4 
       (.I0(\dividend0_reg_n_1_[19] ),
        .O(\dividend0[20]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[20]_i_5 
       (.I0(\dividend0_reg_n_1_[18] ),
        .O(\dividend0[20]_i_5_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[20]_i_6 
       (.I0(\dividend0_reg_n_1_[17] ),
        .O(\dividend0[20]_i_6_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[21]_i_1 
       (.I0(\dividend0_reg[31]_0 [21]),
        .I1(\dividend0_reg[31]_1 [21]),
        .O(D[21]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[21]_i_1__0 
       (.I0(dividend_u0[21]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[21] ),
        .O(dividend_u[21]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[22]_i_1 
       (.I0(\dividend0_reg[31]_0 [22]),
        .I1(\dividend0_reg[31]_1 [22]),
        .O(D[22]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[22]_i_1__0 
       (.I0(dividend_u0[22]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[22] ),
        .O(dividend_u[22]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[23]_i_1 
       (.I0(\dividend0_reg[31]_0 [23]),
        .I1(\dividend0_reg[31]_1 [23]),
        .O(D[23]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[23]_i_1__0 
       (.I0(dividend_u0[23]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[23] ),
        .O(dividend_u[23]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[24]_i_1 
       (.I0(\dividend0_reg[31]_0 [24]),
        .I1(\dividend0_reg[31]_1 [24]),
        .O(D[24]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[24]_i_1__0 
       (.I0(dividend_u0[24]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[24] ),
        .O(dividend_u[24]));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[24]_i_3 
       (.I0(\dividend0_reg_n_1_[24] ),
        .O(\dividend0[24]_i_3_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[24]_i_4 
       (.I0(\dividend0_reg_n_1_[23] ),
        .O(\dividend0[24]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[24]_i_5 
       (.I0(\dividend0_reg_n_1_[22] ),
        .O(\dividend0[24]_i_5_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[24]_i_6 
       (.I0(\dividend0_reg_n_1_[21] ),
        .O(\dividend0[24]_i_6_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[25]_i_1 
       (.I0(\dividend0_reg[31]_0 [25]),
        .I1(\dividend0_reg[31]_1 [25]),
        .O(D[25]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[25]_i_1__0 
       (.I0(dividend_u0[25]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[25] ),
        .O(dividend_u[25]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[26]_i_1 
       (.I0(\dividend0_reg[31]_0 [26]),
        .I1(\dividend0_reg[31]_1 [26]),
        .O(D[26]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[26]_i_1__0 
       (.I0(dividend_u0[26]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[26] ),
        .O(dividend_u[26]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[27]_i_1 
       (.I0(\dividend0_reg[31]_0 [27]),
        .I1(\dividend0_reg[31]_1 [27]),
        .O(D[27]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[27]_i_1__0 
       (.I0(dividend_u0[27]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[27] ),
        .O(dividend_u[27]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[28]_i_1 
       (.I0(\dividend0_reg[31]_0 [28]),
        .I1(\dividend0_reg[31]_1 [28]),
        .O(D[28]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[28]_i_1__0 
       (.I0(dividend_u0[28]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[28] ),
        .O(dividend_u[28]));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[28]_i_3 
       (.I0(\dividend0_reg_n_1_[28] ),
        .O(\dividend0[28]_i_3_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[28]_i_4 
       (.I0(\dividend0_reg_n_1_[27] ),
        .O(\dividend0[28]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[28]_i_5 
       (.I0(\dividend0_reg_n_1_[26] ),
        .O(\dividend0[28]_i_5_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[28]_i_6 
       (.I0(\dividend0_reg_n_1_[25] ),
        .O(\dividend0[28]_i_6_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[29]_i_1 
       (.I0(\dividend0_reg[31]_0 [29]),
        .I1(\dividend0_reg[31]_1 [29]),
        .O(D[29]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[29]_i_1__0 
       (.I0(dividend_u0[29]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[29] ),
        .O(dividend_u[29]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[2]_i_1 
       (.I0(\dividend0_reg[31]_0 [2]),
        .I1(\dividend0_reg[31]_1 [2]),
        .O(D[2]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[2]_i_1__0 
       (.I0(dividend_u0[2]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[2] ),
        .O(dividend_u[2]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[30]_i_1 
       (.I0(\dividend0_reg[31]_0 [30]),
        .I1(\dividend0_reg[31]_1 [30]),
        .O(D[30]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[30]_i_1__0 
       (.I0(dividend_u0[30]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[30] ),
        .O(dividend_u[30]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[31]_i_1 
       (.I0(\dividend0_reg[31]_0 [31]),
        .I1(\dividend0_reg[31]_1 [31]),
        .O(D[31]));
  LUT2 #(
    .INIT(4'h8)) 
    \dividend0[31]_i_1__0 
       (.I0(p_1_in),
        .I1(dividend_u0[31]),
        .O(dividend_u[31]));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[31]_i_3 
       (.I0(p_1_in),
        .O(\dividend0[31]_i_3_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[31]_i_4 
       (.I0(\dividend0_reg_n_1_[30] ),
        .O(\dividend0[31]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[31]_i_5 
       (.I0(\dividend0_reg_n_1_[29] ),
        .O(\dividend0[31]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[3]_i_1 
       (.I0(\dividend0_reg[31]_0 [3]),
        .I1(\dividend0_reg[31]_1 [3]),
        .O(D[3]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[3]_i_1__0 
       (.I0(dividend_u0[3]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[3] ),
        .O(dividend_u[3]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[4]_i_1 
       (.I0(\dividend0_reg[31]_0 [4]),
        .I1(\dividend0_reg[31]_1 [4]),
        .O(D[4]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[4]_i_1__0 
       (.I0(dividend_u0[4]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[4] ),
        .O(dividend_u[4]));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[4]_i_3 
       (.I0(\dividend0_reg_n_1_[0] ),
        .O(\dividend0[4]_i_3_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[4]_i_4 
       (.I0(\dividend0_reg_n_1_[4] ),
        .O(\dividend0[4]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[4]_i_5 
       (.I0(\dividend0_reg_n_1_[3] ),
        .O(\dividend0[4]_i_5_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[4]_i_6 
       (.I0(\dividend0_reg_n_1_[2] ),
        .O(\dividend0[4]_i_6_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[4]_i_7 
       (.I0(\dividend0_reg_n_1_[1] ),
        .O(\dividend0[4]_i_7_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[5]_i_1 
       (.I0(\dividend0_reg[31]_0 [5]),
        .I1(\dividend0_reg[31]_1 [5]),
        .O(D[5]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[5]_i_1__0 
       (.I0(dividend_u0[5]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[5] ),
        .O(dividend_u[5]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[6]_i_1 
       (.I0(\dividend0_reg[31]_0 [6]),
        .I1(\dividend0_reg[31]_1 [6]),
        .O(D[6]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[6]_i_1__0 
       (.I0(dividend_u0[6]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[6] ),
        .O(dividend_u[6]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[7]_i_1 
       (.I0(\dividend0_reg[31]_0 [7]),
        .I1(\dividend0_reg[31]_1 [7]),
        .O(D[7]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[7]_i_1__0 
       (.I0(dividend_u0[7]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[7] ),
        .O(dividend_u[7]));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[8]_i_1 
       (.I0(\dividend0_reg[31]_0 [8]),
        .I1(\dividend0_reg[31]_1 [8]),
        .O(D[8]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[8]_i_1__0 
       (.I0(dividend_u0[8]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[8] ),
        .O(dividend_u[8]));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[8]_i_3 
       (.I0(\dividend0_reg_n_1_[8] ),
        .O(\dividend0[8]_i_3_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[8]_i_4 
       (.I0(\dividend0_reg_n_1_[7] ),
        .O(\dividend0[8]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[8]_i_5 
       (.I0(\dividend0_reg_n_1_[6] ),
        .O(\dividend0[8]_i_5_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \dividend0[8]_i_6 
       (.I0(\dividend0_reg_n_1_[5] ),
        .O(\dividend0[8]_i_6_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \dividend0[9]_i_1 
       (.I0(\dividend0_reg[31]_0 [9]),
        .I1(\dividend0_reg[31]_1 [9]),
        .O(D[9]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dividend0[9]_i_1__0 
       (.I0(dividend_u0[9]),
        .I1(p_1_in),
        .I2(\dividend0_reg_n_1_[9] ),
        .O(dividend_u[9]));
  FDRE \dividend0_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[0]),
        .Q(\dividend0_reg_n_1_[0] ),
        .R(1'b0));
  FDRE \dividend0_reg[10] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[10]),
        .Q(\dividend0_reg_n_1_[10] ),
        .R(1'b0));
  FDRE \dividend0_reg[11] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[11]),
        .Q(\dividend0_reg_n_1_[11] ),
        .R(1'b0));
  FDRE \dividend0_reg[12] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[12]),
        .Q(\dividend0_reg_n_1_[12] ),
        .R(1'b0));
  CARRY4 \dividend0_reg[12]_i_2 
       (.CI(\dividend0_reg[8]_i_2_n_1 ),
        .CO({\dividend0_reg[12]_i_2_n_1 ,\dividend0_reg[12]_i_2_n_2 ,\dividend0_reg[12]_i_2_n_3 ,\dividend0_reg[12]_i_2_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(dividend_u0[12:9]),
        .S({\dividend0[12]_i_3_n_1 ,\dividend0[12]_i_4_n_1 ,\dividend0[12]_i_5_n_1 ,\dividend0[12]_i_6_n_1 }));
  FDRE \dividend0_reg[13] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[13]),
        .Q(\dividend0_reg_n_1_[13] ),
        .R(1'b0));
  FDRE \dividend0_reg[14] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[14]),
        .Q(\dividend0_reg_n_1_[14] ),
        .R(1'b0));
  FDRE \dividend0_reg[15] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[15]),
        .Q(\dividend0_reg_n_1_[15] ),
        .R(1'b0));
  FDRE \dividend0_reg[16] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[16]),
        .Q(\dividend0_reg_n_1_[16] ),
        .R(1'b0));
  CARRY4 \dividend0_reg[16]_i_2 
       (.CI(\dividend0_reg[12]_i_2_n_1 ),
        .CO({\dividend0_reg[16]_i_2_n_1 ,\dividend0_reg[16]_i_2_n_2 ,\dividend0_reg[16]_i_2_n_3 ,\dividend0_reg[16]_i_2_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(dividend_u0[16:13]),
        .S({\dividend0[16]_i_3_n_1 ,\dividend0[16]_i_4_n_1 ,\dividend0[16]_i_5_n_1 ,\dividend0[16]_i_6_n_1 }));
  FDRE \dividend0_reg[17] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[17]),
        .Q(\dividend0_reg_n_1_[17] ),
        .R(1'b0));
  FDRE \dividend0_reg[18] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[18]),
        .Q(\dividend0_reg_n_1_[18] ),
        .R(1'b0));
  FDRE \dividend0_reg[19] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[19]),
        .Q(\dividend0_reg_n_1_[19] ),
        .R(1'b0));
  FDRE \dividend0_reg[1] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[1]),
        .Q(\dividend0_reg_n_1_[1] ),
        .R(1'b0));
  FDRE \dividend0_reg[20] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[20]),
        .Q(\dividend0_reg_n_1_[20] ),
        .R(1'b0));
  CARRY4 \dividend0_reg[20]_i_2 
       (.CI(\dividend0_reg[16]_i_2_n_1 ),
        .CO({\dividend0_reg[20]_i_2_n_1 ,\dividend0_reg[20]_i_2_n_2 ,\dividend0_reg[20]_i_2_n_3 ,\dividend0_reg[20]_i_2_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(dividend_u0[20:17]),
        .S({\dividend0[20]_i_3_n_1 ,\dividend0[20]_i_4_n_1 ,\dividend0[20]_i_5_n_1 ,\dividend0[20]_i_6_n_1 }));
  FDRE \dividend0_reg[21] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[21]),
        .Q(\dividend0_reg_n_1_[21] ),
        .R(1'b0));
  FDRE \dividend0_reg[22] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[22]),
        .Q(\dividend0_reg_n_1_[22] ),
        .R(1'b0));
  FDRE \dividend0_reg[23] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[23]),
        .Q(\dividend0_reg_n_1_[23] ),
        .R(1'b0));
  FDRE \dividend0_reg[24] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[24]),
        .Q(\dividend0_reg_n_1_[24] ),
        .R(1'b0));
  CARRY4 \dividend0_reg[24]_i_2 
       (.CI(\dividend0_reg[20]_i_2_n_1 ),
        .CO({\dividend0_reg[24]_i_2_n_1 ,\dividend0_reg[24]_i_2_n_2 ,\dividend0_reg[24]_i_2_n_3 ,\dividend0_reg[24]_i_2_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(dividend_u0[24:21]),
        .S({\dividend0[24]_i_3_n_1 ,\dividend0[24]_i_4_n_1 ,\dividend0[24]_i_5_n_1 ,\dividend0[24]_i_6_n_1 }));
  FDRE \dividend0_reg[25] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[25]),
        .Q(\dividend0_reg_n_1_[25] ),
        .R(1'b0));
  FDRE \dividend0_reg[26] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[26]),
        .Q(\dividend0_reg_n_1_[26] ),
        .R(1'b0));
  FDRE \dividend0_reg[27] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[27]),
        .Q(\dividend0_reg_n_1_[27] ),
        .R(1'b0));
  FDRE \dividend0_reg[28] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[28]),
        .Q(\dividend0_reg_n_1_[28] ),
        .R(1'b0));
  CARRY4 \dividend0_reg[28]_i_2 
       (.CI(\dividend0_reg[24]_i_2_n_1 ),
        .CO({\dividend0_reg[28]_i_2_n_1 ,\dividend0_reg[28]_i_2_n_2 ,\dividend0_reg[28]_i_2_n_3 ,\dividend0_reg[28]_i_2_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(dividend_u0[28:25]),
        .S({\dividend0[28]_i_3_n_1 ,\dividend0[28]_i_4_n_1 ,\dividend0[28]_i_5_n_1 ,\dividend0[28]_i_6_n_1 }));
  FDRE \dividend0_reg[29] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[29]),
        .Q(\dividend0_reg_n_1_[29] ),
        .R(1'b0));
  FDRE \dividend0_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[2]),
        .Q(\dividend0_reg_n_1_[2] ),
        .R(1'b0));
  FDRE \dividend0_reg[30] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[30]),
        .Q(\dividend0_reg_n_1_[30] ),
        .R(1'b0));
  FDRE \dividend0_reg[31] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[31]),
        .Q(p_1_in),
        .R(1'b0));
  CARRY4 \dividend0_reg[31]_i_2 
       (.CI(\dividend0_reg[28]_i_2_n_1 ),
        .CO({\NLW_dividend0_reg[31]_i_2_CO_UNCONNECTED [3:2],\dividend0_reg[31]_i_2_n_3 ,\dividend0_reg[31]_i_2_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({\NLW_dividend0_reg[31]_i_2_O_UNCONNECTED [3],dividend_u0[31:29]}),
        .S({1'b0,\dividend0[31]_i_3_n_1 ,\dividend0[31]_i_4_n_1 ,\dividend0[31]_i_5_n_1 }));
  FDRE \dividend0_reg[3] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[3]),
        .Q(\dividend0_reg_n_1_[3] ),
        .R(1'b0));
  FDRE \dividend0_reg[4] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[4]),
        .Q(\dividend0_reg_n_1_[4] ),
        .R(1'b0));
  CARRY4 \dividend0_reg[4]_i_2 
       (.CI(1'b0),
        .CO({\dividend0_reg[4]_i_2_n_1 ,\dividend0_reg[4]_i_2_n_2 ,\dividend0_reg[4]_i_2_n_3 ,\dividend0_reg[4]_i_2_n_4 }),
        .CYINIT(\dividend0[4]_i_3_n_1 ),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(dividend_u0[4:1]),
        .S({\dividend0[4]_i_4_n_1 ,\dividend0[4]_i_5_n_1 ,\dividend0[4]_i_6_n_1 ,\dividend0[4]_i_7_n_1 }));
  FDRE \dividend0_reg[5] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[5]),
        .Q(\dividend0_reg_n_1_[5] ),
        .R(1'b0));
  FDRE \dividend0_reg[6] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[6]),
        .Q(\dividend0_reg_n_1_[6] ),
        .R(1'b0));
  FDRE \dividend0_reg[7] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[7]),
        .Q(\dividend0_reg_n_1_[7] ),
        .R(1'b0));
  FDRE \dividend0_reg[8] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[8]),
        .Q(\dividend0_reg_n_1_[8] ),
        .R(1'b0));
  CARRY4 \dividend0_reg[8]_i_2 
       (.CI(\dividend0_reg[4]_i_2_n_1 ),
        .CO({\dividend0_reg[8]_i_2_n_1 ,\dividend0_reg[8]_i_2_n_2 ,\dividend0_reg[8]_i_2_n_3 ,\dividend0_reg[8]_i_2_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(dividend_u0[8:5]),
        .S({\dividend0[8]_i_3_n_1 ,\dividend0[8]_i_4_n_1 ,\dividend0[8]_i_5_n_1 ,\dividend0[8]_i_6_n_1 }));
  FDRE \dividend0_reg[9] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(D[9]),
        .Q(\dividend0_reg_n_1_[9] ),
        .R(1'b0));
  bd_0_hls_inst_0_loop_imperfect_srbkb_div_u_2 loop_imperfect_srbkb_div_u_0
       (.D(dividend_u),
        .E(start0),
        .O7({loop_imperfect_srbkb_div_u_0_n_3,loop_imperfect_srbkb_div_u_0_n_4,loop_imperfect_srbkb_div_u_0_n_5,loop_imperfect_srbkb_div_u_0_n_6,loop_imperfect_srbkb_div_u_0_n_7,loop_imperfect_srbkb_div_u_0_n_8,loop_imperfect_srbkb_div_u_0_n_9,loop_imperfect_srbkb_div_u_0_n_10,loop_imperfect_srbkb_div_u_0_n_11,loop_imperfect_srbkb_div_u_0_n_12,loop_imperfect_srbkb_div_u_0_n_13,loop_imperfect_srbkb_div_u_0_n_14,loop_imperfect_srbkb_div_u_0_n_15,loop_imperfect_srbkb_div_u_0_n_16,loop_imperfect_srbkb_div_u_0_n_17,loop_imperfect_srbkb_div_u_0_n_18,loop_imperfect_srbkb_div_u_0_n_19,loop_imperfect_srbkb_div_u_0_n_20,loop_imperfect_srbkb_div_u_0_n_21,loop_imperfect_srbkb_div_u_0_n_22,loop_imperfect_srbkb_div_u_0_n_23,loop_imperfect_srbkb_div_u_0_n_24,loop_imperfect_srbkb_div_u_0_n_25,loop_imperfect_srbkb_div_u_0_n_26,loop_imperfect_srbkb_div_u_0_n_27,loop_imperfect_srbkb_div_u_0_n_28,loop_imperfect_srbkb_div_u_0_n_29,loop_imperfect_srbkb_div_u_0_n_30,loop_imperfect_srbkb_div_u_0_n_31,loop_imperfect_srbkb_div_u_0_n_32,loop_imperfect_srbkb_div_u_0_n_33,loop_imperfect_srbkb_div_u_0_n_34}),
        .Q({p_1_in,\dividend0_reg_n_1_[0] }),
        .ap_clk(ap_clk),
        .ap_rst(ap_rst),
        .\r_stage_reg[32]_0 (done0),
        .r_stage_reg_r_29_0(r_stage_reg_r_29));
  FDRE \remd_reg[0] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_34),
        .Q(\remd_reg[31]_0 [0]),
        .R(1'b0));
  FDRE \remd_reg[10] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_24),
        .Q(\remd_reg[31]_0 [10]),
        .R(1'b0));
  FDRE \remd_reg[11] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_23),
        .Q(\remd_reg[31]_0 [11]),
        .R(1'b0));
  FDRE \remd_reg[12] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_22),
        .Q(\remd_reg[31]_0 [12]),
        .R(1'b0));
  FDRE \remd_reg[13] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_21),
        .Q(\remd_reg[31]_0 [13]),
        .R(1'b0));
  FDRE \remd_reg[14] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_20),
        .Q(\remd_reg[31]_0 [14]),
        .R(1'b0));
  FDRE \remd_reg[15] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_19),
        .Q(\remd_reg[31]_0 [15]),
        .R(1'b0));
  FDRE \remd_reg[16] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_18),
        .Q(\remd_reg[31]_0 [16]),
        .R(1'b0));
  FDRE \remd_reg[17] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_17),
        .Q(\remd_reg[31]_0 [17]),
        .R(1'b0));
  FDRE \remd_reg[18] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_16),
        .Q(\remd_reg[31]_0 [18]),
        .R(1'b0));
  FDRE \remd_reg[19] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_15),
        .Q(\remd_reg[31]_0 [19]),
        .R(1'b0));
  FDRE \remd_reg[1] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_33),
        .Q(\remd_reg[31]_0 [1]),
        .R(1'b0));
  FDRE \remd_reg[20] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_14),
        .Q(\remd_reg[31]_0 [20]),
        .R(1'b0));
  FDRE \remd_reg[21] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_13),
        .Q(\remd_reg[31]_0 [21]),
        .R(1'b0));
  FDRE \remd_reg[22] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_12),
        .Q(\remd_reg[31]_0 [22]),
        .R(1'b0));
  FDRE \remd_reg[23] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_11),
        .Q(\remd_reg[31]_0 [23]),
        .R(1'b0));
  FDRE \remd_reg[24] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_10),
        .Q(\remd_reg[31]_0 [24]),
        .R(1'b0));
  FDRE \remd_reg[25] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_9),
        .Q(\remd_reg[31]_0 [25]),
        .R(1'b0));
  FDRE \remd_reg[26] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_8),
        .Q(\remd_reg[31]_0 [26]),
        .R(1'b0));
  FDRE \remd_reg[27] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_7),
        .Q(\remd_reg[31]_0 [27]),
        .R(1'b0));
  FDRE \remd_reg[28] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_6),
        .Q(\remd_reg[31]_0 [28]),
        .R(1'b0));
  FDRE \remd_reg[29] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_5),
        .Q(\remd_reg[31]_0 [29]),
        .R(1'b0));
  FDRE \remd_reg[2] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_32),
        .Q(\remd_reg[31]_0 [2]),
        .R(1'b0));
  FDRE \remd_reg[30] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_4),
        .Q(\remd_reg[31]_0 [30]),
        .R(1'b0));
  FDRE \remd_reg[31] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_3),
        .Q(\remd_reg[31]_0 [31]),
        .R(1'b0));
  FDRE \remd_reg[3] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_31),
        .Q(\remd_reg[31]_0 [3]),
        .R(1'b0));
  FDRE \remd_reg[4] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_30),
        .Q(\remd_reg[31]_0 [4]),
        .R(1'b0));
  FDRE \remd_reg[5] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_29),
        .Q(\remd_reg[31]_0 [5]),
        .R(1'b0));
  FDRE \remd_reg[6] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_28),
        .Q(\remd_reg[31]_0 [6]),
        .R(1'b0));
  FDRE \remd_reg[7] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_27),
        .Q(\remd_reg[31]_0 [7]),
        .R(1'b0));
  FDRE \remd_reg[8] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_26),
        .Q(\remd_reg[31]_0 [8]),
        .R(1'b0));
  FDRE \remd_reg[9] 
       (.C(ap_clk),
        .CE(done0),
        .D(loop_imperfect_srbkb_div_u_0_n_25),
        .Q(\remd_reg[31]_0 [9]),
        .R(1'b0));
  FDRE #(
    .INIT(1'b0)) 
    start0_reg
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q),
        .Q(start0),
        .R(1'b0));
endmodule

(* ORIG_REF_NAME = "loop_imperfect_srbkb_div_u" *) 
module bd_0_hls_inst_0_loop_imperfect_srbkb_div_u
   (\r_stage_reg[32]_0 ,
    O9,
    ap_rst,
    E,
    ap_clk,
    Q,
    \r_stage_reg[32]_1 ,
    D);
  output [0:0]\r_stage_reg[32]_0 ;
  output [31:0]O9;
  input ap_rst;
  input [0:0]E;
  input ap_clk;
  input [1:0]Q;
  input \r_stage_reg[32]_1 ;
  input [30:0]D;

  wire [30:0]D;
  wire [0:0]E;
  wire [31:0]O9;
  wire [1:0]Q;
  wire ap_clk;
  wire ap_rst;
  wire cal_tmp_carry__0_i_1__0_n_1;
  wire cal_tmp_carry__0_i_2__0_n_1;
  wire cal_tmp_carry__0_i_3__0_n_1;
  wire cal_tmp_carry__0_n_1;
  wire cal_tmp_carry__0_n_2;
  wire cal_tmp_carry__0_n_3;
  wire cal_tmp_carry__0_n_4;
  wire cal_tmp_carry__0_n_5;
  wire cal_tmp_carry__0_n_6;
  wire cal_tmp_carry__0_n_7;
  wire cal_tmp_carry__0_n_8;
  wire cal_tmp_carry__1_i_1__0_n_1;
  wire cal_tmp_carry__1_i_2__0_n_1;
  wire cal_tmp_carry__1_i_3__0_n_1;
  wire cal_tmp_carry__1_i_4__0_n_1;
  wire cal_tmp_carry__1_n_1;
  wire cal_tmp_carry__1_n_2;
  wire cal_tmp_carry__1_n_3;
  wire cal_tmp_carry__1_n_4;
  wire cal_tmp_carry__1_n_5;
  wire cal_tmp_carry__1_n_6;
  wire cal_tmp_carry__1_n_7;
  wire cal_tmp_carry__1_n_8;
  wire cal_tmp_carry__2_i_1__0_n_1;
  wire cal_tmp_carry__2_i_2__0_n_1;
  wire cal_tmp_carry__2_i_3__0_n_1;
  wire cal_tmp_carry__2_i_4__0_n_1;
  wire cal_tmp_carry__2_n_1;
  wire cal_tmp_carry__2_n_2;
  wire cal_tmp_carry__2_n_3;
  wire cal_tmp_carry__2_n_4;
  wire cal_tmp_carry__2_n_5;
  wire cal_tmp_carry__2_n_6;
  wire cal_tmp_carry__2_n_7;
  wire cal_tmp_carry__2_n_8;
  wire cal_tmp_carry__3_i_1__0_n_1;
  wire cal_tmp_carry__3_i_2__0_n_1;
  wire cal_tmp_carry__3_i_3__0_n_1;
  wire cal_tmp_carry__3_i_4__0_n_1;
  wire cal_tmp_carry__3_n_1;
  wire cal_tmp_carry__3_n_2;
  wire cal_tmp_carry__3_n_3;
  wire cal_tmp_carry__3_n_4;
  wire cal_tmp_carry__3_n_5;
  wire cal_tmp_carry__3_n_6;
  wire cal_tmp_carry__3_n_7;
  wire cal_tmp_carry__3_n_8;
  wire cal_tmp_carry__4_i_1__0_n_1;
  wire cal_tmp_carry__4_i_2__0_n_1;
  wire cal_tmp_carry__4_i_3__0_n_1;
  wire cal_tmp_carry__4_i_4__0_n_1;
  wire cal_tmp_carry__4_n_1;
  wire cal_tmp_carry__4_n_2;
  wire cal_tmp_carry__4_n_3;
  wire cal_tmp_carry__4_n_4;
  wire cal_tmp_carry__4_n_5;
  wire cal_tmp_carry__4_n_6;
  wire cal_tmp_carry__4_n_7;
  wire cal_tmp_carry__4_n_8;
  wire cal_tmp_carry__5_i_1__0_n_1;
  wire cal_tmp_carry__5_i_2__0_n_1;
  wire cal_tmp_carry__5_i_3__0_n_1;
  wire cal_tmp_carry__5_i_4__0_n_1;
  wire cal_tmp_carry__5_n_1;
  wire cal_tmp_carry__5_n_2;
  wire cal_tmp_carry__5_n_3;
  wire cal_tmp_carry__5_n_4;
  wire cal_tmp_carry__5_n_5;
  wire cal_tmp_carry__5_n_6;
  wire cal_tmp_carry__5_n_7;
  wire cal_tmp_carry__5_n_8;
  wire cal_tmp_carry__6_i_1__0_n_1;
  wire cal_tmp_carry__6_i_2__0_n_1;
  wire cal_tmp_carry__6_i_3__0_n_1;
  wire cal_tmp_carry__6_i_4__0_n_1;
  wire cal_tmp_carry__6_n_2;
  wire cal_tmp_carry__6_n_3;
  wire cal_tmp_carry__6_n_4;
  wire cal_tmp_carry__6_n_5;
  wire cal_tmp_carry__6_n_6;
  wire cal_tmp_carry__6_n_7;
  wire cal_tmp_carry__6_n_8;
  wire cal_tmp_carry_i_1_n_1;
  wire cal_tmp_carry_i_2__0_n_1;
  wire cal_tmp_carry_i_3__0_n_1;
  wire cal_tmp_carry_i_4__0_n_1;
  wire cal_tmp_carry_i_5_n_1;
  wire cal_tmp_carry_n_1;
  wire cal_tmp_carry_n_2;
  wire cal_tmp_carry_n_3;
  wire cal_tmp_carry_n_4;
  wire cal_tmp_carry_n_5;
  wire cal_tmp_carry_n_6;
  wire cal_tmp_carry_n_7;
  wire cal_tmp_carry_n_8;
  wire \dividend0_reg_n_1_[0] ;
  wire \dividend0_reg_n_1_[10] ;
  wire \dividend0_reg_n_1_[11] ;
  wire \dividend0_reg_n_1_[12] ;
  wire \dividend0_reg_n_1_[13] ;
  wire \dividend0_reg_n_1_[14] ;
  wire \dividend0_reg_n_1_[15] ;
  wire \dividend0_reg_n_1_[16] ;
  wire \dividend0_reg_n_1_[17] ;
  wire \dividend0_reg_n_1_[18] ;
  wire \dividend0_reg_n_1_[19] ;
  wire \dividend0_reg_n_1_[1] ;
  wire \dividend0_reg_n_1_[20] ;
  wire \dividend0_reg_n_1_[21] ;
  wire \dividend0_reg_n_1_[22] ;
  wire \dividend0_reg_n_1_[23] ;
  wire \dividend0_reg_n_1_[24] ;
  wire \dividend0_reg_n_1_[25] ;
  wire \dividend0_reg_n_1_[26] ;
  wire \dividend0_reg_n_1_[27] ;
  wire \dividend0_reg_n_1_[28] ;
  wire \dividend0_reg_n_1_[29] ;
  wire \dividend0_reg_n_1_[2] ;
  wire \dividend0_reg_n_1_[30] ;
  wire \dividend0_reg_n_1_[31] ;
  wire \dividend0_reg_n_1_[3] ;
  wire \dividend0_reg_n_1_[4] ;
  wire \dividend0_reg_n_1_[5] ;
  wire \dividend0_reg_n_1_[6] ;
  wire \dividend0_reg_n_1_[7] ;
  wire \dividend0_reg_n_1_[8] ;
  wire \dividend0_reg_n_1_[9] ;
  wire \dividend_tmp[10]_i_1_n_1 ;
  wire \dividend_tmp[11]_i_1_n_1 ;
  wire \dividend_tmp[12]_i_1_n_1 ;
  wire \dividend_tmp[13]_i_1_n_1 ;
  wire \dividend_tmp[14]_i_1_n_1 ;
  wire \dividend_tmp[15]_i_1_n_1 ;
  wire \dividend_tmp[16]_i_1_n_1 ;
  wire \dividend_tmp[17]_i_1_n_1 ;
  wire \dividend_tmp[18]_i_1_n_1 ;
  wire \dividend_tmp[19]_i_1_n_1 ;
  wire \dividend_tmp[1]_i_1_n_1 ;
  wire \dividend_tmp[20]_i_1_n_1 ;
  wire \dividend_tmp[21]_i_1_n_1 ;
  wire \dividend_tmp[22]_i_1_n_1 ;
  wire \dividend_tmp[23]_i_1_n_1 ;
  wire \dividend_tmp[24]_i_1_n_1 ;
  wire \dividend_tmp[25]_i_1_n_1 ;
  wire \dividend_tmp[26]_i_1_n_1 ;
  wire \dividend_tmp[27]_i_1_n_1 ;
  wire \dividend_tmp[28]_i_1_n_1 ;
  wire \dividend_tmp[29]_i_1_n_1 ;
  wire \dividend_tmp[2]_i_1_n_1 ;
  wire \dividend_tmp[30]_i_1_n_1 ;
  wire \dividend_tmp[31]_i_1_n_1 ;
  wire \dividend_tmp[3]_i_1_n_1 ;
  wire \dividend_tmp[4]_i_1_n_1 ;
  wire \dividend_tmp[5]_i_1_n_1 ;
  wire \dividend_tmp[6]_i_1_n_1 ;
  wire \dividend_tmp[7]_i_1_n_1 ;
  wire \dividend_tmp[8]_i_1_n_1 ;
  wire \dividend_tmp[9]_i_1_n_1 ;
  wire \dividend_tmp_reg_n_1_[0] ;
  wire \dividend_tmp_reg_n_1_[10] ;
  wire \dividend_tmp_reg_n_1_[11] ;
  wire \dividend_tmp_reg_n_1_[12] ;
  wire \dividend_tmp_reg_n_1_[13] ;
  wire \dividend_tmp_reg_n_1_[14] ;
  wire \dividend_tmp_reg_n_1_[15] ;
  wire \dividend_tmp_reg_n_1_[16] ;
  wire \dividend_tmp_reg_n_1_[17] ;
  wire \dividend_tmp_reg_n_1_[18] ;
  wire \dividend_tmp_reg_n_1_[19] ;
  wire \dividend_tmp_reg_n_1_[1] ;
  wire \dividend_tmp_reg_n_1_[20] ;
  wire \dividend_tmp_reg_n_1_[21] ;
  wire \dividend_tmp_reg_n_1_[22] ;
  wire \dividend_tmp_reg_n_1_[23] ;
  wire \dividend_tmp_reg_n_1_[24] ;
  wire \dividend_tmp_reg_n_1_[25] ;
  wire \dividend_tmp_reg_n_1_[26] ;
  wire \dividend_tmp_reg_n_1_[27] ;
  wire \dividend_tmp_reg_n_1_[28] ;
  wire \dividend_tmp_reg_n_1_[29] ;
  wire \dividend_tmp_reg_n_1_[2] ;
  wire \dividend_tmp_reg_n_1_[30] ;
  wire \dividend_tmp_reg_n_1_[31] ;
  wire \dividend_tmp_reg_n_1_[3] ;
  wire \dividend_tmp_reg_n_1_[4] ;
  wire \dividend_tmp_reg_n_1_[5] ;
  wire \dividend_tmp_reg_n_1_[6] ;
  wire \dividend_tmp_reg_n_1_[7] ;
  wire \dividend_tmp_reg_n_1_[8] ;
  wire \dividend_tmp_reg_n_1_[9] ;
  wire p_0_in;
  wire [0:0]p_2_out;
  wire \r_stage_reg[30]_srl30___loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_28_n_1 ;
  wire \r_stage_reg[31]_loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_29_n_1 ;
  wire [0:0]\r_stage_reg[32]_0 ;
  wire \r_stage_reg[32]_1 ;
  wire r_stage_reg_gate_n_1;
  wire \r_stage_reg_n_1_[0] ;
  wire \remd[11]_i_2__0_n_1 ;
  wire \remd[11]_i_3__0_n_1 ;
  wire \remd[11]_i_4__0_n_1 ;
  wire \remd[11]_i_5__0_n_1 ;
  wire \remd[15]_i_2__0_n_1 ;
  wire \remd[15]_i_3__0_n_1 ;
  wire \remd[15]_i_4__0_n_1 ;
  wire \remd[15]_i_5__0_n_1 ;
  wire \remd[19]_i_2__0_n_1 ;
  wire \remd[19]_i_3__0_n_1 ;
  wire \remd[19]_i_4__0_n_1 ;
  wire \remd[19]_i_5__0_n_1 ;
  wire \remd[23]_i_2__0_n_1 ;
  wire \remd[23]_i_3__0_n_1 ;
  wire \remd[23]_i_4__0_n_1 ;
  wire \remd[23]_i_5__0_n_1 ;
  wire \remd[27]_i_2__0_n_1 ;
  wire \remd[27]_i_3__0_n_1 ;
  wire \remd[27]_i_4__0_n_1 ;
  wire \remd[27]_i_5__0_n_1 ;
  wire \remd[31]_i_2__0_n_1 ;
  wire \remd[31]_i_3__0_n_1 ;
  wire \remd[31]_i_4__0_n_1 ;
  wire \remd[31]_i_5__0_n_1 ;
  wire \remd[3]_i_2__0_n_1 ;
  wire \remd[3]_i_3__0_n_1 ;
  wire \remd[3]_i_4__0_n_1 ;
  wire \remd[3]_i_5__0_n_1 ;
  wire \remd[7]_i_2__0_n_1 ;
  wire \remd[7]_i_3__0_n_1 ;
  wire \remd[7]_i_4__0_n_1 ;
  wire \remd[7]_i_5__0_n_1 ;
  wire \remd_reg[11]_i_1__0_n_1 ;
  wire \remd_reg[11]_i_1__0_n_2 ;
  wire \remd_reg[11]_i_1__0_n_3 ;
  wire \remd_reg[11]_i_1__0_n_4 ;
  wire \remd_reg[15]_i_1__0_n_1 ;
  wire \remd_reg[15]_i_1__0_n_2 ;
  wire \remd_reg[15]_i_1__0_n_3 ;
  wire \remd_reg[15]_i_1__0_n_4 ;
  wire \remd_reg[19]_i_1__0_n_1 ;
  wire \remd_reg[19]_i_1__0_n_2 ;
  wire \remd_reg[19]_i_1__0_n_3 ;
  wire \remd_reg[19]_i_1__0_n_4 ;
  wire \remd_reg[23]_i_1__0_n_1 ;
  wire \remd_reg[23]_i_1__0_n_2 ;
  wire \remd_reg[23]_i_1__0_n_3 ;
  wire \remd_reg[23]_i_1__0_n_4 ;
  wire \remd_reg[27]_i_1__0_n_1 ;
  wire \remd_reg[27]_i_1__0_n_2 ;
  wire \remd_reg[27]_i_1__0_n_3 ;
  wire \remd_reg[27]_i_1__0_n_4 ;
  wire \remd_reg[31]_i_1__0_n_2 ;
  wire \remd_reg[31]_i_1__0_n_3 ;
  wire \remd_reg[31]_i_1__0_n_4 ;
  wire \remd_reg[3]_i_1__0_n_1 ;
  wire \remd_reg[3]_i_1__0_n_2 ;
  wire \remd_reg[3]_i_1__0_n_3 ;
  wire \remd_reg[3]_i_1__0_n_4 ;
  wire \remd_reg[7]_i_1__0_n_1 ;
  wire \remd_reg[7]_i_1__0_n_2 ;
  wire \remd_reg[7]_i_1__0_n_3 ;
  wire \remd_reg[7]_i_1__0_n_4 ;
  wire \remd_tmp[0]_i_1_n_1 ;
  wire \remd_tmp[10]_i_1_n_1 ;
  wire \remd_tmp[11]_i_1_n_1 ;
  wire \remd_tmp[12]_i_1_n_1 ;
  wire \remd_tmp[13]_i_1_n_1 ;
  wire \remd_tmp[14]_i_1_n_1 ;
  wire \remd_tmp[15]_i_1_n_1 ;
  wire \remd_tmp[16]_i_1_n_1 ;
  wire \remd_tmp[17]_i_1_n_1 ;
  wire \remd_tmp[18]_i_1_n_1 ;
  wire \remd_tmp[19]_i_1_n_1 ;
  wire \remd_tmp[1]_i_1_n_1 ;
  wire \remd_tmp[20]_i_1_n_1 ;
  wire \remd_tmp[21]_i_1_n_1 ;
  wire \remd_tmp[22]_i_1_n_1 ;
  wire \remd_tmp[23]_i_1_n_1 ;
  wire \remd_tmp[24]_i_1_n_1 ;
  wire \remd_tmp[25]_i_1_n_1 ;
  wire \remd_tmp[26]_i_1_n_1 ;
  wire \remd_tmp[27]_i_1_n_1 ;
  wire \remd_tmp[28]_i_1_n_1 ;
  wire \remd_tmp[29]_i_1_n_1 ;
  wire \remd_tmp[2]_i_1_n_1 ;
  wire \remd_tmp[30]_i_1_n_1 ;
  wire \remd_tmp[31]_i_1_n_1 ;
  wire \remd_tmp[3]_i_1_n_1 ;
  wire \remd_tmp[4]_i_1_n_1 ;
  wire \remd_tmp[5]_i_1_n_1 ;
  wire \remd_tmp[6]_i_1_n_1 ;
  wire \remd_tmp[7]_i_1_n_1 ;
  wire \remd_tmp[8]_i_1_n_1 ;
  wire \remd_tmp[9]_i_1_n_1 ;
  wire remd_tmp_mux_n_1;
  wire remd_tmp_mux_rep_n_1;
  wire \remd_tmp_reg_n_1_[0] ;
  wire \remd_tmp_reg_n_1_[10] ;
  wire \remd_tmp_reg_n_1_[11] ;
  wire \remd_tmp_reg_n_1_[12] ;
  wire \remd_tmp_reg_n_1_[13] ;
  wire \remd_tmp_reg_n_1_[14] ;
  wire \remd_tmp_reg_n_1_[15] ;
  wire \remd_tmp_reg_n_1_[16] ;
  wire \remd_tmp_reg_n_1_[17] ;
  wire \remd_tmp_reg_n_1_[18] ;
  wire \remd_tmp_reg_n_1_[19] ;
  wire \remd_tmp_reg_n_1_[1] ;
  wire \remd_tmp_reg_n_1_[20] ;
  wire \remd_tmp_reg_n_1_[21] ;
  wire \remd_tmp_reg_n_1_[22] ;
  wire \remd_tmp_reg_n_1_[23] ;
  wire \remd_tmp_reg_n_1_[24] ;
  wire \remd_tmp_reg_n_1_[25] ;
  wire \remd_tmp_reg_n_1_[26] ;
  wire \remd_tmp_reg_n_1_[27] ;
  wire \remd_tmp_reg_n_1_[28] ;
  wire \remd_tmp_reg_n_1_[29] ;
  wire \remd_tmp_reg_n_1_[2] ;
  wire \remd_tmp_reg_n_1_[30] ;
  wire \remd_tmp_reg_n_1_[31] ;
  wire \remd_tmp_reg_n_1_[3] ;
  wire \remd_tmp_reg_n_1_[4] ;
  wire \remd_tmp_reg_n_1_[5] ;
  wire \remd_tmp_reg_n_1_[6] ;
  wire \remd_tmp_reg_n_1_[7] ;
  wire \remd_tmp_reg_n_1_[8] ;
  wire \remd_tmp_reg_n_1_[9] ;
  wire \sign0_reg_n_1_[0] ;
  wire [3:0]NLW_cal_tmp_carry__7_CO_UNCONNECTED;
  wire [3:1]NLW_cal_tmp_carry__7_O_UNCONNECTED;
  wire \NLW_r_stage_reg[30]_srl30___loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_28_Q31_UNCONNECTED ;
  wire [3:3]\NLW_remd_reg[31]_i_1__0_CO_UNCONNECTED ;

  CARRY4 cal_tmp_carry
       (.CI(1'b0),
        .CO({cal_tmp_carry_n_1,cal_tmp_carry_n_2,cal_tmp_carry_n_3,cal_tmp_carry_n_4}),
        .CYINIT(1'b1),
        .DI({1'b1,1'b1,1'b1,cal_tmp_carry_i_1_n_1}),
        .O({cal_tmp_carry_n_5,cal_tmp_carry_n_6,cal_tmp_carry_n_7,cal_tmp_carry_n_8}),
        .S({cal_tmp_carry_i_2__0_n_1,cal_tmp_carry_i_3__0_n_1,cal_tmp_carry_i_4__0_n_1,cal_tmp_carry_i_5_n_1}));
  CARRY4 cal_tmp_carry__0
       (.CI(cal_tmp_carry_n_1),
        .CO({cal_tmp_carry__0_n_1,cal_tmp_carry__0_n_2,cal_tmp_carry__0_n_3,cal_tmp_carry__0_n_4}),
        .CYINIT(1'b0),
        .DI({1'b1,1'b1,1'b1,remd_tmp_mux_n_1}),
        .O({cal_tmp_carry__0_n_5,cal_tmp_carry__0_n_6,cal_tmp_carry__0_n_7,cal_tmp_carry__0_n_8}),
        .S({cal_tmp_carry__0_i_1__0_n_1,cal_tmp_carry__0_i_2__0_n_1,cal_tmp_carry__0_i_3__0_n_1,remd_tmp_mux_rep_n_1}));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__0_i_1__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[6] ),
        .O(cal_tmp_carry__0_i_1__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__0_i_2__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[5] ),
        .O(cal_tmp_carry__0_i_2__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__0_i_3__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[4] ),
        .O(cal_tmp_carry__0_i_3__0_n_1));
  CARRY4 cal_tmp_carry__1
       (.CI(cal_tmp_carry__0_n_1),
        .CO({cal_tmp_carry__1_n_1,cal_tmp_carry__1_n_2,cal_tmp_carry__1_n_3,cal_tmp_carry__1_n_4}),
        .CYINIT(1'b0),
        .DI({1'b1,1'b1,1'b1,1'b1}),
        .O({cal_tmp_carry__1_n_5,cal_tmp_carry__1_n_6,cal_tmp_carry__1_n_7,cal_tmp_carry__1_n_8}),
        .S({cal_tmp_carry__1_i_1__0_n_1,cal_tmp_carry__1_i_2__0_n_1,cal_tmp_carry__1_i_3__0_n_1,cal_tmp_carry__1_i_4__0_n_1}));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__1_i_1__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[10] ),
        .O(cal_tmp_carry__1_i_1__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__1_i_2__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[9] ),
        .O(cal_tmp_carry__1_i_2__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__1_i_3__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[8] ),
        .O(cal_tmp_carry__1_i_3__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__1_i_4__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[7] ),
        .O(cal_tmp_carry__1_i_4__0_n_1));
  CARRY4 cal_tmp_carry__2
       (.CI(cal_tmp_carry__1_n_1),
        .CO({cal_tmp_carry__2_n_1,cal_tmp_carry__2_n_2,cal_tmp_carry__2_n_3,cal_tmp_carry__2_n_4}),
        .CYINIT(1'b0),
        .DI({1'b1,1'b1,1'b1,1'b1}),
        .O({cal_tmp_carry__2_n_5,cal_tmp_carry__2_n_6,cal_tmp_carry__2_n_7,cal_tmp_carry__2_n_8}),
        .S({cal_tmp_carry__2_i_1__0_n_1,cal_tmp_carry__2_i_2__0_n_1,cal_tmp_carry__2_i_3__0_n_1,cal_tmp_carry__2_i_4__0_n_1}));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__2_i_1__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[14] ),
        .O(cal_tmp_carry__2_i_1__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__2_i_2__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[13] ),
        .O(cal_tmp_carry__2_i_2__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__2_i_3__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[12] ),
        .O(cal_tmp_carry__2_i_3__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__2_i_4__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[11] ),
        .O(cal_tmp_carry__2_i_4__0_n_1));
  CARRY4 cal_tmp_carry__3
       (.CI(cal_tmp_carry__2_n_1),
        .CO({cal_tmp_carry__3_n_1,cal_tmp_carry__3_n_2,cal_tmp_carry__3_n_3,cal_tmp_carry__3_n_4}),
        .CYINIT(1'b0),
        .DI({1'b1,1'b1,1'b1,1'b1}),
        .O({cal_tmp_carry__3_n_5,cal_tmp_carry__3_n_6,cal_tmp_carry__3_n_7,cal_tmp_carry__3_n_8}),
        .S({cal_tmp_carry__3_i_1__0_n_1,cal_tmp_carry__3_i_2__0_n_1,cal_tmp_carry__3_i_3__0_n_1,cal_tmp_carry__3_i_4__0_n_1}));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__3_i_1__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[18] ),
        .O(cal_tmp_carry__3_i_1__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__3_i_2__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[17] ),
        .O(cal_tmp_carry__3_i_2__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__3_i_3__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[16] ),
        .O(cal_tmp_carry__3_i_3__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__3_i_4__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[15] ),
        .O(cal_tmp_carry__3_i_4__0_n_1));
  CARRY4 cal_tmp_carry__4
       (.CI(cal_tmp_carry__3_n_1),
        .CO({cal_tmp_carry__4_n_1,cal_tmp_carry__4_n_2,cal_tmp_carry__4_n_3,cal_tmp_carry__4_n_4}),
        .CYINIT(1'b0),
        .DI({1'b1,1'b1,1'b1,1'b1}),
        .O({cal_tmp_carry__4_n_5,cal_tmp_carry__4_n_6,cal_tmp_carry__4_n_7,cal_tmp_carry__4_n_8}),
        .S({cal_tmp_carry__4_i_1__0_n_1,cal_tmp_carry__4_i_2__0_n_1,cal_tmp_carry__4_i_3__0_n_1,cal_tmp_carry__4_i_4__0_n_1}));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__4_i_1__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[22] ),
        .O(cal_tmp_carry__4_i_1__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__4_i_2__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[21] ),
        .O(cal_tmp_carry__4_i_2__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__4_i_3__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[20] ),
        .O(cal_tmp_carry__4_i_3__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__4_i_4__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[19] ),
        .O(cal_tmp_carry__4_i_4__0_n_1));
  CARRY4 cal_tmp_carry__5
       (.CI(cal_tmp_carry__4_n_1),
        .CO({cal_tmp_carry__5_n_1,cal_tmp_carry__5_n_2,cal_tmp_carry__5_n_3,cal_tmp_carry__5_n_4}),
        .CYINIT(1'b0),
        .DI({1'b1,1'b1,1'b1,1'b1}),
        .O({cal_tmp_carry__5_n_5,cal_tmp_carry__5_n_6,cal_tmp_carry__5_n_7,cal_tmp_carry__5_n_8}),
        .S({cal_tmp_carry__5_i_1__0_n_1,cal_tmp_carry__5_i_2__0_n_1,cal_tmp_carry__5_i_3__0_n_1,cal_tmp_carry__5_i_4__0_n_1}));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__5_i_1__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[26] ),
        .O(cal_tmp_carry__5_i_1__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__5_i_2__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[25] ),
        .O(cal_tmp_carry__5_i_2__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__5_i_3__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[24] ),
        .O(cal_tmp_carry__5_i_3__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__5_i_4__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[23] ),
        .O(cal_tmp_carry__5_i_4__0_n_1));
  CARRY4 cal_tmp_carry__6
       (.CI(cal_tmp_carry__5_n_1),
        .CO({p_2_out,cal_tmp_carry__6_n_2,cal_tmp_carry__6_n_3,cal_tmp_carry__6_n_4}),
        .CYINIT(1'b0),
        .DI({1'b1,1'b1,1'b1,1'b1}),
        .O({cal_tmp_carry__6_n_5,cal_tmp_carry__6_n_6,cal_tmp_carry__6_n_7,cal_tmp_carry__6_n_8}),
        .S({cal_tmp_carry__6_i_1__0_n_1,cal_tmp_carry__6_i_2__0_n_1,cal_tmp_carry__6_i_3__0_n_1,cal_tmp_carry__6_i_4__0_n_1}));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__6_i_1__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[30] ),
        .O(cal_tmp_carry__6_i_1__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__6_i_2__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[29] ),
        .O(cal_tmp_carry__6_i_2__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__6_i_3__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[28] ),
        .O(cal_tmp_carry__6_i_3__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__6_i_4__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[27] ),
        .O(cal_tmp_carry__6_i_4__0_n_1));
  CARRY4 cal_tmp_carry__7
       (.CI(p_2_out),
        .CO(NLW_cal_tmp_carry__7_CO_UNCONNECTED[3:0]),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({NLW_cal_tmp_carry__7_O_UNCONNECTED[3:1],p_0_in}),
        .S({1'b0,1'b0,1'b0,1'b1}));
  LUT3 #(
    .INIT(8'hAC)) 
    cal_tmp_carry_i_1
       (.I0(\dividend0_reg_n_1_[31] ),
        .I1(\dividend_tmp_reg_n_1_[31] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(cal_tmp_carry_i_1_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry_i_2__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[2] ),
        .O(cal_tmp_carry_i_2__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry_i_3__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[1] ),
        .O(cal_tmp_carry_i_3__0_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry_i_4__0
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[0] ),
        .O(cal_tmp_carry_i_4__0_n_1));
  LUT3 #(
    .INIT(8'hAC)) 
    cal_tmp_carry_i_5
       (.I0(\dividend0_reg_n_1_[31] ),
        .I1(\dividend_tmp_reg_n_1_[31] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(cal_tmp_carry_i_5_n_1));
  FDRE \dividend0_reg[0] 
       (.C(ap_clk),
        .CE(E),
        .D(Q[0]),
        .Q(\dividend0_reg_n_1_[0] ),
        .R(1'b0));
  FDRE \dividend0_reg[10] 
       (.C(ap_clk),
        .CE(E),
        .D(D[9]),
        .Q(\dividend0_reg_n_1_[10] ),
        .R(1'b0));
  FDRE \dividend0_reg[11] 
       (.C(ap_clk),
        .CE(E),
        .D(D[10]),
        .Q(\dividend0_reg_n_1_[11] ),
        .R(1'b0));
  FDRE \dividend0_reg[12] 
       (.C(ap_clk),
        .CE(E),
        .D(D[11]),
        .Q(\dividend0_reg_n_1_[12] ),
        .R(1'b0));
  FDRE \dividend0_reg[13] 
       (.C(ap_clk),
        .CE(E),
        .D(D[12]),
        .Q(\dividend0_reg_n_1_[13] ),
        .R(1'b0));
  FDRE \dividend0_reg[14] 
       (.C(ap_clk),
        .CE(E),
        .D(D[13]),
        .Q(\dividend0_reg_n_1_[14] ),
        .R(1'b0));
  FDRE \dividend0_reg[15] 
       (.C(ap_clk),
        .CE(E),
        .D(D[14]),
        .Q(\dividend0_reg_n_1_[15] ),
        .R(1'b0));
  FDRE \dividend0_reg[16] 
       (.C(ap_clk),
        .CE(E),
        .D(D[15]),
        .Q(\dividend0_reg_n_1_[16] ),
        .R(1'b0));
  FDRE \dividend0_reg[17] 
       (.C(ap_clk),
        .CE(E),
        .D(D[16]),
        .Q(\dividend0_reg_n_1_[17] ),
        .R(1'b0));
  FDRE \dividend0_reg[18] 
       (.C(ap_clk),
        .CE(E),
        .D(D[17]),
        .Q(\dividend0_reg_n_1_[18] ),
        .R(1'b0));
  FDRE \dividend0_reg[19] 
       (.C(ap_clk),
        .CE(E),
        .D(D[18]),
        .Q(\dividend0_reg_n_1_[19] ),
        .R(1'b0));
  FDRE \dividend0_reg[1] 
       (.C(ap_clk),
        .CE(E),
        .D(D[0]),
        .Q(\dividend0_reg_n_1_[1] ),
        .R(1'b0));
  FDRE \dividend0_reg[20] 
       (.C(ap_clk),
        .CE(E),
        .D(D[19]),
        .Q(\dividend0_reg_n_1_[20] ),
        .R(1'b0));
  FDRE \dividend0_reg[21] 
       (.C(ap_clk),
        .CE(E),
        .D(D[20]),
        .Q(\dividend0_reg_n_1_[21] ),
        .R(1'b0));
  FDRE \dividend0_reg[22] 
       (.C(ap_clk),
        .CE(E),
        .D(D[21]),
        .Q(\dividend0_reg_n_1_[22] ),
        .R(1'b0));
  FDRE \dividend0_reg[23] 
       (.C(ap_clk),
        .CE(E),
        .D(D[22]),
        .Q(\dividend0_reg_n_1_[23] ),
        .R(1'b0));
  FDRE \dividend0_reg[24] 
       (.C(ap_clk),
        .CE(E),
        .D(D[23]),
        .Q(\dividend0_reg_n_1_[24] ),
        .R(1'b0));
  FDRE \dividend0_reg[25] 
       (.C(ap_clk),
        .CE(E),
        .D(D[24]),
        .Q(\dividend0_reg_n_1_[25] ),
        .R(1'b0));
  FDRE \dividend0_reg[26] 
       (.C(ap_clk),
        .CE(E),
        .D(D[25]),
        .Q(\dividend0_reg_n_1_[26] ),
        .R(1'b0));
  FDRE \dividend0_reg[27] 
       (.C(ap_clk),
        .CE(E),
        .D(D[26]),
        .Q(\dividend0_reg_n_1_[27] ),
        .R(1'b0));
  FDRE \dividend0_reg[28] 
       (.C(ap_clk),
        .CE(E),
        .D(D[27]),
        .Q(\dividend0_reg_n_1_[28] ),
        .R(1'b0));
  FDRE \dividend0_reg[29] 
       (.C(ap_clk),
        .CE(E),
        .D(D[28]),
        .Q(\dividend0_reg_n_1_[29] ),
        .R(1'b0));
  FDRE \dividend0_reg[2] 
       (.C(ap_clk),
        .CE(E),
        .D(D[1]),
        .Q(\dividend0_reg_n_1_[2] ),
        .R(1'b0));
  FDRE \dividend0_reg[30] 
       (.C(ap_clk),
        .CE(E),
        .D(D[29]),
        .Q(\dividend0_reg_n_1_[30] ),
        .R(1'b0));
  FDRE \dividend0_reg[31] 
       (.C(ap_clk),
        .CE(E),
        .D(D[30]),
        .Q(\dividend0_reg_n_1_[31] ),
        .R(1'b0));
  FDRE \dividend0_reg[3] 
       (.C(ap_clk),
        .CE(E),
        .D(D[2]),
        .Q(\dividend0_reg_n_1_[3] ),
        .R(1'b0));
  FDRE \dividend0_reg[4] 
       (.C(ap_clk),
        .CE(E),
        .D(D[3]),
        .Q(\dividend0_reg_n_1_[4] ),
        .R(1'b0));
  FDRE \dividend0_reg[5] 
       (.C(ap_clk),
        .CE(E),
        .D(D[4]),
        .Q(\dividend0_reg_n_1_[5] ),
        .R(1'b0));
  FDRE \dividend0_reg[6] 
       (.C(ap_clk),
        .CE(E),
        .D(D[5]),
        .Q(\dividend0_reg_n_1_[6] ),
        .R(1'b0));
  FDRE \dividend0_reg[7] 
       (.C(ap_clk),
        .CE(E),
        .D(D[6]),
        .Q(\dividend0_reg_n_1_[7] ),
        .R(1'b0));
  FDRE \dividend0_reg[8] 
       (.C(ap_clk),
        .CE(E),
        .D(D[7]),
        .Q(\dividend0_reg_n_1_[8] ),
        .R(1'b0));
  FDRE \dividend0_reg[9] 
       (.C(ap_clk),
        .CE(E),
        .D(D[8]),
        .Q(\dividend0_reg_n_1_[9] ),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair20" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[10]_i_1 
       (.I0(\dividend0_reg_n_1_[9] ),
        .I1(\dividend_tmp_reg_n_1_[9] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[10]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair20" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[11]_i_1 
       (.I0(\dividend0_reg_n_1_[10] ),
        .I1(\dividend_tmp_reg_n_1_[10] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[11]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair21" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[12]_i_1 
       (.I0(\dividend0_reg_n_1_[11] ),
        .I1(\dividend_tmp_reg_n_1_[11] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[12]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair21" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[13]_i_1 
       (.I0(\dividend0_reg_n_1_[12] ),
        .I1(\dividend_tmp_reg_n_1_[12] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[13]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair22" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[14]_i_1 
       (.I0(\dividend0_reg_n_1_[13] ),
        .I1(\dividend_tmp_reg_n_1_[13] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[14]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair22" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[15]_i_1 
       (.I0(\dividend0_reg_n_1_[14] ),
        .I1(\dividend_tmp_reg_n_1_[14] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[15]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair23" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[16]_i_1 
       (.I0(\dividend0_reg_n_1_[15] ),
        .I1(\dividend_tmp_reg_n_1_[15] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[16]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair23" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[17]_i_1 
       (.I0(\dividend0_reg_n_1_[16] ),
        .I1(\dividend_tmp_reg_n_1_[16] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[17]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair24" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[18]_i_1 
       (.I0(\dividend0_reg_n_1_[17] ),
        .I1(\dividend_tmp_reg_n_1_[17] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[18]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair24" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[19]_i_1 
       (.I0(\dividend0_reg_n_1_[18] ),
        .I1(\dividend_tmp_reg_n_1_[18] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[19]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair15" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[1]_i_1 
       (.I0(\dividend0_reg_n_1_[0] ),
        .I1(\dividend_tmp_reg_n_1_[0] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[1]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair25" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[20]_i_1 
       (.I0(\dividend0_reg_n_1_[19] ),
        .I1(\dividend_tmp_reg_n_1_[19] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[20]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair25" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[21]_i_1 
       (.I0(\dividend0_reg_n_1_[20] ),
        .I1(\dividend_tmp_reg_n_1_[20] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[21]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair26" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[22]_i_1 
       (.I0(\dividend0_reg_n_1_[21] ),
        .I1(\dividend_tmp_reg_n_1_[21] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[22]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair26" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[23]_i_1 
       (.I0(\dividend0_reg_n_1_[22] ),
        .I1(\dividend_tmp_reg_n_1_[22] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[23]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair27" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[24]_i_1 
       (.I0(\dividend0_reg_n_1_[23] ),
        .I1(\dividend_tmp_reg_n_1_[23] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[24]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair27" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[25]_i_1 
       (.I0(\dividend0_reg_n_1_[24] ),
        .I1(\dividend_tmp_reg_n_1_[24] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[25]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair28" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[26]_i_1 
       (.I0(\dividend0_reg_n_1_[25] ),
        .I1(\dividend_tmp_reg_n_1_[25] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[26]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair28" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[27]_i_1 
       (.I0(\dividend0_reg_n_1_[26] ),
        .I1(\dividend_tmp_reg_n_1_[26] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[27]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair29" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[28]_i_1 
       (.I0(\dividend0_reg_n_1_[27] ),
        .I1(\dividend_tmp_reg_n_1_[27] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[28]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair29" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[29]_i_1 
       (.I0(\dividend0_reg_n_1_[28] ),
        .I1(\dividend_tmp_reg_n_1_[28] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[29]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair16" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[2]_i_1 
       (.I0(\dividend0_reg_n_1_[1] ),
        .I1(\dividend_tmp_reg_n_1_[1] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[2]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[30]_i_1 
       (.I0(\dividend0_reg_n_1_[29] ),
        .I1(\dividend_tmp_reg_n_1_[29] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[30]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair15" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[31]_i_1 
       (.I0(\dividend0_reg_n_1_[30] ),
        .I1(\dividend_tmp_reg_n_1_[30] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[31]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair16" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[3]_i_1 
       (.I0(\dividend0_reg_n_1_[2] ),
        .I1(\dividend_tmp_reg_n_1_[2] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[3]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair17" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[4]_i_1 
       (.I0(\dividend0_reg_n_1_[3] ),
        .I1(\dividend_tmp_reg_n_1_[3] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[4]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair17" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[5]_i_1 
       (.I0(\dividend0_reg_n_1_[4] ),
        .I1(\dividend_tmp_reg_n_1_[4] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[5]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair18" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[6]_i_1 
       (.I0(\dividend0_reg_n_1_[5] ),
        .I1(\dividend_tmp_reg_n_1_[5] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[6]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair18" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[7]_i_1 
       (.I0(\dividend0_reg_n_1_[6] ),
        .I1(\dividend_tmp_reg_n_1_[6] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[7]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair19" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[8]_i_1 
       (.I0(\dividend0_reg_n_1_[7] ),
        .I1(\dividend_tmp_reg_n_1_[7] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[8]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair19" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[9]_i_1 
       (.I0(\dividend0_reg_n_1_[8] ),
        .I1(\dividend_tmp_reg_n_1_[8] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[9]_i_1_n_1 ));
  FDRE \dividend_tmp_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(p_2_out),
        .Q(\dividend_tmp_reg_n_1_[0] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[10] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[10]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[10] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[11] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[11]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[11] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[12] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[12]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[12] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[13] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[13]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[13] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[14] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[14]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[14] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[15] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[15]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[15] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[16] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[16]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[16] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[17] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[17]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[17] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[18] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[18]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[18] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[19] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[19]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[19] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[1] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[1]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[1] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[20] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[20]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[20] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[21] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[21]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[21] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[22] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[22]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[22] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[23] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[23]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[23] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[24] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[24]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[24] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[25] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[25]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[25] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[26] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[26]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[26] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[27] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[27]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[27] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[28] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[28]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[28] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[29] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[29]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[29] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[2]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[2] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[30] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[30]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[30] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[31] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[31]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[31] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[3] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[3]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[3] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[4] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[4]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[4] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[5] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[5]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[5] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[6] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[6]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[6] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[7] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[7]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[7] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[8] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[8]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[8] ),
        .R(1'b0));
  FDRE \dividend_tmp_reg[9] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[9]_i_1_n_1 ),
        .Q(\dividend_tmp_reg_n_1_[9] ),
        .R(1'b0));
  FDRE \r_stage_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(E),
        .Q(\r_stage_reg_n_1_[0] ),
        .R(ap_rst));
  (* srl_bus_name = "inst/\loop_imperfect_srbkb_U2/loop_imperfect_srbkb_div_U/loop_imperfect_srbkb_div_u_0/r_stage_reg " *) 
  (* srl_name = "inst/\loop_imperfect_srbkb_U2/loop_imperfect_srbkb_div_U/loop_imperfect_srbkb_div_u_0/r_stage_reg[30]_srl30___loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_28 " *) 
  SRLC32E \r_stage_reg[30]_srl30___loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_28 
       (.A({1'b1,1'b1,1'b1,1'b0,1'b1}),
        .CE(1'b1),
        .CLK(ap_clk),
        .D(\r_stage_reg_n_1_[0] ),
        .Q(\r_stage_reg[30]_srl30___loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_28_n_1 ),
        .Q31(\NLW_r_stage_reg[30]_srl30___loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_28_Q31_UNCONNECTED ));
  FDRE \r_stage_reg[31]_loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_29 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\r_stage_reg[30]_srl30___loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_28_n_1 ),
        .Q(\r_stage_reg[31]_loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_29_n_1 ),
        .R(1'b0));
  FDRE \r_stage_reg[32] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_gate_n_1),
        .Q(\r_stage_reg[32]_0 ),
        .R(ap_rst));
  LUT2 #(
    .INIT(4'h8)) 
    r_stage_reg_gate
       (.I0(\r_stage_reg[31]_loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_29_n_1 ),
        .I1(\r_stage_reg[32]_1 ),
        .O(r_stage_reg_gate_n_1));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[11]_i_2__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[11] ),
        .O(\remd[11]_i_2__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[11]_i_3__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[10] ),
        .O(\remd[11]_i_3__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[11]_i_4__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[9] ),
        .O(\remd[11]_i_4__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[11]_i_5__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[8] ),
        .O(\remd[11]_i_5__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[15]_i_2__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[15] ),
        .O(\remd[15]_i_2__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[15]_i_3__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[14] ),
        .O(\remd[15]_i_3__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[15]_i_4__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[13] ),
        .O(\remd[15]_i_4__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[15]_i_5__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[12] ),
        .O(\remd[15]_i_5__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[19]_i_2__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[19] ),
        .O(\remd[19]_i_2__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[19]_i_3__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[18] ),
        .O(\remd[19]_i_3__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[19]_i_4__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[17] ),
        .O(\remd[19]_i_4__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[19]_i_5__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[16] ),
        .O(\remd[19]_i_5__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[23]_i_2__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[23] ),
        .O(\remd[23]_i_2__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[23]_i_3__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[22] ),
        .O(\remd[23]_i_3__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[23]_i_4__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[21] ),
        .O(\remd[23]_i_4__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[23]_i_5__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[20] ),
        .O(\remd[23]_i_5__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[27]_i_2__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[27] ),
        .O(\remd[27]_i_2__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[27]_i_3__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[26] ),
        .O(\remd[27]_i_3__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[27]_i_4__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[25] ),
        .O(\remd[27]_i_4__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[27]_i_5__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[24] ),
        .O(\remd[27]_i_5__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[31]_i_2__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[31] ),
        .O(\remd[31]_i_2__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[31]_i_3__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[30] ),
        .O(\remd[31]_i_3__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[31]_i_4__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[29] ),
        .O(\remd[31]_i_4__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[31]_i_5__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[28] ),
        .O(\remd[31]_i_5__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[3]_i_2__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[3] ),
        .O(\remd[3]_i_2__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[3]_i_3__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[2] ),
        .O(\remd[3]_i_3__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[3]_i_4__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[1] ),
        .O(\remd[3]_i_4__0_n_1 ));
  LUT1 #(
    .INIT(2'h2)) 
    \remd[3]_i_5__0 
       (.I0(\remd_tmp_reg_n_1_[0] ),
        .O(\remd[3]_i_5__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[7]_i_2__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[7] ),
        .O(\remd[7]_i_2__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[7]_i_3__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[6] ),
        .O(\remd[7]_i_3__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[7]_i_4__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[5] ),
        .O(\remd[7]_i_4__0_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[7]_i_5__0 
       (.I0(\sign0_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[4] ),
        .O(\remd[7]_i_5__0_n_1 ));
  CARRY4 \remd_reg[11]_i_1__0 
       (.CI(\remd_reg[7]_i_1__0_n_1 ),
        .CO({\remd_reg[11]_i_1__0_n_1 ,\remd_reg[11]_i_1__0_n_2 ,\remd_reg[11]_i_1__0_n_3 ,\remd_reg[11]_i_1__0_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(O9[11:8]),
        .S({\remd[11]_i_2__0_n_1 ,\remd[11]_i_3__0_n_1 ,\remd[11]_i_4__0_n_1 ,\remd[11]_i_5__0_n_1 }));
  CARRY4 \remd_reg[15]_i_1__0 
       (.CI(\remd_reg[11]_i_1__0_n_1 ),
        .CO({\remd_reg[15]_i_1__0_n_1 ,\remd_reg[15]_i_1__0_n_2 ,\remd_reg[15]_i_1__0_n_3 ,\remd_reg[15]_i_1__0_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(O9[15:12]),
        .S({\remd[15]_i_2__0_n_1 ,\remd[15]_i_3__0_n_1 ,\remd[15]_i_4__0_n_1 ,\remd[15]_i_5__0_n_1 }));
  CARRY4 \remd_reg[19]_i_1__0 
       (.CI(\remd_reg[15]_i_1__0_n_1 ),
        .CO({\remd_reg[19]_i_1__0_n_1 ,\remd_reg[19]_i_1__0_n_2 ,\remd_reg[19]_i_1__0_n_3 ,\remd_reg[19]_i_1__0_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(O9[19:16]),
        .S({\remd[19]_i_2__0_n_1 ,\remd[19]_i_3__0_n_1 ,\remd[19]_i_4__0_n_1 ,\remd[19]_i_5__0_n_1 }));
  CARRY4 \remd_reg[23]_i_1__0 
       (.CI(\remd_reg[19]_i_1__0_n_1 ),
        .CO({\remd_reg[23]_i_1__0_n_1 ,\remd_reg[23]_i_1__0_n_2 ,\remd_reg[23]_i_1__0_n_3 ,\remd_reg[23]_i_1__0_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(O9[23:20]),
        .S({\remd[23]_i_2__0_n_1 ,\remd[23]_i_3__0_n_1 ,\remd[23]_i_4__0_n_1 ,\remd[23]_i_5__0_n_1 }));
  CARRY4 \remd_reg[27]_i_1__0 
       (.CI(\remd_reg[23]_i_1__0_n_1 ),
        .CO({\remd_reg[27]_i_1__0_n_1 ,\remd_reg[27]_i_1__0_n_2 ,\remd_reg[27]_i_1__0_n_3 ,\remd_reg[27]_i_1__0_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(O9[27:24]),
        .S({\remd[27]_i_2__0_n_1 ,\remd[27]_i_3__0_n_1 ,\remd[27]_i_4__0_n_1 ,\remd[27]_i_5__0_n_1 }));
  CARRY4 \remd_reg[31]_i_1__0 
       (.CI(\remd_reg[27]_i_1__0_n_1 ),
        .CO({\NLW_remd_reg[31]_i_1__0_CO_UNCONNECTED [3],\remd_reg[31]_i_1__0_n_2 ,\remd_reg[31]_i_1__0_n_3 ,\remd_reg[31]_i_1__0_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(O9[31:28]),
        .S({\remd[31]_i_2__0_n_1 ,\remd[31]_i_3__0_n_1 ,\remd[31]_i_4__0_n_1 ,\remd[31]_i_5__0_n_1 }));
  CARRY4 \remd_reg[3]_i_1__0 
       (.CI(1'b0),
        .CO({\remd_reg[3]_i_1__0_n_1 ,\remd_reg[3]_i_1__0_n_2 ,\remd_reg[3]_i_1__0_n_3 ,\remd_reg[3]_i_1__0_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,\sign0_reg_n_1_[0] }),
        .O(O9[3:0]),
        .S({\remd[3]_i_2__0_n_1 ,\remd[3]_i_3__0_n_1 ,\remd[3]_i_4__0_n_1 ,\remd[3]_i_5__0_n_1 }));
  CARRY4 \remd_reg[7]_i_1__0 
       (.CI(\remd_reg[3]_i_1__0_n_1 ),
        .CO({\remd_reg[7]_i_1__0_n_1 ,\remd_reg[7]_i_1__0_n_2 ,\remd_reg[7]_i_1__0_n_3 ,\remd_reg[7]_i_1__0_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(O9[7:4]),
        .S({\remd[7]_i_2__0_n_1 ,\remd[7]_i_3__0_n_1 ,\remd[7]_i_4__0_n_1 ,\remd[7]_i_5__0_n_1 }));
  LUT5 #(
    .INIT(32'hACFFAC00)) 
    \remd_tmp[0]_i_1 
       (.I0(\dividend0_reg_n_1_[31] ),
        .I1(\dividend_tmp_reg_n_1_[31] ),
        .I2(\r_stage_reg_n_1_[0] ),
        .I3(p_0_in),
        .I4(cal_tmp_carry_n_8),
        .O(\remd_tmp[0]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[10]_i_1 
       (.I0(\remd_tmp_reg_n_1_[9] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__1_n_6),
        .O(\remd_tmp[10]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[11]_i_1 
       (.I0(\remd_tmp_reg_n_1_[10] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__1_n_5),
        .O(\remd_tmp[11]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[12]_i_1 
       (.I0(\remd_tmp_reg_n_1_[11] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__2_n_8),
        .O(\remd_tmp[12]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[13]_i_1 
       (.I0(\remd_tmp_reg_n_1_[12] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__2_n_7),
        .O(\remd_tmp[13]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[14]_i_1 
       (.I0(\remd_tmp_reg_n_1_[13] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__2_n_6),
        .O(\remd_tmp[14]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[15]_i_1 
       (.I0(\remd_tmp_reg_n_1_[14] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__2_n_5),
        .O(\remd_tmp[15]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[16]_i_1 
       (.I0(\remd_tmp_reg_n_1_[15] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__3_n_8),
        .O(\remd_tmp[16]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[17]_i_1 
       (.I0(\remd_tmp_reg_n_1_[16] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__3_n_7),
        .O(\remd_tmp[17]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[18]_i_1 
       (.I0(\remd_tmp_reg_n_1_[17] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__3_n_6),
        .O(\remd_tmp[18]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[19]_i_1 
       (.I0(\remd_tmp_reg_n_1_[18] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__3_n_5),
        .O(\remd_tmp[19]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[1]_i_1 
       (.I0(\remd_tmp_reg_n_1_[0] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry_n_7),
        .O(\remd_tmp[1]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[20]_i_1 
       (.I0(\remd_tmp_reg_n_1_[19] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__4_n_8),
        .O(\remd_tmp[20]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[21]_i_1 
       (.I0(\remd_tmp_reg_n_1_[20] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__4_n_7),
        .O(\remd_tmp[21]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[22]_i_1 
       (.I0(\remd_tmp_reg_n_1_[21] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__4_n_6),
        .O(\remd_tmp[22]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[23]_i_1 
       (.I0(\remd_tmp_reg_n_1_[22] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__4_n_5),
        .O(\remd_tmp[23]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[24]_i_1 
       (.I0(\remd_tmp_reg_n_1_[23] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__5_n_8),
        .O(\remd_tmp[24]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[25]_i_1 
       (.I0(\remd_tmp_reg_n_1_[24] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__5_n_7),
        .O(\remd_tmp[25]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[26]_i_1 
       (.I0(\remd_tmp_reg_n_1_[25] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__5_n_6),
        .O(\remd_tmp[26]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[27]_i_1 
       (.I0(\remd_tmp_reg_n_1_[26] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__5_n_5),
        .O(\remd_tmp[27]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[28]_i_1 
       (.I0(\remd_tmp_reg_n_1_[27] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__6_n_8),
        .O(\remd_tmp[28]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[29]_i_1 
       (.I0(\remd_tmp_reg_n_1_[28] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__6_n_7),
        .O(\remd_tmp[29]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[2]_i_1 
       (.I0(\remd_tmp_reg_n_1_[1] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry_n_6),
        .O(\remd_tmp[2]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[30]_i_1 
       (.I0(\remd_tmp_reg_n_1_[29] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__6_n_6),
        .O(\remd_tmp[30]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[31]_i_1 
       (.I0(\remd_tmp_reg_n_1_[30] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__6_n_5),
        .O(\remd_tmp[31]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[3]_i_1 
       (.I0(\remd_tmp_reg_n_1_[2] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry_n_5),
        .O(\remd_tmp[3]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[4]_i_1 
       (.I0(\remd_tmp_reg_n_1_[3] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__0_n_8),
        .O(\remd_tmp[4]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[5]_i_1 
       (.I0(\remd_tmp_reg_n_1_[4] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__0_n_7),
        .O(\remd_tmp[5]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[6]_i_1 
       (.I0(\remd_tmp_reg_n_1_[5] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__0_n_6),
        .O(\remd_tmp[6]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[7]_i_1 
       (.I0(\remd_tmp_reg_n_1_[6] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__0_n_5),
        .O(\remd_tmp[7]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[8]_i_1 
       (.I0(\remd_tmp_reg_n_1_[7] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__1_n_8),
        .O(\remd_tmp[8]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[9]_i_1 
       (.I0(\remd_tmp_reg_n_1_[8] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__1_n_7),
        .O(\remd_tmp[9]_i_1_n_1 ));
  LUT2 #(
    .INIT(4'h2)) 
    remd_tmp_mux
       (.I0(\remd_tmp_reg_n_1_[3] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .O(remd_tmp_mux_n_1));
  LUT2 #(
    .INIT(4'h2)) 
    remd_tmp_mux_rep
       (.I0(\remd_tmp_reg_n_1_[3] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .O(remd_tmp_mux_rep_n_1));
  FDRE \remd_tmp_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[0]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[0] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[10] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[10]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[10] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[11] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[11]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[11] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[12] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[12]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[12] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[13] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[13]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[13] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[14] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[14]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[14] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[15] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[15]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[15] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[16] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[16]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[16] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[17] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[17]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[17] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[18] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[18]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[18] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[19] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[19]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[19] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[1] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[1]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[1] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[20] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[20]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[20] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[21] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[21]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[21] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[22] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[22]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[22] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[23] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[23]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[23] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[24] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[24]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[24] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[25] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[25]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[25] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[26] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[26]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[26] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[27] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[27]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[27] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[28] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[28]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[28] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[29] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[29]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[29] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[2]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[2] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[30] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[30]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[30] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[31] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[31]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[31] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[3] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[3]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[3] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[4] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[4]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[4] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[5] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[5]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[5] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[6] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[6]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[6] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[7] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[7]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[7] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[8] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[8]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[8] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[9] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[9]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[9] ),
        .R(1'b0));
  FDRE \sign0_reg[0] 
       (.C(ap_clk),
        .CE(E),
        .D(Q[1]),
        .Q(\sign0_reg_n_1_[0] ),
        .R(1'b0));
endmodule

(* ORIG_REF_NAME = "loop_imperfect_srbkb_div_u" *) 
module bd_0_hls_inst_0_loop_imperfect_srbkb_div_u_2
   (r_stage_reg_r_29_0,
    \r_stage_reg[32]_0 ,
    O7,
    ap_rst,
    E,
    ap_clk,
    Q,
    D);
  output r_stage_reg_r_29_0;
  output [0:0]\r_stage_reg[32]_0 ;
  output [31:0]O7;
  input ap_rst;
  input [0:0]E;
  input ap_clk;
  input [1:0]Q;
  input [30:0]D;

  wire [30:0]D;
  wire [0:0]E;
  wire [31:0]O7;
  wire [1:0]Q;
  wire ap_clk;
  wire ap_rst;
  wire cal_tmp_carry__0_i_1_n_1;
  wire cal_tmp_carry__0_i_2_n_1;
  wire cal_tmp_carry__0_i_3_n_1;
  wire cal_tmp_carry__0_n_1;
  wire cal_tmp_carry__0_n_2;
  wire cal_tmp_carry__0_n_3;
  wire cal_tmp_carry__0_n_4;
  wire cal_tmp_carry__0_n_5;
  wire cal_tmp_carry__0_n_6;
  wire cal_tmp_carry__0_n_7;
  wire cal_tmp_carry__0_n_8;
  wire cal_tmp_carry__1_i_1_n_1;
  wire cal_tmp_carry__1_i_2_n_1;
  wire cal_tmp_carry__1_i_3_n_1;
  wire cal_tmp_carry__1_i_4_n_1;
  wire cal_tmp_carry__1_n_1;
  wire cal_tmp_carry__1_n_2;
  wire cal_tmp_carry__1_n_3;
  wire cal_tmp_carry__1_n_4;
  wire cal_tmp_carry__1_n_5;
  wire cal_tmp_carry__1_n_6;
  wire cal_tmp_carry__1_n_7;
  wire cal_tmp_carry__1_n_8;
  wire cal_tmp_carry__2_i_1_n_1;
  wire cal_tmp_carry__2_i_2_n_1;
  wire cal_tmp_carry__2_i_3_n_1;
  wire cal_tmp_carry__2_i_4_n_1;
  wire cal_tmp_carry__2_n_1;
  wire cal_tmp_carry__2_n_2;
  wire cal_tmp_carry__2_n_3;
  wire cal_tmp_carry__2_n_4;
  wire cal_tmp_carry__2_n_5;
  wire cal_tmp_carry__2_n_6;
  wire cal_tmp_carry__2_n_7;
  wire cal_tmp_carry__2_n_8;
  wire cal_tmp_carry__3_i_1_n_1;
  wire cal_tmp_carry__3_i_2_n_1;
  wire cal_tmp_carry__3_i_3_n_1;
  wire cal_tmp_carry__3_i_4_n_1;
  wire cal_tmp_carry__3_n_1;
  wire cal_tmp_carry__3_n_2;
  wire cal_tmp_carry__3_n_3;
  wire cal_tmp_carry__3_n_4;
  wire cal_tmp_carry__3_n_5;
  wire cal_tmp_carry__3_n_6;
  wire cal_tmp_carry__3_n_7;
  wire cal_tmp_carry__3_n_8;
  wire cal_tmp_carry__4_i_1_n_1;
  wire cal_tmp_carry__4_i_2_n_1;
  wire cal_tmp_carry__4_i_3_n_1;
  wire cal_tmp_carry__4_i_4_n_1;
  wire cal_tmp_carry__4_n_1;
  wire cal_tmp_carry__4_n_2;
  wire cal_tmp_carry__4_n_3;
  wire cal_tmp_carry__4_n_4;
  wire cal_tmp_carry__4_n_5;
  wire cal_tmp_carry__4_n_6;
  wire cal_tmp_carry__4_n_7;
  wire cal_tmp_carry__4_n_8;
  wire cal_tmp_carry__5_i_1_n_1;
  wire cal_tmp_carry__5_i_2_n_1;
  wire cal_tmp_carry__5_i_3_n_1;
  wire cal_tmp_carry__5_i_4_n_1;
  wire cal_tmp_carry__5_n_1;
  wire cal_tmp_carry__5_n_2;
  wire cal_tmp_carry__5_n_3;
  wire cal_tmp_carry__5_n_4;
  wire cal_tmp_carry__5_n_5;
  wire cal_tmp_carry__5_n_6;
  wire cal_tmp_carry__5_n_7;
  wire cal_tmp_carry__5_n_8;
  wire cal_tmp_carry__6_i_1_n_1;
  wire cal_tmp_carry__6_i_2_n_1;
  wire cal_tmp_carry__6_i_3_n_1;
  wire cal_tmp_carry__6_i_4_n_1;
  wire cal_tmp_carry__6_n_2;
  wire cal_tmp_carry__6_n_3;
  wire cal_tmp_carry__6_n_4;
  wire cal_tmp_carry__6_n_5;
  wire cal_tmp_carry__6_n_6;
  wire cal_tmp_carry__6_n_7;
  wire cal_tmp_carry__6_n_8;
  wire cal_tmp_carry_i_2_n_1;
  wire cal_tmp_carry_i_3_n_1;
  wire cal_tmp_carry_i_4_n_1;
  wire cal_tmp_carry_i_5_n_1;
  wire cal_tmp_carry_n_1;
  wire cal_tmp_carry_n_2;
  wire cal_tmp_carry_n_3;
  wire cal_tmp_carry_n_4;
  wire cal_tmp_carry_n_5;
  wire cal_tmp_carry_n_6;
  wire cal_tmp_carry_n_7;
  wire cal_tmp_carry_n_8;
  wire \dividend0_reg_n_1_[0] ;
  wire \dividend0_reg_n_1_[10] ;
  wire \dividend0_reg_n_1_[11] ;
  wire \dividend0_reg_n_1_[12] ;
  wire \dividend0_reg_n_1_[13] ;
  wire \dividend0_reg_n_1_[14] ;
  wire \dividend0_reg_n_1_[15] ;
  wire \dividend0_reg_n_1_[16] ;
  wire \dividend0_reg_n_1_[17] ;
  wire \dividend0_reg_n_1_[18] ;
  wire \dividend0_reg_n_1_[19] ;
  wire \dividend0_reg_n_1_[1] ;
  wire \dividend0_reg_n_1_[20] ;
  wire \dividend0_reg_n_1_[21] ;
  wire \dividend0_reg_n_1_[22] ;
  wire \dividend0_reg_n_1_[23] ;
  wire \dividend0_reg_n_1_[24] ;
  wire \dividend0_reg_n_1_[25] ;
  wire \dividend0_reg_n_1_[26] ;
  wire \dividend0_reg_n_1_[27] ;
  wire \dividend0_reg_n_1_[28] ;
  wire \dividend0_reg_n_1_[29] ;
  wire \dividend0_reg_n_1_[2] ;
  wire \dividend0_reg_n_1_[30] ;
  wire \dividend0_reg_n_1_[31] ;
  wire \dividend0_reg_n_1_[3] ;
  wire \dividend0_reg_n_1_[4] ;
  wire \dividend0_reg_n_1_[5] ;
  wire \dividend0_reg_n_1_[6] ;
  wire \dividend0_reg_n_1_[7] ;
  wire \dividend0_reg_n_1_[8] ;
  wire \dividend0_reg_n_1_[9] ;
  wire [31:0]dividend_tmp;
  wire \dividend_tmp[10]_i_1_n_1 ;
  wire \dividend_tmp[11]_i_1_n_1 ;
  wire \dividend_tmp[12]_i_1_n_1 ;
  wire \dividend_tmp[13]_i_1_n_1 ;
  wire \dividend_tmp[14]_i_1_n_1 ;
  wire \dividend_tmp[15]_i_1_n_1 ;
  wire \dividend_tmp[16]_i_1_n_1 ;
  wire \dividend_tmp[17]_i_1_n_1 ;
  wire \dividend_tmp[18]_i_1_n_1 ;
  wire \dividend_tmp[19]_i_1_n_1 ;
  wire \dividend_tmp[1]_i_1_n_1 ;
  wire \dividend_tmp[20]_i_1_n_1 ;
  wire \dividend_tmp[21]_i_1_n_1 ;
  wire \dividend_tmp[22]_i_1_n_1 ;
  wire \dividend_tmp[23]_i_1_n_1 ;
  wire \dividend_tmp[24]_i_1_n_1 ;
  wire \dividend_tmp[25]_i_1_n_1 ;
  wire \dividend_tmp[26]_i_1_n_1 ;
  wire \dividend_tmp[27]_i_1_n_1 ;
  wire \dividend_tmp[28]_i_1_n_1 ;
  wire \dividend_tmp[29]_i_1_n_1 ;
  wire \dividend_tmp[2]_i_1_n_1 ;
  wire \dividend_tmp[30]_i_1_n_1 ;
  wire \dividend_tmp[31]_i_1_n_1 ;
  wire \dividend_tmp[3]_i_1_n_1 ;
  wire \dividend_tmp[4]_i_1_n_1 ;
  wire \dividend_tmp[5]_i_1_n_1 ;
  wire \dividend_tmp[6]_i_1_n_1 ;
  wire \dividend_tmp[7]_i_1_n_1 ;
  wire \dividend_tmp[8]_i_1_n_1 ;
  wire \dividend_tmp[9]_i_1_n_1 ;
  wire p_0_in;
  wire p_1_in0;
  wire [0:0]p_2_out;
  wire \r_stage_reg[30]_srl30___loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_28_n_1 ;
  wire \r_stage_reg[31]_loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_29_n_1 ;
  wire [0:0]\r_stage_reg[32]_0 ;
  wire r_stage_reg_gate_n_1;
  wire \r_stage_reg_n_1_[0] ;
  wire r_stage_reg_r_0_n_1;
  wire r_stage_reg_r_10_n_1;
  wire r_stage_reg_r_11_n_1;
  wire r_stage_reg_r_12_n_1;
  wire r_stage_reg_r_13_n_1;
  wire r_stage_reg_r_14_n_1;
  wire r_stage_reg_r_15_n_1;
  wire r_stage_reg_r_16_n_1;
  wire r_stage_reg_r_17_n_1;
  wire r_stage_reg_r_18_n_1;
  wire r_stage_reg_r_19_n_1;
  wire r_stage_reg_r_1_n_1;
  wire r_stage_reg_r_20_n_1;
  wire r_stage_reg_r_21_n_1;
  wire r_stage_reg_r_22_n_1;
  wire r_stage_reg_r_23_n_1;
  wire r_stage_reg_r_24_n_1;
  wire r_stage_reg_r_25_n_1;
  wire r_stage_reg_r_26_n_1;
  wire r_stage_reg_r_27_n_1;
  wire r_stage_reg_r_28_n_1;
  wire r_stage_reg_r_29_0;
  wire r_stage_reg_r_2_n_1;
  wire r_stage_reg_r_3_n_1;
  wire r_stage_reg_r_4_n_1;
  wire r_stage_reg_r_5_n_1;
  wire r_stage_reg_r_6_n_1;
  wire r_stage_reg_r_7_n_1;
  wire r_stage_reg_r_8_n_1;
  wire r_stage_reg_r_9_n_1;
  wire r_stage_reg_r_n_1;
  wire \remd[11]_i_2_n_1 ;
  wire \remd[11]_i_3_n_1 ;
  wire \remd[11]_i_4_n_1 ;
  wire \remd[11]_i_5_n_1 ;
  wire \remd[15]_i_2_n_1 ;
  wire \remd[15]_i_3_n_1 ;
  wire \remd[15]_i_4_n_1 ;
  wire \remd[15]_i_5_n_1 ;
  wire \remd[19]_i_2_n_1 ;
  wire \remd[19]_i_3_n_1 ;
  wire \remd[19]_i_4_n_1 ;
  wire \remd[19]_i_5_n_1 ;
  wire \remd[23]_i_2_n_1 ;
  wire \remd[23]_i_3_n_1 ;
  wire \remd[23]_i_4_n_1 ;
  wire \remd[23]_i_5_n_1 ;
  wire \remd[27]_i_2_n_1 ;
  wire \remd[27]_i_3_n_1 ;
  wire \remd[27]_i_4_n_1 ;
  wire \remd[27]_i_5_n_1 ;
  wire \remd[31]_i_2_n_1 ;
  wire \remd[31]_i_3_n_1 ;
  wire \remd[31]_i_4_n_1 ;
  wire \remd[31]_i_5_n_1 ;
  wire \remd[3]_i_2_n_1 ;
  wire \remd[3]_i_3_n_1 ;
  wire \remd[3]_i_4_n_1 ;
  wire \remd[3]_i_5_n_1 ;
  wire \remd[7]_i_2_n_1 ;
  wire \remd[7]_i_3_n_1 ;
  wire \remd[7]_i_4_n_1 ;
  wire \remd[7]_i_5_n_1 ;
  wire \remd_reg[11]_i_1_n_1 ;
  wire \remd_reg[11]_i_1_n_2 ;
  wire \remd_reg[11]_i_1_n_3 ;
  wire \remd_reg[11]_i_1_n_4 ;
  wire \remd_reg[15]_i_1_n_1 ;
  wire \remd_reg[15]_i_1_n_2 ;
  wire \remd_reg[15]_i_1_n_3 ;
  wire \remd_reg[15]_i_1_n_4 ;
  wire \remd_reg[19]_i_1_n_1 ;
  wire \remd_reg[19]_i_1_n_2 ;
  wire \remd_reg[19]_i_1_n_3 ;
  wire \remd_reg[19]_i_1_n_4 ;
  wire \remd_reg[23]_i_1_n_1 ;
  wire \remd_reg[23]_i_1_n_2 ;
  wire \remd_reg[23]_i_1_n_3 ;
  wire \remd_reg[23]_i_1_n_4 ;
  wire \remd_reg[27]_i_1_n_1 ;
  wire \remd_reg[27]_i_1_n_2 ;
  wire \remd_reg[27]_i_1_n_3 ;
  wire \remd_reg[27]_i_1_n_4 ;
  wire \remd_reg[31]_i_1_n_2 ;
  wire \remd_reg[31]_i_1_n_3 ;
  wire \remd_reg[31]_i_1_n_4 ;
  wire \remd_reg[3]_i_1_n_1 ;
  wire \remd_reg[3]_i_1_n_2 ;
  wire \remd_reg[3]_i_1_n_3 ;
  wire \remd_reg[3]_i_1_n_4 ;
  wire \remd_reg[7]_i_1_n_1 ;
  wire \remd_reg[7]_i_1_n_2 ;
  wire \remd_reg[7]_i_1_n_3 ;
  wire \remd_reg[7]_i_1_n_4 ;
  wire \remd_tmp[0]_i_1_n_1 ;
  wire \remd_tmp[10]_i_1_n_1 ;
  wire \remd_tmp[11]_i_1_n_1 ;
  wire \remd_tmp[12]_i_1_n_1 ;
  wire \remd_tmp[13]_i_1_n_1 ;
  wire \remd_tmp[14]_i_1_n_1 ;
  wire \remd_tmp[15]_i_1_n_1 ;
  wire \remd_tmp[16]_i_1_n_1 ;
  wire \remd_tmp[17]_i_1_n_1 ;
  wire \remd_tmp[18]_i_1_n_1 ;
  wire \remd_tmp[19]_i_1_n_1 ;
  wire \remd_tmp[1]_i_1_n_1 ;
  wire \remd_tmp[20]_i_1_n_1 ;
  wire \remd_tmp[21]_i_1_n_1 ;
  wire \remd_tmp[22]_i_1_n_1 ;
  wire \remd_tmp[23]_i_1_n_1 ;
  wire \remd_tmp[24]_i_1_n_1 ;
  wire \remd_tmp[25]_i_1_n_1 ;
  wire \remd_tmp[26]_i_1_n_1 ;
  wire \remd_tmp[27]_i_1_n_1 ;
  wire \remd_tmp[28]_i_1_n_1 ;
  wire \remd_tmp[29]_i_1_n_1 ;
  wire \remd_tmp[2]_i_1_n_1 ;
  wire \remd_tmp[30]_i_1_n_1 ;
  wire \remd_tmp[31]_i_1_n_1 ;
  wire \remd_tmp[3]_i_1_n_1 ;
  wire \remd_tmp[4]_i_1_n_1 ;
  wire \remd_tmp[5]_i_1_n_1 ;
  wire \remd_tmp[6]_i_1_n_1 ;
  wire \remd_tmp[7]_i_1_n_1 ;
  wire \remd_tmp[8]_i_1_n_1 ;
  wire \remd_tmp[9]_i_1_n_1 ;
  wire [3:3]remd_tmp_mux__0;
  wire remd_tmp_mux_rep_n_1;
  wire \remd_tmp_reg_n_1_[0] ;
  wire \remd_tmp_reg_n_1_[10] ;
  wire \remd_tmp_reg_n_1_[11] ;
  wire \remd_tmp_reg_n_1_[12] ;
  wire \remd_tmp_reg_n_1_[13] ;
  wire \remd_tmp_reg_n_1_[14] ;
  wire \remd_tmp_reg_n_1_[15] ;
  wire \remd_tmp_reg_n_1_[16] ;
  wire \remd_tmp_reg_n_1_[17] ;
  wire \remd_tmp_reg_n_1_[18] ;
  wire \remd_tmp_reg_n_1_[19] ;
  wire \remd_tmp_reg_n_1_[1] ;
  wire \remd_tmp_reg_n_1_[20] ;
  wire \remd_tmp_reg_n_1_[21] ;
  wire \remd_tmp_reg_n_1_[22] ;
  wire \remd_tmp_reg_n_1_[23] ;
  wire \remd_tmp_reg_n_1_[24] ;
  wire \remd_tmp_reg_n_1_[25] ;
  wire \remd_tmp_reg_n_1_[26] ;
  wire \remd_tmp_reg_n_1_[27] ;
  wire \remd_tmp_reg_n_1_[28] ;
  wire \remd_tmp_reg_n_1_[29] ;
  wire \remd_tmp_reg_n_1_[2] ;
  wire \remd_tmp_reg_n_1_[30] ;
  wire \remd_tmp_reg_n_1_[31] ;
  wire \remd_tmp_reg_n_1_[3] ;
  wire \remd_tmp_reg_n_1_[4] ;
  wire \remd_tmp_reg_n_1_[5] ;
  wire \remd_tmp_reg_n_1_[6] ;
  wire \remd_tmp_reg_n_1_[7] ;
  wire \remd_tmp_reg_n_1_[8] ;
  wire \remd_tmp_reg_n_1_[9] ;
  wire sign0;
  wire [3:0]NLW_cal_tmp_carry__7_CO_UNCONNECTED;
  wire [3:1]NLW_cal_tmp_carry__7_O_UNCONNECTED;
  wire \NLW_r_stage_reg[30]_srl30___loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_28_Q31_UNCONNECTED ;
  wire [3:3]\NLW_remd_reg[31]_i_1_CO_UNCONNECTED ;

  CARRY4 cal_tmp_carry
       (.CI(1'b0),
        .CO({cal_tmp_carry_n_1,cal_tmp_carry_n_2,cal_tmp_carry_n_3,cal_tmp_carry_n_4}),
        .CYINIT(1'b1),
        .DI({1'b1,1'b1,1'b1,p_1_in0}),
        .O({cal_tmp_carry_n_5,cal_tmp_carry_n_6,cal_tmp_carry_n_7,cal_tmp_carry_n_8}),
        .S({cal_tmp_carry_i_2_n_1,cal_tmp_carry_i_3_n_1,cal_tmp_carry_i_4_n_1,cal_tmp_carry_i_5_n_1}));
  CARRY4 cal_tmp_carry__0
       (.CI(cal_tmp_carry_n_1),
        .CO({cal_tmp_carry__0_n_1,cal_tmp_carry__0_n_2,cal_tmp_carry__0_n_3,cal_tmp_carry__0_n_4}),
        .CYINIT(1'b0),
        .DI({1'b1,1'b1,1'b1,remd_tmp_mux__0}),
        .O({cal_tmp_carry__0_n_5,cal_tmp_carry__0_n_6,cal_tmp_carry__0_n_7,cal_tmp_carry__0_n_8}),
        .S({cal_tmp_carry__0_i_1_n_1,cal_tmp_carry__0_i_2_n_1,cal_tmp_carry__0_i_3_n_1,remd_tmp_mux_rep_n_1}));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__0_i_1
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[6] ),
        .O(cal_tmp_carry__0_i_1_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__0_i_2
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[5] ),
        .O(cal_tmp_carry__0_i_2_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__0_i_3
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[4] ),
        .O(cal_tmp_carry__0_i_3_n_1));
  CARRY4 cal_tmp_carry__1
       (.CI(cal_tmp_carry__0_n_1),
        .CO({cal_tmp_carry__1_n_1,cal_tmp_carry__1_n_2,cal_tmp_carry__1_n_3,cal_tmp_carry__1_n_4}),
        .CYINIT(1'b0),
        .DI({1'b1,1'b1,1'b1,1'b1}),
        .O({cal_tmp_carry__1_n_5,cal_tmp_carry__1_n_6,cal_tmp_carry__1_n_7,cal_tmp_carry__1_n_8}),
        .S({cal_tmp_carry__1_i_1_n_1,cal_tmp_carry__1_i_2_n_1,cal_tmp_carry__1_i_3_n_1,cal_tmp_carry__1_i_4_n_1}));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__1_i_1
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[10] ),
        .O(cal_tmp_carry__1_i_1_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__1_i_2
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[9] ),
        .O(cal_tmp_carry__1_i_2_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__1_i_3
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[8] ),
        .O(cal_tmp_carry__1_i_3_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__1_i_4
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[7] ),
        .O(cal_tmp_carry__1_i_4_n_1));
  CARRY4 cal_tmp_carry__2
       (.CI(cal_tmp_carry__1_n_1),
        .CO({cal_tmp_carry__2_n_1,cal_tmp_carry__2_n_2,cal_tmp_carry__2_n_3,cal_tmp_carry__2_n_4}),
        .CYINIT(1'b0),
        .DI({1'b1,1'b1,1'b1,1'b1}),
        .O({cal_tmp_carry__2_n_5,cal_tmp_carry__2_n_6,cal_tmp_carry__2_n_7,cal_tmp_carry__2_n_8}),
        .S({cal_tmp_carry__2_i_1_n_1,cal_tmp_carry__2_i_2_n_1,cal_tmp_carry__2_i_3_n_1,cal_tmp_carry__2_i_4_n_1}));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__2_i_1
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[14] ),
        .O(cal_tmp_carry__2_i_1_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__2_i_2
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[13] ),
        .O(cal_tmp_carry__2_i_2_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__2_i_3
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[12] ),
        .O(cal_tmp_carry__2_i_3_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__2_i_4
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[11] ),
        .O(cal_tmp_carry__2_i_4_n_1));
  CARRY4 cal_tmp_carry__3
       (.CI(cal_tmp_carry__2_n_1),
        .CO({cal_tmp_carry__3_n_1,cal_tmp_carry__3_n_2,cal_tmp_carry__3_n_3,cal_tmp_carry__3_n_4}),
        .CYINIT(1'b0),
        .DI({1'b1,1'b1,1'b1,1'b1}),
        .O({cal_tmp_carry__3_n_5,cal_tmp_carry__3_n_6,cal_tmp_carry__3_n_7,cal_tmp_carry__3_n_8}),
        .S({cal_tmp_carry__3_i_1_n_1,cal_tmp_carry__3_i_2_n_1,cal_tmp_carry__3_i_3_n_1,cal_tmp_carry__3_i_4_n_1}));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__3_i_1
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[18] ),
        .O(cal_tmp_carry__3_i_1_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__3_i_2
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[17] ),
        .O(cal_tmp_carry__3_i_2_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__3_i_3
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[16] ),
        .O(cal_tmp_carry__3_i_3_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__3_i_4
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[15] ),
        .O(cal_tmp_carry__3_i_4_n_1));
  CARRY4 cal_tmp_carry__4
       (.CI(cal_tmp_carry__3_n_1),
        .CO({cal_tmp_carry__4_n_1,cal_tmp_carry__4_n_2,cal_tmp_carry__4_n_3,cal_tmp_carry__4_n_4}),
        .CYINIT(1'b0),
        .DI({1'b1,1'b1,1'b1,1'b1}),
        .O({cal_tmp_carry__4_n_5,cal_tmp_carry__4_n_6,cal_tmp_carry__4_n_7,cal_tmp_carry__4_n_8}),
        .S({cal_tmp_carry__4_i_1_n_1,cal_tmp_carry__4_i_2_n_1,cal_tmp_carry__4_i_3_n_1,cal_tmp_carry__4_i_4_n_1}));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__4_i_1
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[22] ),
        .O(cal_tmp_carry__4_i_1_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__4_i_2
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[21] ),
        .O(cal_tmp_carry__4_i_2_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__4_i_3
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[20] ),
        .O(cal_tmp_carry__4_i_3_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__4_i_4
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[19] ),
        .O(cal_tmp_carry__4_i_4_n_1));
  CARRY4 cal_tmp_carry__5
       (.CI(cal_tmp_carry__4_n_1),
        .CO({cal_tmp_carry__5_n_1,cal_tmp_carry__5_n_2,cal_tmp_carry__5_n_3,cal_tmp_carry__5_n_4}),
        .CYINIT(1'b0),
        .DI({1'b1,1'b1,1'b1,1'b1}),
        .O({cal_tmp_carry__5_n_5,cal_tmp_carry__5_n_6,cal_tmp_carry__5_n_7,cal_tmp_carry__5_n_8}),
        .S({cal_tmp_carry__5_i_1_n_1,cal_tmp_carry__5_i_2_n_1,cal_tmp_carry__5_i_3_n_1,cal_tmp_carry__5_i_4_n_1}));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__5_i_1
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[26] ),
        .O(cal_tmp_carry__5_i_1_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__5_i_2
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[25] ),
        .O(cal_tmp_carry__5_i_2_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__5_i_3
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[24] ),
        .O(cal_tmp_carry__5_i_3_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__5_i_4
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[23] ),
        .O(cal_tmp_carry__5_i_4_n_1));
  CARRY4 cal_tmp_carry__6
       (.CI(cal_tmp_carry__5_n_1),
        .CO({p_2_out,cal_tmp_carry__6_n_2,cal_tmp_carry__6_n_3,cal_tmp_carry__6_n_4}),
        .CYINIT(1'b0),
        .DI({1'b1,1'b1,1'b1,1'b1}),
        .O({cal_tmp_carry__6_n_5,cal_tmp_carry__6_n_6,cal_tmp_carry__6_n_7,cal_tmp_carry__6_n_8}),
        .S({cal_tmp_carry__6_i_1_n_1,cal_tmp_carry__6_i_2_n_1,cal_tmp_carry__6_i_3_n_1,cal_tmp_carry__6_i_4_n_1}));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__6_i_1
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[30] ),
        .O(cal_tmp_carry__6_i_1_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__6_i_2
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[29] ),
        .O(cal_tmp_carry__6_i_2_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__6_i_3
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[28] ),
        .O(cal_tmp_carry__6_i_3_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry__6_i_4
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[27] ),
        .O(cal_tmp_carry__6_i_4_n_1));
  CARRY4 cal_tmp_carry__7
       (.CI(p_2_out),
        .CO(NLW_cal_tmp_carry__7_CO_UNCONNECTED[3:0]),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({NLW_cal_tmp_carry__7_O_UNCONNECTED[3:1],p_0_in}),
        .S({1'b0,1'b0,1'b0,1'b1}));
  LUT3 #(
    .INIT(8'hAC)) 
    cal_tmp_carry_i_1
       (.I0(\dividend0_reg_n_1_[31] ),
        .I1(dividend_tmp[31]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(p_1_in0));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry_i_2
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[2] ),
        .O(cal_tmp_carry_i_2_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry_i_3
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[1] ),
        .O(cal_tmp_carry_i_3_n_1));
  LUT2 #(
    .INIT(4'hB)) 
    cal_tmp_carry_i_4
       (.I0(\r_stage_reg_n_1_[0] ),
        .I1(\remd_tmp_reg_n_1_[0] ),
        .O(cal_tmp_carry_i_4_n_1));
  LUT3 #(
    .INIT(8'hAC)) 
    cal_tmp_carry_i_5
       (.I0(\dividend0_reg_n_1_[31] ),
        .I1(dividend_tmp[31]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(cal_tmp_carry_i_5_n_1));
  FDRE \dividend0_reg[0] 
       (.C(ap_clk),
        .CE(E),
        .D(Q[0]),
        .Q(\dividend0_reg_n_1_[0] ),
        .R(1'b0));
  FDRE \dividend0_reg[10] 
       (.C(ap_clk),
        .CE(E),
        .D(D[9]),
        .Q(\dividend0_reg_n_1_[10] ),
        .R(1'b0));
  FDRE \dividend0_reg[11] 
       (.C(ap_clk),
        .CE(E),
        .D(D[10]),
        .Q(\dividend0_reg_n_1_[11] ),
        .R(1'b0));
  FDRE \dividend0_reg[12] 
       (.C(ap_clk),
        .CE(E),
        .D(D[11]),
        .Q(\dividend0_reg_n_1_[12] ),
        .R(1'b0));
  FDRE \dividend0_reg[13] 
       (.C(ap_clk),
        .CE(E),
        .D(D[12]),
        .Q(\dividend0_reg_n_1_[13] ),
        .R(1'b0));
  FDRE \dividend0_reg[14] 
       (.C(ap_clk),
        .CE(E),
        .D(D[13]),
        .Q(\dividend0_reg_n_1_[14] ),
        .R(1'b0));
  FDRE \dividend0_reg[15] 
       (.C(ap_clk),
        .CE(E),
        .D(D[14]),
        .Q(\dividend0_reg_n_1_[15] ),
        .R(1'b0));
  FDRE \dividend0_reg[16] 
       (.C(ap_clk),
        .CE(E),
        .D(D[15]),
        .Q(\dividend0_reg_n_1_[16] ),
        .R(1'b0));
  FDRE \dividend0_reg[17] 
       (.C(ap_clk),
        .CE(E),
        .D(D[16]),
        .Q(\dividend0_reg_n_1_[17] ),
        .R(1'b0));
  FDRE \dividend0_reg[18] 
       (.C(ap_clk),
        .CE(E),
        .D(D[17]),
        .Q(\dividend0_reg_n_1_[18] ),
        .R(1'b0));
  FDRE \dividend0_reg[19] 
       (.C(ap_clk),
        .CE(E),
        .D(D[18]),
        .Q(\dividend0_reg_n_1_[19] ),
        .R(1'b0));
  FDRE \dividend0_reg[1] 
       (.C(ap_clk),
        .CE(E),
        .D(D[0]),
        .Q(\dividend0_reg_n_1_[1] ),
        .R(1'b0));
  FDRE \dividend0_reg[20] 
       (.C(ap_clk),
        .CE(E),
        .D(D[19]),
        .Q(\dividend0_reg_n_1_[20] ),
        .R(1'b0));
  FDRE \dividend0_reg[21] 
       (.C(ap_clk),
        .CE(E),
        .D(D[20]),
        .Q(\dividend0_reg_n_1_[21] ),
        .R(1'b0));
  FDRE \dividend0_reg[22] 
       (.C(ap_clk),
        .CE(E),
        .D(D[21]),
        .Q(\dividend0_reg_n_1_[22] ),
        .R(1'b0));
  FDRE \dividend0_reg[23] 
       (.C(ap_clk),
        .CE(E),
        .D(D[22]),
        .Q(\dividend0_reg_n_1_[23] ),
        .R(1'b0));
  FDRE \dividend0_reg[24] 
       (.C(ap_clk),
        .CE(E),
        .D(D[23]),
        .Q(\dividend0_reg_n_1_[24] ),
        .R(1'b0));
  FDRE \dividend0_reg[25] 
       (.C(ap_clk),
        .CE(E),
        .D(D[24]),
        .Q(\dividend0_reg_n_1_[25] ),
        .R(1'b0));
  FDRE \dividend0_reg[26] 
       (.C(ap_clk),
        .CE(E),
        .D(D[25]),
        .Q(\dividend0_reg_n_1_[26] ),
        .R(1'b0));
  FDRE \dividend0_reg[27] 
       (.C(ap_clk),
        .CE(E),
        .D(D[26]),
        .Q(\dividend0_reg_n_1_[27] ),
        .R(1'b0));
  FDRE \dividend0_reg[28] 
       (.C(ap_clk),
        .CE(E),
        .D(D[27]),
        .Q(\dividend0_reg_n_1_[28] ),
        .R(1'b0));
  FDRE \dividend0_reg[29] 
       (.C(ap_clk),
        .CE(E),
        .D(D[28]),
        .Q(\dividend0_reg_n_1_[29] ),
        .R(1'b0));
  FDRE \dividend0_reg[2] 
       (.C(ap_clk),
        .CE(E),
        .D(D[1]),
        .Q(\dividend0_reg_n_1_[2] ),
        .R(1'b0));
  FDRE \dividend0_reg[30] 
       (.C(ap_clk),
        .CE(E),
        .D(D[29]),
        .Q(\dividend0_reg_n_1_[30] ),
        .R(1'b0));
  FDRE \dividend0_reg[31] 
       (.C(ap_clk),
        .CE(E),
        .D(D[30]),
        .Q(\dividend0_reg_n_1_[31] ),
        .R(1'b0));
  FDRE \dividend0_reg[3] 
       (.C(ap_clk),
        .CE(E),
        .D(D[2]),
        .Q(\dividend0_reg_n_1_[3] ),
        .R(1'b0));
  FDRE \dividend0_reg[4] 
       (.C(ap_clk),
        .CE(E),
        .D(D[3]),
        .Q(\dividend0_reg_n_1_[4] ),
        .R(1'b0));
  FDRE \dividend0_reg[5] 
       (.C(ap_clk),
        .CE(E),
        .D(D[4]),
        .Q(\dividend0_reg_n_1_[5] ),
        .R(1'b0));
  FDRE \dividend0_reg[6] 
       (.C(ap_clk),
        .CE(E),
        .D(D[5]),
        .Q(\dividend0_reg_n_1_[6] ),
        .R(1'b0));
  FDRE \dividend0_reg[7] 
       (.C(ap_clk),
        .CE(E),
        .D(D[6]),
        .Q(\dividend0_reg_n_1_[7] ),
        .R(1'b0));
  FDRE \dividend0_reg[8] 
       (.C(ap_clk),
        .CE(E),
        .D(D[7]),
        .Q(\dividend0_reg_n_1_[8] ),
        .R(1'b0));
  FDRE \dividend0_reg[9] 
       (.C(ap_clk),
        .CE(E),
        .D(D[8]),
        .Q(\dividend0_reg_n_1_[9] ),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[10]_i_1 
       (.I0(\dividend0_reg_n_1_[9] ),
        .I1(dividend_tmp[9]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[10]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[11]_i_1 
       (.I0(\dividend0_reg_n_1_[10] ),
        .I1(dividend_tmp[10]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[11]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[12]_i_1 
       (.I0(\dividend0_reg_n_1_[11] ),
        .I1(dividend_tmp[11]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[12]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[13]_i_1 
       (.I0(\dividend0_reg_n_1_[12] ),
        .I1(dividend_tmp[12]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[13]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[14]_i_1 
       (.I0(\dividend0_reg_n_1_[13] ),
        .I1(dividend_tmp[13]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[14]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[15]_i_1 
       (.I0(\dividend0_reg_n_1_[14] ),
        .I1(dividend_tmp[14]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[15]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[16]_i_1 
       (.I0(\dividend0_reg_n_1_[15] ),
        .I1(dividend_tmp[15]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[16]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[17]_i_1 
       (.I0(\dividend0_reg_n_1_[16] ),
        .I1(dividend_tmp[16]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[17]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[18]_i_1 
       (.I0(\dividend0_reg_n_1_[17] ),
        .I1(dividend_tmp[17]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[18]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[19]_i_1 
       (.I0(\dividend0_reg_n_1_[18] ),
        .I1(dividend_tmp[18]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[19]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[1]_i_1 
       (.I0(\dividend0_reg_n_1_[0] ),
        .I1(dividend_tmp[0]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[1]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair10" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[20]_i_1 
       (.I0(\dividend0_reg_n_1_[19] ),
        .I1(dividend_tmp[19]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[20]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair10" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[21]_i_1 
       (.I0(\dividend0_reg_n_1_[20] ),
        .I1(dividend_tmp[20]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[21]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair11" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[22]_i_1 
       (.I0(\dividend0_reg_n_1_[21] ),
        .I1(dividend_tmp[21]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[22]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair11" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[23]_i_1 
       (.I0(\dividend0_reg_n_1_[22] ),
        .I1(dividend_tmp[22]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[23]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair12" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[24]_i_1 
       (.I0(\dividend0_reg_n_1_[23] ),
        .I1(dividend_tmp[23]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[24]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair12" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[25]_i_1 
       (.I0(\dividend0_reg_n_1_[24] ),
        .I1(dividend_tmp[24]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[25]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair13" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[26]_i_1 
       (.I0(\dividend0_reg_n_1_[25] ),
        .I1(dividend_tmp[25]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[26]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair13" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[27]_i_1 
       (.I0(\dividend0_reg_n_1_[26] ),
        .I1(dividend_tmp[26]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[27]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair14" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[28]_i_1 
       (.I0(\dividend0_reg_n_1_[27] ),
        .I1(dividend_tmp[27]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[28]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair14" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[29]_i_1 
       (.I0(\dividend0_reg_n_1_[28] ),
        .I1(dividend_tmp[28]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[29]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[2]_i_1 
       (.I0(\dividend0_reg_n_1_[1] ),
        .I1(dividend_tmp[1]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[2]_i_1_n_1 ));
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[30]_i_1 
       (.I0(\dividend0_reg_n_1_[29] ),
        .I1(dividend_tmp[29]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[30]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[31]_i_1 
       (.I0(\dividend0_reg_n_1_[30] ),
        .I1(dividend_tmp[30]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[31]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[3]_i_1 
       (.I0(\dividend0_reg_n_1_[2] ),
        .I1(dividend_tmp[2]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[3]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[4]_i_1 
       (.I0(\dividend0_reg_n_1_[3] ),
        .I1(dividend_tmp[3]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[4]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[5]_i_1 
       (.I0(\dividend0_reg_n_1_[4] ),
        .I1(dividend_tmp[4]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[5]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[6]_i_1 
       (.I0(\dividend0_reg_n_1_[5] ),
        .I1(dividend_tmp[5]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[6]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[7]_i_1 
       (.I0(\dividend0_reg_n_1_[6] ),
        .I1(dividend_tmp[6]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[7]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[8]_i_1 
       (.I0(\dividend0_reg_n_1_[7] ),
        .I1(dividend_tmp[7]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[8]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT3 #(
    .INIT(8'hAC)) 
    \dividend_tmp[9]_i_1 
       (.I0(\dividend0_reg_n_1_[8] ),
        .I1(dividend_tmp[8]),
        .I2(\r_stage_reg_n_1_[0] ),
        .O(\dividend_tmp[9]_i_1_n_1 ));
  FDRE \dividend_tmp_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(p_2_out),
        .Q(dividend_tmp[0]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[10] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[10]_i_1_n_1 ),
        .Q(dividend_tmp[10]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[11] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[11]_i_1_n_1 ),
        .Q(dividend_tmp[11]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[12] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[12]_i_1_n_1 ),
        .Q(dividend_tmp[12]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[13] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[13]_i_1_n_1 ),
        .Q(dividend_tmp[13]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[14] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[14]_i_1_n_1 ),
        .Q(dividend_tmp[14]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[15] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[15]_i_1_n_1 ),
        .Q(dividend_tmp[15]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[16] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[16]_i_1_n_1 ),
        .Q(dividend_tmp[16]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[17] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[17]_i_1_n_1 ),
        .Q(dividend_tmp[17]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[18] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[18]_i_1_n_1 ),
        .Q(dividend_tmp[18]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[19] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[19]_i_1_n_1 ),
        .Q(dividend_tmp[19]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[1] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[1]_i_1_n_1 ),
        .Q(dividend_tmp[1]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[20] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[20]_i_1_n_1 ),
        .Q(dividend_tmp[20]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[21] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[21]_i_1_n_1 ),
        .Q(dividend_tmp[21]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[22] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[22]_i_1_n_1 ),
        .Q(dividend_tmp[22]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[23] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[23]_i_1_n_1 ),
        .Q(dividend_tmp[23]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[24] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[24]_i_1_n_1 ),
        .Q(dividend_tmp[24]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[25] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[25]_i_1_n_1 ),
        .Q(dividend_tmp[25]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[26] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[26]_i_1_n_1 ),
        .Q(dividend_tmp[26]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[27] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[27]_i_1_n_1 ),
        .Q(dividend_tmp[27]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[28] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[28]_i_1_n_1 ),
        .Q(dividend_tmp[28]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[29] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[29]_i_1_n_1 ),
        .Q(dividend_tmp[29]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[2]_i_1_n_1 ),
        .Q(dividend_tmp[2]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[30] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[30]_i_1_n_1 ),
        .Q(dividend_tmp[30]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[31] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[31]_i_1_n_1 ),
        .Q(dividend_tmp[31]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[3] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[3]_i_1_n_1 ),
        .Q(dividend_tmp[3]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[4] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[4]_i_1_n_1 ),
        .Q(dividend_tmp[4]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[5] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[5]_i_1_n_1 ),
        .Q(dividend_tmp[5]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[6] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[6]_i_1_n_1 ),
        .Q(dividend_tmp[6]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[7] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[7]_i_1_n_1 ),
        .Q(dividend_tmp[7]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[8] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[8]_i_1_n_1 ),
        .Q(dividend_tmp[8]),
        .R(1'b0));
  FDRE \dividend_tmp_reg[9] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\dividend_tmp[9]_i_1_n_1 ),
        .Q(dividend_tmp[9]),
        .R(1'b0));
  FDRE \r_stage_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(E),
        .Q(\r_stage_reg_n_1_[0] ),
        .R(ap_rst));
  (* srl_bus_name = "inst/\loop_imperfect_srbkb_U1/loop_imperfect_srbkb_div_U/loop_imperfect_srbkb_div_u_0/r_stage_reg " *) 
  (* srl_name = "inst/\loop_imperfect_srbkb_U1/loop_imperfect_srbkb_div_U/loop_imperfect_srbkb_div_u_0/r_stage_reg[30]_srl30___loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_28 " *) 
  SRLC32E \r_stage_reg[30]_srl30___loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_28 
       (.A({1'b1,1'b1,1'b1,1'b0,1'b1}),
        .CE(1'b1),
        .CLK(ap_clk),
        .D(\r_stage_reg_n_1_[0] ),
        .Q(\r_stage_reg[30]_srl30___loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_28_n_1 ),
        .Q31(\NLW_r_stage_reg[30]_srl30___loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_28_Q31_UNCONNECTED ));
  FDRE \r_stage_reg[31]_loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_29 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\r_stage_reg[30]_srl30___loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_28_n_1 ),
        .Q(\r_stage_reg[31]_loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_29_n_1 ),
        .R(1'b0));
  FDRE \r_stage_reg[32] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_gate_n_1),
        .Q(\r_stage_reg[32]_0 ),
        .R(ap_rst));
  LUT2 #(
    .INIT(4'h8)) 
    r_stage_reg_gate
       (.I0(\r_stage_reg[31]_loop_imperfect_srbkb_U1_loop_imperfect_srbkb_div_U_loop_imperfect_srbkb_div_u_0_r_stage_reg_r_29_n_1 ),
        .I1(r_stage_reg_r_29_0),
        .O(r_stage_reg_gate_n_1));
  FDRE r_stage_reg_r
       (.C(ap_clk),
        .CE(1'b1),
        .D(1'b1),
        .Q(r_stage_reg_r_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_0
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_n_1),
        .Q(r_stage_reg_r_0_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_1
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_0_n_1),
        .Q(r_stage_reg_r_1_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_10
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_9_n_1),
        .Q(r_stage_reg_r_10_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_11
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_10_n_1),
        .Q(r_stage_reg_r_11_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_12
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_11_n_1),
        .Q(r_stage_reg_r_12_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_13
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_12_n_1),
        .Q(r_stage_reg_r_13_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_14
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_13_n_1),
        .Q(r_stage_reg_r_14_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_15
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_14_n_1),
        .Q(r_stage_reg_r_15_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_16
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_15_n_1),
        .Q(r_stage_reg_r_16_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_17
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_16_n_1),
        .Q(r_stage_reg_r_17_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_18
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_17_n_1),
        .Q(r_stage_reg_r_18_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_19
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_18_n_1),
        .Q(r_stage_reg_r_19_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_2
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_1_n_1),
        .Q(r_stage_reg_r_2_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_20
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_19_n_1),
        .Q(r_stage_reg_r_20_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_21
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_20_n_1),
        .Q(r_stage_reg_r_21_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_22
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_21_n_1),
        .Q(r_stage_reg_r_22_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_23
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_22_n_1),
        .Q(r_stage_reg_r_23_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_24
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_23_n_1),
        .Q(r_stage_reg_r_24_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_25
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_24_n_1),
        .Q(r_stage_reg_r_25_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_26
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_25_n_1),
        .Q(r_stage_reg_r_26_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_27
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_26_n_1),
        .Q(r_stage_reg_r_27_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_28
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_27_n_1),
        .Q(r_stage_reg_r_28_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_29
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_28_n_1),
        .Q(r_stage_reg_r_29_0),
        .R(ap_rst));
  FDRE r_stage_reg_r_3
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_2_n_1),
        .Q(r_stage_reg_r_3_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_4
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_3_n_1),
        .Q(r_stage_reg_r_4_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_5
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_4_n_1),
        .Q(r_stage_reg_r_5_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_6
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_5_n_1),
        .Q(r_stage_reg_r_6_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_7
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_6_n_1),
        .Q(r_stage_reg_r_7_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_8
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_7_n_1),
        .Q(r_stage_reg_r_8_n_1),
        .R(ap_rst));
  FDRE r_stage_reg_r_9
       (.C(ap_clk),
        .CE(1'b1),
        .D(r_stage_reg_r_8_n_1),
        .Q(r_stage_reg_r_9_n_1),
        .R(ap_rst));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[11]_i_2 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[11] ),
        .O(\remd[11]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[11]_i_3 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[10] ),
        .O(\remd[11]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[11]_i_4 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[9] ),
        .O(\remd[11]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[11]_i_5 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[8] ),
        .O(\remd[11]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[15]_i_2 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[15] ),
        .O(\remd[15]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[15]_i_3 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[14] ),
        .O(\remd[15]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[15]_i_4 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[13] ),
        .O(\remd[15]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[15]_i_5 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[12] ),
        .O(\remd[15]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[19]_i_2 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[19] ),
        .O(\remd[19]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[19]_i_3 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[18] ),
        .O(\remd[19]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[19]_i_4 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[17] ),
        .O(\remd[19]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[19]_i_5 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[16] ),
        .O(\remd[19]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[23]_i_2 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[23] ),
        .O(\remd[23]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[23]_i_3 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[22] ),
        .O(\remd[23]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[23]_i_4 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[21] ),
        .O(\remd[23]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[23]_i_5 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[20] ),
        .O(\remd[23]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[27]_i_2 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[27] ),
        .O(\remd[27]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[27]_i_3 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[26] ),
        .O(\remd[27]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[27]_i_4 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[25] ),
        .O(\remd[27]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[27]_i_5 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[24] ),
        .O(\remd[27]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[31]_i_2 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[31] ),
        .O(\remd[31]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[31]_i_3 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[30] ),
        .O(\remd[31]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[31]_i_4 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[29] ),
        .O(\remd[31]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[31]_i_5 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[28] ),
        .O(\remd[31]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[3]_i_2 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[3] ),
        .O(\remd[3]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[3]_i_3 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[2] ),
        .O(\remd[3]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[3]_i_4 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[1] ),
        .O(\remd[3]_i_4_n_1 ));
  LUT1 #(
    .INIT(2'h2)) 
    \remd[3]_i_5 
       (.I0(\remd_tmp_reg_n_1_[0] ),
        .O(\remd[3]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[7]_i_2 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[7] ),
        .O(\remd[7]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[7]_i_3 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[6] ),
        .O(\remd[7]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[7]_i_4 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[5] ),
        .O(\remd[7]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \remd[7]_i_5 
       (.I0(sign0),
        .I1(\remd_tmp_reg_n_1_[4] ),
        .O(\remd[7]_i_5_n_1 ));
  CARRY4 \remd_reg[11]_i_1 
       (.CI(\remd_reg[7]_i_1_n_1 ),
        .CO({\remd_reg[11]_i_1_n_1 ,\remd_reg[11]_i_1_n_2 ,\remd_reg[11]_i_1_n_3 ,\remd_reg[11]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(O7[11:8]),
        .S({\remd[11]_i_2_n_1 ,\remd[11]_i_3_n_1 ,\remd[11]_i_4_n_1 ,\remd[11]_i_5_n_1 }));
  CARRY4 \remd_reg[15]_i_1 
       (.CI(\remd_reg[11]_i_1_n_1 ),
        .CO({\remd_reg[15]_i_1_n_1 ,\remd_reg[15]_i_1_n_2 ,\remd_reg[15]_i_1_n_3 ,\remd_reg[15]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(O7[15:12]),
        .S({\remd[15]_i_2_n_1 ,\remd[15]_i_3_n_1 ,\remd[15]_i_4_n_1 ,\remd[15]_i_5_n_1 }));
  CARRY4 \remd_reg[19]_i_1 
       (.CI(\remd_reg[15]_i_1_n_1 ),
        .CO({\remd_reg[19]_i_1_n_1 ,\remd_reg[19]_i_1_n_2 ,\remd_reg[19]_i_1_n_3 ,\remd_reg[19]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(O7[19:16]),
        .S({\remd[19]_i_2_n_1 ,\remd[19]_i_3_n_1 ,\remd[19]_i_4_n_1 ,\remd[19]_i_5_n_1 }));
  CARRY4 \remd_reg[23]_i_1 
       (.CI(\remd_reg[19]_i_1_n_1 ),
        .CO({\remd_reg[23]_i_1_n_1 ,\remd_reg[23]_i_1_n_2 ,\remd_reg[23]_i_1_n_3 ,\remd_reg[23]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(O7[23:20]),
        .S({\remd[23]_i_2_n_1 ,\remd[23]_i_3_n_1 ,\remd[23]_i_4_n_1 ,\remd[23]_i_5_n_1 }));
  CARRY4 \remd_reg[27]_i_1 
       (.CI(\remd_reg[23]_i_1_n_1 ),
        .CO({\remd_reg[27]_i_1_n_1 ,\remd_reg[27]_i_1_n_2 ,\remd_reg[27]_i_1_n_3 ,\remd_reg[27]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(O7[27:24]),
        .S({\remd[27]_i_2_n_1 ,\remd[27]_i_3_n_1 ,\remd[27]_i_4_n_1 ,\remd[27]_i_5_n_1 }));
  CARRY4 \remd_reg[31]_i_1 
       (.CI(\remd_reg[27]_i_1_n_1 ),
        .CO({\NLW_remd_reg[31]_i_1_CO_UNCONNECTED [3],\remd_reg[31]_i_1_n_2 ,\remd_reg[31]_i_1_n_3 ,\remd_reg[31]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(O7[31:28]),
        .S({\remd[31]_i_2_n_1 ,\remd[31]_i_3_n_1 ,\remd[31]_i_4_n_1 ,\remd[31]_i_5_n_1 }));
  CARRY4 \remd_reg[3]_i_1 
       (.CI(1'b0),
        .CO({\remd_reg[3]_i_1_n_1 ,\remd_reg[3]_i_1_n_2 ,\remd_reg[3]_i_1_n_3 ,\remd_reg[3]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,sign0}),
        .O(O7[3:0]),
        .S({\remd[3]_i_2_n_1 ,\remd[3]_i_3_n_1 ,\remd[3]_i_4_n_1 ,\remd[3]_i_5_n_1 }));
  CARRY4 \remd_reg[7]_i_1 
       (.CI(\remd_reg[3]_i_1_n_1 ),
        .CO({\remd_reg[7]_i_1_n_1 ,\remd_reg[7]_i_1_n_2 ,\remd_reg[7]_i_1_n_3 ,\remd_reg[7]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(O7[7:4]),
        .S({\remd[7]_i_2_n_1 ,\remd[7]_i_3_n_1 ,\remd[7]_i_4_n_1 ,\remd[7]_i_5_n_1 }));
  LUT5 #(
    .INIT(32'hACFFAC00)) 
    \remd_tmp[0]_i_1 
       (.I0(\dividend0_reg_n_1_[31] ),
        .I1(dividend_tmp[31]),
        .I2(\r_stage_reg_n_1_[0] ),
        .I3(p_0_in),
        .I4(cal_tmp_carry_n_8),
        .O(\remd_tmp[0]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[10]_i_1 
       (.I0(\remd_tmp_reg_n_1_[9] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__1_n_6),
        .O(\remd_tmp[10]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[11]_i_1 
       (.I0(\remd_tmp_reg_n_1_[10] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__1_n_5),
        .O(\remd_tmp[11]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[12]_i_1 
       (.I0(\remd_tmp_reg_n_1_[11] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__2_n_8),
        .O(\remd_tmp[12]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[13]_i_1 
       (.I0(\remd_tmp_reg_n_1_[12] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__2_n_7),
        .O(\remd_tmp[13]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[14]_i_1 
       (.I0(\remd_tmp_reg_n_1_[13] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__2_n_6),
        .O(\remd_tmp[14]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[15]_i_1 
       (.I0(\remd_tmp_reg_n_1_[14] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__2_n_5),
        .O(\remd_tmp[15]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[16]_i_1 
       (.I0(\remd_tmp_reg_n_1_[15] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__3_n_8),
        .O(\remd_tmp[16]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[17]_i_1 
       (.I0(\remd_tmp_reg_n_1_[16] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__3_n_7),
        .O(\remd_tmp[17]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[18]_i_1 
       (.I0(\remd_tmp_reg_n_1_[17] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__3_n_6),
        .O(\remd_tmp[18]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[19]_i_1 
       (.I0(\remd_tmp_reg_n_1_[18] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__3_n_5),
        .O(\remd_tmp[19]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[1]_i_1 
       (.I0(\remd_tmp_reg_n_1_[0] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry_n_7),
        .O(\remd_tmp[1]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[20]_i_1 
       (.I0(\remd_tmp_reg_n_1_[19] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__4_n_8),
        .O(\remd_tmp[20]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[21]_i_1 
       (.I0(\remd_tmp_reg_n_1_[20] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__4_n_7),
        .O(\remd_tmp[21]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[22]_i_1 
       (.I0(\remd_tmp_reg_n_1_[21] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__4_n_6),
        .O(\remd_tmp[22]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[23]_i_1 
       (.I0(\remd_tmp_reg_n_1_[22] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__4_n_5),
        .O(\remd_tmp[23]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[24]_i_1 
       (.I0(\remd_tmp_reg_n_1_[23] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__5_n_8),
        .O(\remd_tmp[24]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[25]_i_1 
       (.I0(\remd_tmp_reg_n_1_[24] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__5_n_7),
        .O(\remd_tmp[25]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[26]_i_1 
       (.I0(\remd_tmp_reg_n_1_[25] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__5_n_6),
        .O(\remd_tmp[26]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[27]_i_1 
       (.I0(\remd_tmp_reg_n_1_[26] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__5_n_5),
        .O(\remd_tmp[27]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[28]_i_1 
       (.I0(\remd_tmp_reg_n_1_[27] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__6_n_8),
        .O(\remd_tmp[28]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[29]_i_1 
       (.I0(\remd_tmp_reg_n_1_[28] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__6_n_7),
        .O(\remd_tmp[29]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[2]_i_1 
       (.I0(\remd_tmp_reg_n_1_[1] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry_n_6),
        .O(\remd_tmp[2]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[30]_i_1 
       (.I0(\remd_tmp_reg_n_1_[29] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__6_n_6),
        .O(\remd_tmp[30]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[31]_i_1 
       (.I0(\remd_tmp_reg_n_1_[30] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__6_n_5),
        .O(\remd_tmp[31]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[3]_i_1 
       (.I0(\remd_tmp_reg_n_1_[2] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry_n_5),
        .O(\remd_tmp[3]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[4]_i_1 
       (.I0(\remd_tmp_reg_n_1_[3] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__0_n_8),
        .O(\remd_tmp[4]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[5]_i_1 
       (.I0(\remd_tmp_reg_n_1_[4] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__0_n_7),
        .O(\remd_tmp[5]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[6]_i_1 
       (.I0(\remd_tmp_reg_n_1_[5] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__0_n_6),
        .O(\remd_tmp[6]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[7]_i_1 
       (.I0(\remd_tmp_reg_n_1_[6] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__0_n_5),
        .O(\remd_tmp[7]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[8]_i_1 
       (.I0(\remd_tmp_reg_n_1_[7] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__1_n_8),
        .O(\remd_tmp[8]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h2F20)) 
    \remd_tmp[9]_i_1 
       (.I0(\remd_tmp_reg_n_1_[8] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .I2(p_0_in),
        .I3(cal_tmp_carry__1_n_7),
        .O(\remd_tmp[9]_i_1_n_1 ));
  LUT2 #(
    .INIT(4'h2)) 
    remd_tmp_mux
       (.I0(\remd_tmp_reg_n_1_[3] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .O(remd_tmp_mux__0));
  LUT2 #(
    .INIT(4'h2)) 
    remd_tmp_mux_rep
       (.I0(\remd_tmp_reg_n_1_[3] ),
        .I1(\r_stage_reg_n_1_[0] ),
        .O(remd_tmp_mux_rep_n_1));
  FDRE \remd_tmp_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[0]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[0] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[10] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[10]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[10] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[11] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[11]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[11] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[12] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[12]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[12] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[13] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[13]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[13] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[14] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[14]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[14] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[15] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[15]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[15] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[16] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[16]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[16] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[17] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[17]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[17] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[18] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[18]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[18] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[19] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[19]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[19] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[1] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[1]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[1] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[20] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[20]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[20] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[21] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[21]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[21] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[22] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[22]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[22] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[23] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[23]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[23] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[24] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[24]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[24] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[25] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[25]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[25] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[26] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[26]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[26] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[27] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[27]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[27] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[28] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[28]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[28] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[29] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[29]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[29] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[2]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[2] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[30] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[30]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[30] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[31] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[31]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[31] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[3] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[3]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[3] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[4] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[4]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[4] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[5] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[5]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[5] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[6] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[6]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[6] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[7] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[7]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[7] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[8] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[8]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[8] ),
        .R(1'b0));
  FDRE \remd_tmp_reg[9] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\remd_tmp[9]_i_1_n_1 ),
        .Q(\remd_tmp_reg_n_1_[9] ),
        .R(1'b0));
  FDRE \sign0_reg[0] 
       (.C(ap_clk),
        .CE(E),
        .D(Q[1]),
        .Q(sign0),
        .R(1'b0));
endmodule
`ifndef GLBL
`define GLBL
`timescale  1 ps / 1 ps

module glbl ();

    parameter ROC_WIDTH = 100000;
    parameter TOC_WIDTH = 0;

//--------   STARTUP Globals --------------
    wire GSR;
    wire GTS;
    wire GWE;
    wire PRLD;
    tri1 p_up_tmp;
    tri (weak1, strong0) PLL_LOCKG = p_up_tmp;

    wire PROGB_GLBL;
    wire CCLKO_GLBL;
    wire FCSBO_GLBL;
    wire [3:0] DO_GLBL;
    wire [3:0] DI_GLBL;
   
    reg GSR_int;
    reg GTS_int;
    reg PRLD_int;

//--------   JTAG Globals --------------
    wire JTAG_TDO_GLBL;
    wire JTAG_TCK_GLBL;
    wire JTAG_TDI_GLBL;
    wire JTAG_TMS_GLBL;
    wire JTAG_TRST_GLBL;

    reg JTAG_CAPTURE_GLBL;
    reg JTAG_RESET_GLBL;
    reg JTAG_SHIFT_GLBL;
    reg JTAG_UPDATE_GLBL;
    reg JTAG_RUNTEST_GLBL;

    reg JTAG_SEL1_GLBL = 0;
    reg JTAG_SEL2_GLBL = 0 ;
    reg JTAG_SEL3_GLBL = 0;
    reg JTAG_SEL4_GLBL = 0;

    reg JTAG_USER_TDO1_GLBL = 1'bz;
    reg JTAG_USER_TDO2_GLBL = 1'bz;
    reg JTAG_USER_TDO3_GLBL = 1'bz;
    reg JTAG_USER_TDO4_GLBL = 1'bz;

    assign (strong1, weak0) GSR = GSR_int;
    assign (strong1, weak0) GTS = GTS_int;
    assign (weak1, weak0) PRLD = PRLD_int;

    initial begin
	GSR_int = 1'b1;
	PRLD_int = 1'b1;
	#(ROC_WIDTH)
	GSR_int = 1'b0;
	PRLD_int = 1'b0;
    end

    initial begin
	GTS_int = 1'b1;
	#(TOC_WIDTH)
	GTS_int = 1'b0;
    end

endmodule
`endif
