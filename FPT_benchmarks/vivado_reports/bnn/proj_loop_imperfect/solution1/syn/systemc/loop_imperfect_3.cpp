#include "loop_imperfect.h"
#include "AESL_pkg.h"

using namespace std;

namespace ap_rtl {

void loop_imperfect::thread_hdltv_gen() {
    const char* dump_tv = std::getenv("AP_WRITE_TV");
    if (!(dump_tv && string(dump_tv) == "on")) return;

    wait();

    mHdltvinHandle << "[ " << endl;
    mHdltvoutHandle << "[ " << endl;
    int ap_cycleNo = 0;
    while (1) {
        wait();
        const char* mComma = ap_cycleNo == 0 ? " " : ", " ;
        mHdltvinHandle << mComma << "{"  <<  " \"ap_rst\" :  \"" << ap_rst.read() << "\" ";
        mHdltvinHandle << " , " <<  " \"ap_start\" :  \"" << ap_start.read() << "\" ";
        mHdltvoutHandle << mComma << "{"  <<  " \"ap_done\" :  \"" << ap_done.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"ap_idle\" :  \"" << ap_idle.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"ap_ready\" :  \"" << ap_ready.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"data_address0\" :  \"" << data_address0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"data_ce0\" :  \"" << data_ce0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"data_we0\" :  \"" << data_we0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"data_d0\" :  \"" << data_d0.read() << "\" ";
        mHdltvinHandle << " , " <<  " \"data_q0\" :  \"" << data_q0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"addr_in_address0\" :  \"" << addr_in_address0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"addr_in_ce0\" :  \"" << addr_in_ce0.read() << "\" ";
        mHdltvinHandle << " , " <<  " \"addr_in_q0\" :  \"" << addr_in_q0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"addr_in_address1\" :  \"" << addr_in_address1.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"addr_in_ce1\" :  \"" << addr_in_ce1.read() << "\" ";
        mHdltvinHandle << " , " <<  " \"addr_in_q1\" :  \"" << addr_in_q1.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"addr_out_address0\" :  \"" << addr_out_address0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"addr_out_ce0\" :  \"" << addr_out_ce0.read() << "\" ";
        mHdltvinHandle << " , " <<  " \"addr_out_q0\" :  \"" << addr_out_q0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"a_address0\" :  \"" << a_address0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"a_ce0\" :  \"" << a_ce0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"a_we0\" :  \"" << a_we0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"a_d0\" :  \"" << a_d0.read() << "\" ";
        mHdltvinHandle << " , " <<  " \"a_q0\" :  \"" << a_q0.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"a_address1\" :  \"" << a_address1.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"a_ce1\" :  \"" << a_ce1.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"a_we1\" :  \"" << a_we1.read() << "\" ";
        mHdltvoutHandle << " , " <<  " \"a_d1\" :  \"" << a_d1.read() << "\" ";
        mHdltvinHandle << " , " <<  " \"a_q1\" :  \"" << a_q1.read() << "\" ";
        mHdltvinHandle << "}" << std::endl;
        mHdltvoutHandle << "}" << std::endl;
        ap_cycleNo++;
    }
}

}

