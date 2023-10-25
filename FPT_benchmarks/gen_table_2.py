#!/usr/bin/env python3

import sys
import os
import csv
import json
from statistics import geometric_mean, harmonic_mean

sys.path.insert(1, f'{os.environ["ELASTIC_SYCL_HLS_DIR"]}/scripts')

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]
CSV_FILE = f'table_2.csv' 

LSQ_SIZES = [2, 4, 8, 16, 32, 64, 128, 256]

KERNEL = 'histogram_bram'

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

        baseline = get_resource_report(f"{GIT_DIR}/FPT_benchmarks/intelHLS_reports/{KERNEL}")
        base_freq = baseline['Freq']
        base_area = baseline['ALM']
        writer.writerow(['Queue Size', 'Freq', 'x', 'Area', 'x'])
        writer.writerow(['No LSQ', base_freq, 1, base_area, 1])

        for size in LSQ_SIZES:
            ours = get_resource_report(f"{GIT_DIR}/FPT_benchmarks/our_reports/{KERNEL}_lsq_{size}")

            writer.writerow([size, ours['Freq'], round(ours['Freq']/base_freq, 2), ours['ALM'], round(ours['ALM']/base_area, 2)])
           