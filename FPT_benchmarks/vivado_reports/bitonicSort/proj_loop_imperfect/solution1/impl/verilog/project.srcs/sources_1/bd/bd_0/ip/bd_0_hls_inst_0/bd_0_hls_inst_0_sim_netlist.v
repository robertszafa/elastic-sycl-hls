// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
// Date        : Tue Oct 24 09:50:50 2023
// Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
// Command     : write_verilog -force -mode funcsim
//               /home/dynamatic/bitonicSort/proj_loop_imperfect/solution1/impl/verilog/project.srcs/sources_1/bd/bd_0/ip/bd_0_hls_inst_0/bd_0_hls_inst_0_sim_netlist.v
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
   (A_ce0,
    A_we0,
    A_ce1,
    A_we1,
    ap_clk,
    ap_rst,
    ap_start,
    ap_done,
    ap_idle,
    ap_ready,
    A_address0,
    A_d0,
    A_q0,
    A_address1,
    A_d1,
    A_q1);
  output A_ce0;
  output A_we0;
  output A_ce1;
  output A_we1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 ap_clk CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ap_clk, ASSOCIATED_RESET ap_rst, FREQ_HZ 250000000.0, PHASE 0.000, CLK_DOMAIN bd_0_ap_clk_0, INSERT_VIP 0" *) input ap_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 ap_rst RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ap_rst, POLARITY ACTIVE_HIGH, INSERT_VIP 0" *) input ap_rst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl start" *) input ap_start;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl done" *) output ap_done;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl idle" *) output ap_idle;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl ready" *) output ap_ready;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 A_address0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME A_address0, LAYERED_METADATA undef" *) output [5:0]A_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 A_d0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME A_d0, LAYERED_METADATA undef" *) output [31:0]A_d0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 A_q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME A_q0, LAYERED_METADATA undef" *) input [31:0]A_q0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 A_address1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME A_address1, LAYERED_METADATA undef" *) output [5:0]A_address1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 A_d1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME A_d1, LAYERED_METADATA undef" *) output [31:0]A_d1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 A_q1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME A_q1, LAYERED_METADATA undef" *) input [31:0]A_q1;

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
  wire ap_done;
  wire ap_idle;
  wire ap_ready;
  wire ap_rst;
  wire ap_start;

  (* ap_ST_fsm_state1 = "7'b0000001" *) 
  (* ap_ST_fsm_state2 = "7'b0000010" *) 
  (* ap_ST_fsm_state3 = "7'b0000100" *) 
  (* ap_ST_fsm_state4 = "7'b0001000" *) 
  (* ap_ST_fsm_state5 = "7'b0010000" *) 
  (* ap_ST_fsm_state6 = "7'b0100000" *) 
  (* ap_ST_fsm_state7 = "7'b1000000" *) 
  bd_0_hls_inst_0_loop_imperfect inst
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
        .ap_done(ap_done),
        .ap_idle(ap_idle),
        .ap_ready(ap_ready),
        .ap_rst(ap_rst),
        .ap_start(ap_start));
endmodule

(* ORIG_REF_NAME = "loop_imperfect" *) (* ap_ST_fsm_state1 = "7'b0000001" *) (* ap_ST_fsm_state2 = "7'b0000010" *) 
(* ap_ST_fsm_state3 = "7'b0000100" *) (* ap_ST_fsm_state4 = "7'b0001000" *) (* ap_ST_fsm_state5 = "7'b0010000" *) 
(* ap_ST_fsm_state6 = "7'b0100000" *) (* ap_ST_fsm_state7 = "7'b1000000" *) (* hls_module = "yes" *) 
module bd_0_hls_inst_0_loop_imperfect
   (ap_clk,
    ap_rst,
    ap_start,
    ap_done,
    ap_idle,
    ap_ready,
    A_address0,
    A_ce0,
    A_we0,
    A_d0,
    A_q0,
    A_address1,
    A_ce1,
    A_we1,
    A_d1,
    A_q1);
  input ap_clk;
  input ap_rst;
  input ap_start;
  output ap_done;
  output ap_idle;
  output ap_ready;
  output [5:0]A_address0;
  output A_ce0;
  output A_we0;
  output [31:0]A_d0;
  input [31:0]A_q0;
  output [5:0]A_address1;
  output A_ce1;
  output A_we1;
  output [31:0]A_d1;
  input [31:0]A_q1;

  wire [5:0]A_addr_1_reg_284;
  wire A_addr_1_reg_2840;
  wire [5:0]A_addr_reg_279;
  wire [5:0]A_address0;
  wire [5:0]A_address1;
  wire A_ce0;
  wire A_ce1;
  wire [31:0]A_d1;
  wire [31:0]A_q0;
  wire [31:0]A_q1;
  wire A_we0;
  wire A_we0_INST_0_i_10_n_1;
  wire A_we0_INST_0_i_11_n_1;
  wire A_we0_INST_0_i_12_n_1;
  wire A_we0_INST_0_i_13_n_1;
  wire A_we0_INST_0_i_14_n_1;
  wire A_we0_INST_0_i_14_n_2;
  wire A_we0_INST_0_i_14_n_3;
  wire A_we0_INST_0_i_14_n_4;
  wire A_we0_INST_0_i_15_n_1;
  wire A_we0_INST_0_i_16_n_1;
  wire A_we0_INST_0_i_17_n_1;
  wire A_we0_INST_0_i_18_n_1;
  wire A_we0_INST_0_i_19_n_1;
  wire A_we0_INST_0_i_20_n_1;
  wire A_we0_INST_0_i_21_n_1;
  wire A_we0_INST_0_i_22_n_1;
  wire A_we0_INST_0_i_23_n_1;
  wire A_we0_INST_0_i_23_n_2;
  wire A_we0_INST_0_i_23_n_3;
  wire A_we0_INST_0_i_23_n_4;
  wire A_we0_INST_0_i_24_n_1;
  wire A_we0_INST_0_i_25_n_1;
  wire A_we0_INST_0_i_26_n_1;
  wire A_we0_INST_0_i_27_n_1;
  wire A_we0_INST_0_i_28_n_1;
  wire A_we0_INST_0_i_29_n_1;
  wire A_we0_INST_0_i_30_n_1;
  wire A_we0_INST_0_i_31_n_1;
  wire A_we0_INST_0_i_32_n_1;
  wire A_we0_INST_0_i_32_n_2;
  wire A_we0_INST_0_i_32_n_3;
  wire A_we0_INST_0_i_32_n_4;
  wire A_we0_INST_0_i_33_n_1;
  wire A_we0_INST_0_i_34_n_1;
  wire A_we0_INST_0_i_35_n_1;
  wire A_we0_INST_0_i_36_n_1;
  wire A_we0_INST_0_i_37_n_1;
  wire A_we0_INST_0_i_38_n_1;
  wire A_we0_INST_0_i_39_n_1;
  wire A_we0_INST_0_i_3_n_2;
  wire A_we0_INST_0_i_3_n_3;
  wire A_we0_INST_0_i_3_n_4;
  wire A_we0_INST_0_i_40_n_1;
  wire A_we0_INST_0_i_41_n_1;
  wire A_we0_INST_0_i_41_n_2;
  wire A_we0_INST_0_i_41_n_3;
  wire A_we0_INST_0_i_41_n_4;
  wire A_we0_INST_0_i_42_n_1;
  wire A_we0_INST_0_i_43_n_1;
  wire A_we0_INST_0_i_44_n_1;
  wire A_we0_INST_0_i_45_n_1;
  wire A_we0_INST_0_i_46_n_1;
  wire A_we0_INST_0_i_47_n_1;
  wire A_we0_INST_0_i_48_n_1;
  wire A_we0_INST_0_i_49_n_1;
  wire A_we0_INST_0_i_4_n_2;
  wire A_we0_INST_0_i_4_n_3;
  wire A_we0_INST_0_i_4_n_4;
  wire A_we0_INST_0_i_50_n_1;
  wire A_we0_INST_0_i_50_n_2;
  wire A_we0_INST_0_i_50_n_3;
  wire A_we0_INST_0_i_50_n_4;
  wire A_we0_INST_0_i_51_n_1;
  wire A_we0_INST_0_i_52_n_1;
  wire A_we0_INST_0_i_53_n_1;
  wire A_we0_INST_0_i_54_n_1;
  wire A_we0_INST_0_i_55_n_1;
  wire A_we0_INST_0_i_56_n_1;
  wire A_we0_INST_0_i_57_n_1;
  wire A_we0_INST_0_i_58_n_1;
  wire A_we0_INST_0_i_59_n_1;
  wire A_we0_INST_0_i_5_n_1;
  wire A_we0_INST_0_i_5_n_2;
  wire A_we0_INST_0_i_5_n_3;
  wire A_we0_INST_0_i_5_n_4;
  wire A_we0_INST_0_i_60_n_1;
  wire A_we0_INST_0_i_61_n_1;
  wire A_we0_INST_0_i_62_n_1;
  wire A_we0_INST_0_i_63_n_1;
  wire A_we0_INST_0_i_64_n_1;
  wire A_we0_INST_0_i_65_n_1;
  wire A_we0_INST_0_i_66_n_1;
  wire A_we0_INST_0_i_67_n_1;
  wire A_we0_INST_0_i_68_n_1;
  wire A_we0_INST_0_i_69_n_1;
  wire A_we0_INST_0_i_6_n_1;
  wire A_we0_INST_0_i_70_n_1;
  wire A_we0_INST_0_i_71_n_1;
  wire A_we0_INST_0_i_72_n_1;
  wire A_we0_INST_0_i_73_n_1;
  wire A_we0_INST_0_i_74_n_1;
  wire A_we0_INST_0_i_7_n_1;
  wire A_we0_INST_0_i_8_n_1;
  wire A_we0_INST_0_i_9_n_1;
  wire A_we1;
  wire Ai_reg_2950;
  wire and_ln112_1_reg_300;
  wire and_ln112_2_reg_304;
  wire \and_ln112_2_reg_304[0]_i_1_n_1 ;
  wire [5:1]and_ln112_fu_200_p2;
  wire [5:1]and_ln112_reg_289;
  wire \and_ln112_reg_289[5]_i_10_n_1 ;
  wire \and_ln112_reg_289[5]_i_11_n_1 ;
  wire \and_ln112_reg_289[5]_i_13_n_1 ;
  wire \and_ln112_reg_289[5]_i_14_n_1 ;
  wire \and_ln112_reg_289[5]_i_15_n_1 ;
  wire \and_ln112_reg_289[5]_i_16_n_1 ;
  wire \and_ln112_reg_289[5]_i_17_n_1 ;
  wire \and_ln112_reg_289[5]_i_18_n_1 ;
  wire \and_ln112_reg_289[5]_i_19_n_1 ;
  wire \and_ln112_reg_289[5]_i_20_n_1 ;
  wire \and_ln112_reg_289[5]_i_22_n_1 ;
  wire \and_ln112_reg_289[5]_i_23_n_1 ;
  wire \and_ln112_reg_289[5]_i_24_n_1 ;
  wire \and_ln112_reg_289[5]_i_25_n_1 ;
  wire \and_ln112_reg_289[5]_i_26_n_1 ;
  wire \and_ln112_reg_289[5]_i_27_n_1 ;
  wire \and_ln112_reg_289[5]_i_28_n_1 ;
  wire \and_ln112_reg_289[5]_i_29_n_1 ;
  wire \and_ln112_reg_289[5]_i_30_n_1 ;
  wire \and_ln112_reg_289[5]_i_31_n_1 ;
  wire \and_ln112_reg_289[5]_i_32_n_1 ;
  wire \and_ln112_reg_289[5]_i_33_n_1 ;
  wire \and_ln112_reg_289[5]_i_34_n_1 ;
  wire \and_ln112_reg_289[5]_i_35_n_1 ;
  wire \and_ln112_reg_289[5]_i_36_n_1 ;
  wire \and_ln112_reg_289[5]_i_37_n_1 ;
  wire \and_ln112_reg_289[5]_i_6_n_1 ;
  wire \and_ln112_reg_289[5]_i_7_n_1 ;
  wire \and_ln112_reg_289[5]_i_8_n_1 ;
  wire \and_ln112_reg_289[5]_i_9_n_1 ;
  wire \and_ln112_reg_289_reg[5]_i_12_n_1 ;
  wire \and_ln112_reg_289_reg[5]_i_12_n_2 ;
  wire \and_ln112_reg_289_reg[5]_i_12_n_3 ;
  wire \and_ln112_reg_289_reg[5]_i_12_n_4 ;
  wire \and_ln112_reg_289_reg[5]_i_21_n_1 ;
  wire \and_ln112_reg_289_reg[5]_i_21_n_2 ;
  wire \and_ln112_reg_289_reg[5]_i_21_n_3 ;
  wire \and_ln112_reg_289_reg[5]_i_21_n_4 ;
  wire \and_ln112_reg_289_reg[5]_i_4_n_3 ;
  wire \and_ln112_reg_289_reg[5]_i_4_n_4 ;
  wire \and_ln112_reg_289_reg[5]_i_5_n_1 ;
  wire \and_ln112_reg_289_reg[5]_i_5_n_2 ;
  wire \and_ln112_reg_289_reg[5]_i_5_n_3 ;
  wire \and_ln112_reg_289_reg[5]_i_5_n_4 ;
  wire \ap_CS_fsm[3]_i_10_n_1 ;
  wire \ap_CS_fsm[3]_i_12_n_1 ;
  wire \ap_CS_fsm[3]_i_13_n_1 ;
  wire \ap_CS_fsm[3]_i_14_n_1 ;
  wire \ap_CS_fsm[3]_i_15_n_1 ;
  wire \ap_CS_fsm[3]_i_16_n_1 ;
  wire \ap_CS_fsm[3]_i_17_n_1 ;
  wire \ap_CS_fsm[3]_i_18_n_1 ;
  wire \ap_CS_fsm[3]_i_19_n_1 ;
  wire \ap_CS_fsm[3]_i_21_n_1 ;
  wire \ap_CS_fsm[3]_i_22_n_1 ;
  wire \ap_CS_fsm[3]_i_23_n_1 ;
  wire \ap_CS_fsm[3]_i_24_n_1 ;
  wire \ap_CS_fsm[3]_i_25_n_1 ;
  wire \ap_CS_fsm[3]_i_26_n_1 ;
  wire \ap_CS_fsm[3]_i_27_n_1 ;
  wire \ap_CS_fsm[3]_i_28_n_1 ;
  wire \ap_CS_fsm[3]_i_29_n_1 ;
  wire \ap_CS_fsm[3]_i_30_n_1 ;
  wire \ap_CS_fsm[3]_i_31_n_1 ;
  wire \ap_CS_fsm[3]_i_32_n_1 ;
  wire \ap_CS_fsm[3]_i_33_n_1 ;
  wire \ap_CS_fsm[3]_i_34_n_1 ;
  wire \ap_CS_fsm[3]_i_35_n_1 ;
  wire \ap_CS_fsm[3]_i_36_n_1 ;
  wire \ap_CS_fsm[3]_i_4_n_1 ;
  wire \ap_CS_fsm[3]_i_5_n_1 ;
  wire \ap_CS_fsm[3]_i_6_n_1 ;
  wire \ap_CS_fsm[3]_i_7_n_1 ;
  wire \ap_CS_fsm[3]_i_8_n_1 ;
  wire \ap_CS_fsm[3]_i_9_n_1 ;
  wire \ap_CS_fsm[6]_i_1_n_1 ;
  wire \ap_CS_fsm_reg[3]_i_11_n_1 ;
  wire \ap_CS_fsm_reg[3]_i_11_n_2 ;
  wire \ap_CS_fsm_reg[3]_i_11_n_3 ;
  wire \ap_CS_fsm_reg[3]_i_11_n_4 ;
  wire \ap_CS_fsm_reg[3]_i_20_n_1 ;
  wire \ap_CS_fsm_reg[3]_i_20_n_2 ;
  wire \ap_CS_fsm_reg[3]_i_20_n_3 ;
  wire \ap_CS_fsm_reg[3]_i_20_n_4 ;
  wire \ap_CS_fsm_reg[3]_i_2_n_2 ;
  wire \ap_CS_fsm_reg[3]_i_2_n_3 ;
  wire \ap_CS_fsm_reg[3]_i_2_n_4 ;
  wire \ap_CS_fsm_reg[3]_i_3_n_1 ;
  wire \ap_CS_fsm_reg[3]_i_3_n_2 ;
  wire \ap_CS_fsm_reg[3]_i_3_n_3 ;
  wire \ap_CS_fsm_reg[3]_i_3_n_4 ;
  wire \ap_CS_fsm_reg_n_1_[0] ;
  wire ap_CS_fsm_state2;
  wire ap_CS_fsm_state3;
  wire ap_CS_fsm_state4;
  wire ap_CS_fsm_state5;
  wire ap_CS_fsm_state6;
  wire ap_CS_fsm_state7;
  wire [4:0]ap_NS_fsm;
  wire ap_NS_fsm1;
  wire ap_NS_fsm13_out;
  wire ap_clk;
  wire ap_idle;
  wire ap_ready;
  wire ap_ready_INST_0_i_10_n_1;
  wire ap_ready_INST_0_i_11_n_1;
  wire ap_ready_INST_0_i_11_n_2;
  wire ap_ready_INST_0_i_11_n_3;
  wire ap_ready_INST_0_i_11_n_4;
  wire ap_ready_INST_0_i_12_n_1;
  wire ap_ready_INST_0_i_13_n_1;
  wire ap_ready_INST_0_i_14_n_1;
  wire ap_ready_INST_0_i_15_n_1;
  wire ap_ready_INST_0_i_16_n_1;
  wire ap_ready_INST_0_i_17_n_1;
  wire ap_ready_INST_0_i_18_n_1;
  wire ap_ready_INST_0_i_19_n_1;
  wire ap_ready_INST_0_i_1_n_3;
  wire ap_ready_INST_0_i_1_n_4;
  wire ap_ready_INST_0_i_20_n_1;
  wire ap_ready_INST_0_i_21_n_1;
  wire ap_ready_INST_0_i_2_n_1;
  wire ap_ready_INST_0_i_2_n_2;
  wire ap_ready_INST_0_i_2_n_3;
  wire ap_ready_INST_0_i_2_n_4;
  wire ap_ready_INST_0_i_3_n_1;
  wire ap_ready_INST_0_i_4_n_1;
  wire ap_ready_INST_0_i_5_n_1;
  wire ap_ready_INST_0_i_6_n_1;
  wire ap_ready_INST_0_i_6_n_2;
  wire ap_ready_INST_0_i_6_n_3;
  wire ap_ready_INST_0_i_6_n_4;
  wire ap_ready_INST_0_i_7_n_1;
  wire ap_ready_INST_0_i_8_n_1;
  wire ap_ready_INST_0_i_9_n_1;
  wire ap_rst;
  wire ap_start;
  wire [6:0]i_0_reg_102;
  wire i_0_reg_1020;
  wire \i_0_reg_102[6]_i_2_n_1 ;
  wire [6:0]i_fu_165_p2;
  wire [6:0]i_reg_270;
  wire \i_reg_270[6]_i_2_n_1 ;
  wire icmp_ln102_fu_159_p2;
  wire \icmp_ln102_reg_266[0]_i_2_n_1 ;
  wire \icmp_ln102_reg_266_reg_n_1_[0] ;
  wire icmp_ln107_fu_184_p2;
  wire icmp_ln107_reg_275;
  wire \icmp_ln107_reg_275[0]_i_1_n_1 ;
  wire icmp_ln112_1_fu_210_p2;
  wire icmp_ln112_2_fu_222_p2;
  wire icmp_ln96_fu_113_p2;
  wire icmp_ln99_fu_133_p2;
  wire j_0_in_reg_920;
  wire \j_0_in_reg_92[31]_i_1_n_1 ;
  wire \j_0_in_reg_92[31]_i_2_n_1 ;
  wire [30:30]j_reg_251_reg;
  wire k_0_reg_80;
  wire [5:0]l_fu_179_p2;
  wire p_10_in;
  wire [30:1]p_1_in;
  wire p_8_in;
  wire [30:0]trunc_ln1_fu_123_p4;
  wire [31:1]trunc_ln96_fu_119_p1;
  wire \trunc_ln96_reg_242_reg_n_1_[1] ;
  wire \trunc_ln96_reg_242_reg_n_1_[2] ;
  wire \trunc_ln96_reg_242_reg_n_1_[3] ;
  wire \trunc_ln96_reg_242_reg_n_1_[4] ;
  wire \trunc_ln96_reg_242_reg_n_1_[5] ;
  wire [29:0]trunc_ln99_1_reg_256;
  wire [3:0]NLW_A_we0_INST_0_i_14_O_UNCONNECTED;
  wire [3:0]NLW_A_we0_INST_0_i_23_O_UNCONNECTED;
  wire [3:0]NLW_A_we0_INST_0_i_3_O_UNCONNECTED;
  wire [3:0]NLW_A_we0_INST_0_i_32_O_UNCONNECTED;
  wire [3:0]NLW_A_we0_INST_0_i_4_O_UNCONNECTED;
  wire [3:0]NLW_A_we0_INST_0_i_41_O_UNCONNECTED;
  wire [3:0]NLW_A_we0_INST_0_i_5_O_UNCONNECTED;
  wire [3:0]NLW_A_we0_INST_0_i_50_O_UNCONNECTED;
  wire [3:0]\NLW_and_ln112_reg_289_reg[5]_i_12_O_UNCONNECTED ;
  wire [3:0]\NLW_and_ln112_reg_289_reg[5]_i_21_O_UNCONNECTED ;
  wire [3:3]\NLW_and_ln112_reg_289_reg[5]_i_4_CO_UNCONNECTED ;
  wire [3:0]\NLW_and_ln112_reg_289_reg[5]_i_4_O_UNCONNECTED ;
  wire [3:0]\NLW_and_ln112_reg_289_reg[5]_i_5_O_UNCONNECTED ;
  wire [3:0]\NLW_ap_CS_fsm_reg[3]_i_11_O_UNCONNECTED ;
  wire [3:0]\NLW_ap_CS_fsm_reg[3]_i_2_O_UNCONNECTED ;
  wire [3:0]\NLW_ap_CS_fsm_reg[3]_i_20_O_UNCONNECTED ;
  wire [3:0]\NLW_ap_CS_fsm_reg[3]_i_3_O_UNCONNECTED ;
  wire [3:3]NLW_ap_ready_INST_0_i_1_CO_UNCONNECTED;
  wire [3:0]NLW_ap_ready_INST_0_i_1_O_UNCONNECTED;
  wire [3:0]NLW_ap_ready_INST_0_i_11_O_UNCONNECTED;
  wire [3:0]NLW_ap_ready_INST_0_i_2_O_UNCONNECTED;
  wire [3:0]NLW_ap_ready_INST_0_i_6_O_UNCONNECTED;

  assign A_d0[31:0] = A_q1;
  assign ap_done = ap_ready;
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \A_addr_1_reg_284[0]_i_1 
       (.I0(trunc_ln99_1_reg_256[0]),
        .I1(i_0_reg_102[0]),
        .O(l_fu_179_p2[0]));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \A_addr_1_reg_284[1]_i_1 
       (.I0(trunc_ln99_1_reg_256[1]),
        .I1(i_0_reg_102[1]),
        .O(l_fu_179_p2[1]));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \A_addr_1_reg_284[2]_i_1 
       (.I0(trunc_ln99_1_reg_256[2]),
        .I1(i_0_reg_102[2]),
        .O(l_fu_179_p2[2]));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \A_addr_1_reg_284[3]_i_1 
       (.I0(trunc_ln99_1_reg_256[3]),
        .I1(i_0_reg_102[3]),
        .O(l_fu_179_p2[3]));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \A_addr_1_reg_284[4]_i_1 
       (.I0(trunc_ln99_1_reg_256[4]),
        .I1(i_0_reg_102[4]),
        .O(l_fu_179_p2[4]));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \A_addr_1_reg_284[5]_i_1 
       (.I0(trunc_ln99_1_reg_256[5]),
        .I1(i_0_reg_102[5]),
        .O(l_fu_179_p2[5]));
  FDRE \A_addr_1_reg_284_reg[0] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(l_fu_179_p2[0]),
        .Q(A_addr_1_reg_284[0]),
        .R(1'b0));
  FDRE \A_addr_1_reg_284_reg[1] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(l_fu_179_p2[1]),
        .Q(A_addr_1_reg_284[1]),
        .R(1'b0));
  FDRE \A_addr_1_reg_284_reg[2] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(l_fu_179_p2[2]),
        .Q(A_addr_1_reg_284[2]),
        .R(1'b0));
  FDRE \A_addr_1_reg_284_reg[3] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(l_fu_179_p2[3]),
        .Q(A_addr_1_reg_284[3]),
        .R(1'b0));
  FDRE \A_addr_1_reg_284_reg[4] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(l_fu_179_p2[4]),
        .Q(A_addr_1_reg_284[4]),
        .R(1'b0));
  FDRE \A_addr_1_reg_284_reg[5] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(l_fu_179_p2[5]),
        .Q(A_addr_1_reg_284[5]),
        .R(1'b0));
  FDRE \A_addr_reg_279_reg[0] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(i_0_reg_102[0]),
        .Q(A_addr_reg_279[0]),
        .R(1'b0));
  FDRE \A_addr_reg_279_reg[1] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(i_0_reg_102[1]),
        .Q(A_addr_reg_279[1]),
        .R(1'b0));
  FDRE \A_addr_reg_279_reg[2] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(i_0_reg_102[2]),
        .Q(A_addr_reg_279[2]),
        .R(1'b0));
  FDRE \A_addr_reg_279_reg[3] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(i_0_reg_102[3]),
        .Q(A_addr_reg_279[3]),
        .R(1'b0));
  FDRE \A_addr_reg_279_reg[4] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(i_0_reg_102[4]),
        .Q(A_addr_reg_279[4]),
        .R(1'b0));
  FDRE \A_addr_reg_279_reg[5] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(i_0_reg_102[5]),
        .Q(A_addr_reg_279[5]),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair14" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \A_address0[0]_INST_0 
       (.I0(A_addr_reg_279[0]),
        .I1(ap_CS_fsm_state5),
        .I2(i_0_reg_102[0]),
        .O(A_address0[0]));
  (* SOFT_HLUTNM = "soft_lutpair15" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \A_address0[1]_INST_0 
       (.I0(A_addr_reg_279[1]),
        .I1(ap_CS_fsm_state5),
        .I2(i_0_reg_102[1]),
        .O(A_address0[1]));
  (* SOFT_HLUTNM = "soft_lutpair14" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \A_address0[2]_INST_0 
       (.I0(A_addr_reg_279[2]),
        .I1(ap_CS_fsm_state5),
        .I2(i_0_reg_102[2]),
        .O(A_address0[2]));
  (* SOFT_HLUTNM = "soft_lutpair13" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \A_address0[3]_INST_0 
       (.I0(A_addr_reg_279[3]),
        .I1(ap_CS_fsm_state5),
        .I2(i_0_reg_102[3]),
        .O(A_address0[3]));
  (* SOFT_HLUTNM = "soft_lutpair15" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \A_address0[4]_INST_0 
       (.I0(A_addr_reg_279[4]),
        .I1(ap_CS_fsm_state5),
        .I2(i_0_reg_102[4]),
        .O(A_address0[4]));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \A_address0[5]_INST_0 
       (.I0(A_addr_reg_279[5]),
        .I1(ap_CS_fsm_state5),
        .I2(i_0_reg_102[5]),
        .O(A_address0[5]));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT4 #(
    .INIT(16'h8BB8)) 
    \A_address1[0]_INST_0 
       (.I0(A_addr_1_reg_284[0]),
        .I1(ap_CS_fsm_state6),
        .I2(trunc_ln99_1_reg_256[0]),
        .I3(i_0_reg_102[0]),
        .O(A_address1[0]));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT4 #(
    .INIT(16'h8BB8)) 
    \A_address1[1]_INST_0 
       (.I0(A_addr_1_reg_284[1]),
        .I1(ap_CS_fsm_state6),
        .I2(trunc_ln99_1_reg_256[1]),
        .I3(i_0_reg_102[1]),
        .O(A_address1[1]));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT4 #(
    .INIT(16'h8BB8)) 
    \A_address1[2]_INST_0 
       (.I0(A_addr_1_reg_284[2]),
        .I1(ap_CS_fsm_state6),
        .I2(trunc_ln99_1_reg_256[2]),
        .I3(i_0_reg_102[2]),
        .O(A_address1[2]));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT4 #(
    .INIT(16'h8BB8)) 
    \A_address1[3]_INST_0 
       (.I0(A_addr_1_reg_284[3]),
        .I1(ap_CS_fsm_state6),
        .I2(trunc_ln99_1_reg_256[3]),
        .I3(i_0_reg_102[3]),
        .O(A_address1[3]));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT4 #(
    .INIT(16'h8BB8)) 
    \A_address1[4]_INST_0 
       (.I0(A_addr_1_reg_284[4]),
        .I1(ap_CS_fsm_state6),
        .I2(trunc_ln99_1_reg_256[4]),
        .I3(i_0_reg_102[4]),
        .O(A_address1[4]));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT4 #(
    .INIT(16'h8BB8)) 
    \A_address1[5]_INST_0 
       (.I0(A_addr_1_reg_284[5]),
        .I1(ap_CS_fsm_state6),
        .I2(trunc_ln99_1_reg_256[5]),
        .I3(i_0_reg_102[5]),
        .O(A_address1[5]));
  (* SOFT_HLUTNM = "soft_lutpair13" *) 
  LUT2 #(
    .INIT(4'hE)) 
    A_ce0_INST_0
       (.I0(ap_CS_fsm_state5),
        .I1(ap_CS_fsm_state4),
        .O(A_ce0));
  (* SOFT_HLUTNM = "soft_lutpair29" *) 
  LUT2 #(
    .INIT(4'hE)) 
    A_ce1_INST_0
       (.I0(ap_CS_fsm_state4),
        .I1(ap_CS_fsm_state6),
        .O(A_ce1));
  LUT5 #(
    .INIT(32'h30200000)) 
    A_we0_INST_0
       (.I0(p_8_in),
        .I1(\icmp_ln102_reg_266_reg_n_1_[0] ),
        .I2(icmp_ln107_reg_275),
        .I3(p_10_in),
        .I4(ap_CS_fsm_state5),
        .O(A_we0));
  LUT6 #(
    .INIT(64'hAAAAAAAAAAAAAAA8)) 
    A_we0_INST_0_i_1
       (.I0(icmp_ln112_2_fu_222_p2),
        .I1(and_ln112_reg_289[2]),
        .I2(and_ln112_reg_289[3]),
        .I3(and_ln112_reg_289[1]),
        .I4(and_ln112_reg_289[4]),
        .I5(and_ln112_reg_289[5]),
        .O(p_8_in));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_10
       (.I0(A_q1[30]),
        .I1(A_q0[30]),
        .I2(A_q0[31]),
        .I3(A_q1[31]),
        .O(A_we0_INST_0_i_10_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_11
       (.I0(A_q1[28]),
        .I1(A_q0[28]),
        .I2(A_q1[29]),
        .I3(A_q0[29]),
        .O(A_we0_INST_0_i_11_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_12
       (.I0(A_q1[26]),
        .I1(A_q0[26]),
        .I2(A_q1[27]),
        .I3(A_q0[27]),
        .O(A_we0_INST_0_i_12_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_13
       (.I0(A_q1[24]),
        .I1(A_q0[24]),
        .I2(A_q1[25]),
        .I3(A_q0[25]),
        .O(A_we0_INST_0_i_13_n_1));
  CARRY4 A_we0_INST_0_i_14
       (.CI(A_we0_INST_0_i_32_n_1),
        .CO({A_we0_INST_0_i_14_n_1,A_we0_INST_0_i_14_n_2,A_we0_INST_0_i_14_n_3,A_we0_INST_0_i_14_n_4}),
        .CYINIT(1'b0),
        .DI({A_we0_INST_0_i_33_n_1,A_we0_INST_0_i_34_n_1,A_we0_INST_0_i_35_n_1,A_we0_INST_0_i_36_n_1}),
        .O(NLW_A_we0_INST_0_i_14_O_UNCONNECTED[3:0]),
        .S({A_we0_INST_0_i_37_n_1,A_we0_INST_0_i_38_n_1,A_we0_INST_0_i_39_n_1,A_we0_INST_0_i_40_n_1}));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_15
       (.I0(A_q0[30]),
        .I1(A_q1[30]),
        .I2(A_q0[31]),
        .I3(A_q1[31]),
        .O(A_we0_INST_0_i_15_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_16
       (.I0(A_q0[28]),
        .I1(A_q1[28]),
        .I2(A_q1[29]),
        .I3(A_q0[29]),
        .O(A_we0_INST_0_i_16_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_17
       (.I0(A_q0[26]),
        .I1(A_q1[26]),
        .I2(A_q1[27]),
        .I3(A_q0[27]),
        .O(A_we0_INST_0_i_17_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_18
       (.I0(A_q0[24]),
        .I1(A_q1[24]),
        .I2(A_q1[25]),
        .I3(A_q0[25]),
        .O(A_we0_INST_0_i_18_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_19
       (.I0(A_q0[30]),
        .I1(A_q1[30]),
        .I2(A_q1[31]),
        .I3(A_q0[31]),
        .O(A_we0_INST_0_i_19_n_1));
  LUT6 #(
    .INIT(64'h0000000000000002)) 
    A_we0_INST_0_i_2
       (.I0(icmp_ln112_1_fu_210_p2),
        .I1(and_ln112_reg_289[2]),
        .I2(and_ln112_reg_289[1]),
        .I3(and_ln112_reg_289[5]),
        .I4(and_ln112_reg_289[3]),
        .I5(and_ln112_reg_289[4]),
        .O(p_10_in));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_20
       (.I0(A_q0[28]),
        .I1(A_q1[28]),
        .I2(A_q0[29]),
        .I3(A_q1[29]),
        .O(A_we0_INST_0_i_20_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_21
       (.I0(A_q0[26]),
        .I1(A_q1[26]),
        .I2(A_q0[27]),
        .I3(A_q1[27]),
        .O(A_we0_INST_0_i_21_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_22
       (.I0(A_q0[24]),
        .I1(A_q1[24]),
        .I2(A_q0[25]),
        .I3(A_q1[25]),
        .O(A_we0_INST_0_i_22_n_1));
  CARRY4 A_we0_INST_0_i_23
       (.CI(A_we0_INST_0_i_41_n_1),
        .CO({A_we0_INST_0_i_23_n_1,A_we0_INST_0_i_23_n_2,A_we0_INST_0_i_23_n_3,A_we0_INST_0_i_23_n_4}),
        .CYINIT(1'b0),
        .DI({A_we0_INST_0_i_42_n_1,A_we0_INST_0_i_43_n_1,A_we0_INST_0_i_44_n_1,A_we0_INST_0_i_45_n_1}),
        .O(NLW_A_we0_INST_0_i_23_O_UNCONNECTED[3:0]),
        .S({A_we0_INST_0_i_46_n_1,A_we0_INST_0_i_47_n_1,A_we0_INST_0_i_48_n_1,A_we0_INST_0_i_49_n_1}));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_24
       (.I0(A_q1[22]),
        .I1(A_q0[22]),
        .I2(A_q0[23]),
        .I3(A_q1[23]),
        .O(A_we0_INST_0_i_24_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_25
       (.I0(A_q1[20]),
        .I1(A_q0[20]),
        .I2(A_q0[21]),
        .I3(A_q1[21]),
        .O(A_we0_INST_0_i_25_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_26
       (.I0(A_q1[18]),
        .I1(A_q0[18]),
        .I2(A_q0[19]),
        .I3(A_q1[19]),
        .O(A_we0_INST_0_i_26_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_27
       (.I0(A_q1[16]),
        .I1(A_q0[16]),
        .I2(A_q0[17]),
        .I3(A_q1[17]),
        .O(A_we0_INST_0_i_27_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_28
       (.I0(A_q1[22]),
        .I1(A_q0[22]),
        .I2(A_q1[23]),
        .I3(A_q0[23]),
        .O(A_we0_INST_0_i_28_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_29
       (.I0(A_q1[20]),
        .I1(A_q0[20]),
        .I2(A_q1[21]),
        .I3(A_q0[21]),
        .O(A_we0_INST_0_i_29_n_1));
  CARRY4 A_we0_INST_0_i_3
       (.CI(A_we0_INST_0_i_5_n_1),
        .CO({icmp_ln112_2_fu_222_p2,A_we0_INST_0_i_3_n_2,A_we0_INST_0_i_3_n_3,A_we0_INST_0_i_3_n_4}),
        .CYINIT(1'b0),
        .DI({A_we0_INST_0_i_6_n_1,A_we0_INST_0_i_7_n_1,A_we0_INST_0_i_8_n_1,A_we0_INST_0_i_9_n_1}),
        .O(NLW_A_we0_INST_0_i_3_O_UNCONNECTED[3:0]),
        .S({A_we0_INST_0_i_10_n_1,A_we0_INST_0_i_11_n_1,A_we0_INST_0_i_12_n_1,A_we0_INST_0_i_13_n_1}));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_30
       (.I0(A_q1[18]),
        .I1(A_q0[18]),
        .I2(A_q1[19]),
        .I3(A_q0[19]),
        .O(A_we0_INST_0_i_30_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_31
       (.I0(A_q1[16]),
        .I1(A_q0[16]),
        .I2(A_q1[17]),
        .I3(A_q0[17]),
        .O(A_we0_INST_0_i_31_n_1));
  CARRY4 A_we0_INST_0_i_32
       (.CI(A_we0_INST_0_i_50_n_1),
        .CO({A_we0_INST_0_i_32_n_1,A_we0_INST_0_i_32_n_2,A_we0_INST_0_i_32_n_3,A_we0_INST_0_i_32_n_4}),
        .CYINIT(1'b0),
        .DI({A_we0_INST_0_i_51_n_1,A_we0_INST_0_i_52_n_1,A_we0_INST_0_i_53_n_1,A_we0_INST_0_i_54_n_1}),
        .O(NLW_A_we0_INST_0_i_32_O_UNCONNECTED[3:0]),
        .S({A_we0_INST_0_i_55_n_1,A_we0_INST_0_i_56_n_1,A_we0_INST_0_i_57_n_1,A_we0_INST_0_i_58_n_1}));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_33
       (.I0(A_q0[22]),
        .I1(A_q1[22]),
        .I2(A_q1[23]),
        .I3(A_q0[23]),
        .O(A_we0_INST_0_i_33_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_34
       (.I0(A_q0[20]),
        .I1(A_q1[20]),
        .I2(A_q1[21]),
        .I3(A_q0[21]),
        .O(A_we0_INST_0_i_34_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_35
       (.I0(A_q0[18]),
        .I1(A_q1[18]),
        .I2(A_q1[19]),
        .I3(A_q0[19]),
        .O(A_we0_INST_0_i_35_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_36
       (.I0(A_q0[16]),
        .I1(A_q1[16]),
        .I2(A_q1[17]),
        .I3(A_q0[17]),
        .O(A_we0_INST_0_i_36_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_37
       (.I0(A_q0[22]),
        .I1(A_q1[22]),
        .I2(A_q0[23]),
        .I3(A_q1[23]),
        .O(A_we0_INST_0_i_37_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_38
       (.I0(A_q0[20]),
        .I1(A_q1[20]),
        .I2(A_q0[21]),
        .I3(A_q1[21]),
        .O(A_we0_INST_0_i_38_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_39
       (.I0(A_q0[18]),
        .I1(A_q1[18]),
        .I2(A_q0[19]),
        .I3(A_q1[19]),
        .O(A_we0_INST_0_i_39_n_1));
  CARRY4 A_we0_INST_0_i_4
       (.CI(A_we0_INST_0_i_14_n_1),
        .CO({icmp_ln112_1_fu_210_p2,A_we0_INST_0_i_4_n_2,A_we0_INST_0_i_4_n_3,A_we0_INST_0_i_4_n_4}),
        .CYINIT(1'b0),
        .DI({A_we0_INST_0_i_15_n_1,A_we0_INST_0_i_16_n_1,A_we0_INST_0_i_17_n_1,A_we0_INST_0_i_18_n_1}),
        .O(NLW_A_we0_INST_0_i_4_O_UNCONNECTED[3:0]),
        .S({A_we0_INST_0_i_19_n_1,A_we0_INST_0_i_20_n_1,A_we0_INST_0_i_21_n_1,A_we0_INST_0_i_22_n_1}));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_40
       (.I0(A_q0[16]),
        .I1(A_q1[16]),
        .I2(A_q0[17]),
        .I3(A_q1[17]),
        .O(A_we0_INST_0_i_40_n_1));
  CARRY4 A_we0_INST_0_i_41
       (.CI(1'b0),
        .CO({A_we0_INST_0_i_41_n_1,A_we0_INST_0_i_41_n_2,A_we0_INST_0_i_41_n_3,A_we0_INST_0_i_41_n_4}),
        .CYINIT(1'b0),
        .DI({A_we0_INST_0_i_59_n_1,A_we0_INST_0_i_60_n_1,A_we0_INST_0_i_61_n_1,A_we0_INST_0_i_62_n_1}),
        .O(NLW_A_we0_INST_0_i_41_O_UNCONNECTED[3:0]),
        .S({A_we0_INST_0_i_63_n_1,A_we0_INST_0_i_64_n_1,A_we0_INST_0_i_65_n_1,A_we0_INST_0_i_66_n_1}));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_42
       (.I0(A_q1[14]),
        .I1(A_q0[14]),
        .I2(A_q0[15]),
        .I3(A_q1[15]),
        .O(A_we0_INST_0_i_42_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_43
       (.I0(A_q1[12]),
        .I1(A_q0[12]),
        .I2(A_q0[13]),
        .I3(A_q1[13]),
        .O(A_we0_INST_0_i_43_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_44
       (.I0(A_q1[10]),
        .I1(A_q0[10]),
        .I2(A_q0[11]),
        .I3(A_q1[11]),
        .O(A_we0_INST_0_i_44_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_45
       (.I0(A_q1[8]),
        .I1(A_q0[8]),
        .I2(A_q0[9]),
        .I3(A_q1[9]),
        .O(A_we0_INST_0_i_45_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_46
       (.I0(A_q1[14]),
        .I1(A_q0[14]),
        .I2(A_q1[15]),
        .I3(A_q0[15]),
        .O(A_we0_INST_0_i_46_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_47
       (.I0(A_q1[12]),
        .I1(A_q0[12]),
        .I2(A_q1[13]),
        .I3(A_q0[13]),
        .O(A_we0_INST_0_i_47_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_48
       (.I0(A_q1[10]),
        .I1(A_q0[10]),
        .I2(A_q1[11]),
        .I3(A_q0[11]),
        .O(A_we0_INST_0_i_48_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_49
       (.I0(A_q1[8]),
        .I1(A_q0[8]),
        .I2(A_q1[9]),
        .I3(A_q0[9]),
        .O(A_we0_INST_0_i_49_n_1));
  CARRY4 A_we0_INST_0_i_5
       (.CI(A_we0_INST_0_i_23_n_1),
        .CO({A_we0_INST_0_i_5_n_1,A_we0_INST_0_i_5_n_2,A_we0_INST_0_i_5_n_3,A_we0_INST_0_i_5_n_4}),
        .CYINIT(1'b0),
        .DI({A_we0_INST_0_i_24_n_1,A_we0_INST_0_i_25_n_1,A_we0_INST_0_i_26_n_1,A_we0_INST_0_i_27_n_1}),
        .O(NLW_A_we0_INST_0_i_5_O_UNCONNECTED[3:0]),
        .S({A_we0_INST_0_i_28_n_1,A_we0_INST_0_i_29_n_1,A_we0_INST_0_i_30_n_1,A_we0_INST_0_i_31_n_1}));
  CARRY4 A_we0_INST_0_i_50
       (.CI(1'b0),
        .CO({A_we0_INST_0_i_50_n_1,A_we0_INST_0_i_50_n_2,A_we0_INST_0_i_50_n_3,A_we0_INST_0_i_50_n_4}),
        .CYINIT(1'b0),
        .DI({A_we0_INST_0_i_67_n_1,A_we0_INST_0_i_68_n_1,A_we0_INST_0_i_69_n_1,A_we0_INST_0_i_70_n_1}),
        .O(NLW_A_we0_INST_0_i_50_O_UNCONNECTED[3:0]),
        .S({A_we0_INST_0_i_71_n_1,A_we0_INST_0_i_72_n_1,A_we0_INST_0_i_73_n_1,A_we0_INST_0_i_74_n_1}));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_51
       (.I0(A_q0[14]),
        .I1(A_q1[14]),
        .I2(A_q1[15]),
        .I3(A_q0[15]),
        .O(A_we0_INST_0_i_51_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_52
       (.I0(A_q0[12]),
        .I1(A_q1[12]),
        .I2(A_q1[13]),
        .I3(A_q0[13]),
        .O(A_we0_INST_0_i_52_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_53
       (.I0(A_q0[10]),
        .I1(A_q1[10]),
        .I2(A_q1[11]),
        .I3(A_q0[11]),
        .O(A_we0_INST_0_i_53_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_54
       (.I0(A_q0[8]),
        .I1(A_q1[8]),
        .I2(A_q1[9]),
        .I3(A_q0[9]),
        .O(A_we0_INST_0_i_54_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_55
       (.I0(A_q0[14]),
        .I1(A_q1[14]),
        .I2(A_q0[15]),
        .I3(A_q1[15]),
        .O(A_we0_INST_0_i_55_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_56
       (.I0(A_q0[12]),
        .I1(A_q1[12]),
        .I2(A_q0[13]),
        .I3(A_q1[13]),
        .O(A_we0_INST_0_i_56_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_57
       (.I0(A_q0[10]),
        .I1(A_q1[10]),
        .I2(A_q0[11]),
        .I3(A_q1[11]),
        .O(A_we0_INST_0_i_57_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_58
       (.I0(A_q0[8]),
        .I1(A_q1[8]),
        .I2(A_q0[9]),
        .I3(A_q1[9]),
        .O(A_we0_INST_0_i_58_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_59
       (.I0(A_q1[6]),
        .I1(A_q0[6]),
        .I2(A_q0[7]),
        .I3(A_q1[7]),
        .O(A_we0_INST_0_i_59_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_6
       (.I0(A_q1[30]),
        .I1(A_q0[30]),
        .I2(A_q1[31]),
        .I3(A_q0[31]),
        .O(A_we0_INST_0_i_6_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_60
       (.I0(A_q1[4]),
        .I1(A_q0[4]),
        .I2(A_q0[5]),
        .I3(A_q1[5]),
        .O(A_we0_INST_0_i_60_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_61
       (.I0(A_q1[2]),
        .I1(A_q0[2]),
        .I2(A_q0[3]),
        .I3(A_q1[3]),
        .O(A_we0_INST_0_i_61_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_62
       (.I0(A_q1[0]),
        .I1(A_q0[0]),
        .I2(A_q0[1]),
        .I3(A_q1[1]),
        .O(A_we0_INST_0_i_62_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_63
       (.I0(A_q1[6]),
        .I1(A_q0[6]),
        .I2(A_q1[7]),
        .I3(A_q0[7]),
        .O(A_we0_INST_0_i_63_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_64
       (.I0(A_q1[4]),
        .I1(A_q0[4]),
        .I2(A_q1[5]),
        .I3(A_q0[5]),
        .O(A_we0_INST_0_i_64_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_65
       (.I0(A_q1[2]),
        .I1(A_q0[2]),
        .I2(A_q1[3]),
        .I3(A_q0[3]),
        .O(A_we0_INST_0_i_65_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_66
       (.I0(A_q1[0]),
        .I1(A_q0[0]),
        .I2(A_q1[1]),
        .I3(A_q0[1]),
        .O(A_we0_INST_0_i_66_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_67
       (.I0(A_q0[6]),
        .I1(A_q1[6]),
        .I2(A_q1[7]),
        .I3(A_q0[7]),
        .O(A_we0_INST_0_i_67_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_68
       (.I0(A_q0[4]),
        .I1(A_q1[4]),
        .I2(A_q1[5]),
        .I3(A_q0[5]),
        .O(A_we0_INST_0_i_68_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_69
       (.I0(A_q0[2]),
        .I1(A_q1[2]),
        .I2(A_q1[3]),
        .I3(A_q0[3]),
        .O(A_we0_INST_0_i_69_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_7
       (.I0(A_q1[28]),
        .I1(A_q0[28]),
        .I2(A_q0[29]),
        .I3(A_q1[29]),
        .O(A_we0_INST_0_i_7_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_70
       (.I0(A_q0[0]),
        .I1(A_q1[0]),
        .I2(A_q1[1]),
        .I3(A_q0[1]),
        .O(A_we0_INST_0_i_70_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_71
       (.I0(A_q0[6]),
        .I1(A_q1[6]),
        .I2(A_q0[7]),
        .I3(A_q1[7]),
        .O(A_we0_INST_0_i_71_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_72
       (.I0(A_q0[4]),
        .I1(A_q1[4]),
        .I2(A_q0[5]),
        .I3(A_q1[5]),
        .O(A_we0_INST_0_i_72_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_73
       (.I0(A_q0[2]),
        .I1(A_q1[2]),
        .I2(A_q0[3]),
        .I3(A_q1[3]),
        .O(A_we0_INST_0_i_73_n_1));
  LUT4 #(
    .INIT(16'h9009)) 
    A_we0_INST_0_i_74
       (.I0(A_q0[0]),
        .I1(A_q1[0]),
        .I2(A_q0[1]),
        .I3(A_q1[1]),
        .O(A_we0_INST_0_i_74_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_8
       (.I0(A_q1[26]),
        .I1(A_q0[26]),
        .I2(A_q0[27]),
        .I3(A_q1[27]),
        .O(A_we0_INST_0_i_8_n_1));
  LUT4 #(
    .INIT(16'h2F02)) 
    A_we0_INST_0_i_9
       (.I0(A_q1[24]),
        .I1(A_q0[24]),
        .I2(A_q0[25]),
        .I3(A_q1[25]),
        .O(A_we0_INST_0_i_9_n_1));
  LUT5 #(
    .INIT(32'h0A000800)) 
    A_we1_INST_0
       (.I0(ap_CS_fsm_state6),
        .I1(and_ln112_2_reg_304),
        .I2(\icmp_ln102_reg_266_reg_n_1_[0] ),
        .I3(icmp_ln107_reg_275),
        .I4(and_ln112_1_reg_300),
        .O(A_we1));
  LUT3 #(
    .INIT(8'h20)) 
    \Ai_reg_295[31]_i_1 
       (.I0(ap_CS_fsm_state5),
        .I1(\icmp_ln102_reg_266_reg_n_1_[0] ),
        .I2(icmp_ln107_reg_275),
        .O(Ai_reg_2950));
  FDRE \Ai_reg_295_reg[0] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[0]),
        .Q(A_d1[0]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[10] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[10]),
        .Q(A_d1[10]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[11] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[11]),
        .Q(A_d1[11]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[12] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[12]),
        .Q(A_d1[12]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[13] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[13]),
        .Q(A_d1[13]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[14] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[14]),
        .Q(A_d1[14]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[15] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[15]),
        .Q(A_d1[15]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[16] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[16]),
        .Q(A_d1[16]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[17] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[17]),
        .Q(A_d1[17]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[18] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[18]),
        .Q(A_d1[18]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[19] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[19]),
        .Q(A_d1[19]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[1] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[1]),
        .Q(A_d1[1]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[20] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[20]),
        .Q(A_d1[20]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[21] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[21]),
        .Q(A_d1[21]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[22] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[22]),
        .Q(A_d1[22]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[23] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[23]),
        .Q(A_d1[23]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[24] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[24]),
        .Q(A_d1[24]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[25] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[25]),
        .Q(A_d1[25]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[26] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[26]),
        .Q(A_d1[26]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[27] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[27]),
        .Q(A_d1[27]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[28] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[28]),
        .Q(A_d1[28]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[29] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[29]),
        .Q(A_d1[29]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[2] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[2]),
        .Q(A_d1[2]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[30] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[30]),
        .Q(A_d1[30]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[31] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[31]),
        .Q(A_d1[31]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[3] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[3]),
        .Q(A_d1[3]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[4] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[4]),
        .Q(A_d1[4]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[5] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[5]),
        .Q(A_d1[5]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[6] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[6]),
        .Q(A_d1[6]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[7] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[7]),
        .Q(A_d1[7]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[8] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[8]),
        .Q(A_d1[8]),
        .R(1'b0));
  FDRE \Ai_reg_295_reg[9] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(A_q0[9]),
        .Q(A_d1[9]),
        .R(1'b0));
  FDRE \and_ln112_1_reg_300_reg[0] 
       (.C(ap_clk),
        .CE(Ai_reg_2950),
        .D(p_10_in),
        .Q(and_ln112_1_reg_300),
        .R(1'b0));
  LUT6 #(
    .INIT(64'hFFEFFFFF00200000)) 
    \and_ln112_2_reg_304[0]_i_1 
       (.I0(p_8_in),
        .I1(\icmp_ln102_reg_266_reg_n_1_[0] ),
        .I2(icmp_ln107_reg_275),
        .I3(p_10_in),
        .I4(ap_CS_fsm_state5),
        .I5(and_ln112_2_reg_304),
        .O(\and_ln112_2_reg_304[0]_i_1_n_1 ));
  FDRE \and_ln112_2_reg_304_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\and_ln112_2_reg_304[0]_i_1_n_1 ),
        .Q(and_ln112_2_reg_304),
        .R(1'b0));
  LUT2 #(
    .INIT(4'h8)) 
    \and_ln112_reg_289[1]_i_1 
       (.I0(\trunc_ln96_reg_242_reg_n_1_[1] ),
        .I1(i_0_reg_102[1]),
        .O(and_ln112_fu_200_p2[1]));
  LUT2 #(
    .INIT(4'h8)) 
    \and_ln112_reg_289[2]_i_1 
       (.I0(\trunc_ln96_reg_242_reg_n_1_[2] ),
        .I1(i_0_reg_102[2]),
        .O(and_ln112_fu_200_p2[2]));
  LUT2 #(
    .INIT(4'h8)) 
    \and_ln112_reg_289[3]_i_1 
       (.I0(\trunc_ln96_reg_242_reg_n_1_[3] ),
        .I1(i_0_reg_102[3]),
        .O(and_ln112_fu_200_p2[3]));
  LUT2 #(
    .INIT(4'h8)) 
    \and_ln112_reg_289[4]_i_1 
       (.I0(\trunc_ln96_reg_242_reg_n_1_[4] ),
        .I1(i_0_reg_102[4]),
        .O(and_ln112_fu_200_p2[4]));
  LUT2 #(
    .INIT(4'h8)) 
    \and_ln112_reg_289[5]_i_1 
       (.I0(ap_NS_fsm1),
        .I1(icmp_ln107_fu_184_p2),
        .O(A_addr_1_reg_2840));
  LUT2 #(
    .INIT(4'h1)) 
    \and_ln112_reg_289[5]_i_10 
       (.I0(trunc_ln99_1_reg_256[26]),
        .I1(trunc_ln99_1_reg_256[27]),
        .O(\and_ln112_reg_289[5]_i_10_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \and_ln112_reg_289[5]_i_11 
       (.I0(trunc_ln99_1_reg_256[24]),
        .I1(trunc_ln99_1_reg_256[25]),
        .O(\and_ln112_reg_289[5]_i_11_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \and_ln112_reg_289[5]_i_13 
       (.I0(trunc_ln99_1_reg_256[22]),
        .I1(trunc_ln99_1_reg_256[23]),
        .O(\and_ln112_reg_289[5]_i_13_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \and_ln112_reg_289[5]_i_14 
       (.I0(trunc_ln99_1_reg_256[20]),
        .I1(trunc_ln99_1_reg_256[21]),
        .O(\and_ln112_reg_289[5]_i_14_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \and_ln112_reg_289[5]_i_15 
       (.I0(trunc_ln99_1_reg_256[18]),
        .I1(trunc_ln99_1_reg_256[19]),
        .O(\and_ln112_reg_289[5]_i_15_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \and_ln112_reg_289[5]_i_16 
       (.I0(trunc_ln99_1_reg_256[16]),
        .I1(trunc_ln99_1_reg_256[17]),
        .O(\and_ln112_reg_289[5]_i_16_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \and_ln112_reg_289[5]_i_17 
       (.I0(trunc_ln99_1_reg_256[22]),
        .I1(trunc_ln99_1_reg_256[23]),
        .O(\and_ln112_reg_289[5]_i_17_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \and_ln112_reg_289[5]_i_18 
       (.I0(trunc_ln99_1_reg_256[20]),
        .I1(trunc_ln99_1_reg_256[21]),
        .O(\and_ln112_reg_289[5]_i_18_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \and_ln112_reg_289[5]_i_19 
       (.I0(trunc_ln99_1_reg_256[18]),
        .I1(trunc_ln99_1_reg_256[19]),
        .O(\and_ln112_reg_289[5]_i_19_n_1 ));
  LUT2 #(
    .INIT(4'h8)) 
    \and_ln112_reg_289[5]_i_2 
       (.I0(\trunc_ln96_reg_242_reg_n_1_[5] ),
        .I1(i_0_reg_102[5]),
        .O(and_ln112_fu_200_p2[5]));
  LUT2 #(
    .INIT(4'h1)) 
    \and_ln112_reg_289[5]_i_20 
       (.I0(trunc_ln99_1_reg_256[16]),
        .I1(trunc_ln99_1_reg_256[17]),
        .O(\and_ln112_reg_289[5]_i_20_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \and_ln112_reg_289[5]_i_22 
       (.I0(trunc_ln99_1_reg_256[14]),
        .I1(trunc_ln99_1_reg_256[15]),
        .O(\and_ln112_reg_289[5]_i_22_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \and_ln112_reg_289[5]_i_23 
       (.I0(trunc_ln99_1_reg_256[12]),
        .I1(trunc_ln99_1_reg_256[13]),
        .O(\and_ln112_reg_289[5]_i_23_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \and_ln112_reg_289[5]_i_24 
       (.I0(trunc_ln99_1_reg_256[10]),
        .I1(trunc_ln99_1_reg_256[11]),
        .O(\and_ln112_reg_289[5]_i_24_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \and_ln112_reg_289[5]_i_25 
       (.I0(trunc_ln99_1_reg_256[8]),
        .I1(trunc_ln99_1_reg_256[9]),
        .O(\and_ln112_reg_289[5]_i_25_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \and_ln112_reg_289[5]_i_26 
       (.I0(trunc_ln99_1_reg_256[14]),
        .I1(trunc_ln99_1_reg_256[15]),
        .O(\and_ln112_reg_289[5]_i_26_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \and_ln112_reg_289[5]_i_27 
       (.I0(trunc_ln99_1_reg_256[12]),
        .I1(trunc_ln99_1_reg_256[13]),
        .O(\and_ln112_reg_289[5]_i_27_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \and_ln112_reg_289[5]_i_28 
       (.I0(trunc_ln99_1_reg_256[10]),
        .I1(trunc_ln99_1_reg_256[11]),
        .O(\and_ln112_reg_289[5]_i_28_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \and_ln112_reg_289[5]_i_29 
       (.I0(trunc_ln99_1_reg_256[8]),
        .I1(trunc_ln99_1_reg_256[9]),
        .O(\and_ln112_reg_289[5]_i_29_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT5 #(
    .INIT(32'hAAAAAA8A)) 
    \and_ln112_reg_289[5]_i_3 
       (.I0(ap_CS_fsm_state4),
        .I1(\icmp_ln102_reg_266[0]_i_2_n_1 ),
        .I2(i_0_reg_102[6]),
        .I3(i_0_reg_102[0]),
        .I4(i_0_reg_102[5]),
        .O(ap_NS_fsm1));
  LUT3 #(
    .INIT(8'hF2)) 
    \and_ln112_reg_289[5]_i_30 
       (.I0(trunc_ln99_1_reg_256[6]),
        .I1(i_0_reg_102[6]),
        .I2(trunc_ln99_1_reg_256[7]),
        .O(\and_ln112_reg_289[5]_i_30_n_1 ));
  LUT4 #(
    .INIT(16'h0F22)) 
    \and_ln112_reg_289[5]_i_31 
       (.I0(trunc_ln99_1_reg_256[4]),
        .I1(i_0_reg_102[4]),
        .I2(i_0_reg_102[5]),
        .I3(trunc_ln99_1_reg_256[5]),
        .O(\and_ln112_reg_289[5]_i_31_n_1 ));
  LUT4 #(
    .INIT(16'h0F22)) 
    \and_ln112_reg_289[5]_i_32 
       (.I0(trunc_ln99_1_reg_256[2]),
        .I1(i_0_reg_102[2]),
        .I2(i_0_reg_102[3]),
        .I3(trunc_ln99_1_reg_256[3]),
        .O(\and_ln112_reg_289[5]_i_32_n_1 ));
  LUT4 #(
    .INIT(16'h0F22)) 
    \and_ln112_reg_289[5]_i_33 
       (.I0(trunc_ln99_1_reg_256[0]),
        .I1(i_0_reg_102[0]),
        .I2(i_0_reg_102[1]),
        .I3(trunc_ln99_1_reg_256[1]),
        .O(\and_ln112_reg_289[5]_i_33_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \and_ln112_reg_289[5]_i_34 
       (.I0(trunc_ln99_1_reg_256[6]),
        .I1(trunc_ln99_1_reg_256[7]),
        .O(\and_ln112_reg_289[5]_i_34_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \and_ln112_reg_289[5]_i_35 
       (.I0(trunc_ln99_1_reg_256[4]),
        .I1(trunc_ln99_1_reg_256[5]),
        .O(\and_ln112_reg_289[5]_i_35_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \and_ln112_reg_289[5]_i_36 
       (.I0(trunc_ln99_1_reg_256[2]),
        .I1(trunc_ln99_1_reg_256[3]),
        .O(\and_ln112_reg_289[5]_i_36_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \and_ln112_reg_289[5]_i_37 
       (.I0(trunc_ln99_1_reg_256[0]),
        .I1(trunc_ln99_1_reg_256[1]),
        .O(\and_ln112_reg_289[5]_i_37_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \and_ln112_reg_289[5]_i_6 
       (.I0(trunc_ln99_1_reg_256[28]),
        .I1(trunc_ln99_1_reg_256[29]),
        .O(\and_ln112_reg_289[5]_i_6_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \and_ln112_reg_289[5]_i_7 
       (.I0(trunc_ln99_1_reg_256[26]),
        .I1(trunc_ln99_1_reg_256[27]),
        .O(\and_ln112_reg_289[5]_i_7_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \and_ln112_reg_289[5]_i_8 
       (.I0(trunc_ln99_1_reg_256[24]),
        .I1(trunc_ln99_1_reg_256[25]),
        .O(\and_ln112_reg_289[5]_i_8_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \and_ln112_reg_289[5]_i_9 
       (.I0(trunc_ln99_1_reg_256[28]),
        .I1(trunc_ln99_1_reg_256[29]),
        .O(\and_ln112_reg_289[5]_i_9_n_1 ));
  FDRE \and_ln112_reg_289_reg[1] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(and_ln112_fu_200_p2[1]),
        .Q(and_ln112_reg_289[1]),
        .R(1'b0));
  FDRE \and_ln112_reg_289_reg[2] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(and_ln112_fu_200_p2[2]),
        .Q(and_ln112_reg_289[2]),
        .R(1'b0));
  FDRE \and_ln112_reg_289_reg[3] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(and_ln112_fu_200_p2[3]),
        .Q(and_ln112_reg_289[3]),
        .R(1'b0));
  FDRE \and_ln112_reg_289_reg[4] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(and_ln112_fu_200_p2[4]),
        .Q(and_ln112_reg_289[4]),
        .R(1'b0));
  FDRE \and_ln112_reg_289_reg[5] 
       (.C(ap_clk),
        .CE(A_addr_1_reg_2840),
        .D(and_ln112_fu_200_p2[5]),
        .Q(and_ln112_reg_289[5]),
        .R(1'b0));
  CARRY4 \and_ln112_reg_289_reg[5]_i_12 
       (.CI(\and_ln112_reg_289_reg[5]_i_21_n_1 ),
        .CO({\and_ln112_reg_289_reg[5]_i_12_n_1 ,\and_ln112_reg_289_reg[5]_i_12_n_2 ,\and_ln112_reg_289_reg[5]_i_12_n_3 ,\and_ln112_reg_289_reg[5]_i_12_n_4 }),
        .CYINIT(1'b0),
        .DI({\and_ln112_reg_289[5]_i_22_n_1 ,\and_ln112_reg_289[5]_i_23_n_1 ,\and_ln112_reg_289[5]_i_24_n_1 ,\and_ln112_reg_289[5]_i_25_n_1 }),
        .O(\NLW_and_ln112_reg_289_reg[5]_i_12_O_UNCONNECTED [3:0]),
        .S({\and_ln112_reg_289[5]_i_26_n_1 ,\and_ln112_reg_289[5]_i_27_n_1 ,\and_ln112_reg_289[5]_i_28_n_1 ,\and_ln112_reg_289[5]_i_29_n_1 }));
  CARRY4 \and_ln112_reg_289_reg[5]_i_21 
       (.CI(1'b0),
        .CO({\and_ln112_reg_289_reg[5]_i_21_n_1 ,\and_ln112_reg_289_reg[5]_i_21_n_2 ,\and_ln112_reg_289_reg[5]_i_21_n_3 ,\and_ln112_reg_289_reg[5]_i_21_n_4 }),
        .CYINIT(1'b0),
        .DI({\and_ln112_reg_289[5]_i_30_n_1 ,\and_ln112_reg_289[5]_i_31_n_1 ,\and_ln112_reg_289[5]_i_32_n_1 ,\and_ln112_reg_289[5]_i_33_n_1 }),
        .O(\NLW_and_ln112_reg_289_reg[5]_i_21_O_UNCONNECTED [3:0]),
        .S({\and_ln112_reg_289[5]_i_34_n_1 ,\and_ln112_reg_289[5]_i_35_n_1 ,\and_ln112_reg_289[5]_i_36_n_1 ,\and_ln112_reg_289[5]_i_37_n_1 }));
  CARRY4 \and_ln112_reg_289_reg[5]_i_4 
       (.CI(\and_ln112_reg_289_reg[5]_i_5_n_1 ),
        .CO({\NLW_and_ln112_reg_289_reg[5]_i_4_CO_UNCONNECTED [3],icmp_ln107_fu_184_p2,\and_ln112_reg_289_reg[5]_i_4_n_3 ,\and_ln112_reg_289_reg[5]_i_4_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,\and_ln112_reg_289[5]_i_6_n_1 ,\and_ln112_reg_289[5]_i_7_n_1 ,\and_ln112_reg_289[5]_i_8_n_1 }),
        .O(\NLW_and_ln112_reg_289_reg[5]_i_4_O_UNCONNECTED [3:0]),
        .S({1'b0,\and_ln112_reg_289[5]_i_9_n_1 ,\and_ln112_reg_289[5]_i_10_n_1 ,\and_ln112_reg_289[5]_i_11_n_1 }));
  CARRY4 \and_ln112_reg_289_reg[5]_i_5 
       (.CI(\and_ln112_reg_289_reg[5]_i_12_n_1 ),
        .CO({\and_ln112_reg_289_reg[5]_i_5_n_1 ,\and_ln112_reg_289_reg[5]_i_5_n_2 ,\and_ln112_reg_289_reg[5]_i_5_n_3 ,\and_ln112_reg_289_reg[5]_i_5_n_4 }),
        .CYINIT(1'b0),
        .DI({\and_ln112_reg_289[5]_i_13_n_1 ,\and_ln112_reg_289[5]_i_14_n_1 ,\and_ln112_reg_289[5]_i_15_n_1 ,\and_ln112_reg_289[5]_i_16_n_1 }),
        .O(\NLW_and_ln112_reg_289_reg[5]_i_5_O_UNCONNECTED [3:0]),
        .S({\and_ln112_reg_289[5]_i_17_n_1 ,\and_ln112_reg_289[5]_i_18_n_1 ,\and_ln112_reg_289[5]_i_19_n_1 ,\and_ln112_reg_289[5]_i_20_n_1 }));
  LUT4 #(
    .INIT(16'h22F2)) 
    \ap_CS_fsm[0]_i_1 
       (.I0(\ap_CS_fsm_reg_n_1_[0] ),
        .I1(ap_start),
        .I2(ap_CS_fsm_state2),
        .I3(icmp_ln96_fu_113_p2),
        .O(ap_NS_fsm[0]));
  LUT4 #(
    .INIT(16'h88F8)) 
    \ap_CS_fsm[1]_i_1 
       (.I0(\ap_CS_fsm_reg_n_1_[0] ),
        .I1(ap_start),
        .I2(ap_CS_fsm_state3),
        .I3(icmp_ln99_fu_133_p2),
        .O(ap_NS_fsm[1]));
  LUT3 #(
    .INIT(8'hF8)) 
    \ap_CS_fsm[2]_i_1 
       (.I0(ap_CS_fsm_state2),
        .I1(icmp_ln96_fu_113_p2),
        .I2(ap_CS_fsm_state7),
        .O(ap_NS_fsm[2]));
  LUT3 #(
    .INIT(8'hF8)) 
    \ap_CS_fsm[3]_i_1 
       (.I0(ap_CS_fsm_state3),
        .I1(icmp_ln99_fu_133_p2),
        .I2(ap_CS_fsm_state6),
        .O(ap_NS_fsm[3]));
  LUT2 #(
    .INIT(4'h1)) 
    \ap_CS_fsm[3]_i_10 
       (.I0(trunc_ln1_fu_123_p4[24]),
        .I1(trunc_ln1_fu_123_p4[25]),
        .O(\ap_CS_fsm[3]_i_10_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \ap_CS_fsm[3]_i_12 
       (.I0(trunc_ln1_fu_123_p4[22]),
        .I1(trunc_ln1_fu_123_p4[23]),
        .O(\ap_CS_fsm[3]_i_12_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \ap_CS_fsm[3]_i_13 
       (.I0(trunc_ln1_fu_123_p4[20]),
        .I1(trunc_ln1_fu_123_p4[21]),
        .O(\ap_CS_fsm[3]_i_13_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \ap_CS_fsm[3]_i_14 
       (.I0(trunc_ln1_fu_123_p4[18]),
        .I1(trunc_ln1_fu_123_p4[19]),
        .O(\ap_CS_fsm[3]_i_14_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \ap_CS_fsm[3]_i_15 
       (.I0(trunc_ln1_fu_123_p4[16]),
        .I1(trunc_ln1_fu_123_p4[17]),
        .O(\ap_CS_fsm[3]_i_15_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \ap_CS_fsm[3]_i_16 
       (.I0(trunc_ln1_fu_123_p4[22]),
        .I1(trunc_ln1_fu_123_p4[23]),
        .O(\ap_CS_fsm[3]_i_16_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \ap_CS_fsm[3]_i_17 
       (.I0(trunc_ln1_fu_123_p4[20]),
        .I1(trunc_ln1_fu_123_p4[21]),
        .O(\ap_CS_fsm[3]_i_17_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \ap_CS_fsm[3]_i_18 
       (.I0(trunc_ln1_fu_123_p4[18]),
        .I1(trunc_ln1_fu_123_p4[19]),
        .O(\ap_CS_fsm[3]_i_18_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \ap_CS_fsm[3]_i_19 
       (.I0(trunc_ln1_fu_123_p4[16]),
        .I1(trunc_ln1_fu_123_p4[17]),
        .O(\ap_CS_fsm[3]_i_19_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \ap_CS_fsm[3]_i_21 
       (.I0(trunc_ln1_fu_123_p4[14]),
        .I1(trunc_ln1_fu_123_p4[15]),
        .O(\ap_CS_fsm[3]_i_21_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \ap_CS_fsm[3]_i_22 
       (.I0(trunc_ln1_fu_123_p4[12]),
        .I1(trunc_ln1_fu_123_p4[13]),
        .O(\ap_CS_fsm[3]_i_22_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \ap_CS_fsm[3]_i_23 
       (.I0(trunc_ln1_fu_123_p4[10]),
        .I1(trunc_ln1_fu_123_p4[11]),
        .O(\ap_CS_fsm[3]_i_23_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \ap_CS_fsm[3]_i_24 
       (.I0(trunc_ln1_fu_123_p4[8]),
        .I1(trunc_ln1_fu_123_p4[9]),
        .O(\ap_CS_fsm[3]_i_24_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \ap_CS_fsm[3]_i_25 
       (.I0(trunc_ln1_fu_123_p4[14]),
        .I1(trunc_ln1_fu_123_p4[15]),
        .O(\ap_CS_fsm[3]_i_25_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \ap_CS_fsm[3]_i_26 
       (.I0(trunc_ln1_fu_123_p4[12]),
        .I1(trunc_ln1_fu_123_p4[13]),
        .O(\ap_CS_fsm[3]_i_26_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \ap_CS_fsm[3]_i_27 
       (.I0(trunc_ln1_fu_123_p4[10]),
        .I1(trunc_ln1_fu_123_p4[11]),
        .O(\ap_CS_fsm[3]_i_27_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \ap_CS_fsm[3]_i_28 
       (.I0(trunc_ln1_fu_123_p4[8]),
        .I1(trunc_ln1_fu_123_p4[9]),
        .O(\ap_CS_fsm[3]_i_28_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \ap_CS_fsm[3]_i_29 
       (.I0(trunc_ln1_fu_123_p4[6]),
        .I1(trunc_ln1_fu_123_p4[7]),
        .O(\ap_CS_fsm[3]_i_29_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \ap_CS_fsm[3]_i_30 
       (.I0(trunc_ln1_fu_123_p4[4]),
        .I1(trunc_ln1_fu_123_p4[5]),
        .O(\ap_CS_fsm[3]_i_30_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \ap_CS_fsm[3]_i_31 
       (.I0(trunc_ln1_fu_123_p4[2]),
        .I1(trunc_ln1_fu_123_p4[3]),
        .O(\ap_CS_fsm[3]_i_31_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \ap_CS_fsm[3]_i_32 
       (.I0(trunc_ln1_fu_123_p4[0]),
        .I1(trunc_ln1_fu_123_p4[1]),
        .O(\ap_CS_fsm[3]_i_32_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \ap_CS_fsm[3]_i_33 
       (.I0(trunc_ln1_fu_123_p4[6]),
        .I1(trunc_ln1_fu_123_p4[7]),
        .O(\ap_CS_fsm[3]_i_33_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \ap_CS_fsm[3]_i_34 
       (.I0(trunc_ln1_fu_123_p4[4]),
        .I1(trunc_ln1_fu_123_p4[5]),
        .O(\ap_CS_fsm[3]_i_34_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \ap_CS_fsm[3]_i_35 
       (.I0(trunc_ln1_fu_123_p4[2]),
        .I1(trunc_ln1_fu_123_p4[3]),
        .O(\ap_CS_fsm[3]_i_35_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \ap_CS_fsm[3]_i_36 
       (.I0(trunc_ln1_fu_123_p4[0]),
        .I1(trunc_ln1_fu_123_p4[1]),
        .O(\ap_CS_fsm[3]_i_36_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \ap_CS_fsm[3]_i_4 
       (.I0(trunc_ln1_fu_123_p4[28]),
        .I1(trunc_ln1_fu_123_p4[29]),
        .O(\ap_CS_fsm[3]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \ap_CS_fsm[3]_i_5 
       (.I0(trunc_ln1_fu_123_p4[26]),
        .I1(trunc_ln1_fu_123_p4[27]),
        .O(\ap_CS_fsm[3]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'hE)) 
    \ap_CS_fsm[3]_i_6 
       (.I0(trunc_ln1_fu_123_p4[24]),
        .I1(trunc_ln1_fu_123_p4[25]),
        .O(\ap_CS_fsm[3]_i_6_n_1 ));
  LUT1 #(
    .INIT(2'h1)) 
    \ap_CS_fsm[3]_i_7 
       (.I0(trunc_ln1_fu_123_p4[30]),
        .O(\ap_CS_fsm[3]_i_7_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \ap_CS_fsm[3]_i_8 
       (.I0(trunc_ln1_fu_123_p4[28]),
        .I1(trunc_ln1_fu_123_p4[29]),
        .O(\ap_CS_fsm[3]_i_8_n_1 ));
  LUT2 #(
    .INIT(4'h1)) 
    \ap_CS_fsm[3]_i_9 
       (.I0(trunc_ln1_fu_123_p4[26]),
        .I1(trunc_ln1_fu_123_p4[27]),
        .O(\ap_CS_fsm[3]_i_9_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair29" *) 
  LUT2 #(
    .INIT(4'h8)) 
    \ap_CS_fsm[4]_i_1 
       (.I0(ap_CS_fsm_state4),
        .I1(ap_NS_fsm1),
        .O(ap_NS_fsm[4]));
  LUT2 #(
    .INIT(4'h2)) 
    \ap_CS_fsm[6]_i_1 
       (.I0(ap_CS_fsm_state4),
        .I1(ap_NS_fsm1),
        .O(\ap_CS_fsm[6]_i_1_n_1 ));
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
    \ap_CS_fsm_reg[1] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_NS_fsm[1]),
        .Q(ap_CS_fsm_state2),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_NS_fsm[2]),
        .Q(ap_CS_fsm_state3),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[3] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_NS_fsm[3]),
        .Q(ap_CS_fsm_state4),
        .R(ap_rst));
  CARRY4 \ap_CS_fsm_reg[3]_i_11 
       (.CI(\ap_CS_fsm_reg[3]_i_20_n_1 ),
        .CO({\ap_CS_fsm_reg[3]_i_11_n_1 ,\ap_CS_fsm_reg[3]_i_11_n_2 ,\ap_CS_fsm_reg[3]_i_11_n_3 ,\ap_CS_fsm_reg[3]_i_11_n_4 }),
        .CYINIT(1'b0),
        .DI({\ap_CS_fsm[3]_i_21_n_1 ,\ap_CS_fsm[3]_i_22_n_1 ,\ap_CS_fsm[3]_i_23_n_1 ,\ap_CS_fsm[3]_i_24_n_1 }),
        .O(\NLW_ap_CS_fsm_reg[3]_i_11_O_UNCONNECTED [3:0]),
        .S({\ap_CS_fsm[3]_i_25_n_1 ,\ap_CS_fsm[3]_i_26_n_1 ,\ap_CS_fsm[3]_i_27_n_1 ,\ap_CS_fsm[3]_i_28_n_1 }));
  CARRY4 \ap_CS_fsm_reg[3]_i_2 
       (.CI(\ap_CS_fsm_reg[3]_i_3_n_1 ),
        .CO({icmp_ln99_fu_133_p2,\ap_CS_fsm_reg[3]_i_2_n_2 ,\ap_CS_fsm_reg[3]_i_2_n_3 ,\ap_CS_fsm_reg[3]_i_2_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,\ap_CS_fsm[3]_i_4_n_1 ,\ap_CS_fsm[3]_i_5_n_1 ,\ap_CS_fsm[3]_i_6_n_1 }),
        .O(\NLW_ap_CS_fsm_reg[3]_i_2_O_UNCONNECTED [3:0]),
        .S({\ap_CS_fsm[3]_i_7_n_1 ,\ap_CS_fsm[3]_i_8_n_1 ,\ap_CS_fsm[3]_i_9_n_1 ,\ap_CS_fsm[3]_i_10_n_1 }));
  CARRY4 \ap_CS_fsm_reg[3]_i_20 
       (.CI(1'b0),
        .CO({\ap_CS_fsm_reg[3]_i_20_n_1 ,\ap_CS_fsm_reg[3]_i_20_n_2 ,\ap_CS_fsm_reg[3]_i_20_n_3 ,\ap_CS_fsm_reg[3]_i_20_n_4 }),
        .CYINIT(1'b0),
        .DI({\ap_CS_fsm[3]_i_29_n_1 ,\ap_CS_fsm[3]_i_30_n_1 ,\ap_CS_fsm[3]_i_31_n_1 ,\ap_CS_fsm[3]_i_32_n_1 }),
        .O(\NLW_ap_CS_fsm_reg[3]_i_20_O_UNCONNECTED [3:0]),
        .S({\ap_CS_fsm[3]_i_33_n_1 ,\ap_CS_fsm[3]_i_34_n_1 ,\ap_CS_fsm[3]_i_35_n_1 ,\ap_CS_fsm[3]_i_36_n_1 }));
  CARRY4 \ap_CS_fsm_reg[3]_i_3 
       (.CI(\ap_CS_fsm_reg[3]_i_11_n_1 ),
        .CO({\ap_CS_fsm_reg[3]_i_3_n_1 ,\ap_CS_fsm_reg[3]_i_3_n_2 ,\ap_CS_fsm_reg[3]_i_3_n_3 ,\ap_CS_fsm_reg[3]_i_3_n_4 }),
        .CYINIT(1'b0),
        .DI({\ap_CS_fsm[3]_i_12_n_1 ,\ap_CS_fsm[3]_i_13_n_1 ,\ap_CS_fsm[3]_i_14_n_1 ,\ap_CS_fsm[3]_i_15_n_1 }),
        .O(\NLW_ap_CS_fsm_reg[3]_i_3_O_UNCONNECTED [3:0]),
        .S({\ap_CS_fsm[3]_i_16_n_1 ,\ap_CS_fsm[3]_i_17_n_1 ,\ap_CS_fsm[3]_i_18_n_1 ,\ap_CS_fsm[3]_i_19_n_1 }));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[4] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_NS_fsm[4]),
        .Q(ap_CS_fsm_state5),
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
    \ap_CS_fsm_reg[6] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm[6]_i_1_n_1 ),
        .Q(ap_CS_fsm_state7),
        .R(ap_rst));
  LUT2 #(
    .INIT(4'h2)) 
    ap_idle_INST_0
       (.I0(\ap_CS_fsm_reg_n_1_[0] ),
        .I1(ap_start),
        .O(ap_idle));
  LUT2 #(
    .INIT(4'h2)) 
    ap_ready_INST_0
       (.I0(ap_CS_fsm_state2),
        .I1(icmp_ln96_fu_113_p2),
        .O(ap_ready));
  CARRY4 ap_ready_INST_0_i_1
       (.CI(ap_ready_INST_0_i_2_n_1),
        .CO({NLW_ap_ready_INST_0_i_1_CO_UNCONNECTED[3],icmp_ln96_fu_113_p2,ap_ready_INST_0_i_1_n_3,ap_ready_INST_0_i_1_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,trunc_ln96_fu_119_p1[31],1'b0,1'b0}),
        .O(NLW_ap_ready_INST_0_i_1_O_UNCONNECTED[3:0]),
        .S({1'b0,ap_ready_INST_0_i_3_n_1,ap_ready_INST_0_i_4_n_1,ap_ready_INST_0_i_5_n_1}));
  LUT2 #(
    .INIT(4'h1)) 
    ap_ready_INST_0_i_10
       (.I0(trunc_ln96_fu_119_p1[18]),
        .I1(trunc_ln96_fu_119_p1[19]),
        .O(ap_ready_INST_0_i_10_n_1));
  CARRY4 ap_ready_INST_0_i_11
       (.CI(1'b0),
        .CO({ap_ready_INST_0_i_11_n_1,ap_ready_INST_0_i_11_n_2,ap_ready_INST_0_i_11_n_3,ap_ready_INST_0_i_11_n_4}),
        .CYINIT(ap_ready_INST_0_i_16_n_1),
        .DI({1'b0,ap_ready_INST_0_i_17_n_1,1'b0,1'b0}),
        .O(NLW_ap_ready_INST_0_i_11_O_UNCONNECTED[3:0]),
        .S({ap_ready_INST_0_i_18_n_1,ap_ready_INST_0_i_19_n_1,ap_ready_INST_0_i_20_n_1,ap_ready_INST_0_i_21_n_1}));
  LUT2 #(
    .INIT(4'h1)) 
    ap_ready_INST_0_i_12
       (.I0(trunc_ln96_fu_119_p1[16]),
        .I1(trunc_ln96_fu_119_p1[17]),
        .O(ap_ready_INST_0_i_12_n_1));
  LUT2 #(
    .INIT(4'h1)) 
    ap_ready_INST_0_i_13
       (.I0(trunc_ln96_fu_119_p1[14]),
        .I1(trunc_ln96_fu_119_p1[15]),
        .O(ap_ready_INST_0_i_13_n_1));
  LUT2 #(
    .INIT(4'h1)) 
    ap_ready_INST_0_i_14
       (.I0(trunc_ln96_fu_119_p1[12]),
        .I1(trunc_ln96_fu_119_p1[13]),
        .O(ap_ready_INST_0_i_14_n_1));
  LUT2 #(
    .INIT(4'h1)) 
    ap_ready_INST_0_i_15
       (.I0(trunc_ln96_fu_119_p1[10]),
        .I1(trunc_ln96_fu_119_p1[11]),
        .O(ap_ready_INST_0_i_15_n_1));
  LUT1 #(
    .INIT(2'h1)) 
    ap_ready_INST_0_i_16
       (.I0(trunc_ln96_fu_119_p1[1]),
        .O(ap_ready_INST_0_i_16_n_1));
  LUT2 #(
    .INIT(4'h1)) 
    ap_ready_INST_0_i_17
       (.I0(trunc_ln96_fu_119_p1[6]),
        .I1(trunc_ln96_fu_119_p1[7]),
        .O(ap_ready_INST_0_i_17_n_1));
  LUT2 #(
    .INIT(4'h1)) 
    ap_ready_INST_0_i_18
       (.I0(trunc_ln96_fu_119_p1[8]),
        .I1(trunc_ln96_fu_119_p1[9]),
        .O(ap_ready_INST_0_i_18_n_1));
  LUT2 #(
    .INIT(4'h2)) 
    ap_ready_INST_0_i_19
       (.I0(trunc_ln96_fu_119_p1[6]),
        .I1(trunc_ln96_fu_119_p1[7]),
        .O(ap_ready_INST_0_i_19_n_1));
  CARRY4 ap_ready_INST_0_i_2
       (.CI(ap_ready_INST_0_i_6_n_1),
        .CO({ap_ready_INST_0_i_2_n_1,ap_ready_INST_0_i_2_n_2,ap_ready_INST_0_i_2_n_3,ap_ready_INST_0_i_2_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(NLW_ap_ready_INST_0_i_2_O_UNCONNECTED[3:0]),
        .S({ap_ready_INST_0_i_7_n_1,ap_ready_INST_0_i_8_n_1,ap_ready_INST_0_i_9_n_1,ap_ready_INST_0_i_10_n_1}));
  LUT2 #(
    .INIT(4'h1)) 
    ap_ready_INST_0_i_20
       (.I0(trunc_ln96_fu_119_p1[4]),
        .I1(trunc_ln96_fu_119_p1[5]),
        .O(ap_ready_INST_0_i_20_n_1));
  LUT2 #(
    .INIT(4'h1)) 
    ap_ready_INST_0_i_21
       (.I0(trunc_ln96_fu_119_p1[2]),
        .I1(trunc_ln96_fu_119_p1[3]),
        .O(ap_ready_INST_0_i_21_n_1));
  LUT2 #(
    .INIT(4'h1)) 
    ap_ready_INST_0_i_3
       (.I0(trunc_ln96_fu_119_p1[30]),
        .I1(trunc_ln96_fu_119_p1[31]),
        .O(ap_ready_INST_0_i_3_n_1));
  LUT2 #(
    .INIT(4'h1)) 
    ap_ready_INST_0_i_4
       (.I0(trunc_ln96_fu_119_p1[28]),
        .I1(trunc_ln96_fu_119_p1[29]),
        .O(ap_ready_INST_0_i_4_n_1));
  LUT2 #(
    .INIT(4'h1)) 
    ap_ready_INST_0_i_5
       (.I0(trunc_ln96_fu_119_p1[26]),
        .I1(trunc_ln96_fu_119_p1[27]),
        .O(ap_ready_INST_0_i_5_n_1));
  CARRY4 ap_ready_INST_0_i_6
       (.CI(ap_ready_INST_0_i_11_n_1),
        .CO({ap_ready_INST_0_i_6_n_1,ap_ready_INST_0_i_6_n_2,ap_ready_INST_0_i_6_n_3,ap_ready_INST_0_i_6_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(NLW_ap_ready_INST_0_i_6_O_UNCONNECTED[3:0]),
        .S({ap_ready_INST_0_i_12_n_1,ap_ready_INST_0_i_13_n_1,ap_ready_INST_0_i_14_n_1,ap_ready_INST_0_i_15_n_1}));
  LUT2 #(
    .INIT(4'h1)) 
    ap_ready_INST_0_i_7
       (.I0(trunc_ln96_fu_119_p1[24]),
        .I1(trunc_ln96_fu_119_p1[25]),
        .O(ap_ready_INST_0_i_7_n_1));
  LUT2 #(
    .INIT(4'h1)) 
    ap_ready_INST_0_i_8
       (.I0(trunc_ln96_fu_119_p1[22]),
        .I1(trunc_ln96_fu_119_p1[23]),
        .O(ap_ready_INST_0_i_8_n_1));
  LUT2 #(
    .INIT(4'h1)) 
    ap_ready_INST_0_i_9
       (.I0(trunc_ln96_fu_119_p1[20]),
        .I1(trunc_ln96_fu_119_p1[21]),
        .O(ap_ready_INST_0_i_9_n_1));
  LUT2 #(
    .INIT(4'h8)) 
    \i_0_reg_102[6]_i_1 
       (.I0(ap_CS_fsm_state3),
        .I1(icmp_ln99_fu_133_p2),
        .O(i_0_reg_1020));
  LUT2 #(
    .INIT(4'h2)) 
    \i_0_reg_102[6]_i_2 
       (.I0(ap_CS_fsm_state6),
        .I1(\icmp_ln102_reg_266_reg_n_1_[0] ),
        .O(\i_0_reg_102[6]_i_2_n_1 ));
  FDRE \i_0_reg_102_reg[0] 
       (.C(ap_clk),
        .CE(\i_0_reg_102[6]_i_2_n_1 ),
        .D(i_reg_270[0]),
        .Q(i_0_reg_102[0]),
        .R(i_0_reg_1020));
  FDRE \i_0_reg_102_reg[1] 
       (.C(ap_clk),
        .CE(\i_0_reg_102[6]_i_2_n_1 ),
        .D(i_reg_270[1]),
        .Q(i_0_reg_102[1]),
        .R(i_0_reg_1020));
  FDRE \i_0_reg_102_reg[2] 
       (.C(ap_clk),
        .CE(\i_0_reg_102[6]_i_2_n_1 ),
        .D(i_reg_270[2]),
        .Q(i_0_reg_102[2]),
        .R(i_0_reg_1020));
  FDRE \i_0_reg_102_reg[3] 
       (.C(ap_clk),
        .CE(\i_0_reg_102[6]_i_2_n_1 ),
        .D(i_reg_270[3]),
        .Q(i_0_reg_102[3]),
        .R(i_0_reg_1020));
  FDRE \i_0_reg_102_reg[4] 
       (.C(ap_clk),
        .CE(\i_0_reg_102[6]_i_2_n_1 ),
        .D(i_reg_270[4]),
        .Q(i_0_reg_102[4]),
        .R(i_0_reg_1020));
  FDRE \i_0_reg_102_reg[5] 
       (.C(ap_clk),
        .CE(\i_0_reg_102[6]_i_2_n_1 ),
        .D(i_reg_270[5]),
        .Q(i_0_reg_102[5]),
        .R(i_0_reg_1020));
  FDRE \i_0_reg_102_reg[6] 
       (.C(ap_clk),
        .CE(\i_0_reg_102[6]_i_2_n_1 ),
        .D(i_reg_270[6]),
        .Q(i_0_reg_102[6]),
        .R(i_0_reg_1020));
  LUT1 #(
    .INIT(2'h1)) 
    \i_reg_270[0]_i_1 
       (.I0(i_0_reg_102[0]),
        .O(i_fu_165_p2[0]));
  (* SOFT_HLUTNM = "soft_lutpair16" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \i_reg_270[1]_i_1 
       (.I0(i_0_reg_102[0]),
        .I1(i_0_reg_102[1]),
        .O(i_fu_165_p2[1]));
  (* SOFT_HLUTNM = "soft_lutpair16" *) 
  LUT3 #(
    .INIT(8'h78)) 
    \i_reg_270[2]_i_1 
       (.I0(i_0_reg_102[0]),
        .I1(i_0_reg_102[1]),
        .I2(i_0_reg_102[2]),
        .O(i_fu_165_p2[2]));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT4 #(
    .INIT(16'h7F80)) 
    \i_reg_270[3]_i_1 
       (.I0(i_0_reg_102[1]),
        .I1(i_0_reg_102[0]),
        .I2(i_0_reg_102[2]),
        .I3(i_0_reg_102[3]),
        .O(i_fu_165_p2[3]));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'h7FFF8000)) 
    \i_reg_270[4]_i_1 
       (.I0(i_0_reg_102[2]),
        .I1(i_0_reg_102[0]),
        .I2(i_0_reg_102[1]),
        .I3(i_0_reg_102[3]),
        .I4(i_0_reg_102[4]),
        .O(i_fu_165_p2[4]));
  LUT6 #(
    .INIT(64'h7FFFFFFF80000000)) 
    \i_reg_270[5]_i_1 
       (.I0(i_0_reg_102[3]),
        .I1(i_0_reg_102[1]),
        .I2(i_0_reg_102[0]),
        .I3(i_0_reg_102[2]),
        .I4(i_0_reg_102[4]),
        .I5(i_0_reg_102[5]),
        .O(i_fu_165_p2[5]));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT3 #(
    .INIT(8'h78)) 
    \i_reg_270[6]_i_1 
       (.I0(\i_reg_270[6]_i_2_n_1 ),
        .I1(i_0_reg_102[5]),
        .I2(i_0_reg_102[6]),
        .O(i_fu_165_p2[6]));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'h80000000)) 
    \i_reg_270[6]_i_2 
       (.I0(i_0_reg_102[4]),
        .I1(i_0_reg_102[2]),
        .I2(i_0_reg_102[0]),
        .I3(i_0_reg_102[1]),
        .I4(i_0_reg_102[3]),
        .O(\i_reg_270[6]_i_2_n_1 ));
  FDRE \i_reg_270_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(i_fu_165_p2[0]),
        .Q(i_reg_270[0]),
        .R(1'b0));
  FDRE \i_reg_270_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(i_fu_165_p2[1]),
        .Q(i_reg_270[1]),
        .R(1'b0));
  FDRE \i_reg_270_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(i_fu_165_p2[2]),
        .Q(i_reg_270[2]),
        .R(1'b0));
  FDRE \i_reg_270_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(i_fu_165_p2[3]),
        .Q(i_reg_270[3]),
        .R(1'b0));
  FDRE \i_reg_270_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(i_fu_165_p2[4]),
        .Q(i_reg_270[4]),
        .R(1'b0));
  FDRE \i_reg_270_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(i_fu_165_p2[5]),
        .Q(i_reg_270[5]),
        .R(1'b0));
  FDRE \i_reg_270_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(i_fu_165_p2[6]),
        .Q(i_reg_270[6]),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT4 #(
    .INIT(16'h0010)) 
    \icmp_ln102_reg_266[0]_i_1 
       (.I0(i_0_reg_102[5]),
        .I1(i_0_reg_102[0]),
        .I2(i_0_reg_102[6]),
        .I3(\icmp_ln102_reg_266[0]_i_2_n_1 ),
        .O(icmp_ln102_fu_159_p2));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT4 #(
    .INIT(16'hFFFE)) 
    \icmp_ln102_reg_266[0]_i_2 
       (.I0(i_0_reg_102[3]),
        .I1(i_0_reg_102[4]),
        .I2(i_0_reg_102[1]),
        .I3(i_0_reg_102[2]),
        .O(\icmp_ln102_reg_266[0]_i_2_n_1 ));
  FDRE \icmp_ln102_reg_266_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(icmp_ln102_fu_159_p2),
        .Q(\icmp_ln102_reg_266_reg_n_1_[0] ),
        .R(1'b0));
  LUT3 #(
    .INIT(8'hB8)) 
    \icmp_ln107_reg_275[0]_i_1 
       (.I0(icmp_ln107_fu_184_p2),
        .I1(ap_NS_fsm1),
        .I2(icmp_ln107_reg_275),
        .O(\icmp_ln107_reg_275[0]_i_1_n_1 ));
  FDRE \icmp_ln107_reg_275_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\icmp_ln107_reg_275[0]_i_1_n_1 ),
        .Q(icmp_ln107_reg_275),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair19" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[10]_i_1 
       (.I0(trunc_ln99_1_reg_256[10]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[10]),
        .O(p_1_in[10]));
  (* SOFT_HLUTNM = "soft_lutpair20" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[11]_i_1 
       (.I0(trunc_ln99_1_reg_256[11]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[11]),
        .O(p_1_in[11]));
  (* SOFT_HLUTNM = "soft_lutpair21" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[12]_i_1 
       (.I0(trunc_ln99_1_reg_256[12]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[12]),
        .O(p_1_in[12]));
  (* SOFT_HLUTNM = "soft_lutpair21" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[13]_i_1 
       (.I0(trunc_ln99_1_reg_256[13]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[13]),
        .O(p_1_in[13]));
  (* SOFT_HLUTNM = "soft_lutpair22" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[14]_i_1 
       (.I0(trunc_ln99_1_reg_256[14]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[14]),
        .O(p_1_in[14]));
  (* SOFT_HLUTNM = "soft_lutpair22" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[15]_i_1 
       (.I0(trunc_ln99_1_reg_256[15]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[15]),
        .O(p_1_in[15]));
  (* SOFT_HLUTNM = "soft_lutpair23" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[16]_i_1 
       (.I0(trunc_ln99_1_reg_256[16]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[16]),
        .O(p_1_in[16]));
  (* SOFT_HLUTNM = "soft_lutpair23" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[17]_i_1 
       (.I0(trunc_ln99_1_reg_256[17]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[17]),
        .O(p_1_in[17]));
  (* SOFT_HLUTNM = "soft_lutpair24" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[18]_i_1 
       (.I0(trunc_ln99_1_reg_256[18]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[18]),
        .O(p_1_in[18]));
  (* SOFT_HLUTNM = "soft_lutpair24" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[19]_i_1 
       (.I0(trunc_ln99_1_reg_256[19]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[19]),
        .O(p_1_in[19]));
  (* SOFT_HLUTNM = "soft_lutpair10" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[1]_i_1 
       (.I0(trunc_ln99_1_reg_256[1]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[1]),
        .O(p_1_in[1]));
  (* SOFT_HLUTNM = "soft_lutpair25" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[20]_i_1 
       (.I0(trunc_ln99_1_reg_256[20]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[20]),
        .O(p_1_in[20]));
  (* SOFT_HLUTNM = "soft_lutpair12" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[21]_i_1 
       (.I0(trunc_ln99_1_reg_256[21]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[21]),
        .O(p_1_in[21]));
  (* SOFT_HLUTNM = "soft_lutpair11" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[22]_i_1 
       (.I0(trunc_ln99_1_reg_256[22]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[22]),
        .O(p_1_in[22]));
  (* SOFT_HLUTNM = "soft_lutpair20" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[23]_i_1 
       (.I0(trunc_ln99_1_reg_256[23]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[23]),
        .O(p_1_in[23]));
  (* SOFT_HLUTNM = "soft_lutpair25" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[24]_i_1 
       (.I0(trunc_ln99_1_reg_256[24]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[24]),
        .O(p_1_in[24]));
  (* SOFT_HLUTNM = "soft_lutpair26" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[25]_i_1 
       (.I0(trunc_ln99_1_reg_256[25]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[25]),
        .O(p_1_in[25]));
  (* SOFT_HLUTNM = "soft_lutpair27" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[26]_i_1 
       (.I0(trunc_ln99_1_reg_256[26]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[26]),
        .O(p_1_in[26]));
  (* SOFT_HLUTNM = "soft_lutpair26" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[27]_i_1 
       (.I0(trunc_ln99_1_reg_256[27]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[27]),
        .O(p_1_in[27]));
  (* SOFT_HLUTNM = "soft_lutpair27" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[28]_i_1 
       (.I0(trunc_ln99_1_reg_256[28]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[28]),
        .O(p_1_in[28]));
  (* SOFT_HLUTNM = "soft_lutpair28" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[29]_i_1 
       (.I0(trunc_ln99_1_reg_256[29]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[29]),
        .O(p_1_in[29]));
  (* SOFT_HLUTNM = "soft_lutpair11" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[2]_i_1 
       (.I0(trunc_ln99_1_reg_256[2]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[2]),
        .O(p_1_in[2]));
  (* SOFT_HLUTNM = "soft_lutpair28" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[30]_i_1 
       (.I0(j_reg_251_reg),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[30]),
        .O(p_1_in[30]));
  LUT3 #(
    .INIT(8'hEA)) 
    \j_0_in_reg_92[31]_i_1 
       (.I0(ap_CS_fsm_state7),
        .I1(icmp_ln96_fu_113_p2),
        .I2(ap_CS_fsm_state2),
        .O(\j_0_in_reg_92[31]_i_1_n_1 ));
  LUT2 #(
    .INIT(4'h2)) 
    \j_0_in_reg_92[31]_i_2 
       (.I0(trunc_ln96_fu_119_p1[31]),
        .I1(ap_CS_fsm_state7),
        .O(\j_0_in_reg_92[31]_i_2_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair12" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[3]_i_1 
       (.I0(trunc_ln99_1_reg_256[3]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[3]),
        .O(p_1_in[3]));
  (* SOFT_HLUTNM = "soft_lutpair17" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[4]_i_1 
       (.I0(trunc_ln99_1_reg_256[4]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[4]),
        .O(p_1_in[4]));
  (* SOFT_HLUTNM = "soft_lutpair17" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[5]_i_1 
       (.I0(trunc_ln99_1_reg_256[5]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[5]),
        .O(p_1_in[5]));
  (* SOFT_HLUTNM = "soft_lutpair18" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[6]_i_1 
       (.I0(trunc_ln99_1_reg_256[6]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[6]),
        .O(p_1_in[6]));
  (* SOFT_HLUTNM = "soft_lutpair18" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[7]_i_1 
       (.I0(trunc_ln99_1_reg_256[7]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[7]),
        .O(p_1_in[7]));
  (* SOFT_HLUTNM = "soft_lutpair19" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[8]_i_1 
       (.I0(trunc_ln99_1_reg_256[8]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[8]),
        .O(p_1_in[8]));
  (* SOFT_HLUTNM = "soft_lutpair10" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \j_0_in_reg_92[9]_i_1 
       (.I0(trunc_ln99_1_reg_256[9]),
        .I1(ap_CS_fsm_state7),
        .I2(trunc_ln96_fu_119_p1[9]),
        .O(p_1_in[9]));
  FDRE \j_0_in_reg_92_reg[10] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[10]),
        .Q(trunc_ln1_fu_123_p4[9]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[11] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[11]),
        .Q(trunc_ln1_fu_123_p4[10]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[12] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[12]),
        .Q(trunc_ln1_fu_123_p4[11]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[13] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[13]),
        .Q(trunc_ln1_fu_123_p4[12]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[14] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[14]),
        .Q(trunc_ln1_fu_123_p4[13]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[15] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[15]),
        .Q(trunc_ln1_fu_123_p4[14]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[16] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[16]),
        .Q(trunc_ln1_fu_123_p4[15]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[17] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[17]),
        .Q(trunc_ln1_fu_123_p4[16]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[18] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[18]),
        .Q(trunc_ln1_fu_123_p4[17]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[19] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[19]),
        .Q(trunc_ln1_fu_123_p4[18]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[1] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[1]),
        .Q(trunc_ln1_fu_123_p4[0]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[20] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[20]),
        .Q(trunc_ln1_fu_123_p4[19]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[21] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[21]),
        .Q(trunc_ln1_fu_123_p4[20]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[22] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[22]),
        .Q(trunc_ln1_fu_123_p4[21]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[23] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[23]),
        .Q(trunc_ln1_fu_123_p4[22]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[24] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[24]),
        .Q(trunc_ln1_fu_123_p4[23]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[25] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[25]),
        .Q(trunc_ln1_fu_123_p4[24]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[26] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[26]),
        .Q(trunc_ln1_fu_123_p4[25]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[27] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[27]),
        .Q(trunc_ln1_fu_123_p4[26]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[28] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[28]),
        .Q(trunc_ln1_fu_123_p4[27]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[29] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[29]),
        .Q(trunc_ln1_fu_123_p4[28]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[2] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[2]),
        .Q(trunc_ln1_fu_123_p4[1]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[30] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[30]),
        .Q(trunc_ln1_fu_123_p4[29]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[31] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(\j_0_in_reg_92[31]_i_2_n_1 ),
        .Q(trunc_ln1_fu_123_p4[30]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[3] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[3]),
        .Q(trunc_ln1_fu_123_p4[2]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[4] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[4]),
        .Q(trunc_ln1_fu_123_p4[3]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[5] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[5]),
        .Q(trunc_ln1_fu_123_p4[4]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[6] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[6]),
        .Q(trunc_ln1_fu_123_p4[5]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[7] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[7]),
        .Q(trunc_ln1_fu_123_p4[6]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[8] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[8]),
        .Q(trunc_ln1_fu_123_p4[7]),
        .R(1'b0));
  FDRE \j_0_in_reg_92_reg[9] 
       (.C(ap_clk),
        .CE(\j_0_in_reg_92[31]_i_1_n_1 ),
        .D(p_1_in[9]),
        .Q(trunc_ln1_fu_123_p4[8]),
        .R(1'b0));
  FDRE \j_reg_251_reg[30] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[30]),
        .Q(j_reg_251_reg),
        .R(1'b0));
  LUT4 #(
    .INIT(16'h8088)) 
    \k_0_reg_80[31]_i_1 
       (.I0(ap_start),
        .I1(\ap_CS_fsm_reg_n_1_[0] ),
        .I2(icmp_ln99_fu_133_p2),
        .I3(ap_CS_fsm_state3),
        .O(k_0_reg_80));
  LUT2 #(
    .INIT(4'h2)) 
    \k_0_reg_80[31]_i_2 
       (.I0(ap_CS_fsm_state3),
        .I1(icmp_ln99_fu_133_p2),
        .O(ap_NS_fsm13_out));
  FDRE \k_0_reg_80_reg[10] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[9]),
        .Q(trunc_ln96_fu_119_p1[10]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[11] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[10]),
        .Q(trunc_ln96_fu_119_p1[11]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[12] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[11]),
        .Q(trunc_ln96_fu_119_p1[12]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[13] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[12]),
        .Q(trunc_ln96_fu_119_p1[13]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[14] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[13]),
        .Q(trunc_ln96_fu_119_p1[14]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[15] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[14]),
        .Q(trunc_ln96_fu_119_p1[15]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[16] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[15]),
        .Q(trunc_ln96_fu_119_p1[16]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[17] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[16]),
        .Q(trunc_ln96_fu_119_p1[17]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[18] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[17]),
        .Q(trunc_ln96_fu_119_p1[18]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[19] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[18]),
        .Q(trunc_ln96_fu_119_p1[19]),
        .R(k_0_reg_80));
  FDSE \k_0_reg_80_reg[1] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(1'b0),
        .Q(trunc_ln96_fu_119_p1[1]),
        .S(k_0_reg_80));
  FDRE \k_0_reg_80_reg[20] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[19]),
        .Q(trunc_ln96_fu_119_p1[20]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[21] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[20]),
        .Q(trunc_ln96_fu_119_p1[21]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[22] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[21]),
        .Q(trunc_ln96_fu_119_p1[22]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[23] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[22]),
        .Q(trunc_ln96_fu_119_p1[23]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[24] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[23]),
        .Q(trunc_ln96_fu_119_p1[24]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[25] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[24]),
        .Q(trunc_ln96_fu_119_p1[25]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[26] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[25]),
        .Q(trunc_ln96_fu_119_p1[26]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[27] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[26]),
        .Q(trunc_ln96_fu_119_p1[27]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[28] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[27]),
        .Q(trunc_ln96_fu_119_p1[28]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[29] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[28]),
        .Q(trunc_ln96_fu_119_p1[29]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[2] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[1]),
        .Q(trunc_ln96_fu_119_p1[2]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[30] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[29]),
        .Q(trunc_ln96_fu_119_p1[30]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[31] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[30]),
        .Q(trunc_ln96_fu_119_p1[31]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[3] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[2]),
        .Q(trunc_ln96_fu_119_p1[3]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[4] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[3]),
        .Q(trunc_ln96_fu_119_p1[4]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[5] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[4]),
        .Q(trunc_ln96_fu_119_p1[5]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[6] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[5]),
        .Q(trunc_ln96_fu_119_p1[6]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[7] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[6]),
        .Q(trunc_ln96_fu_119_p1[7]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[8] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[7]),
        .Q(trunc_ln96_fu_119_p1[8]),
        .R(k_0_reg_80));
  FDRE \k_0_reg_80_reg[9] 
       (.C(ap_clk),
        .CE(ap_NS_fsm13_out),
        .D(trunc_ln96_fu_119_p1[8]),
        .Q(trunc_ln96_fu_119_p1[9]),
        .R(k_0_reg_80));
  LUT2 #(
    .INIT(4'h8)) 
    \trunc_ln96_reg_242[5]_i_1 
       (.I0(ap_CS_fsm_state2),
        .I1(icmp_ln96_fu_113_p2),
        .O(j_0_in_reg_920));
  FDRE \trunc_ln96_reg_242_reg[1] 
       (.C(ap_clk),
        .CE(j_0_in_reg_920),
        .D(trunc_ln96_fu_119_p1[1]),
        .Q(\trunc_ln96_reg_242_reg_n_1_[1] ),
        .R(1'b0));
  FDRE \trunc_ln96_reg_242_reg[2] 
       (.C(ap_clk),
        .CE(j_0_in_reg_920),
        .D(trunc_ln96_fu_119_p1[2]),
        .Q(\trunc_ln96_reg_242_reg_n_1_[2] ),
        .R(1'b0));
  FDRE \trunc_ln96_reg_242_reg[3] 
       (.C(ap_clk),
        .CE(j_0_in_reg_920),
        .D(trunc_ln96_fu_119_p1[3]),
        .Q(\trunc_ln96_reg_242_reg_n_1_[3] ),
        .R(1'b0));
  FDRE \trunc_ln96_reg_242_reg[4] 
       (.C(ap_clk),
        .CE(j_0_in_reg_920),
        .D(trunc_ln96_fu_119_p1[4]),
        .Q(\trunc_ln96_reg_242_reg_n_1_[4] ),
        .R(1'b0));
  FDRE \trunc_ln96_reg_242_reg[5] 
       (.C(ap_clk),
        .CE(j_0_in_reg_920),
        .D(trunc_ln96_fu_119_p1[5]),
        .Q(\trunc_ln96_reg_242_reg_n_1_[5] ),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[0] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[0]),
        .Q(trunc_ln99_1_reg_256[0]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[10] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[10]),
        .Q(trunc_ln99_1_reg_256[10]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[11] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[11]),
        .Q(trunc_ln99_1_reg_256[11]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[12] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[12]),
        .Q(trunc_ln99_1_reg_256[12]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[13] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[13]),
        .Q(trunc_ln99_1_reg_256[13]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[14] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[14]),
        .Q(trunc_ln99_1_reg_256[14]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[15] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[15]),
        .Q(trunc_ln99_1_reg_256[15]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[16] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[16]),
        .Q(trunc_ln99_1_reg_256[16]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[17] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[17]),
        .Q(trunc_ln99_1_reg_256[17]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[18] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[18]),
        .Q(trunc_ln99_1_reg_256[18]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[19] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[19]),
        .Q(trunc_ln99_1_reg_256[19]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[1] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[1]),
        .Q(trunc_ln99_1_reg_256[1]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[20] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[20]),
        .Q(trunc_ln99_1_reg_256[20]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[21] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[21]),
        .Q(trunc_ln99_1_reg_256[21]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[22] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[22]),
        .Q(trunc_ln99_1_reg_256[22]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[23] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[23]),
        .Q(trunc_ln99_1_reg_256[23]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[24] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[24]),
        .Q(trunc_ln99_1_reg_256[24]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[25] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[25]),
        .Q(trunc_ln99_1_reg_256[25]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[26] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[26]),
        .Q(trunc_ln99_1_reg_256[26]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[27] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[27]),
        .Q(trunc_ln99_1_reg_256[27]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[28] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[28]),
        .Q(trunc_ln99_1_reg_256[28]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[29] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[29]),
        .Q(trunc_ln99_1_reg_256[29]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[2] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[2]),
        .Q(trunc_ln99_1_reg_256[2]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[3] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[3]),
        .Q(trunc_ln99_1_reg_256[3]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[4] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[4]),
        .Q(trunc_ln99_1_reg_256[4]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[5] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[5]),
        .Q(trunc_ln99_1_reg_256[5]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[6] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[6]),
        .Q(trunc_ln99_1_reg_256[6]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[7] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[7]),
        .Q(trunc_ln99_1_reg_256[7]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[8] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[8]),
        .Q(trunc_ln99_1_reg_256[8]),
        .R(1'b0));
  FDRE \trunc_ln99_1_reg_256_reg[9] 
       (.C(ap_clk),
        .CE(i_0_reg_1020),
        .D(trunc_ln1_fu_123_p4[9]),
        .Q(trunc_ln99_1_reg_256[9]),
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
