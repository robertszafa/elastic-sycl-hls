############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
open_project proj_loop_imperfect
set_top loop_imperfect
add_files loop_imperfect.c
add_files -tb loop_imperfect_test.c -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb result.golden.dat -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "solution1"
set_part {xc7k160t-fbg484-1}
create_clock -period 4 -name default
config_sdx -target none
config_interface -clock_enable=0 -expose_global=0 -m_axi_addr64=0 -m_axi_offset off -register_io off -trim_dangling_port=0
config_export -format ip_catalog -rtl verilog -use_netlist top -version 0.1.1 -vivado_optimization_level 3 -vivado_phys_opt all -vivado_report_level 2
config_schedule -effort high -enable_dsp_full_reg=0 -relax_ii_for_timing=0 -verbose=0
set_clock_uncertainty 12.5%
source "./proj_loop_imperfect/solution1/directives.tcl"
csim_design -O
csynth_design
cosim_design -O
export_design -flow impl -rtl verilog -format ip_catalog -version "0.1.1"
