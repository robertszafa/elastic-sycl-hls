import sys
import re
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

EXP_DATA_DIR = 'exp_data/'

KERNEL_ASIZE_PAIRS = {
    'histogram' : 10000,
    'spmv' : 100,
}
PERCENTAGES_WAIT = [0, 10, 20 , 30, 40, 50, 60, 70, 80, 90, 100]

Q_SIZE = 8

plt.rcParams['font.size'] = 14
# colors = seaborn.color_palette("rocket", 3)
colors = ['#c3121e', '#0348a1', '#ffb01c', '#027608',
          '#0193b0', '#9c5300', '#949c01', '#7104b5']
# plt.figure(unique_id) ensures that different invocations of make_plot() don't interfere
fig_id = 0


def get_freq(bin):
    hw_prj = bin.replace('_sim', '') + '.prj'
    try:
        with open(f'{hw_prj}/acl_quartus_report.txt', 'r') as f:
            contents = f.read()
            m = re.search(r'Kernel fmax: (\d+)', contents)
            if m:
                return int(m.group(1))

            return 0
    except:
        return 0

def get_ii(bin):
    hw_prj = bin.replace('_sim', '') + '.prj'
    try:
        with open(f'{hw_prj}/reports/lib/json/loops.json', 'r') as f:
            contents = f.read()
            m = re.findall(r'\["Yes", "~?(\d+)"', contents)
            max = 1
            for s in m:
                max = max if max > int(s) else int(s)
            
            return max
    except:
        print(bin, ' no ii')
        return 1

def make_plot(filename, relative=True, y_label='Speedup (normalised)', title=''):
    global fig_id
    global BINS_STATIC
    global BINS_DYNAMIC

    fig_id = fig_id + 1

    df = pd.read_csv(filename)
    x = df['q_size']
    y = df['static'].replace({0: np.nan})

    x2 = df['q_size']
    y2 = df['dynamic'].replace({0: np.nan})

    if relative:
        static_baseline = y[0]
        y = [1 for _ in df['static']]
        y2 = [static_baseline/val for val in y2]

    # plot
    fig = plt.figure(fig_id, figsize=(8, 8))
    plt.semilogx(x2, y2, linestyle='-', marker='s', label='dynamic (forwarding)',
                 color=colors[1], mfc='w', markersize=8)
    plt.semilogx(x, y, linestyle='-', marker='o', label='static',
                 color=colors[0], mfc='w', markersize=8)

    # Add frequencies as
    if not 'sim' in filename:
        freq = get_freq(BINS_STATIC[0])
        ii = get_ii(BINS_STATIC[0])
        plt.text(x[len(x) - 1], y[len(y) - 1], f'{freq} MHz\nII={ii}', fontsize='x-small', fontstyle='italic')

        for i in range(len(BINS_DYNAMIC)):
            freq = get_freq(BINS_DYNAMIC[i])
            ii = get_ii(BINS_DYNAMIC[i])
            if not np.isnan(y2[i]):
                plt.text(x2[i], y2[i], f'{freq} MHz\nII={ii}', fontsize='x-small', fontstyle='italic')


    xticks = Q_SIZES_DYNAMIC
    plt.xticks(ticks=xticks, labels=xticks)

    plt.xlabel(r'Queue size', fontsize=14)
    plt.ylabel(y_label, fontsize=14)  # label the y axis

    # add the legend (will default to 'best' location)
    plt.legend(fontsize=14)
    plt.title(title)

    plt.savefig(filename.replace('csv', 'png'), dpi=300, bbox_inches="tight")


def make_plot_all_percentages(filename, kernel, relative=False, y_label='Speedup (normalised)', title=''):
    global fig_id
    global BINS_STATIC
    global BINS_DYNAMIC

    fig_id = fig_id + 1

    df = pd.read_csv(filename)
    # Column names in csv
    x = df['percentage']
    y = df['static'].replace({0: np.nan})
    x2 = df['percentage']
    y2 = df[f'dynamic_{Q_SIZE}qsize'].replace({0: np.nan})

    if relative:
        y2 = [y[k]/val for k, val in enumerate(y2)]
        y = [1 for _ in df['static']]

    # plot
    fig = plt.figure(fig_id, figsize=(8, 6))
    plt.plot(x, y, linestyle='-', marker='o', label='Static Intel HLS',
                 color=colors[0], mfc='w', markersize=8)
    plt.plot(x2, y2, linestyle='-', marker='s', 
                 label=f'This Work',
                 color=colors[1], mfc='w', markersize=8)

    xticks = PERCENTAGES_WAIT
    plt.xticks(ticks=xticks, labels=xticks)

    FONTSIZE = 18

    plt.xlabel(r'True data hazard %', fontsize=FONTSIZE)
    plt.ylabel(y_label, fontsize=FONTSIZE)  # label the y axis

    # add the legend (will default to 'best' location)
    
    plt.legend(bbox_to_anchor=(1, 0.9), loc="upper right", fontsize=FONTSIZE)
    plt.title(title)

    plt.savefig(filename.replace('csv', 'pdf'))



if __name__ == '__main__':
    if sys.argv[1] not in ['sim', 'hw']:
        exit("ERROR: No extension provided\nUSAGE: ./vis_exp.py [sim, hw]\n")

    bin_type = sys.argv[1]
    bin_ext = 'fpga_' + bin_type
    is_sim = 'sim' == bin_type

    Y_LABEL = 'Cycles' if is_sim else 'Time - ms'

    for kernel in KERNEL_ASIZE_PAIRS.keys():
        res_file = f'{EXP_DATA_DIR}/dep_percent_exp_{kernel}_{bin_type}.csv'

        make_plot_all_percentages(res_file, kernel, y_label=Y_LABEL, title='')



