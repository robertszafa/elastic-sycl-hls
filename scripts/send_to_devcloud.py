#!/usr/bin/env python3

import os

from constants import *


if __name__ == '__main__':
    for qsize in [8]:
        for kernel in KERNELS:
            print(f'\n-- {kernel} : {qsize} QSIZE')

            report_file = f'{GIT_DIR}/inputs/{kernel}/loop-raw-report.json'
            src_file = f'{GIT_DIR}/inputs/{kernel}/{kernel}.cpp'

            src_file_tmp = f'{src_file}.tmp.cpp_{qsize}qsize'

            basename_src_file_tmp = src_file_tmp.split('/')[1]
            print(basename_src_file_tmp)
            integration_header = f'~/tmp/{basename_src_file_tmp}-integration-header.h'
            integration_footer = f'~/tmp/{basename_src_file_tmp}-integration-footer.h'

            os.system(f'{GIT_DIR}/scripts/compile_to_bc.sh sim {src_file}')
            os.system(f'{GIT_DIR}/scripts/prepare_ir.sh {src_file}.bc')
            os.system(f'''~/git/llvm/build/bin/opt -load-pass-plugin \
                        ~/git/llvm-sycl-passes/build/lib/libLoopRAWHazardReport.so \
                        -passes=loop-raw-report {src_file}.bc -o /dev/null > {report_file}''')
            os.system(f'./TransformAST.py {report_file} {src_file} {qsize} {src_file_tmp}')

            os.system(f'{GIT_DIR}/scripts/compile_to_bc.sh sim {src_file_tmp}')
            os.system(f'{GIT_DIR}/scripts/prepare_ir.sh {src_file_tmp}.bc')
            os.system(f'''export LOOP_RAW_REPORT={report_file} && ~/git/llvm/build/bin/opt \
                    -load-pass-plugin ~/git/llvm-sycl-passes/build/lib/libLoadStoreQueueTransform.so \
                    -passes=lsq-transform {src_file_tmp}.bc -o {src_file_tmp}.out.bc''')
            os.system(f'{GIT_DIR}/scripts/cleanup_ir.sh {src_file_tmp}.out.bc')

            os.system(
                f'scp {src_file_tmp} {src_file_tmp}.out.bc {DEVCLOUD_LOGIN}:~/git/llvm-sycl-passes/inputs/{kernel}')
            os.system(f'scp {integration_header} {integration_footer} {DEVCLOUD_LOGIN}:~/tmp')
