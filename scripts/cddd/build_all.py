#!/usr/bin/env python3

import os
import argparse

from constants import GIT_DIR, KERNEL_ASIZE_FOR_TESTS, KERNEL_ASIZE_FOR_BENCHMARKS, INPUTS_DIR

TARGETS = "emu, sim, hw"

if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog="build_all.py",
                                     description="Compile all CDDD input codes using the baseline and our CDDD approach.",
                                     formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument("--target", type=str, default='hw', help="Compilation target to use (emu, sim or hw). The default is hw.")
    parser.add_argument("--kernels", type=str, help="Comma seperated list of kernels to build. By default all benchmarks are built if hw target, or all tests if emu/sim target.")
    parser.add_argument("--no-baseline", action='store_true', help="Don't compile using the baseline approach.")
    parser.add_argument("--no-cddd", action='store_true', help="Don't compile using the CDDD approach.")
    parser.add_argument("--debug-cddd", action='store_true', help="Don't delete intermediate files generated during the CDDD passes (json reports, llvm ir, etc.)")
    args = parser.parse_args()

    debug_driver_cddd = '-d' if args.debug_cddd else ''

    if args.kernels:
        kernels = args.kernels.split(',')
    else:
        # Only build benchmarks when compiling for hardware.
        kernels = KERNEL_ASIZE_FOR_BENCHMARKS.keys() if args.target == 'hw' else KERNEL_ASIZE_FOR_TESTS.keys()

    for kernel in kernels:
        if not args.no_baseline:
            print(f'\n--- Building baseline ({args.target}): {kernel} ...')
            os.system(f'cd {INPUTS_DIR}/{kernel} && make fpga_{args.target} FILE={kernel}.cpp')
            
        if not args.no_cddd:
            print(f'\n--- Building CDDD ({args.target}): {kernel} ...')
            os.system(f'./driverCDDD.sh {args.target} {INPUTS_DIR}/{kernel}/{kernel}.cpp {debug_driver_cddd}')
