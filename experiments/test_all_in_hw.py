#!/usr/bin/env python3

import os

BENCHMARKS = {
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


}

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]


if __name__ == '__main__':
    TARGET = 'hw'

    for kernel, a_size in BENCHMARKS.items():
        print(f'\n--------------------- {kernel} ---------------------')

        # print("-- Builiding ...")
        # if 'bram' in kernel:
        #     os.system(f'cd {GIT_DIR} && ./elastic_pass.sh {TARGET} experiments/{kernel}.cpp')

        # os.system(f'cd {GIT_DIR}/experiments && make fpga_{TARGET} FILE={kernel}.cpp')
        # print("-- Builiding End")

        # print(f"Static:")
        # os.system(f'cat {GIT_DIR}/experiments/bin/{kernel}.fpga_hw.prj/acl_quartus_report.txt')
        # print(f"Elastic:")
        # os.system(f'cat {GIT_DIR}/experiments/bin/{kernel}.elastic.fpga_hw.prj/acl_quartus_report.txt')

        BIN_STATIC = f'{GIT_DIR}/experiments/bin/{kernel}.fpga_{TARGET}'
        BIN_ELASTIC = f'{GIT_DIR}/experiments/bin/{kernel}.elastic.fpga_{TARGET}'
        for p in [0, 100]:
            print(f"-- Running ({p}% of iterations have a dependency) ...")
            print(f"Static:")
            os.system(f'./{GIT_DIR}/experiments/bin/{kernel}.fpga_hw {a_size} {p}')
            print(f"Elastic:")
            os.system(f'./{GIT_DIR}/experiments/bin/{kernel}.elastic.fpga_hw {a_size} {p}')
