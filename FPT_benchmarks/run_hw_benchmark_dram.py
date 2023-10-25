#!/usr/bin/env python3

import sys
import os
import csv
import json
from statistics import geometric_mean, harmonic_mean

sys.path.insert(1, f'{os.environ["ELASTIC_SYCL_HLS_DIR"]}/scripts')

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]
CSV_FILE = f'our_results_dram_in_hw.csv' # Table 3 from paper

# Get best-out-of REPEAT
REPEAT = 3

BENCHMARKS = {
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

hmean_rounded = lambda s_ups : round(harmonic_mean(s_ups), 2)
gmean_rounded = lambda s_ups : round(geometric_mean(s_ups), 2)

def run_and_get_exec_time(bin, arrays_size, percentage):
    print(f'Running {bin.split("/")[-1]} in hw with {percentage}% true data hazards')
    min_time = None

    tmp_file = f'{GIT_DIR}/tmp_get_time.txt'
    for _ in range(REPEAT):
        os.system(f'{bin} {arrays_size} {percentage} > {tmp_file}')

        with open(tmp_file, 'r') as f:
            out_lines = f.readlines()
            for line in out_lines:
                if 'Kernel time' in line:
                    time = float(line.split(':')[-1])
                    if min_time is None:
                        min_time = time
                    elif time < min_time:
                        min_time = time
    
    min_time = round(min_time * 1000, 1) # ms to micro s
    print(f"Time (micro s): ", min_time)
    return min_time

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
    with open(CSV_FILE, 'w') as f:
        writer = csv.writer(f)
        writer.writerow(['Benchmark', 
                        'Exec time Intel (micro s)', 'Exec time Ours (micro s)', 'Exec time Ours (micro s)', 'Exec time Ours/Intel', 'Exec time Ours/Intel', 
                        'Freq Intel (MHz)', 'Freq Ours (MHz)', 'Freq Ours/Intel',
                        'Area Intel (ALM)', 'Area Ours (ALM)', 'Area Ours/Intel (ALM)'])

        ratios_freq = []
        ratios_exec_time_0 = []
        ratios_exec_time_1 = []
        ratios_area = []

        for kernel, a_size in BENCHMARKS.items():
            print(f'\n --- {kernel} --- ')
            
            BIN_INTEL = f'{GIT_DIR}/experiments/bin/{kernel}.fpga_hw'
            BIN_OURS = f'{GIT_DIR}/experiments/bin/{kernel}.elastic.fpga_hw'
            
            row = [kernel.replace("_dram", "")]

            exec_time_intel = run_and_get_exec_time(BIN_INTEL, a_size, 0)
            row.append(exec_time_intel)
            
            exec_time_ours = []
            for p in [0, 50, 100]: # different % of true data hazards
                exec_time_ours.append(run_and_get_exec_time(BIN_OURS, a_size, p))

            # TIme column
            row.append(min(exec_time_ours))
            row.append(max(exec_time_ours))
            row.append(round(exec_time_ours[0]/exec_time_intel, 2))
            row.append(round(exec_time_ours[1]/exec_time_intel, 2))

            # Frequency column
            area_intel = get_resource_report(f"{GIT_DIR}/FPT_benchmarks/intelHLS_reports/{kernel}")
            area_ours = get_resource_report(f"{GIT_DIR}/FPT_benchmarks/our_reports/{kernel}")
            row.append(area_intel['Freq'])
            row.append(area_ours['Freq'])
            row.append(round(area_ours['Freq']/area_intel['Freq'], 2))

            # Area column
            row.append(area_intel['ALM'])
            row.append(area_ours['ALM'])
            row.append(round(area_ours['ALM']/area_intel['ALM'], 1))

            # Harmonic mean calculation
            ratios_freq.append(area_ours['Freq']/area_intel['Freq'])
            ratios_exec_time_0.append(min(exec_time_ours)/exec_time_intel)
            ratios_exec_time_1.append(max(exec_time_ours)/exec_time_intel)
            ratios_area.append(area_ours['ALM']/area_intel['ALM'])

            # Add columns to row
            writer.writerow(row)
            # Save file after each new row 
            f.flush()

        # Harmonic mean row at the end
        writer.writerow(['Harmonic mean', 
                        '', '', '', hmean_rounded(ratios_exec_time_0), hmean_rounded(ratios_exec_time_1),
                        '', '', hmean_rounded(ratios_freq),
                        '', '', hmean_rounded(ratios_area)])
