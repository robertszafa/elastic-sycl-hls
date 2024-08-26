#!/usr/bin/env python3

import os
import sys

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]
THIS_DIR = os.path.dirname(os.path.realpath(__file__))

arg_list = [
    "1000 0 0",
    "1000 0 20",
    "1000 0 40",
    "1000 0 60",
    "1000 0 80",
    "1000 0 100",
]
BENCHMARKS = [
    "block_1",
    "block_2",
    "block_3",
    "block_4",
    "block_5",
    "block_6",
    "block_7",
    "block_8",
]

BENCHMARKS_MEM_TRANSFER_TIME = 1000*2

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
    CSV_FILE = "misspeculationCost_blocks.csv"

    with open(CSV_FILE, 'w') as f:
        f.write("mis-speculation %")
        for kernel in BENCHMARKS:
            f.write(f",{kernel}")

        for bench_arg in arg_list:
            perc = bench_arg.split(" ")[-1]
            f.write(f"\n{perc}")
            
            for kernel in BENCHMARKS:
                print(f'\n--------------------- {kernel} ---------------------')
                BIN = f'{THIS_DIR}/bin/{kernel}.elastic.fpga_{TARGET}'
                BIN_ORACLE = f'{THIS_DIR}/bin/{kernel}_oracle.elastic.fpga_{TARGET}'

                RES = run_and_get_cycles(BIN, bench_arg) - BENCHMARKS_MEM_TRANSFER_TIME
                RES_ORACLE = run_and_get_cycles(BIN_ORACLE, bench_arg) - BENCHMARKS_MEM_TRANSFER_TIME

                overhead = int((float(RES) / float(RES_ORACLE)) * 100 - 100)
                print(f"{kernel} at {perc}% misspec (SPEC / ORACLE) ({RES} / {RES_ORACLE})")
                f.write(f",{overhead}")
