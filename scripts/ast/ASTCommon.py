#!/usr/bin/env python3

import re
import json

C_VAR_REGEX = r'([a-zA-Z_][a-zA-Z0-9_]*)'

# This regexes is guaranteed to be run on a line "... = {QUEUE}.single_task<..."
# i.e. there will be a space before {QUEUE}
QUEUE_NAME_REGEX = r"\s+" + C_VAR_REGEX + r"\.single_task<"

class SyclPipe:
    def __init__(self, name, ctype, amount=1, depth=None, pipe_array=False):
        # using {name} = pipe<class {class_name}, {ctype}>
        self.name = name                    # The short 'using' name.
        self.class_name = name + "_class"   # The actual class name.
        self.ctype = ctype
        self.depth = depth
        self.pipe_array = pipe_array
        self.amount = amount

    
    def declaration(self):
        depth_part = '' if self.depth is None else f',{self.depth}'
        if self.pipe_array:
            clamped_amount = max(2, self.amount) # A pipe array needs at 2 pipes.
            return f'using {self.name} = PipeArray<class {self.class_name}, {self.ctype} {depth_part}, {clamped_amount}>;'
        else: 
            return f'using {self.name} = pipe<class {self.class_name}, {self.ctype} {depth_part}>;'

    def write_op(self):
        val = '{}'

        res = ''
        if self.pipe_array: 
            for idx in range(self.amount):
                res += f'{self.name}::PipeAt<{idx}>::write({val});\n'
        else: 
            res += f'{self.name}::write({val});\n'

        return res

    def read_op(self):
        res = ''
        if self.pipe_array:
            for idx in range(self.amount):
                res += f'auto _rd_val_{self.name}_{idx} = {self.name}::PipeAt<{idx}>::read();\n'
        else:
            res += f'auto _rd_val_{self.name} = {self.name}::read();\n'

        return res
    
    def __str__(self):
        return self.name


def gen_kernel_copy(q_name, kernel_body, kernel_copy_name):
    kernel_body_str = "\n".join(kernel_body)
    return f'''{q_name}.single_task<{kernel_copy_name}>([=]() [[intel::kernel_args_restrict]] {{
            {kernel_body_str}
        }});\n'''.splitlines()

def insert_before_line(src_lines, line_num, new_lines):
    # Remember lines start at 1; python lists start at 0
    return src_lines[:line_num-1] + new_lines + src_lines[line_num-1:] 

def insert_after_line(src_lines, line_num, new_lines):
    # Remember lines start at 1; python lists start at 0
    return src_lines[:line_num] + new_lines + src_lines[line_num:] 

def get_kernel_body(src_lines, start_line, end_line):
    return src_lines[start_line : end_line-1]

def get_queue_name(line):
    m = re.findall(QUEUE_NAME_REGEX, line)
    if m:
        return m[0]

    exit("ERROR getting queue name.")

# Point before the first call to q.submit
def get_line_of_pattern(src_lines, re_pattern):
    """Line of last pattern ocurrence."""
    insert_line = -1
    for i, line in enumerate(src_lines):
        if re.findall(re_pattern, line):
            insert_line = i

    return insert_line

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

        if "kernel_name_full" not in report:
            exit("Analysis report is empty.")

        # report['blocks_to_decouple'] = list(reversed(report['blocks_to_decouple']))

        # Keep only the useful bits.
        report["kernel_name"] = report["kernel_name_full"].split(' ')[-1].split('::')[-1]

        return report
    except Exception as e:
        exit("Error parsing analysis report " + report_fname)
