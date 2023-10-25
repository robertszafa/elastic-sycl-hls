set moduleName loop_imperfect
set isTopModule 1
set isTaskLevelControl 1
set isCombinational 0
set isDatapathOnly 0
set isFreeRunPipelineModule 0
set isPipelined 0
set pipeline_type none
set FunctionProtocol ap_ctrl_hs
set isOneStateSeq 0
set ProfileFlag 0
set StallSigGenFlag 0
set isEnableWaveformDebug 1
set C_modelName {loop_imperfect}
set C_modelType { void 0 }
set C_modelArgList {
	{ matrix int 32 regular {array 400 { 2 2 } 1 1 }  }
	{ row int 32 regular {array 400 { 1 1 } 1 1 }  }
	{ col int 32 regular {array 400 { 1 1 } 1 1 }  }
	{ a int 32 regular {array 400 { 1 1 } 1 1 }  }
}
set C_modelArgMapList {[ 
	{ "Name" : "matrix", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE", "bitSlice":[{"low":0,"up":31,"cElement": [{"cName": "matrix","cData": "int","bit_use": { "low": 0,"up": 31},"cArray": [{"low" : 0,"up" : 399,"step" : 1}]}]}]} , 
 	{ "Name" : "row", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY", "bitSlice":[{"low":0,"up":31,"cElement": [{"cName": "row","cData": "int","bit_use": { "low": 0,"up": 31},"cArray": [{"low" : 0,"up" : 399,"step" : 1}]}]}]} , 
 	{ "Name" : "col", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY", "bitSlice":[{"low":0,"up":31,"cElement": [{"cName": "col","cData": "int","bit_use": { "low": 0,"up": 31},"cArray": [{"low" : 0,"up" : 399,"step" : 1}]}]}]} , 
 	{ "Name" : "a", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY", "bitSlice":[{"low":0,"up":31,"cElement": [{"cName": "a","cData": "int","bit_use": { "low": 0,"up": 31},"cArray": [{"low" : 0,"up" : 399,"step" : 1}]}]}]} ]}
# RTL Port declarations: 
set portNum 34
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ matrix_address0 sc_out sc_lv 9 signal 0 } 
	{ matrix_ce0 sc_out sc_logic 1 signal 0 } 
	{ matrix_we0 sc_out sc_logic 1 signal 0 } 
	{ matrix_d0 sc_out sc_lv 32 signal 0 } 
	{ matrix_q0 sc_in sc_lv 32 signal 0 } 
	{ matrix_address1 sc_out sc_lv 9 signal 0 } 
	{ matrix_ce1 sc_out sc_logic 1 signal 0 } 
	{ matrix_we1 sc_out sc_logic 1 signal 0 } 
	{ matrix_d1 sc_out sc_lv 32 signal 0 } 
	{ matrix_q1 sc_in sc_lv 32 signal 0 } 
	{ row_address0 sc_out sc_lv 9 signal 1 } 
	{ row_ce0 sc_out sc_logic 1 signal 1 } 
	{ row_q0 sc_in sc_lv 32 signal 1 } 
	{ row_address1 sc_out sc_lv 9 signal 1 } 
	{ row_ce1 sc_out sc_logic 1 signal 1 } 
	{ row_q1 sc_in sc_lv 32 signal 1 } 
	{ col_address0 sc_out sc_lv 9 signal 2 } 
	{ col_ce0 sc_out sc_logic 1 signal 2 } 
	{ col_q0 sc_in sc_lv 32 signal 2 } 
	{ col_address1 sc_out sc_lv 9 signal 2 } 
	{ col_ce1 sc_out sc_logic 1 signal 2 } 
	{ col_q1 sc_in sc_lv 32 signal 2 } 
	{ a_address0 sc_out sc_lv 9 signal 3 } 
	{ a_ce0 sc_out sc_logic 1 signal 3 } 
	{ a_q0 sc_in sc_lv 32 signal 3 } 
	{ a_address1 sc_out sc_lv 9 signal 3 } 
	{ a_ce1 sc_out sc_logic 1 signal 3 } 
	{ a_q1 sc_in sc_lv 32 signal 3 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "matrix_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "matrix", "role": "address0" }} , 
 	{ "name": "matrix_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "matrix", "role": "ce0" }} , 
 	{ "name": "matrix_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "matrix", "role": "we0" }} , 
 	{ "name": "matrix_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "matrix", "role": "d0" }} , 
 	{ "name": "matrix_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "matrix", "role": "q0" }} , 
 	{ "name": "matrix_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "matrix", "role": "address1" }} , 
 	{ "name": "matrix_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "matrix", "role": "ce1" }} , 
 	{ "name": "matrix_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "matrix", "role": "we1" }} , 
 	{ "name": "matrix_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "matrix", "role": "d1" }} , 
 	{ "name": "matrix_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "matrix", "role": "q1" }} , 
 	{ "name": "row_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "row", "role": "address0" }} , 
 	{ "name": "row_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "row", "role": "ce0" }} , 
 	{ "name": "row_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "row", "role": "q0" }} , 
 	{ "name": "row_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "row", "role": "address1" }} , 
 	{ "name": "row_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "row", "role": "ce1" }} , 
 	{ "name": "row_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "row", "role": "q1" }} , 
 	{ "name": "col_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "col", "role": "address0" }} , 
 	{ "name": "col_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "col", "role": "ce0" }} , 
 	{ "name": "col_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "col", "role": "q0" }} , 
 	{ "name": "col_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "col", "role": "address1" }} , 
 	{ "name": "col_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "col", "role": "ce1" }} , 
 	{ "name": "col_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "col", "role": "q1" }} , 
 	{ "name": "a_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "a", "role": "address0" }} , 
 	{ "name": "a_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "ce0" }} , 
 	{ "name": "a_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "a", "role": "q0" }} , 
 	{ "name": "a_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":9, "type": "signal", "bundle":{"name": "a", "role": "address1" }} , 
 	{ "name": "a_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "a", "role": "ce1" }} , 
 	{ "name": "a_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "a", "role": "q1" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "", "Child" : ["1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20"],
		"CDFG" : "loop_imperfect",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "2282", "EstimateLatencyMax" : "2282",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "matrix", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "row", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "col", "Type" : "Memory", "Direction" : "I"},
			{"Name" : "a", "Type" : "Memory", "Direction" : "I"}]},
	{"ID" : "1", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U1", "Parent" : "0"},
	{"ID" : "2", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U2", "Parent" : "0"},
	{"ID" : "3", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U3", "Parent" : "0"},
	{"ID" : "4", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U4", "Parent" : "0"},
	{"ID" : "5", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U5", "Parent" : "0"},
	{"ID" : "6", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U6", "Parent" : "0"},
	{"ID" : "7", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U7", "Parent" : "0"},
	{"ID" : "8", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U8", "Parent" : "0"},
	{"ID" : "9", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U9", "Parent" : "0"},
	{"ID" : "10", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U10", "Parent" : "0"},
	{"ID" : "11", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U11", "Parent" : "0"},
	{"ID" : "12", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U12", "Parent" : "0"},
	{"ID" : "13", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U13", "Parent" : "0"},
	{"ID" : "14", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U14", "Parent" : "0"},
	{"ID" : "15", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U15", "Parent" : "0"},
	{"ID" : "16", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U16", "Parent" : "0"},
	{"ID" : "17", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U17", "Parent" : "0"},
	{"ID" : "18", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U18", "Parent" : "0"},
	{"ID" : "19", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U19", "Parent" : "0"},
	{"ID" : "20", "Level" : "1", "Path" : "`AUTOTB_DUT_INST.loop_imperfect_mubkb_U20", "Parent" : "0"}]}


set ArgLastReadFirstWriteLatency {
	loop_imperfect {
		matrix {Type IO LastRead 117 FirstWrite 7}
		row {Type I LastRead 11 FirstWrite -1}
		col {Type I LastRead 11 FirstWrite -1}
		a {Type I LastRead 11 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "2282", "Max" : "2282"}
	, {"Name" : "Interval", "Min" : "2283", "Max" : "2283"}
]}

set PipelineEnableSignalInfo {[
	{"Pipeline" : "0", "EnableSignal" : "ap_enable_pp0"}
]}

set Spec2ImplPortList { 
	matrix { ap_memory {  { matrix_address0 mem_address 1 9 }  { matrix_ce0 mem_ce 1 1 }  { matrix_we0 mem_we 1 1 }  { matrix_d0 mem_din 1 32 }  { matrix_q0 mem_dout 0 32 }  { matrix_address1 MemPortADDR2 1 9 }  { matrix_ce1 MemPortCE2 1 1 }  { matrix_we1 MemPortWE2 1 1 }  { matrix_d1 MemPortDIN2 1 32 }  { matrix_q1 MemPortDOUT2 0 32 } } }
	row { ap_memory {  { row_address0 mem_address 1 9 }  { row_ce0 mem_ce 1 1 }  { row_q0 mem_dout 0 32 }  { row_address1 MemPortADDR2 1 9 }  { row_ce1 MemPortCE2 1 1 }  { row_q1 MemPortDOUT2 0 32 } } }
	col { ap_memory {  { col_address0 mem_address 1 9 }  { col_ce0 mem_ce 1 1 }  { col_q0 mem_dout 0 32 }  { col_address1 MemPortADDR2 1 9 }  { col_ce1 MemPortCE2 1 1 }  { col_q1 MemPortDOUT2 0 32 } } }
	a { ap_memory {  { a_address0 mem_address 1 9 }  { a_ce0 mem_ce 1 1 }  { a_q0 mem_dout 0 32 }  { a_address1 MemPortADDR2 1 9 }  { a_ce1 MemPortCE2 1 1 }  { a_q1 MemPortDOUT2 0 32 } } }
}

set busDeadlockParameterList { 
}

# RTL port scheduling information:
set fifoSchedulingInfoList { 
}

# RTL bus port read request latency information:
set busReadReqLatencyList { 
}

# RTL bus port write response latency information:
set busWriteResLatencyList { 
}

# RTL array port load latency information:
set memoryLoadLatencyList { 
}
