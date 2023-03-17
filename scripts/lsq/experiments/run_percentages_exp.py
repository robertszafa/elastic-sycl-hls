#!/usr/bin/env python3

import csv
import argparse

from constants import *
from run_benchmarks_exp import run_bin

# We have space for only two.
THIS_EXP_KERNELS_ASIZES = {
    'histogram' : 10000,
    'spmv' : 100,
}
PERCENTAGES_WAIT = [0, 10, 20 , 30, 40, 50, 60, 70, 80, 90, 100]

Q_SIZE = 8


if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog="vis_exp_percentages.py",
                                     description=f"Measure the execution time of codes while varying the % of true data hazards.",
                                     formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument("--target", type=str, default='hw', help="What binary type to use (sim or hw). If not provided, hw is used.")
    args = parser.parse_args()

    Y_LABEL = 'Cycles' if args.target == 'sim' else 'Time - ms'

    for kernel, a_size in THIS_EXP_KERNELS_ASIZES.items():
        res_file = f'{EXP_DATA_DIR}/dep_percent_exp_{kernel}_{args.target}.csv'
        with open(res_file, 'w') as f:
            writer = csv.writer(f)
            writer.writerow(['percentage', 'static', f'lsq_{Q_SIZE}'])
            
            for percentage in PERCENTAGES_WAIT:
                BIN_STATIC = f'{INPUTS_DIR}/{kernel}/bin/{kernel}.fpga_{args.target}'
                static_time = run_bin(BIN_STATIC, a_size, percentage=0)

                BIN_DYNAMIC = f'{INPUTS_DIR}/{kernel}/bin/{kernel}.lsq_{Q_SIZE}.fpga_{args.target}' 
                dyn_time = run_bin(BIN_DYNAMIC, a_size, percentage=percentage)

                writer.writerow([percentage, static_time, dyn_time])


    os.system(f'rm {TMP_FILE}')
