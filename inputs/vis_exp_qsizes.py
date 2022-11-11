from cgitb import text
import sys
import re
import json
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

from run_qsizes_exp import KERNEL_ASIZE_PAIRS, Q_SIZES, EXP_DATA_DIR, PERCENTAGE_WAIT


Q_SIZE = 8

plt.rcParams['font.size'] = 14
# colors = seaborn.color_palette("rocket", 3)
colors = ['#c3121e', '#0348a1', '#ffb01c', '#027608',
          '#0193b0', '#9c5300', '#949c01', '#7104b5']
# plt.figure(unique_id) ensures that different invocations of make_plot() don't interfere
fig_id = 0


# return ALUTs, REGs, RAMs, DSPs, Fmax
def get_resources(acl_quartus_report):
    res = {'ALM' : 0, 'REG' : 0, 'RAM' : 0, 'DSP' : 0, 'Freq' : 0}
    try:
        with open(acl_quartus_report, 'r') as f:
            report_str = f.read()

            logic_usage_str = re.findall("Logic utilization: (.*?)/", report_str)
            reg_usage_str = re.findall("Registers: ([,\d]+)", report_str)
            ram_usage_str = re.findall("RAM blocks: (.*?)/", report_str)
            dsp_usage_str = re.findall("DSP blocks: (.*?)/", report_str)
            freq_str = re.findall("Actual clock freq: (\d+)", report_str)

            res['ALM'] = int(re.sub("[^0-9]", "", logic_usage_str[0]))
            res['REG'] = int(re.sub("[^0-9]", "", reg_usage_str[0]))
            res['RAM'] = int(re.sub("[^0-9]", "", ram_usage_str[0]))
            res['DSP'] = int(re.sub("[^0-9]", "", dsp_usage_str[0]))
            res['FREQ'] = freq_str[0]
    except Exception as e:
        print(e)
        exit()
    
    return res

def get_resources_only_kernels(quartus_data_file):
    res = {'ALM' : 0, 'RAM' : 0, 'DSP' : 0, 'Freq' : 0}
    try:
        with open(quartus_data_file, 'r') as f:
            report_str = f.read()
            report_str = report_str[16:-2]
            data = json.loads(report_str)

            res['FREQ'] = int(float(data['quartusFitClockSummary']['nodes'][0]['kernel clock']))
            for node in data['quartusFitResourceUsageSummary']['nodes']:
                # if node['type'] == 'kernel':
                if node['type'] == 'system':
                    res['ALM'] = int(float(node['alm']))
                    # res['REG'] += int(float(node['reg']))
                    res['RAM'] = int(float(node['ram']))
                    res['DSP'] = int(float(node['dsp']))
                    break
    except Exception as e:
        print('\n*Exception*\n')
        print(e)
        exit()
    
    return res



def make_plot(df, kernel, resource):
    global fig_id

    fig_id = fig_id + 1
    # plot
    fig = plt.figure(fig_id, figsize=(8, 6))
    FONTSIZE = 18

    annot_offset_x = 0.1
    annot_offset_y = 200

    x_time = df['time']
    plt.plot(x_time[0], df[resource][0], marker='o', label='Static Intel HLS', color=colors[0], mfc='w', markersize=8)
    plt.text(x_time[0] + 0.05, df[resource][0] + annot_offset_y, f'Freq. {df["FREQ"][0]} MHz', fontsize=10)

    for i, q in enumerate(Q_SIZES):
        x = x_time[i+1]
        y = df[resource][i+1]

        if i == 0:
          plt.plot(x, y, marker='s', fillstyle='full', label=f'This Work', color=colors[1], mfc='w', markersize=8)
        else:
          plt.plot(x, y, marker='s', fillstyle='full', color=colors[1], mfc='w', markersize=8)
        
        plt.text(x + annot_offset_x, y + annot_offset_y, f'Queue Size {q}\nFreq. {df["FREQ"][i+1]} MHz', fontsize=10)

    xticks = [i for i in range(0, 6)]
    plt.xticks(ticks=xticks, labels=xticks)
    plt.ylim(min(df[resource]) - 500, max(df[resource]) + 1000)

    plt.xlabel('Time - ms', fontsize=FONTSIZE)
    plt.ylabel(f'Resource - {resource}', fontsize=FONTSIZE)  # label the y axis

    # add the legend (will default to 'best' location)
    plt.legend(fontsize=FONTSIZE)
    

    plt.savefig(f'{EXP_DATA_DIR}/qsizes_exp_{resource}_{kernel}.pdf')



if __name__ == '__main__':
    bin_type = 'hw'
    bin_ext = 'fpga_' + bin_type

    for kernel in KERNEL_ASIZE_PAIRS.keys():
        time_res_file = f'{EXP_DATA_DIR}/qsizes_exp_{kernel}_{bin_type}.csv'
        df = pd.read_csv(time_res_file)
        sta_res = get_resources_only_kernels(f'{kernel}/bin/{kernel}.fpga_hw.prj/reports/resources/quartus_data.js')
        
        ALMs = [int(sta_res['ALM'])]
        FREQs = [sta_res['FREQ']]

        for qsize in Q_SIZES:
            BIN_DYNAMIC = f'{kernel}/{kernel}.cpp_{qsize}qsize.tmp.cpp.{bin_ext}' 
            if qsize == 8:
                BIN_DYNAMIC = f'{kernel}/{kernel}.cpp.tmp.cpp_{qsize}qsize.{bin_ext}' 
            
            dyn_res = get_resources_only_kernels(f'{BIN_DYNAMIC}.fpga.prj/reports/resources/quartus_data.js')

            ALMs.append(int(dyn_res['ALM']))
            FREQs.append(dyn_res['FREQ'])

        df['ALM'] = ALMs
        df['FREQ'] = FREQs

        make_plot(df, kernel, 'ALM')
