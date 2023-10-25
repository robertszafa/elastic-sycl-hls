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
	{ vertices int 32 regular {array 2000 { 2 2 } 1 1 }  }
	{ edges int 32 regular {array 2000 { 1 1 } 1 1 }  }
}
set C_modelArgMapList {[ 
	{ "Name" : "vertices", "interface" : "memory", "bitwidth" : 32, "direction" : "READWRITE", "bitSlice":[{"low":0,"up":31,"cElement": [{"cName": "vertices","cData": "int","bit_use": { "low": 0,"up": 31},"cArray": [{"low" : 0,"up" : 1999,"step" : 1}]}]}]} , 
 	{ "Name" : "edges", "interface" : "memory", "bitwidth" : 32, "direction" : "READONLY", "bitSlice":[{"low":0,"up":31,"cElement": [{"cName": "edges","cData": "int","bit_use": { "low": 0,"up": 31},"cArray": [{"low" : 0,"up" : 1999,"step" : 1}]}]}]} ]}
# RTL Port declarations: 
set portNum 22
set portList { 
	{ ap_clk sc_in sc_logic 1 clock -1 } 
	{ ap_rst sc_in sc_logic 1 reset -1 active_high_sync } 
	{ ap_start sc_in sc_logic 1 start -1 } 
	{ ap_done sc_out sc_logic 1 predone -1 } 
	{ ap_idle sc_out sc_logic 1 done -1 } 
	{ ap_ready sc_out sc_logic 1 ready -1 } 
	{ vertices_address0 sc_out sc_lv 11 signal 0 } 
	{ vertices_ce0 sc_out sc_logic 1 signal 0 } 
	{ vertices_we0 sc_out sc_logic 1 signal 0 } 
	{ vertices_d0 sc_out sc_lv 32 signal 0 } 
	{ vertices_q0 sc_in sc_lv 32 signal 0 } 
	{ vertices_address1 sc_out sc_lv 11 signal 0 } 
	{ vertices_ce1 sc_out sc_logic 1 signal 0 } 
	{ vertices_we1 sc_out sc_logic 1 signal 0 } 
	{ vertices_d1 sc_out sc_lv 32 signal 0 } 
	{ vertices_q1 sc_in sc_lv 32 signal 0 } 
	{ edges_address0 sc_out sc_lv 11 signal 1 } 
	{ edges_ce0 sc_out sc_logic 1 signal 1 } 
	{ edges_q0 sc_in sc_lv 32 signal 1 } 
	{ edges_address1 sc_out sc_lv 11 signal 1 } 
	{ edges_ce1 sc_out sc_logic 1 signal 1 } 
	{ edges_q1 sc_in sc_lv 32 signal 1 } 
}
set NewPortList {[ 
	{ "name": "ap_clk", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "clock", "bundle":{"name": "ap_clk", "role": "default" }} , 
 	{ "name": "ap_rst", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "reset", "bundle":{"name": "ap_rst", "role": "default" }} , 
 	{ "name": "ap_start", "direction": "in", "datatype": "sc_logic", "bitwidth":1, "type": "start", "bundle":{"name": "ap_start", "role": "default" }} , 
 	{ "name": "ap_done", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "predone", "bundle":{"name": "ap_done", "role": "default" }} , 
 	{ "name": "ap_idle", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "done", "bundle":{"name": "ap_idle", "role": "default" }} , 
 	{ "name": "ap_ready", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "ready", "bundle":{"name": "ap_ready", "role": "default" }} , 
 	{ "name": "vertices_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":11, "type": "signal", "bundle":{"name": "vertices", "role": "address0" }} , 
 	{ "name": "vertices_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "vertices", "role": "ce0" }} , 
 	{ "name": "vertices_we0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "vertices", "role": "we0" }} , 
 	{ "name": "vertices_d0", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "vertices", "role": "d0" }} , 
 	{ "name": "vertices_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "vertices", "role": "q0" }} , 
 	{ "name": "vertices_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":11, "type": "signal", "bundle":{"name": "vertices", "role": "address1" }} , 
 	{ "name": "vertices_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "vertices", "role": "ce1" }} , 
 	{ "name": "vertices_we1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "vertices", "role": "we1" }} , 
 	{ "name": "vertices_d1", "direction": "out", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "vertices", "role": "d1" }} , 
 	{ "name": "vertices_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "vertices", "role": "q1" }} , 
 	{ "name": "edges_address0", "direction": "out", "datatype": "sc_lv", "bitwidth":11, "type": "signal", "bundle":{"name": "edges", "role": "address0" }} , 
 	{ "name": "edges_ce0", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "edges", "role": "ce0" }} , 
 	{ "name": "edges_q0", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "edges", "role": "q0" }} , 
 	{ "name": "edges_address1", "direction": "out", "datatype": "sc_lv", "bitwidth":11, "type": "signal", "bundle":{"name": "edges", "role": "address1" }} , 
 	{ "name": "edges_ce1", "direction": "out", "datatype": "sc_logic", "bitwidth":1, "type": "signal", "bundle":{"name": "edges", "role": "ce1" }} , 
 	{ "name": "edges_q1", "direction": "in", "datatype": "sc_lv", "bitwidth":32, "type": "signal", "bundle":{"name": "edges", "role": "q1" }}  ]}

set RtlHierarchyInfo {[
	{"ID" : "0", "Level" : "0", "Path" : "`AUTOTB_DUT_INST", "Parent" : "",
		"CDFG" : "loop_imperfect",
		"Protocol" : "ap_ctrl_hs",
		"ControlExist" : "1", "ap_start" : "1", "ap_ready" : "1", "ap_done" : "1", "ap_continue" : "0", "ap_idle" : "1",
		"Pipeline" : "None", "UnalignedPipeline" : "0", "RewindPipeline" : "0", "ProcessNetwork" : "0",
		"II" : "0",
		"VariableLatency" : "1", "ExactLatency" : "-1", "EstimateLatencyMin" : "6001", "EstimateLatencyMax" : "6001",
		"Combinational" : "0",
		"Datapath" : "0",
		"ClockEnable" : "0",
		"HasSubDataflow" : "0",
		"InDataflowNetwork" : "0",
		"HasNonBlockingOperation" : "0",
		"Port" : [
			{"Name" : "vertices", "Type" : "Memory", "Direction" : "IO"},
			{"Name" : "edges", "Type" : "Memory", "Direction" : "I"}]}]}


set ArgLastReadFirstWriteLatency {
	loop_imperfect {
		vertices {Type IO LastRead 4 FirstWrite 5}
		edges {Type I LastRead 2 FirstWrite -1}}}

set hasDtUnsupportedChannel 0

set PerformanceInfo {[
	{"Name" : "Latency", "Min" : "6001", "Max" : "6001"}
	, {"Name" : "Interval", "Min" : "6002", "Max" : "6002"}
]}

set PipelineEnableSignalInfo {[
]}

set Spec2ImplPortList { 
	vertices { ap_memory {  { vertices_address0 mem_address 1 11 }  { vertices_ce0 mem_ce 1 1 }  { vertices_we0 mem_we 1 1 }  { vertices_d0 mem_din 1 32 }  { vertices_q0 mem_dout 0 32 }  { vertices_address1 MemPortADDR2 1 11 }  { vertices_ce1 MemPortCE2 1 1 }  { vertices_we1 MemPortWE2 1 1 }  { vertices_d1 MemPortDIN2 1 32 }  { vertices_q1 MemPortDOUT2 0 32 } } }
	edges { ap_memory {  { edges_address0 mem_address 1 11 }  { edges_ce0 mem_ce 1 1 }  { edges_q0 mem_dout 0 32 }  { edges_address1 MemPortADDR2 1 11 }  { edges_ce1 MemPortCE2 1 1 }  { edges_q1 MemPortDOUT2 0 32 } } }
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
