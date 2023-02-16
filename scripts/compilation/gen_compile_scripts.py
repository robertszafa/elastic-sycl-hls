#!/usr/bin/env python3

"""
This script uses the dpcpp compiler with verbose flags to compile an example code.
Based on the stdout of that compiler, a generic compile_to_bc and compile_from_bc
scripts are constructed.

compile_to_bc   - compiles a sycl source file to LLVM IR.
compile_from_bc - compiles an LLVM IR file to a final binary. 
                  The final binary can target an emulator, simulator or hardware.
"""

import argparse
import re
import os
import subprocess

GIT_DIR = os.environ['LLVM_SYCL_PASSES_DIR']
COMPILE_TO_BC_FNAME = f"{GIT_DIR}/scripts/compilation/compile_to_bc.sh"
COMPILE_FROM_BC_FNAME = f"{GIT_DIR}/scripts/compilation/compile_from_bc.sh"

EXAMPLE_DIR = f"{GIT_DIR}/scripts/compilation/example"
EXAMPLE_FNAME = f"example.cpp"

TARGETS = "emu, sim, hw"


def has_vsim():
    out = subprocess.getoutput("which vsim")
    return out != "" and "no vsim" not in out

def get_commands_list(stdout, is_yielding=False, up_to=''):
    """A command starts with a double quote"""
    is_double_quote_start = lambda line : line.lstrip()[0] == '"'
    res = list(filter(is_double_quote_start, stdout.split("\n")))

    if is_yielding and up_to:
        up_to_i = len(res) - 1
        for i, line in enumerate(res):
            if up_to in line:
                up_to_i = i
        return res[:up_to_i+1]

    return res

def get_bc_fname(to_bc_cmd):
    m = re.findall(r"\S+\.bc", to_bc_cmd)[0]
    if '=' in m:
        return m.split('=')[1]
    return m

def remove_verbose_flags(cmd):
    cmd = re.sub(r'-v\s+', ' ', cmd)
    return re.sub(r'--verbose\s+', ' ', cmd)

def get_ld_cmd():
    err, stdout = subprocess.getstatusoutput(f"make fpga_emu FILE=example.cpp")
    if err != 0:
        exit("Error: failed emu target. Emulation should always be possible.")
    commands = get_commands_list(stdout)
    os.system(f'make clean > /dev/null 2>&1')
    return commands[-1]

def get_bin_llvm_dir(to_bc_cmd):
    return re.findall(r'\s*(\S+)\s', to_bc_cmd)[0]

def add_linker_cmds(bin_llvm_dir, aocx_fname, table_fname, host_obj, ld_cmd):
    result = f"\n{bin_llvm_dir}/file-table-tform -replace=Code,Code -o /tmp/example-3f9847.table {table_fname} {aocx_fname}"
    result += f"\n{bin_llvm_dir}/clang-offload-wrapper -o=/tmp/wrapper-ab389f.bc -host=x86_64-unknown-linux-gnu -target=spir64_fpga -kind=sycl -batch /tmp/example-3f9847.table"
    result += f"\n{bin_llvm_dir}/llc -filetype=obj -o /tmp/example-554ec6.o /tmp/wrapper-ab389f.bc"

    ld_cmd = re.sub(r' -o \S+', ' -o "$4"', ld_cmd)
    ld_cmd = re.sub(fr'/\S+/example-\S+\.o /\S+/example-\S+\.o', f'{host_obj} /tmp/example-554ec6.o', ld_cmd)
    result += f"\n{ld_cmd}"

    return result

if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog="gen_compile_scripts.py",
                                     description="Generate compile to/from bc scripts.",
                                     formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument("--board", help="optional path and name to the fpga board package, e.g.: /opt/intel/oneapi/intel_a10gx_pac:pac_a10")
    parser.add_argument("--targets", type=str, default=TARGETS, help="Comma seperated list of targets (emu, sim or hw). If not provided, all targets are enabled.")
    parser.add_argument("--slow", action='store_true', help="Run the full hw/sim compilation.")
    args = parser.parse_args()

    # This is what we start with
    COMPILE_TO_BC='''#!/bin/bash

    # $1 - emu / sim / hw
    # $2 - filename to compile\n
    '''
    COMPILE_FROM_BC='''#!/bin/bash

    # $1 - emu / sim / hw
    # $2 - bc file
    # $3 - original filename
    # $4 - output binary filename\n
    '''

    TARGET_LIST = args.targets.split(",")

    if "sim" in TARGET_LIST and not has_vsim():
        print("Warning: sim target removed, vsim not in PATH.")
        TARGET_LIST.remove("vsim")

    # Change to the Makefile directory, and ensure all rules fire.
    os.chdir(EXAMPLE_DIR)
    os.system(f'make clean > /dev/null 2>&1')

    ld_cmd = get_ld_cmd()

    print('Creating compilation scripts...')
    for target in TARGET_LIST:
        print(f'\tTarget {target}')

        make_cmd = f"make fpga_{target} FILE=example.cpp"

        is_yielding = not args.slow and (target == 'sim' or target == 'hw')
        if is_yielding:
            TIME_OUT = "10s"
            make_cmd = f"timeout {TIME_OUT} {make_cmd} || echo 0"
        
        err, stdout = subprocess.getstatusoutput(make_cmd)

        if err != 0:
            print(f"Warning: {target} target failed.")
            continue

        # If is yielding, then ignore commands after creating aocx file.
        commands = get_commands_list(stdout, is_yielding, up_to='--out-ext=aocx')
        to_bc_cmd = commands[0]
        from_bc_cmd = "\n".join(commands[1:])

        to_bc_cmd = re.sub(EXAMPLE_FNAME, '"$2"', to_bc_cmd)
        to_bc_cmd = re.sub(get_bc_fname(to_bc_cmd), '"$2.bc"', to_bc_cmd)
        to_bc_cmd = remove_verbose_flags(to_bc_cmd)

        integration_head = re.findall(r'-fsycl-int-header=(\S+) ', to_bc_cmd)[0]
        mk_dir_tmp = f'mkdir -p $(dirname {integration_head})'

        COMPILE_TO_BC += f'\nif [ "$1" == "{target}" ]; then\n\t{mk_dir_tmp}\n{to_bc_cmd}\nexit\nfi\n'

        from_bc_cmd = re.sub(get_bc_fname(from_bc_cmd), '"$2"', from_bc_cmd)
        from_bc_cmd = re.sub(r"bin/example\.fpga_" + target, '"$4"', from_bc_cmd)
        from_bc_cmd = re.sub(r"bin/example\.dev\.o", '"$4".dev.o', from_bc_cmd)
        from_bc_cmd = re.sub(EXAMPLE_FNAME + r'[^\.]', '"$3" ', from_bc_cmd)
        from_bc_cmd = remove_verbose_flags(from_bc_cmd)

        if is_yielding: # Need to add the final linker command if we stopped before that
            bin_llvm_dir = re.findall(r'\s*"(\S+)"', to_bc_cmd)[0].split("clang++")[0]
            aocx_fname = re.findall(r'=(\S+\.aocx)', from_bc_cmd)[0]
            table_fname = re.findall(r'\s(\S+\.table)', from_bc_cmd)[0]

            # Assumption: in clang-offload-bundler's outputs the host_obj comes first
            host_obj_outputs = re.findall(r'dev\.o -outputs=(\S+\.o),', from_bc_cmd)
            host_obj_output = re.findall(r'dev\.o -output=(\S+\.o) ', from_bc_cmd)
            if len(host_obj_outputs) > 0:
                host_obj = host_obj_outputs[0]
            else:
                host_obj = host_obj_output[0]

            from_bc_cmd += add_linker_cmds(bin_llvm_dir, aocx_fname, table_fname, host_obj, ld_cmd)

        COMPILE_FROM_BC += f'\nif [ "$1" == "{target}" ]; then\n\t{from_bc_cmd}\nexit\nfi\n'

    COMPILE_TO_BC += "\nprintf 'Warning: No target matched.'"
    COMPILE_FROM_BC += "\nprintf 'Warning: No target matched.'"
        
    print(f'Saving compilation scripts...\n\t{COMPILE_TO_BC_FNAME}\n\t{COMPILE_FROM_BC_FNAME}')
    with open(COMPILE_TO_BC_FNAME, 'w') as f:
        f.write(COMPILE_TO_BC)
    with open(COMPILE_FROM_BC_FNAME, 'w') as f:
        f.write(COMPILE_FROM_BC)

    os.system(f"chmod +x {COMPILE_TO_BC_FNAME} {COMPILE_FROM_BC_FNAME}")    
    os.system(f'rm /tmp/example-* -rf')    
