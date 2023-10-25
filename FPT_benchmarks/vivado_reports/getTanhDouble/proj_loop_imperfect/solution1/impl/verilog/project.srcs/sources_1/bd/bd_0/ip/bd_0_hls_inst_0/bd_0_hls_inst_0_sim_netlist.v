// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.2 (lin64) Build 2708876 Wed Nov  6 21:39:14 MST 2019
// Date        : Sun Jun 25 13:17:05 2023
// Host        : dynamatic-VirtualBox running 64-bit Ubuntu 18.04.3 LTS
// Command     : write_verilog -force -mode funcsim
//               /home/dynamatic/getTanhDouble/proj_loop_imperfect/solution1/impl/verilog/project.srcs/sources_1/bd/bd_0/ip/bd_0_hls_inst_0/bd_0_hls_inst_0_sim_netlist.v
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
    addr_ce0,
    ap_clk,
    ap_rst,
    ap_start,
    ap_done,
    ap_idle,
    ap_ready,
    A_address0,
    A_d0,
    A_q0,
    addr_address0,
    addr_q0);
  output A_ce0;
  output A_we0;
  output addr_ce0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 ap_clk CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ap_clk, ASSOCIATED_RESET ap_rst, FREQ_HZ 250000000.0, PHASE 0.000, CLK_DOMAIN bd_0_ap_clk_0, INSERT_VIP 0" *) input ap_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 ap_rst RST" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ap_rst, POLARITY ACTIVE_HIGH, INSERT_VIP 0" *) input ap_rst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl start" *) input ap_start;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl done" *) output ap_done;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl idle" *) output ap_idle;
  (* X_INTERFACE_INFO = "xilinx.com:interface:acc_handshake:1.0 ap_ctrl ready" *) output ap_ready;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 A_address0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME A_address0, LAYERED_METADATA undef" *) output [9:0]A_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 A_d0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME A_d0, LAYERED_METADATA undef" *) output [31:0]A_d0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 A_q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME A_q0, LAYERED_METADATA undef" *) input [31:0]A_q0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 addr_address0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME addr_address0, LAYERED_METADATA undef" *) output [9:0]addr_address0;
  (* X_INTERFACE_INFO = "xilinx.com:signal:data:1.0 addr_q0 DATA" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME addr_q0, LAYERED_METADATA undef" *) input [31:0]addr_q0;

  wire [9:0]A_address0;
  wire A_ce0;
  wire [31:0]A_d0;
  wire [31:0]A_q0;
  wire A_we0;
  wire [9:0]addr_address0;
  wire addr_ce0;
  wire [31:0]addr_q0;
  wire ap_clk;
  wire ap_done;
  wire ap_idle;
  wire ap_ready;
  wire ap_rst;
  wire ap_start;

  (* ap_ST_fsm_pp0_stage0 = "16'b0000000000000010" *) 
  (* ap_ST_fsm_pp0_stage1 = "16'b0000000000000100" *) 
  (* ap_ST_fsm_pp0_stage10 = "16'b0000100000000000" *) 
  (* ap_ST_fsm_pp0_stage11 = "16'b0001000000000000" *) 
  (* ap_ST_fsm_pp0_stage12 = "16'b0010000000000000" *) 
  (* ap_ST_fsm_pp0_stage13 = "16'b0100000000000000" *) 
  (* ap_ST_fsm_pp0_stage2 = "16'b0000000000001000" *) 
  (* ap_ST_fsm_pp0_stage3 = "16'b0000000000010000" *) 
  (* ap_ST_fsm_pp0_stage4 = "16'b0000000000100000" *) 
  (* ap_ST_fsm_pp0_stage5 = "16'b0000000001000000" *) 
  (* ap_ST_fsm_pp0_stage6 = "16'b0000000010000000" *) 
  (* ap_ST_fsm_pp0_stage7 = "16'b0000000100000000" *) 
  (* ap_ST_fsm_pp0_stage8 = "16'b0000001000000000" *) 
  (* ap_ST_fsm_pp0_stage9 = "16'b0000010000000000" *) 
  (* ap_ST_fsm_state1 = "16'b0000000000000001" *) 
  (* ap_ST_fsm_state17 = "16'b1000000000000000" *) 
  bd_0_hls_inst_0_loop_imperfect inst
       (.A_address0(A_address0),
        .A_ce0(A_ce0),
        .A_d0(A_d0),
        .A_q0(A_q0),
        .A_we0(A_we0),
        .addr_address0(addr_address0),
        .addr_ce0(addr_ce0),
        .addr_q0(addr_q0),
        .ap_clk(ap_clk),
        .ap_done(ap_done),
        .ap_idle(ap_idle),
        .ap_ready(ap_ready),
        .ap_rst(ap_rst),
        .ap_start(ap_start));
endmodule

(* ORIG_REF_NAME = "loop_imperfect" *) (* ap_ST_fsm_pp0_stage0 = "16'b0000000000000010" *) (* ap_ST_fsm_pp0_stage1 = "16'b0000000000000100" *) 
(* ap_ST_fsm_pp0_stage10 = "16'b0000100000000000" *) (* ap_ST_fsm_pp0_stage11 = "16'b0001000000000000" *) (* ap_ST_fsm_pp0_stage12 = "16'b0010000000000000" *) 
(* ap_ST_fsm_pp0_stage13 = "16'b0100000000000000" *) (* ap_ST_fsm_pp0_stage2 = "16'b0000000000001000" *) (* ap_ST_fsm_pp0_stage3 = "16'b0000000000010000" *) 
(* ap_ST_fsm_pp0_stage4 = "16'b0000000000100000" *) (* ap_ST_fsm_pp0_stage5 = "16'b0000000001000000" *) (* ap_ST_fsm_pp0_stage6 = "16'b0000000010000000" *) 
(* ap_ST_fsm_pp0_stage7 = "16'b0000000100000000" *) (* ap_ST_fsm_pp0_stage8 = "16'b0000001000000000" *) (* ap_ST_fsm_pp0_stage9 = "16'b0000010000000000" *) 
(* ap_ST_fsm_state1 = "16'b0000000000000001" *) (* ap_ST_fsm_state17 = "16'b1000000000000000" *) (* hls_module = "yes" *) 
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
    addr_address0,
    addr_ce0,
    addr_q0);
  input ap_clk;
  input ap_rst;
  input ap_start;
  output ap_done;
  output ap_idle;
  output ap_ready;
  output [9:0]A_address0;
  output A_ce0;
  output A_we0;
  output [31:0]A_d0;
  input [31:0]A_q0;
  output [9:0]addr_address0;
  output addr_ce0;
  input [31:0]addr_q0;

  wire [9:0]A_addr_reg_145;
  wire A_addr_reg_1450;
  wire [9:0]A_address0;
  wire A_ce0;
  wire [31:0]A_d0;
  wire [31:0]A_q0;
  wire A_we0;
  wire [9:0]addr_address0;
  wire addr_ce0;
  wire [31:0]addr_q0;
  wire \ap_CS_fsm[15]_i_2_n_1 ;
  wire \ap_CS_fsm[15]_i_3_n_1 ;
  wire \ap_CS_fsm[15]_i_4_n_1 ;
  wire \ap_CS_fsm[15]_i_5_n_1 ;
  wire \ap_CS_fsm[15]_i_6_n_1 ;
  wire \ap_CS_fsm[2]_i_2_n_1 ;
  wire \ap_CS_fsm[2]_i_3_n_1 ;
  wire \ap_CS_fsm[2]_i_4_n_1 ;
  wire ap_CS_fsm_pp0_stage0;
  wire ap_CS_fsm_pp0_stage1;
  wire ap_CS_fsm_pp0_stage13;
  wire ap_CS_fsm_pp0_stage2;
  wire \ap_CS_fsm_reg_n_1_[0] ;
  wire \ap_CS_fsm_reg_n_1_[10] ;
  wire \ap_CS_fsm_reg_n_1_[11] ;
  wire \ap_CS_fsm_reg_n_1_[12] ;
  wire \ap_CS_fsm_reg_n_1_[13] ;
  wire \ap_CS_fsm_reg_n_1_[4] ;
  wire \ap_CS_fsm_reg_n_1_[5] ;
  wire \ap_CS_fsm_reg_n_1_[6] ;
  wire \ap_CS_fsm_reg_n_1_[7] ;
  wire \ap_CS_fsm_reg_n_1_[8] ;
  wire \ap_CS_fsm_reg_n_1_[9] ;
  wire [15:0]ap_NS_fsm;
  wire ap_NS_fsm12_out;
  wire ap_clk;
  wire ap_done;
  wire ap_enable_reg_pp0_iter0;
  wire ap_enable_reg_pp0_iter0_i_1_n_1;
  wire ap_enable_reg_pp0_iter1_i_1_n_1;
  wire ap_enable_reg_pp0_iter1_reg_n_1;
  wire ap_idle;
  wire ap_rst;
  wire ap_start;
  wire [31:17]beta_reg_150;
  wire beta_reg_1500;
  wire [31:0]grp_fu_113_p1;
  wire [31:0]grp_fu_125_p1;
  wire i_0_reg_68;
  wire \i_0_reg_68_reg_n_1_[0] ;
  wire \i_0_reg_68_reg_n_1_[1] ;
  wire \i_0_reg_68_reg_n_1_[2] ;
  wire \i_0_reg_68_reg_n_1_[3] ;
  wire \i_0_reg_68_reg_n_1_[4] ;
  wire \i_0_reg_68_reg_n_1_[5] ;
  wire \i_0_reg_68_reg_n_1_[6] ;
  wire \i_0_reg_68_reg_n_1_[7] ;
  wire \i_0_reg_68_reg_n_1_[8] ;
  wire \i_0_reg_68_reg_n_1_[9] ;
  wire [7:0]i_fu_85_p2;
  wire \i_reg_135[4]_i_2_n_1 ;
  wire \i_reg_135[5]_i_2_n_1 ;
  wire \i_reg_135[6]_i_1_n_1 ;
  wire \i_reg_135[7]_i_2_n_1 ;
  wire \i_reg_135[8]_i_1_n_1 ;
  wire \i_reg_135[9]_i_1_n_1 ;
  wire \i_reg_135[9]_i_2_n_1 ;
  wire \i_reg_135[9]_i_3_n_1 ;
  wire [9:0]i_reg_135_reg;
  wire \icmp_ln98_reg_131[0]_i_1_n_1 ;
  wire \icmp_ln98_reg_131_reg_n_1_[0] ;
  wire [31:17]\loop_imperfect_mubkb_MulnS_0_U/buff2_reg__0 ;
  wire loop_imperfect_mubkb_U1_n_16;
  wire loop_imperfect_mubkb_U1_n_17;
  wire loop_imperfect_mubkb_U1_n_18;
  wire loop_imperfect_mubkb_U1_n_19;
  wire loop_imperfect_mubkb_U1_n_20;
  wire loop_imperfect_mubkb_U1_n_21;
  wire loop_imperfect_mubkb_U1_n_22;
  wire loop_imperfect_mubkb_U1_n_23;
  wire loop_imperfect_mubkb_U1_n_24;
  wire loop_imperfect_mubkb_U1_n_25;
  wire loop_imperfect_mubkb_U1_n_26;
  wire loop_imperfect_mubkb_U1_n_27;
  wire loop_imperfect_mubkb_U1_n_28;
  wire loop_imperfect_mubkb_U1_n_29;
  wire loop_imperfect_mubkb_U1_n_30;
  wire loop_imperfect_mubkb_U1_n_31;
  wire loop_imperfect_mubkb_U1_n_32;

  assign ap_ready = ap_done;
  LUT2 #(
    .INIT(4'h2)) 
    \A_addr_reg_145[9]_i_1 
       (.I0(ap_CS_fsm_pp0_stage1),
        .I1(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .O(A_addr_reg_1450));
  FDRE \A_addr_reg_145_reg[0] 
       (.C(ap_clk),
        .CE(A_addr_reg_1450),
        .D(addr_q0[0]),
        .Q(A_addr_reg_145[0]),
        .R(1'b0));
  FDRE \A_addr_reg_145_reg[1] 
       (.C(ap_clk),
        .CE(A_addr_reg_1450),
        .D(addr_q0[1]),
        .Q(A_addr_reg_145[1]),
        .R(1'b0));
  FDRE \A_addr_reg_145_reg[2] 
       (.C(ap_clk),
        .CE(A_addr_reg_1450),
        .D(addr_q0[2]),
        .Q(A_addr_reg_145[2]),
        .R(1'b0));
  FDRE \A_addr_reg_145_reg[3] 
       (.C(ap_clk),
        .CE(A_addr_reg_1450),
        .D(addr_q0[3]),
        .Q(A_addr_reg_145[3]),
        .R(1'b0));
  FDRE \A_addr_reg_145_reg[4] 
       (.C(ap_clk),
        .CE(A_addr_reg_1450),
        .D(addr_q0[4]),
        .Q(A_addr_reg_145[4]),
        .R(1'b0));
  FDRE \A_addr_reg_145_reg[5] 
       (.C(ap_clk),
        .CE(A_addr_reg_1450),
        .D(addr_q0[5]),
        .Q(A_addr_reg_145[5]),
        .R(1'b0));
  FDRE \A_addr_reg_145_reg[6] 
       (.C(ap_clk),
        .CE(A_addr_reg_1450),
        .D(addr_q0[6]),
        .Q(A_addr_reg_145[6]),
        .R(1'b0));
  FDRE \A_addr_reg_145_reg[7] 
       (.C(ap_clk),
        .CE(A_addr_reg_1450),
        .D(addr_q0[7]),
        .Q(A_addr_reg_145[7]),
        .R(1'b0));
  FDRE \A_addr_reg_145_reg[8] 
       (.C(ap_clk),
        .CE(A_addr_reg_1450),
        .D(addr_q0[8]),
        .Q(A_addr_reg_145[8]),
        .R(1'b0));
  FDRE \A_addr_reg_145_reg[9] 
       (.C(ap_clk),
        .CE(A_addr_reg_1450),
        .D(addr_q0[9]),
        .Q(A_addr_reg_145[9]),
        .R(1'b0));
  LUT4 #(
    .INIT(16'hEA2A)) 
    \A_address0[0]_INST_0 
       (.I0(addr_q0[0]),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(A_addr_reg_145[0]),
        .O(A_address0[0]));
  LUT4 #(
    .INIT(16'hEA2A)) 
    \A_address0[1]_INST_0 
       (.I0(addr_q0[1]),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(A_addr_reg_145[1]),
        .O(A_address0[1]));
  LUT4 #(
    .INIT(16'hEA2A)) 
    \A_address0[2]_INST_0 
       (.I0(addr_q0[2]),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(A_addr_reg_145[2]),
        .O(A_address0[2]));
  LUT4 #(
    .INIT(16'hEA2A)) 
    \A_address0[3]_INST_0 
       (.I0(addr_q0[3]),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(A_addr_reg_145[3]),
        .O(A_address0[3]));
  LUT4 #(
    .INIT(16'hEA2A)) 
    \A_address0[4]_INST_0 
       (.I0(addr_q0[4]),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(A_addr_reg_145[4]),
        .O(A_address0[4]));
  LUT4 #(
    .INIT(16'hEA2A)) 
    \A_address0[5]_INST_0 
       (.I0(addr_q0[5]),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(A_addr_reg_145[5]),
        .O(A_address0[5]));
  LUT4 #(
    .INIT(16'hEA2A)) 
    \A_address0[6]_INST_0 
       (.I0(addr_q0[6]),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(A_addr_reg_145[6]),
        .O(A_address0[6]));
  LUT4 #(
    .INIT(16'hEA2A)) 
    \A_address0[7]_INST_0 
       (.I0(addr_q0[7]),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(A_addr_reg_145[7]),
        .O(A_address0[7]));
  LUT4 #(
    .INIT(16'hEA2A)) 
    \A_address0[8]_INST_0 
       (.I0(addr_q0[8]),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(A_addr_reg_145[8]),
        .O(A_address0[8]));
  LUT4 #(
    .INIT(16'hEA2A)) 
    \A_address0[9]_INST_0 
       (.I0(addr_q0[9]),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(A_addr_reg_145[9]),
        .O(A_address0[9]));
  LUT4 #(
    .INIT(16'hF888)) 
    A_ce0_INST_0
       (.I0(ap_enable_reg_pp0_iter1_reg_n_1),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_CS_fsm_pp0_stage1),
        .I3(ap_enable_reg_pp0_iter0),
        .O(A_ce0));
  LUT3 #(
    .INIT(8'h40)) 
    A_we0_INST_0
       (.I0(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I1(ap_enable_reg_pp0_iter1_reg_n_1),
        .I2(ap_CS_fsm_pp0_stage0),
        .O(A_we0));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT5 #(
    .INIT(32'hAAEAAA2A)) 
    \addr_address0[0]_INST_0 
       (.I0(\i_0_reg_68_reg_n_1_[0] ),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I4(i_reg_135_reg[0]),
        .O(addr_address0[0]));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT5 #(
    .INIT(32'hAAEAAA2A)) 
    \addr_address0[1]_INST_0 
       (.I0(\i_0_reg_68_reg_n_1_[1] ),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I4(i_reg_135_reg[1]),
        .O(addr_address0[1]));
  LUT5 #(
    .INIT(32'hEFFF2000)) 
    \addr_address0[2]_INST_0 
       (.I0(i_reg_135_reg[2]),
        .I1(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(ap_CS_fsm_pp0_stage0),
        .I4(\i_0_reg_68_reg_n_1_[2] ),
        .O(addr_address0[2]));
  LUT5 #(
    .INIT(32'hEFFF2000)) 
    \addr_address0[3]_INST_0 
       (.I0(i_reg_135_reg[3]),
        .I1(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(ap_CS_fsm_pp0_stage0),
        .I4(\i_0_reg_68_reg_n_1_[3] ),
        .O(addr_address0[3]));
  LUT5 #(
    .INIT(32'hEFFF2000)) 
    \addr_address0[4]_INST_0 
       (.I0(i_reg_135_reg[4]),
        .I1(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(ap_CS_fsm_pp0_stage0),
        .I4(\i_0_reg_68_reg_n_1_[4] ),
        .O(addr_address0[4]));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT5 #(
    .INIT(32'hAAEAAA2A)) 
    \addr_address0[5]_INST_0 
       (.I0(\i_0_reg_68_reg_n_1_[5] ),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I4(i_reg_135_reg[5]),
        .O(addr_address0[5]));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT5 #(
    .INIT(32'hAAEAAA2A)) 
    \addr_address0[6]_INST_0 
       (.I0(\i_0_reg_68_reg_n_1_[6] ),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I4(i_reg_135_reg[6]),
        .O(addr_address0[6]));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT5 #(
    .INIT(32'hAAEAAA2A)) 
    \addr_address0[7]_INST_0 
       (.I0(\i_0_reg_68_reg_n_1_[7] ),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I4(i_reg_135_reg[7]),
        .O(addr_address0[7]));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT5 #(
    .INIT(32'hAAEAAA2A)) 
    \addr_address0[8]_INST_0 
       (.I0(\i_0_reg_68_reg_n_1_[8] ),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I4(i_reg_135_reg[8]),
        .O(addr_address0[8]));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'hAAEAAA2A)) 
    \addr_address0[9]_INST_0 
       (.I0(\i_0_reg_68_reg_n_1_[9] ),
        .I1(ap_CS_fsm_pp0_stage0),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I4(i_reg_135_reg[9]),
        .O(addr_address0[9]));
  LUT2 #(
    .INIT(4'h8)) 
    addr_ce0_INST_0
       (.I0(ap_CS_fsm_pp0_stage0),
        .I1(ap_enable_reg_pp0_iter0),
        .O(addr_ce0));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT3 #(
    .INIT(8'hBA)) 
    \ap_CS_fsm[0]_i_1 
       (.I0(ap_done),
        .I1(ap_start),
        .I2(\ap_CS_fsm_reg_n_1_[0] ),
        .O(ap_NS_fsm[0]));
  LUT6 #(
    .INIT(64'h0000001000000000)) 
    \ap_CS_fsm[15]_i_1 
       (.I0(\ap_CS_fsm[15]_i_2_n_1 ),
        .I1(addr_address0[4]),
        .I2(addr_address0[3]),
        .I3(\ap_CS_fsm[15]_i_3_n_1 ),
        .I4(\ap_CS_fsm[15]_i_4_n_1 ),
        .I5(addr_ce0),
        .O(ap_NS_fsm[15]));
  LUT6 #(
    .INIT(64'hFFFEFFFFFFFFFFFF)) 
    \ap_CS_fsm[15]_i_2 
       (.I0(\i_reg_135[7]_i_2_n_1 ),
        .I1(\ap_CS_fsm[15]_i_5_n_1 ),
        .I2(\i_reg_135[9]_i_2_n_1 ),
        .I3(addr_address0[2]),
        .I4(\ap_CS_fsm[15]_i_6_n_1 ),
        .I5(i_fu_85_p2[0]),
        .O(\ap_CS_fsm[15]_i_2_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair6" *) 
  LUT5 #(
    .INIT(32'h1000DFFF)) 
    \ap_CS_fsm[15]_i_3 
       (.I0(i_reg_135_reg[7]),
        .I1(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(ap_CS_fsm_pp0_stage0),
        .I4(\i_0_reg_68_reg_n_1_[7] ),
        .O(\ap_CS_fsm[15]_i_3_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair5" *) 
  LUT5 #(
    .INIT(32'h1000DFFF)) 
    \ap_CS_fsm[15]_i_4 
       (.I0(i_reg_135_reg[8]),
        .I1(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(ap_CS_fsm_pp0_stage0),
        .I4(\i_0_reg_68_reg_n_1_[8] ),
        .O(\ap_CS_fsm[15]_i_4_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair4" *) 
  LUT5 #(
    .INIT(32'h1000DFFF)) 
    \ap_CS_fsm[15]_i_5 
       (.I0(i_reg_135_reg[6]),
        .I1(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(ap_CS_fsm_pp0_stage0),
        .I4(\i_0_reg_68_reg_n_1_[6] ),
        .O(\ap_CS_fsm[15]_i_5_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair2" *) 
  LUT5 #(
    .INIT(32'h1000DFFF)) 
    \ap_CS_fsm[15]_i_6 
       (.I0(i_reg_135_reg[1]),
        .I1(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(ap_CS_fsm_pp0_stage0),
        .I4(\i_0_reg_68_reg_n_1_[1] ),
        .O(\ap_CS_fsm[15]_i_6_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT3 #(
    .INIT(8'hEA)) 
    \ap_CS_fsm[1]_i_1 
       (.I0(ap_CS_fsm_pp0_stage13),
        .I1(ap_start),
        .I2(\ap_CS_fsm_reg_n_1_[0] ),
        .O(ap_NS_fsm[1]));
  LUT5 #(
    .INIT(32'hAAA2AAAA)) 
    \ap_CS_fsm[2]_i_1 
       (.I0(ap_CS_fsm_pp0_stage0),
        .I1(\ap_CS_fsm[2]_i_2_n_1 ),
        .I2(\ap_CS_fsm[2]_i_3_n_1 ),
        .I3(\ap_CS_fsm[2]_i_4_n_1 ),
        .I4(ap_enable_reg_pp0_iter0),
        .O(ap_NS_fsm[2]));
  LUT6 #(
    .INIT(64'h0000000000004540)) 
    \ap_CS_fsm[2]_i_2 
       (.I0(addr_address0[4]),
        .I1(i_reg_135_reg[3]),
        .I2(A_we0),
        .I3(\i_0_reg_68_reg_n_1_[3] ),
        .I4(\ap_CS_fsm[15]_i_3_n_1 ),
        .I5(\ap_CS_fsm[15]_i_4_n_1 ),
        .O(\ap_CS_fsm[2]_i_2_n_1 ));
  LUT6 #(
    .INIT(64'hF7FFF7F7F7FFFFFF)) 
    \ap_CS_fsm[2]_i_3 
       (.I0(i_fu_85_p2[0]),
        .I1(\ap_CS_fsm[15]_i_6_n_1 ),
        .I2(addr_address0[2]),
        .I3(i_reg_135_reg[9]),
        .I4(A_we0),
        .I5(\i_0_reg_68_reg_n_1_[9] ),
        .O(\ap_CS_fsm[2]_i_3_n_1 ));
  LUT5 #(
    .INIT(32'h335FFF5F)) 
    \ap_CS_fsm[2]_i_4 
       (.I0(\i_0_reg_68_reg_n_1_[6] ),
        .I1(i_reg_135_reg[6]),
        .I2(\i_0_reg_68_reg_n_1_[5] ),
        .I3(A_we0),
        .I4(i_reg_135_reg[5]),
        .O(\ap_CS_fsm[2]_i_4_n_1 ));
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
        .Q(ap_CS_fsm_pp0_stage13),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[15] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_NS_fsm[15]),
        .Q(ap_done),
        .R(ap_rst));
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
        .D(ap_CS_fsm_pp0_stage1),
        .Q(ap_CS_fsm_pp0_stage2),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[4] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_CS_fsm_pp0_stage2),
        .Q(\ap_CS_fsm_reg_n_1_[4] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[5] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[4] ),
        .Q(\ap_CS_fsm_reg_n_1_[5] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[6] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[5] ),
        .Q(\ap_CS_fsm_reg_n_1_[6] ),
        .R(ap_rst));
  (* FSM_ENCODING = "none" *) 
  FDRE #(
    .INIT(1'b0)) 
    \ap_CS_fsm_reg[7] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\ap_CS_fsm_reg_n_1_[6] ),
        .Q(\ap_CS_fsm_reg_n_1_[7] ),
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
  LUT6 #(
    .INIT(64'h00000000DFDFDF00)) 
    ap_enable_reg_pp0_iter0_i_1
       (.I0(\ap_CS_fsm[2]_i_2_n_1 ),
        .I1(\ap_CS_fsm[15]_i_2_n_1 ),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(ap_NS_fsm12_out),
        .I4(ap_enable_reg_pp0_iter0),
        .I5(ap_rst),
        .O(ap_enable_reg_pp0_iter0_i_1_n_1));
  (* SOFT_HLUTNM = "soft_lutpair7" *) 
  LUT2 #(
    .INIT(4'h8)) 
    ap_enable_reg_pp0_iter0_i_2
       (.I0(\ap_CS_fsm_reg_n_1_[0] ),
        .I1(ap_start),
        .O(ap_NS_fsm12_out));
  FDRE #(
    .INIT(1'b0)) 
    ap_enable_reg_pp0_iter0_reg
       (.C(ap_clk),
        .CE(1'b1),
        .D(ap_enable_reg_pp0_iter0_i_1_n_1),
        .Q(ap_enable_reg_pp0_iter0),
        .R(1'b0));
  LUT6 #(
    .INIT(64'h00000000CCC5CCC0)) 
    ap_enable_reg_pp0_iter1_i_1
       (.I0(ap_NS_fsm12_out),
        .I1(ap_enable_reg_pp0_iter0),
        .I2(ap_CS_fsm_pp0_stage13),
        .I3(ap_CS_fsm_pp0_stage0),
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
  (* SOFT_HLUTNM = "soft_lutpair8" *) 
  LUT2 #(
    .INIT(4'h2)) 
    ap_idle_INST_0
       (.I0(\ap_CS_fsm_reg_n_1_[0] ),
        .I1(ap_start),
        .O(ap_idle));
  FDRE \beta_reg_150_reg[17] 
       (.C(ap_clk),
        .CE(beta_reg_1500),
        .D(A_q0[17]),
        .Q(beta_reg_150[17]),
        .R(1'b0));
  FDRE \beta_reg_150_reg[18] 
       (.C(ap_clk),
        .CE(beta_reg_1500),
        .D(A_q0[18]),
        .Q(beta_reg_150[18]),
        .R(1'b0));
  FDRE \beta_reg_150_reg[19] 
       (.C(ap_clk),
        .CE(beta_reg_1500),
        .D(A_q0[19]),
        .Q(beta_reg_150[19]),
        .R(1'b0));
  FDRE \beta_reg_150_reg[20] 
       (.C(ap_clk),
        .CE(beta_reg_1500),
        .D(A_q0[20]),
        .Q(beta_reg_150[20]),
        .R(1'b0));
  FDRE \beta_reg_150_reg[21] 
       (.C(ap_clk),
        .CE(beta_reg_1500),
        .D(A_q0[21]),
        .Q(beta_reg_150[21]),
        .R(1'b0));
  FDRE \beta_reg_150_reg[22] 
       (.C(ap_clk),
        .CE(beta_reg_1500),
        .D(A_q0[22]),
        .Q(beta_reg_150[22]),
        .R(1'b0));
  FDRE \beta_reg_150_reg[23] 
       (.C(ap_clk),
        .CE(beta_reg_1500),
        .D(A_q0[23]),
        .Q(beta_reg_150[23]),
        .R(1'b0));
  FDRE \beta_reg_150_reg[24] 
       (.C(ap_clk),
        .CE(beta_reg_1500),
        .D(A_q0[24]),
        .Q(beta_reg_150[24]),
        .R(1'b0));
  FDRE \beta_reg_150_reg[25] 
       (.C(ap_clk),
        .CE(beta_reg_1500),
        .D(A_q0[25]),
        .Q(beta_reg_150[25]),
        .R(1'b0));
  FDRE \beta_reg_150_reg[26] 
       (.C(ap_clk),
        .CE(beta_reg_1500),
        .D(A_q0[26]),
        .Q(beta_reg_150[26]),
        .R(1'b0));
  FDRE \beta_reg_150_reg[27] 
       (.C(ap_clk),
        .CE(beta_reg_1500),
        .D(A_q0[27]),
        .Q(beta_reg_150[27]),
        .R(1'b0));
  FDRE \beta_reg_150_reg[28] 
       (.C(ap_clk),
        .CE(beta_reg_1500),
        .D(A_q0[28]),
        .Q(beta_reg_150[28]),
        .R(1'b0));
  FDRE \beta_reg_150_reg[29] 
       (.C(ap_clk),
        .CE(beta_reg_1500),
        .D(A_q0[29]),
        .Q(beta_reg_150[29]),
        .R(1'b0));
  FDRE \beta_reg_150_reg[30] 
       (.C(ap_clk),
        .CE(beta_reg_1500),
        .D(A_q0[30]),
        .Q(beta_reg_150[30]),
        .R(1'b0));
  FDRE \beta_reg_150_reg[31] 
       (.C(ap_clk),
        .CE(beta_reg_1500),
        .D(A_q0[31]),
        .Q(beta_reg_150[31]),
        .R(1'b0));
  LUT5 #(
    .INIT(32'h88880888)) 
    \i_0_reg_68[9]_i_1 
       (.I0(ap_start),
        .I1(\ap_CS_fsm_reg_n_1_[0] ),
        .I2(ap_CS_fsm_pp0_stage0),
        .I3(ap_enable_reg_pp0_iter1_reg_n_1),
        .I4(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .O(i_0_reg_68));
  FDRE \i_0_reg_68_reg[0] 
       (.C(ap_clk),
        .CE(A_we0),
        .D(i_reg_135_reg[0]),
        .Q(\i_0_reg_68_reg_n_1_[0] ),
        .R(i_0_reg_68));
  FDRE \i_0_reg_68_reg[1] 
       (.C(ap_clk),
        .CE(A_we0),
        .D(i_reg_135_reg[1]),
        .Q(\i_0_reg_68_reg_n_1_[1] ),
        .R(i_0_reg_68));
  FDRE \i_0_reg_68_reg[2] 
       (.C(ap_clk),
        .CE(A_we0),
        .D(i_reg_135_reg[2]),
        .Q(\i_0_reg_68_reg_n_1_[2] ),
        .R(i_0_reg_68));
  FDRE \i_0_reg_68_reg[3] 
       (.C(ap_clk),
        .CE(A_we0),
        .D(i_reg_135_reg[3]),
        .Q(\i_0_reg_68_reg_n_1_[3] ),
        .R(i_0_reg_68));
  FDRE \i_0_reg_68_reg[4] 
       (.C(ap_clk),
        .CE(A_we0),
        .D(i_reg_135_reg[4]),
        .Q(\i_0_reg_68_reg_n_1_[4] ),
        .R(i_0_reg_68));
  FDRE \i_0_reg_68_reg[5] 
       (.C(ap_clk),
        .CE(A_we0),
        .D(i_reg_135_reg[5]),
        .Q(\i_0_reg_68_reg_n_1_[5] ),
        .R(i_0_reg_68));
  FDRE \i_0_reg_68_reg[6] 
       (.C(ap_clk),
        .CE(A_we0),
        .D(i_reg_135_reg[6]),
        .Q(\i_0_reg_68_reg_n_1_[6] ),
        .R(i_0_reg_68));
  FDRE \i_0_reg_68_reg[7] 
       (.C(ap_clk),
        .CE(A_we0),
        .D(i_reg_135_reg[7]),
        .Q(\i_0_reg_68_reg_n_1_[7] ),
        .R(i_0_reg_68));
  FDRE \i_0_reg_68_reg[8] 
       (.C(ap_clk),
        .CE(A_we0),
        .D(i_reg_135_reg[8]),
        .Q(\i_0_reg_68_reg_n_1_[8] ),
        .R(i_0_reg_68));
  FDRE \i_0_reg_68_reg[9] 
       (.C(ap_clk),
        .CE(A_we0),
        .D(i_reg_135_reg[9]),
        .Q(\i_0_reg_68_reg_n_1_[9] ),
        .R(i_0_reg_68));
  (* SOFT_HLUTNM = "soft_lutpair1" *) 
  LUT5 #(
    .INIT(32'h1000DFFF)) 
    \i_reg_135[0]_i_1 
       (.I0(i_reg_135_reg[0]),
        .I1(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(ap_CS_fsm_pp0_stage0),
        .I4(\i_0_reg_68_reg_n_1_[0] ),
        .O(i_fu_85_p2[0]));
  LUT5 #(
    .INIT(32'h335ACC5A)) 
    \i_reg_135[1]_i_1 
       (.I0(\i_0_reg_68_reg_n_1_[1] ),
        .I1(i_reg_135_reg[1]),
        .I2(\i_0_reg_68_reg_n_1_[0] ),
        .I3(A_we0),
        .I4(i_reg_135_reg[0]),
        .O(i_fu_85_p2[1]));
  LUT6 #(
    .INIT(64'hB8B8B8B847748BB8)) 
    \i_reg_135[2]_i_1 
       (.I0(i_reg_135_reg[2]),
        .I1(A_we0),
        .I2(\i_0_reg_68_reg_n_1_[2] ),
        .I3(\i_0_reg_68_reg_n_1_[1] ),
        .I4(i_reg_135_reg[1]),
        .I5(i_fu_85_p2[0]),
        .O(i_fu_85_p2[2]));
  LUT5 #(
    .INIT(32'hA9AA5955)) 
    \i_reg_135[3]_i_1 
       (.I0(\i_reg_135[4]_i_2_n_1 ),
        .I1(i_reg_135_reg[3]),
        .I2(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I3(ap_enable_reg_pp0_iter1_reg_n_1),
        .I4(\i_0_reg_68_reg_n_1_[3] ),
        .O(i_fu_85_p2[3]));
  LUT6 #(
    .INIT(64'hC3CCA5A5C3CCAAAA)) 
    \i_reg_135[4]_i_1 
       (.I0(\i_0_reg_68_reg_n_1_[4] ),
        .I1(i_reg_135_reg[4]),
        .I2(\i_reg_135[4]_i_2_n_1 ),
        .I3(i_reg_135_reg[3]),
        .I4(A_we0),
        .I5(\i_0_reg_68_reg_n_1_[3] ),
        .O(i_fu_85_p2[4]));
  LUT6 #(
    .INIT(64'hBBBBAFFFFFFFAFFF)) 
    \i_reg_135[4]_i_2 
       (.I0(i_fu_85_p2[0]),
        .I1(i_reg_135_reg[1]),
        .I2(\i_0_reg_68_reg_n_1_[1] ),
        .I3(\i_0_reg_68_reg_n_1_[2] ),
        .I4(A_we0),
        .I5(i_reg_135_reg[2]),
        .O(\i_reg_135[4]_i_2_n_1 ));
  LUT6 #(
    .INIT(64'h3CCC5A5A3CCCAAAA)) 
    \i_reg_135[5]_i_1 
       (.I0(\i_0_reg_68_reg_n_1_[5] ),
        .I1(i_reg_135_reg[5]),
        .I2(\i_reg_135[5]_i_2_n_1 ),
        .I3(i_reg_135_reg[4]),
        .I4(A_we0),
        .I5(\i_0_reg_68_reg_n_1_[4] ),
        .O(i_fu_85_p2[5]));
  LUT6 #(
    .INIT(64'h000000000000E200)) 
    \i_reg_135[5]_i_2 
       (.I0(\i_0_reg_68_reg_n_1_[3] ),
        .I1(A_we0),
        .I2(i_reg_135_reg[3]),
        .I3(addr_address0[2]),
        .I4(\ap_CS_fsm[15]_i_6_n_1 ),
        .I5(i_fu_85_p2[0]),
        .O(\i_reg_135[5]_i_2_n_1 ));
  LUT6 #(
    .INIT(64'hC3CCA5A5C3CCAAAA)) 
    \i_reg_135[6]_i_1 
       (.I0(\i_0_reg_68_reg_n_1_[6] ),
        .I1(i_reg_135_reg[6]),
        .I2(\i_reg_135[9]_i_3_n_1 ),
        .I3(i_reg_135_reg[5]),
        .I4(A_we0),
        .I5(\i_0_reg_68_reg_n_1_[5] ),
        .O(\i_reg_135[6]_i_1_n_1 ));
  LUT6 #(
    .INIT(64'h5555555566655565)) 
    \i_reg_135[7]_i_1 
       (.I0(\ap_CS_fsm[15]_i_3_n_1 ),
        .I1(\i_reg_135[9]_i_3_n_1 ),
        .I2(\i_0_reg_68_reg_n_1_[6] ),
        .I3(A_we0),
        .I4(i_reg_135_reg[6]),
        .I5(\i_reg_135[7]_i_2_n_1 ),
        .O(i_fu_85_p2[7]));
  (* SOFT_HLUTNM = "soft_lutpair3" *) 
  LUT5 #(
    .INIT(32'h1000DFFF)) 
    \i_reg_135[7]_i_2 
       (.I0(i_reg_135_reg[5]),
        .I1(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(ap_CS_fsm_pp0_stage0),
        .I4(\i_0_reg_68_reg_n_1_[5] ),
        .O(\i_reg_135[7]_i_2_n_1 ));
  LUT6 #(
    .INIT(64'hE2E2E2E2E2E2E21D)) 
    \i_reg_135[8]_i_1 
       (.I0(\i_0_reg_68_reg_n_1_[8] ),
        .I1(A_we0),
        .I2(i_reg_135_reg[8]),
        .I3(\ap_CS_fsm[2]_i_4_n_1 ),
        .I4(\i_reg_135[9]_i_3_n_1 ),
        .I5(\ap_CS_fsm[15]_i_3_n_1 ),
        .O(\i_reg_135[8]_i_1_n_1 ));
  LUT5 #(
    .INIT(32'h55555556)) 
    \i_reg_135[9]_i_1 
       (.I0(\i_reg_135[9]_i_2_n_1 ),
        .I1(\ap_CS_fsm[15]_i_4_n_1 ),
        .I2(\ap_CS_fsm[15]_i_3_n_1 ),
        .I3(\i_reg_135[9]_i_3_n_1 ),
        .I4(\ap_CS_fsm[2]_i_4_n_1 ),
        .O(\i_reg_135[9]_i_1_n_1 ));
  (* SOFT_HLUTNM = "soft_lutpair0" *) 
  LUT5 #(
    .INIT(32'h1000DFFF)) 
    \i_reg_135[9]_i_2 
       (.I0(i_reg_135_reg[9]),
        .I1(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .I2(ap_enable_reg_pp0_iter1_reg_n_1),
        .I3(ap_CS_fsm_pp0_stage0),
        .I4(\i_0_reg_68_reg_n_1_[9] ),
        .O(\i_reg_135[9]_i_2_n_1 ));
  LUT5 #(
    .INIT(32'hFDFFFFFF)) 
    \i_reg_135[9]_i_3 
       (.I0(addr_address0[4]),
        .I1(i_fu_85_p2[0]),
        .I2(\ap_CS_fsm[15]_i_6_n_1 ),
        .I3(addr_address0[2]),
        .I4(addr_address0[3]),
        .O(\i_reg_135[9]_i_3_n_1 ));
  FDRE \i_reg_135_reg[0] 
       (.C(ap_clk),
        .CE(addr_ce0),
        .D(i_fu_85_p2[0]),
        .Q(i_reg_135_reg[0]),
        .R(1'b0));
  FDRE \i_reg_135_reg[1] 
       (.C(ap_clk),
        .CE(addr_ce0),
        .D(i_fu_85_p2[1]),
        .Q(i_reg_135_reg[1]),
        .R(1'b0));
  FDRE \i_reg_135_reg[2] 
       (.C(ap_clk),
        .CE(addr_ce0),
        .D(i_fu_85_p2[2]),
        .Q(i_reg_135_reg[2]),
        .R(1'b0));
  FDRE \i_reg_135_reg[3] 
       (.C(ap_clk),
        .CE(addr_ce0),
        .D(i_fu_85_p2[3]),
        .Q(i_reg_135_reg[3]),
        .R(1'b0));
  FDRE \i_reg_135_reg[4] 
       (.C(ap_clk),
        .CE(addr_ce0),
        .D(i_fu_85_p2[4]),
        .Q(i_reg_135_reg[4]),
        .R(1'b0));
  FDRE \i_reg_135_reg[5] 
       (.C(ap_clk),
        .CE(addr_ce0),
        .D(i_fu_85_p2[5]),
        .Q(i_reg_135_reg[5]),
        .R(1'b0));
  FDRE \i_reg_135_reg[6] 
       (.C(ap_clk),
        .CE(addr_ce0),
        .D(\i_reg_135[6]_i_1_n_1 ),
        .Q(i_reg_135_reg[6]),
        .R(1'b0));
  FDRE \i_reg_135_reg[7] 
       (.C(ap_clk),
        .CE(addr_ce0),
        .D(i_fu_85_p2[7]),
        .Q(i_reg_135_reg[7]),
        .R(1'b0));
  FDRE \i_reg_135_reg[8] 
       (.C(ap_clk),
        .CE(addr_ce0),
        .D(\i_reg_135[8]_i_1_n_1 ),
        .Q(i_reg_135_reg[8]),
        .R(1'b0));
  FDRE \i_reg_135_reg[9] 
       (.C(ap_clk),
        .CE(addr_ce0),
        .D(\i_reg_135[9]_i_1_n_1 ),
        .Q(i_reg_135_reg[9]),
        .R(1'b0));
  LUT5 #(
    .INIT(32'h02FF0200)) 
    \icmp_ln98_reg_131[0]_i_1 
       (.I0(\ap_CS_fsm[2]_i_2_n_1 ),
        .I1(\ap_CS_fsm[2]_i_3_n_1 ),
        .I2(\ap_CS_fsm[2]_i_4_n_1 ),
        .I3(ap_CS_fsm_pp0_stage0),
        .I4(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .O(\icmp_ln98_reg_131[0]_i_1_n_1 ));
  FDRE \icmp_ln98_reg_131_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\icmp_ln98_reg_131[0]_i_1_n_1 ),
        .Q(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .R(1'b0));
  bd_0_hls_inst_0_loop_imperfect_mubkb loop_imperfect_mubkb_U1
       (.A_q0(A_q0),
        .Q({loop_imperfect_mubkb_U1_n_16,loop_imperfect_mubkb_U1_n_17,loop_imperfect_mubkb_U1_n_18,loop_imperfect_mubkb_U1_n_19,loop_imperfect_mubkb_U1_n_20,loop_imperfect_mubkb_U1_n_21,loop_imperfect_mubkb_U1_n_22,loop_imperfect_mubkb_U1_n_23,loop_imperfect_mubkb_U1_n_24,loop_imperfect_mubkb_U1_n_25,loop_imperfect_mubkb_U1_n_26,loop_imperfect_mubkb_U1_n_27,loop_imperfect_mubkb_U1_n_28,loop_imperfect_mubkb_U1_n_29,loop_imperfect_mubkb_U1_n_30,loop_imperfect_mubkb_U1_n_31,loop_imperfect_mubkb_U1_n_32}),
        .ap_clk(ap_clk),
        .buff2_reg(\loop_imperfect_mubkb_MulnS_0_U/buff2_reg__0 ),
        .grp_fu_113_p1(grp_fu_113_p1));
  bd_0_hls_inst_0_loop_imperfect_mubkb_0 loop_imperfect_mubkb_U2
       (.Q({loop_imperfect_mubkb_U1_n_16,loop_imperfect_mubkb_U1_n_17,loop_imperfect_mubkb_U1_n_18,loop_imperfect_mubkb_U1_n_19,loop_imperfect_mubkb_U1_n_20,loop_imperfect_mubkb_U1_n_21,loop_imperfect_mubkb_U1_n_22,loop_imperfect_mubkb_U1_n_23,loop_imperfect_mubkb_U1_n_24,loop_imperfect_mubkb_U1_n_25,loop_imperfect_mubkb_U1_n_26,loop_imperfect_mubkb_U1_n_27,loop_imperfect_mubkb_U1_n_28,loop_imperfect_mubkb_U1_n_29,loop_imperfect_mubkb_U1_n_30,loop_imperfect_mubkb_U1_n_31,loop_imperfect_mubkb_U1_n_32}),
        .ap_clk(ap_clk),
        .buff2_reg(\loop_imperfect_mubkb_MulnS_0_U/buff2_reg__0 ),
        .grp_fu_113_p1(grp_fu_113_p1),
        .grp_fu_125_p1(grp_fu_125_p1));
  bd_0_hls_inst_0_loop_imperfect_mubkb_1 loop_imperfect_mubkb_U3
       (.A_d0(A_d0),
        .A_q0(A_q0[16:0]),
        .E(beta_reg_1500),
        .Q(beta_reg_150),
        .ap_clk(ap_clk),
        .ap_enable_reg_pp0_iter0(ap_enable_reg_pp0_iter0),
        .buff1_reg(ap_CS_fsm_pp0_stage2),
        .buff1_reg_0(\icmp_ln98_reg_131_reg_n_1_[0] ),
        .grp_fu_125_p1(grp_fu_125_p1));
endmodule

(* ORIG_REF_NAME = "loop_imperfect_mubkb" *) 
module bd_0_hls_inst_0_loop_imperfect_mubkb
   (buff2_reg,
    Q,
    grp_fu_113_p1,
    ap_clk,
    A_q0);
  output [14:0]buff2_reg;
  output [16:0]Q;
  output [31:0]grp_fu_113_p1;
  input ap_clk;
  input [31:0]A_q0;

  wire [31:0]A_q0;
  wire [16:0]Q;
  wire ap_clk;
  wire [14:0]buff2_reg;
  wire [31:0]grp_fu_113_p1;

  bd_0_hls_inst_0_loop_imperfect_mubkb_MulnS_0_3 loop_imperfect_mubkb_MulnS_0_U
       (.A_q0(A_q0),
        .Q(Q),
        .ap_clk(ap_clk),
        .buff2_reg_0(buff2_reg),
        .grp_fu_113_p1(grp_fu_113_p1));
endmodule

(* ORIG_REF_NAME = "loop_imperfect_mubkb" *) 
module bd_0_hls_inst_0_loop_imperfect_mubkb_0
   (grp_fu_125_p1,
    ap_clk,
    grp_fu_113_p1,
    Q,
    buff2_reg);
  output [31:0]grp_fu_125_p1;
  input ap_clk;
  input [31:0]grp_fu_113_p1;
  input [16:0]Q;
  input [14:0]buff2_reg;

  wire [16:0]Q;
  wire ap_clk;
  wire [14:0]buff2_reg;
  wire [31:0]grp_fu_113_p1;
  wire [31:0]grp_fu_125_p1;

  bd_0_hls_inst_0_loop_imperfect_mubkb_MulnS_0_2 loop_imperfect_mubkb_MulnS_0_U
       (.Q(Q),
        .ap_clk(ap_clk),
        .buff2_reg_0(buff2_reg),
        .grp_fu_113_p1(grp_fu_113_p1),
        .grp_fu_125_p1(grp_fu_125_p1));
endmodule

(* ORIG_REF_NAME = "loop_imperfect_mubkb" *) 
module bd_0_hls_inst_0_loop_imperfect_mubkb_1
   (E,
    A_d0,
    ap_clk,
    grp_fu_125_p1,
    A_q0,
    Q,
    buff1_reg,
    ap_enable_reg_pp0_iter0,
    buff1_reg_0);
  output [0:0]E;
  output [31:0]A_d0;
  input ap_clk;
  input [31:0]grp_fu_125_p1;
  input [16:0]A_q0;
  input [14:0]Q;
  input [0:0]buff1_reg;
  input ap_enable_reg_pp0_iter0;
  input buff1_reg_0;

  wire [31:0]A_d0;
  wire [16:0]A_q0;
  wire [0:0]E;
  wire [14:0]Q;
  wire ap_clk;
  wire ap_enable_reg_pp0_iter0;
  wire [0:0]buff1_reg;
  wire buff1_reg_0;
  wire [31:0]grp_fu_125_p1;

  bd_0_hls_inst_0_loop_imperfect_mubkb_MulnS_0 loop_imperfect_mubkb_MulnS_0_U
       (.A_d0(A_d0),
        .A_q0(A_q0),
        .E(E),
        .Q(Q),
        .ap_clk(ap_clk),
        .ap_enable_reg_pp0_iter0(ap_enable_reg_pp0_iter0),
        .buff1_reg_0(buff1_reg),
        .buff1_reg_1(buff1_reg_0),
        .grp_fu_125_p1(grp_fu_125_p1));
endmodule

(* ORIG_REF_NAME = "loop_imperfect_mubkb_MulnS_0" *) 
module bd_0_hls_inst_0_loop_imperfect_mubkb_MulnS_0
   (E,
    A_d0,
    ap_clk,
    grp_fu_125_p1,
    A_q0,
    Q,
    buff1_reg_0,
    ap_enable_reg_pp0_iter0,
    buff1_reg_1);
  output [0:0]E;
  output [31:0]A_d0;
  input ap_clk;
  input [31:0]grp_fu_125_p1;
  input [16:0]A_q0;
  input [14:0]Q;
  input [0:0]buff1_reg_0;
  input ap_enable_reg_pp0_iter0;
  input buff1_reg_1;

  wire [31:0]A_d0;
  wire [16:0]A_q0;
  wire [0:0]E;
  wire [14:0]Q;
  wire ap_clk;
  wire ap_enable_reg_pp0_iter0;
  wire buff0_reg_n_100;
  wire buff0_reg_n_101;
  wire buff0_reg_n_102;
  wire buff0_reg_n_103;
  wire buff0_reg_n_104;
  wire buff0_reg_n_105;
  wire buff0_reg_n_106;
  wire buff0_reg_n_107;
  wire buff0_reg_n_108;
  wire buff0_reg_n_109;
  wire buff0_reg_n_110;
  wire buff0_reg_n_111;
  wire buff0_reg_n_112;
  wire buff0_reg_n_113;
  wire buff0_reg_n_114;
  wire buff0_reg_n_115;
  wire buff0_reg_n_116;
  wire buff0_reg_n_117;
  wire buff0_reg_n_118;
  wire buff0_reg_n_119;
  wire buff0_reg_n_120;
  wire buff0_reg_n_121;
  wire buff0_reg_n_122;
  wire buff0_reg_n_123;
  wire buff0_reg_n_124;
  wire buff0_reg_n_125;
  wire buff0_reg_n_126;
  wire buff0_reg_n_127;
  wire buff0_reg_n_128;
  wire buff0_reg_n_129;
  wire buff0_reg_n_130;
  wire buff0_reg_n_131;
  wire buff0_reg_n_132;
  wire buff0_reg_n_133;
  wire buff0_reg_n_134;
  wire buff0_reg_n_135;
  wire buff0_reg_n_136;
  wire buff0_reg_n_137;
  wire buff0_reg_n_138;
  wire buff0_reg_n_139;
  wire buff0_reg_n_140;
  wire buff0_reg_n_141;
  wire buff0_reg_n_142;
  wire buff0_reg_n_143;
  wire buff0_reg_n_144;
  wire buff0_reg_n_145;
  wire buff0_reg_n_146;
  wire buff0_reg_n_147;
  wire buff0_reg_n_148;
  wire buff0_reg_n_149;
  wire buff0_reg_n_150;
  wire buff0_reg_n_151;
  wire buff0_reg_n_152;
  wire buff0_reg_n_153;
  wire buff0_reg_n_154;
  wire buff0_reg_n_59;
  wire buff0_reg_n_60;
  wire buff0_reg_n_61;
  wire buff0_reg_n_62;
  wire buff0_reg_n_63;
  wire buff0_reg_n_64;
  wire buff0_reg_n_65;
  wire buff0_reg_n_66;
  wire buff0_reg_n_67;
  wire buff0_reg_n_68;
  wire buff0_reg_n_69;
  wire buff0_reg_n_70;
  wire buff0_reg_n_71;
  wire buff0_reg_n_72;
  wire buff0_reg_n_73;
  wire buff0_reg_n_74;
  wire buff0_reg_n_75;
  wire buff0_reg_n_76;
  wire buff0_reg_n_77;
  wire buff0_reg_n_78;
  wire buff0_reg_n_79;
  wire buff0_reg_n_80;
  wire buff0_reg_n_81;
  wire buff0_reg_n_82;
  wire buff0_reg_n_83;
  wire buff0_reg_n_84;
  wire buff0_reg_n_85;
  wire buff0_reg_n_86;
  wire buff0_reg_n_87;
  wire buff0_reg_n_88;
  wire buff0_reg_n_89;
  wire buff0_reg_n_90;
  wire buff0_reg_n_91;
  wire buff0_reg_n_92;
  wire buff0_reg_n_93;
  wire buff0_reg_n_94;
  wire buff0_reg_n_95;
  wire buff0_reg_n_96;
  wire buff0_reg_n_97;
  wire buff0_reg_n_98;
  wire buff0_reg_n_99;
  wire [0:0]buff1_reg_0;
  wire buff1_reg_1;
  wire buff1_reg_n_107;
  wire buff1_reg_n_108;
  wire buff1_reg_n_109;
  wire buff1_reg_n_110;
  wire buff1_reg_n_111;
  wire buff1_reg_n_112;
  wire buff1_reg_n_113;
  wire buff1_reg_n_114;
  wire buff1_reg_n_115;
  wire buff1_reg_n_116;
  wire buff1_reg_n_117;
  wire buff1_reg_n_118;
  wire buff1_reg_n_119;
  wire buff1_reg_n_120;
  wire buff1_reg_n_121;
  wire buff1_reg_n_122;
  wire buff1_reg_n_123;
  wire buff1_reg_n_124;
  wire buff1_reg_n_125;
  wire buff1_reg_n_126;
  wire buff1_reg_n_127;
  wire buff1_reg_n_128;
  wire buff1_reg_n_129;
  wire buff1_reg_n_130;
  wire buff1_reg_n_131;
  wire buff1_reg_n_132;
  wire buff1_reg_n_133;
  wire buff1_reg_n_134;
  wire buff1_reg_n_135;
  wire buff1_reg_n_136;
  wire buff1_reg_n_137;
  wire buff1_reg_n_138;
  wire buff1_reg_n_139;
  wire buff1_reg_n_140;
  wire buff1_reg_n_141;
  wire buff1_reg_n_142;
  wire buff1_reg_n_143;
  wire buff1_reg_n_144;
  wire buff1_reg_n_145;
  wire buff1_reg_n_146;
  wire buff1_reg_n_147;
  wire buff1_reg_n_148;
  wire buff1_reg_n_149;
  wire buff1_reg_n_150;
  wire buff1_reg_n_151;
  wire buff1_reg_n_152;
  wire buff1_reg_n_153;
  wire buff1_reg_n_154;
  wire \buff1_reg_n_1_[0] ;
  wire \buff1_reg_n_1_[10] ;
  wire \buff1_reg_n_1_[11] ;
  wire \buff1_reg_n_1_[12] ;
  wire \buff1_reg_n_1_[13] ;
  wire \buff1_reg_n_1_[14] ;
  wire \buff1_reg_n_1_[15] ;
  wire \buff1_reg_n_1_[16] ;
  wire \buff1_reg_n_1_[1] ;
  wire \buff1_reg_n_1_[2] ;
  wire \buff1_reg_n_1_[3] ;
  wire \buff1_reg_n_1_[4] ;
  wire \buff1_reg_n_1_[5] ;
  wire \buff1_reg_n_1_[6] ;
  wire \buff1_reg_n_1_[7] ;
  wire \buff1_reg_n_1_[8] ;
  wire \buff1_reg_n_1_[9] ;
  wire buff2_reg_n_59;
  wire buff2_reg_n_60;
  wire buff2_reg_n_61;
  wire buff2_reg_n_62;
  wire buff2_reg_n_63;
  wire buff2_reg_n_64;
  wire buff2_reg_n_65;
  wire buff2_reg_n_66;
  wire buff2_reg_n_67;
  wire buff2_reg_n_68;
  wire buff2_reg_n_69;
  wire buff2_reg_n_70;
  wire buff2_reg_n_71;
  wire buff2_reg_n_72;
  wire buff2_reg_n_73;
  wire buff2_reg_n_74;
  wire buff2_reg_n_75;
  wire buff2_reg_n_76;
  wire buff2_reg_n_77;
  wire buff2_reg_n_78;
  wire buff2_reg_n_79;
  wire buff2_reg_n_80;
  wire buff2_reg_n_81;
  wire buff2_reg_n_82;
  wire buff2_reg_n_83;
  wire buff2_reg_n_84;
  wire buff2_reg_n_85;
  wire buff2_reg_n_86;
  wire buff2_reg_n_87;
  wire buff2_reg_n_88;
  wire buff2_reg_n_89;
  wire buff2_reg_n_90;
  wire buff2_reg_n_91;
  wire [31:0]grp_fu_125_p1;
  wire NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED;
  wire NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED;
  wire NLW_buff0_reg_OVERFLOW_UNCONNECTED;
  wire NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED;
  wire NLW_buff0_reg_PATTERNDETECT_UNCONNECTED;
  wire NLW_buff0_reg_UNDERFLOW_UNCONNECTED;
  wire [29:0]NLW_buff0_reg_ACOUT_UNCONNECTED;
  wire [17:0]NLW_buff0_reg_BCOUT_UNCONNECTED;
  wire [3:0]NLW_buff0_reg_CARRYOUT_UNCONNECTED;
  wire NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED;
  wire NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED;
  wire NLW_buff1_reg_OVERFLOW_UNCONNECTED;
  wire NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED;
  wire NLW_buff1_reg_PATTERNDETECT_UNCONNECTED;
  wire NLW_buff1_reg_UNDERFLOW_UNCONNECTED;
  wire [29:0]NLW_buff1_reg_ACOUT_UNCONNECTED;
  wire [17:0]NLW_buff1_reg_BCOUT_UNCONNECTED;
  wire [3:0]NLW_buff1_reg_CARRYOUT_UNCONNECTED;
  wire [47:0]NLW_buff1_reg_P_UNCONNECTED;
  wire NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED;
  wire NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED;
  wire NLW_buff2_reg_OVERFLOW_UNCONNECTED;
  wire NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED;
  wire NLW_buff2_reg_PATTERNDETECT_UNCONNECTED;
  wire NLW_buff2_reg_UNDERFLOW_UNCONNECTED;
  wire [29:0]NLW_buff2_reg_ACOUT_UNCONNECTED;
  wire [17:0]NLW_buff2_reg_BCOUT_UNCONNECTED;
  wire [3:0]NLW_buff2_reg_CARRYOUT_UNCONNECTED;
  wire [47:0]NLW_buff2_reg_PCOUT_UNCONNECTED;

  DSP48E1 #(
    .ACASCREG(2),
    .ADREG(1),
    .ALUMODEREG(0),
    .AREG(2),
    .AUTORESET_PATDET("NO_RESET"),
    .A_INPUT("DIRECT"),
    .BCASCREG(1),
    .BREG(1),
    .B_INPUT("DIRECT"),
    .CARRYINREG(0),
    .CARRYINSELREG(0),
    .CREG(0),
    .DREG(1),
    .INMODEREG(0),
    .MASK(48'h3FFFFFFFFFFF),
    .MREG(0),
    .OPMODEREG(0),
    .PATTERN(48'h000000000000),
    .PREG(1),
    .SEL_MASK("MASK"),
    .SEL_PATTERN("PATTERN"),
    .USE_DPORT("FALSE"),
    .USE_MULT("MULTIPLY"),
    .USE_PATTERN_DETECT("NO_PATDET"),
    .USE_SIMD("ONE48")) 
    buff0_reg
       (.A({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,A_q0}),
        .ACIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .ACOUT(NLW_buff0_reg_ACOUT_UNCONNECTED[29:0]),
        .ALUMODE({1'b0,1'b0,1'b0,1'b0}),
        .B({1'b0,grp_fu_125_p1[16:0]}),
        .BCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .BCOUT(NLW_buff0_reg_BCOUT_UNCONNECTED[17:0]),
        .C({1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1}),
        .CARRYCASCIN(1'b0),
        .CARRYCASCOUT(NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED),
        .CARRYIN(1'b0),
        .CARRYINSEL({1'b0,1'b0,1'b0}),
        .CARRYOUT(NLW_buff0_reg_CARRYOUT_UNCONNECTED[3:0]),
        .CEA1(E),
        .CEA2(1'b1),
        .CEAD(1'b0),
        .CEALUMODE(1'b0),
        .CEB1(1'b0),
        .CEB2(1'b1),
        .CEC(1'b0),
        .CECARRYIN(1'b0),
        .CECTRL(1'b0),
        .CED(1'b0),
        .CEINMODE(1'b0),
        .CEM(1'b0),
        .CEP(1'b1),
        .CLK(ap_clk),
        .D({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .INMODE({1'b0,1'b0,1'b0,1'b0,1'b0}),
        .MULTSIGNIN(1'b0),
        .MULTSIGNOUT(NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED),
        .OPMODE({1'b0,1'b0,1'b0,1'b0,1'b1,1'b0,1'b1}),
        .OVERFLOW(NLW_buff0_reg_OVERFLOW_UNCONNECTED),
        .P({buff0_reg_n_59,buff0_reg_n_60,buff0_reg_n_61,buff0_reg_n_62,buff0_reg_n_63,buff0_reg_n_64,buff0_reg_n_65,buff0_reg_n_66,buff0_reg_n_67,buff0_reg_n_68,buff0_reg_n_69,buff0_reg_n_70,buff0_reg_n_71,buff0_reg_n_72,buff0_reg_n_73,buff0_reg_n_74,buff0_reg_n_75,buff0_reg_n_76,buff0_reg_n_77,buff0_reg_n_78,buff0_reg_n_79,buff0_reg_n_80,buff0_reg_n_81,buff0_reg_n_82,buff0_reg_n_83,buff0_reg_n_84,buff0_reg_n_85,buff0_reg_n_86,buff0_reg_n_87,buff0_reg_n_88,buff0_reg_n_89,buff0_reg_n_90,buff0_reg_n_91,buff0_reg_n_92,buff0_reg_n_93,buff0_reg_n_94,buff0_reg_n_95,buff0_reg_n_96,buff0_reg_n_97,buff0_reg_n_98,buff0_reg_n_99,buff0_reg_n_100,buff0_reg_n_101,buff0_reg_n_102,buff0_reg_n_103,buff0_reg_n_104,buff0_reg_n_105,buff0_reg_n_106}),
        .PATTERNBDETECT(NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED),
        .PATTERNDETECT(NLW_buff0_reg_PATTERNDETECT_UNCONNECTED),
        .PCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .PCOUT({buff0_reg_n_107,buff0_reg_n_108,buff0_reg_n_109,buff0_reg_n_110,buff0_reg_n_111,buff0_reg_n_112,buff0_reg_n_113,buff0_reg_n_114,buff0_reg_n_115,buff0_reg_n_116,buff0_reg_n_117,buff0_reg_n_118,buff0_reg_n_119,buff0_reg_n_120,buff0_reg_n_121,buff0_reg_n_122,buff0_reg_n_123,buff0_reg_n_124,buff0_reg_n_125,buff0_reg_n_126,buff0_reg_n_127,buff0_reg_n_128,buff0_reg_n_129,buff0_reg_n_130,buff0_reg_n_131,buff0_reg_n_132,buff0_reg_n_133,buff0_reg_n_134,buff0_reg_n_135,buff0_reg_n_136,buff0_reg_n_137,buff0_reg_n_138,buff0_reg_n_139,buff0_reg_n_140,buff0_reg_n_141,buff0_reg_n_142,buff0_reg_n_143,buff0_reg_n_144,buff0_reg_n_145,buff0_reg_n_146,buff0_reg_n_147,buff0_reg_n_148,buff0_reg_n_149,buff0_reg_n_150,buff0_reg_n_151,buff0_reg_n_152,buff0_reg_n_153,buff0_reg_n_154}),
        .RSTA(1'b0),
        .RSTALLCARRYIN(1'b0),
        .RSTALUMODE(1'b0),
        .RSTB(1'b0),
        .RSTC(1'b0),
        .RSTCTRL(1'b0),
        .RSTD(1'b0),
        .RSTINMODE(1'b0),
        .RSTM(1'b0),
        .RSTP(1'b0),
        .UNDERFLOW(NLW_buff0_reg_UNDERFLOW_UNCONNECTED));
  LUT3 #(
    .INIT(8'h08)) 
    buff0_reg_i_1__0
       (.I0(buff1_reg_0),
        .I1(ap_enable_reg_pp0_iter0),
        .I2(buff1_reg_1),
        .O(E));
  DSP48E1 #(
    .ACASCREG(2),
    .ADREG(1),
    .ALUMODEREG(0),
    .AREG(2),
    .AUTORESET_PATDET("NO_RESET"),
    .A_INPUT("DIRECT"),
    .BCASCREG(1),
    .BREG(1),
    .B_INPUT("DIRECT"),
    .CARRYINREG(0),
    .CARRYINSELREG(0),
    .CREG(1),
    .DREG(1),
    .INMODEREG(0),
    .MASK(48'h3FFFFFFFFFFF),
    .MREG(1),
    .OPMODEREG(0),
    .PATTERN(48'h000000000000),
    .PREG(1),
    .SEL_MASK("MASK"),
    .SEL_PATTERN("PATTERN"),
    .USE_DPORT("FALSE"),
    .USE_MULT("MULTIPLY"),
    .USE_PATTERN_DETECT("NO_PATDET"),
    .USE_SIMD("ONE48")) 
    buff1_reg
       (.A({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,A_q0}),
        .ACIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .ACOUT(NLW_buff1_reg_ACOUT_UNCONNECTED[29:0]),
        .ALUMODE({1'b0,1'b0,1'b0,1'b0}),
        .B({grp_fu_125_p1[31],grp_fu_125_p1[31],grp_fu_125_p1[31],grp_fu_125_p1[31:17]}),
        .BCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .BCOUT(NLW_buff1_reg_BCOUT_UNCONNECTED[17:0]),
        .C({1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1}),
        .CARRYCASCIN(1'b0),
        .CARRYCASCOUT(NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED),
        .CARRYIN(1'b0),
        .CARRYINSEL({1'b0,1'b0,1'b0}),
        .CARRYOUT(NLW_buff1_reg_CARRYOUT_UNCONNECTED[3:0]),
        .CEA1(E),
        .CEA2(1'b1),
        .CEAD(1'b0),
        .CEALUMODE(1'b0),
        .CEB1(1'b0),
        .CEB2(1'b1),
        .CEC(1'b0),
        .CECARRYIN(1'b0),
        .CECTRL(1'b0),
        .CED(1'b0),
        .CEINMODE(1'b0),
        .CEM(1'b1),
        .CEP(1'b1),
        .CLK(ap_clk),
        .D({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .INMODE({1'b0,1'b0,1'b0,1'b0,1'b0}),
        .MULTSIGNIN(1'b0),
        .MULTSIGNOUT(NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED),
        .OPMODE({1'b1,1'b0,1'b1,1'b0,1'b1,1'b0,1'b1}),
        .OVERFLOW(NLW_buff1_reg_OVERFLOW_UNCONNECTED),
        .P(NLW_buff1_reg_P_UNCONNECTED[47:0]),
        .PATTERNBDETECT(NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED),
        .PATTERNDETECT(NLW_buff1_reg_PATTERNDETECT_UNCONNECTED),
        .PCIN({buff0_reg_n_107,buff0_reg_n_108,buff0_reg_n_109,buff0_reg_n_110,buff0_reg_n_111,buff0_reg_n_112,buff0_reg_n_113,buff0_reg_n_114,buff0_reg_n_115,buff0_reg_n_116,buff0_reg_n_117,buff0_reg_n_118,buff0_reg_n_119,buff0_reg_n_120,buff0_reg_n_121,buff0_reg_n_122,buff0_reg_n_123,buff0_reg_n_124,buff0_reg_n_125,buff0_reg_n_126,buff0_reg_n_127,buff0_reg_n_128,buff0_reg_n_129,buff0_reg_n_130,buff0_reg_n_131,buff0_reg_n_132,buff0_reg_n_133,buff0_reg_n_134,buff0_reg_n_135,buff0_reg_n_136,buff0_reg_n_137,buff0_reg_n_138,buff0_reg_n_139,buff0_reg_n_140,buff0_reg_n_141,buff0_reg_n_142,buff0_reg_n_143,buff0_reg_n_144,buff0_reg_n_145,buff0_reg_n_146,buff0_reg_n_147,buff0_reg_n_148,buff0_reg_n_149,buff0_reg_n_150,buff0_reg_n_151,buff0_reg_n_152,buff0_reg_n_153,buff0_reg_n_154}),
        .PCOUT({buff1_reg_n_107,buff1_reg_n_108,buff1_reg_n_109,buff1_reg_n_110,buff1_reg_n_111,buff1_reg_n_112,buff1_reg_n_113,buff1_reg_n_114,buff1_reg_n_115,buff1_reg_n_116,buff1_reg_n_117,buff1_reg_n_118,buff1_reg_n_119,buff1_reg_n_120,buff1_reg_n_121,buff1_reg_n_122,buff1_reg_n_123,buff1_reg_n_124,buff1_reg_n_125,buff1_reg_n_126,buff1_reg_n_127,buff1_reg_n_128,buff1_reg_n_129,buff1_reg_n_130,buff1_reg_n_131,buff1_reg_n_132,buff1_reg_n_133,buff1_reg_n_134,buff1_reg_n_135,buff1_reg_n_136,buff1_reg_n_137,buff1_reg_n_138,buff1_reg_n_139,buff1_reg_n_140,buff1_reg_n_141,buff1_reg_n_142,buff1_reg_n_143,buff1_reg_n_144,buff1_reg_n_145,buff1_reg_n_146,buff1_reg_n_147,buff1_reg_n_148,buff1_reg_n_149,buff1_reg_n_150,buff1_reg_n_151,buff1_reg_n_152,buff1_reg_n_153,buff1_reg_n_154}),
        .RSTA(1'b0),
        .RSTALLCARRYIN(1'b0),
        .RSTALUMODE(1'b0),
        .RSTB(1'b0),
        .RSTC(1'b0),
        .RSTCTRL(1'b0),
        .RSTD(1'b0),
        .RSTINMODE(1'b0),
        .RSTM(1'b0),
        .RSTP(1'b0),
        .UNDERFLOW(NLW_buff1_reg_UNDERFLOW_UNCONNECTED));
  FDRE \buff1_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_106),
        .Q(\buff1_reg_n_1_[0] ),
        .R(1'b0));
  FDRE \buff1_reg[10] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_96),
        .Q(\buff1_reg_n_1_[10] ),
        .R(1'b0));
  FDRE \buff1_reg[11] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_95),
        .Q(\buff1_reg_n_1_[11] ),
        .R(1'b0));
  FDRE \buff1_reg[12] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_94),
        .Q(\buff1_reg_n_1_[12] ),
        .R(1'b0));
  FDRE \buff1_reg[13] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_93),
        .Q(\buff1_reg_n_1_[13] ),
        .R(1'b0));
  FDRE \buff1_reg[14] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_92),
        .Q(\buff1_reg_n_1_[14] ),
        .R(1'b0));
  FDRE \buff1_reg[15] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_91),
        .Q(\buff1_reg_n_1_[15] ),
        .R(1'b0));
  FDRE \buff1_reg[16] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_90),
        .Q(\buff1_reg_n_1_[16] ),
        .R(1'b0));
  FDRE \buff1_reg[1] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_105),
        .Q(\buff1_reg_n_1_[1] ),
        .R(1'b0));
  FDRE \buff1_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_104),
        .Q(\buff1_reg_n_1_[2] ),
        .R(1'b0));
  FDRE \buff1_reg[3] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_103),
        .Q(\buff1_reg_n_1_[3] ),
        .R(1'b0));
  FDRE \buff1_reg[4] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_102),
        .Q(\buff1_reg_n_1_[4] ),
        .R(1'b0));
  FDRE \buff1_reg[5] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_101),
        .Q(\buff1_reg_n_1_[5] ),
        .R(1'b0));
  FDRE \buff1_reg[6] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_100),
        .Q(\buff1_reg_n_1_[6] ),
        .R(1'b0));
  FDRE \buff1_reg[7] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_99),
        .Q(\buff1_reg_n_1_[7] ),
        .R(1'b0));
  FDRE \buff1_reg[8] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_98),
        .Q(\buff1_reg_n_1_[8] ),
        .R(1'b0));
  FDRE \buff1_reg[9] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_97),
        .Q(\buff1_reg_n_1_[9] ),
        .R(1'b0));
  DSP48E1 #(
    .ACASCREG(2),
    .ADREG(1),
    .ALUMODEREG(0),
    .AREG(2),
    .AUTORESET_PATDET("NO_RESET"),
    .A_INPUT("DIRECT"),
    .BCASCREG(2),
    .BREG(2),
    .B_INPUT("DIRECT"),
    .CARRYINREG(0),
    .CARRYINSELREG(0),
    .CREG(1),
    .DREG(1),
    .INMODEREG(0),
    .MASK(48'h3FFFFFFFFFFF),
    .MREG(1),
    .OPMODEREG(0),
    .PATTERN(48'h000000000000),
    .PREG(1),
    .SEL_MASK("MASK"),
    .SEL_PATTERN("PATTERN"),
    .USE_DPORT("FALSE"),
    .USE_MULT("MULTIPLY"),
    .USE_PATTERN_DETECT("NO_PATDET"),
    .USE_SIMD("ONE48")) 
    buff2_reg
       (.A({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,grp_fu_125_p1[16:0]}),
        .ACIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .ACOUT(NLW_buff2_reg_ACOUT_UNCONNECTED[29:0]),
        .ALUMODE({1'b0,1'b0,1'b0,1'b0}),
        .B({Q[14],Q[14],Q[14],Q}),
        .BCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .BCOUT(NLW_buff2_reg_BCOUT_UNCONNECTED[17:0]),
        .C({1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1}),
        .CARRYCASCIN(1'b0),
        .CARRYCASCOUT(NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED),
        .CARRYIN(1'b0),
        .CARRYINSEL({1'b0,1'b0,1'b0}),
        .CARRYOUT(NLW_buff2_reg_CARRYOUT_UNCONNECTED[3:0]),
        .CEA1(1'b1),
        .CEA2(1'b1),
        .CEAD(1'b0),
        .CEALUMODE(1'b0),
        .CEB1(1'b1),
        .CEB2(1'b1),
        .CEC(1'b0),
        .CECARRYIN(1'b0),
        .CECTRL(1'b0),
        .CED(1'b0),
        .CEINMODE(1'b0),
        .CEM(1'b1),
        .CEP(1'b1),
        .CLK(ap_clk),
        .D({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .INMODE({1'b0,1'b0,1'b0,1'b0,1'b0}),
        .MULTSIGNIN(1'b0),
        .MULTSIGNOUT(NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED),
        .OPMODE({1'b0,1'b0,1'b1,1'b0,1'b1,1'b0,1'b1}),
        .OVERFLOW(NLW_buff2_reg_OVERFLOW_UNCONNECTED),
        .P({buff2_reg_n_59,buff2_reg_n_60,buff2_reg_n_61,buff2_reg_n_62,buff2_reg_n_63,buff2_reg_n_64,buff2_reg_n_65,buff2_reg_n_66,buff2_reg_n_67,buff2_reg_n_68,buff2_reg_n_69,buff2_reg_n_70,buff2_reg_n_71,buff2_reg_n_72,buff2_reg_n_73,buff2_reg_n_74,buff2_reg_n_75,buff2_reg_n_76,buff2_reg_n_77,buff2_reg_n_78,buff2_reg_n_79,buff2_reg_n_80,buff2_reg_n_81,buff2_reg_n_82,buff2_reg_n_83,buff2_reg_n_84,buff2_reg_n_85,buff2_reg_n_86,buff2_reg_n_87,buff2_reg_n_88,buff2_reg_n_89,buff2_reg_n_90,buff2_reg_n_91,A_d0[31:17]}),
        .PATTERNBDETECT(NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED),
        .PATTERNDETECT(NLW_buff2_reg_PATTERNDETECT_UNCONNECTED),
        .PCIN({buff1_reg_n_107,buff1_reg_n_108,buff1_reg_n_109,buff1_reg_n_110,buff1_reg_n_111,buff1_reg_n_112,buff1_reg_n_113,buff1_reg_n_114,buff1_reg_n_115,buff1_reg_n_116,buff1_reg_n_117,buff1_reg_n_118,buff1_reg_n_119,buff1_reg_n_120,buff1_reg_n_121,buff1_reg_n_122,buff1_reg_n_123,buff1_reg_n_124,buff1_reg_n_125,buff1_reg_n_126,buff1_reg_n_127,buff1_reg_n_128,buff1_reg_n_129,buff1_reg_n_130,buff1_reg_n_131,buff1_reg_n_132,buff1_reg_n_133,buff1_reg_n_134,buff1_reg_n_135,buff1_reg_n_136,buff1_reg_n_137,buff1_reg_n_138,buff1_reg_n_139,buff1_reg_n_140,buff1_reg_n_141,buff1_reg_n_142,buff1_reg_n_143,buff1_reg_n_144,buff1_reg_n_145,buff1_reg_n_146,buff1_reg_n_147,buff1_reg_n_148,buff1_reg_n_149,buff1_reg_n_150,buff1_reg_n_151,buff1_reg_n_152,buff1_reg_n_153,buff1_reg_n_154}),
        .PCOUT(NLW_buff2_reg_PCOUT_UNCONNECTED[47:0]),
        .RSTA(1'b0),
        .RSTALLCARRYIN(1'b0),
        .RSTALUMODE(1'b0),
        .RSTB(1'b0),
        .RSTC(1'b0),
        .RSTCTRL(1'b0),
        .RSTD(1'b0),
        .RSTINMODE(1'b0),
        .RSTM(1'b0),
        .RSTP(1'b0),
        .UNDERFLOW(NLW_buff2_reg_UNDERFLOW_UNCONNECTED));
  FDRE \buff2_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[0] ),
        .Q(A_d0[0]),
        .R(1'b0));
  FDRE \buff2_reg[10] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[10] ),
        .Q(A_d0[10]),
        .R(1'b0));
  FDRE \buff2_reg[11] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[11] ),
        .Q(A_d0[11]),
        .R(1'b0));
  FDRE \buff2_reg[12] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[12] ),
        .Q(A_d0[12]),
        .R(1'b0));
  FDRE \buff2_reg[13] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[13] ),
        .Q(A_d0[13]),
        .R(1'b0));
  FDRE \buff2_reg[14] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[14] ),
        .Q(A_d0[14]),
        .R(1'b0));
  FDRE \buff2_reg[15] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[15] ),
        .Q(A_d0[15]),
        .R(1'b0));
  FDRE \buff2_reg[16] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[16] ),
        .Q(A_d0[16]),
        .R(1'b0));
  FDRE \buff2_reg[1] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[1] ),
        .Q(A_d0[1]),
        .R(1'b0));
  FDRE \buff2_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[2] ),
        .Q(A_d0[2]),
        .R(1'b0));
  FDRE \buff2_reg[3] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[3] ),
        .Q(A_d0[3]),
        .R(1'b0));
  FDRE \buff2_reg[4] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[4] ),
        .Q(A_d0[4]),
        .R(1'b0));
  FDRE \buff2_reg[5] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[5] ),
        .Q(A_d0[5]),
        .R(1'b0));
  FDRE \buff2_reg[6] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[6] ),
        .Q(A_d0[6]),
        .R(1'b0));
  FDRE \buff2_reg[7] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[7] ),
        .Q(A_d0[7]),
        .R(1'b0));
  FDRE \buff2_reg[8] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[8] ),
        .Q(A_d0[8]),
        .R(1'b0));
  FDRE \buff2_reg[9] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[9] ),
        .Q(A_d0[9]),
        .R(1'b0));
endmodule

(* ORIG_REF_NAME = "loop_imperfect_mubkb_MulnS_0" *) 
module bd_0_hls_inst_0_loop_imperfect_mubkb_MulnS_0_2
   (grp_fu_125_p1,
    ap_clk,
    grp_fu_113_p1,
    Q,
    buff2_reg_0);
  output [31:0]grp_fu_125_p1;
  input ap_clk;
  input [31:0]grp_fu_113_p1;
  input [16:0]Q;
  input [14:0]buff2_reg_0;

  wire [16:0]Q;
  wire ap_clk;
  wire buff0_reg_n_100;
  wire buff0_reg_n_101;
  wire buff0_reg_n_102;
  wire buff0_reg_n_103;
  wire buff0_reg_n_104;
  wire buff0_reg_n_105;
  wire buff0_reg_n_106;
  wire buff0_reg_n_107;
  wire buff0_reg_n_108;
  wire buff0_reg_n_109;
  wire buff0_reg_n_110;
  wire buff0_reg_n_111;
  wire buff0_reg_n_112;
  wire buff0_reg_n_113;
  wire buff0_reg_n_114;
  wire buff0_reg_n_115;
  wire buff0_reg_n_116;
  wire buff0_reg_n_117;
  wire buff0_reg_n_118;
  wire buff0_reg_n_119;
  wire buff0_reg_n_120;
  wire buff0_reg_n_121;
  wire buff0_reg_n_122;
  wire buff0_reg_n_123;
  wire buff0_reg_n_124;
  wire buff0_reg_n_125;
  wire buff0_reg_n_126;
  wire buff0_reg_n_127;
  wire buff0_reg_n_128;
  wire buff0_reg_n_129;
  wire buff0_reg_n_130;
  wire buff0_reg_n_131;
  wire buff0_reg_n_132;
  wire buff0_reg_n_133;
  wire buff0_reg_n_134;
  wire buff0_reg_n_135;
  wire buff0_reg_n_136;
  wire buff0_reg_n_137;
  wire buff0_reg_n_138;
  wire buff0_reg_n_139;
  wire buff0_reg_n_140;
  wire buff0_reg_n_141;
  wire buff0_reg_n_142;
  wire buff0_reg_n_143;
  wire buff0_reg_n_144;
  wire buff0_reg_n_145;
  wire buff0_reg_n_146;
  wire buff0_reg_n_147;
  wire buff0_reg_n_148;
  wire buff0_reg_n_149;
  wire buff0_reg_n_150;
  wire buff0_reg_n_151;
  wire buff0_reg_n_152;
  wire buff0_reg_n_153;
  wire buff0_reg_n_154;
  wire buff0_reg_n_59;
  wire buff0_reg_n_60;
  wire buff0_reg_n_61;
  wire buff0_reg_n_62;
  wire buff0_reg_n_63;
  wire buff0_reg_n_64;
  wire buff0_reg_n_65;
  wire buff0_reg_n_66;
  wire buff0_reg_n_67;
  wire buff0_reg_n_68;
  wire buff0_reg_n_69;
  wire buff0_reg_n_70;
  wire buff0_reg_n_71;
  wire buff0_reg_n_72;
  wire buff0_reg_n_73;
  wire buff0_reg_n_74;
  wire buff0_reg_n_75;
  wire buff0_reg_n_76;
  wire buff0_reg_n_77;
  wire buff0_reg_n_78;
  wire buff0_reg_n_79;
  wire buff0_reg_n_80;
  wire buff0_reg_n_81;
  wire buff0_reg_n_82;
  wire buff0_reg_n_83;
  wire buff0_reg_n_84;
  wire buff0_reg_n_85;
  wire buff0_reg_n_86;
  wire buff0_reg_n_87;
  wire buff0_reg_n_88;
  wire buff0_reg_n_89;
  wire buff0_reg_n_90;
  wire buff0_reg_n_91;
  wire buff0_reg_n_92;
  wire buff0_reg_n_93;
  wire buff0_reg_n_94;
  wire buff0_reg_n_95;
  wire buff0_reg_n_96;
  wire buff0_reg_n_97;
  wire buff0_reg_n_98;
  wire buff0_reg_n_99;
  wire buff1_reg_i_1__0_n_3;
  wire buff1_reg_i_1__0_n_4;
  wire buff1_reg_i_2__0_n_1;
  wire buff1_reg_i_2__0_n_2;
  wire buff1_reg_i_2__0_n_3;
  wire buff1_reg_i_2__0_n_4;
  wire buff1_reg_i_3__0_n_1;
  wire buff1_reg_i_3__0_n_2;
  wire buff1_reg_i_3__0_n_3;
  wire buff1_reg_i_3__0_n_4;
  wire buff1_reg_i_4__0_n_1;
  wire buff1_reg_i_4__0_n_2;
  wire buff1_reg_i_4__0_n_3;
  wire buff1_reg_i_4__0_n_4;
  wire buff1_reg_n_107;
  wire buff1_reg_n_108;
  wire buff1_reg_n_109;
  wire buff1_reg_n_110;
  wire buff1_reg_n_111;
  wire buff1_reg_n_112;
  wire buff1_reg_n_113;
  wire buff1_reg_n_114;
  wire buff1_reg_n_115;
  wire buff1_reg_n_116;
  wire buff1_reg_n_117;
  wire buff1_reg_n_118;
  wire buff1_reg_n_119;
  wire buff1_reg_n_120;
  wire buff1_reg_n_121;
  wire buff1_reg_n_122;
  wire buff1_reg_n_123;
  wire buff1_reg_n_124;
  wire buff1_reg_n_125;
  wire buff1_reg_n_126;
  wire buff1_reg_n_127;
  wire buff1_reg_n_128;
  wire buff1_reg_n_129;
  wire buff1_reg_n_130;
  wire buff1_reg_n_131;
  wire buff1_reg_n_132;
  wire buff1_reg_n_133;
  wire buff1_reg_n_134;
  wire buff1_reg_n_135;
  wire buff1_reg_n_136;
  wire buff1_reg_n_137;
  wire buff1_reg_n_138;
  wire buff1_reg_n_139;
  wire buff1_reg_n_140;
  wire buff1_reg_n_141;
  wire buff1_reg_n_142;
  wire buff1_reg_n_143;
  wire buff1_reg_n_144;
  wire buff1_reg_n_145;
  wire buff1_reg_n_146;
  wire buff1_reg_n_147;
  wire buff1_reg_n_148;
  wire buff1_reg_n_149;
  wire buff1_reg_n_150;
  wire buff1_reg_n_151;
  wire buff1_reg_n_152;
  wire buff1_reg_n_153;
  wire buff1_reg_n_154;
  wire \buff1_reg_n_1_[0] ;
  wire \buff1_reg_n_1_[10] ;
  wire \buff1_reg_n_1_[11] ;
  wire \buff1_reg_n_1_[12] ;
  wire \buff1_reg_n_1_[13] ;
  wire \buff1_reg_n_1_[14] ;
  wire \buff1_reg_n_1_[15] ;
  wire \buff1_reg_n_1_[16] ;
  wire \buff1_reg_n_1_[1] ;
  wire \buff1_reg_n_1_[2] ;
  wire \buff1_reg_n_1_[3] ;
  wire \buff1_reg_n_1_[4] ;
  wire \buff1_reg_n_1_[5] ;
  wire \buff1_reg_n_1_[6] ;
  wire \buff1_reg_n_1_[7] ;
  wire \buff1_reg_n_1_[8] ;
  wire \buff1_reg_n_1_[9] ;
  wire [14:0]buff2_reg_0;
  wire [31:0]buff2_reg__0;
  wire buff2_reg_i_1_n_1;
  wire buff2_reg_i_1_n_2;
  wire buff2_reg_i_1_n_3;
  wire buff2_reg_i_1_n_4;
  wire buff2_reg_i_2_n_1;
  wire buff2_reg_i_2_n_2;
  wire buff2_reg_i_2_n_3;
  wire buff2_reg_i_2_n_4;
  wire buff2_reg_i_3_n_1;
  wire buff2_reg_i_3_n_2;
  wire buff2_reg_i_3_n_3;
  wire buff2_reg_i_3_n_4;
  wire buff2_reg_i_4_n_1;
  wire buff2_reg_i_4_n_2;
  wire buff2_reg_i_4_n_3;
  wire buff2_reg_i_4_n_4;
  wire buff2_reg_i_6_n_1;
  wire buff2_reg_n_59;
  wire buff2_reg_n_60;
  wire buff2_reg_n_61;
  wire buff2_reg_n_62;
  wire buff2_reg_n_63;
  wire buff2_reg_n_64;
  wire buff2_reg_n_65;
  wire buff2_reg_n_66;
  wire buff2_reg_n_67;
  wire buff2_reg_n_68;
  wire buff2_reg_n_69;
  wire buff2_reg_n_70;
  wire buff2_reg_n_71;
  wire buff2_reg_n_72;
  wire buff2_reg_n_73;
  wire buff2_reg_n_74;
  wire buff2_reg_n_75;
  wire buff2_reg_n_76;
  wire buff2_reg_n_77;
  wire buff2_reg_n_78;
  wire buff2_reg_n_79;
  wire buff2_reg_n_80;
  wire buff2_reg_n_81;
  wire buff2_reg_n_82;
  wire buff2_reg_n_83;
  wire buff2_reg_n_84;
  wire buff2_reg_n_85;
  wire buff2_reg_n_86;
  wire buff2_reg_n_87;
  wire buff2_reg_n_88;
  wire buff2_reg_n_89;
  wire buff2_reg_n_90;
  wire buff2_reg_n_91;
  wire [31:0]grp_fu_113_p1;
  wire [31:0]grp_fu_125_p1;
  wire NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED;
  wire NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED;
  wire NLW_buff0_reg_OVERFLOW_UNCONNECTED;
  wire NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED;
  wire NLW_buff0_reg_PATTERNDETECT_UNCONNECTED;
  wire NLW_buff0_reg_UNDERFLOW_UNCONNECTED;
  wire [29:0]NLW_buff0_reg_ACOUT_UNCONNECTED;
  wire [17:0]NLW_buff0_reg_BCOUT_UNCONNECTED;
  wire [3:0]NLW_buff0_reg_CARRYOUT_UNCONNECTED;
  wire NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED;
  wire NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED;
  wire NLW_buff1_reg_OVERFLOW_UNCONNECTED;
  wire NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED;
  wire NLW_buff1_reg_PATTERNDETECT_UNCONNECTED;
  wire NLW_buff1_reg_UNDERFLOW_UNCONNECTED;
  wire [29:0]NLW_buff1_reg_ACOUT_UNCONNECTED;
  wire [17:0]NLW_buff1_reg_BCOUT_UNCONNECTED;
  wire [3:0]NLW_buff1_reg_CARRYOUT_UNCONNECTED;
  wire [47:0]NLW_buff1_reg_P_UNCONNECTED;
  wire [3:2]NLW_buff1_reg_i_1__0_CO_UNCONNECTED;
  wire [3:3]NLW_buff1_reg_i_1__0_O_UNCONNECTED;
  wire NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED;
  wire NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED;
  wire NLW_buff2_reg_OVERFLOW_UNCONNECTED;
  wire NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED;
  wire NLW_buff2_reg_PATTERNDETECT_UNCONNECTED;
  wire NLW_buff2_reg_UNDERFLOW_UNCONNECTED;
  wire [29:0]NLW_buff2_reg_ACOUT_UNCONNECTED;
  wire [17:0]NLW_buff2_reg_BCOUT_UNCONNECTED;
  wire [3:0]NLW_buff2_reg_CARRYOUT_UNCONNECTED;
  wire [47:0]NLW_buff2_reg_PCOUT_UNCONNECTED;

  DSP48E1 #(
    .ACASCREG(2),
    .ADREG(1),
    .ALUMODEREG(0),
    .AREG(2),
    .AUTORESET_PATDET("NO_RESET"),
    .A_INPUT("DIRECT"),
    .BCASCREG(1),
    .BREG(1),
    .B_INPUT("DIRECT"),
    .CARRYINREG(0),
    .CARRYINSELREG(0),
    .CREG(0),
    .DREG(1),
    .INMODEREG(0),
    .MASK(48'h3FFFFFFFFFFF),
    .MREG(0),
    .OPMODEREG(0),
    .PATTERN(48'h000000000000),
    .PREG(1),
    .SEL_MASK("MASK"),
    .SEL_PATTERN("PATTERN"),
    .USE_DPORT("FALSE"),
    .USE_MULT("MULTIPLY"),
    .USE_PATTERN_DETECT("NO_PATDET"),
    .USE_SIMD("ONE48")) 
    buff0_reg
       (.A({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,Q}),
        .ACIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .ACOUT(NLW_buff0_reg_ACOUT_UNCONNECTED[29:0]),
        .ALUMODE({1'b0,1'b0,1'b0,1'b0}),
        .B({1'b0,grp_fu_113_p1[16:0]}),
        .BCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .BCOUT(NLW_buff0_reg_BCOUT_UNCONNECTED[17:0]),
        .C({1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1}),
        .CARRYCASCIN(1'b0),
        .CARRYCASCOUT(NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED),
        .CARRYIN(1'b0),
        .CARRYINSEL({1'b0,1'b0,1'b0}),
        .CARRYOUT(NLW_buff0_reg_CARRYOUT_UNCONNECTED[3:0]),
        .CEA1(1'b1),
        .CEA2(1'b1),
        .CEAD(1'b0),
        .CEALUMODE(1'b0),
        .CEB1(1'b0),
        .CEB2(1'b1),
        .CEC(1'b0),
        .CECARRYIN(1'b0),
        .CECTRL(1'b0),
        .CED(1'b0),
        .CEINMODE(1'b0),
        .CEM(1'b0),
        .CEP(1'b1),
        .CLK(ap_clk),
        .D({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .INMODE({1'b0,1'b0,1'b0,1'b0,1'b0}),
        .MULTSIGNIN(1'b0),
        .MULTSIGNOUT(NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED),
        .OPMODE({1'b0,1'b0,1'b0,1'b0,1'b1,1'b0,1'b1}),
        .OVERFLOW(NLW_buff0_reg_OVERFLOW_UNCONNECTED),
        .P({buff0_reg_n_59,buff0_reg_n_60,buff0_reg_n_61,buff0_reg_n_62,buff0_reg_n_63,buff0_reg_n_64,buff0_reg_n_65,buff0_reg_n_66,buff0_reg_n_67,buff0_reg_n_68,buff0_reg_n_69,buff0_reg_n_70,buff0_reg_n_71,buff0_reg_n_72,buff0_reg_n_73,buff0_reg_n_74,buff0_reg_n_75,buff0_reg_n_76,buff0_reg_n_77,buff0_reg_n_78,buff0_reg_n_79,buff0_reg_n_80,buff0_reg_n_81,buff0_reg_n_82,buff0_reg_n_83,buff0_reg_n_84,buff0_reg_n_85,buff0_reg_n_86,buff0_reg_n_87,buff0_reg_n_88,buff0_reg_n_89,buff0_reg_n_90,buff0_reg_n_91,buff0_reg_n_92,buff0_reg_n_93,buff0_reg_n_94,buff0_reg_n_95,buff0_reg_n_96,buff0_reg_n_97,buff0_reg_n_98,buff0_reg_n_99,buff0_reg_n_100,buff0_reg_n_101,buff0_reg_n_102,buff0_reg_n_103,buff0_reg_n_104,buff0_reg_n_105,buff0_reg_n_106}),
        .PATTERNBDETECT(NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED),
        .PATTERNDETECT(NLW_buff0_reg_PATTERNDETECT_UNCONNECTED),
        .PCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .PCOUT({buff0_reg_n_107,buff0_reg_n_108,buff0_reg_n_109,buff0_reg_n_110,buff0_reg_n_111,buff0_reg_n_112,buff0_reg_n_113,buff0_reg_n_114,buff0_reg_n_115,buff0_reg_n_116,buff0_reg_n_117,buff0_reg_n_118,buff0_reg_n_119,buff0_reg_n_120,buff0_reg_n_121,buff0_reg_n_122,buff0_reg_n_123,buff0_reg_n_124,buff0_reg_n_125,buff0_reg_n_126,buff0_reg_n_127,buff0_reg_n_128,buff0_reg_n_129,buff0_reg_n_130,buff0_reg_n_131,buff0_reg_n_132,buff0_reg_n_133,buff0_reg_n_134,buff0_reg_n_135,buff0_reg_n_136,buff0_reg_n_137,buff0_reg_n_138,buff0_reg_n_139,buff0_reg_n_140,buff0_reg_n_141,buff0_reg_n_142,buff0_reg_n_143,buff0_reg_n_144,buff0_reg_n_145,buff0_reg_n_146,buff0_reg_n_147,buff0_reg_n_148,buff0_reg_n_149,buff0_reg_n_150,buff0_reg_n_151,buff0_reg_n_152,buff0_reg_n_153,buff0_reg_n_154}),
        .RSTA(1'b0),
        .RSTALLCARRYIN(1'b0),
        .RSTALUMODE(1'b0),
        .RSTB(1'b0),
        .RSTC(1'b0),
        .RSTCTRL(1'b0),
        .RSTD(1'b0),
        .RSTINMODE(1'b0),
        .RSTM(1'b0),
        .RSTP(1'b0),
        .UNDERFLOW(NLW_buff0_reg_UNDERFLOW_UNCONNECTED));
  DSP48E1 #(
    .ACASCREG(2),
    .ADREG(1),
    .ALUMODEREG(0),
    .AREG(2),
    .AUTORESET_PATDET("NO_RESET"),
    .A_INPUT("DIRECT"),
    .BCASCREG(1),
    .BREG(1),
    .B_INPUT("DIRECT"),
    .CARRYINREG(0),
    .CARRYINSELREG(0),
    .CREG(1),
    .DREG(1),
    .INMODEREG(0),
    .MASK(48'h3FFFFFFFFFFF),
    .MREG(1),
    .OPMODEREG(0),
    .PATTERN(48'h000000000000),
    .PREG(1),
    .SEL_MASK("MASK"),
    .SEL_PATTERN("PATTERN"),
    .USE_DPORT("FALSE"),
    .USE_MULT("MULTIPLY"),
    .USE_PATTERN_DETECT("NO_PATDET"),
    .USE_SIMD("ONE48")) 
    buff1_reg
       (.A({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,Q}),
        .ACIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .ACOUT(NLW_buff1_reg_ACOUT_UNCONNECTED[29:0]),
        .ALUMODE({1'b0,1'b0,1'b0,1'b0}),
        .B({grp_fu_113_p1[31],grp_fu_113_p1[31],grp_fu_113_p1[31],grp_fu_113_p1[31:17]}),
        .BCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .BCOUT(NLW_buff1_reg_BCOUT_UNCONNECTED[17:0]),
        .C({1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1}),
        .CARRYCASCIN(1'b0),
        .CARRYCASCOUT(NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED),
        .CARRYIN(1'b0),
        .CARRYINSEL({1'b0,1'b0,1'b0}),
        .CARRYOUT(NLW_buff1_reg_CARRYOUT_UNCONNECTED[3:0]),
        .CEA1(1'b1),
        .CEA2(1'b1),
        .CEAD(1'b0),
        .CEALUMODE(1'b0),
        .CEB1(1'b0),
        .CEB2(1'b1),
        .CEC(1'b0),
        .CECARRYIN(1'b0),
        .CECTRL(1'b0),
        .CED(1'b0),
        .CEINMODE(1'b0),
        .CEM(1'b1),
        .CEP(1'b1),
        .CLK(ap_clk),
        .D({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .INMODE({1'b0,1'b0,1'b0,1'b0,1'b0}),
        .MULTSIGNIN(1'b0),
        .MULTSIGNOUT(NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED),
        .OPMODE({1'b1,1'b0,1'b1,1'b0,1'b1,1'b0,1'b1}),
        .OVERFLOW(NLW_buff1_reg_OVERFLOW_UNCONNECTED),
        .P(NLW_buff1_reg_P_UNCONNECTED[47:0]),
        .PATTERNBDETECT(NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED),
        .PATTERNDETECT(NLW_buff1_reg_PATTERNDETECT_UNCONNECTED),
        .PCIN({buff0_reg_n_107,buff0_reg_n_108,buff0_reg_n_109,buff0_reg_n_110,buff0_reg_n_111,buff0_reg_n_112,buff0_reg_n_113,buff0_reg_n_114,buff0_reg_n_115,buff0_reg_n_116,buff0_reg_n_117,buff0_reg_n_118,buff0_reg_n_119,buff0_reg_n_120,buff0_reg_n_121,buff0_reg_n_122,buff0_reg_n_123,buff0_reg_n_124,buff0_reg_n_125,buff0_reg_n_126,buff0_reg_n_127,buff0_reg_n_128,buff0_reg_n_129,buff0_reg_n_130,buff0_reg_n_131,buff0_reg_n_132,buff0_reg_n_133,buff0_reg_n_134,buff0_reg_n_135,buff0_reg_n_136,buff0_reg_n_137,buff0_reg_n_138,buff0_reg_n_139,buff0_reg_n_140,buff0_reg_n_141,buff0_reg_n_142,buff0_reg_n_143,buff0_reg_n_144,buff0_reg_n_145,buff0_reg_n_146,buff0_reg_n_147,buff0_reg_n_148,buff0_reg_n_149,buff0_reg_n_150,buff0_reg_n_151,buff0_reg_n_152,buff0_reg_n_153,buff0_reg_n_154}),
        .PCOUT({buff1_reg_n_107,buff1_reg_n_108,buff1_reg_n_109,buff1_reg_n_110,buff1_reg_n_111,buff1_reg_n_112,buff1_reg_n_113,buff1_reg_n_114,buff1_reg_n_115,buff1_reg_n_116,buff1_reg_n_117,buff1_reg_n_118,buff1_reg_n_119,buff1_reg_n_120,buff1_reg_n_121,buff1_reg_n_122,buff1_reg_n_123,buff1_reg_n_124,buff1_reg_n_125,buff1_reg_n_126,buff1_reg_n_127,buff1_reg_n_128,buff1_reg_n_129,buff1_reg_n_130,buff1_reg_n_131,buff1_reg_n_132,buff1_reg_n_133,buff1_reg_n_134,buff1_reg_n_135,buff1_reg_n_136,buff1_reg_n_137,buff1_reg_n_138,buff1_reg_n_139,buff1_reg_n_140,buff1_reg_n_141,buff1_reg_n_142,buff1_reg_n_143,buff1_reg_n_144,buff1_reg_n_145,buff1_reg_n_146,buff1_reg_n_147,buff1_reg_n_148,buff1_reg_n_149,buff1_reg_n_150,buff1_reg_n_151,buff1_reg_n_152,buff1_reg_n_153,buff1_reg_n_154}),
        .RSTA(1'b0),
        .RSTALLCARRYIN(1'b0),
        .RSTALUMODE(1'b0),
        .RSTB(1'b0),
        .RSTC(1'b0),
        .RSTCTRL(1'b0),
        .RSTD(1'b0),
        .RSTINMODE(1'b0),
        .RSTM(1'b0),
        .RSTP(1'b0),
        .UNDERFLOW(NLW_buff1_reg_UNDERFLOW_UNCONNECTED));
  FDRE \buff1_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_106),
        .Q(\buff1_reg_n_1_[0] ),
        .R(1'b0));
  FDRE \buff1_reg[10] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_96),
        .Q(\buff1_reg_n_1_[10] ),
        .R(1'b0));
  FDRE \buff1_reg[11] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_95),
        .Q(\buff1_reg_n_1_[11] ),
        .R(1'b0));
  FDRE \buff1_reg[12] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_94),
        .Q(\buff1_reg_n_1_[12] ),
        .R(1'b0));
  FDRE \buff1_reg[13] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_93),
        .Q(\buff1_reg_n_1_[13] ),
        .R(1'b0));
  FDRE \buff1_reg[14] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_92),
        .Q(\buff1_reg_n_1_[14] ),
        .R(1'b0));
  FDRE \buff1_reg[15] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_91),
        .Q(\buff1_reg_n_1_[15] ),
        .R(1'b0));
  FDRE \buff1_reg[16] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_90),
        .Q(\buff1_reg_n_1_[16] ),
        .R(1'b0));
  FDRE \buff1_reg[1] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_105),
        .Q(\buff1_reg_n_1_[1] ),
        .R(1'b0));
  FDRE \buff1_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_104),
        .Q(\buff1_reg_n_1_[2] ),
        .R(1'b0));
  FDRE \buff1_reg[3] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_103),
        .Q(\buff1_reg_n_1_[3] ),
        .R(1'b0));
  FDRE \buff1_reg[4] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_102),
        .Q(\buff1_reg_n_1_[4] ),
        .R(1'b0));
  FDRE \buff1_reg[5] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_101),
        .Q(\buff1_reg_n_1_[5] ),
        .R(1'b0));
  FDRE \buff1_reg[6] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_100),
        .Q(\buff1_reg_n_1_[6] ),
        .R(1'b0));
  FDRE \buff1_reg[7] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_99),
        .Q(\buff1_reg_n_1_[7] ),
        .R(1'b0));
  FDRE \buff1_reg[8] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_98),
        .Q(\buff1_reg_n_1_[8] ),
        .R(1'b0));
  FDRE \buff1_reg[9] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_97),
        .Q(\buff1_reg_n_1_[9] ),
        .R(1'b0));
  CARRY4 buff1_reg_i_1__0
       (.CI(buff1_reg_i_2__0_n_1),
        .CO({NLW_buff1_reg_i_1__0_CO_UNCONNECTED[3:2],buff1_reg_i_1__0_n_3,buff1_reg_i_1__0_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({NLW_buff1_reg_i_1__0_O_UNCONNECTED[3],grp_fu_125_p1[31:29]}),
        .S({1'b0,buff2_reg__0[31:29]}));
  CARRY4 buff1_reg_i_2__0
       (.CI(buff1_reg_i_3__0_n_1),
        .CO({buff1_reg_i_2__0_n_1,buff1_reg_i_2__0_n_2,buff1_reg_i_2__0_n_3,buff1_reg_i_2__0_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(grp_fu_125_p1[28:25]),
        .S(buff2_reg__0[28:25]));
  CARRY4 buff1_reg_i_3__0
       (.CI(buff1_reg_i_4__0_n_1),
        .CO({buff1_reg_i_3__0_n_1,buff1_reg_i_3__0_n_2,buff1_reg_i_3__0_n_3,buff1_reg_i_3__0_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(grp_fu_125_p1[24:21]),
        .S(buff2_reg__0[24:21]));
  CARRY4 buff1_reg_i_4__0
       (.CI(buff2_reg_i_1_n_1),
        .CO({buff1_reg_i_4__0_n_1,buff1_reg_i_4__0_n_2,buff1_reg_i_4__0_n_3,buff1_reg_i_4__0_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(grp_fu_125_p1[20:17]),
        .S(buff2_reg__0[20:17]));
  DSP48E1 #(
    .ACASCREG(2),
    .ADREG(1),
    .ALUMODEREG(0),
    .AREG(2),
    .AUTORESET_PATDET("NO_RESET"),
    .A_INPUT("DIRECT"),
    .BCASCREG(2),
    .BREG(2),
    .B_INPUT("DIRECT"),
    .CARRYINREG(0),
    .CARRYINSELREG(0),
    .CREG(1),
    .DREG(1),
    .INMODEREG(0),
    .MASK(48'h3FFFFFFFFFFF),
    .MREG(1),
    .OPMODEREG(0),
    .PATTERN(48'h000000000000),
    .PREG(1),
    .SEL_MASK("MASK"),
    .SEL_PATTERN("PATTERN"),
    .USE_DPORT("FALSE"),
    .USE_MULT("MULTIPLY"),
    .USE_PATTERN_DETECT("NO_PATDET"),
    .USE_SIMD("ONE48")) 
    buff2_reg
       (.A({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,grp_fu_113_p1[16:0]}),
        .ACIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .ACOUT(NLW_buff2_reg_ACOUT_UNCONNECTED[29:0]),
        .ALUMODE({1'b0,1'b0,1'b0,1'b0}),
        .B({buff2_reg_0[14],buff2_reg_0[14],buff2_reg_0[14],buff2_reg_0}),
        .BCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .BCOUT(NLW_buff2_reg_BCOUT_UNCONNECTED[17:0]),
        .C({1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1}),
        .CARRYCASCIN(1'b0),
        .CARRYCASCOUT(NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED),
        .CARRYIN(1'b0),
        .CARRYINSEL({1'b0,1'b0,1'b0}),
        .CARRYOUT(NLW_buff2_reg_CARRYOUT_UNCONNECTED[3:0]),
        .CEA1(1'b1),
        .CEA2(1'b1),
        .CEAD(1'b0),
        .CEALUMODE(1'b0),
        .CEB1(1'b1),
        .CEB2(1'b1),
        .CEC(1'b0),
        .CECARRYIN(1'b0),
        .CECTRL(1'b0),
        .CED(1'b0),
        .CEINMODE(1'b0),
        .CEM(1'b1),
        .CEP(1'b1),
        .CLK(ap_clk),
        .D({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .INMODE({1'b0,1'b0,1'b0,1'b0,1'b0}),
        .MULTSIGNIN(1'b0),
        .MULTSIGNOUT(NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED),
        .OPMODE({1'b0,1'b0,1'b1,1'b0,1'b1,1'b0,1'b1}),
        .OVERFLOW(NLW_buff2_reg_OVERFLOW_UNCONNECTED),
        .P({buff2_reg_n_59,buff2_reg_n_60,buff2_reg_n_61,buff2_reg_n_62,buff2_reg_n_63,buff2_reg_n_64,buff2_reg_n_65,buff2_reg_n_66,buff2_reg_n_67,buff2_reg_n_68,buff2_reg_n_69,buff2_reg_n_70,buff2_reg_n_71,buff2_reg_n_72,buff2_reg_n_73,buff2_reg_n_74,buff2_reg_n_75,buff2_reg_n_76,buff2_reg_n_77,buff2_reg_n_78,buff2_reg_n_79,buff2_reg_n_80,buff2_reg_n_81,buff2_reg_n_82,buff2_reg_n_83,buff2_reg_n_84,buff2_reg_n_85,buff2_reg_n_86,buff2_reg_n_87,buff2_reg_n_88,buff2_reg_n_89,buff2_reg_n_90,buff2_reg_n_91,buff2_reg__0[31:17]}),
        .PATTERNBDETECT(NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED),
        .PATTERNDETECT(NLW_buff2_reg_PATTERNDETECT_UNCONNECTED),
        .PCIN({buff1_reg_n_107,buff1_reg_n_108,buff1_reg_n_109,buff1_reg_n_110,buff1_reg_n_111,buff1_reg_n_112,buff1_reg_n_113,buff1_reg_n_114,buff1_reg_n_115,buff1_reg_n_116,buff1_reg_n_117,buff1_reg_n_118,buff1_reg_n_119,buff1_reg_n_120,buff1_reg_n_121,buff1_reg_n_122,buff1_reg_n_123,buff1_reg_n_124,buff1_reg_n_125,buff1_reg_n_126,buff1_reg_n_127,buff1_reg_n_128,buff1_reg_n_129,buff1_reg_n_130,buff1_reg_n_131,buff1_reg_n_132,buff1_reg_n_133,buff1_reg_n_134,buff1_reg_n_135,buff1_reg_n_136,buff1_reg_n_137,buff1_reg_n_138,buff1_reg_n_139,buff1_reg_n_140,buff1_reg_n_141,buff1_reg_n_142,buff1_reg_n_143,buff1_reg_n_144,buff1_reg_n_145,buff1_reg_n_146,buff1_reg_n_147,buff1_reg_n_148,buff1_reg_n_149,buff1_reg_n_150,buff1_reg_n_151,buff1_reg_n_152,buff1_reg_n_153,buff1_reg_n_154}),
        .PCOUT(NLW_buff2_reg_PCOUT_UNCONNECTED[47:0]),
        .RSTA(1'b0),
        .RSTALLCARRYIN(1'b0),
        .RSTALUMODE(1'b0),
        .RSTB(1'b0),
        .RSTC(1'b0),
        .RSTCTRL(1'b0),
        .RSTD(1'b0),
        .RSTINMODE(1'b0),
        .RSTM(1'b0),
        .RSTP(1'b0),
        .UNDERFLOW(NLW_buff2_reg_UNDERFLOW_UNCONNECTED));
  FDRE \buff2_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[0] ),
        .Q(buff2_reg__0[0]),
        .R(1'b0));
  FDRE \buff2_reg[10] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[10] ),
        .Q(buff2_reg__0[10]),
        .R(1'b0));
  FDRE \buff2_reg[11] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[11] ),
        .Q(buff2_reg__0[11]),
        .R(1'b0));
  FDRE \buff2_reg[12] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[12] ),
        .Q(buff2_reg__0[12]),
        .R(1'b0));
  FDRE \buff2_reg[13] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[13] ),
        .Q(buff2_reg__0[13]),
        .R(1'b0));
  FDRE \buff2_reg[14] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[14] ),
        .Q(buff2_reg__0[14]),
        .R(1'b0));
  FDRE \buff2_reg[15] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[15] ),
        .Q(buff2_reg__0[15]),
        .R(1'b0));
  FDRE \buff2_reg[16] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[16] ),
        .Q(buff2_reg__0[16]),
        .R(1'b0));
  FDRE \buff2_reg[1] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[1] ),
        .Q(buff2_reg__0[1]),
        .R(1'b0));
  FDRE \buff2_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[2] ),
        .Q(buff2_reg__0[2]),
        .R(1'b0));
  FDRE \buff2_reg[3] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[3] ),
        .Q(buff2_reg__0[3]),
        .R(1'b0));
  FDRE \buff2_reg[4] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[4] ),
        .Q(buff2_reg__0[4]),
        .R(1'b0));
  FDRE \buff2_reg[5] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[5] ),
        .Q(buff2_reg__0[5]),
        .R(1'b0));
  FDRE \buff2_reg[6] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[6] ),
        .Q(buff2_reg__0[6]),
        .R(1'b0));
  FDRE \buff2_reg[7] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[7] ),
        .Q(buff2_reg__0[7]),
        .R(1'b0));
  FDRE \buff2_reg[8] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[8] ),
        .Q(buff2_reg__0[8]),
        .R(1'b0));
  FDRE \buff2_reg[9] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(\buff1_reg_n_1_[9] ),
        .Q(buff2_reg__0[9]),
        .R(1'b0));
  CARRY4 buff2_reg_i_1
       (.CI(buff2_reg_i_2_n_1),
        .CO({buff2_reg_i_1_n_1,buff2_reg_i_1_n_2,buff2_reg_i_1_n_3,buff2_reg_i_1_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(grp_fu_125_p1[16:13]),
        .S(buff2_reg__0[16:13]));
  CARRY4 buff2_reg_i_2
       (.CI(buff2_reg_i_3_n_1),
        .CO({buff2_reg_i_2_n_1,buff2_reg_i_2_n_2,buff2_reg_i_2_n_3,buff2_reg_i_2_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(grp_fu_125_p1[12:9]),
        .S(buff2_reg__0[12:9]));
  CARRY4 buff2_reg_i_3
       (.CI(buff2_reg_i_4_n_1),
        .CO({buff2_reg_i_3_n_1,buff2_reg_i_3_n_2,buff2_reg_i_3_n_3,buff2_reg_i_3_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(grp_fu_125_p1[8:5]),
        .S(buff2_reg__0[8:5]));
  CARRY4 buff2_reg_i_4
       (.CI(1'b0),
        .CO({buff2_reg_i_4_n_1,buff2_reg_i_4_n_2,buff2_reg_i_4_n_3,buff2_reg_i_4_n_4}),
        .CYINIT(buff2_reg__0[0]),
        .DI({1'b0,1'b0,1'b0,buff2_reg__0[1]}),
        .O(grp_fu_125_p1[4:1]),
        .S({buff2_reg__0[4:2],buff2_reg_i_6_n_1}));
  LUT1 #(
    .INIT(2'h1)) 
    buff2_reg_i_5
       (.I0(buff2_reg__0[0]),
        .O(grp_fu_125_p1[0]));
  LUT1 #(
    .INIT(2'h1)) 
    buff2_reg_i_6
       (.I0(buff2_reg__0[1]),
        .O(buff2_reg_i_6_n_1));
endmodule

(* ORIG_REF_NAME = "loop_imperfect_mubkb_MulnS_0" *) 
module bd_0_hls_inst_0_loop_imperfect_mubkb_MulnS_0_3
   (buff2_reg_0,
    Q,
    grp_fu_113_p1,
    ap_clk,
    A_q0);
  output [14:0]buff2_reg_0;
  output [16:0]Q;
  output [31:0]grp_fu_113_p1;
  input ap_clk;
  input [31:0]A_q0;

  wire [31:0]A_q0;
  wire [16:0]Q;
  wire ap_clk;
  wire buff0_reg_i_1_n_1;
  wire buff0_reg_i_1_n_2;
  wire buff0_reg_i_1_n_3;
  wire buff0_reg_i_1_n_4;
  wire buff0_reg_i_2_n_1;
  wire buff0_reg_i_2_n_2;
  wire buff0_reg_i_2_n_3;
  wire buff0_reg_i_2_n_4;
  wire buff0_reg_i_3_n_1;
  wire buff0_reg_i_3_n_2;
  wire buff0_reg_i_3_n_3;
  wire buff0_reg_i_3_n_4;
  wire buff0_reg_i_4_n_1;
  wire buff0_reg_i_4_n_2;
  wire buff0_reg_i_4_n_3;
  wire buff0_reg_i_4_n_4;
  wire buff0_reg_i_6_n_1;
  wire buff0_reg_i_7_n_1;
  wire buff0_reg_n_100;
  wire buff0_reg_n_101;
  wire buff0_reg_n_102;
  wire buff0_reg_n_103;
  wire buff0_reg_n_104;
  wire buff0_reg_n_105;
  wire buff0_reg_n_106;
  wire buff0_reg_n_107;
  wire buff0_reg_n_108;
  wire buff0_reg_n_109;
  wire buff0_reg_n_110;
  wire buff0_reg_n_111;
  wire buff0_reg_n_112;
  wire buff0_reg_n_113;
  wire buff0_reg_n_114;
  wire buff0_reg_n_115;
  wire buff0_reg_n_116;
  wire buff0_reg_n_117;
  wire buff0_reg_n_118;
  wire buff0_reg_n_119;
  wire buff0_reg_n_120;
  wire buff0_reg_n_121;
  wire buff0_reg_n_122;
  wire buff0_reg_n_123;
  wire buff0_reg_n_124;
  wire buff0_reg_n_125;
  wire buff0_reg_n_126;
  wire buff0_reg_n_127;
  wire buff0_reg_n_128;
  wire buff0_reg_n_129;
  wire buff0_reg_n_130;
  wire buff0_reg_n_131;
  wire buff0_reg_n_132;
  wire buff0_reg_n_133;
  wire buff0_reg_n_134;
  wire buff0_reg_n_135;
  wire buff0_reg_n_136;
  wire buff0_reg_n_137;
  wire buff0_reg_n_138;
  wire buff0_reg_n_139;
  wire buff0_reg_n_140;
  wire buff0_reg_n_141;
  wire buff0_reg_n_142;
  wire buff0_reg_n_143;
  wire buff0_reg_n_144;
  wire buff0_reg_n_145;
  wire buff0_reg_n_146;
  wire buff0_reg_n_147;
  wire buff0_reg_n_148;
  wire buff0_reg_n_149;
  wire buff0_reg_n_150;
  wire buff0_reg_n_151;
  wire buff0_reg_n_152;
  wire buff0_reg_n_153;
  wire buff0_reg_n_154;
  wire buff0_reg_n_59;
  wire buff0_reg_n_60;
  wire buff0_reg_n_61;
  wire buff0_reg_n_62;
  wire buff0_reg_n_63;
  wire buff0_reg_n_64;
  wire buff0_reg_n_65;
  wire buff0_reg_n_66;
  wire buff0_reg_n_67;
  wire buff0_reg_n_68;
  wire buff0_reg_n_69;
  wire buff0_reg_n_70;
  wire buff0_reg_n_71;
  wire buff0_reg_n_72;
  wire buff0_reg_n_73;
  wire buff0_reg_n_74;
  wire buff0_reg_n_75;
  wire buff0_reg_n_76;
  wire buff0_reg_n_77;
  wire buff0_reg_n_78;
  wire buff0_reg_n_79;
  wire buff0_reg_n_80;
  wire buff0_reg_n_81;
  wire buff0_reg_n_82;
  wire buff0_reg_n_83;
  wire buff0_reg_n_84;
  wire buff0_reg_n_85;
  wire buff0_reg_n_86;
  wire buff0_reg_n_87;
  wire buff0_reg_n_88;
  wire buff0_reg_n_89;
  wire buff0_reg_n_90;
  wire buff0_reg_n_91;
  wire buff0_reg_n_92;
  wire buff0_reg_n_93;
  wire buff0_reg_n_94;
  wire buff0_reg_n_95;
  wire buff0_reg_n_96;
  wire buff0_reg_n_97;
  wire buff0_reg_n_98;
  wire buff0_reg_n_99;
  wire buff1_reg_i_1_n_3;
  wire buff1_reg_i_1_n_4;
  wire buff1_reg_i_2_n_1;
  wire buff1_reg_i_2_n_2;
  wire buff1_reg_i_2_n_3;
  wire buff1_reg_i_2_n_4;
  wire buff1_reg_i_3_n_1;
  wire buff1_reg_i_3_n_2;
  wire buff1_reg_i_3_n_3;
  wire buff1_reg_i_3_n_4;
  wire buff1_reg_i_4_n_1;
  wire buff1_reg_i_4_n_2;
  wire buff1_reg_i_4_n_3;
  wire buff1_reg_i_4_n_4;
  wire buff1_reg_n_107;
  wire buff1_reg_n_108;
  wire buff1_reg_n_109;
  wire buff1_reg_n_110;
  wire buff1_reg_n_111;
  wire buff1_reg_n_112;
  wire buff1_reg_n_113;
  wire buff1_reg_n_114;
  wire buff1_reg_n_115;
  wire buff1_reg_n_116;
  wire buff1_reg_n_117;
  wire buff1_reg_n_118;
  wire buff1_reg_n_119;
  wire buff1_reg_n_120;
  wire buff1_reg_n_121;
  wire buff1_reg_n_122;
  wire buff1_reg_n_123;
  wire buff1_reg_n_124;
  wire buff1_reg_n_125;
  wire buff1_reg_n_126;
  wire buff1_reg_n_127;
  wire buff1_reg_n_128;
  wire buff1_reg_n_129;
  wire buff1_reg_n_130;
  wire buff1_reg_n_131;
  wire buff1_reg_n_132;
  wire buff1_reg_n_133;
  wire buff1_reg_n_134;
  wire buff1_reg_n_135;
  wire buff1_reg_n_136;
  wire buff1_reg_n_137;
  wire buff1_reg_n_138;
  wire buff1_reg_n_139;
  wire buff1_reg_n_140;
  wire buff1_reg_n_141;
  wire buff1_reg_n_142;
  wire buff1_reg_n_143;
  wire buff1_reg_n_144;
  wire buff1_reg_n_145;
  wire buff1_reg_n_146;
  wire buff1_reg_n_147;
  wire buff1_reg_n_148;
  wire buff1_reg_n_149;
  wire buff1_reg_n_150;
  wire buff1_reg_n_151;
  wire buff1_reg_n_152;
  wire buff1_reg_n_153;
  wire buff1_reg_n_154;
  wire [14:0]buff2_reg_0;
  wire [16:0]buff2_reg__0;
  wire buff2_reg_n_59;
  wire buff2_reg_n_60;
  wire buff2_reg_n_61;
  wire buff2_reg_n_62;
  wire buff2_reg_n_63;
  wire buff2_reg_n_64;
  wire buff2_reg_n_65;
  wire buff2_reg_n_66;
  wire buff2_reg_n_67;
  wire buff2_reg_n_68;
  wire buff2_reg_n_69;
  wire buff2_reg_n_70;
  wire buff2_reg_n_71;
  wire buff2_reg_n_72;
  wire buff2_reg_n_73;
  wire buff2_reg_n_74;
  wire buff2_reg_n_75;
  wire buff2_reg_n_76;
  wire buff2_reg_n_77;
  wire buff2_reg_n_78;
  wire buff2_reg_n_79;
  wire buff2_reg_n_80;
  wire buff2_reg_n_81;
  wire buff2_reg_n_82;
  wire buff2_reg_n_83;
  wire buff2_reg_n_84;
  wire buff2_reg_n_85;
  wire buff2_reg_n_86;
  wire buff2_reg_n_87;
  wire buff2_reg_n_88;
  wire buff2_reg_n_89;
  wire buff2_reg_n_90;
  wire buff2_reg_n_91;
  wire [31:0]grp_fu_113_p1;
  wire NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED;
  wire NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED;
  wire NLW_buff0_reg_OVERFLOW_UNCONNECTED;
  wire NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED;
  wire NLW_buff0_reg_PATTERNDETECT_UNCONNECTED;
  wire NLW_buff0_reg_UNDERFLOW_UNCONNECTED;
  wire [29:0]NLW_buff0_reg_ACOUT_UNCONNECTED;
  wire [17:0]NLW_buff0_reg_BCOUT_UNCONNECTED;
  wire [3:0]NLW_buff0_reg_CARRYOUT_UNCONNECTED;
  wire NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED;
  wire NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED;
  wire NLW_buff1_reg_OVERFLOW_UNCONNECTED;
  wire NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED;
  wire NLW_buff1_reg_PATTERNDETECT_UNCONNECTED;
  wire NLW_buff1_reg_UNDERFLOW_UNCONNECTED;
  wire [29:0]NLW_buff1_reg_ACOUT_UNCONNECTED;
  wire [17:0]NLW_buff1_reg_BCOUT_UNCONNECTED;
  wire [3:0]NLW_buff1_reg_CARRYOUT_UNCONNECTED;
  wire [47:0]NLW_buff1_reg_P_UNCONNECTED;
  wire [3:2]NLW_buff1_reg_i_1_CO_UNCONNECTED;
  wire [3:3]NLW_buff1_reg_i_1_O_UNCONNECTED;
  wire NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED;
  wire NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED;
  wire NLW_buff2_reg_OVERFLOW_UNCONNECTED;
  wire NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED;
  wire NLW_buff2_reg_PATTERNDETECT_UNCONNECTED;
  wire NLW_buff2_reg_UNDERFLOW_UNCONNECTED;
  wire [29:0]NLW_buff2_reg_ACOUT_UNCONNECTED;
  wire [17:0]NLW_buff2_reg_BCOUT_UNCONNECTED;
  wire [3:0]NLW_buff2_reg_CARRYOUT_UNCONNECTED;
  wire [47:0]NLW_buff2_reg_PCOUT_UNCONNECTED;

  DSP48E1 #(
    .ACASCREG(1),
    .ADREG(1),
    .ALUMODEREG(0),
    .AREG(1),
    .AUTORESET_PATDET("NO_RESET"),
    .A_INPUT("DIRECT"),
    .BCASCREG(1),
    .BREG(1),
    .B_INPUT("DIRECT"),
    .CARRYINREG(0),
    .CARRYINSELREG(0),
    .CREG(0),
    .DREG(1),
    .INMODEREG(0),
    .MASK(48'h3FFFFFFFFFFF),
    .MREG(0),
    .OPMODEREG(0),
    .PATTERN(48'h000000000000),
    .PREG(1),
    .SEL_MASK("MASK"),
    .SEL_PATTERN("PATTERN"),
    .USE_DPORT("FALSE"),
    .USE_MULT("MULTIPLY"),
    .USE_PATTERN_DETECT("NO_PATDET"),
    .USE_SIMD("ONE48")) 
    buff0_reg
       (.A({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,A_q0[16:0]}),
        .ACIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .ACOUT(NLW_buff0_reg_ACOUT_UNCONNECTED[29:0]),
        .ALUMODE({1'b0,1'b0,1'b0,1'b0}),
        .B({1'b0,A_q0[16:0]}),
        .BCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .BCOUT(NLW_buff0_reg_BCOUT_UNCONNECTED[17:0]),
        .C({1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1}),
        .CARRYCASCIN(1'b0),
        .CARRYCASCOUT(NLW_buff0_reg_CARRYCASCOUT_UNCONNECTED),
        .CARRYIN(1'b0),
        .CARRYINSEL({1'b0,1'b0,1'b0}),
        .CARRYOUT(NLW_buff0_reg_CARRYOUT_UNCONNECTED[3:0]),
        .CEA1(1'b0),
        .CEA2(1'b1),
        .CEAD(1'b0),
        .CEALUMODE(1'b0),
        .CEB1(1'b0),
        .CEB2(1'b1),
        .CEC(1'b0),
        .CECARRYIN(1'b0),
        .CECTRL(1'b0),
        .CED(1'b0),
        .CEINMODE(1'b0),
        .CEM(1'b0),
        .CEP(1'b1),
        .CLK(ap_clk),
        .D({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .INMODE({1'b0,1'b0,1'b0,1'b0,1'b0}),
        .MULTSIGNIN(1'b0),
        .MULTSIGNOUT(NLW_buff0_reg_MULTSIGNOUT_UNCONNECTED),
        .OPMODE({1'b0,1'b0,1'b0,1'b0,1'b1,1'b0,1'b1}),
        .OVERFLOW(NLW_buff0_reg_OVERFLOW_UNCONNECTED),
        .P({buff0_reg_n_59,buff0_reg_n_60,buff0_reg_n_61,buff0_reg_n_62,buff0_reg_n_63,buff0_reg_n_64,buff0_reg_n_65,buff0_reg_n_66,buff0_reg_n_67,buff0_reg_n_68,buff0_reg_n_69,buff0_reg_n_70,buff0_reg_n_71,buff0_reg_n_72,buff0_reg_n_73,buff0_reg_n_74,buff0_reg_n_75,buff0_reg_n_76,buff0_reg_n_77,buff0_reg_n_78,buff0_reg_n_79,buff0_reg_n_80,buff0_reg_n_81,buff0_reg_n_82,buff0_reg_n_83,buff0_reg_n_84,buff0_reg_n_85,buff0_reg_n_86,buff0_reg_n_87,buff0_reg_n_88,buff0_reg_n_89,buff0_reg_n_90,buff0_reg_n_91,buff0_reg_n_92,buff0_reg_n_93,buff0_reg_n_94,buff0_reg_n_95,buff0_reg_n_96,buff0_reg_n_97,buff0_reg_n_98,buff0_reg_n_99,buff0_reg_n_100,buff0_reg_n_101,buff0_reg_n_102,buff0_reg_n_103,buff0_reg_n_104,buff0_reg_n_105,buff0_reg_n_106}),
        .PATTERNBDETECT(NLW_buff0_reg_PATTERNBDETECT_UNCONNECTED),
        .PATTERNDETECT(NLW_buff0_reg_PATTERNDETECT_UNCONNECTED),
        .PCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .PCOUT({buff0_reg_n_107,buff0_reg_n_108,buff0_reg_n_109,buff0_reg_n_110,buff0_reg_n_111,buff0_reg_n_112,buff0_reg_n_113,buff0_reg_n_114,buff0_reg_n_115,buff0_reg_n_116,buff0_reg_n_117,buff0_reg_n_118,buff0_reg_n_119,buff0_reg_n_120,buff0_reg_n_121,buff0_reg_n_122,buff0_reg_n_123,buff0_reg_n_124,buff0_reg_n_125,buff0_reg_n_126,buff0_reg_n_127,buff0_reg_n_128,buff0_reg_n_129,buff0_reg_n_130,buff0_reg_n_131,buff0_reg_n_132,buff0_reg_n_133,buff0_reg_n_134,buff0_reg_n_135,buff0_reg_n_136,buff0_reg_n_137,buff0_reg_n_138,buff0_reg_n_139,buff0_reg_n_140,buff0_reg_n_141,buff0_reg_n_142,buff0_reg_n_143,buff0_reg_n_144,buff0_reg_n_145,buff0_reg_n_146,buff0_reg_n_147,buff0_reg_n_148,buff0_reg_n_149,buff0_reg_n_150,buff0_reg_n_151,buff0_reg_n_152,buff0_reg_n_153,buff0_reg_n_154}),
        .RSTA(1'b0),
        .RSTALLCARRYIN(1'b0),
        .RSTALUMODE(1'b0),
        .RSTB(1'b0),
        .RSTC(1'b0),
        .RSTCTRL(1'b0),
        .RSTD(1'b0),
        .RSTINMODE(1'b0),
        .RSTM(1'b0),
        .RSTP(1'b0),
        .UNDERFLOW(NLW_buff0_reg_UNDERFLOW_UNCONNECTED));
  CARRY4 buff0_reg_i_1
       (.CI(buff0_reg_i_2_n_1),
        .CO({buff0_reg_i_1_n_1,buff0_reg_i_1_n_2,buff0_reg_i_1_n_3,buff0_reg_i_1_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(grp_fu_113_p1[16:13]),
        .S(buff2_reg__0[16:13]));
  CARRY4 buff0_reg_i_2
       (.CI(buff0_reg_i_3_n_1),
        .CO({buff0_reg_i_2_n_1,buff0_reg_i_2_n_2,buff0_reg_i_2_n_3,buff0_reg_i_2_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(grp_fu_113_p1[12:9]),
        .S(buff2_reg__0[12:9]));
  CARRY4 buff0_reg_i_3
       (.CI(buff0_reg_i_4_n_1),
        .CO({buff0_reg_i_3_n_1,buff0_reg_i_3_n_2,buff0_reg_i_3_n_3,buff0_reg_i_3_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(grp_fu_113_p1[8:5]),
        .S(buff2_reg__0[8:5]));
  CARRY4 buff0_reg_i_4
       (.CI(1'b0),
        .CO({buff0_reg_i_4_n_1,buff0_reg_i_4_n_2,buff0_reg_i_4_n_3,buff0_reg_i_4_n_4}),
        .CYINIT(buff2_reg__0[0]),
        .DI({buff2_reg__0[4],1'b0,1'b0,buff2_reg__0[1]}),
        .O(grp_fu_113_p1[4:1]),
        .S({buff0_reg_i_6_n_1,buff2_reg__0[3:2],buff0_reg_i_7_n_1}));
  LUT1 #(
    .INIT(2'h1)) 
    buff0_reg_i_5
       (.I0(buff2_reg__0[0]),
        .O(grp_fu_113_p1[0]));
  LUT1 #(
    .INIT(2'h1)) 
    buff0_reg_i_6
       (.I0(buff2_reg__0[4]),
        .O(buff0_reg_i_6_n_1));
  LUT1 #(
    .INIT(2'h1)) 
    buff0_reg_i_7
       (.I0(buff2_reg__0[1]),
        .O(buff0_reg_i_7_n_1));
  DSP48E1 #(
    .ACASCREG(1),
    .ADREG(1),
    .ALUMODEREG(0),
    .AREG(1),
    .AUTORESET_PATDET("NO_RESET"),
    .A_INPUT("DIRECT"),
    .BCASCREG(1),
    .BREG(1),
    .B_INPUT("DIRECT"),
    .CARRYINREG(0),
    .CARRYINSELREG(0),
    .CREG(1),
    .DREG(1),
    .INMODEREG(0),
    .MASK(48'h3FFFFFFFFFFF),
    .MREG(1),
    .OPMODEREG(0),
    .PATTERN(48'h000000000000),
    .PREG(1),
    .SEL_MASK("MASK"),
    .SEL_PATTERN("PATTERN"),
    .USE_DPORT("FALSE"),
    .USE_MULT("MULTIPLY"),
    .USE_PATTERN_DETECT("NO_PATDET"),
    .USE_SIMD("ONE48")) 
    buff1_reg
       (.A({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,A_q0[16:0]}),
        .ACIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .ACOUT(NLW_buff1_reg_ACOUT_UNCONNECTED[29:0]),
        .ALUMODE({1'b0,1'b0,1'b0,1'b0}),
        .B({A_q0[31],A_q0[31],A_q0[31],A_q0[31:17]}),
        .BCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .BCOUT(NLW_buff1_reg_BCOUT_UNCONNECTED[17:0]),
        .C({1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1}),
        .CARRYCASCIN(1'b0),
        .CARRYCASCOUT(NLW_buff1_reg_CARRYCASCOUT_UNCONNECTED),
        .CARRYIN(1'b0),
        .CARRYINSEL({1'b0,1'b0,1'b0}),
        .CARRYOUT(NLW_buff1_reg_CARRYOUT_UNCONNECTED[3:0]),
        .CEA1(1'b0),
        .CEA2(1'b1),
        .CEAD(1'b0),
        .CEALUMODE(1'b0),
        .CEB1(1'b0),
        .CEB2(1'b1),
        .CEC(1'b0),
        .CECARRYIN(1'b0),
        .CECTRL(1'b0),
        .CED(1'b0),
        .CEINMODE(1'b0),
        .CEM(1'b1),
        .CEP(1'b1),
        .CLK(ap_clk),
        .D({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .INMODE({1'b0,1'b0,1'b0,1'b0,1'b0}),
        .MULTSIGNIN(1'b0),
        .MULTSIGNOUT(NLW_buff1_reg_MULTSIGNOUT_UNCONNECTED),
        .OPMODE({1'b1,1'b0,1'b1,1'b0,1'b1,1'b0,1'b1}),
        .OVERFLOW(NLW_buff1_reg_OVERFLOW_UNCONNECTED),
        .P(NLW_buff1_reg_P_UNCONNECTED[47:0]),
        .PATTERNBDETECT(NLW_buff1_reg_PATTERNBDETECT_UNCONNECTED),
        .PATTERNDETECT(NLW_buff1_reg_PATTERNDETECT_UNCONNECTED),
        .PCIN({buff0_reg_n_107,buff0_reg_n_108,buff0_reg_n_109,buff0_reg_n_110,buff0_reg_n_111,buff0_reg_n_112,buff0_reg_n_113,buff0_reg_n_114,buff0_reg_n_115,buff0_reg_n_116,buff0_reg_n_117,buff0_reg_n_118,buff0_reg_n_119,buff0_reg_n_120,buff0_reg_n_121,buff0_reg_n_122,buff0_reg_n_123,buff0_reg_n_124,buff0_reg_n_125,buff0_reg_n_126,buff0_reg_n_127,buff0_reg_n_128,buff0_reg_n_129,buff0_reg_n_130,buff0_reg_n_131,buff0_reg_n_132,buff0_reg_n_133,buff0_reg_n_134,buff0_reg_n_135,buff0_reg_n_136,buff0_reg_n_137,buff0_reg_n_138,buff0_reg_n_139,buff0_reg_n_140,buff0_reg_n_141,buff0_reg_n_142,buff0_reg_n_143,buff0_reg_n_144,buff0_reg_n_145,buff0_reg_n_146,buff0_reg_n_147,buff0_reg_n_148,buff0_reg_n_149,buff0_reg_n_150,buff0_reg_n_151,buff0_reg_n_152,buff0_reg_n_153,buff0_reg_n_154}),
        .PCOUT({buff1_reg_n_107,buff1_reg_n_108,buff1_reg_n_109,buff1_reg_n_110,buff1_reg_n_111,buff1_reg_n_112,buff1_reg_n_113,buff1_reg_n_114,buff1_reg_n_115,buff1_reg_n_116,buff1_reg_n_117,buff1_reg_n_118,buff1_reg_n_119,buff1_reg_n_120,buff1_reg_n_121,buff1_reg_n_122,buff1_reg_n_123,buff1_reg_n_124,buff1_reg_n_125,buff1_reg_n_126,buff1_reg_n_127,buff1_reg_n_128,buff1_reg_n_129,buff1_reg_n_130,buff1_reg_n_131,buff1_reg_n_132,buff1_reg_n_133,buff1_reg_n_134,buff1_reg_n_135,buff1_reg_n_136,buff1_reg_n_137,buff1_reg_n_138,buff1_reg_n_139,buff1_reg_n_140,buff1_reg_n_141,buff1_reg_n_142,buff1_reg_n_143,buff1_reg_n_144,buff1_reg_n_145,buff1_reg_n_146,buff1_reg_n_147,buff1_reg_n_148,buff1_reg_n_149,buff1_reg_n_150,buff1_reg_n_151,buff1_reg_n_152,buff1_reg_n_153,buff1_reg_n_154}),
        .RSTA(1'b0),
        .RSTALLCARRYIN(1'b0),
        .RSTALUMODE(1'b0),
        .RSTB(1'b0),
        .RSTC(1'b0),
        .RSTCTRL(1'b0),
        .RSTD(1'b0),
        .RSTINMODE(1'b0),
        .RSTM(1'b0),
        .RSTP(1'b0),
        .UNDERFLOW(NLW_buff1_reg_UNDERFLOW_UNCONNECTED));
  FDRE \buff1_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_106),
        .Q(Q[0]),
        .R(1'b0));
  FDRE \buff1_reg[10] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_96),
        .Q(Q[10]),
        .R(1'b0));
  FDRE \buff1_reg[11] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_95),
        .Q(Q[11]),
        .R(1'b0));
  FDRE \buff1_reg[12] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_94),
        .Q(Q[12]),
        .R(1'b0));
  FDRE \buff1_reg[13] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_93),
        .Q(Q[13]),
        .R(1'b0));
  FDRE \buff1_reg[14] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_92),
        .Q(Q[14]),
        .R(1'b0));
  FDRE \buff1_reg[15] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_91),
        .Q(Q[15]),
        .R(1'b0));
  FDRE \buff1_reg[16] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_90),
        .Q(Q[16]),
        .R(1'b0));
  FDRE \buff1_reg[1] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_105),
        .Q(Q[1]),
        .R(1'b0));
  FDRE \buff1_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_104),
        .Q(Q[2]),
        .R(1'b0));
  FDRE \buff1_reg[3] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_103),
        .Q(Q[3]),
        .R(1'b0));
  FDRE \buff1_reg[4] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_102),
        .Q(Q[4]),
        .R(1'b0));
  FDRE \buff1_reg[5] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_101),
        .Q(Q[5]),
        .R(1'b0));
  FDRE \buff1_reg[6] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_100),
        .Q(Q[6]),
        .R(1'b0));
  FDRE \buff1_reg[7] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_99),
        .Q(Q[7]),
        .R(1'b0));
  FDRE \buff1_reg[8] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_98),
        .Q(Q[8]),
        .R(1'b0));
  FDRE \buff1_reg[9] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(buff0_reg_n_97),
        .Q(Q[9]),
        .R(1'b0));
  CARRY4 buff1_reg_i_1
       (.CI(buff1_reg_i_2_n_1),
        .CO({NLW_buff1_reg_i_1_CO_UNCONNECTED[3:2],buff1_reg_i_1_n_3,buff1_reg_i_1_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O({NLW_buff1_reg_i_1_O_UNCONNECTED[3],grp_fu_113_p1[31:29]}),
        .S({1'b0,buff2_reg_0[14:12]}));
  CARRY4 buff1_reg_i_2
       (.CI(buff1_reg_i_3_n_1),
        .CO({buff1_reg_i_2_n_1,buff1_reg_i_2_n_2,buff1_reg_i_2_n_3,buff1_reg_i_2_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(grp_fu_113_p1[28:25]),
        .S(buff2_reg_0[11:8]));
  CARRY4 buff1_reg_i_3
       (.CI(buff1_reg_i_4_n_1),
        .CO({buff1_reg_i_3_n_1,buff1_reg_i_3_n_2,buff1_reg_i_3_n_3,buff1_reg_i_3_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(grp_fu_113_p1[24:21]),
        .S(buff2_reg_0[7:4]));
  CARRY4 buff1_reg_i_4
       (.CI(buff0_reg_i_1_n_1),
        .CO({buff1_reg_i_4_n_1,buff1_reg_i_4_n_2,buff1_reg_i_4_n_3,buff1_reg_i_4_n_4}),
        .CYINIT(1'b0),
        .DI({1'b0,1'b0,1'b0,1'b0}),
        .O(grp_fu_113_p1[20:17]),
        .S(buff2_reg_0[3:0]));
  DSP48E1 #(
    .ACASCREG(2),
    .ADREG(1),
    .ALUMODEREG(0),
    .AREG(2),
    .AUTORESET_PATDET("NO_RESET"),
    .A_INPUT("DIRECT"),
    .BCASCREG(2),
    .BREG(2),
    .B_INPUT("DIRECT"),
    .CARRYINREG(0),
    .CARRYINSELREG(0),
    .CREG(1),
    .DREG(1),
    .INMODEREG(0),
    .MASK(48'h3FFFFFFFFFFF),
    .MREG(1),
    .OPMODEREG(0),
    .PATTERN(48'h000000000000),
    .PREG(1),
    .SEL_MASK("MASK"),
    .SEL_PATTERN("PATTERN"),
    .USE_DPORT("FALSE"),
    .USE_MULT("MULTIPLY"),
    .USE_PATTERN_DETECT("NO_PATDET"),
    .USE_SIMD("ONE48")) 
    buff2_reg
       (.A({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,A_q0[16:0]}),
        .ACIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .ACOUT(NLW_buff2_reg_ACOUT_UNCONNECTED[29:0]),
        .ALUMODE({1'b0,1'b0,1'b0,1'b0}),
        .B({A_q0[31],A_q0[31],A_q0[31],A_q0[31:17]}),
        .BCIN({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .BCOUT(NLW_buff2_reg_BCOUT_UNCONNECTED[17:0]),
        .C({1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1,1'b1}),
        .CARRYCASCIN(1'b0),
        .CARRYCASCOUT(NLW_buff2_reg_CARRYCASCOUT_UNCONNECTED),
        .CARRYIN(1'b0),
        .CARRYINSEL({1'b0,1'b0,1'b0}),
        .CARRYOUT(NLW_buff2_reg_CARRYOUT_UNCONNECTED[3:0]),
        .CEA1(1'b1),
        .CEA2(1'b1),
        .CEAD(1'b0),
        .CEALUMODE(1'b0),
        .CEB1(1'b1),
        .CEB2(1'b1),
        .CEC(1'b0),
        .CECARRYIN(1'b0),
        .CECTRL(1'b0),
        .CED(1'b0),
        .CEINMODE(1'b0),
        .CEM(1'b1),
        .CEP(1'b1),
        .CLK(ap_clk),
        .D({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .INMODE({1'b0,1'b0,1'b0,1'b0,1'b0}),
        .MULTSIGNIN(1'b0),
        .MULTSIGNOUT(NLW_buff2_reg_MULTSIGNOUT_UNCONNECTED),
        .OPMODE({1'b0,1'b0,1'b1,1'b0,1'b1,1'b0,1'b1}),
        .OVERFLOW(NLW_buff2_reg_OVERFLOW_UNCONNECTED),
        .P({buff2_reg_n_59,buff2_reg_n_60,buff2_reg_n_61,buff2_reg_n_62,buff2_reg_n_63,buff2_reg_n_64,buff2_reg_n_65,buff2_reg_n_66,buff2_reg_n_67,buff2_reg_n_68,buff2_reg_n_69,buff2_reg_n_70,buff2_reg_n_71,buff2_reg_n_72,buff2_reg_n_73,buff2_reg_n_74,buff2_reg_n_75,buff2_reg_n_76,buff2_reg_n_77,buff2_reg_n_78,buff2_reg_n_79,buff2_reg_n_80,buff2_reg_n_81,buff2_reg_n_82,buff2_reg_n_83,buff2_reg_n_84,buff2_reg_n_85,buff2_reg_n_86,buff2_reg_n_87,buff2_reg_n_88,buff2_reg_n_89,buff2_reg_n_90,buff2_reg_n_91,buff2_reg_0}),
        .PATTERNBDETECT(NLW_buff2_reg_PATTERNBDETECT_UNCONNECTED),
        .PATTERNDETECT(NLW_buff2_reg_PATTERNDETECT_UNCONNECTED),
        .PCIN({buff1_reg_n_107,buff1_reg_n_108,buff1_reg_n_109,buff1_reg_n_110,buff1_reg_n_111,buff1_reg_n_112,buff1_reg_n_113,buff1_reg_n_114,buff1_reg_n_115,buff1_reg_n_116,buff1_reg_n_117,buff1_reg_n_118,buff1_reg_n_119,buff1_reg_n_120,buff1_reg_n_121,buff1_reg_n_122,buff1_reg_n_123,buff1_reg_n_124,buff1_reg_n_125,buff1_reg_n_126,buff1_reg_n_127,buff1_reg_n_128,buff1_reg_n_129,buff1_reg_n_130,buff1_reg_n_131,buff1_reg_n_132,buff1_reg_n_133,buff1_reg_n_134,buff1_reg_n_135,buff1_reg_n_136,buff1_reg_n_137,buff1_reg_n_138,buff1_reg_n_139,buff1_reg_n_140,buff1_reg_n_141,buff1_reg_n_142,buff1_reg_n_143,buff1_reg_n_144,buff1_reg_n_145,buff1_reg_n_146,buff1_reg_n_147,buff1_reg_n_148,buff1_reg_n_149,buff1_reg_n_150,buff1_reg_n_151,buff1_reg_n_152,buff1_reg_n_153,buff1_reg_n_154}),
        .PCOUT(NLW_buff2_reg_PCOUT_UNCONNECTED[47:0]),
        .RSTA(1'b0),
        .RSTALLCARRYIN(1'b0),
        .RSTALUMODE(1'b0),
        .RSTB(1'b0),
        .RSTC(1'b0),
        .RSTCTRL(1'b0),
        .RSTD(1'b0),
        .RSTINMODE(1'b0),
        .RSTM(1'b0),
        .RSTP(1'b0),
        .UNDERFLOW(NLW_buff2_reg_UNDERFLOW_UNCONNECTED));
  FDRE \buff2_reg[0] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[0]),
        .Q(buff2_reg__0[0]),
        .R(1'b0));
  FDRE \buff2_reg[10] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[10]),
        .Q(buff2_reg__0[10]),
        .R(1'b0));
  FDRE \buff2_reg[11] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[11]),
        .Q(buff2_reg__0[11]),
        .R(1'b0));
  FDRE \buff2_reg[12] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[12]),
        .Q(buff2_reg__0[12]),
        .R(1'b0));
  FDRE \buff2_reg[13] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[13]),
        .Q(buff2_reg__0[13]),
        .R(1'b0));
  FDRE \buff2_reg[14] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[14]),
        .Q(buff2_reg__0[14]),
        .R(1'b0));
  FDRE \buff2_reg[15] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[15]),
        .Q(buff2_reg__0[15]),
        .R(1'b0));
  FDRE \buff2_reg[16] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[16]),
        .Q(buff2_reg__0[16]),
        .R(1'b0));
  FDRE \buff2_reg[1] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[1]),
        .Q(buff2_reg__0[1]),
        .R(1'b0));
  FDRE \buff2_reg[2] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[2]),
        .Q(buff2_reg__0[2]),
        .R(1'b0));
  FDRE \buff2_reg[3] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[3]),
        .Q(buff2_reg__0[3]),
        .R(1'b0));
  FDRE \buff2_reg[4] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[4]),
        .Q(buff2_reg__0[4]),
        .R(1'b0));
  FDRE \buff2_reg[5] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[5]),
        .Q(buff2_reg__0[5]),
        .R(1'b0));
  FDRE \buff2_reg[6] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[6]),
        .Q(buff2_reg__0[6]),
        .R(1'b0));
  FDRE \buff2_reg[7] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[7]),
        .Q(buff2_reg__0[7]),
        .R(1'b0));
  FDRE \buff2_reg[8] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[8]),
        .Q(buff2_reg__0[8]),
        .R(1'b0));
  FDRE \buff2_reg[9] 
       (.C(ap_clk),
        .CE(1'b1),
        .D(Q[9]),
        .Q(buff2_reg__0[9]),
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
