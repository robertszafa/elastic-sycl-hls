#!/usr/bin/env python3

import os
import argparse

from constants import GIT_DIR, KERNEL_ASIZE_FOR_TESTS, KERNEL_ASIZE_FOR_BENCHMARKS

INPUTS_DIR = f'{GIT_DIR}/inputs/lsq'
TARGETS = "emu, sim, hw"

if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog="build_all.py",
                                     description="Compile all LSQ input codes using the static baseline and our LSQ approach.",
                                     formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument("--target", type=str, default='hw', help="Compilation target to use (emu, sim or hw). The default is hw.")
    parser.add_argument("--kernels", type=str, help="Comma seperated list of kernels to build. By default all benchmarks are built if hw target, or all tests if emu/sim target.")
    parser.add_argument("--lsq-sizes", type=str, default='8', help="Comma seperated list of LSQ sizes to compile. The default is [8].")
    parser.add_argument("--no-baseline", action='store_true', help="Don't compile using the baseline approach.")
    parser.add_argument("--no-lsq", action='store_true', help="Don't compile using the lsq approach.")
    parser.add_argument("--debug-lsq", action='store_true', help="Don't delete intermediate files generated during the LSQ passes (json reports, llvm ir, etc.)")
    args = parser.parse_args()

    qsizes = [int(s) for s in args.lsq_sizes.split(',')]
    debug_driver_lsq = '-d' if args.debug_lsq else ''

    if args.kernels:
        kernels = args.kernels.split(',')
    else:
        # Only build benchmarks when compiling for hardware.
        kernels = KERNEL_ASIZE_FOR_BENCHMARKS.keys() if args.target == 'hw' else KERNEL_ASIZE_FOR_TESTS.keys()

    for kernel in kernels:
        if not args.no_baseline:
            print(f'\n--- Building baseline ({args.target}): {kernel} ...')
            os.system(f'cd {INPUTS_DIR}/{kernel} && make fpga_{args.target} FILE={kernel}.cpp')
            
        for qsize in qsizes:
            if not args.no_lsq:
                print(f'\n--- Building LSQ ({args.target}): {kernel} ...')
                os.system(f'./driverLSQ.sh {args.target} {INPUTS_DIR}/{kernel}/{kernel}.cpp {qsize} {debug_driver_lsq}')
