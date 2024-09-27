#!/usr/bin/env python3

import os
import sys

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]
BENCHMARKS = [
    "raw_loop",
    "war_loop",
    "waw_loop",

    "gemver_dram",
    "fft2x_dram",

    "bnn_dram",
    "matrix_power_dram",
    "page_rank_dram",

    "hist2x_dram",
    "tanh_spmv_dram",

    # To regular to get a benefit, existing approaches are better than dynamic fusion:
    # "spmv_sort_dram",
    # "fft_conv_dram",
    # "scale_fw_dram",
    # "gsum_sort_dram",

    # "correlation_dram", 
    # "gemm_dram",
    # "kernel_2mm_dram",
    # "kernel_3mm_dram",
    # "doitgen_triple_dram",
    # "lud_dram",

    # simple RAW/WAW/WAR loops but enclosed in more outer loops.
    # "test_fusion_raw",
    # "test_fusion_war",
    # "test_fusion_waw",
]

if __name__ == '__main__':
    TARGET = sys.argv[1]
    if TARGET not in ['emu', 'sim', 'hw']:
        exit("sys.argv[1] must be emu/sim/hw")

    for kernel in BENCHMARKS:
        os.system(f'echo "--------------------- {kernel} ---------------------"')
        
        os.system('printf "\n============== Static =============\n"')
        os.system(f'cd {GIT_DIR}/experiments && make fpga_{TARGET} FILE={kernel}.cpp')

        os.system('printf "\n============== LSQ =============\n"')
        os.system(f'cp {GIT_DIR}/experiments/{kernel}.cpp {GIT_DIR}/experiments/{kernel}_lsq.cpp')
        os.system(f'NO_PE_DECOUPLING=1 {GIT_DIR}/elastic_pass.sh {TARGET} {GIT_DIR}/experiments/{kernel}_lsq.cpp')

        os.system('printf "\n============== FUSION_NO_FRWD =============\n"')
        os.system(f'cp {GIT_DIR}/experiments/{kernel}.cpp {GIT_DIR}/experiments/{kernel}_nofrwd.cpp')
        os.system(f'NO_FUSION_FRWD=1 {GIT_DIR}/elastic_pass.sh {TARGET} {GIT_DIR}/experiments/{kernel}_nofrwd.cpp -f')

        os.system('printf "\n============== Fusion =============\n"')
        os.system(f'{GIT_DIR}/elastic_pass.sh {TARGET} {GIT_DIR}/experiments/{kernel}.cpp -f')
