	.text
	.file	"getTanh_double.cpp"
	.globl	_Z14getTanh_doublePiS_  # -- Begin function _Z14getTanh_doublePiS_
	.p2align	4, 0x90
	.type	_Z14getTanh_doublePiS_,@function
_Z14getTanh_doublePiS_:                 # @_Z14getTanh_doublePiS_
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movq	%rdi, -8(%rbp)          # 8-byte Spill
	movq	%rsi, -16(%rbp)         # 8-byte Spill
	movl	%eax, -20(%rbp)         # 4-byte Spill
	jmp	.LBB0_1
.LBB0_1:                                # %for.body
                                        # =>This Inner Loop Header: Depth=1
	movl	-20(%rbp), %eax         # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movq	-16(%rbp), %rsi         # 8-byte Reload
	movl	(%rsi,%rdx,4), %ecx
	movslq	%ecx, %rdx
	movq	-8(%rbp), %rdi          # 8-byte Reload
	movl	(%rdi,%rdx,4), %r8d
	movl	%r8d, %r9d
	imull	%r9d, %r9d
	addl	$19, %r9d
	imull	%r8d, %r9d
	imull	%r8d, %r9d
	addl	$3, %r9d
	imull	%r8d, %r9d
	movslq	%ecx, %rdx
	movl	%r9d, (%rdi,%rdx,4)
	addl	$1, %eax
	cmpl	$1000, %eax             # imm = 0x3E8
	movl	%eax, %ecx
	movl	%eax, -24(%rbp)         # 4-byte Spill
	movl	%ecx, -20(%rbp)         # 4-byte Spill
	jb	.LBB0_1
# %bb.2:                                # %for.end
	movl	-24(%rbp), %eax         # 4-byte Reload
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z14getTanh_doublePiS_, .Lfunc_end0-_Z14getTanh_doublePiS_
	.cfi_endproc
                                        # -- End function
	.globl	main                    # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$8016, %rsp             # imm = 0x1F50
.LBB1_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_2 Depth 2
	xorl	%eax, %eax
	movl	%eax, -8004(%rbp)       # 4-byte Spill
	jmp	.LBB1_2
.LBB1_2:                                # %for.body3
                                        #   Parent Loop BB1_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-8004(%rbp), %eax       # 4-byte Reload
	movl	$100, %ecx
	movl	%eax, %edx
	movl	%edx, %esi
	movl	%eax, -4000(%rbp,%rsi,4)
	movl	%eax, %edx
	movl	%edx, %esi
	movl	$2, -8000(%rbp,%rsi,4)
	movl	%eax, -8008(%rbp)       # 4-byte Spill
	xorl	%edx, %edx
	divl	%ecx
	cmpl	$0, %edx
	jne	.LBB1_4
# %bb.3:                                # %if.then
                                        #   in Loop: Header=BB1_2 Depth=2
	movl	-8008(%rbp), %eax       # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movl	$0, -4000(%rbp,%rdx,4)
.LBB1_4:                                # %for.inc
                                        #   in Loop: Header=BB1_2 Depth=2
	movl	-8008(%rbp), %eax       # 4-byte Reload
	addl	$1, %eax
	cmpl	$1000, %eax             # imm = 0x3E8
	movl	%eax, -8004(%rbp)       # 4-byte Spill
	jb	.LBB1_2
# %bb.5:                                # %for.inc15
                                        #   in Loop: Header=BB1_1 Depth=1
	xorl	%eax, %eax
	movb	%al, %cl
	testb	$1, %cl
	jne	.LBB1_1
	jmp	.LBB1_6
.LBB1_6:                                # %for.end17
	leaq	-8000(%rbp), %rsi
	leaq	-4000(%rbp), %rdi
	callq	_Z14getTanh_doublePiS_
	xorl	%ecx, %ecx
	movl	%eax, -8012(%rbp)       # 4-byte Spill
	movl	%ecx, %eax
	addq	$8016, %rsp             # imm = 0x1F50
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
