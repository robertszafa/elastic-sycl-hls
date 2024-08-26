#!/usr/bin/env python3

import os
import sys

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]
THIS_DIR = os.path.dirname(os.path.realpath(__file__))

BENCHMARKS = {
    'block_1': '1000 0 0',
    'block_2': '1000 0 0',
    'block_3': '1000 0 0',
    'block_4': '1000 0 0',
    'block_5': '1000 0 0',
    'block_6': '1000 0 0',
    'block_7': '1000 0 0',
    'block_8': '1000 0 0',
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
        if 'Failed' in ''.join(out_lines):
            print("Failed")
        else:
            print("Passed")

        for line in out_lines:
            if ''.join([x for x in line if x.isalpha()]) == 'MainKernel':
                cycles = int(''.join([x for x in line if x.isnumeric()]))
                break

    os.system(f'rm -f {TMP_FILE}')

    return cycles


if __name__ == '__main__':
    TARGET = sys.argv[1] # change to emu if only testing compiler robustness

    CSV_FILE = f"{THIS_DIR}/num_poison_blocks_bench_2.csv"

    with open(CSV_FILE, 'w') as f:
        # f.write("kernel,INORD,DAE,SPEC_DAE,ORACLE\n")
        f.write("kernel,INORD\n")

        for kernel, arg in BENCHMARKS.items():
            print(f'\n--------------------- {kernel} ---------------------')
            f.write(f"{kernel}")

            BINS = {
                "INORDER": f'{THIS_DIR}/bin/{kernel}.fpga_{TARGET}',
                # "DAE": f'{THIS_DIR}/bin/{kernel}_dae.elastic.fpga_{TARGET}',
                # "SPEC_DAE": f'{THIS_DIR}/bin/{kernel}.elastic.fpga_{TARGET}',
                # Oracle will fail
                # "ORACLE": f'{THIS_DIR}/bin/{kernel}_oracle.elastic.fpga_{TARGET}',
            }

            for APPROACH, BIN in BINS.items():
                RES = run_and_get_cycles(BIN, arg) - 1024*2 # bench transfer time
                print(f'{APPROACH}: {RES}')
                
                f.write(f",{RES}")
            f.write("\n")

    print(f"\nSaved to CSV file: {CSV_FILE}")
