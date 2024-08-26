#!/usr/bin/env python3

import os
import matplotlib.pyplot as plt
import numpy as np
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
CSV_FILE = f"{THIS_DIR}/num_poison_blocks_bench.csv"

kernels = {
    'block_1': [1, 1],
    'block_2': [2, 3],
    'block_3': [3, 6],
    'block_4': [4, 10],
    'block_5': [5, 15],
    'block_6': [6, 21],
    'block_7': [7, 28],
    'block_8': [8, 36],
}
normalized_speedup = []
normalized_area_agu = []
normalized_area_main = []

MAX_SPEEDUP = 1

with open(CSV_FILE, 'r') as f:
    lines = f.readlines()
    for line in lines:
        if ',' not in line or 'kernel,' in line:
            continue # empty lines etc
        vals = line.split(',')
        kernel = vals[0]

        if kernel not in kernels.keys():
            break

        normalized_speedup.append(int((1.0 - float(vals[2]) / float(vals[1])) * 100))
        
        main_spec_area, agu_spec_area = get_alms(f"{THIS_DIR}/bin/{kernel}_SPEC_DAE")
        main_oracle_area, agu_oracle_area = get_alms(f"{THIS_DIR}/bin/{kernel}_ORACLE")

        normalized_area_main.append((float(main_spec_area - main_oracle_area) / float(main_oracle_area)) * 100.0)
        normalized_area_agu.append((float(agu_spec_area - agu_oracle_area) / float(agu_oracle_area)) * 100.0)

        MAX_SPEEDUP = max(MAX_SPEEDUP, normalized_speedup[-1])


x = np.arange(len(kernels))  # the label locations

y_speed = np.array(normalized_speedup)
y_area_agu = np.array(normalized_area_agu)
y_area_main = np.array(normalized_area_main)

plt.rcParams['font.family'] = 'serif'
plt.rcParams['font.size'] = 13
# plt.rcParams['font.serif'] = 'Liberation Serif'
# plt.style.use('grayscale')

fig, ax1 = plt.subplots(layout='constrained', figsize=(6, 3))
ax1.plot(x, y_area_main, 'o-', label="DU Area Overhead", color="black")
ax1.plot(x, y_area_agu, 'v-', label="AGU Area Overhead", color="black")
ax1.plot(x, y_speed, 'x-', label="Performance Overhead", color="black")
# ax2 = ax1.twinx()
# ax2.plot(x, y2, 'o-')
# ax2.set_ylabel('% Area Overhead')

ax1.set_ylim([-2, 25])

ax1.set_ylabel('% Overhead')
ax1.set_xlabel("Number of poison (blocks, calls)", labelpad=10)

ax1.legend(loc='upper left', ncols=1)

# y gridlines
ax1.grid(axis="y", color="grey", linestyle="-", linewidth=0.5, alpha=0.4)

ax1.set_xticks([i for i in range(len(kernels))])
ax1.set_xticklabels([
    f"({kernels['block_1'][0]}, {kernels['block_1'][1]})", 
    f"({kernels['block_2'][0]}, {kernels['block_2'][1]})", 
    f"({kernels['block_3'][0]}, {kernels['block_3'][1]})", 
    f"({kernels['block_4'][0]}, {kernels['block_4'][1]})", 
    f"({kernels['block_5'][0]}, {kernels['block_5'][1]})", 
    f"({kernels['block_6'][0]}, {kernels['block_6'][1]})", 
    f"({kernels['block_7'][0]}, {kernels['block_7'][1]})", 
    f"({kernels['block_8'][0]}, {kernels['block_8'][1]})", 
    ])

# plt.show()

PDF_NAME = f"{THIS_DIR}/PlotNumPoisonBlocks.pdf"
print(PDF_NAME)
plt.savefig(PDF_NAME, format="pdf", bbox_inches="tight")
