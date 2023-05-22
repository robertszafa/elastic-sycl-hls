#!/usr/bin/env python3

"""
This script perdorms a source-to-source transformation on a sycl source containing 1 kernel.
Given an analysis json file, we generate: copies of the kernel, calls to an LSQ kernel,
and sycl pipes (pipes are just C++ type/class names).
"""

import sys
import os

from ASTCommon import *

LSQ_DRAM_FILE = f'{os.environ["LLVM_SYCL_PASSES_DIR"]}/lsq/LoadStoreQueueDRAM.hpp'
LSQ_BRAM_FILE = f'{os.environ["LLVM_SYCL_PASSES_DIR"]}/lsq/LoadStoreQueueBRAM.hpp'
REQ_LSQ_DRAM_TYPE = 'req_lsq_dram_t'
LD_REQ_LSQ_BRAM_TYPE = 'ld_req_lsq_bram_t'
ST_REQ_LSQ_BRAM_TYPE = 'st_req_lsq_bram_t'
LSQ_PIPES_DEPTH = 32

LD_Q_SIZE = 4

def get_src(fname):
    try:
        with open(fname, 'r') as f:
            return f.read().splitlines()
    except Exception as e:
        print(e)
        exit("ERROR reading " + fname)

def gen_lsq_kernel_calls(report, q_name, st_q_size):
    lsq_kernel_calls = []
    lsq_events_waits = []

    for i_base, base_addr in enumerate(report['base_addresses']):
        if base_addr['is_onchip']:
            lsq_kernel_calls.append(f'''
            auto lsqEvent_{i_base} = 
                LoadStoreQueueBRAM<{llvm2ctype(base_addr['array_type'])}, pipes_ld_req_{i_base}, pipes_ld_val_{i_base}, 
                                    pipes_st_req_{i_base}, pipes_st_val_{i_base}, pipe_end_lsq_signal_{i_base}, 
                                    {base_addr['array_size']}, {base_addr['num_loads']}, {base_addr['num_stores']}, 
                                    {LD_Q_SIZE}, {st_q_size}>({q_name});
            ''')
        else:
            lsq_kernel_calls.append(f'''
            auto lsqEvent_{i_base} = 
                LoadStoreQueueDRAM<{llvm2ctype(base_addr['array_type'])}, pipes_ld_req_{i_base}, pipes_ld_val_{i_base}, 
                                    pipes_st_req_{i_base}, pipes_st_val_{i_base}, pipe_end_lsq_signal_{i_base}, 
                                    {base_addr['num_loads']}, {base_addr['num_stores']}, {LD_Q_SIZE}, {st_q_size}>({q_name});
            ''')
        lsq_events_waits.append(f'lsqEvent_{i_base}.wait();')
    
    return lsq_kernel_calls, lsq_events_waits

def get_array_name(line_with_array, end_col):
    array = ""
    c_var_regex = r'([a-zA-Z_][a-zA-Z0-9_]*)'
    m = re.findall(c_var_regex, str(line_with_array[:end_col]))
    if m:
        array = m[0]

    return array

def add_array_ivdep(src_lines, start_line, end_line, report):
    IGNORE_DEP_PRAGMA = ''
    for base_addr in report['base_addresses']:
        if "array_line" in base_addr and "array_column" in base_addr:
            array_name = get_array_name(src_lines[base_addr['array_line'] - 1], 
                                        base_addr['array_column'])
            if array_name:
                IGNORE_DEP_PRAGMA += f'[[intel::ivdep({array_name})]] '

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

def gen_pipes(report):
    # Each LSQ gets its own pipes.
    all_ld_req_pipes = []
    all_st_req_pipes = []
    all_ld_val_pipes = []
    all_st_val_pipes = []
    lsq_signal_pipes = []

    for i_base, base_addr in enumerate(report['base_addresses']):
        val_type = llvm2ctype(base_addr['array_type'])
        ld_req_type = LD_REQ_LSQ_BRAM_TYPE if base_addr['is_onchip'] else REQ_LSQ_DRAM_TYPE
        st_req_type = ST_REQ_LSQ_BRAM_TYPE if base_addr['is_onchip'] else REQ_LSQ_DRAM_TYPE
        if base_addr['num_stores'] > 1 and base_addr['is_onchip']:
            st_val_pipe_type = f'tagged_val_lsq_bram_t<{val_type}>'
        elif base_addr['num_stores'] > 1:
            st_val_pipe_type = f'tagged_val_lsq_dram_t<{val_type}>'
        else:
            st_val_pipe_type = val_type 
        is_mux_for_ld_vals = base_addr['is_onchip'] and base_addr['num_loads'] > 1
        ld_val_type = f'tagged_val_lsq_bram_t<{val_type}>' if is_mux_for_ld_vals else val_type

        ld_req_pipes = SyclPipe(f'pipes_ld_req_{i_base}', ld_req_type, 
                                amount=base_addr['num_loads'], depth=LSQ_PIPES_DEPTH, pipe_array=True)
        ld_val_pipes = SyclPipe(f'pipes_ld_val_{i_base}', ld_val_type, 
                                amount=base_addr['num_loads'], depth=LSQ_PIPES_DEPTH, pipe_array=True)
        st_req_pipes = SyclPipe(f'pipes_st_req_{i_base}', st_req_type, 
                                base_addr['num_stores'], LSQ_PIPES_DEPTH, pipe_array=True)
        st_val_pipes = SyclPipe(f'pipes_st_val_{i_base}', st_val_pipe_type,
                                base_addr['num_stores'], LSQ_PIPES_DEPTH, pipe_array=True)

        all_ld_req_pipes.append(ld_req_pipes)
        all_st_req_pipes.append(st_req_pipes)
        all_ld_val_pipes.append(ld_val_pipes)
        all_st_val_pipes.append(st_val_pipes)

        lsq_signal_pipes.append(SyclPipe(f'pipe_end_lsq_signal_{i_base}', 'bool', depth=2))

    return all_ld_req_pipes, all_st_req_pipes, all_ld_val_pipes, all_st_val_pipes, lsq_signal_pipes

# Given various pipes, associate them with the instructions in report, and
# add this mapping to the report.
def add_pipe_names_to_report(report, ld_req_pipes, ld_val_pipes, st_req_pipes, 
                             st_val_pipes, lsq_signal_pipes):
    # Pipes that are part of a PipeArray have an additional field 'struct_id'.
    for i_base, base_addr in enumerate(report['base_addresses']):
        base_addr["pipes_ld_req"] = [{'name': ld_req_pipes[i_base].class_name, 'struct_id': i, 
                                      'instruction':base_addr['load_instructions'][i], 'is_onchip': base_addr['is_onchip']}
                                      for i in range(ld_req_pipes[i_base].amount)]
        base_addr["pipes_ld_val"] = [{'name': ld_val_pipes[i_base].class_name, 'struct_id': i, 
                                      'instruction': base_addr['load_instructions'][i], 'is_onchip': base_addr['is_onchip']} 
                                      for i in range(ld_val_pipes[i_base].amount)]
        base_addr["pipes_st_req"] = [{'name': st_req_pipes[i_base].class_name, 'struct_id': i, 
                                      'instruction': base_addr['store_instructions'][i], 'is_onchip': base_addr['is_onchip']} 
                                    for i in range(st_req_pipes[i_base].amount)]
        base_addr["pipes_st_val"] = [{'name': st_val_pipes[i_base].class_name, 'struct_id': i, 
                                      'instruction':base_addr['store_instructions'][i], 'is_onchip': base_addr['is_onchip']} 
                                      for i in range(st_val_pipes[i_base].amount)]
        
        base_addr["pipe_end_lsq"] = {'name': lsq_signal_pipes[i_base].class_name, 'struct_id': -1}


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
    kernel_end_line = get_kernel_end_line(src_lines, kernel_start_line)

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

    # Insert call to LSQ kernels right before the original kernel call.
    lsq_calls, lsq_waits = gen_lsq_kernel_calls(report, Q_NAME, Q_SIZE)
    src_with_pipe_decl_and_lsq_calls = insert_before_line(
        src_with_pipe_decl, kernel_start_line, lsq_calls)
    kernel_start_line += len(lsq_calls)
    kernel_end_line += len(lsq_calls)
    # Insert call to wait for LSQ event right after the original kernel call.
    src_with_pipe_decl_and_lsq_calls = insert_after_line(
        src_with_pipe_decl_and_lsq_calls, kernel_end_line, lsq_waits)

    # Generate load value and store value pipes.
    main_kernel_pipe_ops = []
    for i_base, base_addr in enumerate(report['base_addresses']):
        main_kernel_pipe_ops.append(ld_val_pipes[i_base].read_op())
        main_kernel_pipe_ops.append(st_val_pipes[i_base].write_op())
        main_kernel_pipe_ops.append(lsq_signal_pipes[i_base].write_op())

    # Generate LSQ reqeust pipes and end signal pipes.
    agu_pipe_ops = []
    # The AGU kernel name is forward declared to avoid mangling.
    agu_kernel_declarations = []
    for i_base, base_addr in enumerate(report['base_addresses']):
        agu_kernel_declarations.append(f"class {report['kernel_name']}_AGU_{i_base};")

        this_base = []
        this_base.append(ld_req_pipes[i_base].write_op()) 
        this_base.append(st_req_pipes[i_base].write_op())
        agu_pipe_ops.append(this_base)

    # Add ld value reads and st value writes to original kernel. 
    src_with_pipe_decl_lsq_calls_and_val_pipes = insert_after_line(
        src_with_pipe_decl_and_lsq_calls, kernel_start_line, main_kernel_pipe_ops)
    kernel_end_line += len(main_kernel_pipe_ops)

    # Create address generation kernel if decoupled flag is set.
    # Add lsq pipe ops to it. If flag not set, add them to orginal kernel.
    agu_kernels = []
    for i_base, base_addr in enumerate(report['base_addresses']):
        if base_addr['decouple_address']:
            base_addr["kernel_agu_name_full"] = f"{report['kernel_name_full']}_AGU_{i_base}"
            agu_kernel = gen_kernel_copy(Q_NAME, kernel_body, f"{report['kernel_name']}_AGU_{i_base}")
            agu_kernel_str = "\n".join(insert_after_line(agu_kernel, 1, agu_pipe_ops[i_base]))
            agu_kernels.append(agu_kernel_str)
        else:  
            base_addr["kernel_agu_name_full"] = report['kernel_name_full']
            src_with_pipe_decl_lsq_calls_and_val_pipes = insert_after_line(
                src_with_pipe_decl_lsq_calls_and_val_pipes, kernel_start_line, agu_pipe_ops[i_base])
            kernel_end_line += len(agu_pipe_ops[i_base])

    # Combine the created AGU kernel with the original kernel (no-op if AGU is empty).
    combined_original_agu_pipes = insert_before_line(
        src_with_pipe_decl_lsq_calls_and_val_pipes, kernel_start_line, agu_kernels)
    kernel_start_line += len(agu_kernels)
    kernel_end_line += len(agu_kernels)

    # Combine all kernels together with the LSQ kernel.
    lsq_src = get_src(LSQ_BRAM_FILE) + get_src(LSQ_DRAM_FILE)
    all_combined = lsq_src + agu_kernel_declarations + combined_original_agu_pipes

    # The end result is a transformed NEW_SRC_FILENAME and 
    # the original json report with added pipe_name: instruction mappings.
    with open(NEW_SRC_FILENAME, 'w') as f:
        f.write('\n'.join(all_combined))
    with open(JSON_REPORT_FNAME, 'w') as f:
        json.dump(report, f, indent=4, sort_keys=True)
