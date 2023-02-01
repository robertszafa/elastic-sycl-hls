import os
import time

GIT_DIR = os.environ['LLVM_SYCL_PASSES_DIR']
EXP_DATA_DIR = f'{GIT_DIR}/inputs/experiments/results/'

KERNEL_ASIZE_PAIRS = {
    'histogram' : 10000,
    # # 'histogram_if' : 10000,
    # # 'histogram_if_2' : 10000,
    # # 'histogram_if_3' : 10000,
    'get_tanh' : 10000,
    'get_tanh_double' : 10000,
    'maximal_matching' : 5000,
    'spmv' : 100,
    'bnn' : 100,
    'vec_trans' : 10000,
    'chaos_ncg' : 5000,
    'sssp' : 100,
    'sort' : 100,
    # 'delaunay_triangulation' : 100,
}
# Decrease domain sizes when running in simulation.
KERNEL_ASIZE_PAIRS_SIM = {
    'histogram' : 1000,
    'histogram_if' : 1000,
    'histogram_if_2' : 1000,
    'histogram_if_3' : 1000,
    'get_tanh' : 1000,
    'get_tanh_double' : 1000,
    'maximal_matching' : 1000,
    'spmv' : 20,
    'bnn' : 100,
    'vec_trans' : 1000,
    'chaos_ncg' : 100,
    'sssp' : 100,
    'sort' : 20,
    # 'delaunay_triangulation' : 20,
}

KERNELS = list(KERNEL_ASIZE_PAIRS.keys())

DATA_DISTRIBUTIONS = {
    0: 'all_wait',
    1: 'no_wait',
    # 2: 'percentage_wait' # run_exp_all_percentages runs for different % of data hazards.
}

PERCENTAGE_WAIT = 10
PERCENTAGES_WAIT = [0, 40, 80, 100]

SIM_CYCLES_FILE = f'{GIT_DIR}/inputs/simulation_raw.json'
TMP_FILE = f'{GIT_DIR}/inputs/.tmp_run_exp{str(time.time())[-5:]}.txt'

DEVCLOUD_LOGIN = f'{os.environ["DEVCLOUD_USERNAME"]}@devcloud'
