This repository contains the source file of llvm passes intended to be run on SYCL/OpenCL FPGA code.
The passes are developed out-of-tree, and intended to be loaded as a .so by the llvm _opt_ tool.
The inputs/driver.sh scripts demonstrates how the passes can be integrated into a compile cycle.

---

**Pre-requisites:**

- CMake >=3.16
- ninja (https://github.com/ninja-build/ninja)
- Intel llvm sycl branch built from source
- Intel DPCPP compiler (2022.2 used). If you have apt: `sudo apt install intel-basekit`
- The basekit allows only emulation. If you want to run simulation, you can install the FPGA add-on for oneapi and the free Questa FPGA simulation software (https://www.intel.com/content/www/us/en/develop/documentation/oneapi-programming-guide/top/programming-interface/fpga-flow/evaluate-kernel-through-simulation/simulation-prerequisites.html). 
- To run in hardware, you can sign up to the Intel DevCloud to access Arria 10 and Stratix 10 FPGAs (https://www.intel.com/content/www/us/en/developer/tools/devcloud/overview.html)

**To build intel llvm sycl branch from source:**
- `mkdir -p ~/git && cd ~/git` 
- `git clone https://github.com/intel/llvm/tree/sycl` 
- `git checkout 40d08c238ce80e132df800ee21c3386f139fd85f` (newer commits not tested)
- `cd llvm && python buildbot/contfigure.py && python buildbot/compile.py` 

**To build the passes in this repo:**

- `cd ~/git`
- `mkdir -p build && cd build`
- `export LT_LLVM_INSTALL_DIR=$HOME/git/llvm/build/`
- `cmake ..`
- `make -j`
