// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.2 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __loop_imperfect_mubkb__HH__
#define __loop_imperfect_mubkb__HH__
#include "ACMP_mul_ss.h"
#include <systemc>

template<
    int ID,
    int NUM_STAGE,
    int din0_WIDTH,
    int din1_WIDTH,
    int dout_WIDTH>
SC_MODULE(loop_imperfect_mubkb) {
    sc_core::sc_in_clk clk;
    sc_core::sc_in<sc_dt::sc_logic> reset;
    sc_core::sc_in<sc_dt::sc_logic> ce;
    sc_core::sc_in< sc_dt::sc_lv<din0_WIDTH> >   din0;
    sc_core::sc_in< sc_dt::sc_lv<din1_WIDTH> >   din1;
    sc_core::sc_out< sc_dt::sc_lv<dout_WIDTH> >   dout;



    ACMP_mul_ss<ID, 5, din0_WIDTH, din1_WIDTH, dout_WIDTH> ACMP_mul_ss_U;

    SC_CTOR(loop_imperfect_mubkb):  ACMP_mul_ss_U ("ACMP_mul_ss_U") {
        ACMP_mul_ss_U.clk(clk);
        ACMP_mul_ss_U.reset(reset);
        ACMP_mul_ss_U.ce(ce);
        ACMP_mul_ss_U.din0(din0);
        ACMP_mul_ss_U.din1(din1);
        ACMP_mul_ss_U.dout(dout);

    }

};

#endif //
