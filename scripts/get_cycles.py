#!/usr/bin/python3


"""
Run a simulation and report cycle counts for each kernel.
"""

import sys
import re

PERIOD = 1000

def get_cycles(sim_project, kernel=None):
    max_cycles = 0
    min_cycles = sys.maxsize
    try:
        sim_raw_fname = f'{sim_project}/reports/resources/json/simulation_raw.json'
        with open(sim_raw_fname, 'r') as f:
            sim_res = f.readlines()
            
            look_next_line = True if kernel is None else False
            last_line = ''
            for line in sim_res:
                # If kernel is supplied then only record events for that kernel.
                if kernel is not None and not look_next_line:
                    look_next_line = f'"{kernel}.' in line

                m_start = re.search(r'"start":(\d+),', line)
                m_end = re.search(r'"end":(\d+)', line)
                if look_next_line and m_start and m_end:
                    start_cycle = int(m_start.group(1))
                    end_cycle = int(m_end.group(1))

                    cycles = int((end_cycle - start_cycle) / PERIOD)
                    kernel_name = last_line.split('"')[1]
                    if 'start' not in kernel_name:
                        print(f'{cycles} -- {kernel_name}')
                    max_cycles = max(cycles, max_cycles)
                    min_cycles = min(cycles, min_cycles)

                    look_next_line = False if kernel is not None else True

                last_line = line

    except Exception as e:
        print(f'Error: did not get cycle count because:\n\t{e}')
        exit()
    
    return min_cycles, max_cycles


if __name__ == '__main__':
    sim_project = sys.argv[1]
    kernel = sys.argv[2] if len(sys.argv) > 2 else None

    print('Cycles')
    min_cycles, max_cycles = get_cycles(sim_project, kernel)
    
    # print('------')
    # print(f'{min_cycles} - {max_cycles}')
    # print('------')
