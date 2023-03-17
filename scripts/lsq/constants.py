import os
import time

GIT_DIR = os.environ['LLVM_SYCL_PASSES_DIR']
INPUTS_DIR = f'{GIT_DIR}/inputs/lsq'
EXP_DATA_DIR = f'{GIT_DIR}/scripts/lsq/experiments/results/'

SIM_CYCLES_FILE = f'{GIT_DIR}/inputs/lsq/simulation_raw.json'
TMP_FILE = f'{GIT_DIR}/inputs/lsq/.tmp_run_exp{str(time.time())[-5:]}.txt'

KERNEL_ASIZE_FOR_BENCHMARKS = {
    'histogram' : 10000, # How large the input array should be for benchmarking.
    'spmv' : 100,
    'chaos_ncg' : 10000,
    'bnn' : 100,
    'get_tanh' : 10000,
    'get_tanh_double' : 10000,
    'vec_trans' : 10000,
    'sssp' : 100,
    'maximal_matching' : 10000,
    'sort' : 100,
}

# Superset of {KERNEL_ASIZE_FOR_BENCHMARKS}
KERNEL_ASIZE_FOR_TESTS = {
    'histogram' : 1000,
    'spmv' : 20,
    'get_tanh' : 1000,
    'get_tanh_double' : 1000,
    'maximal_matching' : 1000,
    'bnn' : 100,
    'vec_trans' : 1000,
    'chaos_ncg' : 100,
    'sssp' : 10,
    'sort' : 20,
    'histogram_2_addresses' : 100,
    'histogram_2_addresses_1_decoupled' : 100,
    'histogram_if' : 100,
    'histogram_if_2' : 100,
    'histogram_if_3' : 100,
}
