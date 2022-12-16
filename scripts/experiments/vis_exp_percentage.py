import sys
import re
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

from constants import EXP_DATA_DIR

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
    plt.style.use(f'{EXP_DATA_DIR}/.plot_style.txt')
    fig = plt.figure(fig_id)
    plt.plot(x, y, label='Static Intel HLS', marker='o')
    plt.plot(x2, y2, label=f'This Work', marker='s')

    xticks = []
    for i, e in enumerate(PERCENTAGES_WAIT):
        if i % 2 == 0:
            xticks.append(e)

    plt.xticks(ticks=xticks, labels=xticks)

    plt.xlabel(r'True data hazard %')
    plt.ylabel(y_label)  # label the y axis
    plt.ylim(0)

    # add the legend (will default to 'best' location)
    plt.legend()
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



