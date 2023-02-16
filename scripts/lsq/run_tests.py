#!/usr/bin/env python3

import os
import re
import argparse

from constants import *

INPUTS_DIR = f'{GIT_DIR}/inputs/lsq'

def test_binary(binary, a_size, distr=0, percentage=0):
    os.system(f'{binary} {a_size} {distr} {percentage} > {TMP_FILE}')

    stdout = ''
    with open(TMP_FILE, 'r') as f:
        stdout = str(f.read())
    if not 'Passed' in stdout:
        bench_name = binary.split('/')[-1]
        print(f'FAIL in {bench_name} ...:\n{stdout}\n')
    else:
        print(f'PASSED')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog="run_tests.py",
                                    description="Check if the LSQ approach gives correct answers.",
                                    formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument("--target", type=str, default='hw', help="What binary type to use (emu, sim or hw). If not provided, hw is used.")
    parser.add_argument("--lsq-sizes", type=str, default='8', help="Comma seperated list of LSQ sizes to check. The default is [8].")
    args = parser.parse_args()

    qsizes = [int(s) for s in args.lsq_sizes.split(',')]
    targets = [t for t in args.target.split(',')]
    
    for kernel, a_size in KERNEL_ASIZE_FOR_TESTS.items():
        for q in qsizes:
            BIN_DYNAMIC = f'{INPUTS_DIR}/{kernel}/bin/{kernel}.lsq_{q}.fpga_{args.target}'

            print(f'\n--Testing {kernel} with qsize {q}:')
            print(f'\t100% data harards: ', end='')
            test_binary(BIN_DYNAMIC, a_size, distr=0) # All wait
            print(f'\t0% data harards:   ', end='')
            test_binary(BIN_DYNAMIC, a_size, distr=1) # No wait
            print(f'\t50% data harards:  ', end='')
            test_binary(BIN_DYNAMIC, a_size, distr=2, percentage=50) # 50% wait
        

    os.system(f'rm {TMP_FILE}')


