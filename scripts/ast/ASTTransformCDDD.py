#!/usr/bin/env python3

"""
This script performs a source-to-source transformation on a sycl source containing 1 kernel.
Given analysis info (see below), the script generates copies of the kernel, and sycl pipes 
to connect all kernels together (sycl pipes are just c++ types/class names).
"""

import sys

from ASTCommon import *

def gen_pipes_for_dependencies(block_info):
    """
    Given a json object (example below), return pipe instances for the
    dependencies. Also add a pipe for the predicate that will govern the 
    execution of the kernel with this block.

    The json looks like this:
    "incoming_uses": [
        {
          "instruction": {...},
          "type": "float",
        },      
    ]

    Also add the names of the generated pipes to the dependencies.
    """
    dep_in_pipes = []
    dep_out_pipes = []
    pred_pipes = []

    # There will be 2 writes per pred_pipe in the main kernel. One triggering 
    # the execution of the servant kernel, and one for terminating it.
    p = SyclPipe(f'pipe_pred_kernel{block_info["id"]}', 'bool') #, write_repeat=2)
    pred_pipes.append(p)
    block_info['pred_pipe'] = {'name': p.class_name, 'type': p.ctype}

    for i_d, dep in enumerate(block_info['incoming_uses']):
        data_type = llvm2ctype(dep["type"])
        p = SyclPipe(f'pipe_{data_type}_kernel{block_info["id"]}_in{i_d}', data_type)
        dep_in_pipes.append(p)
        dep["name"] = p.class_name

    for i_d, dep in enumerate(block_info['outgoing_defs']):
        data_type = llvm2ctype(dep["type"])
        p = SyclPipe(f'pipe_{data_type}_kernel{block_info["id"]}_out{i_d}', data_type)
        dep_out_pipes.append(p)
        dep["name"] = p.class_name

    return dep_in_pipes, dep_out_pipes, pred_pipes

def add_ivdep_pragma(src_lines, start_line, end_line):
    """
    If a loop writes 2 times to the same pipe, the II might suffer unnecessarily. 
    Add pragmas to ignore dep between writes to the same pipe since the ordering 
    is enforced by the dependency on tag anyway.
    """

    # TODO: We should only ivdep for captured base addresses and generated pipes
    IGNORE_DEP_PRAGMA = '[[intel::ivdep]]'
    res = []
    for i_line, line in enumerate(src_lines, 1):
        if i_line >= start_line and i_line < end_line:
            for loop_keyword in [' for ', ' while ', ' do ']:
                if loop_keyword in line:
                    idx = line.index(loop_keyword)
                    line = line[:idx] + IGNORE_DEP_PRAGMA + line[idx:]
                    break
                
        res.append(line)

    return res


if __name__ == '__main__':
    if len(sys.argv) < 4:
        sys.exit("USAGE: ./prog CDDD_REPORT_FILE SRC_FILE NEW_SRC_FILE")

    JSON_REPORT_FNAME = sys.argv[1]
    SRC_FNAME = sys.argv[2]
    NEW_SRC_FILENAME = sys.argv[3]

    with open(SRC_FNAME, 'r') as f:
        src_string = f.read()
    src_lines = src_string.splitlines()

    report = parse_report(JSON_REPORT_FNAME)
    kernel_start_line = report["kernel_start_line"]
    kernel_end_line = report["kernel_end_line"]

    src_lines = add_ivdep_pragma(src_lines, kernel_start_line, kernel_end_line)

    kernel_body = get_kernel_body(src_lines, kernel_start_line, kernel_end_line)
    NUM_BLOCKS = len(report["blocks_to_decouple"])

    Q_NAME = get_queue_name(src_lines[kernel_start_line-1])

    # Pipes for incoming uses into a BB
    dep_in_pipes = [] 
    # Pipes for outgoing definitions out of a BB
    dep_out_pipes = []
    pred_pipes = []
    for block_info in report["blocks_to_decouple"]:
        in_pipes, out_pipes, this_pred_pipe = gen_pipes_for_dependencies(block_info)
        dep_in_pipes.append(in_pipes)
        dep_out_pipes.append(out_pipes)
        pred_pipes.append(this_pred_pipe)

    # Insert pipe type declarations just before the main kernel.
    pipe_declarations = [p.declaration() for i_b in range(
        NUM_BLOCKS) for p in dep_in_pipes[i_b] + dep_out_pipes[i_b] + pred_pipes[i_b]]
    src_with_pipe_declarations = insert_before_line(
        src_lines, kernel_start_line, pipe_declarations)
    kernel_start_line += len(pipe_declarations)
    kernel_end_line += len(pipe_declarations)
    
    # Generate pipe read/write calls for the all pipes.
    dep_in_pipe_reads = [[p.read_op() for p in this_deps] for this_deps in dep_in_pipes]
    dep_in_pipe_writes = [[p.write_op() for p in this_deps] for this_deps in dep_in_pipes]
    dep_out_pipe_reads = [[p.read_op() for p in this_deps] for this_deps in dep_out_pipes]
    dep_out_pipe_writes = [[p.write_op() for p in this_deps] for this_deps in dep_out_pipes]
    pred_pipe_reads = [[p.read_op() for p in this_pred] for this_pred in pred_pipes]
    pred_pipe_writes = [[p.write_op() for p in this_pred] for this_pred in pred_pipes]
    
    # Create a copy of the main kernel for each block.
    # Insert dep_in_pipe reads and dep_out_pipe write calls into the copy kernel.
    kernel_copies_class_declarations = []
    kernel_copies_with_pipes = []
    for i, block_info in enumerate(report["blocks_to_decouple"]):
        kernel_copy_name = f"{report['kernel_name']}_copy_{block_info['id']}"
        block_info['kernel_copy_name'] = kernel_copy_name
        kernel_copy = gen_kernel_copy(Q_NAME, kernel_body, kernel_copy_name)
        kernel_copy_with_dep_pipes = insert_after_line(
            kernel_copy, 1, dep_in_pipe_reads[i] + dep_out_pipe_writes[i] + pred_pipe_reads[i])
        kernel_copies_with_pipes.append("\n".join(kernel_copy_with_dep_pipes))
        kernel_copies_class_declarations.append(f"class {kernel_copy_name};")
    
    # Dually, for each block, insert dep_in_pipe writes and dep_out_pipe read calls into the original kernel.
    all_pipe_ops_in_main = [pipe_op for i in range(
        NUM_BLOCKS) for pipe_op in dep_in_pipe_writes[i] + dep_out_pipe_reads[i] + pred_pipe_writes[i]]
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

