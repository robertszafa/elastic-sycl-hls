#!/usr/bin/env python3

import os
import sys

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]
BENCHMARKS_AND_ARGS = {
    'gemm_dram_dynamic': ['10'],
    'test_fusion_war2_dynamic': ['41', '3'],
    'test_fusion_waw2_dynamic': ['41', '3'],
    'kernel_2mm_dram_dynamic': ['10'],
    'page_rank_dram_dynamic': [f'{GIT_DIR}/experiments/test2-web-NotreDame.txt', '3'],
    # 'page_rank_dram_dynamic': [f'{GIT_DIR}/experiments/test-web-NotreDame.txt', '3'],
    # 'lud_dram_dynamic': ['10'],
}


if __name__ == '__main__':
    TARGET = 'sim' if (len(sys.argv) == 1 or sys.argv[1] != 'emu') else 'emu'
    run_prefix = f'{GIT_DIR}/scripts/run_sim.sh' if TARGET == 'sim' else ''

    for kernel, bench_args in BENCHMARKS_AND_ARGS.items():
        print(f'\n--------------------- {kernel} ---------------------')

        print(f"-- Builiding {kernel} --")
        os.system(f'cd {GIT_DIR}/experiments && make fpga_{TARGET} FILE={kernel}.cpp')

        BIN_ELASTIC = f'{GIT_DIR}/experiments/bin/{kernel}.fpga_{TARGET}'
        args_str = " ".join(bench_args)

        print(f"-- Running {kernel} {args_str} --")
        os.system(f'{run_prefix} {BIN_ELASTIC} {args_str}')
        
