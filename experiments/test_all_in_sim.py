#!/usr/bin/env python3

import os

BENCHMARKS = {
    'filter_sum': 1000,
    'get_tanh_if': 1000,
    'vec_norm_trans': 1000,
    'spmv_if': 1000,
    'doitgen_triple': 100,
    'eval_pos': 1000,

    'histogram_bram': 1000,
    'get_tanh_bram': 1000,
    'get_tanh_double_bram': 1000,
    'vec_trans_bram': 1000,
    'spmv_bram': 20,
    'chaos_ncg_bram': 1000,
    'bnn_bram': 100,
    'histogram_if_bram': 1000,
    'maximal_matching_bram': 1000,
    'floyd_warshall_bram': 10,

    # DRAM LSQ codes need to be run on a real board to get meaningful perf numbers
    'histogram_dram': 1000,
    'get_tanh_dram': 1000,
    'get_tanh_double_dram': 1000,
    'vec_trans_dram': 1000,
    'spmv_dram': 20,
    'chaos_ncg_dram': 1000,
    'bnn_dram': 100,
    'histogram_if_dram': 1000,
    'maximal_matching_dram': 1000,
    'floyd_warshall_dram': 10,
}

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]

def run_and_get_cycles(bin, arrays_size, percentage):
    TMP_FILE = '.tmp_get_cycles.txt'
    run_sim_script = f'{GIT_DIR}/scripts/run_sim.sh'
    os.system(f'{run_sim_script} {bin} {arrays_size} {percentage} > {TMP_FILE}')

    cycles = 1

    with open('.tmp_get_cycles.txt', 'r') as f:
        out_lines = f.readlines()
        if 'Passed' in ''.join(out_lines):
            print("Passed")
        else:
            print("Failed")

        for line in out_lines:
            if ''.join([x for x in line if x.isalpha()]) == 'MainKernel':
                cycles = int(''.join([x for x in line if x.isnumeric()]))
                break

    os.system(f'rm -f {TMP_FILE}')

    return cycles


if __name__ == '__main__':
    bin_ext = 'fpga_sim'

    for kernel, a_size in BENCHMARKS.items():
        print(f'\n--------------------- {kernel} ---------------------')

        print("-- Builiding ...")
        os.system(f'cd {GIT_DIR} && ./elastic_pass.sh sim experiments/{kernel}.cpp')
        print("-- Builiding End")

        BIN_ELASTIC = f'{GIT_DIR}/experiments/bin/{kernel}.elastic.{bin_ext}'
        for p in [0, 100]:
            print(f"-- Running ({p}% of iterations have a dependency) ...")
            res = run_and_get_cycles(BIN_ELASTIC, a_size, p)
            print(f'-- Cycles: {res}')
