#!/usr/bin/env python3

import os
import matplotlib.pyplot as plt
import numpy as np



THIS_DIR = os.path.dirname(os.path.realpath(__file__))
CSV_FILE = f"{THIS_DIR}/misspeculationCost.csv"

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
normalized_area = []

MAX_SPEEDUP = 1

with open(CSV_FILE, 'r') as f:
    lines = f.readlines()
    i = 0
    for line in lines:
        if ',' not in line or 'kernel,' in line:
            continue # empty lines etc
        vals = line.split(',')

        normalized_speedup.append(int((1.0 - float(vals[2]) / float(vals[1])) * 100))
        normalized_area.append(2 + i * 10)
        i += 1

        MAX_SPEEDUP = max(MAX_SPEEDUP, normalized_speedup[-1])


x = np.arange(len(kernels))  # the label locations

y_speed = np.array(normalized_speedup)
y_area = np.array(normalized_area)

plt.rcParams['font.family'] = 'serif'
plt.rcParams['font.size'] = 13
# plt.rcParams['font.serif'] = 'Liberation Serif'
# plt.style.use('grayscale')

fig, ax1 = plt.subplots(layout='constrained', figsize=(6, 3))
ax1.plot(x, y_speed, 'x-', label="Performance", color="black")
ax1.plot(x, y_area, 'o-', label="Area", color="black")
# ax2 = ax1.twinx()
# ax2.plot(x, y2, 'o-')
# ax2.set_ylabel('% Area Overhead')

ax1.set_ylim([0, 100])

ax1.set_ylabel('% Overhead')
ax1.set_xlabel("Number of poison (blocks, calls)", labelpad=10)

ax1.legend(loc='upper left', ncols=2)

ax1.set_xticks([i for i in range(8)])
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
