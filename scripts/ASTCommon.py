#!/usr/bin/env python3

import re
import json

# TODO: get queue name from llvm pass
Q_NAME = 'q'

# This has false positives but we use it only on strings that have a variable name at the beginning
C_VAR_REGEX = r'([a-zA-Z_][a-zA-Z0-9_]*)'


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
        report["kernel_name"] = report["kernel_class_name"].split(' ')[-1].split('::')[-1]
        report['spir_func_name'] = report["spir_func_name"].split('::')[0]
        
        if report["base_addresses"]:
            for base_addr in report["base_addresses"]:
                base_addr['array_type'] = llvm2ctype(base_addr['array_type'])

        return report
    except Exception as e:
        exit("Error parsing analysis report " + report_fname)
