#!/usr/bin/env python3

"""
This script performs a source-to-source transformation on a sycl source containing 1 kernel.
Given analysis info (see below), the script generates copies of the kernel, and sycl pipes 
to connect all kernels together (sycl pipes are just c++ types/class names).
"""

import sys

from ASTCommon import *

def gen_pipes_for_dependencies(bottleneck):
    """
    Given a json object (example below), generate pipe instances for the
    dependencies.

    The bottleneck json looks like this:
    {
      "dependencies_in": [
        "float",
        "i32"
      ],
      "dependencies_out": [
        "float"
      ]
    }
    """
    dep_in_pipes = []
    dep_out_pipes = []

    for i_d, dep in enumerate(bottleneck['dependencies_in']):
        data_type = llvm2ctype(dep["type"])
        p = SyclPipe(f'pipe_{data_type}_kernel{bottleneck["id"]}_in{i_d}', data_type)
        dep_in_pipes.append(p)
        dep["pipe"] = {"name": p.class_name, 'struct_id': -1, 'repeat_id': 0}

    for i_d, dep in enumerate(bottleneck['dependencies_out']):
        data_type = llvm2ctype(dep["type"])
        p = SyclPipe(f'pipe_{data_type}_kernel{bottleneck["id"]}_out{i_d}', data_type)
        dep_out_pipes.append(p)
        dep["pipe"] = {"name": p.class_name, 'struct_id': -1, 'repeat_id': 0}

    return dep_in_pipes, dep_out_pipes



if __name__ == '__main__':
    if len(sys.argv) < 4:
        sys.exit("USAGE: ./prog BOTTLENECK_REPORT_FILE SRC_FILE NEW_SRC_FILE")

    JSON_REPORT_FNAME = sys.argv[1]
    SRC_FNAME = sys.argv[2]
    NEW_SRC_FILENAME = sys.argv[3]

    with open(SRC_FNAME, 'r') as f:
        src_string = f.read()
    src_lines = src_string.splitlines()

    report = parse_report(JSON_REPORT_FNAME)
    kernel_start_line = report["kernel_start_line"]
    kernel_end_line = report["kernel_end_line"]
    kernel_body = get_kernel_body(src_lines, kernel_start_line, kernel_end_line)
    NUM_BOTTLENECKS = len(report["bottlenecks"])

    Q_NAME = get_queue_name(src_lines[kernel_start_line-1])

    dep_in_pipes = [] 
    dep_out_pipes = []
    for bottleneck in report["bottlenecks"]:
        in_pipes, out_pipes = gen_pipes_for_dependencies(bottleneck)
        dep_in_pipes.append(in_pipes)
        dep_out_pipes.append(out_pipes)

    # Insert pipe type declarations just before the main kernel.
    pipe_declarations = [p.declaration() for i_b in range(
        NUM_BOTTLENECKS) for p in dep_in_pipes[i_b] + dep_out_pipes[i_b]]
    src_with_pipe_declarations = insert_before_line(
        src_lines, kernel_start_line, pipe_declarations)
    kernel_start_line += len(pipe_declarations)
    kernel_end_line += len(pipe_declarations)
    
    # Generate pipe read/write calls for the all pipes.
    dep_in_pipe_reads = [[p.read_op() for p in this_deps] for this_deps in dep_in_pipes]
    dep_in_pipe_writes = [[p.write_op() for p in this_deps] for this_deps in dep_in_pipes]
    dep_out_pipe_reads = [[p.read_op() for p in this_deps] for this_deps in dep_out_pipes]
    dep_out_pipe_writes = [[p.write_op() for p in this_deps] for this_deps in dep_out_pipes]
    
    # Create a copy of the main kernel for each bottleneck.
    # Insert dep_in_pipe reads and dep_out_pipe write calls into the copy kernel.
    kernel_copies_class_declarations = []
    kernel_copies_with_pipes = []
    for i, bottleneck in enumerate(report["bottlenecks"]):
        kernel_copy_name = f"{report['kernel_name']}_copy_{bottleneck['id']}"
        bottleneck['kernel_copy_name'] = kernel_copy_name
        kernel_copy = gen_kernel_copy(Q_NAME, kernel_body, kernel_copy_name)
        kernel_copy_with_dep_pipes = insert_after_line(
            kernel_copy, 1, dep_in_pipe_reads[i] + dep_out_pipe_writes[i])
        kernel_copies_with_pipes.append("\n".join(kernel_copy_with_dep_pipes))
        kernel_copies_class_declarations.append(f"class {kernel_copy_name};")
    
    # Dually, for each bottleneck, insert dep_in_pipe writes and 
    # dep_out_pipe read calls into the original kernel.
    all_pipe_ops_in_main = [pipe_op for i in range(
        NUM_BOTTLENECKS) for pipe_op in dep_in_pipe_reads[i] + dep_out_pipe_writes[i]]
    src_with_pipe_declarations_and_pipe_ops = insert_after_line(
        src_with_pipe_declarations, kernel_start_line, all_pipe_ops_in_main)
    kernel_end_line += len(all_pipe_ops_in_main)

    # Combine kernel copy with the original kernel.
    all_combined = kernel_copies_class_declarations + insert_before_line(
        src_with_pipe_declarations_and_pipe_ops, kernel_start_line, kernel_copies_with_pipes)
    
    # The end result is a transformed NEW_SRC_FILENAME and 
    # the original json report with added pipe_name: instruction mappings.
    with open(NEW_SRC_FILENAME, 'w') as f:
        f.write('\n'.join(all_combined))
    with open(JSON_REPORT_FNAME, 'w') as f:
        json.dump(report, f, indent=2)

