import os
import time

GIT_DIR = os.environ['LLVM_SYCL_PASSES_DIR']
INPUTS_DIR = f'{GIT_DIR}/inputs/cddd'
EXP_DATA_DIR = f'{GIT_DIR}/scripts/cddd/experiments/results/'

SIM_CYCLES_FILE = f'{GIT_DIR}/inputs/lsq/simulation_raw.json'
TMP_FILE = f'{GIT_DIR}/inputs/lsq/.tmp_run_exp{str(time.time())[-5:]}.txt'

KERNEL_ASIZE_FOR_BENCHMARKS = {
    'if_mul' : 10000, # How large the input array should be for benchmarking.
    'eval_pos' : 10000,
    'gsum' : 10000,
    'vec_norm_trans' : 10000,
    'doitgen_triple' : 10000,
}

# Superset of {KERNEL_ASIZE_FOR_BENCHMARKS}
KERNEL_ASIZE_FOR_TESTS = {
    'if_mul' : 100, # How large the input array should be for benchmarking.
    'nested_if_mul' : 100, 
    'if_mul_2' : 100, 
    'gsum' : 100,
    'doitgen_triple' : 100,
    # 'vec_norm_trans' : 100,
    # 'eval_pos' : 100,
}
