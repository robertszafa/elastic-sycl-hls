
read_vhdl -vhdl2008 ../sim/VHDL_SRC/arithmetic_units.vhd
read_vhdl -vhdl2008 ../sim/VHDL_SRC/delay_buffer.vhd
read_vhdl -vhdl2008 ../sim/VHDL_SRC/elastic_components.vhd
read_vhdl -vhdl2008 ../sim/VHDL_SRC/MemCont.vhd
read_vhdl -vhdl2008 ../sim/VHDL_SRC/multipliers.vhd
read_vhdl -vhdl2008 ../sim/VHDL_SRC/mul_wrapper.vhd
read_verilog  ../sim/VHDL_SRC/LSQ_A.v


#read_vhdl -vhdl2008 ../../components/array_RAM_fadd_32bkb.vhd
#read_vhdl -vhdl2008 ../../../components/array_RAM_fsub_32bkb.vhd
#read_vhdl -vhdl2008 ../../components/array_RAM_fmul_32cud.vhd
#read_vhdl -vhdl2008 ../../components/array_RAM_fcmp_32cud.vhd

#source ../../components/array_RAM_ap_fadd_8_full_dsp_32_ip.tcl
#source ../../components/array_RAM_ap_fcmp_0_no_dsp_32_ip.tcl
#source ../../../components/array_RAM_ap_fsub_8_full_dsp_32_ip.tcl
#source ../../components/array_RAM_ap_fdiv_28_no_dsp_32_ip.tcl
#source ../../components/array_RAM_ap_fmul_4_max_dsp_32_ip.tcl


read_vhdl -vhdl2008 ../sim/VHDL_SRC/getTanh_double_optimized.vhd


#synth_design -mode out_of_context -flatten_hierarchy rebuilt -top array_RAM -part 7k160tfbg484-1
#comment line below out if no clock constraints
read_xdc period_4.xdc

synth_design -top getTanh_double -part xc7k160tfbg484-1 -no_iobuf -mode out_of_context

report_utilization > utilization_post_syn.rpt
report_timing > timing_post_syn.rpt
opt_design
place_design
route_design
report_utilization > utilization_post_pr.rpt
report_timing > timing_post_pr.rpt

