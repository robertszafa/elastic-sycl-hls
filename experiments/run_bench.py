#!/usr/bin/env python3

import os
import sys

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]
THIS_DIR = os.path.dirname(os.path.realpath(__file__))

BENCHMARKS = {
    'bfs_bram': f'{THIS_DIR}/email-Eu-core.txt',
    'bc_bram': f'{THIS_DIR}/email-Eu-core.txt',
    'sssp_queue_bram': f'{THIS_DIR}/email-Eu-core.txt',
    'histogram_if_bram': '1000 10 5',
    'threashold_bram': '1000 10 95',
    'maximal_matching_bram': '1000 10 20',
    'floyd_warshall_bram': f'{THIS_DIR}/test-web-NotreDame.txt ',
    'bitonic_sort_bram': '64 10 60',
    'spmv_if_bram': '20 10 40',
    # 'histogram_if_4_bram': '1000 10',
}

BENCHMARKS_MEM_TRANSFER_TIME = {
    'bfs_bram': (1005 + 50), # 50 cycles loop mem transfer enter/exit overhead
    'bc_bram': (1005 + 50) * 2,
    'sssp_queue_bram': (1005 + 50) * 2,
    'histogram_if_bram': (1000 + 50) * 2, 
    'histogram_if_4_bram': (1000 + 50) * 2,
    'threashold_bram': (1000 + 50) * 2,
    'maximal_matching_bram': (1000 + 50) * 2,
    'floyd_warshall_bram': (10*10 + 50) * 2,
    'bitonic_sort_bram': (64*2 + 50) , 
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
    CSV_FILE = f"{THIS_DIR}/performance.csv"

    with open(CSV_FILE, 'w') as f:
        f.write("kernel,INORD,DAE,SPEC_DAE,ORACLE\n")

        for kernel, arg in BENCHMARKS.items():
            print(f'\n--------------------- {kernel} ---------------------')
            f.write(f"{kernel}")

            BINS = {
                "INORDER": f'{THIS_DIR}/bin/{kernel}.fpga_{TARGET}',
                "DAE": f'{THIS_DIR}/bin/{kernel}_dae.elastic.fpga_{TARGET}',
                "SPEC_DAE": f'{THIS_DIR}/bin/{kernel}.elastic.fpga_{TARGET}',
                # Oracle will fail
                "ORACLE": f'{THIS_DIR}/bin/{kernel}_oracle.elastic.fpga_{TARGET}',
            }

            for APPROACH, BIN in BINS.items():
                RES = run_and_get_cycles(BIN, arg)
                RES_COMP = RES - BENCHMARKS_MEM_TRANSFER_TIME[kernel]
                print(f'{APPROACH}: {RES_COMP}')
                
                f.write(f",{RES_COMP}")
            f.write("\n")

    print(f"\nSaved to CSV file: {CSV_FILE}")
