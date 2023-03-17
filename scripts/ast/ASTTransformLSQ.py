#!/usr/bin/env python3

"""
This script perdorms a source-to-source transformation on a sycl source containing 1 kernel.
Given an analysis json file, we generate: copies of the kernel, calls to an LSQ kernel,
and sycl pipes (pipes are just C++ type/class names).
"""

import sys
import os

from ASTCommon import *

LSQ_FILE = f'{os.environ["LLVM_SYCL_PASSES_DIR"]}/lsq/LoadStoreQueue.hpp'
MUX_FILE = f'{os.environ["LLVM_SYCL_PASSES_DIR"]}/lsq/MultiplexorLSQ.hpp'
LSQ_REQUEST_TYPE = 'request_lsq_t'
PIPE_DEPTH = 64
MAX_STORES_WO_MUX = 2

def get_src(fname):
    try:
        with open(fname, 'r') as f:
            return f.read().splitlines()
    except Exception as e:
        print(e)
        exit("ERROR reading " + fname)

def gen_lsq_kernel_calls(report, q_name, q_size):
    result = []
    lsq_events = []

    for i_base, base_addr in enumerate(report['base_addresses']):
        # The stores will either come directly from the main kernel, or will be routed through a Mux kernel.
        pipe_st_req = f'pipe_out_mux_streq_{i_base}' if base_addr['insert_st_mux'] else f'pipe_st_req_{i_base}'
        pipe_st_val = f'pipe_out_mux_stval_{i_base}' if base_addr['insert_st_mux'] else f'pipe_st_val_{i_base}'

        result.append(f'''
        auto lsqEvent_{i_base} = 
            LoadStoreQueue<{llvm2ctype(base_addr['array_type'])}, pipes_ld_req_{i_base}, pipes_ld_val_{i_base}, 
                           {pipe_st_req}, {pipe_st_val}, pipe_end_lsq_signal_{i_base}, 
                           {base_addr['num_loads']}, {q_size}>({q_name});
        ''')
        lsq_events.append(f'lsqEvent_{i_base}')
    
    # The lsq wait calls might be superflous but add them to make sure.
    for e in lsq_events:
        result.append(f'{e}.wait();')

    return result

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

def gen_tagged_val_type(scalar_type):
    return f'tagged_val_t<{scalar_type}>'

def gen_mux_kernel_calls(report, q_name, ii):
    result = []

    for i_base, base_addr in enumerate(report['base_addresses']):
        val_type = gen_tagged_val_type(llvm2ctype(base_addr['array_type']))
        call = f'''
            auto muxStreqEvent_{i_base} = 
                Multiplexor<pipes_st_req_{i_base}, pipe_out_mux_streq_{i_base}, 
                            pipe_end_mux_streq_signal_{i_base}, {LSQ_REQUEST_TYPE}, 
                            {base_addr['num_stores']}, {ii}>({q_name});

            auto muxStvalEvent_{i_base} = 
                Multiplexor<pipes_st_val_{i_base}, pipe_out_mux_stval_{i_base}, 
                            pipe_end_mux_stval_signal_{i_base}, {val_type},
                            {base_addr['num_stores']}, {ii}>({q_name});
            '''

        # Only use {call} when this base needs a mux for stores.
        if base_addr['insert_st_mux']:
            result.append(call)
    
    return result

def gen_pipes(report):
    ld_address_pipes = []
    st_req_pipes = []
    ld_value_pipes = []
    st_value_pipes = []
    lsq_signal_pipes = []
    mux_streq_signal_pipes = []
    mux_stval_signal_pipes = []
    mux_streq_out_pipes = []
    mux_stval_out_pipes = []

    for i_base, base_addr in enumerate(report['base_addresses']):
        val_type = llvm2ctype(base_addr['array_type'])
        mux_stores = base_addr['num_stores'] > MAX_STORES_WO_MUX
        base_addr['insert_st_mux'] = mux_stores
        if mux_stores:
            mux_streq_signal_pipes.append(SyclPipe(f'pipe_end_mux_streq_signal_{i_base}', 'int'))
            mux_stval_signal_pipes.append(SyclPipe(f'pipe_end_mux_stval_signal_{i_base}', 'int'))
            mux_streq_out_pipes.append(SyclPipe(f'pipe_out_mux_streq_{i_base}', LSQ_REQUEST_TYPE))
            mux_stval_out_pipes.append(SyclPipe(f'pipe_out_mux_stval_{i_base}', val_type))

        ld_address_pipe_array = SyclPipe(
            f'pipes_ld_req_{i_base}', LSQ_REQUEST_TYPE, base_addr['num_loads'], PIPE_DEPTH)
        ld_val_pipe_array = SyclPipe(
            f'pipes_ld_val_{i_base}', val_type, base_addr['num_loads'], PIPE_DEPTH)

        if mux_stores: # Need to route through multiplexor.
            st_req_pipe = SyclPipe(f'pipes_st_req_{i_base}', LSQ_REQUEST_TYPE, 
                                    base_addr['num_stores'], PIPE_DEPTH)
            # Need tag to mux values.
            st_val_pipe = SyclPipe(f'pipes_st_val_{i_base}', gen_tagged_val_type(val_type), 
                                   base_addr['num_stores'], PIPE_DEPTH)
        else: # Direct pipe from application kernel to LSQ
            st_req_pipe = SyclPipe(f'pipe_st_req_{i_base}', LSQ_REQUEST_TYPE, depth=PIPE_DEPTH, write_repeat=base_addr['num_stores'])
            st_val_pipe = SyclPipe(f'pipe_st_val_{i_base}', val_type, depth=PIPE_DEPTH, write_repeat=base_addr['num_stores'])

        ld_address_pipes.append(ld_address_pipe_array)
        st_req_pipes.append(st_req_pipe)
        ld_value_pipes.append(ld_val_pipe_array)
        st_value_pipes.append(st_val_pipe)

        lsq_signal_pipes.append(SyclPipe(f'pipe_end_lsq_signal_{i_base}', 'int'))

    return ld_address_pipes, st_req_pipes, ld_value_pipes, st_value_pipes, lsq_signal_pipes, \
        mux_streq_signal_pipes, mux_stval_signal_pipes, mux_streq_out_pipes, mux_stval_out_pipes

# Given various pipes, associate them with the instructions in report, and
# add this mapping to the report.
def add_pipe_names_to_report(report, ld_req_pipes, ld_val_pipes, st_req_pipes, st_val_pipes, 
                             lsq_signal_pipes, mux_streq_signal_pipes, mux_stval_signal_pipes):
    # Pipes that are part of a PipeArray have an additional field 'struct_id'.
    # We also have a 'repeat_id' field to differentiate between multiple calls
    # to the same pipe.
    for i_base, base_addr in enumerate(report['base_addresses']):
        base_addr["pipes_ld_req"] = [{'name': ld_req_pipes[i_base].class_name, 'struct_id': i, 'repeat_id': 0, 'instruction':
                                      base_addr['load_instructions'][i]} for i in range(ld_req_pipes[i_base].amount)]
        base_addr["pipes_ld_val"] = [{'name': ld_val_pipes[i_base].class_name, 'struct_id': i, 'repeat_id': 0, 'instruction':
                                      base_addr['load_instructions'][i]} for i in range(ld_val_pipes[i_base].amount)]

        if base_addr['insert_st_mux']:
            base_addr["pipes_st_req"] = [{'name': st_req_pipes[i_base].class_name, 'struct_id': i, 'repeat_id': 0, 'instruction':
                                        base_addr['store_instructions'][i]} for i in range(st_req_pipes[i_base].amount)]
            base_addr["pipes_st_val"] = [{'name': st_val_pipes[i_base].class_name, 'struct_id': i, 'repeat_id': 0, 'instruction':
                                        base_addr['store_instructions'][i]} for i in range(st_val_pipes[i_base].amount)]
        else:
            base_addr["pipes_st_req"] = [{'name': st_req_pipes[i_base].class_name, 'struct_id': -1, 'repeat_id': i, 'instruction':
                                        base_addr['store_instructions'][i]} for i in range(st_req_pipes[i_base].write_repeat)]
            base_addr["pipes_st_val"] = [{'name': st_val_pipes[i_base].class_name, 'struct_id': -1, 'repeat_id': i, 'instruction':
                                        base_addr['store_instructions'][i]} for i in range(st_val_pipes[i_base].write_repeat)]
        
        base_addr["pipe_end_lsq"] = {'name': lsq_signal_pipes[i_base].class_name, 'struct_id': -1, 'repeat_id': 0}

        if base_addr['insert_st_mux']:
            base_addr["pipe_end_mux_streq"] = {'name': mux_streq_signal_pipes[i_base].class_name, 'struct_id': -1, 'repeat_id': 0}
            base_addr["pipe_end_mux_stval"] = {'name': mux_stval_signal_pipes[i_base].class_name, 'struct_id': -1, 'repeat_id': 0}


if __name__ == '__main__':
    if len(sys.argv) < 5:
        sys.exit("USAGE: ./prog LOOP_REPORT_FILE SRC_FILE NEW_SRC_FILE Q_SIZE")

    JSON_REPORT_FNAME = sys.argv[1]
    SRC_FNAME = sys.argv[2]
    NEW_SRC_FILENAME = sys.argv[3]
    Q_SIZE = sys.argv[4]
    
    # For the muxes to have not fmaxz bottleneck, the II should be equal to number of stores.
    # However, there is no benefot to MUX_II < LSQ_II
    MUX_II = Q_SIZE

    with open(SRC_FNAME, 'r') as f:
        src_string = f.read()
    src_lines = src_string.splitlines()
    
    report = parse_report(JSON_REPORT_FNAME)
    # Keep track of kernel boundaries (and adjust their values if necessary).
    kernel_start_line = report["kernel_start_line"]
    kernel_end_line = report["kernel_end_line"]

    src_lines = add_array_ivdep(src_lines, kernel_start_line, kernel_end_line, report)

    kernel_body = get_kernel_body(src_lines, kernel_start_line, kernel_end_line)

    Q_NAME = get_queue_name(src_lines[kernel_start_line-1])

    # Generate pipes given the report.
    # There is one pipe object per cluster in each list.
    # It is up to the pipe object to hold info on the actual amount of pipes
    # (this is done using a CYL PipeArray), and the amount of read/write ops.
    ld_req_pipes, st_req_pipes, ld_val_pipes, st_val_pipes, lsq_signal_pipes, \
        mux_streq_signal_pipes, mux_stval_signal_pipes, mux_streq_out_pipes, mux_stval_out_pipes = gen_pipes(report)
    all_pipes = ld_req_pipes + st_req_pipes + ld_val_pipes + st_val_pipes + lsq_signal_pipes + \
        mux_streq_signal_pipes + mux_stval_signal_pipes + mux_streq_out_pipes + mux_stval_out_pipes

    # Make it easier for the later LLVM transformation to find the required pipe
    # ops and the associated instruction.
    add_pipe_names_to_report(
        report, ld_req_pipes, ld_val_pipes, st_req_pipes, st_val_pipes, lsq_signal_pipes, mux_streq_signal_pipes, mux_stval_signal_pipes)

    # Insert pipe type declarations into the src file.
    pipe_declarations = [p.declaration() for p in all_pipes] 
    src_with_pipe_decl = insert_before_line(src_lines, kernel_start_line, pipe_declarations)
    kernel_start_line += len(pipe_declarations)
    kernel_end_line += len(pipe_declarations)

    # Insert call to LSQ kernels right after the original kernel call.
    mux_calls = gen_mux_kernel_calls(report, Q_NAME, MUX_II)
    lsq_calls = gen_lsq_kernel_calls(report, Q_NAME, Q_SIZE)
    src_with_pipe_decl_and_lsq_calls = insert_after_line(
        src_with_pipe_decl, kernel_end_line, mux_calls + lsq_calls)

    # Generate load value and store value pipes.
    val_pipe_ops = []
    for i_base, base_addr in enumerate(report['base_addresses']):
        val_pipe_ops.append(ld_val_pipes[i_base].read_op())
        val_pipe_ops.append(st_val_pipes[i_base].write_op())
        if base_addr['insert_st_mux']:
            val_pipe_ops.append(mux_stval_signal_pipes[i_base].write_op())

    # Generate LSQ reqeust pipes and end signal pipes.
    lsq_pipe_ops = []
    # The AGU kernel name is forward declared to avoid mangling.
    agu_kernel_declarations = []
    for i_base, base_addr in enumerate(report['base_addresses']):
        this_base = []
        this_base.append(ld_req_pipes[i_base].write_op()) 
        this_base.append(st_req_pipes[i_base].write_op())
        this_base.append(lsq_signal_pipes[i_base].write_op())
        if base_addr['insert_st_mux']:
            this_base.append(mux_streq_signal_pipes[i_base].write_op())

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
    all_combined = get_src(LSQ_FILE) + get_src(MUX_FILE) + \
        agu_kernel_declarations + combined_original_agu_pipes

    # The end result is a transformed NEW_SRC_FILENAME and 
    # the original json report with added pipe_name: instruction mappings.
    with open(NEW_SRC_FILENAME, 'w') as f:
        f.write('\n'.join(all_combined))
    with open(JSON_REPORT_FNAME, 'w') as f:
        json.dump(report, f, indent=4, sort_keys=True)
