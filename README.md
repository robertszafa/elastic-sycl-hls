## Overview:
This repository contains llvm passes which selectively introduce dynamic scheduling into SYCL HLS code. You can read [our paper](https://arxiv.org/pdf/2308.15120.pdf) for more details. 

If you want to cite this work:
```
@inproceedings{szafarczyk2023fpl,
  title={Compiler Discovered Dynamic Scheduling of Irregular Code in High-Level Synthesis}, 
  author={Szafarczyk, Robert and Nabi, Syed Waqar and Vanderbauwhede, Wim},
  booktitle={2023 33rd International Conference on Field-Programmable Logic and Applications (FPL)}, 
  year={2023},
}
```

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

---

## Use:

The optional '-d' flag ensures that compiler generated files are not deleted.
```bash
export ELASTIC_SYCL_HLS_DIR=path/to/elastic_sycl_hls 
elastic_pass.sh emu|sim|hw src_file [-d]
```

To run benchmarks targetted by the passes.
```bash
export ELASTIC_SYCL_HLS_DIR=path/to/elastic_sycl_hls 
cd experiments
./test_all_in_sim.py
```
