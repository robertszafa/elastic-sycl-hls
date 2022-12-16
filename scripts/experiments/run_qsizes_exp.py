

import sys
import os
import re
import csv
from pathlib import Path

from constants import *

Q_SIZES = [2, 4, 8, 16]

# Only these two for this experiment
KERNEL_ASIZE_PAIRS = {
    'histogram' : 10000,
    'spmv' : 100,
}
# Decrease domain sizes when running in simulation.
KERNEL_ASIZE_PAIRS_SIM = {
    'histogram' : 1000,
    'spmv' : 20,
}


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

    for kernel, a_size in KERNEL_ASIZE_PAIRS.items():
        print('Running kernel:', kernel)

        # Ensure dir structure exists
        Path(f'{EXP_DATA_DIR}/').mkdir(parents=True, exist_ok=True)
        res_file = f'{EXP_DATA_DIR}/qsizes_exp_{kernel}_{bin_type}.csv'

        with open(res_file, 'w') as f:
            writer = csv.writer(f)
            writer.writerow(['qsize', 'time'])

            # Record lowest-highest speedup against static across all % for each q_size.
            lowest_speedup = {q : 100000 for q in Q_SIZES}
            highest_speedup = {q : 0 for q in Q_SIZES}
            all_speedups = {q : [] for q in Q_SIZES}

            BIN_STATIC = f'{GIT_DIR}/inputs/{kernel}/bin/{kernel}.{bin_ext}'
            # static_time = run_bin(BIN_STATIC, a_size, percentage=PERCENTAGE_WAIT)
            writer.writerow(['static', 355])

            for qsize in Q_SIZES:
                new_row = [qsize]

                BIN_DYNAMIC = f'{GIT_DIR}/inputs/{kernel}/{kernel}.cpp_{qsize}qsize.tmp.cpp.{bin_ext}' 
                if qsize == 8:
                    BIN_DYNAMIC = f'{GIT_DIR}/inputs/{kernel}/{kernel}.cpp.tmp.cpp_{qsize}qsize.{bin_ext}' 

                dyn_time = run_bin(BIN_DYNAMIC, a_size, percentage=PERCENTAGE_WAIT)

                writer.writerow([qsize, dyn_time])


        # Emulation perf results are meaningless, delete.
        if is_emu:
            os.system(f'rm {res_file}')


    os.system(f'rm {TMP_FILE}')


