#!/usr/bin/env python3

import sys
import os
import re

from constants import *



def run_bin(bin, a_size, distr=0, percentage=0):
    os.system(f'{bin} {a_size} {distr} {percentage} > {TMP_FILE}')

    stdout = ''
    with open(TMP_FILE, 'r') as f:
        stdout = str(f.read())
    if not 'Passed' in stdout:
        print(f'*FAIL*\n\t{bin} {a_size} {distr} {percentage}')
    else:
        print(f'PASSED')
    
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
            return float(match.group(1))


if __name__ == '__main__':
    exec_type = sys.argv[1] # sim/emu/hw
    is_debug = False
    if len(sys.argv) > 2:
        is_debug = sys.argv[2] == '-d'

    BIN_EXTENSION = 'fpga' 
    if 'sim' in exec_type:
        BIN_EXTENSION += '_sim'
        KERNEL_ASIZE_PAIRS = KERNEL_ASIZE_PAIRS_SIM
    elif 'emu' in exec_type:
        BIN_EXTENSION += '_emu'
        KERNEL_ASIZE_PAIRS = KERNEL_ASIZE_PAIRS_SIM
    
    for kernel, a_size in KERNEL_ASIZE_PAIRS.items():
        print('\nKernel:', kernel)
        compile_cmd = f'{GIT_DIR}/driver.sh {exec_type} {GIT_DIR}/inputs/{kernel}/{kernel}.cpp 8'
        if not is_debug:
            compile_cmd += f' > {TMP_FILE} 2>&1'
        os.system(compile_cmd)

        BIN_DYNAMIC = f'{GIT_DIR}/inputs/{kernel}/bin/{kernel}.cpp.{BIN_EXTENSION}'
        for distr_idx, distr_name in DATA_DISTRIBUTIONS.items():
            dyn_time = run_bin(BIN_DYNAMIC, a_size, distr=distr_idx, percentage=PERCENTAGE_WAIT)
        

    os.system(f'rm {TMP_FILE}')


