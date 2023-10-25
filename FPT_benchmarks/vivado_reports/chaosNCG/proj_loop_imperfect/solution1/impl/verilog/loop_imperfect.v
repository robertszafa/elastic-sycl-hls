// ==============================================================
// RTL generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and OpenCL
// Version: 2019.2
// Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
// 
// ===========================================================

`timescale 1 ns / 1 ps 

(* CORE_GENERATION_INFO="loop_imperfect,hls_ip_2019_2,{HLS_INPUT_TYPE=c,HLS_INPUT_FLOAT=0,HLS_INPUT_FIXED=0,HLS_INPUT_PART=xc7k160t-fbg484-1,HLS_INPUT_CLOCK=4.000000,HLS_INPUT_ARCH=others,HLS_SYN_CLOCK=3.364000,HLS_SYN_LAT=82001,HLS_SYN_TPT=none,HLS_SYN_MEM=0,HLS_SYN_DSP=0,HLS_SYN_FF=1312,HLS_SYN_LUT=1326,HLS_VERSION=2019_2}" *)

module loop_imperfect (
        ap_clk,
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
        M_q1
);

parameter    ap_ST_fsm_state1 = 83'd1;
parameter    ap_ST_fsm_state2 = 83'd2;
parameter    ap_ST_fsm_state3 = 83'd4;
parameter    ap_ST_fsm_state4 = 83'd8;
parameter    ap_ST_fsm_state5 = 83'd16;
parameter    ap_ST_fsm_state6 = 83'd32;
parameter    ap_ST_fsm_state7 = 83'd64;
parameter    ap_ST_fsm_state8 = 83'd128;
parameter    ap_ST_fsm_state9 = 83'd256;
parameter    ap_ST_fsm_state10 = 83'd512;
parameter    ap_ST_fsm_state11 = 83'd1024;
parameter    ap_ST_fsm_state12 = 83'd2048;
parameter    ap_ST_fsm_state13 = 83'd4096;
parameter    ap_ST_fsm_state14 = 83'd8192;
parameter    ap_ST_fsm_state15 = 83'd16384;
parameter    ap_ST_fsm_state16 = 83'd32768;
parameter    ap_ST_fsm_state17 = 83'd65536;
parameter    ap_ST_fsm_state18 = 83'd131072;
parameter    ap_ST_fsm_state19 = 83'd262144;
parameter    ap_ST_fsm_state20 = 83'd524288;
parameter    ap_ST_fsm_state21 = 83'd1048576;
parameter    ap_ST_fsm_state22 = 83'd2097152;
parameter    ap_ST_fsm_state23 = 83'd4194304;
parameter    ap_ST_fsm_state24 = 83'd8388608;
parameter    ap_ST_fsm_state25 = 83'd16777216;
parameter    ap_ST_fsm_state26 = 83'd33554432;
parameter    ap_ST_fsm_state27 = 83'd67108864;
parameter    ap_ST_fsm_state28 = 83'd134217728;
parameter    ap_ST_fsm_state29 = 83'd268435456;
parameter    ap_ST_fsm_state30 = 83'd536870912;
parameter    ap_ST_fsm_state31 = 83'd1073741824;
parameter    ap_ST_fsm_state32 = 83'd2147483648;
parameter    ap_ST_fsm_state33 = 83'd4294967296;
parameter    ap_ST_fsm_state34 = 83'd8589934592;
parameter    ap_ST_fsm_state35 = 83'd17179869184;
parameter    ap_ST_fsm_state36 = 83'd34359738368;
parameter    ap_ST_fsm_state37 = 83'd68719476736;
parameter    ap_ST_fsm_state38 = 83'd137438953472;
parameter    ap_ST_fsm_state39 = 83'd274877906944;
parameter    ap_ST_fsm_state40 = 83'd549755813888;
parameter    ap_ST_fsm_state41 = 83'd1099511627776;
parameter    ap_ST_fsm_state42 = 83'd2199023255552;
parameter    ap_ST_fsm_state43 = 83'd4398046511104;
parameter    ap_ST_fsm_state44 = 83'd8796093022208;
parameter    ap_ST_fsm_state45 = 83'd17592186044416;
parameter    ap_ST_fsm_state46 = 83'd35184372088832;
parameter    ap_ST_fsm_state47 = 83'd70368744177664;
parameter    ap_ST_fsm_state48 = 83'd140737488355328;
parameter    ap_ST_fsm_state49 = 83'd281474976710656;
parameter    ap_ST_fsm_state50 = 83'd562949953421312;
parameter    ap_ST_fsm_state51 = 83'd1125899906842624;
parameter    ap_ST_fsm_state52 = 83'd2251799813685248;
parameter    ap_ST_fsm_state53 = 83'd4503599627370496;
parameter    ap_ST_fsm_state54 = 83'd9007199254740992;
parameter    ap_ST_fsm_state55 = 83'd18014398509481984;
parameter    ap_ST_fsm_state56 = 83'd36028797018963968;
parameter    ap_ST_fsm_state57 = 83'd72057594037927936;
parameter    ap_ST_fsm_state58 = 83'd144115188075855872;
parameter    ap_ST_fsm_state59 = 83'd288230376151711744;
parameter    ap_ST_fsm_state60 = 83'd576460752303423488;
parameter    ap_ST_fsm_state61 = 83'd1152921504606846976;
parameter    ap_ST_fsm_state62 = 83'd2305843009213693952;
parameter    ap_ST_fsm_state63 = 83'd4611686018427387904;
parameter    ap_ST_fsm_state64 = 83'd9223372036854775808;
parameter    ap_ST_fsm_state65 = 83'd18446744073709551616;
parameter    ap_ST_fsm_state66 = 83'd36893488147419103232;
parameter    ap_ST_fsm_state67 = 83'd73786976294838206464;
parameter    ap_ST_fsm_state68 = 83'd147573952589676412928;
parameter    ap_ST_fsm_state69 = 83'd295147905179352825856;
parameter    ap_ST_fsm_state70 = 83'd590295810358705651712;
parameter    ap_ST_fsm_state71 = 83'd1180591620717411303424;
parameter    ap_ST_fsm_state72 = 83'd2361183241434822606848;
parameter    ap_ST_fsm_state73 = 83'd4722366482869645213696;
parameter    ap_ST_fsm_state74 = 83'd9444732965739290427392;
parameter    ap_ST_fsm_state75 = 83'd18889465931478580854784;
parameter    ap_ST_fsm_state76 = 83'd37778931862957161709568;
parameter    ap_ST_fsm_state77 = 83'd75557863725914323419136;
parameter    ap_ST_fsm_state78 = 83'd151115727451828646838272;
parameter    ap_ST_fsm_state79 = 83'd302231454903657293676544;
parameter    ap_ST_fsm_state80 = 83'd604462909807314587353088;
parameter    ap_ST_fsm_state81 = 83'd1208925819614629174706176;
parameter    ap_ST_fsm_state82 = 83'd2417851639229258349412352;
parameter    ap_ST_fsm_state83 = 83'd4835703278458516698824704;

input   ap_clk;
input   ap_rst;
input   ap_start;
output   ap_done;
output   ap_idle;
output   ap_ready;
output  [11:0] buffer_r_address0;
output   buffer_r_ce0;
output   buffer_r_we0;
output  [31:0] buffer_r_d0;
input  [31:0] buffer_r_q0;
output  [11:0] buffer_r_address1;
output   buffer_r_ce1;
output   buffer_r_we1;
output  [31:0] buffer_r_d1;
input  [31:0] buffer_r_q1;
output  [11:0] M_address0;
output   M_ce0;
input  [31:0] M_q0;
output  [11:0] M_address1;
output   M_ce1;
input  [31:0] M_q1;

reg ap_done;
reg ap_idle;
reg ap_ready;
reg buffer_r_ce0;
reg buffer_r_we0;
reg[11:0] buffer_r_address1;
reg buffer_r_ce1;
reg buffer_r_we1;
reg[11:0] M_address0;
reg M_ce0;
reg M_ce1;

(* fsm_encoding = "none" *) reg   [82:0] ap_CS_fsm;
wire    ap_CS_fsm_state1;
reg   [31:0] reg_123;
wire    ap_CS_fsm_state4;
wire    ap_CS_fsm_state82;
wire   [10:0] add_ln101_fu_134_p2;
reg   [10:0] add_ln101_reg_305;
wire    ap_CS_fsm_state2;
wire   [0:0] icmp_ln100_fu_128_p2;
wire    ap_CS_fsm_state3;
reg   [11:0] buffer_addr_reg_320;
wire    ap_CS_fsm_state5;
reg   [31:0] b0_reg_330;
wire    ap_CS_fsm_state6;
reg   [31:0] b1_reg_336;
wire   [31:0] b0_1_fu_159_p2;
reg   [31:0] b0_1_reg_341;
wire    ap_CS_fsm_state7;
wire   [31:0] grp_fu_163_p2;
reg   [31:0] srem_ln105_reg_347;
wire    ap_CS_fsm_state42;
wire   [5:0] trunc_ln105_fu_169_p1;
reg   [5:0] trunc_ln105_reg_352;
wire   [31:0] shl_ln105_fu_173_p2;
reg   [31:0] shl_ln105_reg_357;
wire    ap_CS_fsm_state43;
wire   [5:0] sub_ln105_fu_178_p2;
reg   [5:0] sub_ln105_reg_362;
reg   [25:0] tmp_reg_367;
wire   [31:0] b0_2_fu_214_p2;
reg   [31:0] b0_2_reg_372;
wire    ap_CS_fsm_state44;
wire   [31:0] b1_1_fu_219_p2;
reg   [31:0] b1_1_reg_377;
wire   [31:0] grp_fu_224_p2;
reg   [31:0] srem_ln108_reg_383;
wire    ap_CS_fsm_state80;
wire   [5:0] trunc_ln108_fu_229_p1;
reg   [5:0] trunc_ln108_reg_388;
wire   [31:0] shl_ln108_fu_233_p2;
reg   [31:0] shl_ln108_reg_393;
wire    ap_CS_fsm_state81;
wire   [10:0] i_fu_249_p2;
reg   [10:0] i_reg_404;
wire   [31:0] b1_2_fu_292_p2;
reg   [31:0] b1_2_reg_409;
reg   [10:0] i_0_reg_111;
wire    ap_CS_fsm_state83;
wire   [63:0] zext_ln101_fu_140_p1;
wire   [63:0] zext_ln101_1_fu_145_p1;
wire  signed [63:0] sext_ln102_fu_149_p1;
wire  signed [63:0] sext_ln102_1_fu_154_p1;
wire   [63:0] zext_ln111_fu_244_p1;
wire  signed [63:0] sext_ln111_fu_297_p1;
wire    ap_CS_fsm_state45;
wire   [31:0] grp_fu_163_p0;
wire   [5:0] grp_fu_163_p1;
wire   [5:0] trunc_ln105_1_fu_198_p1;
wire   [5:0] lshr_ln105_fu_193_p2;
wire   [5:0] or_ln105_fu_201_p2;
wire   [31:0] or_ln_fu_207_p3;
wire   [5:0] grp_fu_224_p1;
wire   [10:0] or_ln111_fu_238_p2;
wire   [5:0] sub_ln108_fu_255_p2;
wire   [5:0] trunc_ln108_1_fu_266_p1;
wire   [5:0] lshr_ln108_fu_260_p2;
wire   [25:0] tmp_1_fu_275_p4;
wire   [5:0] or_ln108_fu_269_p2;
wire   [31:0] or_ln1_fu_284_p3;
reg    grp_fu_163_ap_start;
wire    grp_fu_163_ap_done;
reg    grp_fu_224_ap_start;
wire    grp_fu_224_ap_done;
reg   [82:0] ap_NS_fsm;

// power-on initialization
initial begin
#0 ap_CS_fsm = 83'd1;
end

loop_imperfect_srbkb #(
    .ID( 1 ),
    .NUM_STAGE( 36 ),
    .din0_WIDTH( 32 ),
    .din1_WIDTH( 6 ),
    .dout_WIDTH( 32 ))
loop_imperfect_srbkb_U1(
    .clk(ap_clk),
    .reset(ap_rst),
    .start(grp_fu_163_ap_start),
    .done(grp_fu_163_ap_done),
    .din0(grp_fu_163_p0),
    .din1(grp_fu_163_p1),
    .ce(1'b1),
    .dout(grp_fu_163_p2)
);

loop_imperfect_srbkb #(
    .ID( 1 ),
    .NUM_STAGE( 36 ),
    .din0_WIDTH( 32 ),
    .din1_WIDTH( 6 ),
    .dout_WIDTH( 32 ))
loop_imperfect_srbkb_U2(
    .clk(ap_clk),
    .reset(ap_rst),
    .start(grp_fu_224_ap_start),
    .done(grp_fu_224_ap_done),
    .din0(b1_1_reg_377),
    .din1(grp_fu_224_p1),
    .ce(1'b1),
    .dout(grp_fu_224_p2)
);

always @ (posedge ap_clk) begin
    if (ap_rst == 1'b1) begin
        ap_CS_fsm <= ap_ST_fsm_state1;
    end else begin
        ap_CS_fsm <= ap_NS_fsm;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_CS_fsm_state83)) begin
        i_0_reg_111 <= i_reg_404;
    end else if (((ap_start == 1'b1) & (1'b1 == ap_CS_fsm_state1))) begin
        i_0_reg_111 <= 11'd0;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_CS_fsm_state82)) begin
        reg_123 <= M_q0;
    end else if ((1'b1 == ap_CS_fsm_state4)) begin
        reg_123 <= M_q1;
    end
end

always @ (posedge ap_clk) begin
    if (((1'b1 == ap_CS_fsm_state2) & (icmp_ln100_fu_128_p2 == 1'd1))) begin
        add_ln101_reg_305 <= add_ln101_fu_134_p2;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_CS_fsm_state7)) begin
        b0_1_reg_341 <= b0_1_fu_159_p2;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_CS_fsm_state44)) begin
        b0_2_reg_372 <= b0_2_fu_214_p2;
        b1_1_reg_377 <= b1_1_fu_219_p2;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_CS_fsm_state6)) begin
        b0_reg_330 <= buffer_r_q0;
        b1_reg_336 <= buffer_r_q1;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_CS_fsm_state82)) begin
        b1_2_reg_409 <= b1_2_fu_292_p2;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_CS_fsm_state4)) begin
        buffer_addr_reg_320 <= sext_ln102_fu_149_p1;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_CS_fsm_state81)) begin
        i_reg_404 <= i_fu_249_p2;
        shl_ln108_reg_393 <= shl_ln108_fu_233_p2;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_CS_fsm_state43)) begin
        shl_ln105_reg_357 <= shl_ln105_fu_173_p2;
        sub_ln105_reg_362 <= sub_ln105_fu_178_p2;
        tmp_reg_367 <= {{shl_ln105_fu_173_p2[31:6]}};
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_CS_fsm_state42)) begin
        srem_ln105_reg_347 <= grp_fu_163_p2;
        trunc_ln105_reg_352 <= trunc_ln105_fu_169_p1;
    end
end

always @ (posedge ap_clk) begin
    if ((1'b1 == ap_CS_fsm_state80)) begin
        srem_ln108_reg_383 <= grp_fu_224_p2;
        trunc_ln108_reg_388 <= trunc_ln108_fu_229_p1;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state81)) begin
        M_address0 = zext_ln111_fu_244_p1;
    end else if ((1'b1 == ap_CS_fsm_state3)) begin
        M_address0 = zext_ln101_fu_140_p1;
    end else begin
        M_address0 = 'bx;
    end
end

always @ (*) begin
    if (((1'b1 == ap_CS_fsm_state81) | (1'b1 == ap_CS_fsm_state3))) begin
        M_ce0 = 1'b1;
    end else begin
        M_ce0 = 1'b0;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state3)) begin
        M_ce1 = 1'b1;
    end else begin
        M_ce1 = 1'b0;
    end
end

always @ (*) begin
    if (((1'b1 == ap_CS_fsm_state2) & (icmp_ln100_fu_128_p2 == 1'd0))) begin
        ap_done = 1'b1;
    end else begin
        ap_done = 1'b0;
    end
end

always @ (*) begin
    if (((ap_start == 1'b0) & (1'b1 == ap_CS_fsm_state1))) begin
        ap_idle = 1'b1;
    end else begin
        ap_idle = 1'b0;
    end
end

always @ (*) begin
    if (((1'b1 == ap_CS_fsm_state2) & (icmp_ln100_fu_128_p2 == 1'd0))) begin
        ap_ready = 1'b1;
    end else begin
        ap_ready = 1'b0;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state83)) begin
        buffer_r_address1 = sext_ln111_fu_297_p1;
    end else if ((1'b1 == ap_CS_fsm_state5)) begin
        buffer_r_address1 = sext_ln102_1_fu_154_p1;
    end else begin
        buffer_r_address1 = 'bx;
    end
end

always @ (*) begin
    if (((1'b1 == ap_CS_fsm_state45) | (1'b1 == ap_CS_fsm_state5))) begin
        buffer_r_ce0 = 1'b1;
    end else begin
        buffer_r_ce0 = 1'b0;
    end
end

always @ (*) begin
    if (((1'b1 == ap_CS_fsm_state83) | (1'b1 == ap_CS_fsm_state5))) begin
        buffer_r_ce1 = 1'b1;
    end else begin
        buffer_r_ce1 = 1'b0;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state45)) begin
        buffer_r_we0 = 1'b1;
    end else begin
        buffer_r_we0 = 1'b0;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state83)) begin
        buffer_r_we1 = 1'b1;
    end else begin
        buffer_r_we1 = 1'b0;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state7)) begin
        grp_fu_163_ap_start = 1'b1;
    end else begin
        grp_fu_163_ap_start = 1'b0;
    end
end

always @ (*) begin
    if ((1'b1 == ap_CS_fsm_state45)) begin
        grp_fu_224_ap_start = 1'b1;
    end else begin
        grp_fu_224_ap_start = 1'b0;
    end
end

always @ (*) begin
    case (ap_CS_fsm)
        ap_ST_fsm_state1 : begin
            if (((ap_start == 1'b1) & (1'b1 == ap_CS_fsm_state1))) begin
                ap_NS_fsm = ap_ST_fsm_state2;
            end else begin
                ap_NS_fsm = ap_ST_fsm_state1;
            end
        end
        ap_ST_fsm_state2 : begin
            if (((1'b1 == ap_CS_fsm_state2) & (icmp_ln100_fu_128_p2 == 1'd0))) begin
                ap_NS_fsm = ap_ST_fsm_state1;
            end else begin
                ap_NS_fsm = ap_ST_fsm_state3;
            end
        end
        ap_ST_fsm_state3 : begin
            ap_NS_fsm = ap_ST_fsm_state4;
        end
        ap_ST_fsm_state4 : begin
            ap_NS_fsm = ap_ST_fsm_state5;
        end
        ap_ST_fsm_state5 : begin
            ap_NS_fsm = ap_ST_fsm_state6;
        end
        ap_ST_fsm_state6 : begin
            ap_NS_fsm = ap_ST_fsm_state7;
        end
        ap_ST_fsm_state7 : begin
            ap_NS_fsm = ap_ST_fsm_state8;
        end
        ap_ST_fsm_state8 : begin
            ap_NS_fsm = ap_ST_fsm_state9;
        end
        ap_ST_fsm_state9 : begin
            ap_NS_fsm = ap_ST_fsm_state10;
        end
        ap_ST_fsm_state10 : begin
            ap_NS_fsm = ap_ST_fsm_state11;
        end
        ap_ST_fsm_state11 : begin
            ap_NS_fsm = ap_ST_fsm_state12;
        end
        ap_ST_fsm_state12 : begin
            ap_NS_fsm = ap_ST_fsm_state13;
        end
        ap_ST_fsm_state13 : begin
            ap_NS_fsm = ap_ST_fsm_state14;
        end
        ap_ST_fsm_state14 : begin
            ap_NS_fsm = ap_ST_fsm_state15;
        end
        ap_ST_fsm_state15 : begin
            ap_NS_fsm = ap_ST_fsm_state16;
        end
        ap_ST_fsm_state16 : begin
            ap_NS_fsm = ap_ST_fsm_state17;
        end
        ap_ST_fsm_state17 : begin
            ap_NS_fsm = ap_ST_fsm_state18;
        end
        ap_ST_fsm_state18 : begin
            ap_NS_fsm = ap_ST_fsm_state19;
        end
        ap_ST_fsm_state19 : begin
            ap_NS_fsm = ap_ST_fsm_state20;
        end
        ap_ST_fsm_state20 : begin
            ap_NS_fsm = ap_ST_fsm_state21;
        end
        ap_ST_fsm_state21 : begin
            ap_NS_fsm = ap_ST_fsm_state22;
        end
        ap_ST_fsm_state22 : begin
            ap_NS_fsm = ap_ST_fsm_state23;
        end
        ap_ST_fsm_state23 : begin
            ap_NS_fsm = ap_ST_fsm_state24;
        end
        ap_ST_fsm_state24 : begin
            ap_NS_fsm = ap_ST_fsm_state25;
        end
        ap_ST_fsm_state25 : begin
            ap_NS_fsm = ap_ST_fsm_state26;
        end
        ap_ST_fsm_state26 : begin
            ap_NS_fsm = ap_ST_fsm_state27;
        end
        ap_ST_fsm_state27 : begin
            ap_NS_fsm = ap_ST_fsm_state28;
        end
        ap_ST_fsm_state28 : begin
            ap_NS_fsm = ap_ST_fsm_state29;
        end
        ap_ST_fsm_state29 : begin
            ap_NS_fsm = ap_ST_fsm_state30;
        end
        ap_ST_fsm_state30 : begin
            ap_NS_fsm = ap_ST_fsm_state31;
        end
        ap_ST_fsm_state31 : begin
            ap_NS_fsm = ap_ST_fsm_state32;
        end
        ap_ST_fsm_state32 : begin
            ap_NS_fsm = ap_ST_fsm_state33;
        end
        ap_ST_fsm_state33 : begin
            ap_NS_fsm = ap_ST_fsm_state34;
        end
        ap_ST_fsm_state34 : begin
            ap_NS_fsm = ap_ST_fsm_state35;
        end
        ap_ST_fsm_state35 : begin
            ap_NS_fsm = ap_ST_fsm_state36;
        end
        ap_ST_fsm_state36 : begin
            ap_NS_fsm = ap_ST_fsm_state37;
        end
        ap_ST_fsm_state37 : begin
            ap_NS_fsm = ap_ST_fsm_state38;
        end
        ap_ST_fsm_state38 : begin
            ap_NS_fsm = ap_ST_fsm_state39;
        end
        ap_ST_fsm_state39 : begin
            ap_NS_fsm = ap_ST_fsm_state40;
        end
        ap_ST_fsm_state40 : begin
            ap_NS_fsm = ap_ST_fsm_state41;
        end
        ap_ST_fsm_state41 : begin
            ap_NS_fsm = ap_ST_fsm_state42;
        end
        ap_ST_fsm_state42 : begin
            ap_NS_fsm = ap_ST_fsm_state43;
        end
        ap_ST_fsm_state43 : begin
            ap_NS_fsm = ap_ST_fsm_state44;
        end
        ap_ST_fsm_state44 : begin
            ap_NS_fsm = ap_ST_fsm_state45;
        end
        ap_ST_fsm_state45 : begin
            ap_NS_fsm = ap_ST_fsm_state46;
        end
        ap_ST_fsm_state46 : begin
            ap_NS_fsm = ap_ST_fsm_state47;
        end
        ap_ST_fsm_state47 : begin
            ap_NS_fsm = ap_ST_fsm_state48;
        end
        ap_ST_fsm_state48 : begin
            ap_NS_fsm = ap_ST_fsm_state49;
        end
        ap_ST_fsm_state49 : begin
            ap_NS_fsm = ap_ST_fsm_state50;
        end
        ap_ST_fsm_state50 : begin
            ap_NS_fsm = ap_ST_fsm_state51;
        end
        ap_ST_fsm_state51 : begin
            ap_NS_fsm = ap_ST_fsm_state52;
        end
        ap_ST_fsm_state52 : begin
            ap_NS_fsm = ap_ST_fsm_state53;
        end
        ap_ST_fsm_state53 : begin
            ap_NS_fsm = ap_ST_fsm_state54;
        end
        ap_ST_fsm_state54 : begin
            ap_NS_fsm = ap_ST_fsm_state55;
        end
        ap_ST_fsm_state55 : begin
            ap_NS_fsm = ap_ST_fsm_state56;
        end
        ap_ST_fsm_state56 : begin
            ap_NS_fsm = ap_ST_fsm_state57;
        end
        ap_ST_fsm_state57 : begin
            ap_NS_fsm = ap_ST_fsm_state58;
        end
        ap_ST_fsm_state58 : begin
            ap_NS_fsm = ap_ST_fsm_state59;
        end
        ap_ST_fsm_state59 : begin
            ap_NS_fsm = ap_ST_fsm_state60;
        end
        ap_ST_fsm_state60 : begin
            ap_NS_fsm = ap_ST_fsm_state61;
        end
        ap_ST_fsm_state61 : begin
            ap_NS_fsm = ap_ST_fsm_state62;
        end
        ap_ST_fsm_state62 : begin
            ap_NS_fsm = ap_ST_fsm_state63;
        end
        ap_ST_fsm_state63 : begin
            ap_NS_fsm = ap_ST_fsm_state64;
        end
        ap_ST_fsm_state64 : begin
            ap_NS_fsm = ap_ST_fsm_state65;
        end
        ap_ST_fsm_state65 : begin
            ap_NS_fsm = ap_ST_fsm_state66;
        end
        ap_ST_fsm_state66 : begin
            ap_NS_fsm = ap_ST_fsm_state67;
        end
        ap_ST_fsm_state67 : begin
            ap_NS_fsm = ap_ST_fsm_state68;
        end
        ap_ST_fsm_state68 : begin
            ap_NS_fsm = ap_ST_fsm_state69;
        end
        ap_ST_fsm_state69 : begin
            ap_NS_fsm = ap_ST_fsm_state70;
        end
        ap_ST_fsm_state70 : begin
            ap_NS_fsm = ap_ST_fsm_state71;
        end
        ap_ST_fsm_state71 : begin
            ap_NS_fsm = ap_ST_fsm_state72;
        end
        ap_ST_fsm_state72 : begin
            ap_NS_fsm = ap_ST_fsm_state73;
        end
        ap_ST_fsm_state73 : begin
            ap_NS_fsm = ap_ST_fsm_state74;
        end
        ap_ST_fsm_state74 : begin
            ap_NS_fsm = ap_ST_fsm_state75;
        end
        ap_ST_fsm_state75 : begin
            ap_NS_fsm = ap_ST_fsm_state76;
        end
        ap_ST_fsm_state76 : begin
            ap_NS_fsm = ap_ST_fsm_state77;
        end
        ap_ST_fsm_state77 : begin
            ap_NS_fsm = ap_ST_fsm_state78;
        end
        ap_ST_fsm_state78 : begin
            ap_NS_fsm = ap_ST_fsm_state79;
        end
        ap_ST_fsm_state79 : begin
            ap_NS_fsm = ap_ST_fsm_state80;
        end
        ap_ST_fsm_state80 : begin
            ap_NS_fsm = ap_ST_fsm_state81;
        end
        ap_ST_fsm_state81 : begin
            ap_NS_fsm = ap_ST_fsm_state82;
        end
        ap_ST_fsm_state82 : begin
            ap_NS_fsm = ap_ST_fsm_state83;
        end
        ap_ST_fsm_state83 : begin
            ap_NS_fsm = ap_ST_fsm_state2;
        end
        default : begin
            ap_NS_fsm = 'bx;
        end
    endcase
end

assign M_address1 = zext_ln101_1_fu_145_p1;

assign add_ln101_fu_134_p2 = (11'd4 + i_0_reg_111);

assign ap_CS_fsm_state1 = ap_CS_fsm[32'd0];

assign ap_CS_fsm_state2 = ap_CS_fsm[32'd1];

assign ap_CS_fsm_state3 = ap_CS_fsm[32'd2];

assign ap_CS_fsm_state4 = ap_CS_fsm[32'd3];

assign ap_CS_fsm_state42 = ap_CS_fsm[32'd41];

assign ap_CS_fsm_state43 = ap_CS_fsm[32'd42];

assign ap_CS_fsm_state44 = ap_CS_fsm[32'd43];

assign ap_CS_fsm_state45 = ap_CS_fsm[32'd44];

assign ap_CS_fsm_state5 = ap_CS_fsm[32'd4];

assign ap_CS_fsm_state6 = ap_CS_fsm[32'd5];

assign ap_CS_fsm_state7 = ap_CS_fsm[32'd6];

assign ap_CS_fsm_state80 = ap_CS_fsm[32'd79];

assign ap_CS_fsm_state81 = ap_CS_fsm[32'd80];

assign ap_CS_fsm_state82 = ap_CS_fsm[32'd81];

assign ap_CS_fsm_state83 = ap_CS_fsm[32'd82];

assign b0_1_fu_159_p2 = (b1_reg_336 ^ b0_reg_330);

assign b0_2_fu_214_p2 = (or_ln_fu_207_p3 ^ b0_1_reg_341);

assign b1_1_fu_219_p2 = (or_ln_fu_207_p3 ^ b0_reg_330);

assign b1_2_fu_292_p2 = (or_ln1_fu_284_p3 + b1_1_reg_377);

assign buffer_r_address0 = buffer_addr_reg_320;

assign buffer_r_d0 = b0_2_reg_372;

assign buffer_r_d1 = b1_2_reg_409;

assign grp_fu_163_p0 = (b1_reg_336 ^ b0_reg_330);

assign grp_fu_163_p1 = 32'd17;

assign grp_fu_224_p1 = 32'd17;

assign i_fu_249_p2 = (11'd2 + i_0_reg_111);

assign icmp_ln100_fu_128_p2 = ((i_0_reg_111 < 11'd2000) ? 1'b1 : 1'b0);

assign lshr_ln105_fu_193_p2 = 6'd2 >> sub_ln105_reg_362;

assign lshr_ln108_fu_260_p2 = 6'd3 >> sub_ln108_fu_255_p2;

assign or_ln105_fu_201_p2 = (trunc_ln105_1_fu_198_p1 | lshr_ln105_fu_193_p2);

assign or_ln108_fu_269_p2 = (trunc_ln108_1_fu_266_p1 | lshr_ln108_fu_260_p2);

assign or_ln111_fu_238_p2 = (i_0_reg_111 | 11'd1);

assign or_ln1_fu_284_p3 = {{tmp_1_fu_275_p4}, {or_ln108_fu_269_p2}};

assign or_ln_fu_207_p3 = {{tmp_reg_367}, {or_ln105_fu_201_p2}};

assign sext_ln102_1_fu_154_p1 = $signed(reg_123);

assign sext_ln102_fu_149_p1 = $signed(M_q0);

assign sext_ln111_fu_297_p1 = $signed(reg_123);

assign shl_ln105_fu_173_p2 = 32'd2 << srem_ln105_reg_347;

assign shl_ln108_fu_233_p2 = 32'd3 << srem_ln108_reg_383;

assign sub_ln105_fu_178_p2 = (6'd16 - trunc_ln105_reg_352);

assign sub_ln108_fu_255_p2 = (6'd16 - trunc_ln108_reg_388);

assign tmp_1_fu_275_p4 = {{shl_ln108_reg_393[31:6]}};

assign trunc_ln105_1_fu_198_p1 = shl_ln105_reg_357[5:0];

assign trunc_ln105_fu_169_p1 = grp_fu_163_p2[5:0];

assign trunc_ln108_1_fu_266_p1 = shl_ln108_reg_393[5:0];

assign trunc_ln108_fu_229_p1 = grp_fu_224_p2[5:0];

assign zext_ln101_1_fu_145_p1 = add_ln101_reg_305;

assign zext_ln101_fu_140_p1 = i_0_reg_111;

assign zext_ln111_fu_244_p1 = or_ln111_fu_238_p2;

endmodule //loop_imperfect
