// ==============================================================
// RTL generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and OpenCL
// Version: 2019.2
// Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
// 
// ===========================================================

#ifndef _loop_imperfect_HH_
#define _loop_imperfect_HH_

#include "systemc.h"
#include "AESL_pkg.h"

#include "loop_imperfect_srbkb.h"

namespace ap_rtl {

struct loop_imperfect : public sc_module {
    // Port declarations 22
    sc_in_clk ap_clk;
    sc_in< sc_logic > ap_rst;
    sc_in< sc_logic > ap_start;
    sc_out< sc_logic > ap_done;
    sc_out< sc_logic > ap_idle;
    sc_out< sc_logic > ap_ready;
    sc_out< sc_lv<12> > buffer_r_address0;
    sc_out< sc_logic > buffer_r_ce0;
    sc_out< sc_logic > buffer_r_we0;
    sc_out< sc_lv<32> > buffer_r_d0;
    sc_in< sc_lv<32> > buffer_r_q0;
    sc_out< sc_lv<12> > buffer_r_address1;
    sc_out< sc_logic > buffer_r_ce1;
    sc_out< sc_logic > buffer_r_we1;
    sc_out< sc_lv<32> > buffer_r_d1;
    sc_in< sc_lv<32> > buffer_r_q1;
    sc_out< sc_lv<12> > M_address0;
    sc_out< sc_logic > M_ce0;
    sc_in< sc_lv<32> > M_q0;
    sc_out< sc_lv<12> > M_address1;
    sc_out< sc_logic > M_ce1;
    sc_in< sc_lv<32> > M_q1;
    sc_signal< sc_logic > ap_var_for_const0;


    // Module declarations
    loop_imperfect(sc_module_name name);
    SC_HAS_PROCESS(loop_imperfect);

    ~loop_imperfect();

    sc_trace_file* mVcdFile;

    ofstream mHdltvinHandle;
    ofstream mHdltvoutHandle;
    loop_imperfect_srbkb<1,36,32,6,32>* loop_imperfect_srbkb_U1;
    loop_imperfect_srbkb<1,36,32,6,32>* loop_imperfect_srbkb_U2;
    sc_signal< sc_lv<83> > ap_CS_fsm;
    sc_signal< sc_logic > ap_CS_fsm_state1;
    sc_signal< sc_lv<32> > reg_123;
    sc_signal< sc_logic > ap_CS_fsm_state4;
    sc_signal< sc_logic > ap_CS_fsm_state82;
    sc_signal< sc_lv<11> > add_ln101_fu_134_p2;
    sc_signal< sc_lv<11> > add_ln101_reg_305;
    sc_signal< sc_logic > ap_CS_fsm_state2;
    sc_signal< sc_lv<1> > icmp_ln100_fu_128_p2;
    sc_signal< sc_logic > ap_CS_fsm_state3;
    sc_signal< sc_lv<12> > buffer_addr_reg_320;
    sc_signal< sc_logic > ap_CS_fsm_state5;
    sc_signal< sc_lv<32> > b0_reg_330;
    sc_signal< sc_logic > ap_CS_fsm_state6;
    sc_signal< sc_lv<32> > b1_reg_336;
    sc_signal< sc_lv<32> > b0_1_fu_159_p2;
    sc_signal< sc_lv<32> > b0_1_reg_341;
    sc_signal< sc_logic > ap_CS_fsm_state7;
    sc_signal< sc_lv<32> > grp_fu_163_p2;
    sc_signal< sc_lv<32> > srem_ln105_reg_347;
    sc_signal< sc_logic > ap_CS_fsm_state42;
    sc_signal< sc_lv<6> > trunc_ln105_fu_169_p1;
    sc_signal< sc_lv<6> > trunc_ln105_reg_352;
    sc_signal< sc_lv<32> > shl_ln105_fu_173_p2;
    sc_signal< sc_lv<32> > shl_ln105_reg_357;
    sc_signal< sc_logic > ap_CS_fsm_state43;
    sc_signal< sc_lv<6> > sub_ln105_fu_178_p2;
    sc_signal< sc_lv<6> > sub_ln105_reg_362;
    sc_signal< sc_lv<26> > tmp_reg_367;
    sc_signal< sc_lv<32> > b0_2_fu_214_p2;
    sc_signal< sc_lv<32> > b0_2_reg_372;
    sc_signal< sc_logic > ap_CS_fsm_state44;
    sc_signal< sc_lv<32> > b1_1_fu_219_p2;
    sc_signal< sc_lv<32> > b1_1_reg_377;
    sc_signal< sc_lv<32> > grp_fu_224_p2;
    sc_signal< sc_lv<32> > srem_ln108_reg_383;
    sc_signal< sc_logic > ap_CS_fsm_state80;
    sc_signal< sc_lv<6> > trunc_ln108_fu_229_p1;
    sc_signal< sc_lv<6> > trunc_ln108_reg_388;
    sc_signal< sc_lv<32> > shl_ln108_fu_233_p2;
    sc_signal< sc_lv<32> > shl_ln108_reg_393;
    sc_signal< sc_logic > ap_CS_fsm_state81;
    sc_signal< sc_lv<11> > i_fu_249_p2;
    sc_signal< sc_lv<11> > i_reg_404;
    sc_signal< sc_lv<32> > b1_2_fu_292_p2;
    sc_signal< sc_lv<32> > b1_2_reg_409;
    sc_signal< sc_lv<11> > i_0_reg_111;
    sc_signal< sc_logic > ap_CS_fsm_state83;
    sc_signal< sc_lv<64> > zext_ln101_fu_140_p1;
    sc_signal< sc_lv<64> > zext_ln101_1_fu_145_p1;
    sc_signal< sc_lv<64> > sext_ln102_fu_149_p1;
    sc_signal< sc_lv<64> > sext_ln102_1_fu_154_p1;
    sc_signal< sc_lv<64> > zext_ln111_fu_244_p1;
    sc_signal< sc_lv<64> > sext_ln111_fu_297_p1;
    sc_signal< sc_logic > ap_CS_fsm_state45;
    sc_signal< sc_lv<32> > grp_fu_163_p0;
    sc_signal< sc_lv<6> > grp_fu_163_p1;
    sc_signal< sc_lv<6> > trunc_ln105_1_fu_198_p1;
    sc_signal< sc_lv<6> > lshr_ln105_fu_193_p2;
    sc_signal< sc_lv<6> > or_ln105_fu_201_p2;
    sc_signal< sc_lv<32> > or_ln_fu_207_p3;
    sc_signal< sc_lv<6> > grp_fu_224_p1;
    sc_signal< sc_lv<11> > or_ln111_fu_238_p2;
    sc_signal< sc_lv<6> > sub_ln108_fu_255_p2;
    sc_signal< sc_lv<6> > trunc_ln108_1_fu_266_p1;
    sc_signal< sc_lv<6> > lshr_ln108_fu_260_p2;
    sc_signal< sc_lv<26> > tmp_1_fu_275_p4;
    sc_signal< sc_lv<6> > or_ln108_fu_269_p2;
    sc_signal< sc_lv<32> > or_ln1_fu_284_p3;
    sc_signal< sc_logic > grp_fu_163_ap_start;
    sc_signal< sc_logic > grp_fu_163_ap_done;
    sc_signal< sc_logic > grp_fu_224_ap_start;
    sc_signal< sc_logic > grp_fu_224_ap_done;
    sc_signal< sc_lv<83> > ap_NS_fsm;
    static const sc_logic ap_const_logic_1;
    static const sc_logic ap_const_logic_0;
    static const sc_lv<83> ap_ST_fsm_state1;
    static const sc_lv<83> ap_ST_fsm_state2;
    static const sc_lv<83> ap_ST_fsm_state3;
    static const sc_lv<83> ap_ST_fsm_state4;
    static const sc_lv<83> ap_ST_fsm_state5;
    static const sc_lv<83> ap_ST_fsm_state6;
    static const sc_lv<83> ap_ST_fsm_state7;
    static const sc_lv<83> ap_ST_fsm_state8;
    static const sc_lv<83> ap_ST_fsm_state9;
    static const sc_lv<83> ap_ST_fsm_state10;
    static const sc_lv<83> ap_ST_fsm_state11;
    static const sc_lv<83> ap_ST_fsm_state12;
    static const sc_lv<83> ap_ST_fsm_state13;
    static const sc_lv<83> ap_ST_fsm_state14;
    static const sc_lv<83> ap_ST_fsm_state15;
    static const sc_lv<83> ap_ST_fsm_state16;
    static const sc_lv<83> ap_ST_fsm_state17;
    static const sc_lv<83> ap_ST_fsm_state18;
    static const sc_lv<83> ap_ST_fsm_state19;
    static const sc_lv<83> ap_ST_fsm_state20;
    static const sc_lv<83> ap_ST_fsm_state21;
    static const sc_lv<83> ap_ST_fsm_state22;
    static const sc_lv<83> ap_ST_fsm_state23;
    static const sc_lv<83> ap_ST_fsm_state24;
    static const sc_lv<83> ap_ST_fsm_state25;
    static const sc_lv<83> ap_ST_fsm_state26;
    static const sc_lv<83> ap_ST_fsm_state27;
    static const sc_lv<83> ap_ST_fsm_state28;
    static const sc_lv<83> ap_ST_fsm_state29;
    static const sc_lv<83> ap_ST_fsm_state30;
    static const sc_lv<83> ap_ST_fsm_state31;
    static const sc_lv<83> ap_ST_fsm_state32;
    static const sc_lv<83> ap_ST_fsm_state33;
    static const sc_lv<83> ap_ST_fsm_state34;
    static const sc_lv<83> ap_ST_fsm_state35;
    static const sc_lv<83> ap_ST_fsm_state36;
    static const sc_lv<83> ap_ST_fsm_state37;
    static const sc_lv<83> ap_ST_fsm_state38;
    static const sc_lv<83> ap_ST_fsm_state39;
    static const sc_lv<83> ap_ST_fsm_state40;
    static const sc_lv<83> ap_ST_fsm_state41;
    static const sc_lv<83> ap_ST_fsm_state42;
    static const sc_lv<83> ap_ST_fsm_state43;
    static const sc_lv<83> ap_ST_fsm_state44;
    static const sc_lv<83> ap_ST_fsm_state45;
    static const sc_lv<83> ap_ST_fsm_state46;
    static const sc_lv<83> ap_ST_fsm_state47;
    static const sc_lv<83> ap_ST_fsm_state48;
    static const sc_lv<83> ap_ST_fsm_state49;
    static const sc_lv<83> ap_ST_fsm_state50;
    static const sc_lv<83> ap_ST_fsm_state51;
    static const sc_lv<83> ap_ST_fsm_state52;
    static const sc_lv<83> ap_ST_fsm_state53;
    static const sc_lv<83> ap_ST_fsm_state54;
    static const sc_lv<83> ap_ST_fsm_state55;
    static const sc_lv<83> ap_ST_fsm_state56;
    static const sc_lv<83> ap_ST_fsm_state57;
    static const sc_lv<83> ap_ST_fsm_state58;
    static const sc_lv<83> ap_ST_fsm_state59;
    static const sc_lv<83> ap_ST_fsm_state60;
    static const sc_lv<83> ap_ST_fsm_state61;
    static const sc_lv<83> ap_ST_fsm_state62;
    static const sc_lv<83> ap_ST_fsm_state63;
    static const sc_lv<83> ap_ST_fsm_state64;
    static const sc_lv<83> ap_ST_fsm_state65;
    static const sc_lv<83> ap_ST_fsm_state66;
    static const sc_lv<83> ap_ST_fsm_state67;
    static const sc_lv<83> ap_ST_fsm_state68;
    static const sc_lv<83> ap_ST_fsm_state69;
    static const sc_lv<83> ap_ST_fsm_state70;
    static const sc_lv<83> ap_ST_fsm_state71;
    static const sc_lv<83> ap_ST_fsm_state72;
    static const sc_lv<83> ap_ST_fsm_state73;
    static const sc_lv<83> ap_ST_fsm_state74;
    static const sc_lv<83> ap_ST_fsm_state75;
    static const sc_lv<83> ap_ST_fsm_state76;
    static const sc_lv<83> ap_ST_fsm_state77;
    static const sc_lv<83> ap_ST_fsm_state78;
    static const sc_lv<83> ap_ST_fsm_state79;
    static const sc_lv<83> ap_ST_fsm_state80;
    static const sc_lv<83> ap_ST_fsm_state81;
    static const sc_lv<83> ap_ST_fsm_state82;
    static const sc_lv<83> ap_ST_fsm_state83;
    static const sc_lv<32> ap_const_lv32_0;
    static const sc_lv<32> ap_const_lv32_3;
    static const sc_lv<32> ap_const_lv32_51;
    static const sc_lv<32> ap_const_lv32_1;
    static const sc_lv<1> ap_const_lv1_1;
    static const sc_lv<32> ap_const_lv32_2;
    static const sc_lv<32> ap_const_lv32_4;
    static const sc_lv<32> ap_const_lv32_5;
    static const sc_lv<32> ap_const_lv32_6;
    static const sc_lv<32> ap_const_lv32_29;
    static const sc_lv<32> ap_const_lv32_2A;
    static const sc_lv<32> ap_const_lv32_2B;
    static const sc_lv<32> ap_const_lv32_4F;
    static const sc_lv<32> ap_const_lv32_50;
    static const sc_lv<11> ap_const_lv11_0;
    static const sc_lv<32> ap_const_lv32_52;
    static const sc_lv<32> ap_const_lv32_2C;
    static const sc_lv<11> ap_const_lv11_7D0;
    static const sc_lv<11> ap_const_lv11_4;
    static const sc_lv<32> ap_const_lv32_11;
    static const sc_lv<6> ap_const_lv6_10;
    static const sc_lv<32> ap_const_lv32_1F;
    static const sc_lv<6> ap_const_lv6_2;
    static const sc_lv<11> ap_const_lv11_1;
    static const sc_lv<11> ap_const_lv11_2;
    static const sc_lv<6> ap_const_lv6_3;
    static const sc_lv<1> ap_const_lv1_0;
    static const bool ap_const_boolean_1;
    // Thread declarations
    void thread_ap_var_for_const0();
    void thread_ap_clk_no_reset_();
    void thread_M_address0();
    void thread_M_address1();
    void thread_M_ce0();
    void thread_M_ce1();
    void thread_add_ln101_fu_134_p2();
    void thread_ap_CS_fsm_state1();
    void thread_ap_CS_fsm_state2();
    void thread_ap_CS_fsm_state3();
    void thread_ap_CS_fsm_state4();
    void thread_ap_CS_fsm_state42();
    void thread_ap_CS_fsm_state43();
    void thread_ap_CS_fsm_state44();
    void thread_ap_CS_fsm_state45();
    void thread_ap_CS_fsm_state5();
    void thread_ap_CS_fsm_state6();
    void thread_ap_CS_fsm_state7();
    void thread_ap_CS_fsm_state80();
    void thread_ap_CS_fsm_state81();
    void thread_ap_CS_fsm_state82();
    void thread_ap_CS_fsm_state83();
    void thread_ap_done();
    void thread_ap_idle();
    void thread_ap_ready();
    void thread_b0_1_fu_159_p2();
    void thread_b0_2_fu_214_p2();
    void thread_b1_1_fu_219_p2();
    void thread_b1_2_fu_292_p2();
    void thread_buffer_r_address0();
    void thread_buffer_r_address1();
    void thread_buffer_r_ce0();
    void thread_buffer_r_ce1();
    void thread_buffer_r_d0();
    void thread_buffer_r_d1();
    void thread_buffer_r_we0();
    void thread_buffer_r_we1();
    void thread_grp_fu_163_ap_start();
    void thread_grp_fu_163_p0();
    void thread_grp_fu_163_p1();
    void thread_grp_fu_224_ap_start();
    void thread_grp_fu_224_p1();
    void thread_i_fu_249_p2();
    void thread_icmp_ln100_fu_128_p2();
    void thread_lshr_ln105_fu_193_p2();
    void thread_lshr_ln108_fu_260_p2();
    void thread_or_ln105_fu_201_p2();
    void thread_or_ln108_fu_269_p2();
    void thread_or_ln111_fu_238_p2();
    void thread_or_ln1_fu_284_p3();
    void thread_or_ln_fu_207_p3();
    void thread_sext_ln102_1_fu_154_p1();
    void thread_sext_ln102_fu_149_p1();
    void thread_sext_ln111_fu_297_p1();
    void thread_shl_ln105_fu_173_p2();
    void thread_shl_ln108_fu_233_p2();
    void thread_sub_ln105_fu_178_p2();
    void thread_sub_ln108_fu_255_p2();
    void thread_tmp_1_fu_275_p4();
    void thread_trunc_ln105_1_fu_198_p1();
    void thread_trunc_ln105_fu_169_p1();
    void thread_trunc_ln108_1_fu_266_p1();
    void thread_trunc_ln108_fu_229_p1();
    void thread_zext_ln101_1_fu_145_p1();
    void thread_zext_ln101_fu_140_p1();
    void thread_zext_ln111_fu_244_p1();
    void thread_ap_NS_fsm();
    void thread_hdltv_gen();
};

}

using namespace ap_rtl;

#endif
