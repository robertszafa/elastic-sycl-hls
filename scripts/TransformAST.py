#!/usr/bin/env python3

"""
This script perdorms a source-to-source transformation on a sycl source containing 1 kernel.
Given analysis info (see below), the script generates copies of the kernel, generates a call
to a kernel implementing memory disambiguation, and generates sycl pipes to connect all kernels 
together (sycl pipes are just types/class names).

This transformation could be implemented in clang to make it more robust (ASTTreeTransform, etc.). 
"""

import re
import sys
import json

from constants import GIT_DIR

# TODO: get queue name from llvm pass
Q_NAME = 'q'
LSQ_FILE = f'{GIT_DIR}/lsq/LoadStoreQueue.hpp'

# This has false positives but we use it only on strings that have a variable name at the beginning
c_var_regex = r'([a-zA-Z_][a-zA-Z0-9_]*)'


def get_lsq_src():
    try:
        with open(LSQ_FILE, 'r') as f:
            return f.read()
    except Exception as e:
        print(e)
        exit("ERROR getting lsq src")

def gen_store_queue_syntax(report, q_size):
    IDX_TAG_TYPE = 'request_lsq_t'
    PIPE_DEPTH = 64

    all_lsq_pipes = ""

    for i_base, base_addr in enumerate(report['base_addresses']):
        all_lsq_pipes += f'''
        using val_type_{i_base} = {base_addr['array_type']};
        constexpr int kNumLoads_{i_base} = {base_addr['num_loads']};
        constexpr int kNumStores_{i_base} = {base_addr['num_stores']};
        constexpr int kQueueSize_{i_base} = {q_size};

        using ld_idx_pipes_{i_base} = PipeArray<class ld_idx_pipes_{i_base}_class, {IDX_TAG_TYPE}, {PIPE_DEPTH}, kNumLoads_{i_base}>;
        using ld_val_pipes_{i_base} = PipeArray<class ld_val_pipes_{i_base}_class, val_type_{i_base}, {PIPE_DEPTH}, kNumLoads_{i_base}>;
        using st_idx_pipe_{i_base} = pipe<class st_idx_pipe_{i_base}_class, {IDX_TAG_TYPE}, {PIPE_DEPTH}>;
        using st_val_pipe_{i_base} = pipe<class st_val_pipe_{i_base}_class, val_type_{i_base}, {PIPE_DEPTH}>;
        using end_lsq_signal_pipe_{i_base} = pipe<class end_lsq_signal_pipe_{i_base}_class, int>;
        
        auto eventStoreQueue_{i_base} = 
            LoadStoreQueue<val_type_{i_base}, ld_idx_pipes_{i_base}, ld_val_pipes_{i_base}, 
                           st_idx_pipe_{i_base}, st_val_pipe_{i_base}, end_lsq_signal_pipe_{i_base}, 
                           kNumLoads_{i_base}, kQueueSize_{i_base}>({Q_NAME});
        '''

    return all_lsq_pipes


def insert_storeq_wait(src, original_kernel_start):
    """Given the src lines with inserted store queue call, insert a eventStoreQueue_{i_base}.wait() before
    any device-> host data transfer"""
    insert_before_line = 0
    for i, line in enumerate(src):
        if i < original_kernel_start:
            continue
        if f'{Q_NAME}.memcpy' in line or f'{Q_NAME}.copy' in line:
            insert_before_line = i
            break
    
    lsq_event_wait_calls = []
    for i_base in range(len(report['base_addresses'])):
        lsq_event_wait_calls.append(f'eventStoreQueue_{i_base}.wait();')
    
    return src[:insert_before_line] + lsq_event_wait_calls + src[insert_before_line:]


def add_agu_pipe_connections(kernel_body, report):
    kernel_body_lines = list(map(lambda x : x + '\n', kernel_body.split('\n')))

    agu_pipe_writes = []

    for i_base, base_addr in enumerate(report['base_addresses']):
        for i in range(base_addr['num_loads']):
            agu_pipe_writes.append(f'ld_idx_pipes_{i_base}::PipeAt<{i}>::write({{0, 0}});\n')
        for i in range(base_addr['num_stores']):
            agu_pipe_writes.append(f'st_idx_pipe_{i_base}::write({{0, 0}});\n')

    agu_kernel_body = kernel_body_lines[:1] + agu_pipe_writes + kernel_body_lines[1:] 
    return agu_kernel_body, agu_pipe_writes


def gen_val_pipe_connections(report):
    pipe_calls = []

    for i_base, base_addr in enumerate(report['base_addresses']):
        # Merge address generation pipes with main kernel, if cannot decouple.
        if not report['decouple_address']:
            for i in range(base_addr['num_loads']):
                pipe_calls.append(f'ld_idx_pipes_{i_base}::PipeAt<{i}>::write({{0, 0}});\n')
            for i in range(base_addr['num_stores']):
                pipe_calls.append(f'st_idx_pipe_{i_base}::write({{0, 0}});\n')
            
        for i in range(base_addr['num_loads']):
            pipe_calls.append(f'auto ld_val_pipe_{i_base}_rd_{i} = ld_val_pipes_{i_base}::PipeAt<{i}>::read();')

        for i in range(base_addr['num_stores']):
            pipe_calls.append(f'st_val_pipe_{i_base}::write(val_type_{i_base}());')
        
        pipe_calls.append(f'int __total_store_req_{i_base} = 0;')
        pipe_calls.append(f'end_lsq_signal_pipe_{i_base}::write(__total_store_req_{i_base});')
    
    return pipe_calls


def get_kernel_body(s, kernel_name):
    body = ""
    m = re.findall(r'<\s*(?:class\s+)?' + kernel_name + r'\s*>\s*(\(.*?}\s*\)\s*;)', s, re.DOTALL)
    if m:
        body = m[0]
    else:
        exit("Failed match kernel body.")

    return body


# Point before the first call to q.submit
def get_line_of_pattern(source_file_lines, re_pattern):
    """Line of last pattern ocurrence."""
    insert_line = -1
    for i, line in enumerate(source_file_lines):
        if re.findall(re_pattern, line):
            insert_line = i

    return insert_line


def get_array_name(line_with_array, end_col):
    array = ""
    m = re.findall(c_var_regex, str(line_with_array[:end_col]))
    if m:
        array = m[0]

    return array

def llvm2ctype(llvmtype):
    if llvmtype == 'i8':
        return 'signed char'
    elif llvmtype == 'i16':
        return 'signed short'
    elif llvmtype == 'i32':
        return 'int'
    elif llvmtype == 'i64':
        return 'signed long int'

    return llvmtype

def parse_report(report_fname):
    try:
        with open(report_fname, 'r') as f:
            str = f.read()

        report = json.loads(str)
        report["kernel_name"] = report["kernel_class_name"].split(' ')[-1].split('::')[-1]
        report['spir_func_name'] = report["spir_func_name"].split('::')[0]
        for base_addr in report["base_addresses"]:
            base_addr['array_type'] = llvm2ctype(base_addr['array_type'])

        return report
    except Exception as e:
        exit("Error parsing analysis report " + report_fname)




if __name__ == '__main__':
    if len(sys.argv) < 4:
        sys.exit("USAGE: ./prog LOOP_REPORT_FILE SRC_FILE Q_SIZE")

    SRC_FNAME = sys.argv[2]
    Q_SIZE = sys.argv[3]

    with open(SRC_FNAME, 'r') as f:
        source_file = f.read()
    source_file_lines = source_file.splitlines()

    report = parse_report(sys.argv[1])

    kernel_body = get_kernel_body(source_file, report['kernel_name'])
    agu_kernel_name = f'{report["kernel_name"]}_AGU'
    agu_kernel_class_decl = f'class {agu_kernel_name};'

    agu_kernel_body, agu_pipe_writes = add_agu_pipe_connections(kernel_body, report)
    main_kernel_pipes = gen_val_pipe_connections(report)

    storeq_syntax = gen_store_queue_syntax(report, Q_SIZE)

    agu_kernel = f'{Q_NAME}.single_task<{agu_kernel_name}>{"".join(agu_kernel_body)}'

    insert_line_idx_kernels = get_line_of_pattern(source_file_lines, Q_NAME + r'\.submit')

    main_kernel_lambda = r'(<\s*' + report['kernel_name'] + \
                         r'\s*>)|(<\s*class\s*' + report['kernel_name'] + r'\s*>)'
    insert_for_main_kernel_pipes = get_line_of_pattern(source_file_lines, main_kernel_lambda)

    src_lines_with_pipes = source_file_lines[:insert_for_main_kernel_pipes+1] + \
                           main_kernel_pipes + source_file_lines[insert_for_main_kernel_pipes+1:]

    lsq_src = get_lsq_src()
    src_lines_with_pipes_and_agu = [lsq_src, agu_kernel_class_decl] + \
                                   src_lines_with_pipes[:insert_line_idx_kernels] + \
                                   [storeq_syntax]

    if report['decouple_address']:
        src_lines_with_pipes_and_agu += ["\n/// AGU", agu_kernel, "/// END AGU\n"]

    src_lines_with_pipes_and_agu += src_lines_with_pipes[insert_line_idx_kernels:]
    
    src_lines_with_storeq = insert_storeq_wait(src_lines_with_pipes_and_agu, insert_line_idx_kernels)
    
    new_filename = SRC_FNAME + '.tmp.cpp' 
    if len(sys.argv) > 4:
        new_filename = sys.argv[4]

    with open(new_filename, 'w') as f:
        f.write('\n'.join(src_lines_with_storeq))
