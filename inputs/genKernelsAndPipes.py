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

import sys
import re


STOREQ_HEADER = '#include "store_queue.hpp"'
START_MARKER = '--Analysis:'
# TODO: get queue name from llvm pass
Q_NAME = 'q'
# TODO: compute required piped depths
PIPE_DEPTH = 64
IDX_TAG_TYPE = 'pair'
# This has false positives but we use it only on strings that have a variable name at the beginning
c_var_regex = r'([a-zA-Z_][a-zA-Z0-9_]*)'


def gen_store_queue_syntax(array_name, num_loads, num_stores, forward_q=False, q_size=8, st_latency=8):
    return f'''
    //// gen_store_queue_syntax(array_name, num_loads)
    using val_type = std::remove_reference<decltype( *{array_name} )>::type;
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

def get_paren_contents(s):
    results = set()
    for start in range(len(s)):
        string = s[start:]
        results.update(re.findall('\(.*?\)', string))

    return results

def get_kernel_body(s):
    body = ""
    m = re.findall(r'<\s*' + kernel_name + r'\s*>\s*(\(.*?}\s*\)\s*;)', s, re.DOTALL)
    if m:
        body = m[0]
    return body

# Point before the first call to q.submit
def get_insert_point_for_copies(source_file_lines):
    """Line of first kernel submission."""
    insert_line = -1
    for i, line in enumerate(source_file_lines):
        if f'{Q_NAME}.submit' in line:
            insert_line = i

    return insert_line

def get_array_name(line_with_array, end_col):
    array = ""
    m = re.findall(c_var_regex, str(line_with_array[:end_col]))
    if m:
        array = m[0]

    return array

def parse_analysis(fname):
    with open(fname) as f:
        str = f.read().split(START_MARKER)[-1]

    for line in str.splitlines():
        if 'Kernel:' in line:
            kernel_name = line.split(' ')[-1]
        if 'Num Copies:' in line:
            num_copy = int(line.split(' ')[-1])
        if 'Num Loads:' in line:
            num_loads = int(line.split(' ')[-1])
        if 'Num Stores:' in line:
            num_stores = int(line.split(' ')[-1])
        if 'Array Line:' in line:
            array_line = int(line.split(' ')[-1])
        if 'Array Column:' in line:
            array_column = int(line.split(' ')[-1])

    return kernel_name, num_copy, num_loads, num_stores, array_line, array_column


if __name__ == '__main__':
    if len(sys.argv) < 3:
        sys.exit("No input files")

    kernel_name, num_copy, num_loads, num_stores, array_line, array_column = parse_analysis(sys.argv[1])

    with open(sys.argv[2], 'r') as f:
        source_file = f.read()

    kernel_body = get_kernel_body(source_file)

    kernel_names_decl = [f'class {kernel_name}_{i};' for i in range(num_copy)]
    full_kernel_copies = [f'\n{Q_NAME}.single_task<{kernel_name}_{i}>{kernel_body}\n' for i in range(num_copy)]
    
    source_file_lines = source_file.splitlines()
    insert_line = get_insert_point_for_copies(source_file_lines)
    array_name = get_array_name(source_file_lines[array_line-1], array_column)
    storeq_syntax = gen_store_queue_syntax(array_name, num_loads, num_stores)

    new_source_file_lines = [STOREQ_HEADER] + \
                            kernel_names_decl + \
                            source_file_lines[:insert_line] + \
                            [storeq_syntax] + \
                            ["\n//// Kernel copies"] + \
                            full_kernel_copies + \
                            ["//// End Kernel copies\n"] + \
                            source_file_lines[insert_line:]
    
    new_source_file_lines_with_wait_call = insert_storeq_wait(new_source_file_lines, insert_line)
    
    new_filename = sys.argv[2].replace('.cpp', '.tmp.cpp')
    with open(new_filename, 'w') as f:
        f.write('\n'.join(new_source_file_lines_with_wait_call))

    print(f'-- Refactored source file at {new_filename}')

