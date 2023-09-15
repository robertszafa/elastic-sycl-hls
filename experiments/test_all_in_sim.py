#!/usr/bin/env python3

import os

BENCHMARKS = {
    # LSQ protecting BRAM (set TEST pragma in the files to test correctness)
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

    # Decoupled single basic blocks.    
    'filter_sum': 1000,
    'get_tanh_if': 1000,
    'vec_norm_trans': 1000,
    'doitgen_triple': 100,
    'eval_pos': 1000,
    # Decoupled whole loops.    
    'spmv_if': 1000,

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

    # Some edge cases to stress the composability of 1). decoupling BBs 2). decoupling loops 3). LSQ 4). Address Gen 5). Speculation
    'spmv_if_2': 1000, 
    'spmv_if_pe_and_lsq_dram': 1000, # decoupled for loop that uses an LSQ.
    'spmv_if_pe_and_lsq_bram': 1000, # decoupled for loop that uses an LSQ.
    'histogram_2_addresses_1_speculated': 1000, # 2 LSQ, 1 with speculation
    'histogram_2_addresses_1_decoupled': 1000, # 2 LSQ, 1 without decoupled addresses
    'doitgen_triple_2': 100, # decoupled BB, LSQ connection to both main loop and PE, speculated store where the true BB is decoupled
    'doitgen_triple_3': 100, # decoupled BB, LSQ connection to both main loop and PE, speculation load & store where the true BB is decoupled
    'filter_sum_2_bram': 1000,
    'filter_sum_2_dram': 1000,
    'nested_if_mul': 1000,
    'histogram_dram_2loops': 1000, # Sibling loops decoupled
    'histogram_dram_2loops_1nested': 1000, # Sibling loops decoupled
    'histogram_dram_4loops_1triplenested': 1000, # Sibling loops decoupled
}

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]

def run_and_get_cycles(bin, arrays_size, percentage):
    TMP_FILE = '.tmp_get_cycles.txt'
    run_sim_script = f'{GIT_DIR}/scripts/run_sim.sh'
    os.system(f'{run_sim_script} {bin} {arrays_size} {percentage} > {TMP_FILE}')

    cycles = 1

    with open('.tmp_get_cycles.txt', 'r') as f:
        out_lines = f.readlines()
        if 'Failed' in ''.join(out_lines):
            print("Failed")
        else:
            print("Passed")

        for line in out_lines:
            if ''.join([x for x in line if x.isalpha()]) == 'MainKernel':
                cycles = int(''.join([x for x in line if x.isnumeric()]))
                break

    os.system(f'rm -f {TMP_FILE}')

    return cycles


if __name__ == '__main__':
    TARGET = 'sim' # change to emu if only testing compiler robustness

    for kernel, a_size in BENCHMARKS.items():
        print(f'\n--------------------- {kernel} ---------------------')

        print("-- Builiding ...")
        os.system(f'cd {GIT_DIR} && ./elastic_pass.sh {TARGET} experiments/{kernel}.cpp')

        BIN_ELASTIC = f'{GIT_DIR}/experiments/bin/{kernel}.elastic.fpga_{TARGET}'

        if TARGET == "sim":
            for p in [0, 100]:
                print(f"-- Running ({p}% of iterations have a dependency) ...")
                res = run_and_get_cycles(BIN_ELASTIC, a_size, p)
                print(f'-- Cycles: {res}')
