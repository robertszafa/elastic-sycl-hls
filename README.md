## Overview:
This repository contains llvm passes which selectively introduce dynamic scheduling into SYCL HLS code.
The passes are developed out-of-tree, and intended to be loaded as a .so by the llvm _opt_ tool.

---

## Build:

**Pre-requisites:**
- gcc >= 7.1
- CMake >=3.16
- git
- Intel SYCL compiler. The version used in this project is 2023.1.0. To install it on Ubuntu: `sudo apt install intel-basekit-2023.1.0`
- If you want to run in simulation, you have to install an rtl simulator (I used Questa FPGA Starter Edition, which is free and works well enough for small designs). 
- To run in hardware, you can sign up to the Intel DevCloud for free to access Arria 10 and Stratix 10 FPGAs.

**Installation:**
```bash
# This env variable should also be set when using the passes. 
export ELASTIC_SYCL_HLS_DIR=path/to/elastic_sycl_hls 
```

```bash
# Installs llvm/sycl and builds the code from this repo.
bash install.sh
```

```bash
# Generates generic `compile_to_bc.sh` and `compile_from_bc.sh`.
# Choose desired targets: emu,sim,hw. (The hw target can take a while).
# The SYCL compiler is multi-pass -- it consists of multiple sub-commands 
# to build a host and device binary. This script runs a full compilation in 
# verbose mode, and then break up the compiler steps into constituent commands.
./scripts/compilation/gen_compile_scripts.py --targets=emu,sim
```

```bash
# To save time, do the two in parallel:
bash install.sh & ./scripts/compilation/gen_compile_scripts.py --targets=emu,sim
```

---

## Use:

```bash
# The optional -d flag ensures that compiler generated files are stored in file_workdir for later inspection.
elastic_pass.sh emu|sim|hw src_file [-d]
```
