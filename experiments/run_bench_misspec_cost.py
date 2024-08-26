#!/usr/bin/env python3

import os
import sys
from statistics import stdev

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]
THIS_DIR = os.path.dirname(os.path.realpath(__file__))

BENCHMARKS = {
    "histogram_if_bram": [
        "1000 0 0",
        "1000 0 20",
        "1000 0 40",
        "1000 0 60",
        "1000 0 80",
        "1000 0 100",
    ],
    "threashold_bram": [
        "1000 0 0",
        "1000 0 20",
        "1000 0 40",
        "1000 0 60",
        "1000 0 80",
        "1000 0 100",
    ],
    "maximal_matching_bram": [
        "1000 0 0",
        "1000 0 20",
        "1000 0 40",
        "1000 0 60",
        "1000 0 80",
        "1000 0 100",
    ],
    "spmv_if_bram": [
        "20 0 0",
        "20 0 20",
        "20 0 40",
        "20 0 60",
        "20 0 80",
        "20 0 100",
    ],
}

BENCHMARKS_MEM_TRANSFER_TIME = {
    'histogram_if_bram': (1000 + 50) * 2, 
    'threashold_bram': (1000 + 50) * 2,
    'maximal_matching_bram': (1000 + 50) * 2,
    'spmv_if_bram': (20 + 50) * 2,
}


def run_and_get_cycles(bin, arg):
    if "fpga_emu" in bin:
        os.system(f'{bin} {arg}')
        return 500

    TMP_FILE = '.tmp_get_cycles.txt'
    run_sim_script = f'{GIT_DIR}/scripts/run_sim.sh'
    os.system(f'{run_sim_script} {bin} {arg} > {TMP_FILE}')

    cycles = 1

    with open('.tmp_get_cycles.txt', 'r') as f:
        out_lines = f.readlines()
        for line in out_lines:
            if ''.join([x for x in line if x.isalpha()]) == 'MainKernel':
                cycles = int(''.join([x for x in line if x.isnumeric()]))
                break

    os.system(f'rm -f {TMP_FILE}')

    return cycles


if __name__ == '__main__':
    TARGET = sys.argv[1] # change to emu if only testing compiler robustness
    CSV_FILE = f"{THIS_DIR}/misspeculationCost.csv"

    with open(CSV_FILE, 'w') as f:
        f.write("kernel,0,20,40,60,80,100,stdev\n")

        for kernel, args_list in BENCHMARKS.items():
            print(f'\n--------------------- {kernel} ---------------------')
            BIN = f'{THIS_DIR}/bin/{kernel}.elastic.fpga_{TARGET}'
            BIN_ORACLE = f'{THIS_DIR}/bin/{kernel}.elastic.fpga_{TARGET}'

            f.write(f"{kernel}")

            this_kernel_cycles = []
            for i_arg, arg in enumerate(args_list):
                RES = run_and_get_cycles(BIN, arg) - BENCHMARKS_MEM_TRANSFER_TIME[kernel]
                # RES_ORACLE = run_and_get_cycles(BIN_ORACLE, arg) - BENCHMARKS_MEM_TRANSFER_TIME[kernel]

                perc = arg.split(" ")[-1]

                # overhead = 0
                # if RES > RES_ORACLE:
                #     overhead = int((float(RES - RES_ORACLE) / float(RES_ORACLE)) * 100.0)
                # print(f"{kernel} at {perc}% misspec (SPEC / ORACLE) ({RES} / {RES_ORACLE}) = {overhead}")
                # f.write(f",{overhead}")

                f.write(f",{RES}")
                this_kernel_cycles.append(RES)

            kernel_stdev = stdev(this_kernel_cycles)
            f.write(f",{kernel_stdev}\n")
