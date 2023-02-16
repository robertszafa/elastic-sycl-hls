import os
import time

GIT_DIR = os.environ['LLVM_SYCL_PASSES_DIR']
EXP_DATA_DIR = f'{GIT_DIR}/scripts/lsq/experiments/results/'

SIM_CYCLES_FILE = f'{GIT_DIR}/inputs/lsq/simulation_raw.json'
TMP_FILE = f'{GIT_DIR}/inputs/lsq/.tmp_run_exp{str(time.time())[-5:]}.txt'

KERNEL_ASIZE_FOR_BENCHMARKS = {
    'histogram' : 10000, # How large the input array should be for benchmarking.
    'spmv' : 100,
    'get_tanh' : 10000,
    'get_tanh_double' : 10000,
    'maximal_matching' : 5000,
    'bnn' : 100,
    'vec_trans' : 10000,
    'chaos_ncg' : 5000,
    'sssp' : 100,
    'sort' : 100,
}

# Superset of {KERNEL_ASIZE_FOR_BENCHMARKS}
KERNEL_ASIZE_FOR_TESTS = {
    'histogram' : 1000,
    'spmv' : 20,
    'histogram_2_addresses' : 1000,
    'histogram_2_addresses_1_decoupled' : 1000,
    'histogram_if' : 1000,
    'histogram_if_2' : 1000,
    'histogram_if_3' : 1000,
    'get_tanh' : 1000,
    'get_tanh_double' : 1000,
    'maximal_matching' : 1000,
    'bnn' : 100,
    'vec_trans' : 1000,
    'chaos_ncg' : 100,
    'sssp' : 100,
    'sort' : 20,
}
