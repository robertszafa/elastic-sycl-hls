#!/usr/bin/env python3

import os
import argparse

from constants import *

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
                                    description="Check if the CDDD transformation gives correct answers.",
                                    formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument("--target", type=str, default='hw', help="What binary type to use (emu, sim or hw). If not provided, hw is used.")
    args = parser.parse_args()

    targets = [t for t in args.target.split(',')]
    kernels_asizes = KERNEL_ASIZE_FOR_BENCHMARKS if args.target == 'hw' else KERNEL_ASIZE_FOR_TESTS

    for kernel, a_size in kernels_asizes.items():
        BIN_DYNAMIC = f'{INPUTS_DIR}/{kernel}/bin/{kernel}.cddd.fpga_{args.target}'

        print(f'\n--Testing {kernel}:')
        test_binary(BIN_DYNAMIC, a_size, distr=2, percentage=50) 
        

    os.system(f'rm {TMP_FILE}')


