## Overview:
This repository contains llvm passes which selectively introduce dynamic scheduling into SYCL HLS code.
The passes are developed out-of-tree, and intended to be loaded as a .so by the llvm _opt_ tool.

---

## Build:

**Pre-requisites:**
- CMake >=3.16
- ninja (https://github.com/ninja-build/ninja)
- Intel llvm sycl branch built from source
- Intel SYCL compiler (the driver is invoked with `icpx -fsycl`). The version used in this project is 2023.1.0. To install it on Ubuntu: `sudo apt install intel-basekit-2023.1.0`
- The basekit allows only emulation. If you want to run simulation, you have to install an rtl simulator (I used Questa FPGA Starter Edition, which is free and works well enough for small designs). 
- To run in hardware, you can sign up to the Intel DevCloud for free to access Arria 10 and Stratix 10 FPGAs.

**Build the intel llvm sycl branch from source:**
```bash
git clone https://github.com/intel/llvm.git && cd llvm
# The LLVM version should match the version used to build the SYCL compiler: 
# e.g. for intel-basekit-2023.1.0 the LLVM intel/sycl commit hash is 756ba26161. 
git checkout 756ba26161
python buildbot/configure.py --llvm-external-projects=clang-tools-extra && python buildbot/compile.py
cd build && ninja 
```

**Build the passes in this repo:**
```bash
# Add to your environment variables:
export LT_LLVM_INSTALL_DIR=path/to/llvm/build
export ELASTIC_SYCL_HLS_DIR=path/to/elastic_sycl_hls 
mkdir -p build && cd build
cmake ..
make -j
```

**Setup SYCL HLS compilation scripts:**

The SYCL compiler is multi-pass -- it consists of multiple sub-commands to build a host and device binary. To run the LLVM passes from this repo, we need to break up the driver into constituent commands such that we can run our passes on the kernel code and compile the transformed LLVM IR code into the final binary. The `genCompileScripts.py` runs the dpcpp driver in verbose mode and generates generic `compile_to_bc.sh` and `compile_from_bc.sh` scripts which automate this process.
```bash
# Use --targets=emu,sim,hw to choose which targets should be generated.
$ELASTIC_SYCL_HLS_DIR/scripts/compilation/gen_compile_scripts.py`
```

---

## Run transformations:

```bash
# The optional -d flag ensures that compiler generated files are stored in file_workdir for later inspection.
elastic_pass.sh emu|sim|hw src_file [-d]
```
