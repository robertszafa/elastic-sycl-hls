#!/usr/bin/env python3

import os
import sys
import statistics

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]
THIS_DIR = os.path.dirname(os.path.realpath(__file__))

NUM_REPEAT = 3

BENCHMARKS_AND_ARGS = {
    "raw_loop": [
        "150",  # sim
        "10000000",  # hw
    ],
    "waw_loop": [
        "150",  # sim
        "10000000",  # hw
    ],
    "war_loop": [
        "150",  # sim
        "10000000",  # hw
    ],
    "fft2x_dram": [
      "32",     # sim
      "1048576",   # hw
    ], 
    "gemver_dram": [
      "10",     # sim
      "10000",   # hw
    ],    
    "bnn_dram": [
        "10",  # sim
        "10000",  # hw
    ],
    "matrix_power_dram": [
        "32",  # sim
        "4096",  # hw
    ],
    "page_rank_dram": [
        f"{GIT_DIR}/experiments/test2-web-NotreDame.txt 3",  # sim
        f"{GIT_DIR}/experiments/web-NotreDame.txt 10",  # hw
    ],
    "hist2x_dram": [
        "1000",  # sim
        "10000000",  # hw
    ],
    "tanh_spmv_dram": [
        "100 0.1",  # sim
        "10000 0.0001",  # hw
    ],
}


def get_time(BIN_NAME, args_str):
    run_prefix = f'{GIT_DIR}/scripts/run_sim.sh' if 'fpga_sim' in BIN_NAME else ''

    TMP_FILE = '.tmp_get_time.txt'

    MIN_TIME = 2**31
    for _ in range(NUM_REPEAT):
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
                    print(f"{res} ms")
                    MIN_TIME = min(MIN_TIME, res)
    
    return MIN_TIME


if __name__ == '__main__':
    CSV_FILE = f"{THIS_DIR}/performance.csv"

    TARGET = sys.argv[1]
    if TARGET not in ['emu', 'sim', 'hw']:
        exit("sys.argv[1] must be emu/sim/hw")

    with open(CSV_FILE, 'w') as f:
        f.write("kernel,STA,LSQ,FUS,FUS+FRWD\n")

        ALL_SPEEDUPS_LSQ = []
        ALL_SPEEDUPS_FUS = []
        ALL_SPEEDUPS_FUS_FRWD = []
        for kernel, args in BENCHMARKS_AND_ARGS.items():
            os.system(f'printf "\n--------------- {kernel} ---------------\n"')

            BIN_STA = f'{GIT_DIR}/experiments/bin/{kernel}.fpga_{TARGET}'
            BIN_LSQ = f'{GIT_DIR}/experiments/bin/{kernel}_lsq.elastic.fpga_{TARGET}'
            BIN_FUS = f'{GIT_DIR}/experiments/bin/{kernel}_nofrwd.elastic.fpga_{TARGET}'
            BIN_FUS_FRWD = f'{GIT_DIR}/experiments/bin/{kernel}.elastic.fpga_{TARGET}'

            RESULTS_STA = []
            RESULTS_LSQ = []
            RESULTS_FUS = []
            RESULTS_FUS_FRWD = []

            test_vectors_to_use = args[1:] if TARGET == 'hw' else args[:1]
            for arg in test_vectors_to_use:
                os.system('printf "\n============== STA =============\n"')
                RESULTS_STA.append(get_time(BIN_STA, arg))

                os.system('printf "\n============== LSQ =============\n"')
                RESULTS_LSQ.append(get_time(BIN_LSQ, arg))

                os.system('printf "\n============== FUS =============\n"')
                RESULTS_FUS.append(get_time(BIN_FUS, arg))

                os.system('printf "\n============== FUS+FRWD =============\n"')
                RESULTS_FUS_FRWD.append(get_time(BIN_FUS_FRWD, arg))
            
            RES_STAT = statistics.mean(RESULTS_STA)
            RES_LSQ = statistics.mean(RESULTS_LSQ)
            RES_FUS = statistics.mean(RESULTS_FUS)
            RESULTS_FUS_FRWD = statistics.mean(RESULTS_FUS_FRWD)

            SPEEDUP_LSQ = round(float(RES_STAT) / float(RES_LSQ), 2) if RES_LSQ != 0 else 1
            SPEEDUP_FUS = round(float(RES_STAT) / float(RES_FUS), 2) if RES_FUS != 0 else 1
            SPEEDUP_FUS_FRWD = round(float(RES_STAT) / float(RESULTS_FUS_FRWD), 2) if RESULTS_FUS_FRWD != 0 else 1

            ALL_SPEEDUPS_LSQ.append(SPEEDUP_LSQ)
            ALL_SPEEDUPS_FUS.append(SPEEDUP_FUS)
            ALL_SPEEDUPS_FUS_FRWD.append(SPEEDUP_FUS_FRWD)

            RES_STAT = round(RES_STAT, 2)
            RES_LSQ = round(RES_LSQ, 2)
            RES_FUS = round(RES_FUS, 2)
            RESULTS_FUS_FRWD = round(RESULTS_FUS_FRWD, 2)

            kernel_name = kernel.split("_dram")[0]
            # f.write(f"{kernel_name},{RES_STAT} (1x),{RES_LSQ} ({SPEEDUP_LSQ}x),{RES_FUS} ({SPEEDUP_FUS}x),{RESULTS_FUS_FRWD} ({SPEEDUP_FUS_FRWD}x)\n")
            f.write(f"{kernel_name},{RES_STAT},{RES_LSQ},{RES_FUS},{RESULTS_FUS_FRWD}\n")

        MEAN_SPEEDUP_LSQ = statistics.harmonic_mean(ALL_SPEEDUPS_LSQ)
        MEAN_SPEEDUP_FUS = statistics.harmonic_mean(ALL_SPEEDUPS_FUS)
        MEAN_SPEEDUP_FUS_FRWD = statistics.harmonic_mean(ALL_SPEEDUPS_FUS_FRWD)
        f.write(f"mean,1,{MEAN_SPEEDUP_LSQ},{MEAN_SPEEDUP_FUS},{MEAN_SPEEDUP_FUS_FRWD}\n")

    print(f"\nSaved to CSV file: {CSV_FILE}")
