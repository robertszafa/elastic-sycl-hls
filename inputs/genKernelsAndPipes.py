"""
This script perdorms a source-to-source transformation on a sycl source containing 1 kernel.
Given analysis info (see below), the script generates copies of the kernel, generates a call
to a kernel implementing memory disambiguation, and generates sycl pipes to connect all kernels 
together (sycl pipes are just types/class names).

This transformation could be implemented in clang to make it more robust (ASTTreeTransform, etc.). 

--Analysis:
Kernel: typeinfo name for MainKernel
Num Copies: 2
Num Loads: 1
Num Stores: 1
Array Line: 29
Array Column: 26
"""

import re
import sys
import json

STOREQ_HEADER = '#include "store_queue.hpp"'
# TODO: get queue name from llvm pass
Q_NAME = 'q'
# TODO: compute required piped depths
PIPE_DEPTH = 64
IDX_TAG_TYPE = 'pair'
# This has false positives but we use it only on strings that have a variable name at the beginning
c_var_regex = r'([a-zA-Z_][a-zA-Z0-9_]*)'


def gen_store_queue_syntax(array_name, array_type, num_loads, num_stores, 
                           forward_q=False, q_size=8, st_latency=8):
    return f'''
    //// gen_store_queue_syntax(array_name, num_loads)
    using val_type = {array_type};
    constexpr int kNumLoads = {num_loads};
    constexpr int kNumStores = {num_stores};
    constexpr bool isQueueForwarding = {'true' if forward_q else 'false'};
    constexpr int kQueueSize = {q_size};
    constexpr int kStoreLatency = {st_latency};

    using ld_idx_pipes = PipeArray<class ld_idx_pipes_class, {IDX_TAG_TYPE}, {PIPE_DEPTH}, kNumLoads>;
    using ld_val_pipes = PipeArray<class ld_val_pipes_class, val_type, {PIPE_DEPTH}, kNumLoads>;
    using st_idx_pipe = pipe<class st_idx_pipe_class, {IDX_TAG_TYPE}, {PIPE_DEPTH}>;
    using st_val_pipe = pipe<class st_val_pipe_class, val_type, {PIPE_DEPTH}>;
    using end_signal_pipe = pipe<class end_signal_pipe_class, bool>;
    
    auto eventStoreQueue = 
        StoreQueue<ld_idx_pipes, ld_val_pipes, kNumLoads, st_idx_pipe, st_val_pipe, end_signal_pipe,
                   isQueueForwarding, kQueueSize, kStoreLatency>({Q_NAME}, sycl::device_ptr<val_type>({array_name}));
    //// END gen_store_queue_syntax(array_name, num_loads)\n
    '''

def insert_storeq_wait(src, original_kernel_start):
    """Given the src lines with inserted store queue call, insert a eventStoreQueue.wait() before
    any device-> host data transfer"""
    insert_before_line = 0
    for i, line in enumerate(src):
        if i < original_kernel_start:
            continue
        if f'{Q_NAME}.memcpy' in line or f'{Q_NAME}.copy' in line:
            insert_before_line = i
            break
    
    return src[:insert_before_line] + [f'eventStoreQueue.wait();'] + src[insert_before_line:]

def add_idx_pipe_connections(kernel_body, num_loads, num_stores):
    kernel_body_lines = list(map(lambda x : x + '\n', kernel_body.split('\n')))

    ld_kernel_bodies = []
    for i in range(num_loads):
        new_kernel_body = kernel_body_lines[:1] + [f'ld_idx_pipes::PipeAt<{i}>::write({{0, 0}});\n'] + kernel_body_lines[1:] 
        ld_kernel_bodies.append(new_kernel_body)

    st_kernel_bodies = []
    for i in range(num_stores):
        new_kernel_body = kernel_body_lines[:1] + [f'st_idx_pipe::write({{0, 0}});\n'] + kernel_body_lines[1:] 
        st_kernel_bodies.append(new_kernel_body)
    
    return ld_kernel_bodies, st_kernel_bodies

def gen_val_pipe_connections(num_loads, num_stores):
    pipe_calls = []
    for i in range(num_loads):
        pipe_calls.append(f'auto ld_val_pipe_rd_{i} = ld_val_pipes::PipeAt<{i}>::read();')

    for i in range(num_stores):
        pipe_calls.append(f'st_val_pipe::write(val_type());')
        
    pipe_calls.append(f'end_signal_pipe::write(0);')
    
    return pipe_calls

def get_kernel_body(s):
    body = ""
    m = re.findall(r'<\s*' + kernel_name + r'\s*>\s*(\(.*?}\s*\)\s*;)', s, re.DOTALL)
    if m:
        body = m[0]
    return body

# Point before the first call to q.submit
def get_line_of_pattern(source_file_lines, pattern):
    """Line of last pattern ocurrence."""
    insert_line = -1
    for i, line in enumerate(source_file_lines):
        # TODO: Use regex to account for spaces etc.
        if pattern in line:
            insert_line = i

    return insert_line

def get_array_name(line_with_array, end_col):
    array = ""
    m = re.findall(c_var_regex, str(line_with_array[:end_col]))
    if m:
        array = m[0]

    return array

def parse_report(report_fname):
    with open(report_fname, 'r') as f:
        str = f.read()

    report = json.loads(str)
    report["kernel_class_name"] = report["kernel_class_name"].split(' ')[-1]
    report['spir_func_name'] = report["spir_func_name"].split('::')[0]

    return report['kernel_class_name'], report['spir_func_name'], report['num_copies'], report['num_loads'], \
           report['num_stores'], report['array_line'], report['array_column'], report['val_type']

if __name__ == '__main__':
    if len(sys.argv) < 3:
        sys.exit("loop-raw-report and src filename required")

    kernel_name, spir_func_name, num_copy, num_loads, num_stores, array_line, array_column, array_type = \
        parse_report(sys.argv[1])

    with open(sys.argv[2], 'r') as f:
        source_file = f.read()

    kernel_body = get_kernel_body(source_file)

    ld_kernel_bodies, st_kernel_bodies = add_idx_pipe_connections(kernel_body, num_loads, num_stores)
    main_kernel_pipes = gen_val_pipe_connections(num_loads, num_stores)

    kernel_names_decl = [f'class {kernel_name}_load_{i};' for i in range(num_loads)] + \
                        [f'class {kernel_name}_store_{i};' for i in range(num_stores)] 
    ld_idx_kernel_copies = [f'\n{Q_NAME}.single_task<{kernel_name}_load_{i}>{"".join(ld_kernel_bodies[i])}\n' \
                            for i in range(num_loads)]
    # TODO: handle multiple stores with a demultiplexer kernel.
    st_idx_kernel_copies = [f'\n{Q_NAME}.single_task<{kernel_name}_store_{i}>{"".join(st_kernel_bodies[i])}\n' \
                            for i in range(num_stores)]
    
    source_file_lines = source_file.splitlines()

    insert_line_idx_kernels = get_line_of_pattern(source_file_lines, f'{Q_NAME}.submit')
    array_name = get_array_name(source_file_lines[array_line-1], array_column)
    storeq_syntax = gen_store_queue_syntax(array_name, array_type, num_loads, num_stores)

    insert_line_main_kernel_pipes = get_line_of_pattern(source_file_lines, f'<{kernel_name}>')
    source_file_lines_with_pipes = source_file.splitlines()[:insert_line_main_kernel_pipes+1] + \
                                   main_kernel_pipes + source_file.splitlines()[insert_line_main_kernel_pipes+1:]

    src_lines = [STOREQ_HEADER] + \
                kernel_names_decl + \
                source_file_lines_with_pipes[:insert_line_idx_kernels] + \
                [storeq_syntax] + \
                ["\n//// Kernel copies"] + \
                ld_idx_kernel_copies + \
                st_idx_kernel_copies + \
                ["//// End Kernel copies\n"] + \
                source_file_lines_with_pipes[insert_line_idx_kernels:]
    
    src_lines_with_storeq = insert_storeq_wait(src_lines, insert_line_idx_kernels)
    
    new_filename = sys.argv[2].replace('.cpp', '.tmp.cpp')
    with open(new_filename, 'w') as f:
        f.write('\n'.join(src_lines_with_storeq))

    print(f'-- Refactored source file at {new_filename}')

