#!/usr/bin/env python3

"""
This script reads a dpcpp FPGA HLS report and C++ source line numbers with
operations that cause a data dependency bottlenck.
I.e. an operation that increases the II of a loop.
"""

import sys
import json


def get_json_report(report_fname):
    try:
        with open(report_fname, 'r') as f:
            str = f.read()

        return json.loads(str)
    except Exception as e:
        exit("Error parsing bottlenck report " + report_fname)


if __name__ == '__main__':
    if len(sys.argv) < 2:
        sys.exit("USAGE: ./prog bottleneck.json")

    bottlenecks = get_json_report(sys.argv[1])['bottlenecks']

    # TODO: Nodes or details['loop feedback that lowered II]?
    # TODO: Only take 'data dependencies'?
    for bottleneck in bottlenecks:
        # TODO: Only get last node or more?
        print(bottleneck['nodes'][-1]['debug'][0][0]['line'])
