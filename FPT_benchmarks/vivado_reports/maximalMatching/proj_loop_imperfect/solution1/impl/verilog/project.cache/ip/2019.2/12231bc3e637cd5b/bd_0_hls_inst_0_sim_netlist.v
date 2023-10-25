// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
// Date        : Fri Jun 23 11:57:06 2023
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
   (vertices_ce0,
    vertices_we0,
    vertices_ce1,
    vertices_we1,
    edges_ce0,
    edges_ce1,
    ap_clk,
    ap_rst,
    ap_start,
    ap_done,
    ap_idle,
    ap_ready,
    vertices_address0,
    vertices_d0,
    vertices_q0,
    vertices_address1,
    vertices_d1,
    vertices_q1,
    edges_address0,
    edges_q0,
    edges_address1,
    edges_q1);
  output vertices_ce0;
  output vertices_we0;
  output vertices_ce1;
  output vertices_we1;
  output edges_ce0;
  output edges_ce1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 ap_clk CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ap_clk, ASSOCIATED_RESET ap_rst, FREQ_HZ 250000000.0, PHASE 0.000, CLK_DOMAIN bd_0_ap_clk_0, INSERT_VIP 0" *) input ap_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 ap_rst RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ap_rst, POLARITY ACTIVE_HIGH, INSERT_VIP 0" *) input ap_rst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl start" *) input ap_start;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl done" *) output ap_done;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl idle" *) output ap_idle;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl ready" *) output ap_ready;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 vertices_address0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME vertices_address0, LAYERED_METADATA undef" *) output [10:0]vertices_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 vertices_d0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME vertices_d0, LAYERED_METADATA undef" *) output [31:0]vertices_d0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 vertices_q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME vertices_q0, LAYERED_METADATA undef" *) input [31:0]vertices_q0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 vertices_address1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME vertices_address1, LAYERED_METADATA undef" *) output [10:0]vertices_address1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 vertices_d1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME vertices_d1, LAYERED_METADATA undef" *) output [31:0]vertices_d1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 vertices_q1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME vertices_q1, LAYERED_METADATA undef" *) input [31:0]vertices_q1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 edges_address0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME edges_address0, LAYERED_METADATA undef" *) output [10:0]edges_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 edges_q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME edges_q0, LAYERED_METADATA undef" *) input [31:0]edges_q0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 edges_address1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME edges_address1, LAYERED_METADATA undef" *) output [10:0]edges_address1;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 edges_q1 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME edges_q1, LAYERED_METADATA undef" *) input [31:0]edges_q1;

  wire ap_clk;
  wire ap_done;
  wire ap_idle;
  wire ap_ready;
  wire ap_rst;
  wire ap_start;
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

  (* ap_ST_fsm_state1 = "7'b0000001" *) 
  (* ap_ST_fsm_state2 = "7'b0000010" *) 
  (* ap_ST_fsm_state3 = "7'b0000100" *) 
  (* ap_ST_fsm_state4 = "7'b0001000" *) 
  (* ap_ST_fsm_state5 = "7'b0010000" *) 
  (* ap_ST_fsm_state6 = "7'b0100000" *) 
  (* ap_ST_fsm_state7 = "7'b1000000" *) 
  decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect inst
       (.ap_clk(ap_clk),
        .ap_done(ap_done),
        .ap_idle(ap_idle),
        .ap_ready(ap_ready),
        .ap_rst(ap_rst),
        .ap_start(ap_start),
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

(* ap_ST_fsm_state1 = "7'b0000001" *) (* ap_ST_fsm_state2 = "7'b0000010" *) (* ap_ST_fsm_state3 = "7'b0000100" *) 
(* ap_ST_fsm_state4 = "7'b0001000" *) (* ap_ST_fsm_state5 = "7'b0010000" *) (* ap_ST_fsm_state6 = "7'b0100000" *) 
(* ap_ST_fsm_state7 = "7'b1000000" *) (* hls_module = "yes" *) 
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_loop_imperfect
   (ap_clk,
    ap_rst,
    ap_start,
    ap_done,
    ap_idle,
    ap_ready,
    vertices_address0,
    vertices_ce0,
    vertices_we0,
    vertices_d0,
    vertices_q0,
    vertices_address1,
    vertices_ce1,
    vertices_we1,
    vertices_d1,
    vertices_q1,
    edges_address0,
    edges_ce0,
    edges_q0,
    edges_address1,
    edges_ce1,
    edges_q1);
  input ap_clk;
  input ap_rst;
  input ap_start;
  output ap_done;
  output ap_idle;
  output ap_ready;
  output [10:0]vertices_address0;
  output vertices_ce0;
  output vertices_we0;
  output [31:0]vertices_d0;
  input [31:0]vertices_q0;
  output [10:0]vertices_address1;
  output vertices_ce1;
  output vertices_we1;
  output [31:0]vertices_d1;
  input [31:0]vertices_q1;
  output [10:0]edges_address0;
  output edges_ce0;
  input [31:0]edges_q0;
  output [10:0]edges_address1;
  output edges_ce1;
  input [31:0]edges_q1;

  wire \<const0> ;
  wire \<const1> ;
  wire \ap_CS_fsm[0]_i_2_n_1 ;
  wire \ap_CS_fsm[0]_i_3_n_1 ;
  wire \ap_CS_fsm_reg_n_1_[0] ;
  wire ap_CS_fsm_state3;
  wire ap_CS_fsm_state4;
  wire ap_CS_fsm_state5;
  wire ap_CS_fsm_state6;
  wire ap_CS_fsm_state7;
  wire [2:0]ap_NS_fsm;
  wire ap_clk;
  wire ap_idle;
  wire ap_ready;
  wire ap_ready_INST_0_i_1_n_1;
  wire ap_ready_INST_0_i_2_n_1;
  wire ap_rst;
  wire ap_start;
  wire [10:1]\^edges_address1 ;
  wire edges_ce1;
  wire [31:0]edges_q0;
  wire [31:0]edges_q1;
  wire i_0_reg_78;
  wire [9:0]i_fu_95_p2;
  wire [9:0]i_reg_148;
  wire \i_reg_148[8]_i_2_n_1 ;
  wire \i_reg_148[9]_i_2_n_1 ;
  wire icmp_ln108_reg_185;
  wire \icmp_ln108_reg_185[0]_i_1_n_1 ;
  wire \icmp_ln108_reg_185[0]_i_2_n_1 ;
  wire \icmp_ln108_reg_185[0]_i_3_n_1 ;
  wire \icmp_ln108_reg_185[0]_i_4_n_1 ;
  wire [10:0]vertices_address0;
  wire [10:0]vertices_address1;
  wire vertices_ce0;
  wire vertices_ce1;
  wire [31:0]vertices_d0;
  wire [31:0]vertices_d1;
  wire [31:0]vertices_q0;
  wire [31:0]vertices_q1;
  wire vertices_we0;
  wire vertices_we0_INST_0_i_10_n_1;
  wire vertices_we0_INST_0_i_11_n_1;
  wire vertices_we0_INST_0_i_12_n_1;
  wire vertices_we0_INST_0_i_13_n_1;
  wire vertices_we0_INST_0_i_14_n_1;
  wire vertices_we0_INST_0_i_15_n_1;
  wire vertices_we0_INST_0_i_16_n_1;
  wire vertices_we0_INST_0_i_17_n_1;
  wire vertices_we0_INST_0_i_18_n_1;
  wire vertices_we0_INST_0_i_19_n_1;
  wire vertices_we0_INST_0_i_1_n_1;
  wire vertices_we0_INST_0_i_2_n_1;
  wire vertices_we0_INST_0_i_3_n_1;
  wire vertices_we0_INST_0_i_4_n_1;
  wire vertices_we0_INST_0_i_5_n_1;
  wire vertices_we0_INST_0_i_6_n_1;
  wire vertices_we0_INST_0_i_7_n_1;
  wire vertices_we0_INST_0_i_8_n_1;
  wire vertices_we0_INST_0_i_9_n_1;
  wire vertices_we1;

  assign ap_done = ap_ready;
  assign edges_address0[10:1] = \^edges_address1 [10:1];
  assign edges_address0[0] = \<const0> ;
  assign edges_address1[10:1] = \^edges_address1 [10:1];
  assign edges_address1[0] = \<const1> ;
  assign edges_ce0 = edges_ce1;
  GND GND
       (.G(\<const0> ));
  VCC VCC
       (.P(\<const1> ));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT5 #(
    .INIT(32'h44444F44)) 
    \ap_CS_fsm[0]_i_1 
       (.I0(ap_start),
        .I1(\ap_CS_fsm_reg_n_1_[0] ),
        .I2(\ap_CS_fsm[0]_i_2_n_1 ),
        .I3(\ap_CS_fsm[0]_i_3_n_1 ),
        .I4(ap_ready_INST_0_i_1_n_1),
        .O(ap_NS_fsm[0]));
  LUT6 #(
    .INIT(64'hFFFFFFFFFFFFFFFE)) 
    \ap_CS_fsm[0]_i_2 
       (.I0(ap_CS_fsm_state6),
        .I1(ap_CS_fsm_state4),
        .I2(\ap_CS_fsm_reg_n_1_[0] ),
        .I3(ap_CS_fsm_state5),
        .I4(ap_CS_fsm_state7),
        .I5(ap_CS_fsm_state3),
        .O(\ap_CS_fsm[0]_i_2_n_1 ));
  LUT4 #(
    .INIT(16'h8000)) 
    \ap_CS_fsm[0]_i_3 
       (.I0(\^edges_address1 [10]),
        .I1(edges_ce1),
        .I2(\^edges_address1 [7]),
        .I3(\^edges_address1 [4]),
        .O(\ap_CS_fsm[0]_i_3_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT3 #(
    .INIT(8'hEA)) 
    \ap_CS_fsm[1]_i_1 
       (.I0(ap_CS_fsm_state7),
        .I1(\ap_CS_fsm_reg_n_1_[0] ),
        .I2(ap_start),
        .O(ap_NS_fsm[1]));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT5 #(
    .INIT(32'hB0F0F0F0)) 
    \ap_CS_fsm[2]_i_1 
       (.I0(ap_ready_INST_0_i_1_n_1),
        .I1(\^edges_address1 [10]),
        .I2(edges_ce1),
        .I3(\^edges_address1 [7]),
        .I4(\^edges_address1 [4]),
        .O(ap_NS_fsm[2]));
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
        .Q(edges_ce1),
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
        .D(ap_CS_fsm_state3),
        .Q(ap_CS_fsm_state4),
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
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT2 #(
    .INIT(4'h2)) 
    ap_idle_INST_0
       (.I0(\ap_CS_fsm_reg_n_1_[0] ),
        .I1(ap_start),
        .O(ap_idle));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT5 #(
    .INIT(32'h00008000)) 
    ap_ready_INST_0
       (.I0(\^edges_address1 [4]),
        .I1(\^edges_address1 [7]),
        .I2(edges_ce1),
        .I3(\^edges_address1 [10]),
        .I4(ap_ready_INST_0_i_1_n_1),
        .O(ap_ready));
  LUT6 #(
    .INIT(64'hFFFFFFFFFFFFFFEF)) 
    ap_ready_INST_0_i_1
       (.I0(\^edges_address1 [5]),
        .I1(\^edges_address1 [3]),
        .I2(\^edges_address1 [6]),
        .I3(ap_ready_INST_0_i_2_n_1),
        .I4(\^edges_address1 [1]),
        .I5(\^edges_address1 [2]),
        .O(ap_ready_INST_0_i_1_n_1));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT2 #(
    .INIT(4'h7)) 
    ap_ready_INST_0_i_2
       (.I0(\^edges_address1 [8]),
        .I1(\^edges_address1 [9]),
        .O(ap_ready_INST_0_i_2_n_1));
  LUT3 #(
    .INIT(8'h08)) 
    \i_0_reg_78[9]_i_1 
       (.I0(\ap_CS_fsm_reg_n_1_[0] ),
        .I1(ap_start),
        .I2(ap_CS_fsm_state7),
        .O(i_0_reg_78));
  FDRE \i_0_reg_78_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(i_reg_148[0]),
        .Q(\^edges_address1 [1]),
        .R(i_0_reg_78));
  FDRE \i_0_reg_78_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(i_reg_148[1]),
        .Q(\^edges_address1 [2]),
        .R(i_0_reg_78));
  FDRE \i_0_reg_78_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(i_reg_148[2]),
        .Q(\^edges_address1 [3]),
        .R(i_0_reg_78));
  FDRE \i_0_reg_78_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(i_reg_148[3]),
        .Q(\^edges_address1 [4]),
        .R(i_0_reg_78));
  FDRE \i_0_reg_78_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(i_reg_148[4]),
        .Q(\^edges_address1 [5]),
        .R(i_0_reg_78));
  FDRE \i_0_reg_78_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(i_reg_148[5]),
        .Q(\^edges_address1 [6]),
        .R(i_0_reg_78));
  FDRE \i_0_reg_78_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(i_reg_148[6]),
        .Q(\^edges_address1 [7]),
        .R(i_0_reg_78));
  FDRE \i_0_reg_78_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(i_reg_148[7]),
        .Q(\^edges_address1 [8]),
        .R(i_0_reg_78));
  FDRE \i_0_reg_78_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(i_reg_148[8]),
        .Q(\^edges_address1 [9]),
        .R(i_0_reg_78));
  FDRE \i_0_reg_78_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state7),
        .D(i_reg_148[9]),
        .Q(\^edges_address1 [10]),
        .R(i_0_reg_78));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT1 #(
    .INIT(2'h1)) 
    \i_reg_148[0]_i_1 
       (.I0(\^edges_address1 [1]),
        .O(i_fu_95_p2[0]));
  (* SOFT_HLUTNM = "soft_lutpair9" *) 
  LUT2 #(
    .INIT(4'h6)) 
    \i_reg_148[1]_i_1 
       (.I0(\^edges_address1 [1]),
        .I1(\^edges_address1 [2]),
        .O(i_fu_95_p2[1]));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT3 #(
    .INIT(8'h6A)) 
    \i_reg_148[2]_i_1 
       (.I0(\^edges_address1 [3]),
        .I1(\^edges_address1 [1]),
        .I2(\^edges_address1 [2]),
        .O(i_fu_95_p2[2]));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT4 #(
    .INIT(16'h7F80)) 
    \i_reg_148[3]_i_1 
       (.I0(\^edges_address1 [2]),
        .I1(\^edges_address1 [1]),
        .I2(\^edges_address1 [3]),
        .I3(\^edges_address1 [4]),
        .O(i_fu_95_p2[3]));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT5 #(
    .INIT(32'h6AAAAAAA)) 
    \i_reg_148[4]_i_1 
       (.I0(\^edges_address1 [5]),
        .I1(\^edges_address1 [2]),
        .I2(\^edges_address1 [1]),
        .I3(\^edges_address1 [3]),
        .I4(\^edges_address1 [4]),
        .O(i_fu_95_p2[4]));
  LUT6 #(
    .INIT(64'h6AAAAAAAAAAAAAAA)) 
    \i_reg_148[5]_i_1 
       (.I0(\^edges_address1 [6]),
        .I1(\^edges_address1 [4]),
        .I2(\^edges_address1 [3]),
        .I3(\^edges_address1 [1]),
        .I4(\^edges_address1 [2]),
        .I5(\^edges_address1 [5]),
        .O(i_fu_95_p2[5]));
  LUT4 #(
    .INIT(16'h6AAA)) 
    \i_reg_148[6]_i_1 
       (.I0(\^edges_address1 [7]),
        .I1(\^edges_address1 [5]),
        .I2(\i_reg_148[8]_i_2_n_1 ),
        .I3(\^edges_address1 [6]),
        .O(i_fu_95_p2[6]));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT5 #(
    .INIT(32'h6AAAAAAA)) 
    \i_reg_148[7]_i_1 
       (.I0(\^edges_address1 [8]),
        .I1(\^edges_address1 [6]),
        .I2(\i_reg_148[8]_i_2_n_1 ),
        .I3(\^edges_address1 [5]),
        .I4(\^edges_address1 [7]),
        .O(i_fu_95_p2[7]));
  LUT6 #(
    .INIT(64'h6AAAAAAAAAAAAAAA)) 
    \i_reg_148[8]_i_1 
       (.I0(\^edges_address1 [9]),
        .I1(\^edges_address1 [7]),
        .I2(\^edges_address1 [5]),
        .I3(\i_reg_148[8]_i_2_n_1 ),
        .I4(\^edges_address1 [6]),
        .I5(\^edges_address1 [8]),
        .O(i_fu_95_p2[8]));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT4 #(
    .INIT(16'h8000)) 
    \i_reg_148[8]_i_2 
       (.I0(\^edges_address1 [4]),
        .I1(\^edges_address1 [3]),
        .I2(\^edges_address1 [1]),
        .I3(\^edges_address1 [2]),
        .O(\i_reg_148[8]_i_2_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT4 #(
    .INIT(16'h6AAA)) 
    \i_reg_148[9]_i_1 
       (.I0(\^edges_address1 [10]),
        .I1(\^edges_address1 [8]),
        .I2(\^edges_address1 [9]),
        .I3(\i_reg_148[9]_i_2_n_1 ),
        .O(i_fu_95_p2[9]));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT4 #(
    .INIT(16'h8000)) 
    \i_reg_148[9]_i_2 
       (.I0(\^edges_address1 [7]),
        .I1(\^edges_address1 [5]),
        .I2(\i_reg_148[8]_i_2_n_1 ),
        .I3(\^edges_address1 [6]),
        .O(\i_reg_148[9]_i_2_n_1 ));
  FDRE \i_reg_148_reg[0] 
       (.C(ap_clk),
        .CE(edges_ce1),
        .D(i_fu_95_p2[0]),
        .Q(i_reg_148[0]),
        .R(1'b0));
  FDRE \i_reg_148_reg[1] 
       (.C(ap_clk),
        .CE(edges_ce1),
        .D(i_fu_95_p2[1]),
        .Q(i_reg_148[1]),
        .R(1'b0));
  FDRE \i_reg_148_reg[2] 
       (.C(ap_clk),
        .CE(edges_ce1),
        .D(i_fu_95_p2[2]),
        .Q(i_reg_148[2]),
        .R(1'b0));
  FDRE \i_reg_148_reg[3] 
       (.C(ap_clk),
        .CE(edges_ce1),
        .D(i_fu_95_p2[3]),
        .Q(i_reg_148[3]),
        .R(1'b0));
  FDRE \i_reg_148_reg[4] 
       (.C(ap_clk),
        .CE(edges_ce1),
        .D(i_fu_95_p2[4]),
        .Q(i_reg_148[4]),
        .R(1'b0));
  FDRE \i_reg_148_reg[5] 
       (.C(ap_clk),
        .CE(edges_ce1),
        .D(i_fu_95_p2[5]),
        .Q(i_reg_148[5]),
        .R(1'b0));
  FDRE \i_reg_148_reg[6] 
       (.C(ap_clk),
        .CE(edges_ce1),
        .D(i_fu_95_p2[6]),
        .Q(i_reg_148[6]),
        .R(1'b0));
  FDRE \i_reg_148_reg[7] 
       (.C(ap_clk),
        .CE(edges_ce1),
        .D(i_fu_95_p2[7]),
        .Q(i_reg_148[7]),
        .R(1'b0));
  FDRE \i_reg_148_reg[8] 
       (.C(ap_clk),
        .CE(edges_ce1),
        .D(i_fu_95_p2[8]),
        .Q(i_reg_148[8]),
        .R(1'b0));
  FDRE \i_reg_148_reg[9] 
       (.C(ap_clk),
        .CE(edges_ce1),
        .D(i_fu_95_p2[9]),
        .Q(i_reg_148[9]),
        .R(1'b0));
  LUT6 #(
    .INIT(64'h0004FFFF00040000)) 
    \icmp_ln108_reg_185[0]_i_1 
       (.I0(vertices_we0_INST_0_i_5_n_1),
        .I1(\icmp_ln108_reg_185[0]_i_2_n_1 ),
        .I2(vertices_we0_INST_0_i_2_n_1),
        .I3(vertices_we0_INST_0_i_1_n_1),
        .I4(ap_CS_fsm_state6),
        .I5(icmp_ln108_reg_185),
        .O(\icmp_ln108_reg_185[0]_i_1_n_1 ));
  LUT4 #(
    .INIT(16'h0004)) 
    \icmp_ln108_reg_185[0]_i_2 
       (.I0(vertices_we0_INST_0_i_15_n_1),
        .I1(\icmp_ln108_reg_185[0]_i_3_n_1 ),
        .I2(vertices_we0_INST_0_i_14_n_1),
        .I3(\icmp_ln108_reg_185[0]_i_4_n_1 ),
        .O(\icmp_ln108_reg_185[0]_i_2_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT4 #(
    .INIT(16'h0001)) 
    \icmp_ln108_reg_185[0]_i_3 
       (.I0(vertices_d1[12]),
        .I1(vertices_d1[0]),
        .I2(vertices_d0[30]),
        .I3(vertices_d1[15]),
        .O(\icmp_ln108_reg_185[0]_i_3_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT4 #(
    .INIT(16'hFFFE)) 
    \icmp_ln108_reg_185[0]_i_4 
       (.I0(vertices_d0[27]),
        .I1(vertices_d0[18]),
        .I2(vertices_d0[24]),
        .I3(vertices_d0[5]),
        .O(\icmp_ln108_reg_185[0]_i_4_n_1 ));
  FDRE \icmp_ln108_reg_185_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\icmp_ln108_reg_185[0]_i_1_n_1 ),
        .Q(icmp_ln108_reg_185),
        .R(1'b0));
  FDRE \v1_reg_173_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[0]),
        .Q(vertices_d1[0]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[10]),
        .Q(vertices_d1[10]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[11] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[11]),
        .Q(vertices_d1[11]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[12] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[12]),
        .Q(vertices_d1[12]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[13] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[13]),
        .Q(vertices_d1[13]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[14] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[14]),
        .Q(vertices_d1[14]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[15] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[15]),
        .Q(vertices_d1[15]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[16] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[16]),
        .Q(vertices_d1[16]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[17] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[17]),
        .Q(vertices_d1[17]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[18] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[18]),
        .Q(vertices_d1[18]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[19] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[19]),
        .Q(vertices_d1[19]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[1]),
        .Q(vertices_d1[1]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[20] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[20]),
        .Q(vertices_d1[20]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[21] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[21]),
        .Q(vertices_d1[21]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[22] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[22]),
        .Q(vertices_d1[22]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[23] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[23]),
        .Q(vertices_d1[23]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[24] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[24]),
        .Q(vertices_d1[24]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[25] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[25]),
        .Q(vertices_d1[25]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[26] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[26]),
        .Q(vertices_d1[26]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[27] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[27]),
        .Q(vertices_d1[27]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[28] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[28]),
        .Q(vertices_d1[28]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[29] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[29]),
        .Q(vertices_d1[29]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[2]),
        .Q(vertices_d1[2]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[30] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[30]),
        .Q(vertices_d1[30]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[31] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[31]),
        .Q(vertices_d1[31]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[3]),
        .Q(vertices_d1[3]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[4]),
        .Q(vertices_d1[4]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[5]),
        .Q(vertices_d1[5]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[6]),
        .Q(vertices_d1[6]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[7]),
        .Q(vertices_d1[7]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[8]),
        .Q(vertices_d1[8]),
        .R(1'b0));
  FDRE \v1_reg_173_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q0[9]),
        .Q(vertices_d1[9]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[0]),
        .Q(vertices_d0[0]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[10]),
        .Q(vertices_d0[10]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[11] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[11]),
        .Q(vertices_d0[11]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[12] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[12]),
        .Q(vertices_d0[12]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[13] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[13]),
        .Q(vertices_d0[13]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[14] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[14]),
        .Q(vertices_d0[14]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[15] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[15]),
        .Q(vertices_d0[15]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[16] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[16]),
        .Q(vertices_d0[16]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[17] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[17]),
        .Q(vertices_d0[17]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[18] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[18]),
        .Q(vertices_d0[18]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[19] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[19]),
        .Q(vertices_d0[19]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[1]),
        .Q(vertices_d0[1]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[20] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[20]),
        .Q(vertices_d0[20]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[21] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[21]),
        .Q(vertices_d0[21]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[22] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[22]),
        .Q(vertices_d0[22]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[23] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[23]),
        .Q(vertices_d0[23]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[24] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[24]),
        .Q(vertices_d0[24]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[25] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[25]),
        .Q(vertices_d0[25]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[26] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[26]),
        .Q(vertices_d0[26]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[27] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[27]),
        .Q(vertices_d0[27]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[28] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[28]),
        .Q(vertices_d0[28]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[29] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[29]),
        .Q(vertices_d0[29]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[2]),
        .Q(vertices_d0[2]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[30] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[30]),
        .Q(vertices_d0[30]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[31] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[31]),
        .Q(vertices_d0[31]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[3]),
        .Q(vertices_d0[3]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[4]),
        .Q(vertices_d0[4]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[5]),
        .Q(vertices_d0[5]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[6]),
        .Q(vertices_d0[6]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[7]),
        .Q(vertices_d0[7]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[8]),
        .Q(vertices_d0[8]),
        .R(1'b0));
  FDRE \v2_reg_179_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state5),
        .D(vertices_q1[9]),
        .Q(vertices_d0[9]),
        .R(1'b0));
  FDRE \vertices_addr_1_reg_168_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q1[0]),
        .Q(vertices_address1[0]),
        .R(1'b0));
  FDRE \vertices_addr_1_reg_168_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q1[10]),
        .Q(vertices_address1[10]),
        .R(1'b0));
  FDRE \vertices_addr_1_reg_168_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q1[1]),
        .Q(vertices_address1[1]),
        .R(1'b0));
  FDRE \vertices_addr_1_reg_168_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q1[2]),
        .Q(vertices_address1[2]),
        .R(1'b0));
  FDRE \vertices_addr_1_reg_168_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q1[3]),
        .Q(vertices_address1[3]),
        .R(1'b0));
  FDRE \vertices_addr_1_reg_168_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q1[4]),
        .Q(vertices_address1[4]),
        .R(1'b0));
  FDRE \vertices_addr_1_reg_168_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q1[5]),
        .Q(vertices_address1[5]),
        .R(1'b0));
  FDRE \vertices_addr_1_reg_168_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q1[6]),
        .Q(vertices_address1[6]),
        .R(1'b0));
  FDRE \vertices_addr_1_reg_168_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q1[7]),
        .Q(vertices_address1[7]),
        .R(1'b0));
  FDRE \vertices_addr_1_reg_168_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q1[8]),
        .Q(vertices_address1[8]),
        .R(1'b0));
  FDRE \vertices_addr_1_reg_168_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q1[9]),
        .Q(vertices_address1[9]),
        .R(1'b0));
  FDRE \vertices_addr_reg_163_reg[0] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q0[0]),
        .Q(vertices_address0[0]),
        .R(1'b0));
  FDRE \vertices_addr_reg_163_reg[10] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q0[10]),
        .Q(vertices_address0[10]),
        .R(1'b0));
  FDRE \vertices_addr_reg_163_reg[1] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q0[1]),
        .Q(vertices_address0[1]),
        .R(1'b0));
  FDRE \vertices_addr_reg_163_reg[2] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q0[2]),
        .Q(vertices_address0[2]),
        .R(1'b0));
  FDRE \vertices_addr_reg_163_reg[3] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q0[3]),
        .Q(vertices_address0[3]),
        .R(1'b0));
  FDRE \vertices_addr_reg_163_reg[4] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q0[4]),
        .Q(vertices_address0[4]),
        .R(1'b0));
  FDRE \vertices_addr_reg_163_reg[5] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q0[5]),
        .Q(vertices_address0[5]),
        .R(1'b0));
  FDRE \vertices_addr_reg_163_reg[6] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q0[6]),
        .Q(vertices_address0[6]),
        .R(1'b0));
  FDRE \vertices_addr_reg_163_reg[7] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q0[7]),
        .Q(vertices_address0[7]),
        .R(1'b0));
  FDRE \vertices_addr_reg_163_reg[8] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q0[8]),
        .Q(vertices_address0[8]),
        .R(1'b0));
  FDRE \vertices_addr_reg_163_reg[9] 
       (.C(ap_clk),
        .CE(ap_CS_fsm_state3),
        .D(edges_q0[9]),
        .Q(vertices_address0[9]),
        .R(1'b0));
  LUT2 #(
    .INIT(4'hE)) 
    vertices_ce0_INST_0
       (.I0(ap_CS_fsm_state4),
        .I1(ap_CS_fsm_state6),
        .O(vertices_ce0));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT2 #(
    .INIT(4'hE)) 
    vertices_ce1_INST_0
       (.I0(ap_CS_fsm_state7),
        .I1(ap_CS_fsm_state4),
        .O(vertices_ce1));
  LUT6 #(
    .INIT(64'h0000000000020000)) 
    vertices_we0_INST_0
       (.I0(ap_CS_fsm_state6),
        .I1(vertices_we0_INST_0_i_1_n_1),
        .I2(vertices_we0_INST_0_i_2_n_1),
        .I3(vertices_we0_INST_0_i_3_n_1),
        .I4(vertices_we0_INST_0_i_4_n_1),
        .I5(vertices_we0_INST_0_i_5_n_1),
        .O(vertices_we0));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_1
       (.I0(vertices_we0_INST_0_i_6_n_1),
        .I1(vertices_we0_INST_0_i_7_n_1),
        .I2(vertices_we0_INST_0_i_8_n_1),
        .I3(vertices_we0_INST_0_i_9_n_1),
        .O(vertices_we0_INST_0_i_1_n_1));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_10
       (.I0(vertices_d0[28]),
        .I1(vertices_d0[19]),
        .I2(vertices_d0[6]),
        .I3(vertices_d0[3]),
        .O(vertices_we0_INST_0_i_10_n_1));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_11
       (.I0(vertices_d0[31]),
        .I1(vertices_d0[16]),
        .I2(vertices_d0[26]),
        .I3(vertices_d0[25]),
        .O(vertices_we0_INST_0_i_11_n_1));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_12
       (.I0(vertices_d1[30]),
        .I1(vertices_d1[17]),
        .I2(vertices_d1[27]),
        .I3(vertices_d1[24]),
        .O(vertices_we0_INST_0_i_12_n_1));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_13
       (.I0(vertices_d1[7]),
        .I1(vertices_d1[2]),
        .I2(vertices_d1[28]),
        .I3(vertices_d1[4]),
        .O(vertices_we0_INST_0_i_13_n_1));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_14
       (.I0(vertices_d0[17]),
        .I1(vertices_d0[11]),
        .I2(vertices_d0[29]),
        .I3(vertices_d0[8]),
        .O(vertices_we0_INST_0_i_14_n_1));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_15
       (.I0(vertices_d1[14]),
        .I1(vertices_d1[5]),
        .I2(vertices_d1[13]),
        .I3(vertices_d1[1]),
        .O(vertices_we0_INST_0_i_15_n_1));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_16
       (.I0(vertices_d1[22]),
        .I1(vertices_d1[8]),
        .I2(vertices_d1[23]),
        .I3(vertices_d1[20]),
        .O(vertices_we0_INST_0_i_16_n_1));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_17
       (.I0(vertices_d1[21]),
        .I1(vertices_d1[11]),
        .I2(vertices_d1[29]),
        .I3(vertices_d1[18]),
        .O(vertices_we0_INST_0_i_17_n_1));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_18
       (.I0(vertices_d0[23]),
        .I1(vertices_d0[9]),
        .I2(vertices_d0[22]),
        .I3(vertices_d0[21]),
        .O(vertices_we0_INST_0_i_18_n_1));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_19
       (.I0(vertices_d0[20]),
        .I1(vertices_d0[10]),
        .I2(vertices_d1[6]),
        .I3(vertices_d1[3]),
        .O(vertices_we0_INST_0_i_19_n_1));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_2
       (.I0(vertices_we0_INST_0_i_10_n_1),
        .I1(vertices_we0_INST_0_i_11_n_1),
        .I2(vertices_we0_INST_0_i_12_n_1),
        .I3(vertices_we0_INST_0_i_13_n_1),
        .O(vertices_we0_INST_0_i_2_n_1));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT5 #(
    .INIT(32'hFFFFFFFE)) 
    vertices_we0_INST_0_i_3
       (.I0(vertices_d0[5]),
        .I1(vertices_d0[24]),
        .I2(vertices_d0[18]),
        .I3(vertices_d0[27]),
        .I4(vertices_we0_INST_0_i_14_n_1),
        .O(vertices_we0_INST_0_i_3_n_1));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'h00000001)) 
    vertices_we0_INST_0_i_4
       (.I0(vertices_d1[15]),
        .I1(vertices_d0[30]),
        .I2(vertices_d1[0]),
        .I3(vertices_d1[12]),
        .I4(vertices_we0_INST_0_i_15_n_1),
        .O(vertices_we0_INST_0_i_4_n_1));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_5
       (.I0(vertices_we0_INST_0_i_16_n_1),
        .I1(vertices_we0_INST_0_i_17_n_1),
        .I2(vertices_we0_INST_0_i_18_n_1),
        .I3(vertices_we0_INST_0_i_19_n_1),
        .O(vertices_we0_INST_0_i_5_n_1));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_6
       (.I0(vertices_d1[26]),
        .I1(vertices_d1[19]),
        .I2(vertices_d1[31]),
        .I3(vertices_d1[25]),
        .O(vertices_we0_INST_0_i_6_n_1));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_7
       (.I0(vertices_d1[16]),
        .I1(vertices_d1[10]),
        .I2(vertices_d0[15]),
        .I3(vertices_d1[9]),
        .O(vertices_we0_INST_0_i_7_n_1));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_8
       (.I0(vertices_d0[12]),
        .I1(vertices_d0[0]),
        .I2(vertices_d0[13]),
        .I3(vertices_d0[1]),
        .O(vertices_we0_INST_0_i_8_n_1));
  LUT4 #(
    .INIT(16'hFFFE)) 
    vertices_we0_INST_0_i_9
       (.I0(vertices_d0[14]),
        .I1(vertices_d0[7]),
        .I2(vertices_d0[4]),
        .I3(vertices_d0[2]),
        .O(vertices_we0_INST_0_i_9_n_1));
  LUT2 #(
    .INIT(4'h8)) 
    vertices_we1_INST_0
       (.I0(ap_CS_fsm_state7),
        .I1(icmp_ln108_reg_185),
        .O(vertices_we1));
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
