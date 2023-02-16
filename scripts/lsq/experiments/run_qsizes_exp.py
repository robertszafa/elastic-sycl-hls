#!/usr/bin/env python3

import os
import re
import csv
from pathlib import Path
import argparse

from constants import GIT_DIR, EXP_DATA_DIR, TMP_FILE, SIM_CYCLES_FILE

INPUTS_DIR = f'{GIT_DIR}/inputs/lsq'

# A realistic % of data hazards
PERCENTAGE_WAIT = 10

Q_SIZES = [1, 2, 4, 8, 16]

# Only these two for this experiment
KERNEL_ASIZE_FOR_BENCHMARKS = {
    'histogram' : 10000,
    'spmv' : 100,
}
# Decrease domain sizes when running in simulation.
KERNEL_ASIZE_PAIRS_SIM = {
    'histogram' : 1000,
    'spmv' : 20,
}


def run_bin(bin, a_size, percentage=0):
    if percentage == 100: # all wait
        bin_invoc = f'{bin} {a_size} 0'
    elif percentage == 0: # no wait
        bin_invoc = f'{bin} {a_size} 1'
    else: # percentage wait
        bin_invoc = f'{bin} {a_size} 2 {percentage}'

    os.system(f'{bin_invoc} > {TMP_FILE}')
    stdout = ''
    with open(TMP_FILE, 'r') as f:
        stdout = str(f.read())
    
    if 'fpga_sim' in bin: 
        # Get cycle count
        with open(SIM_CYCLES_FILE, 'r') as f:
            match = re.search(r'"time":"(\d+)"', f.read())
        if match:
            return int(match.group(1))
    else: 
        # Get time
        match = re.search(r'Kernel time \(ms\): (\d+\.\d+|\d+)', stdout)
        if match:
            return float(match.group(1))
    
    return 1


if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog="run_Q_SIZES_exp.py",
                                     description="Run the same benchamrks for different LSQ sizes and save resource/fmax/performance numbers to CSV files.",
                                     formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument("--target", type=str, default='hw', help="What binary type to use (emu, sim or hw). If not provided, hw is used.")
    args = parser.parse_args()

    # Record lowest-, highest-, gmean-speedup against static across all kernels for every qsize.
    lowest_speedup_all_kernels = {q : 1000 for q in Q_SIZES}
    highest_speedup_all_kernels = {q : 0 for q in Q_SIZES}
    gmean_speedup_all_kernels = {q : -1 for q in Q_SIZES}

    for kernel, a_size in KERNEL_ASIZE_FOR_BENCHMARKS.items():
        print('\n---', kernel)

        # Ensure dir structure exists
        Path(f'{EXP_DATA_DIR}').mkdir(parents=True, exist_ok=True)
        res_file = f'{EXP_DATA_DIR}/Q_SIZES_exp_{kernel}_{args.target}.csv'

        with open(res_file, 'w') as f:
            writer = csv.writer(f)
            writer.writerow(['qsize', 'time'])

            # Record lowest-highest speedup against static across all % for each q_size.
            lowest_speedup = {q : 100000 for q in Q_SIZES}
            highest_speedup = {q : 0 for q in Q_SIZES}
            all_speedups = {q : [] for q in Q_SIZES}

            BIN_STATIC = f'{INPUTS_DIR}/{kernel}/bin/{kernel}.fpga_{args.target}'
            static_time = run_bin(BIN_STATIC, a_size, percentage=PERCENTAGE_WAIT)
            writer.writerow(['static', static_time])
            print('baseline:', static_time)

            for qsize in Q_SIZES:
                new_row = [qsize]
                BIN_DYNAMIC = f'{INPUTS_DIR}/{kernel}/bin/{kernel}.lsq_{qsize}.fpga_{args.target}' 
                dyn_time = run_bin(BIN_DYNAMIC, a_size, percentage=PERCENTAGE_WAIT)
                print(f'lsq_{qsize}: {dyn_time}')

                writer.writerow([f'lsq_{qsize}', dyn_time])


        # Emulation perf results are meaningless, delete.
        if args.target == 'emu':
            os.system(f'rm {res_file}')


    os.system(f'rm {TMP_FILE}')


