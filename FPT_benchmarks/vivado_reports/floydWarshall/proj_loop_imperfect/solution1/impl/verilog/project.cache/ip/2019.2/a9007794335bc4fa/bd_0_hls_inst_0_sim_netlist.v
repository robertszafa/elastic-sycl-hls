// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
// Date        : Wed Jul  5 16:28:02 2023
// Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
// Command     : write_verilog -force -mode funcsim -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
//               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ bd_0_hls_inst_0_sim_netlist.v
// Design      : bd_0_hls_inst_0
// Purpose     : This verilog netlist is a functional simulation representation of the design and should not be modified
//               or synthesized. This netlist cannot be used for SDF annotated simulation.
// Device      : xc7k160tfbg484-1
// --------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CHECK_LICENSE_TYPE = "bd_0_hls_inst_0,loop_imperfect,{}" *) (* DowngradeIPIdentifiedWarnings = "yes" *) (* IP_DEFINITION_SOURCE = "HLS" *) 
(* X_CORE_INFO = "loop_imperfect,Vivado 2019.2" *) (* hls_module = "yes" *) 
(* NotValidForBitStream *)
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix
   (dist_ce0,
    dist_ce1,
    dist_we1,
    ap_clk,
    ap_rst,
    ap_start,
    ap_done,
    ap_idle,
    ap_ready,
    dist_address0,
    dist_q0,
    dist_address1,
    dist_d1,
    dist_q1);
  output dist_ce0;
  output dist_ce1;
  output dist_we1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 ap_clk CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ap_clk, ASSOCIATED_RESET ap_rst, FREQ_HZ 250000000.0, PHASE 0.000, CLK_DOMAIN bd_0_ap_clk_0, INSERT_VIP 0" *) input ap_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 ap_rst RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ap_rst, POLARITY ACTIVE_HIGH, INSERT_VIP 0" *) input ap_rst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl start" *) input ap_start;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl done" *) output ap_done;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl idle" *) output ap_idle;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl ready" *) output ap_ready;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 dist_address0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME dist_address0, LAYERED_METADATA undef" *) output [6:0]dist_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 dist_q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME dist_q0, LAYERED_METADATA undef" *) input [31:0]dist_q0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 dist_address1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME dist_address1, LAYERED_METADATA undef" *) output [6:0]dist_address1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 dist_d1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME dist_d1, LAYERED_METADATA undef" *) output [31:0]dist_d1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 dist_q1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME dist_q1, LAYERED_METADATA undef" *) input [31:0]dist_q1;

  wire ap_clk;
  wire ap_done;
  wire ap_idle;
  wire ap_ready;
  wire ap_rst;
  wire ap_start;
  wire [6:0]dist_address0;
  wire [6:0]dist_address1;
  wire dist_ce0;
  wire dist_ce1;
  wire [31:0]dist_d1;
  wire [31:0]dist_q0;
  wire [31:0]dist_q1;
  wire dist_we1;

  (* ap_ST_fsm_state1 = "9'b000000001" *) 
  (* ap_ST_fsm_state2 = "9'b000000010" *) 
  (* ap_ST_fsm_state3 = "9'b000000100" *) 
  (* ap_ST_fsm_state4 = "9'b000001000" *) 
  (* ap_ST_fsm_state5 = "9'b000010000" *) 
  (* ap_ST_fsm_state6 = "9'b000100000" *) 
  (* ap_ST_fsm_state7 = "9'b001000000" *) 
  (* ap_ST_fsm_state8 = "9'b010000000" *) 
  (* ap_ST_fsm_state9 = "9'b100000000" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect inst
       (.ap_clk(ap_clk),
        .ap_done(ap_done),
        .ap_idle(ap_idle),
        .ap_ready(ap_ready),
        .ap_rst(ap_rst),
        .ap_start(ap_start),
        .dist_address0(dist_address0),
        .dist_address1(dist_address1),
        .dist_ce0(dist_ce0),
        .dist_ce1(dist_ce1),
        .dist_d1(dist_d1),
        .dist_q0(dist_q0),
        .dist_q1(dist_q1),
        .dist_we1(dist_we1));
endmodule

(* ap_ST_fsm_state1 = "9'b000000001" *) (* ap_ST_fsm_state2 = "9'b000000010" *) (* ap_ST_fsm_state3 = "9'b000000100" *) 
(* ap_ST_fsm_state4 = "9'b000001000" *) (* ap_ST_fsm_state5 = "9'b000010000" *) (* ap_ST_fsm_state6 = "9'b000100000" *) 
(* ap_ST_fsm_state7 = "9'b001000000" *) (* ap_ST_fsm_state8 = "9'b010000000" *) (* ap_ST_fsm_state9 = "9'b100000000" *) 
(* hls_module = "yes" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect
   (ap_clk,
    ap_rst,
    ap_start,
    ap_done,
    ap_idle,
    ap_ready,
    dist_address0,
    dist_ce0,
    dist_q0,
    dist_address1,
    dist_ce1,
    dist_we1,
    dist_d1,
    dist_q1);
  input ap_clk;
  input ap_rst;
  input ap_start;
  output ap_done;
  output ap_idle;
  output ap_ready;
  output [6:0]dist_address0;
  output dist_ce0;
  input [31:0]dist_q0;
  output [6:0]dist_address1;
  output dist_ce1;
  output dist_we1;
  output [31:0]dist_d1;
  input [31:0]dist_q1;

  wire [6:1]add_ln99_1_fu_193_p2;
  wire [6:0]add_ln99_1_reg_278;
  wire add_ln99_1_reg_2780;
  wire \add_ln99_1_reg_278[6]_i_3_n_1 ;
  wire [6:3]add_ln99_2_fu_207_p2;
  wire [6:1]add_ln99_2_reg_283;
  wire \add_ln99_2_reg_283[6]_i_2_n_1 ;
  wire \add_ln99_2_reg_283[6]_i_3_n_1 ;
  wire \add_ln99_2_reg_283[6]_i_4_n_1 ;
  wire \add_ln99_2_reg_283_reg[6]_i_1_n_2 ;
  wire \add_ln99_2_reg_283_reg[6]_i_1_n_3 ;
  wire \add_ln99_2_reg_283_reg[6]_i_1_n_4 ;
  wire [31:0]add_ln99_3_fu_220_p2;
  wire \add_ln99_3_reg_314[11]_i_2_n_1 ;
  wire \add_ln99_3_reg_314[11]_i_3_n_1 ;
  wire \add_ln99_3_reg_314[11]_i_4_n_1 ;
  wire \add_ln99_3_reg_314[11]_i_5_n_1 ;
  wire \add_ln99_3_reg_314[15]_i_2_n_1 ;
  wire \add_ln99_3_reg_314[15]_i_3_n_1 ;
  wire \add_ln99_3_reg_314[15]_i_4_n_1 ;
  wire \add_ln99_3_reg_314[15]_i_5_n_1 ;
  wire \add_ln99_3_reg_314[19]_i_2_n_1 ;
  wire \add_ln99_3_reg_314[19]_i_3_n_1 ;
  wire \add_ln99_3_reg_314[19]_i_4_n_1 ;
  wire \add_ln99_3_reg_314[19]_i_5_n_1 ;
  wire \add_ln99_3_reg_314[23]_i_2_n_1 ;
  wire \add_ln99_3_reg_314[23]_i_3_n_1 ;
  wire \add_ln99_3_reg_314[23]_i_4_n_1 ;
  wire \add_ln99_3_reg_314[23]_i_5_n_1 ;
  wire \add_ln99_3_reg_314[27]_i_2_n_1 ;
  wire \add_ln99_3_reg_314[27]_i_3_n_1 ;
  wire \add_ln99_3_reg_314[27]_i_4_n_1 ;
  wire \add_ln99_3_reg_314[27]_i_5_n_1 ;
  wire \add_ln99_3_reg_314[31]_i_2_n_1 ;
  wire \add_ln99_3_reg_314[31]_i_3_n_1 ;
  wire \add_ln99_3_reg_314[31]_i_4_n_1 ;
  wire \add_ln99_3_reg_314[31]_i_5_n_1 ;
  wire \add_ln99_3_reg_314[3]_i_2_n_1 ;
  wire \add_ln99_3_reg_314[3]_i_3_n_1 ;
  wire \add_ln99_3_reg_314[3]_i_4_n_1 ;
  wire \add_ln99_3_reg_314[3]_i_5_n_1 ;
  wire \add_ln99_3_reg_314[7]_i_2_n_1 ;
  wire \add_ln99_3_reg_314[7]_i_3_n_1 ;
  wire \add_ln99_3_reg_314[7]_i_4_n_1 ;
  wire \add_ln99_3_reg_314[7]_i_5_n_1 ;
  wire \add_ln99_3_reg_314_reg[11]_i_1_n_1 ;
  wire \add_ln99_3_reg_314_reg[11]_i_1_n_2 ;
  wire \add_ln99_3_reg_314_reg[11]_i_1_n_3 ;
  wire \add_ln99_3_reg_314_reg[11]_i_1_n_4 ;
  wire \add_ln99_3_reg_314_reg[15]_i_1_n_1 ;
  wire \add_ln99_3_reg_314_reg[15]_i_1_n_2 ;
  wire \add_ln99_3_reg_314_reg[15]_i_1_n_3 ;
  wire \add_ln99_3_reg_314_reg[15]_i_1_n_4 ;
  wire \add_ln99_3_reg_314_reg[19]_i_1_n_1 ;
  wire \add_ln99_3_reg_314_reg[19]_i_1_n_2 ;
  wire \add_ln99_3_reg_314_reg[19]_i_1_n_3 ;
  wire \add_ln99_3_reg_314_reg[19]_i_1_n_4 ;
  wire \add_ln99_3_reg_314_reg[23]_i_1_n_1 ;
  wire \add_ln99_3_reg_314_reg[23]_i_1_n_2 ;
  wire \add_ln99_3_reg_314_reg[23]_i_1_n_3 ;
  wire \add_ln99_3_reg_314_reg[23]_i_1_n_4 ;
  wire \add_ln99_3_reg_314_reg[27]_i_1_n_1 ;
  wire \add_ln99_3_reg_314_reg[27]_i_1_n_2 ;
  wire \add_ln99_3_reg_314_reg[27]_i_1_n_3 ;
  wire \add_ln99_3_reg_314_reg[27]_i_1_n_4 ;
  wire \add_ln99_3_reg_314_reg[31]_i_1_n_2 ;
  wire \add_ln99_3_reg_314_reg[31]_i_1_n_3 ;
  wire \add_ln99_3_reg_314_reg[31]_i_1_n_4 ;
  wire \add_ln99_3_reg_314_reg[3]_i_1_n_1 ;
  wire \add_ln99_3_reg_314_reg[3]_i_1_n_2 ;
  wire \add_ln99_3_reg_314_reg[3]_i_1_n_3 ;
  wire \add_ln99_3_reg_314_reg[3]_i_1_n_4 ;
  wire \add_ln99_3_reg_314_reg[7]_i_1_n_1 ;
  wire \add_ln99_3_reg_314_reg[7]_i_1_n_2 ;
  wire \add_ln99_3_reg_314_reg[7]_i_1_n_3 ;
  wire \add_ln99_3_reg_314_reg[7]_i_1_n_4 ;
  wire [6:3]add_ln99_4_fu_157_p2;
  wire [6:1]add_ln99_4_reg_260;
  wire [6:0]add_ln99_fu_163_p2;
  wire \ap_CS_fsm[0]_i_2_n_1 ;
  wire \ap_CS_fsm_reg_n_1_[0] ;
  wire ap_CS_fsm_state2;
  wire ap_CS_fsm_state3;
  wire ap_CS_fsm_state4;
  wire ap_CS_fsm_state5;
  wire ap_CS_fsm_state6;
  wire ap_CS_fsm_state7;
  wire ap_CS_fsm_state8;
  wire ap_CS_fsm_state9;
  wire [4:0]ap_NS_fsm;
  wire ap_NS_fsm1;
  wire ap_NS_fsm10_out;
  wire ap_clk;
  wire ap_idle;
  wire ap_ready;
  wire ap_rst;
  wire ap_start;
  wire [6:0]dist_addr_1_reg_288;
  wire [6:0]dist_addr_reg_265;
  wire \dist_addr_reg_265[3]_i_2_n_1 ;
  wire \dist_addr_reg_265[3]_i_3_n_1 ;
  wire \dist_addr_reg_265[3]_i_4_n_1 ;
  wire \dist_addr_reg_265[6]_i_2_n_1 ;
  wire \dist_addr_reg_265[6]_i_3_n_1 ;
  wire \dist_addr_reg_265[6]_i_4_n_1 ;
  wire \dist_addr_reg_265[6]_i_5_n_1 ;
  wire \dist_addr_reg_265[6]_i_6_n_1 ;
  wire \dist_addr_reg_265_reg[3]_i_1_n_1 ;
  wire \dist_addr_reg_265_reg[3]_i_1_n_2 ;
  wire \dist_addr_reg_265_reg[3]_i_1_n_3 ;
  wire \dist_addr_reg_265_reg[3]_i_1_n_4 ;
  wire \dist_addr_reg_265_reg[6]_i_1_n_3 ;
  wire \dist_addr_reg_265_reg[6]_i_1_n_4 ;
  wire [6:0]dist_address0;
  wire [6:0]dist_address1;
  wire dist_ce0;
  wire dist_ce1;
  wire [31:0]dist_d1;
  wire [31:0]dist_load_1_reg_304;
  wire [31:0]dist_load_2_reg_309;
  wire [31:0]dist_load_reg_294;
  wire [31:0]dist_q0;
  wire [31:0]dist_q1;
  wire dist_we1;
  wire i_0_reg_71;
  wire i_0_reg_710;
  wire [3:0]i_fu_131_p2;
  wire [3:0]i_reg_255;
  wire icmp_ln96_fu_97_p2;
  wire icmp_ln99_fu_224_p2;
  wire icmp_ln99_reg_319;
  wire \icmp_ln99_reg_319[0]_i_10_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_12_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_13_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_14_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_15_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_16_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_17_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_18_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_19_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_21_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_22_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_23_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_24_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_25_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_26_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_27_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_28_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_29_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_30_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_31_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_32_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_33_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_34_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_35_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_36_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_3_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_4_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_5_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_6_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_7_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_8_n_1 ;
  wire \icmp_ln99_reg_319[0]_i_9_n_1 ;
  wire \icmp_ln99_reg_319_reg[0]_i_11_n_1 ;
  wire \icmp_ln99_reg_319_reg[0]_i_11_n_2 ;
  wire \icmp_ln99_reg_319_reg[0]_i_11_n_3 ;
  wire \icmp_ln99_reg_319_reg[0]_i_11_n_4 ;
  wire \icmp_ln99_reg_319_reg[0]_i_1_n_2 ;
  wire \icmp_ln99_reg_319_reg[0]_i_1_n_3 ;
  wire \icmp_ln99_reg_319_reg[0]_i_1_n_4 ;
  wire \icmp_ln99_reg_319_reg[0]_i_20_n_1 ;
  wire \icmp_ln99_reg_319_reg[0]_i_20_n_2 ;
  wire \icmp_ln99_reg_319_reg[0]_i_20_n_3 ;
  wire \icmp_ln99_reg_319_reg[0]_i_20_n_4 ;
  wire \icmp_ln99_reg_319_reg[0]_i_2_n_1 ;
  wire \icmp_ln99_reg_319_reg[0]_i_2_n_2 ;
  wire \icmp_ln99_reg_319_reg[0]_i_2_n_3 ;
  wire \icmp_ln99_reg_319_reg[0]_i_2_n_4 ;
  wire [3:0]j_0_reg_82;
  wire j_0_reg_820;
  wire [3:0]j_fu_187_p2;
  wire [3:0]j_reg_273;
  wire k_0_reg_60;
  wire \k_0_reg_60_reg_n_1_[0] ;
  wire \k_0_reg_60_reg_n_1_[1] ;
  wire \k_0_reg_60_reg_n_1_[2] ;
  wire \k_0_reg_60_reg_n_1_[3] ;
  wire [3:0]k_fu_103_p2;
  wire [3:0]k_reg_237;
  wire [3:0]shl_ln_reg_242_reg;
  wire [3:0]zext_ln96_reg_229;
  wire [4:1]zext_ln99_3_fu_153_p1;
  wire [2:1]zext_ln99_4_fu_203_p1;
  wire [3:3]\NLW_add_ln99_2_reg_283_reg[6]_i_1_CO_UNCONNECTED ;
  wire [0:0]\NLW_add_ln99_2_reg_283_reg[6]_i_1_O_UNCONNECTED ;
  wire [3:3]\NLW_add_ln99_3_reg_314_reg[31]_i_1_CO_UNCONNECTED ;
  wire [3:2]\NLW_dist_addr_reg_265_reg[6]_i_1_CO_UNCONNECTED ;
  wire [3:3]\NLW_dist_addr_reg_265_reg[6]_i_1_O_UNCONNECTED ;
  wire [3:0]\NLW_icmp_ln99_reg_319_reg[0]_i_1_O_UNCONNECTED ;
  wire [3:0]\NLW_icmp_ln99_reg_319_reg[0]_i_11_O_UNCONNECTED ;
  wire [3:0]\NLW_icmp_ln99_reg_319_reg[0]_i_2_O_UNCONNECTED ;
  wire [3:0]\NLW_icmp_ln99_reg_319_reg[0]_i_20_O_UNCONNECTED ;

  assign ap_done = ap_ready;
  (* SOFT_HLUTNM = "soft_lutpair18" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_1_reg_278[1]_i_1 
       (.I0(add_ln99_4_reg_260[1]),
        .I1(j_0_reg_82[1]),
        .O(add_ln99_1_fu_193_p2[1]));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT4 #(
    .INIT(16'h9666)) 
    \add_ln99_1_reg_278[2]_i_1 
       (.I0(j_0_reg_82[2]),
        .I1(add_ln99_4_reg_260[2]),
        .I2(j_0_reg_82[1]),
        .I3(add_ln99_4_reg_260[1]),
        .O(add_ln99_1_fu_193_p2[2]));
  LUT6 #(
    .INIT(64'h9999966696666666)) 
    \add_ln99_1_reg_278[3]_i_1 
       (.I0(add_ln99_4_reg_260[3]),
        .I1(j_0_reg_82[3]),
        .I2(add_ln99_4_reg_260[1]),
        .I3(j_0_reg_82[1]),
        .I4(j_0_reg_82[2]),
        .I5(add_ln99_4_reg_260[2]),
        .O(add_ln99_1_fu_193_p2[3]));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT4 #(
    .INIT(16'h17E8)) 
    \add_ln99_1_reg_278[4]_i_1 
       (.I0(\add_ln99_1_reg_278[6]_i_3_n_1 ),
        .I1(j_0_reg_82[3]),
        .I2(add_ln99_4_reg_260[3]),
        .I3(add_ln99_4_reg_260[4]),
        .O(add_ln99_1_fu_193_p2[4]));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT5 #(
    .INIT(32'h17FFE800)) 
    \add_ln99_1_reg_278[5]_i_1 
       (.I0(add_ln99_4_reg_260[3]),
        .I1(j_0_reg_82[3]),
        .I2(\add_ln99_1_reg_278[6]_i_3_n_1 ),
        .I3(add_ln99_4_reg_260[4]),
        .I4(add_ln99_4_reg_260[5]),
        .O(add_ln99_1_fu_193_p2[5]));
  LUT5 #(
    .INIT(32'hFBFF0000)) 
    \add_ln99_1_reg_278[6]_i_1 
       (.I0(j_0_reg_82[0]),
        .I1(j_0_reg_82[1]),
        .I2(j_0_reg_82[2]),
        .I3(j_0_reg_82[3]),
        .I4(ap_CS_fsm_state4),
        .O(add_ln99_1_reg_2780));
  LUT6 #(
    .INIT(64'h777F7FFF88808000)) 
    \add_ln99_1_reg_278[6]_i_2 
       (.I0(add_ln99_4_reg_260[5]),
        .I1(add_ln99_4_reg_260[4]),
        .I2(\add_ln99_1_reg_278[6]_i_3_n_1 ),
        .I3(j_0_reg_82[3]),
        .I4(add_ln99_4_reg_260[3]),
        .I5(add_ln99_4_reg_260[6]),
        .O(add_ln99_1_fu_193_p2[6]));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT4 #(
    .INIT(16'hF880)) 
    \add_ln99_1_reg_278[6]_i_3 
       (.I0(add_ln99_4_reg_260[1]),
        .I1(j_0_reg_82[1]),
        .I2(j_0_reg_82[2]),
        .I3(add_ln99_4_reg_260[2]),
        .O(\add_ln99_1_reg_278[6]_i_3_n_1 ));
  FDRE \add_ln99_1_reg_278_reg[0] 
       (.C(ap_clk),
        .CE(add_ln99_1_reg_2780),
        .D(j_0_reg_82[0]),
        .Q(add_ln99_1_reg_278[0]),
        .R(1'b0));
  FDRE \add_ln99_1_reg_278_reg[1] 
       (.C(ap_clk),
        .CE(add_ln99_1_reg_2780),
        .D(add_ln99_1_fu_193_p2[1]),
        .Q(add_ln99_1_reg_278[1]),
        .R(1'b0));
  FDRE \add_ln99_1_reg_278_reg[2] 
       (.C(ap_clk),
        .CE(add_ln99_1_reg_2780),
        .D(add_ln99_1_fu_193_p2[2]),
        .Q(add_ln99_1_reg_278[2]),
        .R(1'b0));
  FDRE \add_ln99_1_reg_278_reg[3] 
       (.C(ap_clk),
        .CE(add_ln99_1_reg_2780),
        .D(add_ln99_1_fu_193_p2[3]),
        .Q(add_ln99_1_reg_278[3]),
        .R(1'b0));
  FDRE \add_ln99_1_reg_278_reg[4] 
       (.C(ap_clk),
        .CE(add_ln99_1_reg_2780),
        .D(add_ln99_1_fu_193_p2[4]),
        .Q(add_ln99_1_reg_278[4]),
        .R(1'b0));
  FDRE \add_ln99_1_reg_278_reg[5] 
       (.C(ap_clk),
        .CE(add_ln99_1_reg_2780),
        .D(add_ln99_1_fu_193_p2[5]),
        .Q(add_ln99_1_reg_278[5]),
        .R(1'b0));
  FDRE \add_ln99_1_reg_278_reg[6] 
       (.C(ap_clk),
        .CE(add_ln99_1_reg_2780),
        .D(add_ln99_1_fu_193_p2[6]),
        .Q(add_ln99_1_reg_278[6]),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair18" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_2_reg_283[1]_i_1 
       (.I0(shl_ln_reg_242_reg[0]),
        .I1(j_0_reg_82[1]),
        .O(zext_ln99_4_fu_203_p1[1]));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT4 #(
    .INIT(16'h9666)) 
    \add_ln99_2_reg_283[2]_i_1 
       (.I0(j_0_reg_82[2]),
        .I1(shl_ln_reg_242_reg[1]),
        .I2(j_0_reg_82[1]),
        .I3(shl_ln_reg_242_reg[0]),
        .O(zext_ln99_4_fu_203_p1[2]));
  LUT6 #(
    .INIT(64'h17E8E817C03F3FC0)) 
    \add_ln99_2_reg_283[3]_i_1 
       (.I0(j_0_reg_82[1]),
        .I1(j_0_reg_82[2]),
        .I2(shl_ln_reg_242_reg[1]),
        .I3(j_0_reg_82[3]),
        .I4(shl_ln_reg_242_reg[2]),
        .I5(shl_ln_reg_242_reg[0]),
        .O(add_ln99_2_fu_207_p2[3]));
  LUT6 #(
    .INIT(64'hC993C9C96C6C366C)) 
    \add_ln99_2_reg_283[6]_i_2 
       (.I0(j_0_reg_82[3]),
        .I1(shl_ln_reg_242_reg[3]),
        .I2(shl_ln_reg_242_reg[2]),
        .I3(j_0_reg_82[2]),
        .I4(\add_ln99_2_reg_283[6]_i_4_n_1 ),
        .I5(shl_ln_reg_242_reg[1]),
        .O(\add_ln99_2_reg_283[6]_i_2_n_1 ));
  LUT6 #(
    .INIT(64'h17E8E817C03F3FC0)) 
    \add_ln99_2_reg_283[6]_i_3 
       (.I0(j_0_reg_82[1]),
        .I1(j_0_reg_82[2]),
        .I2(shl_ln_reg_242_reg[1]),
        .I3(j_0_reg_82[3]),
        .I4(shl_ln_reg_242_reg[2]),
        .I5(shl_ln_reg_242_reg[0]),
        .O(\add_ln99_2_reg_283[6]_i_3_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT2 #(
    .INIT(4'h7)) 
    \add_ln99_2_reg_283[6]_i_4 
       (.I0(shl_ln_reg_242_reg[0]),
        .I1(j_0_reg_82[1]),
        .O(\add_ln99_2_reg_283[6]_i_4_n_1 ));
  FDRE \add_ln99_2_reg_283_reg[1] 
       (.C(ap_clk),
        .CE(add_ln99_1_reg_2780),
        .D(zext_ln99_4_fu_203_p1[1]),
        .Q(add_ln99_2_reg_283[1]),
        .R(1'b0));
  FDRE \add_ln99_2_reg_283_reg[2] 
       (.C(ap_clk),
        .CE(add_ln99_1_reg_2780),
        .D(zext_ln99_4_fu_203_p1[2]),
        .Q(add_ln99_2_reg_283[2]),
        .R(1'b0));
  FDRE \add_ln99_2_reg_283_reg[3] 
       (.C(ap_clk),
        .CE(add_ln99_1_reg_2780),
        .D(add_ln99_2_fu_207_p2[3]),
        .Q(add_ln99_2_reg_283[3]),
        .R(1'b0));
  FDRE \add_ln99_2_reg_283_reg[4] 
       (.C(ap_clk),
        .CE(add_ln99_1_reg_2780),
        .D(add_ln99_2_fu_207_p2[4]),
        .Q(add_ln99_2_reg_283[4]),
        .R(1'b0));
  FDRE \add_ln99_2_reg_283_reg[5] 
       (.C(ap_clk),
        .CE(add_ln99_1_reg_2780),
        .D(add_ln99_2_fu_207_p2[5]),
        .Q(add_ln99_2_reg_283[5]),
        .R(1'b0));
  FDRE \add_ln99_2_reg_283_reg[6] 
       (.C(ap_clk),
        .CE(add_ln99_1_reg_2780),
        .D(add_ln99_2_fu_207_p2[6]),
        .Q(add_ln99_2_reg_283[6]),
        .R(1'b0));
  CARRY4 \add_ln99_2_reg_283_reg[6]_i_1 
       (.CI(1'b0),
        .CO({\NLW_add_ln99_2_reg_283_reg[6]_i_1_CO_UNCONNECTED [3],\add_ln99_2_reg_283_reg[6]_i_1_n_2 ,\add_ln99_2_reg_283_reg[6]_i_1_n_3 ,\add_ln99_2_reg_283_reg[6]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,shl_ln_reg_242_reg[1:0]}),
        .O({add_ln99_2_fu_207_p2[6:4],\NLW_add_ln99_2_reg_283_reg[6]_i_1_O_UNCONNECTED [0]}),
        .S({shl_ln_reg_242_reg[3:2],\add_ln99_2_reg_283[6]_i_2_n_1 ,\add_ln99_2_reg_283[6]_i_3_n_1 }));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[11]_i_2 
       (.I0(dist_load_1_reg_304[11]),
        .I1(dist_load_2_reg_309[11]),
        .O(\add_ln99_3_reg_314[11]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[11]_i_3 
       (.I0(dist_load_1_reg_304[10]),
        .I1(dist_load_2_reg_309[10]),
        .O(\add_ln99_3_reg_314[11]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[11]_i_4 
       (.I0(dist_load_1_reg_304[9]),
        .I1(dist_load_2_reg_309[9]),
        .O(\add_ln99_3_reg_314[11]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[11]_i_5 
       (.I0(dist_load_1_reg_304[8]),
        .I1(dist_load_2_reg_309[8]),
        .O(\add_ln99_3_reg_314[11]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[15]_i_2 
       (.I0(dist_load_1_reg_304[15]),
        .I1(dist_load_2_reg_309[15]),
        .O(\add_ln99_3_reg_314[15]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[15]_i_3 
       (.I0(dist_load_1_reg_304[14]),
        .I1(dist_load_2_reg_309[14]),
        .O(\add_ln99_3_reg_314[15]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[15]_i_4 
       (.I0(dist_load_1_reg_304[13]),
        .I1(dist_load_2_reg_309[13]),
        .O(\add_ln99_3_reg_314[15]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[15]_i_5 
       (.I0(dist_load_1_reg_304[12]),
        .I1(dist_load_2_reg_309[12]),
        .O(\add_ln99_3_reg_314[15]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[19]_i_2 
       (.I0(dist_load_1_reg_304[19]),
        .I1(dist_load_2_reg_309[19]),
        .O(\add_ln99_3_reg_314[19]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[19]_i_3 
       (.I0(dist_load_1_reg_304[18]),
        .I1(dist_load_2_reg_309[18]),
        .O(\add_ln99_3_reg_314[19]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[19]_i_4 
       (.I0(dist_load_1_reg_304[17]),
        .I1(dist_load_2_reg_309[17]),
        .O(\add_ln99_3_reg_314[19]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[19]_i_5 
       (.I0(dist_load_1_reg_304[16]),
        .I1(dist_load_2_reg_309[16]),
        .O(\add_ln99_3_reg_314[19]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[23]_i_2 
       (.I0(dist_load_1_reg_304[23]),
        .I1(dist_load_2_reg_309[23]),
        .O(\add_ln99_3_reg_314[23]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[23]_i_3 
       (.I0(dist_load_1_reg_304[22]),
        .I1(dist_load_2_reg_309[22]),
        .O(\add_ln99_3_reg_314[23]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[23]_i_4 
       (.I0(dist_load_1_reg_304[21]),
        .I1(dist_load_2_reg_309[21]),
        .O(\add_ln99_3_reg_314[23]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[23]_i_5 
       (.I0(dist_load_1_reg_304[20]),
        .I1(dist_load_2_reg_309[20]),
        .O(\add_ln99_3_reg_314[23]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[27]_i_2 
       (.I0(dist_load_1_reg_304[27]),
        .I1(dist_load_2_reg_309[27]),
        .O(\add_ln99_3_reg_314[27]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[27]_i_3 
       (.I0(dist_load_1_reg_304[26]),
        .I1(dist_load_2_reg_309[26]),
        .O(\add_ln99_3_reg_314[27]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[27]_i_4 
       (.I0(dist_load_1_reg_304[25]),
        .I1(dist_load_2_reg_309[25]),
        .O(\add_ln99_3_reg_314[27]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[27]_i_5 
       (.I0(dist_load_1_reg_304[24]),
        .I1(dist_load_2_reg_309[24]),
        .O(\add_ln99_3_reg_314[27]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[31]_i_2 
       (.I0(dist_load_1_reg_304[31]),
        .I1(dist_load_2_reg_309[31]),
        .O(\add_ln99_3_reg_314[31]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[31]_i_3 
       (.I0(dist_load_1_reg_304[30]),
        .I1(dist_load_2_reg_309[30]),
        .O(\add_ln99_3_reg_314[31]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[31]_i_4 
       (.I0(dist_load_1_reg_304[29]),
        .I1(dist_load_2_reg_309[29]),
        .O(\add_ln99_3_reg_314[31]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[31]_i_5 
       (.I0(dist_load_1_reg_304[28]),
        .I1(dist_load_2_reg_309[28]),
        .O(\add_ln99_3_reg_314[31]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[3]_i_2 
       (.I0(dist_load_1_reg_304[3]),
        .I1(dist_load_2_reg_309[3]),
        .O(\add_ln99_3_reg_314[3]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[3]_i_3 
       (.I0(dist_load_1_reg_304[2]),
        .I1(dist_load_2_reg_309[2]),
        .O(\add_ln99_3_reg_314[3]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[3]_i_4 
       (.I0(dist_load_1_reg_304[1]),
        .I1(dist_load_2_reg_309[1]),
        .O(\add_ln99_3_reg_314[3]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[3]_i_5 
       (.I0(dist_load_1_reg_304[0]),
        .I1(dist_load_2_reg_309[0]),
        .O(\add_ln99_3_reg_314[3]_i_5_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[7]_i_2 
       (.I0(dist_load_1_reg_304[7]),
        .I1(dist_load_2_reg_309[7]),
        .O(\add_ln99_3_reg_314[7]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[7]_i_3 
       (.I0(dist_load_1_reg_304[6]),
        .I1(dist_load_2_reg_309[6]),
        .O(\add_ln99_3_reg_314[7]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[7]_i_4 
       (.I0(dist_load_1_reg_304[5]),
        .I1(dist_load_2_reg_309[5]),
        .O(\add_ln99_3_reg_314[7]_i_4_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_3_reg_314[7]_i_5 
       (.I0(dist_load_1_reg_304[4]),
        .I1(dist_load_2_reg_309[4]),
        .O(\add_ln99_3_reg_314[7]_i_5_n_1 ));
  FDRE \add_ln99_3_reg_314_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[0]),
        .Q(dist_d1[0]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[10]),
        .Q(dist_d1[10]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[11] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[11]),
        .Q(dist_d1[11]),
        .R(1'b0));
  CARRY4 \add_ln99_3_reg_314_reg[11]_i_1 
       (.CI(\add_ln99_3_reg_314_reg[7]_i_1_n_1 ),
        .CO({\add_ln99_3_reg_314_reg[11]_i_1_n_1 ,\add_ln99_3_reg_314_reg[11]_i_1_n_2 ,\add_ln99_3_reg_314_reg[11]_i_1_n_3 ,\add_ln99_3_reg_314_reg[11]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI(dist_load_1_reg_304[11:8]),
        .O(add_ln99_3_fu_220_p2[11:8]),
        .S({\add_ln99_3_reg_314[11]_i_2_n_1 ,\add_ln99_3_reg_314[11]_i_3_n_1 ,\add_ln99_3_reg_314[11]_i_4_n_1 ,\add_ln99_3_reg_314[11]_i_5_n_1 }));
  FDRE \add_ln99_3_reg_314_reg[12] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[12]),
        .Q(dist_d1[12]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[13] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[13]),
        .Q(dist_d1[13]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[14] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[14]),
        .Q(dist_d1[14]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[15] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[15]),
        .Q(dist_d1[15]),
        .R(1'b0));
  CARRY4 \add_ln99_3_reg_314_reg[15]_i_1 
       (.CI(\add_ln99_3_reg_314_reg[11]_i_1_n_1 ),
        .CO({\add_ln99_3_reg_314_reg[15]_i_1_n_1 ,\add_ln99_3_reg_314_reg[15]_i_1_n_2 ,\add_ln99_3_reg_314_reg[15]_i_1_n_3 ,\add_ln99_3_reg_314_reg[15]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI(dist_load_1_reg_304[15:12]),
        .O(add_ln99_3_fu_220_p2[15:12]),
        .S({\add_ln99_3_reg_314[15]_i_2_n_1 ,\add_ln99_3_reg_314[15]_i_3_n_1 ,\add_ln99_3_reg_314[15]_i_4_n_1 ,\add_ln99_3_reg_314[15]_i_5_n_1 }));
  FDRE \add_ln99_3_reg_314_reg[16] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[16]),
        .Q(dist_d1[16]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[17] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[17]),
        .Q(dist_d1[17]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[18] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[18]),
        .Q(dist_d1[18]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[19] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[19]),
        .Q(dist_d1[19]),
        .R(1'b0));
  CARRY4 \add_ln99_3_reg_314_reg[19]_i_1 
       (.CI(\add_ln99_3_reg_314_reg[15]_i_1_n_1 ),
        .CO({\add_ln99_3_reg_314_reg[19]_i_1_n_1 ,\add_ln99_3_reg_314_reg[19]_i_1_n_2 ,\add_ln99_3_reg_314_reg[19]_i_1_n_3 ,\add_ln99_3_reg_314_reg[19]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI(dist_load_1_reg_304[19:16]),
        .O(add_ln99_3_fu_220_p2[19:16]),
        .S({\add_ln99_3_reg_314[19]_i_2_n_1 ,\add_ln99_3_reg_314[19]_i_3_n_1 ,\add_ln99_3_reg_314[19]_i_4_n_1 ,\add_ln99_3_reg_314[19]_i_5_n_1 }));
  FDRE \add_ln99_3_reg_314_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[1]),
        .Q(dist_d1[1]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[20] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[20]),
        .Q(dist_d1[20]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[21] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[21]),
        .Q(dist_d1[21]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[22] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[22]),
        .Q(dist_d1[22]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[23] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[23]),
        .Q(dist_d1[23]),
        .R(1'b0));
  CARRY4 \add_ln99_3_reg_314_reg[23]_i_1 
       (.CI(\add_ln99_3_reg_314_reg[19]_i_1_n_1 ),
        .CO({\add_ln99_3_reg_314_reg[23]_i_1_n_1 ,\add_ln99_3_reg_314_reg[23]_i_1_n_2 ,\add_ln99_3_reg_314_reg[23]_i_1_n_3 ,\add_ln99_3_reg_314_reg[23]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI(dist_load_1_reg_304[23:20]),
        .O(add_ln99_3_fu_220_p2[23:20]),
        .S({\add_ln99_3_reg_314[23]_i_2_n_1 ,\add_ln99_3_reg_314[23]_i_3_n_1 ,\add_ln99_3_reg_314[23]_i_4_n_1 ,\add_ln99_3_reg_314[23]_i_5_n_1 }));
  FDRE \add_ln99_3_reg_314_reg[24] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[24]),
        .Q(dist_d1[24]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[25] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[25]),
        .Q(dist_d1[25]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[26] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[26]),
        .Q(dist_d1[26]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[27] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[27]),
        .Q(dist_d1[27]),
        .R(1'b0));
  CARRY4 \add_ln99_3_reg_314_reg[27]_i_1 
       (.CI(\add_ln99_3_reg_314_reg[23]_i_1_n_1 ),
        .CO({\add_ln99_3_reg_314_reg[27]_i_1_n_1 ,\add_ln99_3_reg_314_reg[27]_i_1_n_2 ,\add_ln99_3_reg_314_reg[27]_i_1_n_3 ,\add_ln99_3_reg_314_reg[27]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI(dist_load_1_reg_304[27:24]),
        .O(add_ln99_3_fu_220_p2[27:24]),
        .S({\add_ln99_3_reg_314[27]_i_2_n_1 ,\add_ln99_3_reg_314[27]_i_3_n_1 ,\add_ln99_3_reg_314[27]_i_4_n_1 ,\add_ln99_3_reg_314[27]_i_5_n_1 }));
  FDRE \add_ln99_3_reg_314_reg[28] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[28]),
        .Q(dist_d1[28]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[29] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[29]),
        .Q(dist_d1[29]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[2]),
        .Q(dist_d1[2]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[30] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[30]),
        .Q(dist_d1[30]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[31] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[31]),
        .Q(dist_d1[31]),
        .R(1'b0));
  CARRY4 \add_ln99_3_reg_314_reg[31]_i_1 
       (.CI(\add_ln99_3_reg_314_reg[27]_i_1_n_1 ),
        .CO({\NLW_add_ln99_3_reg_314_reg[31]_i_1_CO_UNCONNECTED [3],\add_ln99_3_reg_314_reg[31]_i_1_n_2 ,\add_ln99_3_reg_314_reg[31]_i_1_n_3 ,\add_ln99_3_reg_314_reg[31]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,dist_load_1_reg_304[30:28]}),
        .O(add_ln99_3_fu_220_p2[31:28]),
        .S({\add_ln99_3_reg_314[31]_i_2_n_1 ,\add_ln99_3_reg_314[31]_i_3_n_1 ,\add_ln99_3_reg_314[31]_i_4_n_1 ,\add_ln99_3_reg_314[31]_i_5_n_1 }));
  FDRE \add_ln99_3_reg_314_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[3]),
        .Q(dist_d1[3]),
        .R(1'b0));
  CARRY4 \add_ln99_3_reg_314_reg[3]_i_1 
       (.CI(1'b0),
        .CO({\add_ln99_3_reg_314_reg[3]_i_1_n_1 ,\add_ln99_3_reg_314_reg[3]_i_1_n_2 ,\add_ln99_3_reg_314_reg[3]_i_1_n_3 ,\add_ln99_3_reg_314_reg[3]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI(dist_load_1_reg_304[3:0]),
        .O(add_ln99_3_fu_220_p2[3:0]),
        .S({\add_ln99_3_reg_314[3]_i_2_n_1 ,\add_ln99_3_reg_314[3]_i_3_n_1 ,\add_ln99_3_reg_314[3]_i_4_n_1 ,\add_ln99_3_reg_314[3]_i_5_n_1 }));
  FDRE \add_ln99_3_reg_314_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[4]),
        .Q(dist_d1[4]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[5]),
        .Q(dist_d1[5]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[6]),
        .Q(dist_d1[6]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[7]),
        .Q(dist_d1[7]),
        .R(1'b0));
  CARRY4 \add_ln99_3_reg_314_reg[7]_i_1 
       (.CI(\add_ln99_3_reg_314_reg[3]_i_1_n_1 ),
        .CO({\add_ln99_3_reg_314_reg[7]_i_1_n_1 ,\add_ln99_3_reg_314_reg[7]_i_1_n_2 ,\add_ln99_3_reg_314_reg[7]_i_1_n_3 ,\add_ln99_3_reg_314_reg[7]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI(dist_load_1_reg_304[7:4]),
        .O(add_ln99_3_fu_220_p2[7:4]),
        .S({\add_ln99_3_reg_314[7]_i_2_n_1 ,\add_ln99_3_reg_314[7]_i_3_n_1 ,\add_ln99_3_reg_314[7]_i_4_n_1 ,\add_ln99_3_reg_314[7]_i_5_n_1 }));
  FDRE \add_ln99_3_reg_314_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[8]),
        .Q(dist_d1[8]),
        .R(1'b0));
  FDRE \add_ln99_3_reg_314_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(add_ln99_3_fu_220_p2[9]),
        .Q(dist_d1[9]),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \add_ln99_4_reg_260[3]_i_1 
       (.I0(zext_ln99_3_fu_153_p1[3]),
        .I1(zext_ln99_3_fu_153_p1[1]),
        .O(add_ln99_4_fu_157_p2[3]));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT4 #(
    .INIT(16'h8778)) 
    \add_ln99_4_reg_260[4]_i_1 
       (.I0(zext_ln99_3_fu_153_p1[3]),
        .I1(zext_ln99_3_fu_153_p1[1]),
        .I2(zext_ln99_3_fu_153_p1[2]),
        .I3(zext_ln99_3_fu_153_p1[4]),
        .O(add_ln99_4_fu_157_p2[4]));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT4 #(
    .INIT(16'h17C0)) 
    \add_ln99_4_reg_260[5]_i_1 
       (.I0(zext_ln99_3_fu_153_p1[1]),
        .I1(zext_ln99_3_fu_153_p1[4]),
        .I2(zext_ln99_3_fu_153_p1[2]),
        .I3(zext_ln99_3_fu_153_p1[3]),
        .O(add_ln99_4_fu_157_p2[5]));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT4 #(
    .INIT(16'h3870)) 
    \add_ln99_4_reg_260[6]_i_1 
       (.I0(zext_ln99_3_fu_153_p1[2]),
        .I1(zext_ln99_3_fu_153_p1[3]),
        .I2(zext_ln99_3_fu_153_p1[4]),
        .I3(zext_ln99_3_fu_153_p1[1]),
        .O(add_ln99_4_fu_157_p2[6]));
  FDRE \add_ln99_4_reg_260_reg[1] 
       (.C(ap_clk),
        .CE(j_0_reg_820),
        .D(zext_ln99_3_fu_153_p1[1]),
        .Q(add_ln99_4_reg_260[1]),
        .R(1'b0));
  FDRE \add_ln99_4_reg_260_reg[2] 
       (.C(ap_clk),
        .CE(j_0_reg_820),
        .D(zext_ln99_3_fu_153_p1[2]),
        .Q(add_ln99_4_reg_260[2]),
        .R(1'b0));
  FDRE \add_ln99_4_reg_260_reg[3] 
       (.C(ap_clk),
        .CE(j_0_reg_820),
        .D(add_ln99_4_fu_157_p2[3]),
        .Q(add_ln99_4_reg_260[3]),
        .R(1'b0));
  FDRE \add_ln99_4_reg_260_reg[4] 
       (.C(ap_clk),
        .CE(j_0_reg_820),
        .D(add_ln99_4_fu_157_p2[4]),
        .Q(add_ln99_4_reg_260[4]),
        .R(1'b0));
  FDRE \add_ln99_4_reg_260_reg[5] 
       (.C(ap_clk),
        .CE(j_0_reg_820),
        .D(add_ln99_4_fu_157_p2[5]),
        .Q(add_ln99_4_reg_260[5]),
        .R(1'b0));
  FDRE \add_ln99_4_reg_260_reg[6] 
       (.C(ap_clk),
        .CE(j_0_reg_820),
        .D(add_ln99_4_fu_157_p2[6]),
        .Q(add_ln99_4_reg_260[6]),
        .R(1'b0));
  LUT6 #(
    .INIT(64'h5500550055C05500)) 
    \ap_CS_fsm[0]_i_1 
       (.I0(ap_start),
        .I1(\ap_CS_fsm[0]_i_2_n_1 ),
        .I2(icmp_ln96_fu_97_p2),
        .I3(\ap_CS_fsm_reg_n_1_[0] ),
        .I4(ap_CS_fsm_state2),
        .I5(ap_CS_fsm_state3),
        .O(ap_NS_fsm[0]));
  LUT6 #(
    .INIT(64'h0000000000000001)) 
    \ap_CS_fsm[0]_i_2 
       (.I0(ap_CS_fsm_state4),
        .I1(ap_CS_fsm_state5),
        .I2(ap_CS_fsm_state6),
        .I3(ap_CS_fsm_state7),
        .I4(ap_CS_fsm_state9),
        .I5(ap_CS_fsm_state8),
        .O(\ap_CS_fsm[0]_i_2_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT4 #(
    .INIT(16'h0400)) 
    \ap_CS_fsm[0]_i_3 
       (.I0(\k_0_reg_60_reg_n_1_[0] ),
        .I1(\k_0_reg_60_reg_n_1_[1] ),
        .I2(\k_0_reg_60_reg_n_1_[2] ),
        .I3(\k_0_reg_60_reg_n_1_[3] ),
        .O(icmp_ln96_fu_97_p2));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT4 #(
    .INIT(16'hAAC0)) 
    \ap_CS_fsm[1]_i_1 
       (.I0(ap_start),
        .I1(ap_NS_fsm10_out),
        .I2(ap_CS_fsm_state3),
        .I3(\ap_CS_fsm_reg_n_1_[0] ),
        .O(ap_NS_fsm[1]));
  LUT4 #(
    .INIT(16'hAAEA)) 
    \ap_CS_fsm[2]_i_1 
       (.I0(i_0_reg_710),
        .I1(ap_NS_fsm1),
        .I2(ap_CS_fsm_state4),
        .I3(ap_CS_fsm_state2),
        .O(ap_NS_fsm[2]));
  LUT6 #(
    .INIT(64'hFFFFFFFFAA8AAAAA)) 
    \ap_CS_fsm[3]_i_1 
       (.I0(ap_CS_fsm_state3),
        .I1(zext_ln99_3_fu_153_p1[3]),
        .I2(zext_ln99_3_fu_153_p1[4]),
        .I3(zext_ln99_3_fu_153_p1[1]),
        .I4(zext_ln99_3_fu_153_p1[2]),
        .I5(ap_CS_fsm_state9),
        .O(ap_NS_fsm[3]));
  LUT5 #(
    .INIT(32'hFFDF0000)) 
    \ap_CS_fsm[4]_i_1 
       (.I0(j_0_reg_82[3]),
        .I1(j_0_reg_82[2]),
        .I2(j_0_reg_82[1]),
        .I3(j_0_reg_82[0]),
        .I4(ap_CS_fsm_state4),
        .O(ap_NS_fsm[4]));
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
        .D(ap_CS_fsm_state6),
        .Q(ap_CS_fsm_state7),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[7] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_CS_fsm_state7),
        .Q(ap_CS_fsm_state8),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[8] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_CS_fsm_state8),
        .Q(ap_CS_fsm_state9),
        .R(ap_rst));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT2 #(
    .INIT(4'h2)) 
    ap_idle_INST_0
       (.I0(\ap_CS_fsm_reg_n_1_[0] ),
        .I1(ap_start),
        .O(ap_idle));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'h00200000)) 
    ap_ready_INST_0
       (.I0(\k_0_reg_60_reg_n_1_[3] ),
        .I1(\k_0_reg_60_reg_n_1_[2] ),
        .I2(\k_0_reg_60_reg_n_1_[1] ),
        .I3(\k_0_reg_60_reg_n_1_[0] ),
        .I4(ap_CS_fsm_state2),
        .O(ap_ready));
  FDRE \dist_addr_1_reg_288_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(add_ln99_1_reg_278[0]),
        .Q(dist_addr_1_reg_288[0]),
        .R(1'b0));
  FDRE \dist_addr_1_reg_288_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(add_ln99_1_reg_278[1]),
        .Q(dist_addr_1_reg_288[1]),
        .R(1'b0));
  FDRE \dist_addr_1_reg_288_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(add_ln99_1_reg_278[2]),
        .Q(dist_addr_1_reg_288[2]),
        .R(1'b0));
  FDRE \dist_addr_1_reg_288_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(add_ln99_1_reg_278[3]),
        .Q(dist_addr_1_reg_288[3]),
        .R(1'b0));
  FDRE \dist_addr_1_reg_288_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(add_ln99_1_reg_278[4]),
        .Q(dist_addr_1_reg_288[4]),
        .R(1'b0));
  FDRE \dist_addr_1_reg_288_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(add_ln99_1_reg_278[5]),
        .Q(dist_addr_1_reg_288[5]),
        .R(1'b0));
  FDRE \dist_addr_1_reg_288_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(add_ln99_1_reg_278[6]),
        .Q(dist_addr_1_reg_288[6]),
        .R(1'b0));
  LUT3 #(
    .INIT(8'h96)) 
    \dist_addr_reg_265[3]_i_2 
       (.I0(zext_ln99_3_fu_153_p1[1]),
        .I1(zext_ln99_3_fu_153_p1[3]),
        .I2(zext_ln96_reg_229[3]),
        .O(\dist_addr_reg_265[3]_i_2_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \dist_addr_reg_265[3]_i_3 
       (.I0(zext_ln99_3_fu_153_p1[2]),
        .I1(zext_ln96_reg_229[2]),
        .O(\dist_addr_reg_265[3]_i_3_n_1 ));
  LUT2 #(
    .INIT(4'h6)) 
    \dist_addr_reg_265[3]_i_4 
       (.I0(zext_ln99_3_fu_153_p1[1]),
        .I1(zext_ln96_reg_229[1]),
        .O(\dist_addr_reg_265[3]_i_4_n_1 ));
  LUT4 #(
    .INIT(16'h17C0)) 
    \dist_addr_reg_265[6]_i_2 
       (.I0(zext_ln99_3_fu_153_p1[1]),
        .I1(zext_ln99_3_fu_153_p1[4]),
        .I2(zext_ln99_3_fu_153_p1[2]),
        .I3(zext_ln99_3_fu_153_p1[3]),
        .O(\dist_addr_reg_265[6]_i_2_n_1 ));
  LUT4 #(
    .INIT(16'h8778)) 
    \dist_addr_reg_265[6]_i_3 
       (.I0(zext_ln99_3_fu_153_p1[3]),
        .I1(zext_ln99_3_fu_153_p1[1]),
        .I2(zext_ln99_3_fu_153_p1[2]),
        .I3(zext_ln99_3_fu_153_p1[4]),
        .O(\dist_addr_reg_265[6]_i_3_n_1 ));
  LUT4 #(
    .INIT(16'h3870)) 
    \dist_addr_reg_265[6]_i_4 
       (.I0(zext_ln99_3_fu_153_p1[2]),
        .I1(zext_ln99_3_fu_153_p1[3]),
        .I2(zext_ln99_3_fu_153_p1[4]),
        .I3(zext_ln99_3_fu_153_p1[1]),
        .O(\dist_addr_reg_265[6]_i_4_n_1 ));
  LUT4 #(
    .INIT(16'h17C0)) 
    \dist_addr_reg_265[6]_i_5 
       (.I0(zext_ln99_3_fu_153_p1[1]),
        .I1(zext_ln99_3_fu_153_p1[4]),
        .I2(zext_ln99_3_fu_153_p1[2]),
        .I3(zext_ln99_3_fu_153_p1[3]),
        .O(\dist_addr_reg_265[6]_i_5_n_1 ));
  LUT4 #(
    .INIT(16'h8778)) 
    \dist_addr_reg_265[6]_i_6 
       (.I0(zext_ln99_3_fu_153_p1[3]),
        .I1(zext_ln99_3_fu_153_p1[1]),
        .I2(zext_ln99_3_fu_153_p1[2]),
        .I3(zext_ln99_3_fu_153_p1[4]),
        .O(\dist_addr_reg_265[6]_i_6_n_1 ));
  FDRE \dist_addr_reg_265_reg[0] 
       (.C(ap_clk),
        .CE(j_0_reg_820),
        .D(add_ln99_fu_163_p2[0]),
        .Q(dist_addr_reg_265[0]),
        .R(1'b0));
  FDRE \dist_addr_reg_265_reg[1] 
       (.C(ap_clk),
        .CE(j_0_reg_820),
        .D(add_ln99_fu_163_p2[1]),
        .Q(dist_addr_reg_265[1]),
        .R(1'b0));
  FDRE \dist_addr_reg_265_reg[2] 
       (.C(ap_clk),
        .CE(j_0_reg_820),
        .D(add_ln99_fu_163_p2[2]),
        .Q(dist_addr_reg_265[2]),
        .R(1'b0));
  FDRE \dist_addr_reg_265_reg[3] 
       (.C(ap_clk),
        .CE(j_0_reg_820),
        .D(add_ln99_fu_163_p2[3]),
        .Q(dist_addr_reg_265[3]),
        .R(1'b0));
  CARRY4 \dist_addr_reg_265_reg[3]_i_1 
       (.CI(1'b0),
        .CO({\dist_addr_reg_265_reg[3]_i_1_n_1 ,\dist_addr_reg_265_reg[3]_i_1_n_2 ,\dist_addr_reg_265_reg[3]_i_1_n_3 ,\dist_addr_reg_265_reg[3]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({zext_ln96_reg_229[3],zext_ln99_3_fu_153_p1[2:1],1'b0}),
        .O(add_ln99_fu_163_p2[3:0]),
        .S({\dist_addr_reg_265[3]_i_2_n_1 ,\dist_addr_reg_265[3]_i_3_n_1 ,\dist_addr_reg_265[3]_i_4_n_1 ,zext_ln96_reg_229[0]}));
  FDRE \dist_addr_reg_265_reg[4] 
       (.C(ap_clk),
        .CE(j_0_reg_820),
        .D(add_ln99_fu_163_p2[4]),
        .Q(dist_addr_reg_265[4]),
        .R(1'b0));
  FDRE \dist_addr_reg_265_reg[5] 
       (.C(ap_clk),
        .CE(j_0_reg_820),
        .D(add_ln99_fu_163_p2[5]),
        .Q(dist_addr_reg_265[5]),
        .R(1'b0));
  FDRE \dist_addr_reg_265_reg[6] 
       (.C(ap_clk),
        .CE(j_0_reg_820),
        .D(add_ln99_fu_163_p2[6]),
        .Q(dist_addr_reg_265[6]),
        .R(1'b0));
  CARRY4 \dist_addr_reg_265_reg[6]_i_1 
       (.CI(\dist_addr_reg_265_reg[3]_i_1_n_1 ),
        .CO({\NLW_dist_addr_reg_265_reg[6]_i_1_CO_UNCONNECTED [3:2],\dist_addr_reg_265_reg[6]_i_1_n_3 ,\dist_addr_reg_265_reg[6]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,\dist_addr_reg_265[6]_i_2_n_1 ,\dist_addr_reg_265[6]_i_3_n_1 }),
        .O({\NLW_dist_addr_reg_265_reg[6]_i_1_O_UNCONNECTED [3],add_ln99_fu_163_p2[6:4]}),
        .S({1'b0,\dist_addr_reg_265[6]_i_4_n_1 ,\dist_addr_reg_265[6]_i_5_n_1 ,\dist_addr_reg_265[6]_i_6_n_1 }));
  (* SOFT_HLUTNM = "soft_lutpair15" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \dist_address0[0]_INST_0 
       (.I0(dist_addr_reg_265[0]),
        .I1(ap_CS_fsm_state6),
        .I2(add_ln99_1_reg_278[0]),
        .O(dist_address0[0]));
  (* SOFT_HLUTNM = "soft_lutpair16" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \dist_address0[1]_INST_0 
       (.I0(dist_addr_reg_265[1]),
        .I1(ap_CS_fsm_state6),
        .I2(add_ln99_1_reg_278[1]),
        .O(dist_address0[1]));
  (* SOFT_HLUTNM = "soft_lutpair17" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \dist_address0[2]_INST_0 
       (.I0(dist_addr_reg_265[2]),
        .I1(ap_CS_fsm_state6),
        .I2(add_ln99_1_reg_278[2]),
        .O(dist_address0[2]));
  (* SOFT_HLUTNM = "soft_lutpair17" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \dist_address0[3]_INST_0 
       (.I0(dist_addr_reg_265[3]),
        .I1(ap_CS_fsm_state6),
        .I2(add_ln99_1_reg_278[3]),
        .O(dist_address0[3]));
  (* SOFT_HLUTNM = "soft_lutpair16" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \dist_address0[4]_INST_0 
       (.I0(dist_addr_reg_265[4]),
        .I1(ap_CS_fsm_state6),
        .I2(add_ln99_1_reg_278[4]),
        .O(dist_address0[4]));
  (* SOFT_HLUTNM = "soft_lutpair15" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \dist_address0[5]_INST_0 
       (.I0(dist_addr_reg_265[5]),
        .I1(ap_CS_fsm_state6),
        .I2(add_ln99_1_reg_278[5]),
        .O(dist_address0[5]));
  (* SOFT_HLUTNM = "soft_lutpair14" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \dist_address0[6]_INST_0 
       (.I0(dist_addr_reg_265[6]),
        .I1(ap_CS_fsm_state6),
        .I2(add_ln99_1_reg_278[6]),
        .O(dist_address0[6]));
  (* SOFT_HLUTNM = "soft_lutpair10" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \dist_address1[0]_INST_0 
       (.I0(dist_addr_1_reg_288[0]),
        .I1(ap_CS_fsm_state9),
        .I2(add_ln99_1_reg_278[0]),
        .O(dist_address1[0]));
  (* SOFT_HLUTNM = "soft_lutpair11" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \dist_address1[1]_INST_0 
       (.I0(dist_addr_1_reg_288[1]),
        .I1(ap_CS_fsm_state9),
        .I2(add_ln99_2_reg_283[1]),
        .O(dist_address1[1]));
  (* SOFT_HLUTNM = "soft_lutpair12" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \dist_address1[2]_INST_0 
       (.I0(dist_addr_1_reg_288[2]),
        .I1(ap_CS_fsm_state9),
        .I2(add_ln99_2_reg_283[2]),
        .O(dist_address1[2]));
  (* SOFT_HLUTNM = "soft_lutpair13" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \dist_address1[3]_INST_0 
       (.I0(dist_addr_1_reg_288[3]),
        .I1(ap_CS_fsm_state9),
        .I2(add_ln99_2_reg_283[3]),
        .O(dist_address1[3]));
  LUT3 #(
    .INIT(8'hB8)) 
    \dist_address1[4]_INST_0 
       (.I0(dist_addr_1_reg_288[4]),
        .I1(ap_CS_fsm_state9),
        .I2(add_ln99_2_reg_283[4]),
        .O(dist_address1[4]));
  (* SOFT_HLUTNM = "soft_lutpair13" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \dist_address1[5]_INST_0 
       (.I0(dist_addr_1_reg_288[5]),
        .I1(ap_CS_fsm_state9),
        .I2(add_ln99_2_reg_283[5]),
        .O(dist_address1[5]));
  (* SOFT_HLUTNM = "soft_lutpair12" *) 
  LUT3 #(
    .INIT(8'hB8)) 
    \dist_address1[6]_INST_0 
       (.I0(dist_addr_1_reg_288[6]),
        .I1(ap_CS_fsm_state9),
        .I2(add_ln99_2_reg_283[6]),
        .O(dist_address1[6]));
  (* SOFT_HLUTNM = "soft_lutpair14" *) 
  LUT2 #(
    .INIT(4'hE)) 
    dist_ce0_INST_0
       (.I0(ap_CS_fsm_state6),
        .I1(ap_CS_fsm_state5),
        .O(dist_ce0));
  (* SOFT_HLUTNM = "soft_lutpair10" *) 
  LUT2 #(
    .INIT(4'hE)) 
    dist_ce1_INST_0
       (.I0(ap_CS_fsm_state6),
        .I1(ap_CS_fsm_state9),
        .O(dist_ce1));
  FDRE \dist_load_1_reg_304_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[0]),
        .Q(dist_load_1_reg_304[0]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[10]),
        .Q(dist_load_1_reg_304[10]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[11] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[11]),
        .Q(dist_load_1_reg_304[11]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[12] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[12]),
        .Q(dist_load_1_reg_304[12]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[13] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[13]),
        .Q(dist_load_1_reg_304[13]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[14] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[14]),
        .Q(dist_load_1_reg_304[14]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[15] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[15]),
        .Q(dist_load_1_reg_304[15]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[16] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[16]),
        .Q(dist_load_1_reg_304[16]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[17] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[17]),
        .Q(dist_load_1_reg_304[17]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[18] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[18]),
        .Q(dist_load_1_reg_304[18]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[19] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[19]),
        .Q(dist_load_1_reg_304[19]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[1]),
        .Q(dist_load_1_reg_304[1]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[20] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[20]),
        .Q(dist_load_1_reg_304[20]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[21] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[21]),
        .Q(dist_load_1_reg_304[21]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[22] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[22]),
        .Q(dist_load_1_reg_304[22]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[23] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[23]),
        .Q(dist_load_1_reg_304[23]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[24] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[24]),
        .Q(dist_load_1_reg_304[24]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[25] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[25]),
        .Q(dist_load_1_reg_304[25]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[26] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[26]),
        .Q(dist_load_1_reg_304[26]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[27] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[27]),
        .Q(dist_load_1_reg_304[27]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[28] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[28]),
        .Q(dist_load_1_reg_304[28]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[29] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[29]),
        .Q(dist_load_1_reg_304[29]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[2]),
        .Q(dist_load_1_reg_304[2]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[30] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[30]),
        .Q(dist_load_1_reg_304[30]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[31] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[31]),
        .Q(dist_load_1_reg_304[31]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[3]),
        .Q(dist_load_1_reg_304[3]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[4]),
        .Q(dist_load_1_reg_304[4]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[5]),
        .Q(dist_load_1_reg_304[5]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[6]),
        .Q(dist_load_1_reg_304[6]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[7]),
        .Q(dist_load_1_reg_304[7]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[8]),
        .Q(dist_load_1_reg_304[8]),
        .R(1'b0));
  FDRE \dist_load_1_reg_304_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q0[9]),
        .Q(dist_load_1_reg_304[9]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[0]),
        .Q(dist_load_2_reg_309[0]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[10]),
        .Q(dist_load_2_reg_309[10]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[11] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[11]),
        .Q(dist_load_2_reg_309[11]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[12] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[12]),
        .Q(dist_load_2_reg_309[12]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[13] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[13]),
        .Q(dist_load_2_reg_309[13]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[14] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[14]),
        .Q(dist_load_2_reg_309[14]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[15] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[15]),
        .Q(dist_load_2_reg_309[15]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[16] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[16]),
        .Q(dist_load_2_reg_309[16]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[17] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[17]),
        .Q(dist_load_2_reg_309[17]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[18] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[18]),
        .Q(dist_load_2_reg_309[18]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[19] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[19]),
        .Q(dist_load_2_reg_309[19]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[1]),
        .Q(dist_load_2_reg_309[1]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[20] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[20]),
        .Q(dist_load_2_reg_309[20]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[21] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[21]),
        .Q(dist_load_2_reg_309[21]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[22] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[22]),
        .Q(dist_load_2_reg_309[22]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[23] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[23]),
        .Q(dist_load_2_reg_309[23]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[24] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[24]),
        .Q(dist_load_2_reg_309[24]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[25] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[25]),
        .Q(dist_load_2_reg_309[25]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[26] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[26]),
        .Q(dist_load_2_reg_309[26]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[27] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[27]),
        .Q(dist_load_2_reg_309[27]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[28] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[28]),
        .Q(dist_load_2_reg_309[28]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[29] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[29]),
        .Q(dist_load_2_reg_309[29]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[2]),
        .Q(dist_load_2_reg_309[2]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[30] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[30]),
        .Q(dist_load_2_reg_309[30]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[31] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[31]),
        .Q(dist_load_2_reg_309[31]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[3]),
        .Q(dist_load_2_reg_309[3]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[4]),
        .Q(dist_load_2_reg_309[4]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[5]),
        .Q(dist_load_2_reg_309[5]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[6]),
        .Q(dist_load_2_reg_309[6]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[7]),
        .Q(dist_load_2_reg_309[7]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[8]),
        .Q(dist_load_2_reg_309[8]),
        .R(1'b0));
  FDRE \dist_load_2_reg_309_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(dist_q1[9]),
        .Q(dist_load_2_reg_309[9]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[0]),
        .Q(dist_load_reg_294[0]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[10]),
        .Q(dist_load_reg_294[10]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[11] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[11]),
        .Q(dist_load_reg_294[11]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[12] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[12]),
        .Q(dist_load_reg_294[12]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[13] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[13]),
        .Q(dist_load_reg_294[13]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[14] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[14]),
        .Q(dist_load_reg_294[14]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[15] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[15]),
        .Q(dist_load_reg_294[15]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[16] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[16]),
        .Q(dist_load_reg_294[16]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[17] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[17]),
        .Q(dist_load_reg_294[17]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[18] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[18]),
        .Q(dist_load_reg_294[18]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[19] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[19]),
        .Q(dist_load_reg_294[19]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[1]),
        .Q(dist_load_reg_294[1]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[20] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[20]),
        .Q(dist_load_reg_294[20]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[21] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[21]),
        .Q(dist_load_reg_294[21]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[22] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[22]),
        .Q(dist_load_reg_294[22]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[23] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[23]),
        .Q(dist_load_reg_294[23]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[24] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[24]),
        .Q(dist_load_reg_294[24]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[25] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[25]),
        .Q(dist_load_reg_294[25]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[26] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[26]),
        .Q(dist_load_reg_294[26]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[27] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[27]),
        .Q(dist_load_reg_294[27]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[28] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[28]),
        .Q(dist_load_reg_294[28]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[29] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[29]),
        .Q(dist_load_reg_294[29]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[2]),
        .Q(dist_load_reg_294[2]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[30] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[30]),
        .Q(dist_load_reg_294[30]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[31] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[31]),
        .Q(dist_load_reg_294[31]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[3]),
        .Q(dist_load_reg_294[3]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[4]),
        .Q(dist_load_reg_294[4]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[5]),
        .Q(dist_load_reg_294[5]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[6]),
        .Q(dist_load_reg_294[6]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[7]),
        .Q(dist_load_reg_294[7]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[8]),
        .Q(dist_load_reg_294[8]),
        .R(1'b0));
  FDRE \dist_load_reg_294_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state6),
        .D(dist_q0[9]),
        .Q(dist_load_reg_294[9]),
        .R(1'b0));
  (* SOFT_HLUTNM = "soft_lutpair11" *) 
  LUT2 #(
    .INIT(4'h8)) 
    dist_we1_INST_0
       (.I0(ap_CS_fsm_state9),
        .I1(icmp_ln99_reg_319),
        .O(dist_we1));
  LUT6 #(
    .INIT(64'h00000000FBFF0000)) 
    \i_0_reg_71[3]_i_1 
       (.I0(\k_0_reg_60_reg_n_1_[0] ),
        .I1(\k_0_reg_60_reg_n_1_[1] ),
        .I2(\k_0_reg_60_reg_n_1_[2] ),
        .I3(\k_0_reg_60_reg_n_1_[3] ),
        .I4(ap_CS_fsm_state2),
        .I5(ap_NS_fsm1),
        .O(i_0_reg_71));
  LUT5 #(
    .INIT(32'h00200000)) 
    \i_0_reg_71[3]_i_2 
       (.I0(ap_CS_fsm_state4),
        .I1(j_0_reg_82[0]),
        .I2(j_0_reg_82[1]),
        .I3(j_0_reg_82[2]),
        .I4(j_0_reg_82[3]),
        .O(ap_NS_fsm1));
  FDRE \i_0_reg_71_reg[0] 
       (.C(ap_clk),
        .CE(ap_NS_fsm1),
        .D(i_reg_255[0]),
        .Q(zext_ln99_3_fu_153_p1[1]),
        .R(i_0_reg_71));
  FDRE \i_0_reg_71_reg[1] 
       (.C(ap_clk),
        .CE(ap_NS_fsm1),
        .D(i_reg_255[1]),
        .Q(zext_ln99_3_fu_153_p1[2]),
        .R(i_0_reg_71));
  FDRE \i_0_reg_71_reg[2] 
       (.C(ap_clk),
        .CE(ap_NS_fsm1),
        .D(i_reg_255[2]),
        .Q(zext_ln99_3_fu_153_p1[3]),
        .R(i_0_reg_71));
  FDRE \i_0_reg_71_reg[3] 
       (.C(ap_clk),
        .CE(ap_NS_fsm1),
        .D(i_reg_255[3]),
        .Q(zext_ln99_3_fu_153_p1[4]),
        .R(i_0_reg_71));
  (* SOFT_HLUTNM = "soft_lutpair20" *) 
  LUT1 #(
    .INIT(2'h1)) 
    \i_reg_255[0]_i_1 
       (.I0(zext_ln99_3_fu_153_p1[1]),
        .O(i_fu_131_p2[0]));
  (* SOFT_HLUTNM = "soft_lutpair20" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \i_reg_255[1]_i_1 
       (.I0(zext_ln99_3_fu_153_p1[2]),
        .I1(zext_ln99_3_fu_153_p1[1]),
        .O(i_fu_131_p2[1]));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT3 #(
    .INIT(8'h78)) 
    \i_reg_255[2]_i_1 
       (.I0(zext_ln99_3_fu_153_p1[2]),
        .I1(zext_ln99_3_fu_153_p1[1]),
        .I2(zext_ln99_3_fu_153_p1[3]),
        .O(i_fu_131_p2[2]));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT4 #(
    .INIT(16'h6CCC)) 
    \i_reg_255[3]_i_1 
       (.I0(zext_ln99_3_fu_153_p1[3]),
        .I1(zext_ln99_3_fu_153_p1[4]),
        .I2(zext_ln99_3_fu_153_p1[1]),
        .I3(zext_ln99_3_fu_153_p1[2]),
        .O(i_fu_131_p2[3]));
  FDRE \i_reg_255_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(i_fu_131_p2[0]),
        .Q(i_reg_255[0]),
        .R(1'b0));
  FDRE \i_reg_255_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(i_fu_131_p2[1]),
        .Q(i_reg_255[1]),
        .R(1'b0));
  FDRE \i_reg_255_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(i_fu_131_p2[2]),
        .Q(i_reg_255[2]),
        .R(1'b0));
  FDRE \i_reg_255_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(i_fu_131_p2[3]),
        .Q(i_reg_255[3]),
        .R(1'b0));
  LUT4 #(
    .INIT(16'h9009)) 
    \icmp_ln99_reg_319[0]_i_10 
       (.I0(dist_load_reg_294[24]),
        .I1(add_ln99_3_fu_220_p2[24]),
        .I2(dist_load_reg_294[25]),
        .I3(add_ln99_3_fu_220_p2[25]),
        .O(\icmp_ln99_reg_319[0]_i_10_n_1 ));
  LUT4 #(
    .INIT(16'h2F02)) 
    \icmp_ln99_reg_319[0]_i_12 
       (.I0(dist_load_reg_294[22]),
        .I1(add_ln99_3_fu_220_p2[22]),
        .I2(add_ln99_3_fu_220_p2[23]),
        .I3(dist_load_reg_294[23]),
        .O(\icmp_ln99_reg_319[0]_i_12_n_1 ));
  LUT4 #(
    .INIT(16'h2F02)) 
    \icmp_ln99_reg_319[0]_i_13 
       (.I0(dist_load_reg_294[20]),
        .I1(add_ln99_3_fu_220_p2[20]),
        .I2(add_ln99_3_fu_220_p2[21]),
        .I3(dist_load_reg_294[21]),
        .O(\icmp_ln99_reg_319[0]_i_13_n_1 ));
  LUT4 #(
    .INIT(16'h2F02)) 
    \icmp_ln99_reg_319[0]_i_14 
       (.I0(dist_load_reg_294[18]),
        .I1(add_ln99_3_fu_220_p2[18]),
        .I2(add_ln99_3_fu_220_p2[19]),
        .I3(dist_load_reg_294[19]),
        .O(\icmp_ln99_reg_319[0]_i_14_n_1 ));
  LUT4 #(
    .INIT(16'h2F02)) 
    \icmp_ln99_reg_319[0]_i_15 
       (.I0(dist_load_reg_294[16]),
        .I1(add_ln99_3_fu_220_p2[16]),
        .I2(add_ln99_3_fu_220_p2[17]),
        .I3(dist_load_reg_294[17]),
        .O(\icmp_ln99_reg_319[0]_i_15_n_1 ));
  LUT4 #(
    .INIT(16'h9009)) 
    \icmp_ln99_reg_319[0]_i_16 
       (.I0(dist_load_reg_294[22]),
        .I1(add_ln99_3_fu_220_p2[22]),
        .I2(dist_load_reg_294[23]),
        .I3(add_ln99_3_fu_220_p2[23]),
        .O(\icmp_ln99_reg_319[0]_i_16_n_1 ));
  LUT4 #(
    .INIT(16'h9009)) 
    \icmp_ln99_reg_319[0]_i_17 
       (.I0(dist_load_reg_294[20]),
        .I1(add_ln99_3_fu_220_p2[20]),
        .I2(dist_load_reg_294[21]),
        .I3(add_ln99_3_fu_220_p2[21]),
        .O(\icmp_ln99_reg_319[0]_i_17_n_1 ));
  LUT4 #(
    .INIT(16'h9009)) 
    \icmp_ln99_reg_319[0]_i_18 
       (.I0(dist_load_reg_294[18]),
        .I1(add_ln99_3_fu_220_p2[18]),
        .I2(dist_load_reg_294[19]),
        .I3(add_ln99_3_fu_220_p2[19]),
        .O(\icmp_ln99_reg_319[0]_i_18_n_1 ));
  LUT4 #(
    .INIT(16'h9009)) 
    \icmp_ln99_reg_319[0]_i_19 
       (.I0(dist_load_reg_294[16]),
        .I1(add_ln99_3_fu_220_p2[16]),
        .I2(dist_load_reg_294[17]),
        .I3(add_ln99_3_fu_220_p2[17]),
        .O(\icmp_ln99_reg_319[0]_i_19_n_1 ));
  LUT4 #(
    .INIT(16'h2F02)) 
    \icmp_ln99_reg_319[0]_i_21 
       (.I0(dist_load_reg_294[14]),
        .I1(add_ln99_3_fu_220_p2[14]),
        .I2(add_ln99_3_fu_220_p2[15]),
        .I3(dist_load_reg_294[15]),
        .O(\icmp_ln99_reg_319[0]_i_21_n_1 ));
  LUT4 #(
    .INIT(16'h2F02)) 
    \icmp_ln99_reg_319[0]_i_22 
       (.I0(dist_load_reg_294[12]),
        .I1(add_ln99_3_fu_220_p2[12]),
        .I2(add_ln99_3_fu_220_p2[13]),
        .I3(dist_load_reg_294[13]),
        .O(\icmp_ln99_reg_319[0]_i_22_n_1 ));
  LUT4 #(
    .INIT(16'h2F02)) 
    \icmp_ln99_reg_319[0]_i_23 
       (.I0(dist_load_reg_294[10]),
        .I1(add_ln99_3_fu_220_p2[10]),
        .I2(add_ln99_3_fu_220_p2[11]),
        .I3(dist_load_reg_294[11]),
        .O(\icmp_ln99_reg_319[0]_i_23_n_1 ));
  LUT4 #(
    .INIT(16'h2F02)) 
    \icmp_ln99_reg_319[0]_i_24 
       (.I0(dist_load_reg_294[8]),
        .I1(add_ln99_3_fu_220_p2[8]),
        .I2(add_ln99_3_fu_220_p2[9]),
        .I3(dist_load_reg_294[9]),
        .O(\icmp_ln99_reg_319[0]_i_24_n_1 ));
  LUT4 #(
    .INIT(16'h9009)) 
    \icmp_ln99_reg_319[0]_i_25 
       (.I0(dist_load_reg_294[14]),
        .I1(add_ln99_3_fu_220_p2[14]),
        .I2(dist_load_reg_294[15]),
        .I3(add_ln99_3_fu_220_p2[15]),
        .O(\icmp_ln99_reg_319[0]_i_25_n_1 ));
  LUT4 #(
    .INIT(16'h9009)) 
    \icmp_ln99_reg_319[0]_i_26 
       (.I0(dist_load_reg_294[12]),
        .I1(add_ln99_3_fu_220_p2[12]),
        .I2(dist_load_reg_294[13]),
        .I3(add_ln99_3_fu_220_p2[13]),
        .O(\icmp_ln99_reg_319[0]_i_26_n_1 ));
  LUT4 #(
    .INIT(16'h9009)) 
    \icmp_ln99_reg_319[0]_i_27 
       (.I0(dist_load_reg_294[10]),
        .I1(add_ln99_3_fu_220_p2[10]),
        .I2(dist_load_reg_294[11]),
        .I3(add_ln99_3_fu_220_p2[11]),
        .O(\icmp_ln99_reg_319[0]_i_27_n_1 ));
  LUT4 #(
    .INIT(16'h9009)) 
    \icmp_ln99_reg_319[0]_i_28 
       (.I0(dist_load_reg_294[8]),
        .I1(add_ln99_3_fu_220_p2[8]),
        .I2(dist_load_reg_294[9]),
        .I3(add_ln99_3_fu_220_p2[9]),
        .O(\icmp_ln99_reg_319[0]_i_28_n_1 ));
  LUT4 #(
    .INIT(16'h2F02)) 
    \icmp_ln99_reg_319[0]_i_29 
       (.I0(dist_load_reg_294[6]),
        .I1(add_ln99_3_fu_220_p2[6]),
        .I2(add_ln99_3_fu_220_p2[7]),
        .I3(dist_load_reg_294[7]),
        .O(\icmp_ln99_reg_319[0]_i_29_n_1 ));
  LUT4 #(
    .INIT(16'h2F02)) 
    \icmp_ln99_reg_319[0]_i_3 
       (.I0(dist_load_reg_294[30]),
        .I1(add_ln99_3_fu_220_p2[30]),
        .I2(dist_load_reg_294[31]),
        .I3(add_ln99_3_fu_220_p2[31]),
        .O(\icmp_ln99_reg_319[0]_i_3_n_1 ));
  LUT4 #(
    .INIT(16'h2F02)) 
    \icmp_ln99_reg_319[0]_i_30 
       (.I0(dist_load_reg_294[4]),
        .I1(add_ln99_3_fu_220_p2[4]),
        .I2(add_ln99_3_fu_220_p2[5]),
        .I3(dist_load_reg_294[5]),
        .O(\icmp_ln99_reg_319[0]_i_30_n_1 ));
  LUT4 #(
    .INIT(16'h2F02)) 
    \icmp_ln99_reg_319[0]_i_31 
       (.I0(dist_load_reg_294[2]),
        .I1(add_ln99_3_fu_220_p2[2]),
        .I2(add_ln99_3_fu_220_p2[3]),
        .I3(dist_load_reg_294[3]),
        .O(\icmp_ln99_reg_319[0]_i_31_n_1 ));
  LUT4 #(
    .INIT(16'h2F02)) 
    \icmp_ln99_reg_319[0]_i_32 
       (.I0(dist_load_reg_294[0]),
        .I1(add_ln99_3_fu_220_p2[0]),
        .I2(add_ln99_3_fu_220_p2[1]),
        .I3(dist_load_reg_294[1]),
        .O(\icmp_ln99_reg_319[0]_i_32_n_1 ));
  LUT4 #(
    .INIT(16'h9009)) 
    \icmp_ln99_reg_319[0]_i_33 
       (.I0(dist_load_reg_294[6]),
        .I1(add_ln99_3_fu_220_p2[6]),
        .I2(dist_load_reg_294[7]),
        .I3(add_ln99_3_fu_220_p2[7]),
        .O(\icmp_ln99_reg_319[0]_i_33_n_1 ));
  LUT4 #(
    .INIT(16'h9009)) 
    \icmp_ln99_reg_319[0]_i_34 
       (.I0(dist_load_reg_294[4]),
        .I1(add_ln99_3_fu_220_p2[4]),
        .I2(dist_load_reg_294[5]),
        .I3(add_ln99_3_fu_220_p2[5]),
        .O(\icmp_ln99_reg_319[0]_i_34_n_1 ));
  LUT4 #(
    .INIT(16'h9009)) 
    \icmp_ln99_reg_319[0]_i_35 
       (.I0(dist_load_reg_294[2]),
        .I1(add_ln99_3_fu_220_p2[2]),
        .I2(dist_load_reg_294[3]),
        .I3(add_ln99_3_fu_220_p2[3]),
        .O(\icmp_ln99_reg_319[0]_i_35_n_1 ));
  LUT4 #(
    .INIT(16'h9009)) 
    \icmp_ln99_reg_319[0]_i_36 
       (.I0(dist_load_reg_294[0]),
        .I1(add_ln99_3_fu_220_p2[0]),
        .I2(dist_load_reg_294[1]),
        .I3(add_ln99_3_fu_220_p2[1]),
        .O(\icmp_ln99_reg_319[0]_i_36_n_1 ));
  LUT4 #(
    .INIT(16'h2F02)) 
    \icmp_ln99_reg_319[0]_i_4 
       (.I0(dist_load_reg_294[28]),
        .I1(add_ln99_3_fu_220_p2[28]),
        .I2(add_ln99_3_fu_220_p2[29]),
        .I3(dist_load_reg_294[29]),
        .O(\icmp_ln99_reg_319[0]_i_4_n_1 ));
  LUT4 #(
    .INIT(16'h2F02)) 
    \icmp_ln99_reg_319[0]_i_5 
       (.I0(dist_load_reg_294[26]),
        .I1(add_ln99_3_fu_220_p2[26]),
        .I2(add_ln99_3_fu_220_p2[27]),
        .I3(dist_load_reg_294[27]),
        .O(\icmp_ln99_reg_319[0]_i_5_n_1 ));
  LUT4 #(
    .INIT(16'h2F02)) 
    \icmp_ln99_reg_319[0]_i_6 
       (.I0(dist_load_reg_294[24]),
        .I1(add_ln99_3_fu_220_p2[24]),
        .I2(add_ln99_3_fu_220_p2[25]),
        .I3(dist_load_reg_294[25]),
        .O(\icmp_ln99_reg_319[0]_i_6_n_1 ));
  LUT4 #(
    .INIT(16'h9009)) 
    \icmp_ln99_reg_319[0]_i_7 
       (.I0(dist_load_reg_294[30]),
        .I1(add_ln99_3_fu_220_p2[30]),
        .I2(add_ln99_3_fu_220_p2[31]),
        .I3(dist_load_reg_294[31]),
        .O(\icmp_ln99_reg_319[0]_i_7_n_1 ));
  LUT4 #(
    .INIT(16'h9009)) 
    \icmp_ln99_reg_319[0]_i_8 
       (.I0(dist_load_reg_294[28]),
        .I1(add_ln99_3_fu_220_p2[28]),
        .I2(dist_load_reg_294[29]),
        .I3(add_ln99_3_fu_220_p2[29]),
        .O(\icmp_ln99_reg_319[0]_i_8_n_1 ));
  LUT4 #(
    .INIT(16'h9009)) 
    \icmp_ln99_reg_319[0]_i_9 
       (.I0(dist_load_reg_294[26]),
        .I1(add_ln99_3_fu_220_p2[26]),
        .I2(dist_load_reg_294[27]),
        .I3(add_ln99_3_fu_220_p2[27]),
        .O(\icmp_ln99_reg_319[0]_i_9_n_1 ));
  FDRE \icmp_ln99_reg_319_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state8),
        .D(icmp_ln99_fu_224_p2),
        .Q(icmp_ln99_reg_319),
        .R(1'b0));
  CARRY4 \icmp_ln99_reg_319_reg[0]_i_1 
       (.CI(\icmp_ln99_reg_319_reg[0]_i_2_n_1 ),
        .CO({icmp_ln99_fu_224_p2,\icmp_ln99_reg_319_reg[0]_i_1_n_2 ,\icmp_ln99_reg_319_reg[0]_i_1_n_3 ,\icmp_ln99_reg_319_reg[0]_i_1_n_4 }),
        .CYINIT(1'b0),
        .DI({\icmp_ln99_reg_319[0]_i_3_n_1 ,\icmp_ln99_reg_319[0]_i_4_n_1 ,\icmp_ln99_reg_319[0]_i_5_n_1 ,\icmp_ln99_reg_319[0]_i_6_n_1 }),
        .O(\NLW_icmp_ln99_reg_319_reg[0]_i_1_O_UNCONNECTED [3:0]),
        .S({\icmp_ln99_reg_319[0]_i_7_n_1 ,\icmp_ln99_reg_319[0]_i_8_n_1 ,\icmp_ln99_reg_319[0]_i_9_n_1 ,\icmp_ln99_reg_319[0]_i_10_n_1 }));
  CARRY4 \icmp_ln99_reg_319_reg[0]_i_11 
       (.CI(\icmp_ln99_reg_319_reg[0]_i_20_n_1 ),
        .CO({\icmp_ln99_reg_319_reg[0]_i_11_n_1 ,\icmp_ln99_reg_319_reg[0]_i_11_n_2 ,\icmp_ln99_reg_319_reg[0]_i_11_n_3 ,\icmp_ln99_reg_319_reg[0]_i_11_n_4 }),
        .CYINIT(1'b0),
        .DI({\icmp_ln99_reg_319[0]_i_21_n_1 ,\icmp_ln99_reg_319[0]_i_22_n_1 ,\icmp_ln99_reg_319[0]_i_23_n_1 ,\icmp_ln99_reg_319[0]_i_24_n_1 }),
        .O(\NLW_icmp_ln99_reg_319_reg[0]_i_11_O_UNCONNECTED [3:0]),
        .S({\icmp_ln99_reg_319[0]_i_25_n_1 ,\icmp_ln99_reg_319[0]_i_26_n_1 ,\icmp_ln99_reg_319[0]_i_27_n_1 ,\icmp_ln99_reg_319[0]_i_28_n_1 }));
  CARRY4 \icmp_ln99_reg_319_reg[0]_i_2 
       (.CI(\icmp_ln99_reg_319_reg[0]_i_11_n_1 ),
        .CO({\icmp_ln99_reg_319_reg[0]_i_2_n_1 ,\icmp_ln99_reg_319_reg[0]_i_2_n_2 ,\icmp_ln99_reg_319_reg[0]_i_2_n_3 ,\icmp_ln99_reg_319_reg[0]_i_2_n_4 }),
        .CYINIT(1'b0),
        .DI({\icmp_ln99_reg_319[0]_i_12_n_1 ,\icmp_ln99_reg_319[0]_i_13_n_1 ,\icmp_ln99_reg_319[0]_i_14_n_1 ,\icmp_ln99_reg_319[0]_i_15_n_1 }),
        .O(\NLW_icmp_ln99_reg_319_reg[0]_i_2_O_UNCONNECTED [3:0]),
        .S({\icmp_ln99_reg_319[0]_i_16_n_1 ,\icmp_ln99_reg_319[0]_i_17_n_1 ,\icmp_ln99_reg_319[0]_i_18_n_1 ,\icmp_ln99_reg_319[0]_i_19_n_1 }));
  CARRY4 \icmp_ln99_reg_319_reg[0]_i_20 
       (.CI(1'b0),
        .CO({\icmp_ln99_reg_319_reg[0]_i_20_n_1 ,\icmp_ln99_reg_319_reg[0]_i_20_n_2 ,\icmp_ln99_reg_319_reg[0]_i_20_n_3 ,\icmp_ln99_reg_319_reg[0]_i_20_n_4 }),
        .CYINIT(1'b0),
        .DI({\icmp_ln99_reg_319[0]_i_29_n_1 ,\icmp_ln99_reg_319[0]_i_30_n_1 ,\icmp_ln99_reg_319[0]_i_31_n_1 ,\icmp_ln99_reg_319[0]_i_32_n_1 }),
        .O(\NLW_icmp_ln99_reg_319_reg[0]_i_20_O_UNCONNECTED [3:0]),
        .S({\icmp_ln99_reg_319[0]_i_33_n_1 ,\icmp_ln99_reg_319[0]_i_34_n_1 ,\icmp_ln99_reg_319[0]_i_35_n_1 ,\icmp_ln99_reg_319[0]_i_36_n_1 }));
  LUT5 #(
    .INIT(32'hFBFF0000)) 
    \j_0_reg_82[3]_i_1 
       (.I0(zext_ln99_3_fu_153_p1[3]),
        .I1(zext_ln99_3_fu_153_p1[4]),
        .I2(zext_ln99_3_fu_153_p1[1]),
        .I3(zext_ln99_3_fu_153_p1[2]),
        .I4(ap_CS_fsm_state3),
        .O(j_0_reg_820));
  FDRE \j_0_reg_82_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state9),
        .D(j_reg_273[0]),
        .Q(j_0_reg_82[0]),
        .R(j_0_reg_820));
  FDRE \j_0_reg_82_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state9),
        .D(j_reg_273[1]),
        .Q(j_0_reg_82[1]),
        .R(j_0_reg_820));
  FDRE \j_0_reg_82_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state9),
        .D(j_reg_273[2]),
        .Q(j_0_reg_82[2]),
        .R(j_0_reg_820));
  FDRE \j_0_reg_82_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state9),
        .D(j_reg_273[3]),
        .Q(j_0_reg_82[3]),
        .R(j_0_reg_820));
  (* SOFT_HLUTNM = "soft_lutpair21" *) 
  LUT1 #(
    .INIT(2'h1)) 
    \j_reg_273[0]_i_1 
       (.I0(j_0_reg_82[0]),
        .O(j_fu_187_p2[0]));
  (* SOFT_HLUTNM = "soft_lutpair21" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \j_reg_273[1]_i_1 
       (.I0(j_0_reg_82[1]),
        .I1(j_0_reg_82[0]),
        .O(j_fu_187_p2[1]));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT3 #(
    .INIT(8'h78)) 
    \j_reg_273[2]_i_1 
       (.I0(j_0_reg_82[1]),
        .I1(j_0_reg_82[0]),
        .I2(j_0_reg_82[2]),
        .O(j_fu_187_p2[2]));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT4 #(
    .INIT(16'h6CCC)) 
    \j_reg_273[3]_i_1 
       (.I0(j_0_reg_82[2]),
        .I1(j_0_reg_82[3]),
        .I2(j_0_reg_82[0]),
        .I3(j_0_reg_82[1]),
        .O(j_fu_187_p2[3]));
  FDRE \j_reg_273_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(j_fu_187_p2[0]),
        .Q(j_reg_273[0]),
        .R(1'b0));
  FDRE \j_reg_273_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(j_fu_187_p2[1]),
        .Q(j_reg_273[1]),
        .R(1'b0));
  FDRE \j_reg_273_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(j_fu_187_p2[2]),
        .Q(j_reg_273[2]),
        .R(1'b0));
  FDRE \j_reg_273_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state4),
        .D(j_fu_187_p2[3]),
        .Q(j_reg_273[3]),
        .R(1'b0));
  LUT3 #(
    .INIT(8'h08)) 
    \k_0_reg_60[3]_i_1 
       (.I0(\ap_CS_fsm_reg_n_1_[0] ),
        .I1(ap_start),
        .I2(ap_NS_fsm10_out),
        .O(k_0_reg_60));
  LUT5 #(
    .INIT(32'h00200000)) 
    \k_0_reg_60[3]_i_2 
       (.I0(ap_CS_fsm_state3),
        .I1(zext_ln99_3_fu_153_p1[3]),
        .I2(zext_ln99_3_fu_153_p1[4]),
        .I3(zext_ln99_3_fu_153_p1[1]),
        .I4(zext_ln99_3_fu_153_p1[2]),
        .O(ap_NS_fsm10_out));
  FDRE \k_0_reg_60_reg[0] 
       (.C(ap_clk),
        .CE(ap_NS_fsm10_out),
        .D(k_reg_237[0]),
        .Q(\k_0_reg_60_reg_n_1_[0] ),
        .R(k_0_reg_60));
  FDRE \k_0_reg_60_reg[1] 
       (.C(ap_clk),
        .CE(ap_NS_fsm10_out),
        .D(k_reg_237[1]),
        .Q(\k_0_reg_60_reg_n_1_[1] ),
        .R(k_0_reg_60));
  FDRE \k_0_reg_60_reg[2] 
       (.C(ap_clk),
        .CE(ap_NS_fsm10_out),
        .D(k_reg_237[2]),
        .Q(\k_0_reg_60_reg_n_1_[2] ),
        .R(k_0_reg_60));
  FDRE \k_0_reg_60_reg[3] 
       (.C(ap_clk),
        .CE(ap_NS_fsm10_out),
        .D(k_reg_237[3]),
        .Q(\k_0_reg_60_reg_n_1_[3] ),
        .R(k_0_reg_60));
  (* SOFT_HLUTNM = "soft_lutpair19" *) 
  LUT1 #(
    .INIT(2'h1)) 
    \k_reg_237[0]_i_1 
       (.I0(\k_0_reg_60_reg_n_1_[0] ),
        .O(k_fu_103_p2[0]));
  (* SOFT_HLUTNM = "soft_lutpair19" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \k_reg_237[1]_i_1 
       (.I0(\k_0_reg_60_reg_n_1_[0] ),
        .I1(\k_0_reg_60_reg_n_1_[1] ),
        .O(k_fu_103_p2[1]));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT3 #(
    .INIT(8'h78)) 
    \k_reg_237[2]_i_1 
       (.I0(\k_0_reg_60_reg_n_1_[1] ),
        .I1(\k_0_reg_60_reg_n_1_[0] ),
        .I2(\k_0_reg_60_reg_n_1_[2] ),
        .O(k_fu_103_p2[2]));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT4 #(
    .INIT(16'h7F80)) 
    \k_reg_237[3]_i_1 
       (.I0(\k_0_reg_60_reg_n_1_[2] ),
        .I1(\k_0_reg_60_reg_n_1_[0] ),
        .I2(\k_0_reg_60_reg_n_1_[1] ),
        .I3(\k_0_reg_60_reg_n_1_[3] ),
        .O(k_fu_103_p2[3]));
  FDRE \k_reg_237_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state2),
        .D(k_fu_103_p2[0]),
        .Q(k_reg_237[0]),
        .R(1'b0));
  FDRE \k_reg_237_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state2),
        .D(k_fu_103_p2[1]),
        .Q(k_reg_237[1]),
        .R(1'b0));
  FDRE \k_reg_237_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state2),
        .D(k_fu_103_p2[2]),
        .Q(k_reg_237[2]),
        .R(1'b0));
  FDRE \k_reg_237_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state2),
        .D(k_fu_103_p2[3]),
        .Q(k_reg_237[3]),
        .R(1'b0));
  LUT5 #(
    .INIT(32'hAAAAA2AA)) 
    \shl_ln_reg_242[6]_i_1 
       (.I0(ap_CS_fsm_state2),
        .I1(\k_0_reg_60_reg_n_1_[3] ),
        .I2(\k_0_reg_60_reg_n_1_[2] ),
        .I3(\k_0_reg_60_reg_n_1_[1] ),
        .I4(\k_0_reg_60_reg_n_1_[0] ),
        .O(i_0_reg_710));
  FDRE \shl_ln_reg_242_reg[3] 
       (.C(ap_clk),
        .CE(i_0_reg_710),
        .D(\k_0_reg_60_reg_n_1_[0] ),
        .Q(shl_ln_reg_242_reg[0]),
        .R(1'b0));
  FDRE \shl_ln_reg_242_reg[4] 
       (.C(ap_clk),
        .CE(i_0_reg_710),
        .D(\k_0_reg_60_reg_n_1_[1] ),
        .Q(shl_ln_reg_242_reg[1]),
        .R(1'b0));
  FDRE \shl_ln_reg_242_reg[5] 
       (.C(ap_clk),
        .CE(i_0_reg_710),
        .D(\k_0_reg_60_reg_n_1_[2] ),
        .Q(shl_ln_reg_242_reg[2]),
        .R(1'b0));
  FDRE \shl_ln_reg_242_reg[6] 
       (.C(ap_clk),
        .CE(i_0_reg_710),
        .D(\k_0_reg_60_reg_n_1_[3] ),
        .Q(shl_ln_reg_242_reg[3]),
        .R(1'b0));
  FDRE \zext_ln96_reg_229_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state2),
        .D(\k_0_reg_60_reg_n_1_[0] ),
        .Q(zext_ln96_reg_229[0]),
        .R(1'b0));
  FDRE \zext_ln96_reg_229_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state2),
        .D(\k_0_reg_60_reg_n_1_[1] ),
        .Q(zext_ln96_reg_229[1]),
        .R(1'b0));
  FDRE \zext_ln96_reg_229_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state2),
        .D(\k_0_reg_60_reg_n_1_[2] ),
        .Q(zext_ln96_reg_229[2]),
        .R(1'b0));
  FDRE \zext_ln96_reg_229_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state2),
        .D(\k_0_reg_60_reg_n_1_[3] ),
        .Q(zext_ln96_reg_229[3]),
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
