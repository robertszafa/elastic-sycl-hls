#!/usr/bin/env python3

import re
import json

C_VAR_REGEX = r'([a-zA-Z_][a-zA-Z0-9_]*)'

# This regexes is guaranteed to be run on a line "... = {QUEUE}.single_task<..."
# i.e. there will be a space before {QUEUE}
QUEUE_NAME_REGEX = r"\s+" + C_VAR_REGEX + r"\.single_task<"

class SyclPipe:
    def __init__(self, name, ctype, amount=None, depth=None, write_repeat=None, write_value=None):
        # using {name} = pipe<class {class_name}, {ctype}>
        self.name = name                    # The short name.
        self.class_name = name + "_class"   # The actual class name.
        self.ctype = ctype
        self.amount = amount
        self.depth = depth
        self.write_repeat = 1 if write_repeat is None else write_repeat
        self.write_value = write_value
    
    def declaration(self):
        if self.amount is not None and self.depth is not None:
            return f'using {self.name} = PipeArray<class {self.class_name}, {self.ctype}, {self.depth}, {self.amount}>;'
        elif self.amount is not None:
            return f'using {self.name} = PipeArray<class {self.class_name}, {self.ctype}, {self.amount}>;'
        elif self.depth is not None:
            return f'using {self.name} = pipe<class {self.class_name}, {self.ctype}, {self.depth}>;'
        else: # no amount and no depth specified
            return f'using {self.name} = pipe<class {self.class_name}, {self.ctype}>;'

    def write_op(self):
        value = self.write_value if self.write_value is not None else '{}'

        res = ''
        if self.amount is not None: # Array of pipes
            for idx in range(self.amount):
                res += f'{self.name}::PipeAt<{idx}>::write({value});\n'
        else: 
            for _ in range(self.write_repeat):
                res += f'{self.name}::write({value});\n'

        return res

    def read_op(self):
        if self.amount is not None: # Array of pipes
            res = ''
            for idx in range(self.amount):
                res += f'auto _rd_val_{self.name}_{idx} = {self.name}::PipeAt<{idx}>::read();\n'
            return res

        return f'auto _rd_val_{self.name} = {self.name}::read();\n'
    
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
