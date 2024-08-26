## Benchmarks:

**Build benchmark codes:**
```bash
# simulation builds used to obtain cycle counts
python3 experiments/build_bench.py sim
python3 experiments/build_bench_num_poison_blocks.py sim

# hardware builds with full place & route used to obtain area usage
python3 experiments/build_bench.py hw
python3 experiments/build_bench_num_poison_blocks.py hw
``` 

**Collect cycle counts:**
```bash
python3 experiments/run_bench.py sim # Performance of benchmarks
python3 experiments/run_bench_misspec_cost.py sim
python3 experiments/run_bench_num_poison_blocks.py sim
``` 

**Plot figures and generate latex table:**
```bash
python3 experiments/plot_performance.py
python3 experiments/plot_misspeculation_cost.py
python3 experiments/plot_performance_num_poison_blocks.py
python3 experiments/gen_latex_table.py
``` 



## Build:

**Pre-requisites:**
- Ubuntu 20 or 22.
- gcc >= 7.1
- CMake >=3.16
- git
- Intel SYCL compiler. The version used in this project is 2023.1.0. To install it on Ubuntu: `sudo apt install intel-basekit-2023.1.0`
- RTL simulator on path. I used the Questa FPGA Starter Edition which is free but requires a free license. Install Quartus 19.2 together with Arria 10 device support, the pac_a10 board support packege, and OneAPI support (https://www.intel.com/content/www/us/en/software-kit/661712/intel-quartus-prime-pro-edition-design-software-version-19-2-for-linux.html).
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
