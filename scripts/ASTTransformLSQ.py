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

def gen_lsq_kernel_calls(report, q_name, q_size):
    result = []
    lsq_events = []

    for i_base, base_addr in enumerate(report['base_addresses']):
        result.append(f'''
        auto lsqEvent_{i_base} = 
            LoadStoreQueue<{llvm2ctype(base_addr['array_type'])}, pipes_ld_req_{i_base}, pipes_ld_val_{i_base}, 
                           pipe_st_req_{i_base}, pipe_st_val_{i_base}, pipe_end_lsq_signal_{i_base}, 
                           {base_addr['num_loads']}, {q_size}>({q_name});
        ''')
        lsq_events.append(f'lsqEvent_{i_base}')
    
    # The lsq wait calls might be superflous but add them to make sure.
    for e in lsq_events:
        result.append(f'{e}.wait();')

    return result

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

# Given various pipes, associate them with the instructions in report, and
# add this mapping to the report.
def add_pipe_names_to_report(report, ld_req_pipes, ld_val_pipes, st_req_pipes, st_val_pipes, lsq_signal_pipes):
    # Pipes that are part of a PipeArray have an additional field 'struct_id'.
    # We also have a 'repeat_id' field to differentiate between multiple calls
    # to the same pipe.
    for i_base, base_addr in enumerate(report['base_addresses']):
        base_addr["pipes_ld_req"] = [{'name': ld_req_pipes[i_base].class_name, 'struct_id': i, 'repeat_id': 0, 'instruction':
                                      base_addr['load_instructions'][i]} for i in range(ld_req_pipes[i_base].amount)]
        base_addr["pipes_ld_val"] = [{'name': ld_val_pipes[i_base].class_name, 'struct_id': i, 'repeat_id': 0, 'instruction':
                                      base_addr['load_instructions'][i]} for i in range(ld_val_pipes[i_base].amount)]

        # struct id -1 if not a pipe array.
        base_addr["pipes_st_req"] = [{'name': st_req_pipes[i_base].class_name, 'struct_id': -1, 'repeat_id': i, 'instruction':
                                      base_addr['store_instructions'][i]} for i in range(st_req_pipes[i_base].write_repeat)]
        base_addr["pipes_st_val"] = [{'name': st_val_pipes[i_base].class_name, 'struct_id': -1, 'repeat_id': i, 'instruction':
                                      base_addr['store_instructions'][i]} for i in range(st_val_pipes[i_base].write_repeat)]
        
        base_addr["pipe_end_lsq"] = {'name': lsq_signal_pipes[i_base].class_name, 'struct_id': -1, 'repeat_id': 0}


if __name__ == '__main__':
    if len(sys.argv) < 5:
        sys.exit("USAGE: ./prog LOOP_REPORT_FILE SRC_FILE NEW_SRC_FILE Q_SIZE")

    JSON_REPORT_FNAME = sys.argv[1]
    SRC_FNAME = sys.argv[2]
    NEW_SRC_FILENAME = sys.argv[3]
    Q_SIZE = sys.argv[4]

    with open(SRC_FNAME, 'r') as f:
        src_string = f.read()
    src_lines = src_string.splitlines()
    
    report = parse_report(JSON_REPORT_FNAME)
    # Keep track of kernel boundaries (and adjust their values if necessary).
    kernel_start_line = report["kernel_start_line"]
    kernel_end_line = report["kernel_end_line"]
    kernel_body = get_kernel_body(src_lines, kernel_start_line, kernel_end_line)

    Q_NAME = get_queue_name(src_lines[kernel_start_line-1])

    # Generate pipes given the report.
    # There is one pipe object per cluster in each list.
    # It is up to the pipe object to hold info on the actual amount of pipes
    # (this is done using a CYL PipeArray), and the amount of read/write ops.
    ld_req_pipes, st_req_pipes, ld_val_pipes, st_val_pipes, lsq_signal_pipes = gen_pipes(report)
    all_pipes = ld_req_pipes + st_req_pipes + ld_val_pipes + st_val_pipes + lsq_signal_pipes

    # Make it easier for the later LLVM transformation to find the required pipe
    # ops and the associated instruction.
    add_pipe_names_to_report(
        report, ld_req_pipes, ld_val_pipes, st_req_pipes, st_val_pipes, lsq_signal_pipes)

    # Insert pipe type declarations into the src file.
    pipe_declarations = [p.declaration() for p in all_pipes] 
    src_with_pipe_decl = insert_before_line(src_lines, kernel_start_line, pipe_declarations)
    kernel_start_line += len(pipe_declarations)
    kernel_end_line += len(pipe_declarations)

    # Insert call to LSQ kernels right after the original kernel call.
    lsq_calls = gen_lsq_kernel_calls(report, Q_NAME, Q_SIZE)
    src_with_pipe_decl_and_lsq_calls = insert_after_line(
        src_with_pipe_decl, kernel_end_line, lsq_calls)

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
    src_with_pipe_decl_lsq_calls_and_val_pipes = insert_after_line(
        src_with_pipe_decl_and_lsq_calls, kernel_start_line, val_pipe_ops)
    kernel_end_line += len(val_pipe_ops)

    # Create address generation kernel if decoupled flag is set.
    # Add lsq pipe ops to it. If flag not set, add them to orginal kernel.
    agu_kernels = []
    for i_base, base_addr in enumerate(report['base_addresses']):
        if base_addr['decouple_address']:
            base_addr["kernel_agu_name"] = f"{report['kernel_name']}_AGU_{i_base}"
            agu_kernel = gen_kernel_copy(Q_NAME, kernel_body, f"{report['kernel_name']}_AGU_{i_base}")
            agu_kernel_str = "\n".join(insert_after_line(agu_kernel, 1, lsq_pipe_ops[i_base]))
            agu_kernels.append(agu_kernel_str)
        else:  
            base_addr["kernel_agu_name"] = report['kernel_name']
            src_with_pipe_decl_lsq_calls_and_val_pipes = insert_after_line(
                src_with_pipe_decl_lsq_calls_and_val_pipes, kernel_start_line, lsq_pipe_ops[i_base])
            kernel_end_line += len(lsq_pipe_ops[i_base])

    # Combine the created AGU kernel with the original kernel (no-op if AGU is empty).
    combined_original_agu_pipes = insert_before_line(
        src_with_pipe_decl_lsq_calls_and_val_pipes, kernel_start_line, agu_kernels)
    kernel_start_line += len(agu_kernels)
    kernel_end_line += len(agu_kernels)

    # Combine all kernels together with the LSQ kernel.
    all_combined = get_lsq_src() + agu_kernel_declarations + combined_original_agu_pipes

    # The end result is a transformed NEW_SRC_FILENAME and 
    # the original json report with added pipe_name: instruction mappings.
    with open(NEW_SRC_FILENAME, 'w') as f:
        f.write('\n'.join(all_combined))
    with open(JSON_REPORT_FNAME, 'w') as f:
        json.dump(report, f, indent=4, sort_keys=True)
