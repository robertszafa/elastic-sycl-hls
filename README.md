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
- gcc >= 7.1
- CMake >=3.16
- git
- Intel SYCL compiler. The version used in this project is 2023.1.0. To install it on Ubuntu: `sudo apt install intel-basekit-2023.1.0`
- If you want to run in simulation, you have to install an rtl simulator (e.g., Questa FPGA Starter Edition is free and works well enough for small designs). 
- To run in hardware, you can sign up to the Intel DevCloud for free to access Arria 10 and Stratix 10 FPGAs.

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
