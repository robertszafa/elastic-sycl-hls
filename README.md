## Overview:
This repository contains LLVM passes (under lib/) and a Load-Store Queue implementation (under LSQ/) that introduce dynamically scheduled memory operations into the Intel SYCL HLS tool. The instructions below describe how to build this code, use our compiler passes, and how to reproduce the benchmarks from the FPT23 paper.

---

## Build:

**Pre-requisites:**
- Ubuntu 20 or 22.
- gcc >= 7.1
- CMake >=3.16
- git
- Intel SYCL compiler. The version used in this project is 2023.1.0. To install it on Ubuntu: `sudo apt install intel-basekit-2023.1.0`
- RTL simulator on path (I used the Questa FPGA Starter Edition which is free but requires a free license). 
- (Optional) To run in hardware, I used the Intel DevCloud for free to access the Altera Arria 10AX115S FPGA.

**Installation:**
The install script downloads the intel/llvm github repo and builds it. Then it builds the passes from this repo out-of-tree.
```bash
export ELASTIC_SYCL_HLS_DIR=path/to/elastic_sycl_hls 
bash install.sh
```

Our compiler uses latency-insensitive channels and BRAM IPs provided by Altera. We change the channel write-to-read latency to 1 cycle, and we change the read-during-write behaviour of dual-port BRAM to return "old data", i.e. data before the store. The pipe change is purely for improved preformance, the BRAM change is needed to gurantee correctness of our BRAM LSQ.
```bash
mkdir $ELASTIC_SYCL_HLS_DIR/ip
cp $INTELFPGAOCLSDKROOT/ip/acl_channel_fifo.v $ELASTIC_SYCL_HLS_DIR/ip
cp $INTELFPGAOCLSDKROOT/ip/acl_mem1x.v $ELASTIC_SYCL_HLS_DIR/ip
sed -i 's/RDW_MODE\s*==/"OLD_DATA"==/g;s/= RDW_MODE/= "OLD_DATA"/g' $ELASTIC_SYCL_HLS_DIR/ip/acl_mem1x.v
sed -i 's/INTER_KERNEL_PIPELINING/0/g;s/0 = 0/INTER_KERNEL_PIPELINING = 0/g;s/(ALLOW_HIGH_SPEED_FIFO_USAGE ? "hs" : "ms")/("ll")/g' $ELASTIC_SYCL_HLS_DIR/ip/acl_channel_fifo.v
```

This script generates generic `compile_to_bc.sh` and `compile_from_bc.sh` scripts used to inject our passes into the SYCL compiler pipeline.
Remove undesired emu,sim,hw targets (the hw target takes a while).
The SYCL compiler is multi-pass -- it consists of multiple sub-commands 
to build a host and device binary. This script runs a full compilation in 
verbose mode, and then break up the compiler steps into constituent commands.
```bash
./scripts/compilation/gen_compile_scripts.py --targets=emu,sim
```

If you want to tun in hardware, then add the 'hw' target.
```bash
./scripts/compilation/gen_compile_scripts.py --targets=emu,sim,hw
```

---

## Use:

The optional '-d' flag doesn't delete temporary debug files.
```bash
elastic_pass.sh emu|sim|hw src_file [-d]
```

---

## Reproduce benchmarks FPT23:

The steps below will reproduce the csv files in FPT_benchmarks/expected_csv_files

### Build for simulation
```bash
cd $ELASTIC_SYCL_HLS_DIR/FPT_benchmarks
./build_all.py sim
```

### (Optional) Build for hardware
We provide Quartus reports from hardware compiles in 'intelHLS_reports' and 'our_reports' so that hardware compiles are not needed to reproduce the tables from the paper.
The hardware compiles were generated on the Intel devcloud (https://devcloud.intel.com/oneapi/get_started/) using Quartus 19.2 targetting the Altera Arria 10AX115S FPGA board.
```bash
./build_all.py hw

# Only needed for table 2:
./build_for_table_2.sh
```

### Generate table I

```bash
./run_sim_benchmarks.py bram
```
This script will build and run all benchmarks in simulation producing a 'our_results_bram.csv' table.
'our_results.csv' will contain results for the Intel HLS baseline and for our work.
Together, 'our_results.csv' and 'dynamatic_vivado_results.csv' contain all the information from the paper's Table I.
'dynamatic_vivado_results.csv' contain benchmark results for the Dynamatic HLS compiler, and Vivado 2019, respectively (obtained using 10.5281/zenodo.7406580).


### Generate table II

```bash
./gen_table_2.py bram
```
This will generate 'table_2.csv' with frequency and area numbers for varying store allocation queue sizes.


### Genearate table III (simulation runs)

```bash
./run_sim_benchmarks.py dram
```
The result of this script will be in 'our_results_dram.csv'. The results in the paper have been generated using harwdare runs, but simulation runs give very similar results (slight differences in DRAM behaviour).


### (Optional) Genearate table III (hardware runs)

If you have the hardware, you can reproduce table III using hardware runs. This will generate 'our_results_dram_in_hw.csv':
```bash
./run_hw_benchmark_dram.py
```
