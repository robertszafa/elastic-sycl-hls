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
    "test_fusion_waw2": [
        ["150", "3"],
        ["100", "1"],
    ],
    "test_fusion_war2": [
        ["150", "3"],
        ["100", "1"],
    ],
    "gemm_dram": [["10"]],
    "kernel_2mm_dram": [["10"]],
    "lud_dram": [["10"]],
    "bnn_dram": [["10"]],
    "page_rank_dram": [
        [f"{GIT_DIR}/experiments/test2-web-NotreDame.txt", "3"],
        # [f"{GIT_DIR}/experiments/test-web-NotreDame.txt", "3"],
    ],
}


if __name__ == '__main__':
    TARGET = sys.argv[1]
    if TARGET not in ['emu', 'sim', 'hw']:
        exit("sys.argv[1] must be emu/sim/hw")

    run_prefix = f'{GIT_DIR}/scripts/run_sim.sh' if TARGET == 'sim' else ''

    for kernel, test_vectors in BENCHMARKS_AND_ARGS.items():
        print(f'\n--------------------- {kernel} ---------------------')
        print(f"\n-- Builiding {kernel} --")
        os.system(f'{GIT_DIR}/elastic_pass.sh {TARGET} {GIT_DIR}/experiments/{kernel}.cpp -f')
        
        BIN_ELASTIC = f'{GIT_DIR}/experiments/bin/{kernel}.elastic.fpga_{TARGET}'

        for test_vector in test_vectors:
            args_str = " ".join(test_vector)
            print(f"\n-- Running {kernel} {args_str} --")
            os.system(f'{run_prefix} {BIN_ELASTIC} {args_str}')
