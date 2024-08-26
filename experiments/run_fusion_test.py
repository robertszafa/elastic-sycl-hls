#!/usr/bin/env python3

import os
import sys

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]
BENCHMARKS_AND_ARGS = {
    "test_fusion_waw": [
        ["150", "3"],
        ["100", "1"],
    ],
    "test_fusion_war": [
        ["150", "3"],
        ["100", "1"],
    ],
    "test_fusion_raw": [
        ["150", "3"],
        ["100", "1"],
    ],
    "page_rank_dram": [
        [f"{GIT_DIR}/experiments/test2-web-NotreDame.txt", "3"],
        # [f"{GIT_DIR}/experiments/test-web-NotreDame.txt", "3"],
    ],
    "bnn_dram": [["10"]],

    "gemm_dram": [["10"]],
    "gemver_dram": [["10"]],
    "kernel_2mm_dram": [["10"]],
    "kernel_3mm_dram": [["10"]],
    "doitgen_triple_dram": [["10"]],
    # "lud_dram": [["10"]],
}


if __name__ == '__main__':
    TARGET = sys.argv[1]
    if TARGET not in ['emu', 'sim', 'hw']:
        exit("sys.argv[1] must be emu/sim/hw")

    run_prefix = f'{GIT_DIR}/scripts/run_sim.sh' if TARGET == 'sim' else ''

    for kernel, test_vectors in BENCHMARKS_AND_ARGS.items():
        print(f'\n--------------------- {kernel} ---------------------')

        BIN_STATIC = f'{GIT_DIR}/experiments/bin/{kernel}.fpga_{TARGET}'
        BIN_LSQ = f'{GIT_DIR}/experiments/bin/{kernel}_lsq.elastic.fpga_{TARGET}'
        BIN_FUSION = f'{GIT_DIR}/experiments/bin/{kernel}.elastic.fpga_{TARGET}'

        # print("\nStatic:")
        # for test_vector in test_vectors:
        #     args_str = " ".join(test_vector)
        #     os.system(f'{run_prefix} {BIN_STATIC} {args_str}')
        
        # print("\nLSQ:")
        # for test_vector in test_vectors:
        #     args_str = " ".join(test_vector)
        #     os.system(f'{run_prefix} {BIN_LSQ} {args_str}')

        print("\nFUSION:")
        for test_vector in test_vectors:
            args_str = " ".join(test_vector)
            os.system(f'{run_prefix} {BIN_FUSION} {args_str}')


