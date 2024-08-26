#!/usr/bin/env python3

import os
import sys

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]
THIS_DIR = os.path.dirname(os.path.realpath(__file__))

BENCHMARKS = [
    'block_1',
    'block_2',
    'block_3',
    'block_4',
    'block_5',
    'block_6',
    'block_7',
    'block_8',
]


if __name__ == '__main__':
    TARGET = sys.argv[1] # change to emu if only testing compiler robustness

    for kernel in BENCHMARKS:
        print(f'\n--------------------- {kernel} ---------------------')

        print("\n\n============\nBuiliding INORDER\n=============")
        os.system(f'cd {THIS_DIR} && make fpga_{TARGET} FILE={kernel}.cpp')

        print("\n\n============\nBuiliding DAE\n=============")
        os.system(f'cp {THIS_DIR}/{kernel}.cpp {THIS_DIR}/{kernel}_dae.cpp && cd {GIT_DIR} && \
                    LSQ_SIZE=32 USE_ONE_AGU=1 NO_PE_DECOUPLING=1 NO_AGU_DECOUPLING=1 ./elastic_pass.sh {TARGET} {THIS_DIR}/{kernel}_dae.cpp')

        print("\n\n============\nBuiliding SPEC_DAE\n=============")
        os.system(f'cd {GIT_DIR} && LSQ_SIZE=32 USE_ONE_AGU=1 NO_PE_DECOUPLING=1 ./elastic_pass.sh {TARGET} {THIS_DIR}/{kernel}.cpp')

        print("\n\n============\nBuiliding ORACLE\n=============")
        os.system(f'cd {GIT_DIR} && LSQ_SIZE=32 USE_ONE_AGU=1 NO_PE_DECOUPLING=1 ./elastic_pass.sh {TARGET} {THIS_DIR}/{kernel}_oracle.cpp')
