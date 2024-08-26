#!/usr/bin/env python3

import os
import json

def get_alms(project_dir):
    res_main = {'ALM' : 0, 'REG': 0, 'RAM' : 0, 'DSP' : 0, 'Freq' : 0}
    res_agu = {'ALM' : 0, 'REG': 0, 'RAM' : 0, 'DSP' : 0, 'Freq' : 0}

    try:
        with open(f'{project_dir}/resources/quartus_data.js', 'r') as f:
            report_str = f.read()
            report_str = report_str[16:-2]
            data = json.loads(report_str)

            res_main['Freq'] = int(float(data['quartusFitClockSummary']['nodes'][0]['kernel clock']))
            for node in data['quartusFitResourceUsageSummary']['nodes']:

                if node['type'] == 'kernel' and "MainKernel" in node['name'] and 'AGU' not in node['name']:
                    res_main['ALM'] += int(float(node['alm']))
                    res_main['REG'] += int(float(node['reg']))
                    res_main['RAM'] += int(float(node['ram']))
                    res_main['DSP'] += int(float(node['dsp']))

                if node['type'] == 'kernel' and "MainKernel_AGU" in node['name']:
                    res_agu['ALM'] += int(float(node['alm']))
                    res_agu['REG'] += int(float(node['reg']))
                    res_agu['RAM'] += int(float(node['ram']))
                    res_agu['DSP'] += int(float(node['dsp']))

    except Exception as e:
        print(f'\nException in get_alm({project_dir})\n')
        print(e)
        exit()

    return res_main['ALM'], res_agu['ALM']


THIS_DIR = os.path.dirname(os.path.realpath(__file__))
CYCLES_CSV_FILE = f"{THIS_DIR}/performance.csv"
LATEX_TABLE_FILE = f"{THIS_DIR}/table.txt"

keys = [
    "bfs",
    "bc",
    "sssp_queue",
    "histogram_if",
    "threashold",
    "maximal_matching",
    "floyd_warshall",
    "bitonic_sort",
    "spmv_if",
]
cycles = {k: [] for k in keys} # each key holds 4 values: INORD, DAE, SPEC_DAE, ORACLE; in this order
area = {k: [] for k in keys}
num_poison_calls = {
    "bfs": 1,
    "bc": 2,
    "sssp_queue": 1,
    "histogram_if": 1,
    "threashold": 3,
    "maximal_matching": 2,
    "floyd_warshall": 1,
    "bitonic_sort": 2,
    "spmv_if": 1,
}
num_poison_blocks = {
    "bfs": 1,
    "bc": 2,
    "sssp_queue": 1,
    "histogram_if": 1,
    "threashold": 1,
    "maximal_matching": 1,
    "floyd_warshall": 1,
    "bitonic_sort": 1,
    "spmv_if": 1,
}
# Collected by countin in the LSQ (accounting for memory transfer stores).
percentage_misspec = {
    "bfs": "95%",
    "bc": "95%, 82%",
    "sssp_queue": "95%",
    "histogram_if": "2%",
    "threashold": "97%",
    "maximal_matching": "32%",
    "floyd_warshall": "31%",
    "bitonic_sort": "85%",
    "spmv_if": "49%",
}


with open(CYCLES_CSV_FILE, 'r') as f:
    lines = f.readlines()
    i = 0
    for line in lines:
        if ',' not in line or 'kernel,' in line:
            continue # empty lines etc
        vals = line.split(',')

        kernel = keys[i]
        i += 1

        cycles[kernel].append(vals[1])
        cycles[kernel].append(vals[2])
        cycles[kernel].append(vals[3])
        cycles[kernel].append(vals[4])

        area[kernel].append(get_alms(f"{THIS_DIR}/bin/{kernel}_bram_INORD"))
        area[kernel].append(get_alms(f"{THIS_DIR}/bin/{kernel}_bram_DAE"))
        area[kernel].append(get_alms(f"{THIS_DIR}/bin/{kernel}_bram_SPEC_DAE"))
        area[kernel].append(get_alms(f"{THIS_DIR}/bin/{kernel}_bram_ORACLE"))

        area_overhead = (1 - (sum(area[kernel][-2]) / sum(area[kernel][-1]))) * 100
        print(f"{kernel} area_overhead = {area_overhead}")



with open(LATEX_TABLE_FILE, 'w') as f:
    f.write("Kernel & Poison Blocks & Poison Calls & Mis-spec. rate & Cycles INORD & Cycles DAE & Cycles SPEC_DAE & Cycles ORACLE & Area INORD & Area DAE & Area SPEC_DAE & Area ORACLE\n")

    for kernel in keys:
        f.write(f"{kernel}")
        f.write(f" & {num_poison_blocks[kernel]}")
        f.write(f" & {num_poison_calls[kernel]}")
        f.write(f" & {percentage_misspec[kernel]}")

        if len(cycles) != len(area):
            exit("len(cycles) != len(area)")

        for c in cycles[kernel]:
            c = c.strip()
            f.write(f" & {int(c):,}")
        for a in area[kernel]:
            # f.write(f" & {a[0]:,} + {a[1]:,}")
            f.write(f" & {a[0] + a[1]:,}")
        
        f.write("\n")

    print(f"Saved table to {LATEX_TABLE_FILE}")
