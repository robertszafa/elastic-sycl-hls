#!/usr/bin/env python3

import os
import re
import csv
from scipy.stats import gmean
from pathlib import Path

import os
import argparse

from constants import *

INPUTS_DIR = f'{GIT_DIR}/inputs/lsq'

# Range of data hazard % to test
PERCENTAGES_WAIT = [0, 40, 80, 100]


def run_bin(binary, a_size, percentage=0):
    """Return the execution time if bin runs in hardware and num cycles if in sim."""
    # print(f'> {bin} : ', end='')

    if percentage == 100: # all wait
        bin_invoc = f'{binary} {a_size} 0'
    elif percentage == 0: # no wait
        bin_invoc = f'{binary} {a_size} 1'
    else: # percentage wait
        bin_invoc = f'{binary} {a_size} 2 {percentage}'

    os.system(f'{bin_invoc} > {TMP_FILE}')
    stdout = ''
    with open(TMP_FILE, 'r') as f:
        stdout = str(f.read())
    
    if 'fpga_sim' in binary: # Get cycle count
        with open(SIM_CYCLES_FILE, 'r') as f:
            match = re.search(r'"time":"(\d+)"', f.read())
        if match:
            return int(match.group(1))
    else: # Get wall time
        match = re.search(r'Kernel time \(ms\): (\d+\.\d+|\d+)', stdout)
        if match:
            return float(match.group(1))
    
    return 1

if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog="run_benchmarks_exp.py",
                                     description="Run all benchmarks using the static baseline and our LSQ approach. Results are saved to CSV files.",
                                     formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument("--target", type=str, default='hw', help="What binary type to use (emu, sim or hw). If not provided, hw is used.")
    parser.add_argument("--lsq-sizes", type=str, default='8', help="The LSQ size configurations to test. Default is 8.")
    args = parser.parse_args()

    qsizes = [int(s) for s in args.lsq_sizes.split(',')]

    # Record lowest-, highest-, gmean-speedup against static across all kernels for every qsize.
    lowest_speedup_all_kernels = {q : 1000 for q in qsizes}
    highest_speedup_all_kernels = {q : 0 for q in qsizes}
    gmean_speedup_all_kernels = {q : -1 for q in qsizes}

    for kernel, a_size in KERNEL_ASIZE_FOR_BENCHMARKS.items():
        # Ensure dir structure exists
        Path(f'{EXP_DATA_DIR}/').mkdir(parents=True, exist_ok=True)
        res_file = f'{EXP_DATA_DIR}/{kernel}_{args.target}.csv'

        with open(res_file, 'w') as f:
            writer = csv.writer(f)
            writer.writerow(['percentage', 'static'] + [f'lsq_{q}' for q in qsizes])

            # Record lowest-highest speedup against static across all % for each q_size.
            lowest_speedup = {q : 100000 for q in qsizes}
            highest_speedup = {q : 0 for q in qsizes}
            all_speedups = {q : [] for q in qsizes}

            for percentage in PERCENTAGES_WAIT:
                print(f'\n-- {kernel} with {percentage}% data hazards')

                new_row = [percentage]

                BIN_STATIC = f'{INPUTS_DIR}/{kernel}/bin/{kernel}.fpga_{args.target}'
                static_time = run_bin(BIN_STATIC, a_size, percentage=percentage)
                new_row.append(static_time)
                print('baseline:', static_time)

                for q in qsizes:
                    BIN_DYNAMIC = f'{INPUTS_DIR}/{kernel}/bin/{kernel}.lsq_{q}.fpga_{args.target}' 
                    dyn_time = run_bin(BIN_DYNAMIC, a_size, percentage=percentage)
                    new_row.append(dyn_time)
                    print(f'lsq_{q}: {dyn_time}')

                    this_perc_speedup = round(static_time / dyn_time, 2)
                    all_speedups[q].append(this_perc_speedup)
                    lowest_speedup[q] = min(this_perc_speedup, lowest_speedup[q])
                    highest_speedup[q] = max(this_perc_speedup, highest_speedup[q])

                writer.writerow(new_row)

            gmean_rounded = lambda s_ups : round(gmean(s_ups), 2)

            gmean_speedup = list(map(gmean_rounded, list(all_speedups.values())))
            lowest_speedup_row = ['lowest_speedup', '1'] + list(lowest_speedup.values())
            highest_speedup_row = ['highest_speedup', '1'] + list(highest_speedup.values())
            gmean_speedup_row = ['gmean_speedup', '1'] + list(gmean_speedup)

            writer.writerow(lowest_speedup_row)
            writer.writerow(highest_speedup_row)
            writer.writerow(gmean_speedup_row)

            print(f'\nSummary {kernel}\n{lowest_speedup_row}\n{highest_speedup_row}\n{gmean_speedup_row}\n')

            # Update gmean for all kernels.
            for i_q, q in enumerate(qsizes):
                # Need to init to first gmean for this to work.
                if gmean_speedup_all_kernels[q] == -1:
                    gmean_speedup_all_kernels[q] = gmean_speedup[i_q]

                gmean_speedup_all_kernels[q] = gmean_rounded([gmean_speedup_all_kernels[q], 
                                                               gmean_speedup[i_q]])
                lowest_speedup_all_kernels[q] = min(lowest_speedup_all_kernels[q], lowest_speedup[q])
                highest_speedup_all_kernels[q] = max(highest_speedup_all_kernels[q], highest_speedup[q])


        # Emulation perf results are meaningless, delete.
        if args.target == 'emu':
            os.system(f'rm {res_file}')

    
    print('\nlowest_speedup over all kernels:')
    for q, q_lowest in lowest_speedup_all_kernels.items():
        print(f'{q}qsize : {q_lowest}')

    print('\nhighest_speedup over all kernels:')
    for q, q_highest in highest_speedup_all_kernels.items():
        print(f'{q}qsize : {q_highest}')

    print('\ngmean_speedup over all kernels:')
    for q, q_gmean in gmean_speedup_all_kernels.items():
        print(f'{q}qsize : {q_gmean}')
    

    os.system(f'rm {TMP_FILE}')


