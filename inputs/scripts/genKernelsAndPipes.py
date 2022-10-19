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

# TODO: get queue name from llvm pass
Q_NAME = 'q'
STOREQ_HEADER = '#include "store_queue.hpp"'

# This has false positives but we use it only on strings that have a variable name at the beginning
c_var_regex = r'([a-zA-Z_][a-zA-Z0-9_]*)'


def gen_store_queue_syntax(report, q_size):
    IDX_TAG_TYPE = 'pair_t'
    PIPE_DEPTH = 64
    ST_LATENCY = 12

    return f'''
    using val_type = {report['array_type']};
    constexpr int kNumLoads = {report['num_loads']};
    constexpr int kNumStores = {report['num_stores']};
    constexpr bool isQueueForwarding = true;
    constexpr int kQueueSize = {q_size};
    constexpr int kStoreLatency = {ST_LATENCY};

    using ld_idx_pipes = PipeArray<class ld_idx_pipes_class, {IDX_TAG_TYPE}, {PIPE_DEPTH}, kNumLoads>;
    using ld_val_pipes = PipeArray<class ld_val_pipes_class, val_type, {PIPE_DEPTH}, kNumLoads>;
    using st_idx_pipe = pipe<class st_idx_pipe_class, {IDX_TAG_TYPE}, {PIPE_DEPTH}>;
    using st_val_pipe = pipe<class st_val_pipe_class, val_type, {PIPE_DEPTH}>;
    using end_signal_pipe = pipe<class end_signal_pipe_class, int>;
    
    auto __array_device_ptr = sycl::device_ptr<val_type>({report['array_name']});
    auto eventStoreQueue = 
        StoreQueue<ld_idx_pipes, ld_val_pipes, kNumLoads, st_idx_pipe, st_val_pipe, end_signal_pipe,
                   isQueueForwarding, kQueueSize, kStoreLatency>({Q_NAME}, __array_device_ptr);
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


def add_agu_pipe_connections(kernel_body, report):
    kernel_body_lines = list(map(lambda x : x + '\n', kernel_body.split('\n')))

    agu_pipe_writes = []
    for i in range(report['num_loads']):
       agu_pipe_writes.append(f'ld_idx_pipes::PipeAt<{i}>::write({{0, 0}});\n')
    agu_pipe_writes.append(f'st_idx_pipe::write({{0, 0}});\n')

    agu_kernel_body = kernel_body_lines[:1] + agu_pipe_writes + kernel_body_lines[1:] 
    return agu_kernel_body, agu_pipe_writes


def gen_val_pipe_connections(report):
    pipe_calls = []

    for i in range(report['num_loads']):
        pipe_calls.append(f'auto ld_val_pipe_rd_{i} = ld_val_pipes::PipeAt<{i}>::read();')

    for i in range(report['num_stores']):
        pipe_calls.append(f'st_val_pipe::write(val_type());')
        
    pipe_calls.append(f'int __total_store_req = 0;')
    pipe_calls.append(f'end_signal_pipe::write(__total_store_req);')
    
    return pipe_calls


def get_kernel_body(s):
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


def llvm2ctype(llvm_type):
    if llvm_type == 'i32':
        return 'int'

    return llvm_type


def parse_report(report_fname, source_file_lines):
    try:
        with open(report_fname, 'r') as f:
            str = f.read()

        report = json.loads(str)
        report["kernel_name"] = report["kernel_class_name"].split(' ')[-1].split('::')[-1]
        report['array_name'] = get_array_name(source_file_lines[report['array_line']-1], 
                                              report['array_column'])
        report['array_type'] = llvm2ctype(report['val_type'])
        report['spir_func_name'] = report["spir_func_name"].split('::')[0]

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

    report = parse_report(sys.argv[1], source_file_lines)

    kernel_name = report['kernel_name']
    kernel_body = get_kernel_body(source_file)
    agu_kernel_name = f'{kernel_name}_AGU'
    agu_kernel_class_decl = f'class {agu_kernel_name};'

    agu_kernel_body, agu_pipe_writes = add_agu_pipe_connections(kernel_body, report)
    main_kernel_pipes = gen_val_pipe_connections(report)
    if not report['split_stores']:
        main_kernel_pipes =  agu_pipe_writes + main_kernel_pipes
    storeq_syntax = gen_store_queue_syntax(report, Q_SIZE)

    agu_kernel = f'{Q_NAME}.single_task<{agu_kernel_name}>{"".join(agu_kernel_body)}'

    insert_line_idx_kernels = get_line_of_pattern(source_file_lines, Q_NAME + r'\.submit')

    main_kernel_lambda = r'(<\s*' + kernel_name + r'\s*>)|(<\s*class\s*' + kernel_name + r'\s*>)'
    insert_for_main_kernel_pipes = get_line_of_pattern(source_file_lines, main_kernel_lambda)

    src_lines_with_pipes = source_file_lines[:insert_for_main_kernel_pipes+1] + \
                           main_kernel_pipes + source_file_lines[insert_for_main_kernel_pipes+1:]

    src_lines_with_pipes_and_agu = [STOREQ_HEADER, agu_kernel_class_decl] + \
                                   src_lines_with_pipes[:insert_line_idx_kernels] + \
                                   [storeq_syntax]

    if report['split_stores']:
        src_lines_with_pipes_and_agu += ["\n/// AGU", agu_kernel, "/// END AGU\n"]

    src_lines_with_pipes_and_agu += src_lines_with_pipes[insert_line_idx_kernels:]
    
    src_lines_with_storeq = insert_storeq_wait(src_lines_with_pipes_and_agu, insert_line_idx_kernels)
    
    new_filename = SRC_FNAME + '.tmp.cpp' 
    with open(new_filename, 'w') as f:
        f.write('\n'.join(src_lines_with_storeq))

    print(f'-- Refactored source file at {new_filename}')
