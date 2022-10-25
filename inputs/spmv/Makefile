BENCHMARK := spmv

SRC := $(FILE)
# HDR := ""
BIN := bin/$(BENCHMARK)

I = ${HOME}/git/sycl-playground/include

CXX := dpcpp
CXXFLAGS += -std=c++17 -O2 -I$(I)
CXXFLAGS += -qactypes
# CXXFLAGS += --verbose
# CXXFLAGS += -Xsprofile
# CXXFLAGS += -g
# CXXFLAGS += -Xsghdl

HARDWARE_FLAGS := -Xshardware -DFPGA=1 
HARDWARE_FLAGS += -Xsboard=/opt/intel/oneapi/intel_a10gx_pac:pac_a10
# HARDWARE_FLAGS += -reuse-exe=$(BIN).fpga
# -Xssfc-exit-fifo-type=zero-latency

SIMULATION_FLAGS := -Xssimulation -DFPGA=1 
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
	$(CXX) $(CXXFLAGS) -fintelfpga $< -o $@ -DFPGA_EMULATOR=1

$(BIN).fpga_sim: $(BIN).dev.o | bin
	$(CXX) $(CXXFLAGS) -fintelfpga $< -o $@ $(SIMULATION_FLAGS)

$(BIN).fpga_hw: $(BIN).dev.o | bin
	$(CXX) $(CXXFLAGS) -fintelfpga $< -o $@ $(HARDWARE_FLAGS)

$(BIN).dev.o: $(SRC) | bin
	$(CXX) $(CXXFLAGS) -fintelfpga -c $< -o $@ -DFPGA=1 

# This is just for generating fpga resource report.
$(BIN).a:  $(BIN).dev.o | bin
	$(CXX) $(CXXFLAGS) -fintelfpga -fsycl-link $< -o $@ -Xshardware

# Make bin/ dir if doesn't exist
bin:
	mkdir -p $@

clean:
	rm -rf *.o *.d *.out *.mon *.emu *.aocr *.aoco *.prj *.fpga_emu *.fpga *.log *.a bin/*
