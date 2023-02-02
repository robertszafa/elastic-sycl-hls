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


def gen_pipes_for_dependencies(report):
    """
    Given a json report (example below), generate pipe instances for the
    dependencies.

    Example of report:
    {"bottlenecks": [
      {
        "dependencies_in": [
          "float",
          "i32"
        ],
        "dependencies_out": [
          "float"
        ]
      }
    ]}
    """
    dep_in_pipes = []
    dep_out_pipes = []

    for i_b, bottleneck in enumerate(report['bottlenecks']):
        for i_d, data_type_llvm in enumerate(bottleneck['dependencies_in']):
            data_type = llvm2ctype(data_type_llvm)
            name = f'pipe_{data_type}_kernel{i_b}_in{i_d}'
            dep_in_pipes.append(SyclPipe(name, data_type))

        for i_d, data_type_llvm in enumerate(bottleneck['dependencies_out']):
            data_type = llvm2ctype(data_type_llvm)
            name = f'pipe_{data_type}_kernel{i_b}_out{i_d}'
            dep_out_pipes.append(SyclPipe(name, data_type))

    return dep_in_pipes, dep_out_pipes



if __name__ == '__main__':
    if len(sys.argv) < 4:
        sys.exit("USAGE: ./prog BOTTLENECK_REPORT_FILE SRC_FILE NEW_SRC_FILE")

    SRC_FNAME = sys.argv[2]
    NEW_SRC_FILENAME = sys.argv[3]

    with open(SRC_FNAME, 'r') as f:
        src_string = f.read()

    src_lines = src_string.splitlines()
    # q_submit_line = get_line_of_pattern(src_lines, Q_NAME + r'\.submit')

    report = parse_report(sys.argv[1])

    dep_in_pipes, dep_out_pipes = gen_pipes_for_dependencies(report)

    # Insert pipe type declarations.
    src_with_pipe_declarations = add_pipe_declarations(src_lines, dep_in_pipes + dep_out_pipes)
    
    # Get a copy of the kernel with a changed kernel name.
    kernel_copy = gen_kernel_copy(src_lines, report['kernel_name'], f"class {report['kernel_name']}_copy_0")
    
    # Insert pipe read and write calls into the copy kernel.
    kernel_copy_with_pipe_ops = add_pipe_ops(kernel_copy, dep_in_pipes, dep_out_pipes)

    # Dually, insert pipe read and write calls into the original kernel.
    src_with_pipe_declarations_and_pipe_ops = add_pipe_ops(
        src_with_pipe_declarations, dep_out_pipes, dep_in_pipes)

    # Combine kernel copy with the original kernel.
    all_combined = insert_before_line(src_with_pipe_declarations_and_pipe_ops, get_qsubmit_line(
        src_with_pipe_declarations_and_pipe_ops), kernel_copy_with_pipe_ops)

    with open(NEW_SRC_FILENAME, 'w') as f:
        f.write('\n'.join(all_combined))
