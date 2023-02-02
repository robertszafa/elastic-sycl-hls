#!/usr/bin/env python3

import re
import json

# TODO: get queue name from llvm pass
Q_NAME = 'q'

# This has false positives but we use it only on strings that have a variable name at the beginning
C_VAR_REGEX = r'([a-zA-Z_][a-zA-Z0-9_]*)'

class SyclPipe:
    def __init__(self, name, ctype, amount=None, depth=None, write_repeat=None, write_value=None):
        self.name = name
        self.ctype = ctype
        self.amount = amount
        self.depth = depth
        self.write_repeat = write_repeat
        self.write_value = write_value
    
    def declaration(self):
        if self.amount is not None and self.depth is not None:
            return f'using {self.name} = PipeArray<class {self.name}_class, {self.ctype}, {self.depth}, {self.amount}>;'
        elif self.amount is not None:
            return f'using {self.name} = PipeArray<class {self.name}_class, {self.ctype}, {self.amount}>;'
        elif self.depth is not None:
            return f'using {self.name} = pipe<class {self.name}_class, {self.ctype}, {self.depth}>;'
        else: # no amount and no depth specified
            return f'using {self.name} = pipe<class {self.name}_class, {self.ctype}>;'

    def write_op(self):
        value = self.write_value if self.write_value is not None else f'{self.ctype}{{}}'

        res = ''
        if self.amount is not None: # Array of pipes
            for idx in range(self.amount):
                res += f'{self.name}::PipeAt<{idx}>::write({value});\n'
        elif self.write_repeat is not None:
            for _ in range(self.write_repeat):
                res += f'{self.name}::write({value});\n'
        else: 
            res = f'{self.name}::write({value});\n'

        return res

    def read_op(self):
        if self.amount is not None: # Array of pipes
            res = ''
            for idx in range(self.amount):
                res += f'auto _ld_res_{self.name}_{idx} = {self.name}::PipeAt<{idx}>::read();\n'
            return res

        return f'auto _ld_res_{self.name} = {self.name}::read();\n'
    
    def __str__(self):
        return self.name


def gen_kernel_copy(src_lines, kernel_name, copy_name):
    kernel_body = get_kernel_body(src_lines, kernel_name)
    return f'{Q_NAME}.single_task<{copy_name}>{"".join(kernel_body)}'.splitlines()

def add_pipe_ops(src_lines, read_pipes, write_pipes):
    read_ops = [p.read_op() for p in read_pipes]
    write_ops = [p.write_op() for p in write_pipes]
    line_inside_kernel = get_qsubmit_line(src_lines) + 1
    return insert_after_line(src_lines, line_inside_kernel, read_ops + write_ops)
    
def add_pipe_declarations(src_lines, pipes):
    declarations = [p.declaration() for p in pipes]
    return insert_before_line(src_lines, get_qsubmit_line(src_lines), declarations)

def insert_before_line(src_lines, line_num, new_lines):
    return src_lines[:line_num] + new_lines +src_lines[line_num:] 

def insert_after_line(src_lines, line_num, new_lines):
    return src_lines[:line_num+1] + new_lines +src_lines[line_num+1:] 

def get_kernel_body(src_lines, kernel_name):
    src_string = "\n".join(src_lines)
    body = ""
    m = re.findall(r'<\s*(?:class\s+)?' + kernel_name +
                   r'\s*>\s*(\(.*?}\s*\)\s*;)', src_string, re.DOTALL)
    if m:
        body = m[0]
    else:
        exit("Failed match kernel body.")

    return body

def get_qsubmit_line(src_lines):
    return get_line_of_pattern(src_lines, Q_NAME + r'\.submit')

# Point before the first call to q.submit
def get_line_of_pattern(src_lines, re_pattern):
    """Line of last pattern ocurrence."""
    insert_line = -1
    for i, line in enumerate(src_lines):
        if re.findall(re_pattern, line):
            insert_line = i

    return insert_line

def get_array_name(line_with_array, end_col):
    array = ""
    m = re.findall(C_VAR_REGEX, str(line_with_array[:end_col]))
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

        # Keep only the useful bits.
        report["kernel_name"] = report["kernel_class_name"].split(' ')[-1].split('::')[-1]
        report['spir_func_name'] = report["spir_func_name"].split('::')[0]

        return report
    except Exception as e:
        exit("Error parsing analysis report " + report_fname)
