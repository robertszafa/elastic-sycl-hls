#!/usr/bin/env python3

import sys
import os
import csv
import json
from statistics import geometric_mean, harmonic_mean

sys.path.insert(1, f'{os.environ["ELASTIC_SYCL_HLS_DIR"]}/scripts')

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]
CSV_FILE_BRAM = f'our_results_bram.csv' # Table 1 from paper
CSV_FILE_DRAM = f'our_results_dram.csv' # Table 3 from paper

BENCHMARKS_DRAM = {
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
    'bitonic_sort_dram': 64,
}
BENCHMARKS_BRAM = {
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
    'bitonic_sort_bram': 64,
}

hmean_rounded = lambda s_ups : round(harmonic_mean(s_ups), 2)
gmean_rounded = lambda s_ups : round(geometric_mean(s_ups), 2)

def run_and_get_cycles(bin, arrays_size, percentage):
    print(f'Simulating {bin.split("/")[-1]} with {percentage}% true data hazards')

    run_sim_script = f'{GIT_DIR}/scripts/run_sim.sh'
    os.system(f'{run_sim_script} {bin} {arrays_size} {percentage} > .tmp_get_cycles.txt')

    with open('.tmp_get_cycles.txt', 'r') as f:
        out_lines = f.readlines()
        for line in out_lines:
            if ''.join([x for x in line if x.isalpha()]) == 'MainKernel':
                num_cycles = int(''.join([x for x in line if x.isnumeric()]))
                print("Cycles: ", num_cycles)
                return num_cycles
    
    return 1

def get_resource_report(report_dir):
    res = {'ALM' : 0, 'RAM' : 0, 'DSP' : 0, 'Freq' : 0}
    try:
        with open(f'{report_dir}/resources/json/quartus.json', 'r') as f:
            report_str = f.read()
            data = json.loads(report_str)

            res['Freq'] = int(float(data['quartusFitClockSummary']['nodes'][0]['kernel clock']))
            for node in data['quartusFitResourceUsageSummary']['nodes']:
                if node['name'] == 'Quartus Fitter: Kernel System':
                    res['ALM'] = int(float(node['alm']))

    except Exception as e:
        print('\n*Exception*\n')
        print(e)
        exit()
    
    return res



if __name__ == '__main__':
    if len(sys.argv) < 2 or (sys.argv[1] != 'dram' and sys.argv[1] != 'bram'):
        exit("Usage: run_sum_benchmarks.py bram|dram")

    CSV_FILE = CSV_FILE_DRAM if 'dram' == sys.argv[1] else CSV_FILE_BRAM
    BENCHMARKS = BENCHMARKS_DRAM if 'dram' == sys.argv[1] else BENCHMARKS_BRAM

    with open(CSV_FILE, 'a') as f:
        writer = csv.writer(f)
        writer.writerow(['Benchmark', 'Cycles Intel', 'Cycles Ours', 'Cycles Ours', 'Cycles Ours/Intel', 'Cycles Ours/Intel', 
                        'Freq Intel (MHz)', 'Freq Ours (MHz)', 'Freq Ours/Intel',
                        'Exec time Intel (micro s)', 'Exec time Ours (micro s)', 'Exec time Ours (micro s)', 'Exec time Ours/Intel', 'Exec time Ours/Intel', 
                        'Area Intel (ALM)', 'Area Ours (ALM)', 'Area Ours/Intel (ALM)'])

        ratios_cycle_0 = []
        ratios_cycle_1 = []
        ratios_freq = []
        ratios_exec_time_0 = []
        ratios_exec_time_1 = []
        ratios_area = []

        for kernel, a_size in BENCHMARKS.items():
            print(f'\n --- {kernel} --- ')
            
            BIN_INTEL = f'{GIT_DIR}/experiments/bin/{kernel}.fpga_sim'
            BIN_OURS = f'{GIT_DIR}/experiments/bin/{kernel}.elastic.fpga_sim'
            if not os.path.exists(BIN_INTEL) or not os.path.exists(BIN_OURS):
                print(f"Missing binary:\n{BIN_INTEL}\nor\n{BIN_OURS}")
                print(f"See build_all.py to build")
                print(f"Skipping {kernel}...")
                continue

            
            row = [kernel.replace("_dram", "")]

            cycles_intel = run_and_get_cycles(BIN_INTEL, a_size, 0)
            row.append(cycles_intel)
            
            cycles_ours = []
            for p in [0, 50, 100]: # different % of true data hazards
                cycles_ours.append(run_and_get_cycles(BIN_OURS, a_size, p))

            # Cycles column
            row.append(min(cycles_ours))
            row.append(max(cycles_ours))
            row.append(round(cycles_ours[0]/cycles_intel, 2))
            row.append(round(cycles_ours[1]/cycles_intel, 2))

            # Frequency column
            area_intel = get_resource_report(f"{GIT_DIR}/FPT_benchmarks/intelHLS_reports/{kernel}")
            area_ours = get_resource_report(f"{GIT_DIR}/FPT_benchmarks/our_reports/{kernel}")
            row.append(area_intel['Freq'])
            row.append(area_ours['Freq'])
            row.append(round(area_ours['Freq']/area_intel['Freq'], 2))

            # Execution time column (calculated from cycles and frequency)
            exec_time_intel = cycles_intel * (1000 / area_intel['Freq']) / 1000
            exec_time_ours0 = cycles_ours[0] * (1000 / area_ours['Freq']) / 1000
            exec_time_ours1 = cycles_ours[1] * (1000 / area_ours['Freq']) / 1000
            row.append(round(exec_time_intel, 2))
            row.append(round(exec_time_ours0, 2))
            row.append(round(exec_time_ours1, 2))
            row.append(round(exec_time_ours0/exec_time_intel, 2))
            row.append(round(exec_time_ours1/exec_time_intel, 2))

            # Area column
            row.append(area_intel['ALM'])
            row.append(area_ours['ALM'])
            row.append(round(area_ours['ALM']/area_intel['ALM'], 1))

            # Harmonic mean calculation
            ratios_cycle_0.append(cycles_ours[0]/cycles_intel)
            ratios_cycle_1.append(cycles_ours[1]/cycles_intel)
            ratios_freq.append(area_ours['Freq']/area_intel['Freq'])
            ratios_exec_time_0.append(exec_time_ours0/exec_time_intel)
            ratios_exec_time_1.append(exec_time_ours1/exec_time_intel)
            ratios_area.append(area_ours['ALM']/area_intel['ALM'])

            # Add columns to row
            writer.writerow(row)
            # Save file after each new row 
            f.flush()

        # Harmonic mean row at the end
        writer.writerow(['Harmonic mean', '', '', '', hmean_rounded(ratios_cycle_0), hmean_rounded(ratios_cycle_1), 
                        '', '', hmean_rounded(ratios_freq),
                        '', '', '', hmean_rounded(ratios_exec_time_0), hmean_rounded(ratios_exec_time_1),
                        '', '', hmean_rounded(ratios_area)])
