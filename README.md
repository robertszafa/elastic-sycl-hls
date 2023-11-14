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

The below install script downloads the intel/llvm github repo and builds it. Then it builds the passes from this repo out-of-tree.
```bash
export ELASTIC_SYCL_HLS_DIR=path/to/elastic_sycl_hls 
bash install.sh
```

The below script generates generic `compile_to_bc.sh` and `compile_from_bc.sh` scripts used to inject our passes into the SYCL compiler pipeline.
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

**Using this code in the DevCloud:**
- Find free nodes with an Arria 10 FPGA: `pbsnodes :arria10 | grep "state = free" -B 1 -A 1`
- Log into a selected node for 6 hrs (can choose up to 24): `qsub -I -l nodes=s001-n084:ppn=2 -l walltime=06:00:00 -d .`
- Initialize tools: `source /glob/development-tools/versions/oneapi/2023.1.1/oneapi/setvars.sh --force > /dev/null 2>&1`
- Install this repo: `install.sh`
- No need to run `gen_compile_scripts.py` in the devcloud. We have provided `compile_to_bc.sh` and `compile_from_bc.sh` scripts for the devcloud in this repo.
- Use `elastic_pass.sh hw <src-code>`

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
