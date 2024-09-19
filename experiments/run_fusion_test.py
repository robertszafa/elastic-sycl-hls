#!/usr/bin/env python3

import os
import sys
import statistics

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]
THIS_DIR = os.path.dirname(os.path.realpath(__file__))

BENCHMARKS_AND_ARGS = {
    "gemver_dram": [
        ["10"],     # sim
        ["1000"]    # hw
    ],
    "correlation_dram": [
        ["10", "10"],           # sim
        ["1000", "1000"]        # hw
    ],

    "page_rank_dram": [
        [f"{GIT_DIR}/experiments/test2-web-NotreDame.txt", "3"],    # sim
        [f"{GIT_DIR}/experiments/web-NotreDame.txt", "10"],         # hw
    ],
    "bnn_dram": [
        ["10"],     # sim
        ["1000"],   # hw

    ],

    "fft_conv_dram": [
        ["32"],     # sim
        ["1024"],   # hw
    ],
    "gsum_sort_dram": [
        ["32"],     # sim
        ["1024"],   # hw
    ],
    "scale_fw_dram": [
        ["10"],     # sim
        ["128"],   # hw
    ],

    "raw_loop": [
        ["150"],       # sim
        ["1000000"],  # hw
    ],
    "waw_loop": [
        ["150"],       # sim
        ["1000000"],  # hw
    ],
    "war_loop": [
        ["150"],       # sim
        ["1000000"],  # hw
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
    
    # simple RAW/WAW/WAR loops but enclosed in more outer loops.
    # "test_fusion_waw": [
    #     ["150", "3"],       # sim
    #     ["10000", "10"],  # hw
    # ],
    # "test_fusion_war": [
    #     ["150", "3"],       # sim
    #     ["10000", "10"],  # hw
    # ],
    # "test_fusion_raw": [
    #     ["150", "3"],       # sim
    #     ["10000", "10"],  # hw
    # ],
}

def get_time(BIN_NAME, args_str):
    run_prefix = f'{GIT_DIR}/scripts/run_sim.sh' if 'fpga_sim' in BIN_NAME else ''

    TMP_FILE = '.tmp_get_time.txt'
    os.system(f'{run_prefix} {BIN_NAME} {args_str} > {TMP_FILE}')

    with open(TMP_FILE, 'r') as f:
        out_lines = f.readlines()
        if 'Failed' in ''.join(out_lines):
            os.system('echo "Failed"')
        # else:
        #     os.system('echo "Passed"')

        for line in out_lines:
            if "Kernel time (ms):" in line:
                res = float(line.split("Kernel time (ms):")[1])
                os.system(f'echo "{res} ms"')
                return res
    
    return 0


if __name__ == '__main__':
    CSV_FILE = f"{THIS_DIR}/performance.csv"

    TARGET = sys.argv[1]
    if TARGET not in ['emu', 'sim', 'hw']:
        exit("sys.argv[1] must be emu/sim/hw")

    with open(CSV_FILE, 'w') as f:
        f.write("kernel,STATIC,LSQ,FUSION\n")

        for kernel, test_vectors in BENCHMARKS_AND_ARGS.items():
            os.system(f'printf "\n--------------- {kernel} ---------------\n"')

            BIN_STATIC = f'{GIT_DIR}/experiments/bin/{kernel}.fpga_{TARGET}'
            BIN_LSQ = f'{GIT_DIR}/experiments/bin/{kernel}_lsq.elastic.fpga_{TARGET}'
            BIN_FUSION = f'{GIT_DIR}/experiments/bin/{kernel}.elastic.fpga_{TARGET}'


            RESULTS_STATIC = []
            RESULTS_LSQ = []
            RESULTS_FUSION = []

            test_vectors_to_use = test_vectors[1:] if TARGET == 'hw' else test_vectors[:1]
            for test_vector in test_vectors_to_use:
                args_str = " ".join(test_vector)

                os.system('printf "\n============== STATIC =============\n"')
                RESULTS_STATIC.append(get_time(BIN_STATIC, args_str))

                os.system('printf "\n============== LSQ =============\n"')
                RESULTS_LSQ.append(get_time(BIN_LSQ, args_str))

                os.system('printf "\n============== FUSION =============\n"')
                RESULTS_FUSION.append(get_time(BIN_FUSION, args_str))
            
            RES_STATIC = statistics.mean(RESULTS_STATIC)
            RES_LSQ = statistics.mean(RESULTS_LSQ)
            RES_FUSION = statistics.mean(RESULTS_FUSION)

            SPEEDUP_LSQ = round(float(RES_STATIC) / float(RES_LSQ), 2) if RES_LSQ != 0 else 1
            SPEEDUP_FUSION = round(float(RES_STATIC) / float(RES_FUSION), 2) if RES_FUSION != 0 else 1

            RES_STATIC = round(RES_STATIC, 2)
            RES_LSQ = round(RES_LSQ, 2)
            RES_FUSION = round(RES_FUSION, 2)

            f.write(f"{kernel},{RES_STATIC} (1x),{RES_LSQ} ({SPEEDUP_LSQ}x),{RES_FUSION} ({SPEEDUP_FUSION}x)\n")

    print(f"\nSaved to CSV file: {CSV_FILE}")
