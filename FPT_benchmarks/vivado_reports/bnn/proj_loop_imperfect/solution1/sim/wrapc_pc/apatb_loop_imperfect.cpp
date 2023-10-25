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


// wrapc file define: "data"
#define AUTOTB_TVIN_data  "../tv/cdatafile/c.loop_imperfect.autotvin_data.dat"
#define AUTOTB_TVOUT_data  "../tv/cdatafile/c.loop_imperfect.autotvout_data.dat"
// wrapc file define: "addr_in"
#define AUTOTB_TVIN_addr_in  "../tv/cdatafile/c.loop_imperfect.autotvin_addr_in.dat"
// wrapc file define: "addr_out"
#define AUTOTB_TVIN_addr_out  "../tv/cdatafile/c.loop_imperfect.autotvin_addr_out.dat"

#define INTER_TCL  "../tv/cdatafile/ref.tcl"

// tvout file define: "data"
#define AUTOTB_TVOUT_PC_data  "../tv/rtldatafile/rtl.loop_imperfect.autotvout_data.dat"

class INTER_TCL_FILE {
	public:
		INTER_TCL_FILE(const char* name) {
			mName = name;
			data_depth = 0;
			addr_in_depth = 0;
			addr_out_depth = 0;
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
			total_list << "{data " << data_depth << "}\n";
			total_list << "{addr_in " << addr_in_depth << "}\n";
			total_list << "{addr_out " << addr_out_depth << "}\n";
			return total_list.str();
		}

		void set_num (int num , int* class_num) {
			(*class_num) = (*class_num) > num ? (*class_num) : num;
		}
	public:
		int data_depth;
		int addr_in_depth;
		int addr_out_depth;
		int trans_num;

	private:
		ofstream mFile;
		const char* mName;
};

extern "C" void loop_imperfect (
int data[10000],
int addr_in[10000],
int addr_out[10000],
int a[10000]);

extern "C" void AESL_WRAP_loop_imperfect (
int data[10000],
int addr_in[10000],
int addr_out[10000],
int a[10000])
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


		// output port post check: "data"
		aesl_fh.read(AUTOTB_TVOUT_PC_data, AESL_token); // [[transaction]]
		if (AESL_token != "[[transaction]]")
		{
			exit(1);
		}
		aesl_fh.read(AUTOTB_TVOUT_PC_data, AESL_num); // transaction number

		if (atoi(AESL_num.c_str()) == AESL_transaction_pc)
		{
			aesl_fh.read(AUTOTB_TVOUT_PC_data, AESL_token); // data

			sc_bv<32> *data_pc_buffer = new sc_bv<32>[10000];
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
							cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'X' on port 'data', possible cause: There are uninitialized variables in the C design." << endl;
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
							cerr << "WARNING: [SIM 212-201] RTL produces unknown value 'X' on port 'data', possible cause: There are uninitialized variables in the C design." << endl;
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
					data_pc_buffer[i] = AESL_token.c_str();
					i++;
				}

				aesl_fh.read(AUTOTB_TVOUT_PC_data, AESL_token); // data or [[/transaction]]

				if (AESL_token == "[[[/runtime]]]" || aesl_fh.eof(AUTOTB_TVOUT_PC_data))
				{
					exit(1);
				}
			}

			// ***********************************
			if (i > 0)
			{
				// RTL Name: data
				{
					// bitslice(31, 0)
					// {
						// celement: data(31, 0)
						// {
							sc_lv<32>* data_lv0_0_9999_1 = new sc_lv<32>[10000];
						// }
					// }

					// bitslice(31, 0)
					{
						int hls_map_index = 0;
						// celement: data(31, 0)
						{
							// carray: (0) => (9999) @ (1)
							for (int i_0 = 0; i_0 <= 9999; i_0 += 1)
							{
								if (&(data[0]) != NULL) // check the null address if the c port is array or others
								{
									data_lv0_0_9999_1[hls_map_index].range(31, 0) = sc_bv<32>(data_pc_buffer[hls_map_index].range(31, 0));
									hls_map_index++;
								}
							}
						}
					}

					// bitslice(31, 0)
					{
						int hls_map_index = 0;
						// celement: data(31, 0)
						{
							// carray: (0) => (9999) @ (1)
							for (int i_0 = 0; i_0 <= 9999; i_0 += 1)
							{
								// sub                    : i_0
								// ori_name               : data[i_0]
								// sub_1st_elem           : 0
								// ori_name_1st_elem      : data[0]
								// output_left_conversion : data[i_0]
								// output_type_conversion : (data_lv0_0_9999_1[hls_map_index]).to_uint64()
								if (&(data[0]) != NULL) // check the null address if the c port is array or others
								{
									data[i_0] = (data_lv0_0_9999_1[hls_map_index]).to_uint64();
									hls_map_index++;
								}
							}
						}
					}
				}
			}

			// release memory allocation
			delete [] data_pc_buffer;
		}

		AESL_transaction_pc++;
	}
	else
	{
		CodeState = ENTER_WRAPC;
		static unsigned AESL_transaction;

		static AESL_FILE_HANDLER aesl_fh;

		// "data"
		char* tvin_data = new char[50];
		aesl_fh.touch(AUTOTB_TVIN_data);
		char* tvout_data = new char[50];
		aesl_fh.touch(AUTOTB_TVOUT_data);

		// "addr_in"
		char* tvin_addr_in = new char[50];
		aesl_fh.touch(AUTOTB_TVIN_addr_in);

		// "addr_out"
		char* tvin_addr_out = new char[50];
		aesl_fh.touch(AUTOTB_TVIN_addr_out);

		CodeState = DUMP_INPUTS;
		static INTER_TCL_FILE tcl_file(INTER_TCL);
		int leading_zero;

		// [[transaction]]
		sprintf(tvin_data, "[[transaction]] %d\n", AESL_transaction);
		aesl_fh.write(AUTOTB_TVIN_data, tvin_data);

		sc_bv<32>* data_tvin_wrapc_buffer = new sc_bv<32>[10000];

		// RTL Name: data
		{
			// bitslice(31, 0)
			{
				int hls_map_index = 0;
				// celement: data(31, 0)
				{
					// carray: (0) => (9999) @ (1)
					for (int i_0 = 0; i_0 <= 9999; i_0 += 1)
					{
						// sub                   : i_0
						// ori_name              : data[i_0]
						// sub_1st_elem          : 0
						// ori_name_1st_elem     : data[0]
						// regulate_c_name       : data
						// input_type_conversion : data[i_0]
						if (&(data[0]) != NULL) // check the null address if the c port is array or others
						{
							sc_lv<32> data_tmp_mem;
							data_tmp_mem = data[i_0];
							data_tvin_wrapc_buffer[hls_map_index].range(31, 0) = data_tmp_mem.range(31, 0);
                                 	       hls_map_index++;
						}
					}
				}
			}
		}

		// dump tv to file
		for (int i = 0; i < 10000; i++)
		{
			sprintf(tvin_data, "%s\n", (data_tvin_wrapc_buffer[i]).to_string(SC_HEX).c_str());
			aesl_fh.write(AUTOTB_TVIN_data, tvin_data);
		}

		tcl_file.set_num(10000, &tcl_file.data_depth);
		sprintf(tvin_data, "[[/transaction]] \n");
		aesl_fh.write(AUTOTB_TVIN_data, tvin_data);

		// release memory allocation
		delete [] data_tvin_wrapc_buffer;

		// [[transaction]]
		sprintf(tvin_addr_in, "[[transaction]] %d\n", AESL_transaction);
		aesl_fh.write(AUTOTB_TVIN_addr_in, tvin_addr_in);

		sc_bv<32>* addr_in_tvin_wrapc_buffer = new sc_bv<32>[10000];

		// RTL Name: addr_in
		{
			// bitslice(31, 0)
			{
				int hls_map_index = 0;
				// celement: addr_in(31, 0)
				{
					// carray: (0) => (9999) @ (1)
					for (int i_0 = 0; i_0 <= 9999; i_0 += 1)
					{
						// sub                   : i_0
						// ori_name              : addr_in[i_0]
						// sub_1st_elem          : 0
						// ori_name_1st_elem     : addr_in[0]
						// regulate_c_name       : addr_in
						// input_type_conversion : addr_in[i_0]
						if (&(addr_in[0]) != NULL) // check the null address if the c port is array or others
						{
							sc_lv<32> addr_in_tmp_mem;
							addr_in_tmp_mem = addr_in[i_0];
							addr_in_tvin_wrapc_buffer[hls_map_index].range(31, 0) = addr_in_tmp_mem.range(31, 0);
                                 	       hls_map_index++;
						}
					}
				}
			}
		}

		// dump tv to file
		for (int i = 0; i < 10000; i++)
		{
			sprintf(tvin_addr_in, "%s\n", (addr_in_tvin_wrapc_buffer[i]).to_string(SC_HEX).c_str());
			aesl_fh.write(AUTOTB_TVIN_addr_in, tvin_addr_in);
		}

		tcl_file.set_num(10000, &tcl_file.addr_in_depth);
		sprintf(tvin_addr_in, "[[/transaction]] \n");
		aesl_fh.write(AUTOTB_TVIN_addr_in, tvin_addr_in);

		// release memory allocation
		delete [] addr_in_tvin_wrapc_buffer;

		// [[transaction]]
		sprintf(tvin_addr_out, "[[transaction]] %d\n", AESL_transaction);
		aesl_fh.write(AUTOTB_TVIN_addr_out, tvin_addr_out);

		sc_bv<32>* addr_out_tvin_wrapc_buffer = new sc_bv<32>[10000];

		// RTL Name: addr_out
		{
			// bitslice(31, 0)
			{
				int hls_map_index = 0;
				// celement: addr_out(31, 0)
				{
					// carray: (0) => (9999) @ (1)
					for (int i_0 = 0; i_0 <= 9999; i_0 += 1)
					{
						// sub                   : i_0
						// ori_name              : addr_out[i_0]
						// sub_1st_elem          : 0
						// ori_name_1st_elem     : addr_out[0]
						// regulate_c_name       : addr_out
						// input_type_conversion : addr_out[i_0]
						if (&(addr_out[0]) != NULL) // check the null address if the c port is array or others
						{
							sc_lv<32> addr_out_tmp_mem;
							addr_out_tmp_mem = addr_out[i_0];
							addr_out_tvin_wrapc_buffer[hls_map_index].range(31, 0) = addr_out_tmp_mem.range(31, 0);
                                 	       hls_map_index++;
						}
					}
				}
			}
		}

		// dump tv to file
		for (int i = 0; i < 10000; i++)
		{
			sprintf(tvin_addr_out, "%s\n", (addr_out_tvin_wrapc_buffer[i]).to_string(SC_HEX).c_str());
			aesl_fh.write(AUTOTB_TVIN_addr_out, tvin_addr_out);
		}

		tcl_file.set_num(10000, &tcl_file.addr_out_depth);
		sprintf(tvin_addr_out, "[[/transaction]] \n");
		aesl_fh.write(AUTOTB_TVIN_addr_out, tvin_addr_out);

		// release memory allocation
		delete [] addr_out_tvin_wrapc_buffer;

// [call_c_dut] ---------->

		CodeState = CALL_C_DUT;
		loop_imperfect(data, addr_in, addr_out, a);

		CodeState = DUMP_OUTPUTS;

		// [[transaction]]
		sprintf(tvout_data, "[[transaction]] %d\n", AESL_transaction);
		aesl_fh.write(AUTOTB_TVOUT_data, tvout_data);

		sc_bv<32>* data_tvout_wrapc_buffer = new sc_bv<32>[10000];

		// RTL Name: data
		{
			// bitslice(31, 0)
			{
				int hls_map_index = 0;
				// celement: data(31, 0)
				{
					// carray: (0) => (9999) @ (1)
					for (int i_0 = 0; i_0 <= 9999; i_0 += 1)
					{
						// sub                   : i_0
						// ori_name              : data[i_0]
						// sub_1st_elem          : 0
						// ori_name_1st_elem     : data[0]
						// regulate_c_name       : data
						// input_type_conversion : data[i_0]
						if (&(data[0]) != NULL) // check the null address if the c port is array or others
						{
							sc_lv<32> data_tmp_mem;
							data_tmp_mem = data[i_0];
							data_tvout_wrapc_buffer[hls_map_index].range(31, 0) = data_tmp_mem.range(31, 0);
                                 	       hls_map_index++;
						}
					}
				}
			}
		}

		// dump tv to file
		for (int i = 0; i < 10000; i++)
		{
			sprintf(tvout_data, "%s\n", (data_tvout_wrapc_buffer[i]).to_string(SC_HEX).c_str());
			aesl_fh.write(AUTOTB_TVOUT_data, tvout_data);
		}

		tcl_file.set_num(10000, &tcl_file.data_depth);
		sprintf(tvout_data, "[[/transaction]] \n");
		aesl_fh.write(AUTOTB_TVOUT_data, tvout_data);

		// release memory allocation
		delete [] data_tvout_wrapc_buffer;

		CodeState = DELETE_CHAR_BUFFERS;
		// release memory allocation: "data"
		delete [] tvin_data;
		delete [] tvout_data;
		// release memory allocation: "addr_in"
		delete [] tvin_addr_in;
		// release memory allocation: "addr_out"
		delete [] tvin_addr_out;

		AESL_transaction++;

		tcl_file.set_num(AESL_transaction , &tcl_file.trans_num);
	}
}

