#!/usr/bin/env python3

import os
import sys


# To cimpile all codes for simulation takes ~1hrs on a laptop with 32GB RAM.
# 
# Hardware compiles take ~24hrs on the Intel devcloud (each benchmark is ~1.5hrs).
# The executables will be: {ELASTIC_SYCL_HLS_DIR}/experiments/bin/<kernel_name>.fpga_hw
# Quartus reports will be copied to: 
#   - for baseline to {ELASTIC_SYCL_HLS_DIR}/FPT_benchmarks/bin/intelHLS_reports/<kernel_name>
#   - for our work to {ELASTIC_SYCL_HLS_DIR}/FPT_benchmarks/bin/our_reports/<kernel_name>
BENCHMARKS = {
    'histogram_bram': 1000,
    'get_tanh_bram': 1000,
    'get_tanh_double_bram': 1000,
    'vec_trans_bram': 1000,
    'spmv_bram': 20,
    'chaos_ncg_bram': 1000,
    'bnn_bram': 100,
    'histogram_if_bram': 1000,
    'maximal_matching_bram': 1000,
    'floyd_warshall_bram': 100,
    'bitonic_sort_bram': 100,

    'histogram_dram': 1000,
    'get_tanh_dram': 1000,
    'get_tanh_double_dram': 1000,
    'vec_trans_dram': 1000,
    'spmv_dram': 20,
    'chaos_ncg_dram': 1000,
    'bnn_dram': 100,
    'histogram_if_dram': 1000,
    'maximal_matching_dram': 1000,
    'floyd_warshall_dram': 100,
    'bitonic_sort_dram': 100,
}

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]

if __name__ == '__main__':
    if len(sys.argv) < 2 or (sys.argv[1] != 'sim' and sys.argv[1] != 'hw'):
        exit("Usage: build_all.py sim|hw")
    
    target = sys.argv[1]

    if target == 'hw':
        os.system(f'mkdir -p {GIT_DIR}/FPT_benchmarks/our_reports')
        os.system(f'mkdir -p {GIT_DIR}/FPT_benchmarks/intelHLS_reports')

    for kernel, _ in BENCHMARKS.items():
        print(f'\n --- {kernel} --- ')

        print(f'Building baseline ...')
        os.system(f'cd {GIT_DIR}/experiments && make fpga_{target} FILE={kernel}.cpp')

        print(f'Building with our passes and LSQ ...')
        os.system(f'cd {GIT_DIR} && ./elastic_pass.sh {target} experiments/{kernel}.cpp')

        # Copy quartus reports into FPT_benchmarks/*_reports/kernel
        # We only really need reports/resources/json/quartus.json, but copy all anyway 
        if target == 'hw':
            os.system(f'mkdir -p {GIT_DIR}/FPT_benchmarks/our_reports/{kernel}')
            os.system(f'mkdir -p {GIT_DIR}/FPT_benchmarks/intelHLS_reports/{kernel}')

            os.system(f'cp -r {GIT_DIR}/experiments/bin/{kernel}.fpga_hw.prj/reports {GIT_DIR}/FPT_benchmarks/intelHLS_reports/{kernel}/')
            os.system(f'cp -r {GIT_DIR}/experiments/bin/{kernel}.elastic.fpga_hw.prj/reports {GIT_DIR}/FPT_benchmarks/our_reports/{kernel}/')
