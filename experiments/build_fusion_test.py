#!/usr/bin/env python3

import os
import sys

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]
BENCHMARKS = [
    # Synthetic
    "test_fusion_raw",
    "test_fusion_war",
    "test_fusion_waw",

    "page_rank_dram",
    "bnn_dram",

    "fft_conv_dram",
    "gsum_sort_dram",

    "gemver_dram",
    "correlation_dram",

    # To regular to get a benefit, existing approaches are better than dynamic fusion:
    # "gemm_dram",
    # "kernel_2mm_dram",
    # "kernel_3mm_dram",
    # "doitgen_triple_dram",
    # "lud_dram",
]


if __name__ == '__main__':
    TARGET = sys.argv[1]
    if TARGET not in ['emu', 'sim', 'hw']:
        exit("sys.argv[1] must be emu/sim/hw")

    for kernel in BENCHMARKS:
        os.system(f'echo "--------------------- {kernel} ---------------------"')
        
        os.system('echo "Static"')
        os.system(f'cd {GIT_DIR}/experiments && make fpga_{TARGET} FILE={kernel}.cpp')

        os.system('echo "LSQ"')
        os.system(f'cp {GIT_DIR}/experiments/{kernel}.cpp {GIT_DIR}/experiments/{kernel}_lsq.cpp')
        os.system(f'NO_PE_DECOUPLING=1 {GIT_DIR}/elastic_pass.sh {TARGET} {GIT_DIR}/experiments/{kernel}_lsq.cpp')

        os.system('echo "Fusion"')
        os.system(f'{GIT_DIR}/elastic_pass.sh {TARGET} {GIT_DIR}/experiments/{kernel}.cpp -f')
