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

def get_lsq_store_queue_size(src):
    """
    Return the size of the store queue based on the II of a loop.

    At the moment, a single (maximum) size is returned for all LSQs. 
    Future work: return a single size for each LSQ.
    """
    DEFAULT_SIZE = 16
    MIN_SIZE = 4

    next_power_of_2 = lambda x : 1 if x == 0 else 2**math.ceil(math.log2(x))

    try:
        # Use standard compiler to perform II analysis
        ii_report_cmd = f"icpx -fsycl -std=c++17 -O2 -I{GIT_DIR}/lsq -I{GIT_DIR}/include_sycl \
        -fintelfpga -DFPGA_HW -Xshardware -Xsboard=intel_a10gx_pac:pac_a10 \
        -fsycl-link=early {src} -o {src}.a > /dev/null 2>&1"
        os.system(ii_report_cmd)

        # Look for max II in kernel.
        # TODO: only check the basic block of where the loads/stores occur.
        # This shouldn't be hard since the report contains the llvm BB names.
        loop_report_name = f"{src}.prj/reports/resources/json/loop_attr.json"
        with open(loop_report_name, 'r') as f:
            loop_report = json.loads(f.read())
            max_ii = 1
            for kernel in loop_report["nodes"]: 
                if "children" in kernel:
                    for loop in kernel["children"]: 
                        if "ii" in loop:
                            l_ii = int(loop["ii"])
                            max_ii = l_ii if (l_ii > max_ii) else max_ii
                        if "children" in loop:
                            for bb in loop["children"]: 
                                if "ii" in bb:
                                    bb_ii = int(bb["ii"])
                                    max_ii = bb_ii if (bb_ii > max_ii) else max_ii

        # +2 margin because there will be an additional pipe write-to-read latency
        res = max(next_power_of_2(max_ii + 2), MIN_SIZE)

        return res
    except Exception as e:
        pass
    
    # print(f"Info: Could not determine LSQ store queue size, using default of {DEFAULT_SIZE} instead")
    return DEFAULT_SIZE

def gen_lsq_kernel_calls(report, q_name, st_q_size):
    LD_Q_SIZE = 4

    lsq_kernel_calls = []
    lsq_events_waits = []

    for i_mem, mem_info in enumerate(report['memory_to_decouple']):
        if mem_info['is_onchip']:
            lsq_kernel_calls.append(f'''
            auto lsqEvent_{i_mem} = 
                LoadStoreQueueBRAM<{llvm2ctype(mem_info['array_type'])}, pipes_ld_req_{i_mem}, pipes_ld_val_{i_mem}, 
                                    pipes_st_req_{i_mem}, pipes_st_val_{i_mem}, pipe_end_lsq_signal_{i_mem}, 
                                    {mem_info['array_size']}, {mem_info['max_loads_per_bb']}, {mem_info['max_stores_per_bb']}, 
                                    {LD_Q_SIZE}, {st_q_size}>({q_name});
            ''')
        else:
            lsq_kernel_calls.append(f'''
            auto lsqEvent_{i_mem} = 
                LoadStoreQueueDRAM<{llvm2ctype(mem_info['array_type'])}, pipes_ld_req_{i_mem}, pipes_ld_val_{i_mem}, 
                                    pipes_st_req_{i_mem}, pipes_st_val_{i_mem}, pipe_end_lsq_signal_{i_mem}, 
                                    {mem_info['max_loads_per_bb']}, {mem_info['max_stores_per_bb']}, {LD_Q_SIZE}, {st_q_size}>({q_name});
            ''')
        lsq_events_waits.append(f'lsqEvent_{i_mem}.wait();')
    
    return lsq_kernel_calls, lsq_events_waits


def gen_lsq_pipe_declarations(report):
    """
    Generate shortcut names for arrays of pipes that are passed to the LSQ IP.
    """
    REQ_LSQ_DRAM_TYPE = 'req_lsq_dram_t'
    LD_REQ_LSQ_BRAM_TYPE = 'ld_req_lsq_bram_t'
    ST_REQ_LSQ_BRAM_TYPE = 'st_req_lsq_bram_t'
    LSQ_PIPES_DEPTH = 16

    res = []

    # Pipes for LSQ.
    for i_mem, mem_info in enumerate(report['memory_to_decouple']):
        val_type = llvm2ctype(mem_info['array_type'])
        ld_req_type = LD_REQ_LSQ_BRAM_TYPE if mem_info['is_onchip'] else REQ_LSQ_DRAM_TYPE
        st_req_type = ST_REQ_LSQ_BRAM_TYPE if mem_info['is_onchip'] else REQ_LSQ_DRAM_TYPE
        st_val_type = val_type 
        if mem_info['max_stores_per_bb'] > 1 and mem_info['is_onchip']:
            st_val_type = f'tagged_val_lsq_bram_t<{val_type}>'
        elif mem_info['max_stores_per_bb'] > 1:
            st_val_type = f'tagged_val_lsq_dram_t<{val_type}>'

        res.append(f"using pipes_ld_req_{i_mem} = \
            PipeArray<class pipes_ld_req_{i_mem}_class, {ld_req_type}, \
                   {LSQ_PIPES_DEPTH}, {mem_info['max_loads_per_bb']}>;")
        res.append(f"using pipes_st_req_{i_mem} = \
            PipeArray<class pipes_st_req_{i_mem}_class, {st_req_type}, \
                   {LSQ_PIPES_DEPTH}, {mem_info['max_stores_per_bb']}>;")
        res.append(f"using pipes_ld_val_{i_mem} = \
            PipeArray<class pipes_ld_val_{i_mem}_class, {val_type}, \
                   {LSQ_PIPES_DEPTH}, {mem_info['max_loads_per_bb']}>;")
        res.append(f"using pipes_st_val_{i_mem} = \
            PipeArray<class pipes_st_val_{i_mem}_class, {st_val_type}, \
                   {LSQ_PIPES_DEPTH}, {mem_info['max_stores_per_bb']}>;")

        res.append(f"using pipe_end_lsq_signal_{i_mem} = \
            pipe<class pipe_end_lsq_signal_{i_mem}_class, bool>;")

    # Pipes for predicated PE.
    done_pe_pipes = set()
    for dir_info in report['instr2pipe_directives']:
        p_name = dir_info['pipe_name']
        if 'pipe_pe_' in p_name and p_name not in done_pe_pipes:
            done_pe_pipes.add(p_name)
            res.append(f"using {p_name.split('_class')[0]} = pipe<class {p_name}, {dir_info['pipe_type']}>;")

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
    NUM_LSQs = len(report['memory_to_decouple'])
    NUM_PEs = len(report['blocks_to_decouple'])

    # Keep track of kernel boundaries (and adjust their values if necessary).
    kernel_start_line = report["kernel_start_line"]
    kernel_end_line = get_kernel_end_line(src_lines, kernel_start_line)
    kernel_body = get_kernel_body(src_lines, kernel_start_line, kernel_end_line)
    Q_NAME = get_queue_name(src_lines[kernel_start_line-1])
    LSQ_ST_QUEUE_SIZE = 16
    lsq_size_text = ""
    if NUM_LSQs > 0:
        # Only search for LSQ size when an LSQ is actually needed.
        LSQ_ST_QUEUE_SIZE = get_lsq_store_queue_size(SRC_FNAME)
        lsq_size_text = f" (size {LSQ_ST_QUEUE_SIZE})"

    print(f"Info: Generating {NUM_LSQs} LSQs{lsq_size_text} and {NUM_PEs} PEs")

    pipe_decl = gen_lsq_pipe_declarations(report)

    # Insert pipe type declarations into the src file.
    src_after_pipe_decl = insert_before_line(src_lines, kernel_start_line, pipe_decl)
    kernel_start_line += len(pipe_decl)
    kernel_end_line += len(pipe_decl)

    # Pipe operations in main kernel
    main_kernel_pipe_ops = gen_pipe_ops(report, report['main_kernel_name'])
    src_after_main_kernel_pipes = insert_after_line(src_after_pipe_decl, kernel_start_line, main_kernel_pipe_ops)
    kernel_end_line += len(main_kernel_pipe_ops)

    pe_kernels = []
    pe_kernel_forward_decl = []
    pe_kernel_waits = []
    for i_pe, pe_info in enumerate(report['blocks_to_decouple']):
        pe_name = f"{report['main_kernel_name']}_PE_{i_pe}"
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
    for i_mem, mem_info in enumerate(report['memory_to_decouple']):
        # If the address is not decoupled, then the agu pipe calls are in the main kernel.
        if mem_info['decouple_address']:
            agu_name = f"{report['main_kernel_name']}_AGU_{i_mem}"
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
    lsq_calls, lsq_waits = gen_lsq_kernel_calls(report, Q_NAME, LSQ_ST_QUEUE_SIZE)
    src_after_event_waits = insert_before_line(src_after_agu, kernel_start_line, lsq_calls)
    kernel_start_line += len(lsq_calls)
    kernel_end_line += len(lsq_calls)
    # Insert call to wait for LSQ event right after the original kernel call.
    all_event_waits = lsq_waits + pe_kernel_waits + agu_kernel_waits
    src_after_event_waits = insert_after_line(src_after_event_waits, kernel_end_line, all_event_waits)

    # Combine all kernels together with the LSQ kernel.
    lsq_src = get_src(LSQ_BRAM_FILE) + get_src(LSQ_DRAM_FILE) if NUM_LSQs > 0 else []
    all_combined = pe_kernel_forward_decl + agu_kernel_forward_decl + lsq_src + src_after_event_waits

    # The end result is a transformed NEW_SRC_FILENAME and 
    # the original json report with added pipe_name: instruction mappings.
    with open(NEW_SRC_FILENAME, 'w') as f:
        f.write('\n'.join(all_combined))
    with open(JSON_REPORT_FNAME, 'w') as f:
        json.dump(report, f, indent=2, sort_keys=True)
