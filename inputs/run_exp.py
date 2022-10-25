import sys
import os
import re
import csv
import time
from scipy.stats import gmean
from pathlib import Path


EXP_DATA_DIR = 'exp_data/'
SIM_CYCLES_FILE = 'simulation_raw.json'
TMP_FILE = f'.tmp_run_exp{str(time.time())[-5:]}.txt'

Q_SIZES = [8]

KERNEL_ASIZE_PAIRS = {
    'histogram' : 1000000,
    'histogram_if' : 1000000,
    'spmv' : 1000,
    'maximal_matching' : 1000000,
    'get_tanh' : 1000000,
    'bnn' : 1000,
    'vec_trans' : 1000000,
    'chaos_ncg' : 1000000,
}
# Decrease domain sizes when running in simulation.
KERNEL_ASIZE_PAIRS_SIM = {
    'histogram' : 1000,
    'histogram_if' : 1000,
    'spmv' : 20,
    'maximal_matching' : 1000,
    'get_tanh' : 1000,
    'bnn' : 100,
    'vec_trans' : 1000,
    'chaos_ncg' : 1000,
}
# For info.
DATA_DISTRIBUTIONS = {
    0: 'all_wait',
    1: 'no_wait',
    2: 'percentage_wait',
}

PERCENTAGES_WAIT = [0, 40, 80, 100]


def run_bin(bin, a_size, percentage=0):
    print(f'> {bin} : ', end='')

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
    if not 'Passed' in stdout:
        print(f'ERROR: Fail in {bin_invoc}\nOUTPUT:\n{stdout}')
    
    if 'fpga_sim' in bin: 
        # Get cycle count
        with open(SIM_CYCLES_FILE, 'r') as f:
            match = re.search(r'"time":"(\d+)"', f.read())
        if match:
            print(f'{int(match.group(1))}')
            return int(match.group(1))
    else: 
        # Get time
        match = re.search(r'Kernel time \(ms\): (\d+\.\d+|\d+)', stdout)
        if match:
            print(f'{float(match.group(1))}')
            return float(match.group(1))
    
    return 1


if __name__ == '__main__':
    if sys.argv[1] not in ['emu', 'sim', 'hw']:
        exit("ERROR: No extension provided\nUSAGE: ./build_all.py [emu, sim, hw]\n")

    bin_type = sys.argv[1]
    bin_ext = 'fpga_' + bin_type
    is_sim = 'sim' == bin_type
    is_emu = 'emu' == bin_type

    kernel_asize_pairs = KERNEL_ASIZE_PAIRS_SIM if is_sim or is_emu else KERNEL_ASIZE_PAIRS

    # Record lowest-, highest-, gmean-speedup against static across all kernels for every qsize.
    lowest_speedup_all_kernels = {q : 1000 for q in Q_SIZES}
    highest_speedup_all_kernels = {q : 0 for q in Q_SIZES}
    gmean_speedup_all_kernels = {q : -1 for q in Q_SIZES}

    for kernel, a_size in KERNEL_ASIZE_PAIRS.items():
        print('Running kernel:', kernel)

        # Ensure dir structure exists
        Path(f'{EXP_DATA_DIR}/').mkdir(parents=True, exist_ok=True)
        res_file = f'{EXP_DATA_DIR}/{kernel}_{bin_type}.csv'

        with open(res_file, 'w') as f:
            writer = csv.writer(f)
            writer.writerow(['percentage', 'static'] + [f'dynamic_{q}qsize' for q in Q_SIZES])

            # Record lowest-highest speedup against static across all % for each q_size.
            lowest_speedup = {q : 100000 for q in Q_SIZES}
            highest_speedup = {q : 0 for q in Q_SIZES}
            all_speedups = {q : [] for q in Q_SIZES}

            for percentage in PERCENTAGES_WAIT:
                print(f'\nRunning with percentage_wait {percentage} %')

                new_row = [percentage]

                BIN_STATIC = f'{kernel}/bin/{kernel}.{bin_ext}'
                static_time = run_bin(BIN_STATIC, a_size, percentage=percentage)
                new_row.append(static_time)

                for q in Q_SIZES:
                    BIN_DYNAMIC = f'{kernel}/{kernel}.cpp.tmp.cpp_{q}qsize.{bin_ext}' 
                    dyn_time = run_bin(BIN_DYNAMIC, a_size, percentage=percentage)
                    new_row.append(dyn_time)

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

            print(kernel)
            print(lowest_speedup_row)
            print(highest_speedup_row)
            print(gmean_speedup_row)
            print()

            # Update for all kernels.
            for i_q, q in enumerate(Q_SIZES):
                # Need to init to first gmean for this to work.
                if gmean_speedup_all_kernels[q] == -1:
                    gmean_speedup_all_kernels[q] = gmean_speedup[i_q]

                gmean_speedup_all_kernels[q] = gmean_rounded([gmean_speedup_all_kernels[q], 
                                                               gmean_speedup[i_q]])
                lowest_speedup_all_kernels[q] = min(lowest_speedup_all_kernels[q], lowest_speedup[q])
                highest_speedup_all_kernels[q] = max(highest_speedup_all_kernels[q], highest_speedup[q])


        # Emulation perf results are meaningless, delete.
        if is_emu:
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


