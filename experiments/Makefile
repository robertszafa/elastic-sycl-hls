# Usage:
#	  make target FILE={benchmark_name.cpp}
#     Targets: fpga_emu, fpga_sim, fpga_hw, host (kernel runs on cpu)
#     Make output: bin/benchmark_name.target

BENCHMARK := $(basename $(FILE))
BIN := bin/$(BENCHMARK)

I = -I${ELASTIC_SYCL_HLS_DIR}/include_sycl
I += -I${ELASTIC_SYCL_HLS_DIR}/lsq

CXX := icpx
CXXFLAGS += -fsycl -std=c++17 -O2 $(I) 
# use IEEE floating point exactly (easier testing against CPU execution)
CXXFLAGS += -fp-model=precise 
# ac_int.h header (to use algorithmic C datatypes -- custom bit-width numerics).
CXXFLAGS += -qactypes 

# Arria 10 fpga
FPGA_TARGET := -Xsboard=intel_a10gx_pac:pac_a10

HARDWARE_FLAGS := -DFPGA_HW -Xshardware $(FPGA_TARGET)
# HARDWARE_FLAGS += -reuse-exe=$(BIN).fpga_hw	# If only the host code changed.

SIMULATION_FLAGS := -DFPGA_SIM -Xssimulation $(FPGA_TARGET)
# SIMULATION_FLAGS += -reuse-exe=$(BIN).fpga_sim
# SIMULATION_FLAGS += -Xsghdl	# To generate waveforms during simulation.

.PHONY: host fpga_emu fpga_sim fpga_hw

all: host
host: $(BIN)
fpga_emu: $(BIN).fpga_emu
fpga_sim: $(BIN).fpga_sim
fpga_hw: $(BIN).fpga_hw
report: $(BIN).a 

$(BIN): $(FILE) | bin
	$(CXX) $(CXXFLAGS) -o $@ $(FILE)

$(BIN).fpga_emu: $(FILE) | bin
	$(CXX) $(CXXFLAGS) -fintelfpga -DFPGA_EMU $< -o $@

$(BIN).fpga_sim: $(FILE) | bin
	$(CXX) $(CXXFLAGS) -fintelfpga $(SIMULATION_FLAGS) $< -o $@ 

$(BIN).fpga_hw: $(FILE) | bin
	$(CXX) $(CXXFLAGS) -fintelfpga $(HARDWARE_FLAGS) $< -o $@ 

$(BIN).a:  $(FILE) | bin
	$(CXX) $(CXXFLAGS) -fintelfpga $(HARDWARE_FLAGS) -fsycl-link=early $< -o $@ 

# Make bin/ dir if it doesn't exist
bin:
	mkdir -p $@

clean:
	rm -rf *.bc *.ll *.json *.tmp.* bin *_workdir
