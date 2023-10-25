
/home/dynamatic/opt/xilinx/Vivado/2019.2/bin/xelab xil_defaultlib.apatb_loop_imperfect_top glbl -prj loop_imperfect.prj -L smartconnect_v1_0 -L axi_protocol_checker_v1_1_12 -L axi_protocol_checker_v1_1_13 -L axis_protocol_checker_v1_1_11 -L axis_protocol_checker_v1_1_12 -L xil_defaultlib -L unisims_ver -L xpm --initfile "/home/dynamatic/opt/xilinx/Vivado/2019.2/data/xsim/ip/xsim_ip.ini" --lib "ieee_proposed=./ieee_proposed" -s loop_imperfect 
/home/dynamatic/opt/xilinx/Vivado/2019.2/bin/xsim --noieeewarnings loop_imperfect -tclbatch loop_imperfect.tcl

