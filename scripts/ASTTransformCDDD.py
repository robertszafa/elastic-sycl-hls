#!/usr/bin/env python3

"""
This script perdorms a source-to-source transformation on a sycl source containing 1 kernel.
Given analysis info (see below), the script generates copies of the kernel, and sycl pipes 
to connect all kernels together (sycl pipes are just types/class names).
"""

import re
import sys
import json

from ASTCommon import *


if __name__ == '__main__':
    if len(sys.argv) < 4:
        sys.exit("USAGE: ./prog BOTTLENECK_REPORT_FILE SRC_FILE NEW_SRC_FILE")

    SRC_FNAME = sys.argv[2]

    with open(SRC_FNAME, 'r') as f:
        source_file = f.read()
    source_file_lines = source_file.splitlines()

    report = parse_report(sys.argv[1])

    kernel_body = get_kernel_body(source_file, report['kernel_name'])
    