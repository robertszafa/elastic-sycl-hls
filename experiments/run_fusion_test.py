#!/usr/bin/env python3

import os
import sys

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]
BENCHMARKS_AND_ARGS = {
    "test_fusion_waw": [
        ["150", "3"],       # sim
        # ["10000", "10"],  # hw
    ],
    "test_fusion_war": [
        ["150", "3"],       # sim
        # ["10000", "10"],  # hw
    ],
    "test_fusion_raw": [
        ["150", "3"],       # sim
        # ["10000", "10"],  # hw
    ],

    "page_rank_dram": [
        [f"{GIT_DIR}/experiments/test2-web-NotreDame.txt", "3"],    # sim
        # [f"{GIT_DIR}/experiments/web-NotreDame.txt", "10"],         # hw
    ],
    "bnn_dram": [
        ["10"],     # sim
        # ["1000"],   # hw

    ],

    "fft_conv_dram": [
        ["32"],     # sim
        # ["1024"],   # hw
    ],
    "gsum_sort_dram": [
        ["32"],     # sim
        # ["1024"],   # hw
    ],
    "scale_fw_dram": [
        ["10"],     # sim
        # ["128"],   # hw
    ],

    "gemver_dram": [
        ["10"],     # sim
        # ["1000"]    # hw
    ],
    "correlation_dram": [
        ["10", "10"],           # sim
        # ["1000", "1000"]        # hw
    ],
 
    # To regular to get a benefit, existing approaches are better than dynamic fusion:
    # "gemm_dram": [
    #     ["10"],     # sim
    #     # ["1000"]    # hw
    # ],
    # "kernel_2mm_dram": [
    #     ["10"],     # sim
    #     # ["1000"]    # hw
    # ],
    # "kernel_3mm_dram": [
    #     ["10"],     # sim
    #     # ["1000"]    # hw
    # ],
    # "doitgen_triple_dram": [
    #     ["10"],     # sim
    #     # ["1000"]    # hw
    # ],

    # "lud_dram": [["10"]],
}


if __name__ == '__main__':
    TARGET = sys.argv[1]
    if TARGET not in ['emu', 'sim', 'hw']:
        exit("sys.argv[1] must be emu/sim/hw")

    run_prefix = f'{GIT_DIR}/scripts/run_sim.sh' if TARGET == 'sim' else ''

    for kernel, test_vectors in BENCHMARKS_AND_ARGS.items():
        os.system(f'echo "--------------------- {kernel} ---------------------"')

        BIN_STATIC = f'{GIT_DIR}/experiments/bin/{kernel}.fpga_{TARGET}'
        BIN_LSQ = f'{GIT_DIR}/experiments/bin/{kernel}_lsq.elastic.fpga_{TARGET}'
        BIN_FUSION = f'{GIT_DIR}/experiments/bin/{kernel}.elastic.fpga_{TARGET}'

        os.system('printf "\n============== STATIC =============\n"')
        for test_vector in test_vectors:
            args_str = " ".join(test_vector)
            os.system(f'{run_prefix} {BIN_STATIC} {args_str}')
        
        os.system('printf "\n============== LSQ =============\n"')
        for test_vector in test_vectors:
            args_str = " ".join(test_vector)
            os.system(f'{run_prefix} {BIN_LSQ} {args_str}')

        os.system('printf "\n============== FUSION =============\n"')
        for test_vector in test_vectors:
            args_str = " ".join(test_vector)
            os.system(f'{run_prefix} {BIN_FUSION} {args_str}')


