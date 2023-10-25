// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.2 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================

#include <systemc>
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <stdint.h>
#include "SysCFileHandler.h"
#include "ap_int.h"
#include "ap_fixed.h"
#include <complex>
#include <stdbool.h>
#include "autopilot_cbe.h"
#include "hls_stream.h"
#include "hls_half.h"
#include "hls_signal_handler.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;


// [dump_struct_tree [build_nameSpaceTree] dumpedStructList] ---------->


// [dump_enumeration [get_enumeration_list]] ---------->


// wrapc file define: "A"
#define AUTOTB_TVIN_A  "../tv/cdatafile/c.loop_imperfect.autotvin_A.dat"
#define AUTOTB_TVOUT_A  "../tv/cdatafile/c.loop_imperfect.autotvout_A.dat"
// wrapc file define: "addr"
#define AUTOTB_TVIN_addr  "../tv/cdatafile/c.loop_imperfect.autotvin_addr.dat"

#define INTER_TCL  "../tv/cdatafile/ref.tcl"

// tvout file define: "A"
#define AUTOTB_TVOUT_PC_A  "../tv/rtldatafile/rtl.loop_imperfect.autotvout_A.dat"

class INTER_TCL_FILE {
	public:
		INTER_TCL_FILE(const char* name) {
			mName = name;
			A_depth = 0;
			addr_depth = 0;
			trans_num =0;
		}

		~INTER_TCL_FILE() {
			mFile.open(mName);
			if (!mFile.good()) {
				cout << "Failed to open file ref.tcl" << endl;
				exit (1);
			}
			string total_list = get_depth_list();
			mFile << "set depth_list {\n";
			mFile << total_list;
			mFile << "}\n";
			mFile << "set trans_num "<<trans_num<<endl;
			mFile.close();
		}

		string get_depth_list () {
			stringstream total_list;
			total_list << "{A " << A_depth << "}\n";
			total_list << "{addr " << addr_depth << "}\n";
			return total_list.str();
		}

		void set_num (int num , int* class_num) {
			(*class_num) = (*class_num) > num ? (*class_num) : num;
		}
	public:
		int A_depth;
		int addr_depth;
		int trans_num;

	private:
		ofstream mFile;
		const char* mName;
};

extern "C" void loop_imperfect (
float A[1000],
int addr[1000]);

extern "C" void AESL_WRAP_loop_imperfect (
float A[1000],
int addr[1000])
{
	refine_signal_handler();
	fstream wrapc_switch_file_token;
	wrapc_switch_file_token.open(".hls_cosim_wrapc_switch.log");
	int AESL_i;
	if (wrapc_switch_file_token.good())
	{
		CodeState = ENTER_WRAPC_PC;
		static unsigned AESL_transaction_pc = 0;
		string AESL_token;
		string AESL_num;
		static AESL_FILE_HANDLER aesl_fh;


		// output port post check: "A"
		aesl_fh.read(AUTOTB_TVOUT_PC_A, AESL_token); // [[transaction]]
		if (AESL_token != "[[transaction]]")
		{
			exit(1);
		}
		aesl_fh.read(AUTOTB_TVOUT_PC_A, AESL_num); // transaction number

		if (atoi(AESL_num.c_str()) == AESL_transaction_pc)
		{
			aesl_fh.read(AUTOTB_TVOUT_PC_A, AESL_token); // data

			sc_bv<32> *A_pc_buffer = new sc_bv<32>[1000];
			int i = 0;

			while (AESL_token != "[[/transaction]]")
			{
				bool no_x = false;
				bool err = false;

				// search and replace 'X' with "0" from the 1st char of token
				while (!no_x)
				{
					size_t x_found = AESL_token.find('X');
					if (x_found != string::npos)
					{
						if (!err)
						{
							cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'X' on port 'A', possible cause: There are uninitialized variables in the C design." << endl;
							err = true;
						}
						AESL_token.replace(x_found, 1, "0");
					}
					else
					{
						no_x = true;
					}
				}

				no_x = false;

				// search and replace 'x' with "0" from the 3rd char of token
				while (!no_x)
				{
					size_t x_found = AESL_token.find('x', 2);

					if (x_found != string::npos)
					{
						if (!err)
						{
							cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'X' on port 'A', possible cause: There are uninitialized variables in the C design." << endl;
							err = true;
						}
						AESL_token.replace(x_found, 1, "0");
					}
					else
					{
						no_x = true;
					}
				}

				// push token into output port buffer
				if (AESL_token != "")
				{
					A_pc_buffer[i] = AESL_token.c_str();
					i++;
				}

				aesl_fh.read(AUTOTB_TVOUT_PC_A, AESL_token); // data or [[/transaction]]

				if (AESL_token == "[[[/runtime]]]" || aesl_fh.eof(AUTOTB_TVOUT_PC_A))
				{
					exit(1);
				}
			}

			// ***********************************
			if (i > 0)
			{
				// RTL Name: A
				{
					// bitslice(31, 0)
					// {
						// celement: A(31, 0)
						// {
							sc_lv<32>* A_lv0_0_999_1 = new sc_lv<32>[1000];
						// }
					// }

					// bitslice(31, 0)
					{
						int hls_map_index = 0;
						// celement: A(31, 0)
						{
							// carray: (0) => (999) @ (1)
							for (int i_0 = 0; i_0 <= 999; i_0 += 1)
							{
								if (&(A[0]) != NULL) // check the null address if the c port is array or others
								{
									A_lv0_0_999_1[hls_map_index].range(31, 0) = sc_bv<32>(A_pc_buffer[hls_map_index].range(31, 0));
									hls_map_index++;
								}
							}
						}
					}

					// bitslice(31, 0)
					{
						int hls_map_index = 0;
						// celement: A(31, 0)
						{
							// carray: (0) => (999) @ (1)
							for (int i_0 = 0; i_0 <= 999; i_0 += 1)
							{
								// sub                    : i_0
								// ori_name               : A[i_0]
								// sub_1st_elem           : 0
								// ori_name_1st_elem      : A[0]
								// output_left_conversion : *(int*)&A[i_0]
								// output_type_conversion : (A_lv0_0_999_1[hls_map_index]).to_uint64()
								if (&(A[0]) != NULL) // check the null address if the c port is array or others
								{
									*(int*)&A[i_0] = (A_lv0_0_999_1[hls_map_index]).to_uint64();
									hls_map_index++;
								}
							}
						}
					}
				}
			}

			// release memory allocation
			delete [] A_pc_buffer;
		}

		AESL_transaction_pc++;
	}
	else
	{
		CodeState = ENTER_WRAPC;
		static unsigned AESL_transaction;

		static AESL_FILE_HANDLER aesl_fh;

		// "A"
		char* tvin_A = new char[50];
		aesl_fh.touch(AUTOTB_TVIN_A);
		char* tvout_A = new char[50];
		aesl_fh.touch(AUTOTB_TVOUT_A);

		// "addr"
		char* tvin_addr = new char[50];
		aesl_fh.touch(AUTOTB_TVIN_addr);

		CodeState = DUMP_INPUTS;
		static INTER_TCL_FILE tcl_file(INTER_TCL);
		int leading_zero;

		// [[transaction]]
		sprintf(tvin_A, "[[transaction]] %d\n", AESL_transaction);
		aesl_fh.write(AUTOTB_TVIN_A, tvin_A);

		sc_bv<32>* A_tvin_wrapc_buffer = new sc_bv<32>[1000];

		// RTL Name: A
		{
			// bitslice(31, 0)
			{
				int hls_map_index = 0;
				// celement: A(31, 0)
				{
					// carray: (0) => (999) @ (1)
					for (int i_0 = 0; i_0 <= 999; i_0 += 1)
					{
						// sub                   : i_0
						// ori_name              : A[i_0]
						// sub_1st_elem          : 0
						// ori_name_1st_elem     : A[0]
						// regulate_c_name       : A
						// input_type_conversion : *(int*)&A[i_0]
						if (&(A[0]) != NULL) // check the null address if the c port is array or others
						{
							sc_lv<32> A_tmp_mem;
							A_tmp_mem = *(int*)&A[i_0];
							A_tvin_wrapc_buffer[hls_map_index].range(31, 0) = A_tmp_mem.range(31, 0);
                                 	       hls_map_index++;
						}
					}
				}
			}
		}

		// dump tv to file
		for (int i = 0; i < 1000; i++)
		{
			sprintf(tvin_A, "%s\n", (A_tvin_wrapc_buffer[i]).to_string(SC_HEX).c_str());
			aesl_fh.write(AUTOTB_TVIN_A, tvin_A);
		}

		tcl_file.set_num(1000, &tcl_file.A_depth);
		sprintf(tvin_A, "[[/transaction]] \n");
		aesl_fh.write(AUTOTB_TVIN_A, tvin_A);

		// release memory allocation
		delete [] A_tvin_wrapc_buffer;

		// [[transaction]]
		sprintf(tvin_addr, "[[transaction]] %d\n", AESL_transaction);
		aesl_fh.write(AUTOTB_TVIN_addr, tvin_addr);

		sc_bv<32>* addr_tvin_wrapc_buffer = new sc_bv<32>[1000];

		// RTL Name: addr
		{
			// bitslice(31, 0)
			{
				int hls_map_index = 0;
				// celement: addr(31, 0)
				{
					// carray: (0) => (999) @ (1)
					for (int i_0 = 0; i_0 <= 999; i_0 += 1)
					{
						// sub                   : i_0
						// ori_name              : addr[i_0]
						// sub_1st_elem          : 0
						// ori_name_1st_elem     : addr[0]
						// regulate_c_name       : addr
						// input_type_conversion : addr[i_0]
						if (&(addr[0]) != NULL) // check the null address if the c port is array or others
						{
							sc_lv<32> addr_tmp_mem;
							addr_tmp_mem = addr[i_0];
							addr_tvin_wrapc_buffer[hls_map_index].range(31, 0) = addr_tmp_mem.range(31, 0);
                                 	       hls_map_index++;
						}
					}
				}
			}
		}

		// dump tv to file
		for (int i = 0; i < 1000; i++)
		{
			sprintf(tvin_addr, "%s\n", (addr_tvin_wrapc_buffer[i]).to_string(SC_HEX).c_str());
			aesl_fh.write(AUTOTB_TVIN_addr, tvin_addr);
		}

		tcl_file.set_num(1000, &tcl_file.addr_depth);
		sprintf(tvin_addr, "[[/transaction]] \n");
		aesl_fh.write(AUTOTB_TVIN_addr, tvin_addr);

		// release memory allocation
		delete [] addr_tvin_wrapc_buffer;

// [call_c_dut] ---------->

		CodeState = CALL_C_DUT;
		loop_imperfect(A, addr);

		CodeState = DUMP_OUTPUTS;

		// [[transaction]]
		sprintf(tvout_A, "[[transaction]] %d\n", AESL_transaction);
		aesl_fh.write(AUTOTB_TVOUT_A, tvout_A);

		sc_bv<32>* A_tvout_wrapc_buffer = new sc_bv<32>[1000];

		// RTL Name: A
		{
			// bitslice(31, 0)
			{
				int hls_map_index = 0;
				// celement: A(31, 0)
				{
					// carray: (0) => (999) @ (1)
					for (int i_0 = 0; i_0 <= 999; i_0 += 1)
					{
						// sub                   : i_0
						// ori_name              : A[i_0]
						// sub_1st_elem          : 0
						// ori_name_1st_elem     : A[0]
						// regulate_c_name       : A
						// input_type_conversion : *(int*)&A[i_0]
						if (&(A[0]) != NULL) // check the null address if the c port is array or others
						{
							sc_lv<32> A_tmp_mem;
							A_tmp_mem = *(int*)&A[i_0];
							A_tvout_wrapc_buffer[hls_map_index].range(31, 0) = A_tmp_mem.range(31, 0);
                                 	       hls_map_index++;
						}
					}
				}
			}
		}

		// dump tv to file
		for (int i = 0; i < 1000; i++)
		{
			sprintf(tvout_A, "%s\n", (A_tvout_wrapc_buffer[i]).to_string(SC_HEX).c_str());
			aesl_fh.write(AUTOTB_TVOUT_A, tvout_A);
		}

		tcl_file.set_num(1000, &tcl_file.A_depth);
		sprintf(tvout_A, "[[/transaction]] \n");
		aesl_fh.write(AUTOTB_TVOUT_A, tvout_A);

		// release memory allocation
		delete [] A_tvout_wrapc_buffer;

		CodeState = DELETE_CHAR_BUFFERS;
		// release memory allocation: "A"
		delete [] tvin_A;
		delete [] tvout_A;
		// release memory allocation: "addr"
		delete [] tvin_addr;

		AESL_transaction++;

		tcl_file.set_num(AESL_transaction , &tcl_file.trans_num);
	}
}

