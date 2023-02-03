#!/usr/bin/env python3

"""
This script perdorms a source-to-source transformation on a sycl source containing 1 kernel.
Given analysis info (see below), the script generates copies of the kernel, generates a call
to a kernel implementing memory disambiguation, and generates sycl pipes to connect all kernels 
together (sycl pipes are just types/class names).

This transformation could be implemented in clang to make it more robust (ASTTreeTransform, etc.). 
"""

import sys

from constants import GIT_DIR
from ASTCommon import *

LSQ_FILE = f'{GIT_DIR}/lsq/LoadStoreQueue.hpp'
LSQ_REQUEST_TYPE = 'request_lsq_t'
PIPE_DEPTH = 64

def get_lsq_src():
    try:
        with open(LSQ_FILE, 'r') as f:
            return f.read().splitlines()
    except Exception as e:
        print(e)
        exit("ERROR getting lsq src")

def gen_lsq_kernel_calls(report, q_size):
    result = []

    for i_base, base_addr in enumerate(report['base_addresses']):
        result.append(f'''
        using val_type_{i_base} = {llvm2ctype(base_addr['array_type'])};

        auto lsqEvent_{i_base} = 
            LoadStoreQueue<val_type_{i_base}, pipes_ld_req_{i_base}, pipes_ld_val_{i_base}, 
                           pipe_st_req_{i_base}, pipe_st_val_{i_base}, pipe_end_lsq_signal_{i_base}, 
                           {base_addr['num_loads']}, {q_size}>({Q_NAME});
        ''')    

    return result

def add_lsq_event_wait(src):
    """
    Given the src lines with inserted store queue call, insert 
    eventStoreQueue_{i_base}.wait() before any device-> host data transfer.
    """
    insert_before_line = 0
    for i, line in enumerate(src):
        if i < get_qsubmit_line(src):
            continue
        if f'{Q_NAME}.memcpy' in line or f'{Q_NAME}.copy' in line:
            insert_before_line = i
            break
    
    lsq_event_wait_calls = []
    for i_base in range(len(report['base_addresses'])):
        lsq_event_wait_calls.append(f'lsqEvent_{i_base}.wait();')
    
    return src[:insert_before_line] + lsq_event_wait_calls + src[insert_before_line:]

def gen_pipes(report):
    ld_address_pipes = []
    st_address_pipes = []
    ld_value_pipes = []
    st_value_pipes = []
    lsq_signal_pipes = []

    for i_base, base_addr in enumerate(report['base_addresses']):
        val_type = llvm2ctype(base_addr['array_type'])

        ld_address_pipe_array = SyclPipe(
            f'pipes_ld_req_{i_base}', LSQ_REQUEST_TYPE, base_addr['num_loads'], PIPE_DEPTH)
        ld_val_pipe_array = SyclPipe(
            f'pipes_ld_val_{i_base}', val_type, base_addr['num_loads'], PIPE_DEPTH)

        # multiple store requests and values are merged onto one pipe
        st_address_pipe = SyclPipe(
            f'pipe_st_req_{i_base}', LSQ_REQUEST_TYPE, depth=PIPE_DEPTH, write_repeat=base_addr['num_stores'])
        st_val_pipe = SyclPipe(
            f'pipe_st_val_{i_base}', val_type, depth=PIPE_DEPTH, write_repeat=base_addr['num_stores'])

        ld_address_pipes.append(ld_address_pipe_array)
        st_address_pipes.append(st_address_pipe)
        ld_value_pipes.append(ld_val_pipe_array)
        st_value_pipes.append(st_val_pipe)

        lsq_signal_pipes.append(SyclPipe(f'pipe_end_lsq_signal_{i_base}', 'int'))

    return ld_address_pipes, st_address_pipes, ld_value_pipes, st_value_pipes, lsq_signal_pipes


if __name__ == '__main__':
    if len(sys.argv) < 5:
        sys.exit("USAGE: ./prog LOOP_REPORT_FILE SRC_FILE NEW_SRC_FILE Q_SIZE")

    SRC_FNAME = sys.argv[2]
    NEW_SRC_FILENAME = sys.argv[3]
    Q_SIZE = sys.argv[4]

    with open(SRC_FNAME, 'r') as f:
        src_string = f.read()
    src_lines = src_string.splitlines()

    report = parse_report(sys.argv[1])

    # Generate pipes given the report.
    ld_req_pipes, st_req_pipes, ld_val_pipes, st_val_pipes, lsq_signal_pipes = gen_pipes(report)
    all_pipes = ld_req_pipes + st_req_pipes + ld_val_pipes + st_val_pipes + lsq_signal_pipes

    for i_base, base_addr in enumerate(report['base_addresses']):
        base_addr["kernel_agu_name"] = f"kernel_address_{i_base}" if report[
            'decouple_address'] else report['kernel_name']
        base_addr["pipes_ld_req"] = [{'name': p.name, 'struct_id': i, 'load_instruction': base_addr['load_instructions'][i]} for p in ld_req_pipes for i in range(p.amount) ]
        base_addr["pipes_ld_val"] = [{'name': p.name, 'struct_id': i} for p in ld_val_pipes for i in range(p.amount) ]
        base_addr["pipes_st_req"] = [{'name': p.name, 'struct_id': -1} for p in st_req_pipes]
        base_addr["pipes_st_val"] = [{'name': p.name, 'struct_id': -1} for p in st_val_pipes]
    
    print(report)

    # Insert pipe type declarations into the src file.
    src_with_pipe_decl = add_pipe_declarations(src_lines, all_pipes)

    # Insert call to LSQ kernels
    lsq_calls = gen_lsq_kernel_calls(report, Q_SIZE)
    src_with_pipe_decl_and_lsq_calls = insert_before_qsubmit(src_with_pipe_decl, lsq_calls)

    # Generate load value and store value pipes.
    val_pipe_ops = []
    for i_base in range(len(report['base_addresses'])):
        val_pipe_ops.append(ld_val_pipes[i_base].read_op())
        val_pipe_ops.append(st_val_pipes[i_base].write_op())

    # Generate LSQ reqeust pipes and end signal pipes.
    lsq_pipe_ops = []
    # The AGU kernel name is forward declared to avoid mangling.
    agu_kernel_declarations = []
    for i_base in range(len(report['base_addresses'])):
        this_base = []
        this_base.append(ld_req_pipes[i_base].write_op()) 
        this_base.append(st_req_pipes[i_base].write_op())
        this_base.append(lsq_signal_pipes[i_base].write_op())

        lsq_pipe_ops.append(this_base)
        agu_kernel_declarations.append(f"class {report['kernel_name']}_AGU_{i_base};")

    # Add ld value reads and st value writes to original kernel. 
    src_with_pipe_decl_lsq_calls_and_val_pipes = insert_after_qsubmit(src_with_pipe_decl_and_lsq_calls, val_pipe_ops)

    # Create address generation kernel if decoupled flag is set.
    # Add lsq pipe ops to it. If flag not set, add them to orginal kernel.
    agu_kernels = []
    if report['decouple_address']:  # TODO: decoupling decision per each base
        for i_base, base_addr in enumerate(report['base_addresses']):
            agu_kernel = gen_kernel_copy(src_lines, report['kernel_name'], f"{report['kernel_name']}_AGU_{i_base}")
            agu_kernel_str = "\n".join(insert_after_qsubmit(agu_kernel, lsq_pipe_ops[i_base]))
            agu_kernels.append(agu_kernel_str)
    else:  
        flat_lsq_pipe_ops = list(map(lambda l: "\n".join(l), lsq_pipe_ops))
        src_with_pipe_decl_lsq_calls_and_val_pipes = insert_after_qsubmit(
            src_with_pipe_decl_lsq_calls_and_val_pipes, flat_lsq_pipe_ops)

    # Combine the created AGU kernel with the original kernel (no-op if AGU is empty).
    combined_original_agu_pipes = insert_before_qsubmit(
        src_with_pipe_decl_lsq_calls_and_val_pipes, agu_kernels)

    # Add call to wait for LSQ kernel completion.
    combined_original_agu_pipes = add_lsq_event_wait(combined_original_agu_pipes)

    # Combine all kernels together with the LSQ kernel.
    all_combined = get_lsq_src() + agu_kernel_declarations + combined_original_agu_pipes

    with open(NEW_SRC_FILENAME, 'w') as f:
        f.write('\n'.join(all_combined))
    