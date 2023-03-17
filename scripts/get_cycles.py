#!/usr/bin/python3

import sys
import re

PERIOD = 1000

def get_cycles(sim_res_fname, kernel=None):
    max_cycles = 0
    try:
        with open(sim_res_fname, 'r') as f:
            sim_res = f.readlines()
            
            look_next_line = True if kernel is None else False
            for line in sim_res:
                # If kernel is supplied then only record events for that kernel.
                if kernel is not None and not look_next_line:
                    look_next_line = f'"{kernel}.' in line

                m_start = re.search(r'"start":(\d+),', line)
                m_end = re.search(r'"end":(\d+),', line)
                if look_next_line and m_start and m_end:
                    start_cycle = int(m_start.group(1))
                    end_cycle = int(m_end.group(1))

                    cycles = int((end_cycle - start_cycle) / PERIOD)
                    max_cycles = max(cycles, max_cycles)

                    look_next_line = False if kernel is not None else True

    except Exception as e:
        print('Exception: ', e)
        exit(1)
    
    return max_cycles


if __name__ == '__main__':
    fname = sys.argv[1]
    kernel = sys.argv[2] if len(sys.argv) > 2 else None

    cycles = get_cycles(fname, kernel)
    
    print(cycles)
