// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
// Date        : Mon Jun 19 13:13:23 2023
// Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
// Command     : write_verilog -force -mode funcsim
//               /home/dynamatic/loop_imperfect/proj_loop_imperfect/solution1/impl/verilog/project.srcs/sources_1/bd/bd_0/ip/bd_0_hls_inst_0/bd_0_hls_inst_0_sim_netlist.v
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
    hist_ce0,
    hist_we0,
    C_ce0,
    ap_clk,
    ap_rst,
    ap_start,
    ap_done,
    ap_idle,
    ap_ready,
    A_address0,
    A_q0,
    hist_address0,
    hist_d0,
    hist_q0,
    C_address0,
    C_q0);
  output A_ce0;
  output hist_ce0;
  output hist_we0;
  output C_ce0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 ap_clk CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ap_clk, ASSOCIATED_RESET ap_rst, FREQ_HZ 250000000.0, PHASE 0.000, CLK_DOMAIN bd_0_ap_clk_0, INSERT_VIP 0" *) input ap_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 ap_rst RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ap_rst, POLARITY ACTIVE_HIGH, INSERT_VIP 0" *) input ap_rst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl start" *) input ap_start;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl done" *) output ap_done;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl idle" *) output ap_idle;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl ready" *) output ap_ready;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 A_address0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME A_address0, LAYERED_METADATA undef" *) output [9:0]A_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 A_q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME A_q0, LAYERED_METADATA undef" *) input [31:0]A_q0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 hist_address0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME hist_address0, LAYERED_METADATA undef" *) output [9:0]hist_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 hist_d0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME hist_d0, LAYERED_METADATA undef" *) output [31:0]hist_d0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 hist_q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME hist_q0, LAYERED_METADATA undef" *) input [31:0]hist_q0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 C_address0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME C_address0, LAYERED_METADATA undef" *) output [9:0]C_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 C_q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME C_q0, LAYERED_METADATA undef" *) input [31:0]C_q0;

  wire [9:0]A_address0;
  wire A_ce0;
  wire [31:0]A_q0;
  wire [9:0]C_address0;
  wire C_ce0;
  wire [31:0]C_q0;
  wire ap_clk;
  wire ap_done;
  wire ap_idle;
  wire ap_ready;
  wire ap_rst;
  wire ap_start;
  wire [9:0]hist_address0;
  wire hist_ce0;
  wire [31:0]hist_d0;
  wire [31:0]hist_q0;
  wire hist_we0;

  (* ap_ST_fsm_pp0_stage0 = "4'b0010" *) 
  (* ap_ST_fsm_pp0_stage1 = "4'b0100" *) 
  (* ap_ST_fsm_state1 = "4'b0001" *) 
  (* ap_ST_fsm_state6 = "4'b1000" *) 
  bd_0_hls_inst_0_loop_imperfect inst
       (.A_address0(A_address0),
        .A_ce0(A_ce0),
        .A_q0(A_q0),
        .C_address0(C_address0),
        .C_ce0(C_ce0),
        .C_q0(C_q0),
        .ap_clk(ap_clk),
        .ap_done(ap_done),
        .ap_idle(ap_idle),
        .ap_ready(ap_ready),
        .ap_rst(ap_rst),
        .ap_start(ap_start),
        .hist_address0(hist_address0),
        .hist_ce0(hist_ce0),
        .hist_d0(hist_d0),
        .hist_q0(hist_q0),
        .hist_we0(hist_we0));
endmodule

(* ORIG_REF_NAME = "loop_imperfect" *) (* ap_ST_fsm_pp0_stage0 = "4'b0010" *) (* ap_ST_fsm_pp0_stage1 = "4'b0100" *) 
(* ap_ST_fsm_state1 = "4'b0001" *) (* ap_ST_fsm_state6 = "4'b1000" *) (* hls_module = "yes" *) 
module bd_0_hls_inst_0_loop_imperfect
   (ap_clk,
    ap_rst,
    ap_start,
    ap_done,
    ap_idle,
    ap_ready,
    A_address0,
    A_ce0,
    A_q0,
    hist_address0,
    hist_ce0,
    hist_we0,
    hist_d0,
    hist_q0,
    C_address0,
    C_ce0,
    C_q0);
  input ap_clk;
  input ap_rst;
  input ap_start;
  output ap_done;
  output ap_idle;
  output ap_ready;
  output [9:0]A_address0;
  output A_ce0;
  input [31:0]A_q0;
  output [9:0]hist_address0;
  output hist_ce0;
  output hist_we0;
  output [31:0]hist_d0;
  input [31:0]hist_q0;
  output [9:0]C_address0;
  output C_ce0;
  input [31:0]C_q0;

  wire [31:0]A_q0;
  wire [9:0]C_address0;
  wire C_ce0;
  wire [31:0]C_q0;
  wire \ap_CS_fsm[2]_i_2_n_1 ;
  wire \ap_CS_fsm[2]_i_3_n_1 ;
  wire \ap_CS_fsm[3]_i_2_n_1 ;
  wire \ap_CS_fsm[3]_i_3_n_1 ;
  wire \ap_CS_fsm[3]_i_4_n_1 ;
  wire ap_CS_fsm_pp0_stage0;
  wire ap_CS_fsm_pp0_stage1;
  wire \ap_CS_fsm_reg_n_1_[0] ;
  wire [3:0]ap_NS_fsm;
  wire ap_NS_fsm14_out;
  wire ap_clk;
  wire ap_done;
  wire ap_enable_reg_pp0_iter0;
  wire ap_enable_reg_pp0_iter0_i_1_n_1;
  wire ap_enable_reg_pp0_iter1_i_1_n_1;
  wire ap_enable_reg_pp0_iter1_reg_n_1;
  wire ap_idle;
  wire ap_phi_mux_i_0_phi_fu_84_p41;
  wire ap_rst;
  wire ap_start;
  wire hist_addr_reg_1440;
  wire [9:0]hist_address0;
  wire hist_ce0;
  wire [31:0]hist_d0;
  wire \hist_d0[0]_INST_0_i_1_n_1 ;
  wire \hist_d0[0]_INST_0_i_2_n_1 ;
  wire \hist_d0[0]_INST_0_i_3_n_1 ;
  wire \hist_d0[0]_INST_0_i_4_n_1 ;
  wire \hist_d0[0]_INST_0_n_1 ;
  wire \hist_d0[0]_INST_0_n_2 ;
  wire \hist_d0[0]_INST_0_n_3 ;
  wire \hist_d0[0]_INST_0_n_4 ;
  wire \hist_d0[12]_INST_0_i_1_n_1 ;
  wire \hist_d0[12]_INST_0_i_2_n_1 ;
  wire \hist_d0[12]_INST_0_i_3_n_1 ;
  wire \hist_d0[12]_INST_0_i_4_n_1 ;
  wire \hist_d0[12]_INST_0_n_1 ;
  wire \hist_d0[12]_INST_0_n_2 ;
  wire \hist_d0[12]_INST_0_n_3 ;
  wire \hist_d0[12]_INST_0_n_4 ;
  wire \hist_d0[16]_INST_0_i_1_n_1 ;
  wire \hist_d0[16]_INST_0_i_2_n_1 ;
  wire \hist_d0[16]_INST_0_i_3_n_1 ;
  wire \hist_d0[16]_INST_0_i_4_n_1 ;
  wire \hist_d0[16]_INST_0_n_1 ;
  wire \hist_d0[16]_INST_0_n_2 ;
  wire \hist_d0[16]_INST_0_n_3 ;
  wire \hist_d0[16]_INST_0_n_4 ;
  wire \hist_d0[20]_INST_0_i_1_n_1 ;
  wire \hist_d0[20]_INST_0_i_2_n_1 ;
  wire \hist_d0[20]_INST_0_i_3_n_1 ;
  wire \hist_d0[20]_INST_0_i_4_n_1 ;
  wire \hist_d0[20]_INST_0_n_1 ;
  wire \hist_d0[20]_INST_0_n_2 ;
  wire \hist_d0[20]_INST_0_n_3 ;
  wire \hist_d0[20]_INST_0_n_4 ;
  wire \hist_d0[24]_INST_0_i_1_n_1 ;
  wire \hist_d0[24]_INST_0_i_2_n_1 ;
  wire \hist_d0[24]_INST_0_i_3_n_1 ;
  wire \hist_d0[24]_INST_0_i_4_n_1 ;
  wire \hist_d0[24]_INST_0_n_1 ;
  wire \hist_d0[24]_INST_0_n_2 ;
  wire \hist_d0[24]_INST_0_n_3 ;
  wire \hist_d0[24]_INST_0_n_4 ;
  wire \hist_d0[28]_INST_0_i_1_n_1 ;
  wire \hist_d0[28]_INST_0_i_2_n_1 ;
  wire \hist_d0[28]_INST_0_i_3_n_1 ;
  wire \hist_d0[28]_INST_0_i_4_n_1 ;
  wire \hist_d0[28]_INST_0_n_2 ;
  wire \hist_d0[28]_INST_0_n_3 ;
  wire \hist_d0[28]_INST_0_n_4 ;
  wire \hist_d0[4]_INST_0_i_1_n_1 ;
  wire \hist_d0[4]_INST_0_i_2_n_1 ;
  wire \hist_d0[4]_INST_0_i_3_n_1 ;
  wire \hist_d0[4]_INST_0_i_4_n_1 ;
  wire \hist_d0[4]_INST_0_n_1 ;
  wire \hist_d0[4]_INST_0_n_2 ;
  wire \hist_d0[4]_INST_0_n_3 ;
  wire \hist_d0[4]_INST_0_n_4 ;
  wire \hist_d0[8]_INST_0_i_1_n_1 ;
  wire \hist_d0[8]_INST_0_i_2_n_1 ;
  wire \hist_d0[8]_INST_0_i_3_n_1 ;
  wire \hist_d0[8]_INST_0_i_4_n_1 ;
  wire \hist_d0[8]_INST_0_n_1 ;
  wire \hist_d0[8]_INST_0_n_2 ;
  wire \hist_d0[8]_INST_0_n_3 ;
  wire \hist_d0[8]_INST_0_n_4 ;
  wire [31:0]hist_q0;
  wire hist_we0;
  wire i_0_reg_80;
  wire \i_0_reg_80_reg_n_1_[0] ;
  wire \i_0_reg_80_reg_n_1_[1] ;
  wire \i_0_reg_80_reg_n_1_[2] ;
  wire \i_0_reg_80_reg_n_1_[3] ;
  wire \i_0_reg_80_reg_n_1_[4] ;
  wire \i_0_reg_80_reg_n_1_[5] ;
  wire \i_0_reg_80_reg_n_1_[6] ;
  wire \i_0_reg_80_reg_n_1_[7] ;
  wire \i_0_reg_80_reg_n_1_[8] ;
  wire \i_0_reg_80_reg_n_1_[9] ;
  wire [7:0]i_fu_97_p2;
  wire \i_reg_124[4]_i_2_n_1 ;
  wire \i_reg_124[5]_i_2_n_1 ;
  wire \i_reg_124[6]_i_1_n_1 ;
  wire \i_reg_124[7]_i_2_n_1 ;
  wire \i_reg_124[8]_i_1_n_1 ;
  wire \i_reg_124[8]_i_2_n_1 ;
  wire \i_reg_124[9]_i_1_n_1 ;
  wire \i_reg_124[9]_i_2_n_1 ;
  wire \i_reg_124[9]_i_3_n_1 ;
  wire \i_reg_124[9]_i_4_n_1 ;
  wire \i_reg_124[9]_i_5_n_1 ;
  wire [9:0]i_reg_124_reg;
  wire icmp_ln97_reg_120;
  wire \icmp_ln97_reg_120[0]_i_1_n_1 ;
  wire icmp_ln97_reg_120_pp0_iter1_reg;
  wire \icmp_ln97_reg_120_pp0_iter1_reg[0]_i_1_n_1 ;
  wire [31:0]wt_reg_139;
  wire [3:3]\NLW_hist_d0[28]_INST_0_CO_UNCONNECTED ;

  assign A_address0[9:0] = C_address0;
  assign A_ce0 = C_ce0;
  assign ap_ready = ap_done;
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT5 #(
    .INIT(32'hBAAA8AAA)) 
    \C_address0[0]_INST_0 
       (.I0(\i_0_reg_80_reg_n_1_[0] ),
        .I1(icmp_ln97_reg_120),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(ap_enable_reg_pp0_iter1_reg_n_1),
        .I4(i_reg_124_reg[0]),
        .O(C_address0[0]));
  LUT5 #(
    .INIT(32'hBAAA8AAA)) 
    \C_address0[1]_INST_0 
       (.I0(\i_0_reg_80_reg_n_1_[1] ),
        .I1(icmp_ln97_reg_120),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(ap_enable_reg_pp0_iter1_reg_n_1),
        .I4(i_reg_124_reg[1]),
        .O(C_address0[1]));
  LUT5 #(
    .INIT(32'hBAAA8AAA)) 
    \C_address0[2]_INST_0 
       (.I0(\i_0_reg_80_reg_n_1_[2] ),
        .I1(icmp_ln97_reg_120),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(ap_enable_reg_pp0_iter1_reg_n_1),
        .I4(i_reg_124_reg[2]),
        .O(C_address0[2]));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'hBAAA8AAA)) 
    \C_address0[3]_INST_0 
       (.I0(\i_0_reg_80_reg_n_1_[3] ),
        .I1(icmp_ln97_reg_120),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(ap_enable_reg_pp0_iter1_reg_n_1),
        .I4(i_reg_124_reg[3]),
        .O(C_address0[3]));
  LUT5 #(
    .INIT(32'hBAAA8AAA)) 
    \C_address0[4]_INST_0 
       (.I0(\i_0_reg_80_reg_n_1_[4] ),
        .I1(icmp_ln97_reg_120),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(ap_enable_reg_pp0_iter1_reg_n_1),
        .I4(i_reg_124_reg[4]),
        .O(C_address0[4]));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT5 #(
    .INIT(32'hFFBF0080)) 
    \C_address0[5]_INST_0 
       (.I0(i_reg_124_reg[5]),
        .I1(ap_enable_reg_pp0_iter1_reg_n_1),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(icmp_ln97_reg_120),
        .I4(\i_0_reg_80_reg_n_1_[5] ),
        .O(C_address0[5]));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT5 #(
    .INIT(32'hFFBF0080)) 
    \C_address0[6]_INST_0 
       (.I0(i_reg_124_reg[6]),
        .I1(ap_enable_reg_pp0_iter1_reg_n_1),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(icmp_ln97_reg_120),
        .I4(\i_0_reg_80_reg_n_1_[6] ),
        .O(C_address0[6]));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT5 #(
    .INIT(32'hFFBF0080)) 
    \C_address0[7]_INST_0 
       (.I0(i_reg_124_reg[7]),
        .I1(ap_enable_reg_pp0_iter1_reg_n_1),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(icmp_ln97_reg_120),
        .I4(\i_0_reg_80_reg_n_1_[7] ),
        .O(C_address0[7]));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT5 #(
    .INIT(32'hFFBF0080)) 
    \C_address0[8]_INST_0 
       (.I0(i_reg_124_reg[8]),
        .I1(ap_enable_reg_pp0_iter1_reg_n_1),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(icmp_ln97_reg_120),
        .I4(\i_0_reg_80_reg_n_1_[8] ),
        .O(C_address0[8]));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT5 #(
    .INIT(32'hFFBF0080)) 
    \C_address0[9]_INST_0 
       (.I0(i_reg_124_reg[9]),
        .I1(ap_enable_reg_pp0_iter1_reg_n_1),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(icmp_ln97_reg_120),
        .I4(\i_0_reg_80_reg_n_1_[9] ),
        .O(C_address0[9]));
  LUT2 #(
    .INIT(4'h8)) 
    C_ce0_INST_0
       (.I0(ap_CS_fsm_pp0_stage0),
        .I1(ap_enable_reg_pp0_iter0),
        .O(C_ce0));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT3 #(
    .INIT(8'hBA)) 
    \ap_CS_fsm[0]_i_1 
       (.I0(ap_done),
        .I1(ap_start),
        .I2(\ap_CS_fsm_reg_n_1_[0] ),
        .O(ap_NS_fsm[0]));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT5 #(
    .INIT(32'hF8F888F8)) 
    \ap_CS_fsm[1]_i_1 
       (.I0(ap_start),
        .I1(\ap_CS_fsm_reg_n_1_[0] ),
        .I2(ap_CS_fsm_pp0_stage1),
        .I3(ap_enable_reg_pp0_iter1_reg_n_1),
        .I4(ap_enable_reg_pp0_iter0),
        .O(ap_NS_fsm[1]));
  LUT6 #(
    .INIT(64'hAAAAAAAAAAAA8AAA)) 
    \ap_CS_fsm[2]_i_1 
       (.I0(ap_CS_fsm_pp0_stage0),
        .I1(ap_enable_reg_pp0_iter1_reg_n_1),
        .I2(ap_enable_reg_pp0_iter0),
        .I3(\ap_CS_fsm[3]_i_3_n_1 ),
        .I4(\ap_CS_fsm[2]_i_2_n_1 ),
        .I5(\ap_CS_fsm[2]_i_3_n_1 ),
        .O(ap_NS_fsm[2]));
  LUT6 #(
    .INIT(64'hFFFFFFFFEFEEEFFF)) 
    \ap_CS_fsm[2]_i_2 
       (.I0(C_address0[1]),
        .I1(C_address0[0]),
        .I2(\i_0_reg_80_reg_n_1_[9] ),
        .I3(\i_reg_124[8]_i_2_n_1 ),
        .I4(i_reg_124_reg[9]),
        .I5(\i_reg_124[9]_i_3_n_1 ),
        .O(\ap_CS_fsm[2]_i_2_n_1 ));
  LUT5 #(
    .INIT(32'h335FFF5F)) 
    \ap_CS_fsm[2]_i_3 
       (.I0(i_reg_124_reg[5]),
        .I1(\i_0_reg_80_reg_n_1_[5] ),
        .I2(i_reg_124_reg[6]),
        .I3(\i_reg_124[8]_i_2_n_1 ),
        .I4(\i_0_reg_80_reg_n_1_[6] ),
        .O(\ap_CS_fsm[2]_i_3_n_1 ));
  LUT6 #(
    .INIT(64'h00AA300000AA0000)) 
    \ap_CS_fsm[3]_i_1 
       (.I0(ap_CS_fsm_pp0_stage1),
        .I1(\ap_CS_fsm[3]_i_2_n_1 ),
        .I2(\ap_CS_fsm[3]_i_3_n_1 ),
        .I3(ap_enable_reg_pp0_iter0),
        .I4(ap_enable_reg_pp0_iter1_reg_n_1),
        .I5(ap_CS_fsm_pp0_stage0),
        .O(ap_NS_fsm[3]));
  LUT6 #(
    .INIT(64'hFFFFFFFFFFFFFFFE)) 
    \ap_CS_fsm[3]_i_2 
       (.I0(\ap_CS_fsm[3]_i_4_n_1 ),
        .I1(\i_reg_124[7]_i_2_n_1 ),
        .I2(\i_reg_124[9]_i_3_n_1 ),
        .I3(\i_reg_124[9]_i_2_n_1 ),
        .I4(C_address0[0]),
        .I5(C_address0[1]),
        .O(\ap_CS_fsm[3]_i_2_n_1 ));
  LUT6 #(
    .INIT(64'h000000000000B800)) 
    \ap_CS_fsm[3]_i_3 
       (.I0(\i_0_reg_80_reg_n_1_[7] ),
        .I1(\i_reg_124[8]_i_2_n_1 ),
        .I2(i_reg_124_reg[7]),
        .I3(C_address0[3]),
        .I4(C_address0[2]),
        .I5(C_address0[4]),
        .O(\ap_CS_fsm[3]_i_3_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT5 #(
    .INIT(32'h45557555)) 
    \ap_CS_fsm[3]_i_4 
       (.I0(\i_0_reg_80_reg_n_1_[6] ),
        .I1(icmp_ln97_reg_120),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(ap_enable_reg_pp0_iter1_reg_n_1),
        .I4(i_reg_124_reg[6]),
        .O(\ap_CS_fsm[3]_i_4_n_1 ));
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
        .Q(ap_CS_fsm_pp0_stage0),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_NS_fsm[2]),
        .Q(ap_CS_fsm_pp0_stage1),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[3] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_NS_fsm[3]),
        .Q(ap_done),
        .R(ap_rst));
  LUT6 #(
    .INIT(64'h5454005454545454)) 
    ap_enable_reg_pp0_iter0_i_1
       (.I0(ap_rst),
        .I1(ap_NS_fsm14_out),
        .I2(ap_enable_reg_pp0_iter0),
        .I3(\ap_CS_fsm[3]_i_3_n_1 ),
        .I4(\ap_CS_fsm[3]_i_2_n_1 ),
        .I5(ap_CS_fsm_pp0_stage0),
        .O(ap_enable_reg_pp0_iter0_i_1_n_1));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT2 #(
    .INIT(4'h8)) 
    ap_enable_reg_pp0_iter0_i_2
       (.I0(\ap_CS_fsm_reg_n_1_[0] ),
        .I1(ap_start),
        .O(ap_NS_fsm14_out));
  FDRE #(
    .INIT(1'b0)) 
    ap_enable_reg_pp0_iter0_reg
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_enable_reg_pp0_iter0_i_1_n_1),
        .Q(ap_enable_reg_pp0_iter0),
        .R(1'b0));
  LUT6 #(
    .INIT(64'h00000000F077F000)) 
    ap_enable_reg_pp0_iter1_i_1
       (.I0(\ap_CS_fsm_reg_n_1_[0] ),
        .I1(ap_start),
        .I2(ap_enable_reg_pp0_iter0),
        .I3(ap_CS_fsm_pp0_stage1),
        .I4(ap_enable_reg_pp0_iter1_reg_n_1),
        .I5(ap_rst),
        .O(ap_enable_reg_pp0_iter1_i_1_n_1));
  FDRE #(
    .INIT(1'b0)) 
    ap_enable_reg_pp0_iter1_reg
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_enable_reg_pp0_iter1_i_1_n_1),
        .Q(ap_enable_reg_pp0_iter1_reg_n_1),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT2 #(
    .INIT(4'h2)) 
    ap_idle_INST_0
       (.I0(\ap_CS_fsm_reg_n_1_[0] ),
        .I1(ap_start),
        .O(ap_idle));
  LUT2 #(
    .INIT(4'h2)) 
    \hist_addr_reg_144[9]_i_1 
       (.I0(ap_CS_fsm_pp0_stage1),
        .I1(icmp_ln97_reg_120),
        .O(hist_addr_reg_1440));
  FDRE \hist_addr_reg_144_reg[0] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(A_q0[0]),
        .Q(hist_address0[0]),
        .R(1'b0));
  FDRE \hist_addr_reg_144_reg[1] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(A_q0[1]),
        .Q(hist_address0[1]),
        .R(1'b0));
  FDRE \hist_addr_reg_144_reg[2] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(A_q0[2]),
        .Q(hist_address0[2]),
        .R(1'b0));
  FDRE \hist_addr_reg_144_reg[3] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(A_q0[3]),
        .Q(hist_address0[3]),
        .R(1'b0));
  FDRE \hist_addr_reg_144_reg[4] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(A_q0[4]),
        .Q(hist_address0[4]),
        .R(1'b0));
  FDRE \hist_addr_reg_144_reg[5] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(A_q0[5]),
        .Q(hist_address0[5]),
        .R(1'b0));
  FDRE \hist_addr_reg_144_reg[6] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(A_q0[6]),
        .Q(hist_address0[6]),
        .R(1'b0));
  FDRE \hist_addr_reg_144_reg[7] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(A_q0[7]),
        .Q(hist_address0[7]),
        .R(1'b0));
  FDRE \hist_addr_reg_144_reg[8] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(A_q0[8]),
        .Q(hist_address0[8]),
        .R(1'b0));
  FDRE \hist_addr_reg_144_reg[9] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(A_q0[9]),
        .Q(hist_address0[9]),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT3 #(
    .INIT(8'hE0)) 
    hist_ce0_INST_0
       (.I0(ap_CS_fsm_pp0_stage1),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .O(hist_ce0));
  CARRY4 \hist_d0[0]_INST_0 
       (.CI(1'b0),
        .CO({\hist_d0[0]_INST_0_n_1 ,\hist_d0[0]_INST_0_n_2 ,\hist_d0[0]_INST_0_n_3 ,\hist_d0[0]_INST_0_n_4 }),
        .CYINIT(1'b0),
        .DI(wt_reg_139[3:0]),
        .O(hist_d0[3:0]),
        .S({\hist_d0[0]_INST_0_i_1_n_1 ,\hist_d0[0]_INST_0_i_2_n_1 ,\hist_d0[0]_INST_0_i_3_n_1 ,\hist_d0[0]_INST_0_i_4_n_1 }));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[0]_INST_0_i_1 
       (.I0(wt_reg_139[3]),
        .I1(hist_q0[3]),
        .O(\hist_d0[0]_INST_0_i_1_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[0]_INST_0_i_2 
       (.I0(wt_reg_139[2]),
        .I1(hist_q0[2]),
        .O(\hist_d0[0]_INST_0_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[0]_INST_0_i_3 
       (.I0(wt_reg_139[1]),
        .I1(hist_q0[1]),
        .O(\hist_d0[0]_INST_0_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[0]_INST_0_i_4 
       (.I0(wt_reg_139[0]),
        .I1(hist_q0[0]),
        .O(\hist_d0[0]_INST_0_i_4_n_1 ));
  CARRY4 \hist_d0[12]_INST_0 
       (.CI(\hist_d0[8]_INST_0_n_1 ),
        .CO({\hist_d0[12]_INST_0_n_1 ,\hist_d0[12]_INST_0_n_2 ,\hist_d0[12]_INST_0_n_3 ,\hist_d0[12]_INST_0_n_4 }),
        .CYINIT(1'b0),
        .DI(wt_reg_139[15:12]),
        .O(hist_d0[15:12]),
        .S({\hist_d0[12]_INST_0_i_1_n_1 ,\hist_d0[12]_INST_0_i_2_n_1 ,\hist_d0[12]_INST_0_i_3_n_1 ,\hist_d0[12]_INST_0_i_4_n_1 }));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[12]_INST_0_i_1 
       (.I0(wt_reg_139[15]),
        .I1(hist_q0[15]),
        .O(\hist_d0[12]_INST_0_i_1_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[12]_INST_0_i_2 
       (.I0(wt_reg_139[14]),
        .I1(hist_q0[14]),
        .O(\hist_d0[12]_INST_0_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[12]_INST_0_i_3 
       (.I0(wt_reg_139[13]),
        .I1(hist_q0[13]),
        .O(\hist_d0[12]_INST_0_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[12]_INST_0_i_4 
       (.I0(wt_reg_139[12]),
        .I1(hist_q0[12]),
        .O(\hist_d0[12]_INST_0_i_4_n_1 ));
  CARRY4 \hist_d0[16]_INST_0 
       (.CI(\hist_d0[12]_INST_0_n_1 ),
        .CO({\hist_d0[16]_INST_0_n_1 ,\hist_d0[16]_INST_0_n_2 ,\hist_d0[16]_INST_0_n_3 ,\hist_d0[16]_INST_0_n_4 }),
        .CYINIT(1'b0),
        .DI(wt_reg_139[19:16]),
        .O(hist_d0[19:16]),
        .S({\hist_d0[16]_INST_0_i_1_n_1 ,\hist_d0[16]_INST_0_i_2_n_1 ,\hist_d0[16]_INST_0_i_3_n_1 ,\hist_d0[16]_INST_0_i_4_n_1 }));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[16]_INST_0_i_1 
       (.I0(wt_reg_139[19]),
        .I1(hist_q0[19]),
        .O(\hist_d0[16]_INST_0_i_1_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[16]_INST_0_i_2 
       (.I0(wt_reg_139[18]),
        .I1(hist_q0[18]),
        .O(\hist_d0[16]_INST_0_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[16]_INST_0_i_3 
       (.I0(wt_reg_139[17]),
        .I1(hist_q0[17]),
        .O(\hist_d0[16]_INST_0_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[16]_INST_0_i_4 
       (.I0(wt_reg_139[16]),
        .I1(hist_q0[16]),
        .O(\hist_d0[16]_INST_0_i_4_n_1 ));
  CARRY4 \hist_d0[20]_INST_0 
       (.CI(\hist_d0[16]_INST_0_n_1 ),
        .CO({\hist_d0[20]_INST_0_n_1 ,\hist_d0[20]_INST_0_n_2 ,\hist_d0[20]_INST_0_n_3 ,\hist_d0[20]_INST_0_n_4 }),
        .CYINIT(1'b0),
        .DI(wt_reg_139[23:20]),
        .O(hist_d0[23:20]),
        .S({\hist_d0[20]_INST_0_i_1_n_1 ,\hist_d0[20]_INST_0_i_2_n_1 ,\hist_d0[20]_INST_0_i_3_n_1 ,\hist_d0[20]_INST_0_i_4_n_1 }));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[20]_INST_0_i_1 
       (.I0(wt_reg_139[23]),
        .I1(hist_q0[23]),
        .O(\hist_d0[20]_INST_0_i_1_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[20]_INST_0_i_2 
       (.I0(wt_reg_139[22]),
        .I1(hist_q0[22]),
        .O(\hist_d0[20]_INST_0_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[20]_INST_0_i_3 
       (.I0(wt_reg_139[21]),
        .I1(hist_q0[21]),
        .O(\hist_d0[20]_INST_0_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[20]_INST_0_i_4 
       (.I0(wt_reg_139[20]),
        .I1(hist_q0[20]),
        .O(\hist_d0[20]_INST_0_i_4_n_1 ));
  CARRY4 \hist_d0[24]_INST_0 
       (.CI(\hist_d0[20]_INST_0_n_1 ),
        .CO({\hist_d0[24]_INST_0_n_1 ,\hist_d0[24]_INST_0_n_2 ,\hist_d0[24]_INST_0_n_3 ,\hist_d0[24]_INST_0_n_4 }),
        .CYINIT(1'b0),
        .DI(wt_reg_139[27:24]),
        .O(hist_d0[27:24]),
        .S({\hist_d0[24]_INST_0_i_1_n_1 ,\hist_d0[24]_INST_0_i_2_n_1 ,\hist_d0[24]_INST_0_i_3_n_1 ,\hist_d0[24]_INST_0_i_4_n_1 }));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[24]_INST_0_i_1 
       (.I0(wt_reg_139[27]),
        .I1(hist_q0[27]),
        .O(\hist_d0[24]_INST_0_i_1_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[24]_INST_0_i_2 
       (.I0(wt_reg_139[26]),
        .I1(hist_q0[26]),
        .O(\hist_d0[24]_INST_0_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[24]_INST_0_i_3 
       (.I0(wt_reg_139[25]),
        .I1(hist_q0[25]),
        .O(\hist_d0[24]_INST_0_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[24]_INST_0_i_4 
       (.I0(wt_reg_139[24]),
        .I1(hist_q0[24]),
        .O(\hist_d0[24]_INST_0_i_4_n_1 ));
  CARRY4 \hist_d0[28]_INST_0 
       (.CI(\hist_d0[24]_INST_0_n_1 ),
        .CO({\NLW_hist_d0[28]_INST_0_CO_UNCONNECTED [3],\hist_d0[28]_INST_0_n_2 ,\hist_d0[28]_INST_0_n_3 ,\hist_d0[28]_INST_0_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,wt_reg_139[30:28]}),
        .O(hist_d0[31:28]),
        .S({\hist_d0[28]_INST_0_i_1_n_1 ,\hist_d0[28]_INST_0_i_2_n_1 ,\hist_d0[28]_INST_0_i_3_n_1 ,\hist_d0[28]_INST_0_i_4_n_1 }));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[28]_INST_0_i_1 
       (.I0(wt_reg_139[31]),
        .I1(hist_q0[31]),
        .O(\hist_d0[28]_INST_0_i_1_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[28]_INST_0_i_2 
       (.I0(wt_reg_139[30]),
        .I1(hist_q0[30]),
        .O(\hist_d0[28]_INST_0_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[28]_INST_0_i_3 
       (.I0(wt_reg_139[29]),
        .I1(hist_q0[29]),
        .O(\hist_d0[28]_INST_0_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[28]_INST_0_i_4 
       (.I0(wt_reg_139[28]),
        .I1(hist_q0[28]),
        .O(\hist_d0[28]_INST_0_i_4_n_1 ));
  CARRY4 \hist_d0[4]_INST_0 
       (.CI(\hist_d0[0]_INST_0_n_1 ),
        .CO({\hist_d0[4]_INST_0_n_1 ,\hist_d0[4]_INST_0_n_2 ,\hist_d0[4]_INST_0_n_3 ,\hist_d0[4]_INST_0_n_4 }),
        .CYINIT(1'b0),
        .DI(wt_reg_139[7:4]),
        .O(hist_d0[7:4]),
        .S({\hist_d0[4]_INST_0_i_1_n_1 ,\hist_d0[4]_INST_0_i_2_n_1 ,\hist_d0[4]_INST_0_i_3_n_1 ,\hist_d0[4]_INST_0_i_4_n_1 }));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[4]_INST_0_i_1 
       (.I0(wt_reg_139[7]),
        .I1(hist_q0[7]),
        .O(\hist_d0[4]_INST_0_i_1_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[4]_INST_0_i_2 
       (.I0(wt_reg_139[6]),
        .I1(hist_q0[6]),
        .O(\hist_d0[4]_INST_0_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[4]_INST_0_i_3 
       (.I0(wt_reg_139[5]),
        .I1(hist_q0[5]),
        .O(\hist_d0[4]_INST_0_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[4]_INST_0_i_4 
       (.I0(wt_reg_139[4]),
        .I1(hist_q0[4]),
        .O(\hist_d0[4]_INST_0_i_4_n_1 ));
  CARRY4 \hist_d0[8]_INST_0 
       (.CI(\hist_d0[4]_INST_0_n_1 ),
        .CO({\hist_d0[8]_INST_0_n_1 ,\hist_d0[8]_INST_0_n_2 ,\hist_d0[8]_INST_0_n_3 ,\hist_d0[8]_INST_0_n_4 }),
        .CYINIT(1'b0),
        .DI(wt_reg_139[11:8]),
        .O(hist_d0[11:8]),
        .S({\hist_d0[8]_INST_0_i_1_n_1 ,\hist_d0[8]_INST_0_i_2_n_1 ,\hist_d0[8]_INST_0_i_3_n_1 ,\hist_d0[8]_INST_0_i_4_n_1 }));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[8]_INST_0_i_1 
       (.I0(wt_reg_139[11]),
        .I1(hist_q0[11]),
        .O(\hist_d0[8]_INST_0_i_1_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[8]_INST_0_i_2 
       (.I0(wt_reg_139[10]),
        .I1(hist_q0[10]),
        .O(\hist_d0[8]_INST_0_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[8]_INST_0_i_3 
       (.I0(wt_reg_139[9]),
        .I1(hist_q0[9]),
        .O(\hist_d0[8]_INST_0_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \hist_d0[8]_INST_0_i_4 
       (.I0(wt_reg_139[8]),
        .I1(hist_q0[8]),
        .O(\hist_d0[8]_INST_0_i_4_n_1 ));
  LUT3 #(
    .INIT(8'h08)) 
    hist_we0_INST_0
       (.I0(ap_CS_fsm_pp0_stage1),
        .I1(ap_enable_reg_pp0_iter1_reg_n_1),
        .I2(icmp_ln97_reg_120_pp0_iter1_reg),
        .O(hist_we0));
  LUT5 #(
    .INIT(32'h88880888)) 
    \i_0_reg_80[9]_i_1 
       (.I0(ap_start),
        .I1(\ap_CS_fsm_reg_n_1_[0] ),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(ap_CS_fsm_pp0_stage0),
        .I4(icmp_ln97_reg_120),
        .O(i_0_reg_80));
  LUT3 #(
    .INIT(8'h08)) 
    \i_0_reg_80[9]_i_2 
       (.I0(ap_enable_reg_pp0_iter1_reg_n_1),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(icmp_ln97_reg_120),
        .O(ap_phi_mux_i_0_phi_fu_84_p41));
  FDRE \i_0_reg_80_reg[0] 
       (.C(ap_clk),
        .CE(ap_phi_mux_i_0_phi_fu_84_p41),
        .D(i_reg_124_reg[0]),
        .Q(\i_0_reg_80_reg_n_1_[0] ),
        .R(i_0_reg_80));
  FDRE \i_0_reg_80_reg[1] 
       (.C(ap_clk),
        .CE(ap_phi_mux_i_0_phi_fu_84_p41),
        .D(i_reg_124_reg[1]),
        .Q(\i_0_reg_80_reg_n_1_[1] ),
        .R(i_0_reg_80));
  FDRE \i_0_reg_80_reg[2] 
       (.C(ap_clk),
        .CE(ap_phi_mux_i_0_phi_fu_84_p41),
        .D(i_reg_124_reg[2]),
        .Q(\i_0_reg_80_reg_n_1_[2] ),
        .R(i_0_reg_80));
  FDRE \i_0_reg_80_reg[3] 
       (.C(ap_clk),
        .CE(ap_phi_mux_i_0_phi_fu_84_p41),
        .D(i_reg_124_reg[3]),
        .Q(\i_0_reg_80_reg_n_1_[3] ),
        .R(i_0_reg_80));
  FDRE \i_0_reg_80_reg[4] 
       (.C(ap_clk),
        .CE(ap_phi_mux_i_0_phi_fu_84_p41),
        .D(i_reg_124_reg[4]),
        .Q(\i_0_reg_80_reg_n_1_[4] ),
        .R(i_0_reg_80));
  FDRE \i_0_reg_80_reg[5] 
       (.C(ap_clk),
        .CE(ap_phi_mux_i_0_phi_fu_84_p41),
        .D(i_reg_124_reg[5]),
        .Q(\i_0_reg_80_reg_n_1_[5] ),
        .R(i_0_reg_80));
  FDRE \i_0_reg_80_reg[6] 
       (.C(ap_clk),
        .CE(ap_phi_mux_i_0_phi_fu_84_p41),
        .D(i_reg_124_reg[6]),
        .Q(\i_0_reg_80_reg_n_1_[6] ),
        .R(i_0_reg_80));
  FDRE \i_0_reg_80_reg[7] 
       (.C(ap_clk),
        .CE(ap_phi_mux_i_0_phi_fu_84_p41),
        .D(i_reg_124_reg[7]),
        .Q(\i_0_reg_80_reg_n_1_[7] ),
        .R(i_0_reg_80));
  FDRE \i_0_reg_80_reg[8] 
       (.C(ap_clk),
        .CE(ap_phi_mux_i_0_phi_fu_84_p41),
        .D(i_reg_124_reg[8]),
        .Q(\i_0_reg_80_reg_n_1_[8] ),
        .R(i_0_reg_80));
  FDRE \i_0_reg_80_reg[9] 
       (.C(ap_clk),
        .CE(ap_phi_mux_i_0_phi_fu_84_p41),
        .D(i_reg_124_reg[9]),
        .Q(\i_0_reg_80_reg_n_1_[9] ),
        .R(i_0_reg_80));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT4 #(
    .INIT(16'h4575)) 
    \i_reg_124[0]_i_1 
       (.I0(\i_0_reg_80_reg_n_1_[0] ),
        .I1(icmp_ln97_reg_120),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(i_reg_124_reg[0]),
        .O(i_fu_97_p2[0]));
  LUT5 #(
    .INIT(32'h335ACC5A)) 
    \i_reg_124[1]_i_1 
       (.I0(i_reg_124_reg[0]),
        .I1(\i_0_reg_80_reg_n_1_[0] ),
        .I2(i_reg_124_reg[1]),
        .I3(\i_reg_124[8]_i_2_n_1 ),
        .I4(\i_0_reg_80_reg_n_1_[1] ),
        .O(i_fu_97_p2[1]));
  LUT6 #(
    .INIT(64'h5A5A66AAAAAA66AA)) 
    \i_reg_124[2]_i_1 
       (.I0(C_address0[2]),
        .I1(i_reg_124_reg[0]),
        .I2(\i_0_reg_80_reg_n_1_[0] ),
        .I3(i_reg_124_reg[1]),
        .I4(\i_reg_124[8]_i_2_n_1 ),
        .I5(\i_0_reg_80_reg_n_1_[1] ),
        .O(i_fu_97_p2[2]));
  LUT5 #(
    .INIT(32'h9A999599)) 
    \i_reg_124[3]_i_1 
       (.I0(\i_reg_124[4]_i_2_n_1 ),
        .I1(\i_0_reg_80_reg_n_1_[3] ),
        .I2(icmp_ln97_reg_120),
        .I3(ap_enable_reg_pp0_iter1_reg_n_1),
        .I4(i_reg_124_reg[3]),
        .O(i_fu_97_p2[3]));
  LUT6 #(
    .INIT(64'hC3CCA5A5C3CCAAAA)) 
    \i_reg_124[4]_i_1 
       (.I0(i_reg_124_reg[4]),
        .I1(\i_0_reg_80_reg_n_1_[4] ),
        .I2(\i_reg_124[4]_i_2_n_1 ),
        .I3(\i_0_reg_80_reg_n_1_[3] ),
        .I4(\i_reg_124[8]_i_2_n_1 ),
        .I5(i_reg_124_reg[3]),
        .O(i_fu_97_p2[4]));
  LUT6 #(
    .INIT(64'h47CF77FFFFFFFFFF)) 
    \i_reg_124[4]_i_2 
       (.I0(\i_0_reg_80_reg_n_1_[1] ),
        .I1(\i_reg_124[8]_i_2_n_1 ),
        .I2(i_reg_124_reg[1]),
        .I3(\i_0_reg_80_reg_n_1_[0] ),
        .I4(i_reg_124_reg[0]),
        .I5(C_address0[2]),
        .O(\i_reg_124[4]_i_2_n_1 ));
  LUT6 #(
    .INIT(64'h3CCC5A5A3CCCAAAA)) 
    \i_reg_124[5]_i_1 
       (.I0(i_reg_124_reg[5]),
        .I1(\i_0_reg_80_reg_n_1_[5] ),
        .I2(\i_reg_124[5]_i_2_n_1 ),
        .I3(\i_0_reg_80_reg_n_1_[4] ),
        .I4(\i_reg_124[8]_i_2_n_1 ),
        .I5(i_reg_124_reg[4]),
        .O(i_fu_97_p2[5]));
  LUT6 #(
    .INIT(64'hE200000000000000)) 
    \i_reg_124[5]_i_2 
       (.I0(i_reg_124_reg[3]),
        .I1(\i_reg_124[8]_i_2_n_1 ),
        .I2(\i_0_reg_80_reg_n_1_[3] ),
        .I3(C_address0[2]),
        .I4(C_address0[0]),
        .I5(C_address0[1]),
        .O(\i_reg_124[5]_i_2_n_1 ));
  LUT6 #(
    .INIT(64'hC3CCA5A5C3CCAAAA)) 
    \i_reg_124[6]_i_1 
       (.I0(i_reg_124_reg[6]),
        .I1(\i_0_reg_80_reg_n_1_[6] ),
        .I2(\i_reg_124[9]_i_5_n_1 ),
        .I3(\i_0_reg_80_reg_n_1_[5] ),
        .I4(\i_reg_124[8]_i_2_n_1 ),
        .I5(i_reg_124_reg[5]),
        .O(\i_reg_124[6]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h5656565555555655)) 
    \i_reg_124[7]_i_1 
       (.I0(\i_reg_124[9]_i_4_n_1 ),
        .I1(\i_reg_124[9]_i_5_n_1 ),
        .I2(\i_reg_124[7]_i_2_n_1 ),
        .I3(i_reg_124_reg[6]),
        .I4(\i_reg_124[8]_i_2_n_1 ),
        .I5(\i_0_reg_80_reg_n_1_[6] ),
        .O(i_fu_97_p2[7]));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT5 #(
    .INIT(32'h45557555)) 
    \i_reg_124[7]_i_2 
       (.I0(\i_0_reg_80_reg_n_1_[5] ),
        .I1(icmp_ln97_reg_120),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(ap_enable_reg_pp0_iter1_reg_n_1),
        .I4(i_reg_124_reg[5]),
        .O(\i_reg_124[7]_i_2_n_1 ));
  LUT6 #(
    .INIT(64'hE2E2E2E2E2E2E21D)) 
    \i_reg_124[8]_i_1 
       (.I0(i_reg_124_reg[8]),
        .I1(\i_reg_124[8]_i_2_n_1 ),
        .I2(\i_0_reg_80_reg_n_1_[8] ),
        .I3(\ap_CS_fsm[2]_i_3_n_1 ),
        .I4(\i_reg_124[9]_i_5_n_1 ),
        .I5(\i_reg_124[9]_i_4_n_1 ),
        .O(\i_reg_124[8]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT3 #(
    .INIT(8'hBF)) 
    \i_reg_124[8]_i_2 
       (.I0(icmp_ln97_reg_120),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .O(\i_reg_124[8]_i_2_n_1 ));
  LUT5 #(
    .INIT(32'h55555556)) 
    \i_reg_124[9]_i_1 
       (.I0(\i_reg_124[9]_i_2_n_1 ),
        .I1(\i_reg_124[9]_i_3_n_1 ),
        .I2(\i_reg_124[9]_i_4_n_1 ),
        .I3(\i_reg_124[9]_i_5_n_1 ),
        .I4(\ap_CS_fsm[2]_i_3_n_1 ),
        .O(\i_reg_124[9]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT5 #(
    .INIT(32'h45557555)) 
    \i_reg_124[9]_i_2 
       (.I0(\i_0_reg_80_reg_n_1_[9] ),
        .I1(icmp_ln97_reg_120),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(ap_enable_reg_pp0_iter1_reg_n_1),
        .I4(i_reg_124_reg[9]),
        .O(\i_reg_124[9]_i_2_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT5 #(
    .INIT(32'h45557555)) 
    \i_reg_124[9]_i_3 
       (.I0(\i_0_reg_80_reg_n_1_[8] ),
        .I1(icmp_ln97_reg_120),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(ap_enable_reg_pp0_iter1_reg_n_1),
        .I4(i_reg_124_reg[8]),
        .O(\i_reg_124[9]_i_3_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT5 #(
    .INIT(32'h45557555)) 
    \i_reg_124[9]_i_4 
       (.I0(\i_0_reg_80_reg_n_1_[7] ),
        .I1(icmp_ln97_reg_120),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(ap_enable_reg_pp0_iter1_reg_n_1),
        .I4(i_reg_124_reg[7]),
        .O(\i_reg_124[9]_i_4_n_1 ));
  LUT5 #(
    .INIT(32'h7FFFFFFF)) 
    \i_reg_124[9]_i_5 
       (.I0(C_address0[4]),
        .I1(C_address0[1]),
        .I2(C_address0[0]),
        .I3(C_address0[2]),
        .I4(C_address0[3]),
        .O(\i_reg_124[9]_i_5_n_1 ));
  FDRE \i_reg_124_reg[0] 
       (.C(ap_clk),
        .CE(C_ce0),
        .D(i_fu_97_p2[0]),
        .Q(i_reg_124_reg[0]),
        .R(1'b0));
  FDRE \i_reg_124_reg[1] 
       (.C(ap_clk),
        .CE(C_ce0),
        .D(i_fu_97_p2[1]),
        .Q(i_reg_124_reg[1]),
        .R(1'b0));
  FDRE \i_reg_124_reg[2] 
       (.C(ap_clk),
        .CE(C_ce0),
        .D(i_fu_97_p2[2]),
        .Q(i_reg_124_reg[2]),
        .R(1'b0));
  FDRE \i_reg_124_reg[3] 
       (.C(ap_clk),
        .CE(C_ce0),
        .D(i_fu_97_p2[3]),
        .Q(i_reg_124_reg[3]),
        .R(1'b0));
  FDRE \i_reg_124_reg[4] 
       (.C(ap_clk),
        .CE(C_ce0),
        .D(i_fu_97_p2[4]),
        .Q(i_reg_124_reg[4]),
        .R(1'b0));
  FDRE \i_reg_124_reg[5] 
       (.C(ap_clk),
        .CE(C_ce0),
        .D(i_fu_97_p2[5]),
        .Q(i_reg_124_reg[5]),
        .R(1'b0));
  FDRE \i_reg_124_reg[6] 
       (.C(ap_clk),
        .CE(C_ce0),
        .D(\i_reg_124[6]_i_1_n_1 ),
        .Q(i_reg_124_reg[6]),
        .R(1'b0));
  FDRE \i_reg_124_reg[7] 
       (.C(ap_clk),
        .CE(C_ce0),
        .D(i_fu_97_p2[7]),
        .Q(i_reg_124_reg[7]),
        .R(1'b0));
  FDRE \i_reg_124_reg[8] 
       (.C(ap_clk),
        .CE(C_ce0),
        .D(\i_reg_124[8]_i_1_n_1 ),
        .Q(i_reg_124_reg[8]),
        .R(1'b0));
  FDRE \i_reg_124_reg[9] 
       (.C(ap_clk),
        .CE(C_ce0),
        .D(\i_reg_124[9]_i_1_n_1 ),
        .Q(i_reg_124_reg[9]),
        .R(1'b0));
  LUT5 #(
    .INIT(32'h02FF0200)) 
    \icmp_ln97_reg_120[0]_i_1 
       (.I0(\ap_CS_fsm[3]_i_3_n_1 ),
        .I1(\ap_CS_fsm[2]_i_2_n_1 ),
        .I2(\ap_CS_fsm[2]_i_3_n_1 ),
        .I3(ap_CS_fsm_pp0_stage0),
        .I4(icmp_ln97_reg_120),
        .O(\icmp_ln97_reg_120[0]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \icmp_ln97_reg_120_pp0_iter1_reg[0]_i_1 
       (.I0(icmp_ln97_reg_120),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(icmp_ln97_reg_120_pp0_iter1_reg),
        .O(\icmp_ln97_reg_120_pp0_iter1_reg[0]_i_1_n_1 ));
  FDRE \icmp_ln97_reg_120_pp0_iter1_reg_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\icmp_ln97_reg_120_pp0_iter1_reg[0]_i_1_n_1 ),
        .Q(icmp_ln97_reg_120_pp0_iter1_reg),
        .R(1'b0));
  FDRE \icmp_ln97_reg_120_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\icmp_ln97_reg_120[0]_i_1_n_1 ),
        .Q(icmp_ln97_reg_120),
        .R(1'b0));
  FDRE \wt_reg_139_reg[0] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[0]),
        .Q(wt_reg_139[0]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[10] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[10]),
        .Q(wt_reg_139[10]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[11] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[11]),
        .Q(wt_reg_139[11]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[12] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[12]),
        .Q(wt_reg_139[12]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[13] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[13]),
        .Q(wt_reg_139[13]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[14] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[14]),
        .Q(wt_reg_139[14]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[15] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[15]),
        .Q(wt_reg_139[15]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[16] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[16]),
        .Q(wt_reg_139[16]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[17] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[17]),
        .Q(wt_reg_139[17]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[18] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[18]),
        .Q(wt_reg_139[18]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[19] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[19]),
        .Q(wt_reg_139[19]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[1] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[1]),
        .Q(wt_reg_139[1]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[20] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[20]),
        .Q(wt_reg_139[20]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[21] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[21]),
        .Q(wt_reg_139[21]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[22] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[22]),
        .Q(wt_reg_139[22]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[23] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[23]),
        .Q(wt_reg_139[23]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[24] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[24]),
        .Q(wt_reg_139[24]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[25] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[25]),
        .Q(wt_reg_139[25]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[26] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[26]),
        .Q(wt_reg_139[26]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[27] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[27]),
        .Q(wt_reg_139[27]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[28] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[28]),
        .Q(wt_reg_139[28]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[29] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[29]),
        .Q(wt_reg_139[29]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[2] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[2]),
        .Q(wt_reg_139[2]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[30] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[30]),
        .Q(wt_reg_139[30]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[31] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[31]),
        .Q(wt_reg_139[31]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[3] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[3]),
        .Q(wt_reg_139[3]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[4] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[4]),
        .Q(wt_reg_139[4]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[5] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[5]),
        .Q(wt_reg_139[5]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[6] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[6]),
        .Q(wt_reg_139[6]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[7] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[7]),
        .Q(wt_reg_139[7]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[8] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[8]),
        .Q(wt_reg_139[8]),
        .R(1'b0));
  FDRE \wt_reg_139_reg[9] 
       (.C(ap_clk),
        .CE(hist_addr_reg_1440),
        .D(C_q0[9]),
        .Q(wt_reg_139[9]),
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
