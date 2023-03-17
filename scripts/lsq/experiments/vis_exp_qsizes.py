#!/usr/bin/env python3

import re
import json
import pandas as pd
import matplotlib.pyplot as plt

from run_qsizes_exp import KERNEL_ASIZE_FOR_BENCHMARKS, Q_SIZES, EXP_DATA_DIR, INPUTS_DIR

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
    plt.style.use(f'{EXP_DATA_DIR}/.plot_style.txt')
    fig = plt.figure(fig_id)

    annot_offset_x = 0.08
    annot_offset_y = 200

    x_time = df['time']
    plt.plot(x_time[0], df[resource][0], marker='o', label='Static Intel HLS')
    plt.text(x_time[0] + annot_offset_x, df[resource][0] + annot_offset_y, f'Freq. {df["FREQ"][0]} MHz')

    for i, q in enumerate(Q_SIZES):
        x = x_time[i+1]
        y = df[resource][i+1]

        label = None
        if i == 0:
            label = 'This Work'
        plt.plot(x, y, marker='s', color='#b3de69', label=label)
        plt.text(x + annot_offset_x, y + annot_offset_y, f'Queue Size {q}\nFreq. {df["FREQ"][i+1]} MHz')

    # xticks = [i for i in range(0, 6)]
    plt.xticks()

    plt.xlabel('Time - ms')
    plt.ylabel(f'Resource - {resource}')  # label the y axis
    # add the legend (will default to 'best' location)
    plt.legend()

    plt.savefig(f'{EXP_DATA_DIR}/qsizes_exp_{resource}_{kernel}.pdf')



if __name__ == '__main__':
    # Only hw makes sense since we gather resource info.
    target = 'hw'

    for kernel in KERNEL_ASIZE_FOR_BENCHMARKS.keys():
        time_res_file = f'{EXP_DATA_DIR}/qsizes_exp_{kernel}_{target}.csv'
        df = pd.read_csv(time_res_file)
        sta_res = get_resources_only_kernels(f'{INPUTS_DIR}/{kernel}/bin/{kernel}.fpga_{target}.prj/reports/resources/json/quartus.json')
        
        ALMs = [int(sta_res['ALM'])]
        FREQs = [sta_res['FREQ']]

        for qsize in Q_SIZES:
            BIN_DYNAMIC = f'{INPUTS_DIR}/{kernel}/bin/{kernel}.lsq_{qsize}.fpga_{target}' 
            dyn_res = get_resources_only_kernels(f'{BIN_DYNAMIC}.prj/reports/resources/json/quartus.json')

            ALMs.append(int(dyn_res['ALM']))
            FREQs.append(dyn_res['FREQ'])

        df['ALM'] = ALMs
        df['FREQ'] = FREQs

        make_plot(df, kernel, 'ALM')
