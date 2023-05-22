BENCHMARK := $(basename $(FILE))

SRC := $(FILE)
BIN := bin/$(BENCHMARK)

I = -I${ELASTIC_SYCL_HLS_DIR}/include_sycl
I += -I${ELASTIC_SYCL_HLS_DIR}/lsq

CXX := icpx
CXXFLAGS += -fsycl -std=c++17 -O2 $(I)
CXXFLAGS += -qactypes
CXXFLAGS += -v --verbose

FPGA_TARGET := -Xsboard=intel_a10gx_pac:pac_a10
HARDWARE_FLAGS := -DFPGA_HW -Xshardware $(FPGA_TARGET)
# HARDWARE_FLAGS += -reuse-exe=$(BIN).fpga_hw
SIMULATION_FLAGS := -DFPGA_SIM -Xssimulation $(FPGA_TARGET)
# SIMULATION_FLAGS += -Xsghdl
# SIMULATION_FLAGS += -reuse-exe=$(BIN).fpga_sim

.PHONY: host fpga_emu fpga_hw

all: host
host: $(BIN)
fpga_emu: $(BIN).fpga_emu
fpga_sim: $(BIN).fpga_sim
fpga_hw: $(BIN).fpga_hw
report: $(BIN).a 


$(BIN): $(SRC) | bin
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

$(BIN).fpga_emu: $(SRC) | bin
	$(CXX) $(CXXFLAGS) -fintelfpga -DFPGA_EMU $< -o $@

$(BIN).fpga_sim: $(SRC) | bin
	$(CXX) $(CXXFLAGS) -fintelfpga $(SIMULATION_FLAGS) $< -o $@ 

$(BIN).fpga_hw: $(SRC) | bin
	$(CXX) $(CXXFLAGS) -fintelfpga $(HARDWARE_FLAGS) $< -o $@ 

$(BIN).a:  $(SRC) | bin
	$(CXX) $(CXXFLAGS) -fintelfpga $(HARDWARE_FLAGS) -fsycl-link=early $< -o $@ 

# Make bin/ dir if it doesn't exist
bin:
	mkdir -p $@

clean:
	rm -rf *.o *.d *.out *.mon *.emu *.aocr *.aoco *.prj *.fpga_emu *.fpga *.log *.a bin/*
