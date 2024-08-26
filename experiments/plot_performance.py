#!/usr/bin/env python3

import os
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator
import numpy as np
from statistics import harmonic_mean



THIS_DIR = os.path.dirname(os.path.realpath(__file__))
CSV_FILE = f"{THIS_DIR}/performance.csv"

kernels = [
    "bfs",
    "bc",
    "sssp",
    "hist",
    "thr",
    "mm",
    "fw",
    "sort",
    "spmv",
    "",
    "hmean",
]
normalized_speedup = {
    "INORD": [],
    "DAE": [],
    "SPEC_DAE": [],
    "ORACLE": []
}
abs_values = {k: [] for k in normalized_speedup.keys()}

MAX_SPEEDUP = 1

with open(CSV_FILE, 'r') as f:
    lines = f.readlines()
    for line in lines:
        if ',' not in line or 'kernel,' in line:
            continue # empty lines etc
        vals = line.split(',')

        # kernels.append(vals[0].split("_bram")[0])
        abs_values["INORD"].append(int(vals[1]))
        abs_values["DAE"].append(int(vals[2]))
        abs_values["SPEC_DAE"].append(int(vals[3]))
        abs_values["ORACLE"].append(int(vals[4]))

        normalized_speedup["INORD"].append(1)
        normalized_speedup["DAE"].append(abs_values["INORD"][-1] / abs_values["DAE"][-1])
        normalized_speedup["SPEC_DAE"].append(abs_values["INORD"][-1] / abs_values["SPEC_DAE"][-1])
        normalized_speedup["ORACLE"].append(abs_values["INORD"][-1] / abs_values["ORACLE"][-1])

        MAX_SPEEDUP = max(MAX_SPEEDUP, abs_values["INORD"][-1] / abs_values["DAE"][-1])
        MAX_SPEEDUP = max(MAX_SPEEDUP, abs_values["INORD"][-1] / abs_values["SPEC_DAE"][-1])
        MAX_SPEEDUP = max(MAX_SPEEDUP, abs_values["INORD"][-1] / abs_values["ORACLE"][-1])


hmean_DAE = harmonic_mean(normalized_speedup["DAE"])
hmean_SPEC_DAE = harmonic_mean(normalized_speedup["SPEC_DAE"])
hmean_ORACLE = harmonic_mean(normalized_speedup["ORACLE"])
print(f"hmean_SPEC_DAE = {hmean_SPEC_DAE}")
print(f"hmean_ORACLE = {hmean_ORACLE}")

# Add space between hmean and rest
normalized_speedup["INORD"].append(0)
normalized_speedup["DAE"].append(0)
normalized_speedup["SPEC_DAE"].append(0)
normalized_speedup["ORACLE"].append(0)

normalized_speedup["INORD"].append(1)
normalized_speedup["DAE"].append(hmean_DAE)
normalized_speedup["SPEC_DAE"].append(hmean_SPEC_DAE)
normalized_speedup["ORACLE"].append(hmean_ORACLE)


x = np.arange(len(kernels))  # the label locations
width = 0.2  # the width of the bars

plt.rcParams['font.family'] = 'serif'
plt.rcParams['font.size'] = 14
# plt.rcParams['font.serif'] = 'Liberation Serif'
plt.style.use('grayscale')

fig, ax = plt.subplots(layout='constrained', figsize=(8, 4))

ax.grid(axis="y", color="grey", linestyle="-", linewidth=0.5, alpha=0.4)
ax.yaxis.set_major_locator(MaxNLocator(integer=True))
gridlines = ax.yaxis.get_gridlines()[1].set_color("black")
ax.xaxis.set_ticks_position('none')

# hatches = ['', '....', 'XXXX', '////']
hatches = ['']*4    

i = 0
for attribute, measurement in normalized_speedup.items():
    h = hatches[i]
    offset = width * i
    rects = ax.bar(x + offset, measurement, width, label=attribute, hatch=h)
    i += 1


# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_ylabel('Normalized speedup')
# ax.set_title('')
ax.set_xticks(x + width, kernels)
plt.xticks(rotation=0)
ax.legend(loc='upper left', ncols=len(kernels))
ax.set_ylim(0, int(MAX_SPEEDUP) + 1)

# plt.show()
PDF_NAME = f"{THIS_DIR}/PlotPerformance.pdf"
print(PDF_NAME)
plt.savefig(PDF_NAME, format="pdf", bbox_inches="tight")
