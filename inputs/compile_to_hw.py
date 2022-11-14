import sys
import os
import re
import csv
from pathlib import Path


KERNELS = [
    'histogram',
    'histogram_if',
    'get_tanh',
    'maximal_matching',
    'spmv',
    'bnn',
    'vec_trans',
    'chaos_ncg',
    'get_tanh_double',
    'sssp',
]

if __name__ == '__main__':

    for qsize in [8]:
        for kernel in KERNELS:
            print('\n--Building kernel:', kernel)

            src_file = f'{kernel}/{kernel}.cpp.tmp.cpp_{qsize}qsize'
            bc_file = f'{src_file}.out.bc'
            bin_name = f'{src_file}.fpga_hw'

            # Not needed if integration header/footer was sent.
            # os.system(f'./scripts/compile_to_bc.sh hw {src_fi# Not neededle}') 

            os.system(f'./scripts/compile_from_bc.sh hw {bc_file} {src_file} {bin_name}')
