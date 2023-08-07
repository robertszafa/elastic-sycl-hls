#!/usr/bin/env python3

"""
This script perdorms a source-to-source transformation on a sycl source containing 1 kernel.
Given an analysis json file, we generate: copies of the kernel, calls to an LSQ kernel,
and sycl pipes (pipes are just C++ type/class names).
"""

import sys
import os
import json
import re
import math

GIT_DIR = os.environ["ELASTIC_SYCL_HLS_DIR"]
LSQ_DRAM_FILE = f'{GIT_DIR}/lsq/LoadStoreQueueDRAM.hpp'
LSQ_BRAM_FILE = f'{GIT_DIR}/lsq/LoadStoreQueueBRAM.hpp'


def get_src(fname):
    try:
        with open(fname, 'r') as f:
            return f.read().splitlines()
    except Exception as e:
        print(e)
        exit("ERROR reading " + fname)

def gen_kernel_copy(q_name, kernel_body, kernel_copy_name):
    kernel_body_str = "\n".join(kernel_body)
    return f'''auto event_{kernel_copy_name} = {q_name}.single_task<{kernel_copy_name}>([=]() [[intel::kernel_args_restrict]] {{
            {kernel_body_str}
        }});\n'''.splitlines(), f'event_{kernel_copy_name}'

def insert_before_line(src_lines, line_num, new_lines):
    # Remember lines start at 1; python lists start at 0
    return src_lines[:line_num-1] + new_lines + src_lines[line_num-1:] 

def insert_after_line(src_lines, line_num, new_lines):
    # Remember lines start at 1; python lists start at 0
    return src_lines[:line_num] + new_lines + src_lines[line_num:] 

def get_kernel_body(src_lines, start_line, end_line):
    return src_lines[start_line : end_line-1]

def get_queue_name(line):
    # This regexes is guaranteed to be run on a line "... = {QUEUE}.single_task<..."
    # i.e. there will be a space before {QUEUE}
    C_VAR_REGEX = r'([a-zA-Z_][a-zA-Z0-9_]*)'
    QUEUE_NAME_REGEX = r"\s+" + C_VAR_REGEX + r"\.single_task<"
    m = re.findall(QUEUE_NAME_REGEX, line)
    if m:
        return m[0]

    exit("ERROR getting queue name.")

def llvm2ctype(llvmtype):
    if llvmtype == 'i1':
        return 'bool'
    elif llvmtype == 'i8':
        return 'signed char'
    elif llvmtype == 'i16':
        return 'signed short'
    elif llvmtype == 'i32':
        return 'int'
    elif llvmtype == 'i64':
        return 'signed long int'
    elif 'addrspace' in llvmtype:
        return 'int64_t'

    return llvmtype

def get_kernel_end_line(src_lines, kernel_start_line):
    KERNEL_CLOSE = '});'
    for i, line in enumerate(src_lines[kernel_start_line+1:], kernel_start_line+1):
        if KERNEL_CLOSE in line:
            return i + 1
    
    exit("Did not find kernel end line")

def parse_report(report_fname):
    try:
        with open(report_fname, 'r') as f:
            str = f.read()

        report = json.loads(str)

        if "main_kernel_name" not in report:
            exit("Analysis report is empty.")
        return report
    except Exception as e:
        exit("Error parsing analysis report " + report_fname)

def gen_lsq_kernel_calls(report, q_name):
    LD_Q_SIZE = 4

    lsq_kernel_calls = []
    lsq_events_waits = []

    for i_lsq, lsq_info in enumerate(report['lsq_array']):
        print(f"Info: Generating LSQ_{i_lsq}")
        print(f"  store queue size: {lsq_info['store_queue_size']}")
        print(f"  address gen. decoupled: {lsq_info['decouple_address']}")
        print(f"  speculation: {lsq_info['is_any_speculation']}")

        if lsq_info['is_onchip']:
            lsq_kernel_calls.append(f'''
            auto lsqEvent_{i_lsq} = 
                LoadStoreQueueBRAM<{llvm2ctype(lsq_info['array_type'])}, pipes_ld_req_{i_lsq}, pipes_ld_val_{i_lsq}, 
                                    pipes_st_req_{i_lsq}, pipes_st_val_{i_lsq}, pipe_end_lsq_signal_{i_lsq}, {int(lsq_info['is_any_speculation'])},
                                    {lsq_info['array_size']}, {lsq_info['max_loads_per_bb']}, {lsq_info['max_stores_per_bb']}, 
                                    {LD_Q_SIZE}, {lsq_info['store_queue_size']}>({q_name});
            ''')
        else:
            lsq_kernel_calls.append(f'''
            auto lsqEvent_{i_lsq} = 
                LoadStoreQueueDRAM<{llvm2ctype(lsq_info['array_type'])}, pipes_ld_req_{i_lsq}, pipes_ld_val_{i_lsq}, 
                                    pipes_st_req_{i_lsq}, pipes_st_val_{i_lsq}, pipe_end_lsq_signal_{i_lsq}, {int(lsq_info['is_any_speculation'])},
                                    {lsq_info['max_loads_per_bb']}, {lsq_info['max_stores_per_bb']}, {LD_Q_SIZE}, {lsq_info['store_queue_size']}>({q_name});
            ''')
        lsq_events_waits.append(f'lsqEvent_{i_lsq}.wait();')
    
    return lsq_kernel_calls, lsq_events_waits


def gen_lsq_pipe_declarations(report):
    """
    Generate shortcut names for arrays of pipes that are passed to the LSQ IP.
    """
    LSQ_LD_PIPE_DEPTH = 32
    LSQ_ST_PIPE_DEPTH = 32

    res = []

    # Pipes for LSQ.
    for i_lsq, lsq_info in enumerate(report['lsq_array']):
        val_type = llvm2ctype(lsq_info['array_type'])
        ld_req_type = 'ld_req_lsq_bram_t' if lsq_info['is_onchip'] else 'req_lsq_dram_t'
        st_req_type = 'st_req_lsq_bram_t' if lsq_info['is_onchip'] else 'req_lsq_dram_t'
        st_val_type = f'tagged_val_lsq_bram_t<{val_type}>' if lsq_info['is_onchip'] else f'tagged_val_lsq_dram_t<{val_type}>'
        
        # Adjust pipe sizes going into the LSQ based on num lds/sts.
        # LD pipes only need to be adjusted for BRAM LSQs, because DRAM has multiple ld ports.
        if lsq_info['is_onchip']: 
            LSQ_LD_PIPE_DEPTH *= max(1, math.ceil(lsq_info['max_loads_per_bb']/lsq_info['max_stores_per_bb']))
        LSQ_ST_PIPE_DEPTH *= max(1, math.ceil(lsq_info['max_stores_per_bb']/lsq_info['max_loads_per_bb']))

        res.append(f"using pipes_ld_req_{i_lsq} = \
            PipeArray<class pipes_ld_req_{i_lsq}_class, {ld_req_type}, \
                   {LSQ_LD_PIPE_DEPTH}, {lsq_info['max_loads_per_bb']}>;")
        res.append(f"using pipes_ld_val_{i_lsq} = \
            PipeArray<class pipes_ld_val_{i_lsq}_class, {val_type}, \
                   {LSQ_LD_PIPE_DEPTH}, {lsq_info['max_loads_per_bb']}>;")
        res.append(f"using pipes_st_req_{i_lsq} = \
            PipeArray<class pipes_st_req_{i_lsq}_class, {st_req_type}, \
                   {LSQ_ST_PIPE_DEPTH}, {lsq_info['max_stores_per_bb']}>;")
        res.append(f"using pipes_st_val_{i_lsq} = \
            PipeArray<class pipes_st_val_{i_lsq}_class, {st_val_type}, \
                   {LSQ_ST_PIPE_DEPTH}, {lsq_info['max_stores_per_bb']}>;")

        res.append(f"using pipe_end_lsq_signal_{i_lsq} = \
            pipe<class pipe_end_lsq_signal_{i_lsq}_class, bool>;")

    # Pipes for predicated PE.
    done_pe_pipes = set()
    for dir_info in report['instr2pipe_directives']:
        p_name = dir_info['pipe_name']
        if 'pipe_pe_' in p_name and p_name not in done_pe_pipes:
            done_pe_pipes.add(p_name)
            res.append(f"using {p_name.split('_class')[0]} = pipe<class {p_name}, {llvm2ctype(dir_info['pipe_type'])}>;")

    return res

def gen_pipe_ops(report, kernel_name):
    res = []
    for i2p in report['instr2pipe_directives']:
        if i2p['kernel_name'] == kernel_name:
            this_call = i2p['pipe_name'].split('_class')[0]
            if 'seq_in_bb' in i2p:
                this_call += f"::PipeAt<{i2p['seq_in_bb']}>"
            
            if i2p['read/write'] == 'read':
                seqinbb = ""
                bbidx = ""
                if 'seq_in_bb' in i2p: 
                    seqinbb += f"_seq_in_bb_{i2p['seq_in_bb']}"
                if 'instruction' in i2p:
                    bbidx += f"_bb_idx_{i2p['instruction']['basic_block_idx']}"
                this_call = f"auto read_res_{i2p['pipe_name']}{bbidx}{seqinbb} = " + this_call + "::read();"
            else:
                this_call += "::write({});"

            res.append(this_call)
    
    return res


if __name__ == '__main__':
    if len(sys.argv) < 4:
        sys.exit("USAGE: ./prog ANALYSIS_JSON_REPORT SRC_FILE NEW_SRC_FILE")

    JSON_REPORT_FNAME = sys.argv[1]
    SRC_FNAME = sys.argv[2]
    NEW_SRC_FILENAME = sys.argv[3]
    
    with open(SRC_FNAME, 'r') as f:
        src_string = f.read()
    src_lines = src_string.splitlines()
    
    report = parse_report(JSON_REPORT_FNAME)

    # Keep track of kernel boundaries (and adjust their values if necessary).
    kernel_start_line = report["kernel_start_line"]
    kernel_end_line = get_kernel_end_line(src_lines, kernel_start_line)
    kernel_body = get_kernel_body(src_lines, kernel_start_line, kernel_end_line)
    Q_NAME = get_queue_name(src_lines[kernel_start_line-1])

    lsq_pipe_decl = gen_lsq_pipe_declarations(report)

    # Insert pipe type declarations into the src file.
    src_after_pipe_decl = insert_before_line(src_lines, kernel_start_line, lsq_pipe_decl)
    kernel_start_line += len(lsq_pipe_decl)
    kernel_end_line += len(lsq_pipe_decl)

    # Pipe operations in main kernel
    main_kernel_pipe_ops = gen_pipe_ops(report, report['main_kernel_name'])
    src_after_main_kernel_pipes = insert_after_line(src_after_pipe_decl, kernel_start_line, main_kernel_pipe_ops)
    kernel_end_line += len(main_kernel_pipe_ops)

    pe_kernels = []
    pe_kernel_forward_decl = []
    pe_kernel_waits = []
    for i_pe, pe_info in enumerate(report['pe_array']):
        pe_name = pe_info['pe_kernel_name']
        print(f"Info: Generating {pe_name.split(' ')[-1]}")

        pe_kernel_forward_decl.append(f"class {pe_name.split(' ')[-1]};")
        # Use split to extract 'MainKernel' from 'typeinfo name for MainKernel'.
        pe_kernel, pe_event = gen_kernel_copy(Q_NAME, kernel_body, pe_name.split(' ')[-1])
        pe_kernel_waits.append(f"{pe_event}.wait();\n")
        pe_pipe_ops = gen_pipe_ops(report, pe_name)
        pe_kernel_str = "\n".join(insert_after_line(pe_kernel, 1, pe_pipe_ops))
        pe_kernels.append(pe_kernel_str)
    # Combine the created PE kernel with the original kernel.
    src_after_pe = insert_before_line(src_after_main_kernel_pipes, kernel_start_line, pe_kernels)
    kernel_start_line += len(pe_kernels)
    kernel_end_line += len(pe_kernels)

    # Create address generation kernel if decoupled flag is set.
    # Add lsq pipe ops to it. If flag not set, add them to orginal kernel.
    agu_kernels = []
    agu_kernel_forward_decl = []
    agu_kernel_waits = []
    for i_lsq, lsq_info in enumerate(report['lsq_array']):
        # If the address is not decoupled, then the agu pipe calls are in the main kernel.
        if lsq_info['decouple_address']:
            agu_name = f"{report['main_kernel_name']}_AGU_{i_lsq}"
            agu_kernel_forward_decl.append(f"class {agu_name.split(' ')[-1]};")
            # Use split to extract 'MainKernel' from 'typeinfo name for MainKernel'.
            agu_kernel, agu_event = gen_kernel_copy(Q_NAME, kernel_body, agu_name.split(' ')[-1])
            agu_kernel_waits.append(f'{agu_event}.wait();\n')
            agu_pipe_ops = gen_pipe_ops(report, agu_name)
            agu_kernel_str = "\n".join(insert_after_line(agu_kernel, 1, agu_pipe_ops))
            agu_kernels.append(agu_kernel_str)
    # Combine the created AGU kernel with the original kernel (no-op if AGU is empty).
    src_after_agu = insert_before_line(src_after_pe, kernel_start_line, agu_kernels)
    kernel_start_line += len(agu_kernels)
    kernel_end_line += len(agu_kernels)

    # Insert call to LSQ kernels right before the original kernel call.
    lsq_calls, lsq_waits = gen_lsq_kernel_calls(report, Q_NAME)
    src_after_event_waits = insert_before_line(src_after_agu, kernel_start_line, lsq_calls)
    kernel_start_line += len(lsq_calls)
    kernel_end_line += len(lsq_calls)
    # Insert call to wait for LSQ event right after the original kernel call.
    all_event_waits = lsq_waits + pe_kernel_waits + agu_kernel_waits
    src_after_event_waits = insert_after_line(src_after_event_waits, kernel_end_line, all_event_waits)

    # Combine all kernels together with the LSQ kernel.
    lsq_src = get_src(LSQ_BRAM_FILE) + get_src(LSQ_DRAM_FILE) if len(report['lsq_array']) > 0 else []
    all_combined = pe_kernel_forward_decl + agu_kernel_forward_decl + lsq_src + src_after_event_waits

    # The end result is a transformed NEW_SRC_FILENAME and 
    # the original json report with added pipe_name: instruction mappings.
    with open(NEW_SRC_FILENAME, 'w') as f:
        f.write('\n'.join(all_combined))
    with open(JSON_REPORT_FNAME, 'w') as f:
        json.dump(report, f, indent=2, sort_keys=True)
