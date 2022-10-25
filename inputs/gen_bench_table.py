import re
import csv
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path

# Keep parameters synced
from run_exp import (EXP_DATA_DIR, Q_SIZES, KERNEL_ASIZE_PAIRS, PERCENTAGES_WAIT)


KERNELS = list(KERNEL_ASIZE_PAIRS.keys())
Q_SIZE = 8



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
            res['Freq'] = freq_str[0]
    except Exception as e:
        print(e)
        exit()
    
    return res


def get_min_max_runtime(kernel, approach_column):
    filename = f'{EXP_DATA_DIR}/{kernel}_hw.csv'
    min = np.Inf
    max = -np.Inf
    try:
        df = pd.read_csv(filename)
        i = 0
        while True:
            if not str(df.loc[i]['percentage']).isnumeric():
                return min, max

            cell = df.loc[i][approach_column]
            if cell < min:
                min = cell
            if cell > max:
                max = cell
            i += 1

    except Exception as e:
        print(e)
        return min, max


if __name__ == '__main__':
    bin_ext = 'fpga_hw'

    TABLE_FNAME = f'{EXP_DATA_DIR}/benchmark_table.csv'
    Path(EXP_DATA_DIR).mkdir(parents=True, exist_ok=True)

    with open(TABLE_FNAME, 'w') as f:
        writer = csv.writer(f)
        writer.writerow(['Benchmark', 'ALM - k', '', '', 'REG - k', '', '', 'RAM', '', '', 'DSP', '', '', 'Frequency - MHz', '', '', 'Wall-time - ms', '', ''])
        writer.writerow(['', 'Base', 'Ours', 'X', 'Base', 'Ours', 'X', 'Base', 'Ours', 'X', 'Base', 'Ours', 'X', 'Base', 'Ours', 'X', 'Base', 'Ours', 'X'])

        for kernel in KERNELS:
            kernel_row = [kernel]

            resources_static = get_resources(f'{kernel}/bin/{kernel}.{bin_ext}.prj/acl_quartus_report.txt')
            resources_dynamic = get_resources(f'{kernel}/{kernel}.cpp.tmp.cpp_{Q_SIZE}qsize.{bin_ext}.fpga.prj/acl_quartus_report.txt')

            min_static, max_static = get_min_max_runtime(kernel, 'static')
            min_dynamic, max_dynamic = get_min_max_runtime(kernel, f'dynamic_{Q_SIZE}qsize')
            min_speedup = max_static/min_dynamic
            max_speedup = max_static/max_dynamic
            
            for i in range(len(resources_static)):
                res_static = list(resources_static.values())[i]
                res_dynamic = list(resources_dynamic.values())[i]

                any_zero = res_static == 0 or res_dynamic == 0
                res_norm = round(int(res_dynamic)/int(res_static), 2) if not any_zero else '-'
                
                # Report LEs and REGs in thousands
                if i <= 2:
                    res_static = round(res_static/1000, 2)
                    res_dynamic = round(res_dynamic/1000, 2)

                kernel_row.append(res_static)
                kernel_row.append(res_dynamic)
                kernel_row.append(res_norm)

            kernel_row.append(f'{round(max_static, 2)}')
            kernel_row.append(f'{round(min_dynamic, 2)}-{round(max_dynamic, 2)}')
            kernel_row.append(f'{round(min_speedup, 2)}-{round(max_speedup, 2)}')
            
            writer.writerow(kernel_row)
