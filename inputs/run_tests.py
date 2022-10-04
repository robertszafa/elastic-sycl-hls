import sys
import os
import re
import csv
from pathlib import Path


EXP_DATA_DIR = 'exp_data/'

KERNEL_ASIZE_PAIRS = {
    'histogram' : 1000000,
    'histogram_if' : 1000000,
    'histogram_if_2' : 1000000,
}
# Decrease domain sizes when running in simulation.
KERNEL_ASIZE_PAIRS_SIM = {
    'histogram' : 100,
    'histogram_if' : 100,
    'histogram_if_2' : 100,
}

DATA_DISTRIBUTIONS = {
    0: 'all_wait',
    1: 'no_wait',
    # 2: 'percentage_wait' # run_exp_all_percentages runs for different % of data hazards.
}

# Q_SIZES_DYNAMIC = [1, 2, 4, 8, 16, 32, 64]
# Q_SIZES_DYNAMIC_NO_FORWARD = [1, 2, 4, 8, 16, 32, 64]
Q_SIZES_DYNAMIC = [4]
Q_SIZES_DYNAMIC_NO_FORWARD = [4]

PERCENTAGE_WAIT = 5

SIM_CYCLES_FILE = 'simulation_raw.json'
TMP_FILE = '.tmp_run_exp.txt'


def run_bin(bin, a_size, distr=0, percentage=0):
    print(f'> {bin} : ', end='')
    os.system(f'{bin} {a_size} {distr} {percentage} > {TMP_FILE}')

    stdout = ''
    with open(TMP_FILE, 'r') as f:
        stdout = str(f.read())
    if not 'Passed' in stdout:
        print(f' - Fail in {bin} {a_size} {distr} {percentage}')
    
    if 'fpga_sim' in bin: 
        # Get cycle count
        with open(SIM_CYCLES_FILE, 'r') as f:
            match = re.search(r'"time":"(\d+)"', f.read())
        if (match):
            print(f'{int(match.group(1))}')
            return int(match.group(1))
    else: 
        # Get time
        match = re.search(r'Kernel time \(ms\): (\d+\.\d+|\d+)', stdout)
        if (match):
            print(f'{float(match.group(1))}')
            return float(match.group(1))


if __name__ == '__main__':
    is_sim = any('sim' in arg for arg in sys.argv[1:])

    BIN_EXTENSION = 'fpga_sim' if is_sim else 'fpga'
    KERNEL_ASIZE_PAIRS = KERNEL_ASIZE_PAIRS_SIM if is_sim else KERNEL_ASIZE_PAIRS

    for kernel, a_size in KERNEL_ASIZE_PAIRS.items():
        print('\n--Building kernel:', kernel)
        os.system(f'./driver.sh sim {kernel}/{kernel}.cpp > {TMP_FILE}')
        BIN_DYNAMIC = f'{kernel}/bin/{kernel}.cpp.{BIN_EXTENSION}'

        for distr_idx, distr_name in DATA_DISTRIBUTIONS.items():
            print(f'--Running {kernel} with distr {distr_name}')
            dyn_time = run_bin(BIN_DYNAMIC, a_size, distr=distr_idx, percentage=PERCENTAGE_WAIT)
        

    os.system(f'rm {TMP_FILE}')


