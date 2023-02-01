#!/usr/bin/env python3

import os

from constants import GIT_DIR, KERNELS


if __name__ == '__main__':

    for qsize in [8]:
        for kernel in KERNELS:
            print('\n--Building kernel:', kernel)

            src_file = f'{GIT_DIR}/inputs/{kernel}/{kernel}.cpp.tmp.cpp_{qsize}qsize'
            bc_file = f'{GIT_DIR}/inputs/{src_file}.out.bc'
            bin_name = f'{GIT_DIR}/inputs/{src_file}.fpga_hw'

            # Needed if int. header sent?
            os.system(f'./compile_to_bc.sh hw {src_file}')
            os.system(f'./compile_from_bc.sh hw {bc_file} {src_file} {bin_name}')
