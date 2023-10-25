	.text
	.file	"histogramIf.cpp"
	.globl	_Z11histogramIfPiS_S_i  # -- Begin function _Z11histogramIfPiS_S_i
	.p2align	4, 0x90
	.type	_Z11histogramIfPiS_S_i,@function
_Z11histogramIfPiS_S_i:                 # @_Z11histogramIfPiS_S_i
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	cmpl	$0, %ecx
	movq	%rdx, -8(%rbp)          # 8-byte Spill
	movq	%rsi, -16(%rbp)         # 8-byte Spill
	movq	%rdi, -24(%rbp)         # 8-byte Spill
	movl	%ecx, -28(%rbp)         # 4-byte Spill
	movl	%eax, -32(%rbp)         # 4-byte Spill
	jle	.LBB0_5
# %bb.1:                                # %for.body.lr.ph
	xorl	%eax, %eax
	movl	%eax, -36(%rbp)         # 4-byte Spill
	jmp	.LBB0_2
.LBB0_2:                                # %for.body
                                        # =>This Inner Loop Header: Depth=1
	movl	-36(%rbp), %eax         # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movq	-24(%rbp), %rsi         # 8-byte Reload
	movl	(%rsi,%rdx,4), %ecx
	movslq	%ecx, %rdx
	movq	-8(%rbp), %rdi          # 8-byte Reload
	movl	(%rdi,%rdx,4), %r8d
	cmpl	$0, %r8d
	movl	%eax, -40(%rbp)         # 4-byte Spill
	movl	%ecx, -44(%rbp)         # 4-byte Spill
	movl	%r8d, -48(%rbp)         # 4-byte Spill
	jle	.LBB0_4
# %bb.3:                                # %if.then
                                        #   in Loop: Header=BB0_2 Depth=1
	movl	-40(%rbp), %eax         # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movl	-48(%rbp), %ecx         # 4-byte Reload
	movq	-16(%rbp), %rsi         # 8-byte Reload
	addl	(%rsi,%rdx,4), %ecx
	movl	-44(%rbp), %edi         # 4-byte Reload
	movslq	%edi, %rdx
	movq	-8(%rbp), %r8           # 8-byte Reload
	movl	%ecx, (%r8,%rdx,4)
.LBB0_4:                                # %for.inc
                                        #   in Loop: Header=BB0_2 Depth=1
	movl	-40(%rbp), %eax         # 4-byte Reload
	addl	$1, %eax
	movl	-28(%rbp), %ecx         # 4-byte Reload
	cmpl	%ecx, %eax
	movl	%eax, %edx
	movl	%edx, -36(%rbp)         # 4-byte Spill
	movl	%eax, -32(%rbp)         # 4-byte Spill
	jl	.LBB0_2
.LBB0_5:                                # %for.end
	movl	-32(%rbp), %eax         # 4-byte Reload
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z11histogramIfPiS_S_i, .Lfunc_end0-_Z11histogramIfPiS_S_i
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
	subq	$12016, %rsp            # imm = 0x2EF0
	movl	$13, %edi
	callq	srand
.LBB1_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_2 Depth 2
	xorl	%eax, %eax
	movl	$1000, -12004(%rbp)     # imm = 0x3E8
	movl	%eax, -12008(%rbp)      # 4-byte Spill
.LBB1_2:                                # %for.body3
                                        #   Parent Loop BB1_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-12008(%rbp), %eax      # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movl	$1, -4000(%rbp,%rdx,4)
	movl	%eax, -12012(%rbp)      # 4-byte Spill
	callq	rand
	movl	$100, %ecx
	cltd
	idivl	%ecx
	movl	-12012(%rbp), %ecx      # 4-byte Reload
	movl	%ecx, %esi
	movl	%esi, %edi
	movl	%edx, -8000(%rbp,%rdi,4)
	movl	%ecx, %edx
	movl	%edx, %edi
	movl	$1, -12000(%rbp,%rdi,4)
	addl	$1, %ecx
	cmpl	$1000, %ecx             # imm = 0x3E8
	movl	%ecx, -12008(%rbp)      # 4-byte Spill
	jb	.LBB1_2
# %bb.3:                                # %for.inc16
                                        #   in Loop: Header=BB1_1 Depth=1
	xorl	%eax, %eax
	movb	%al, %cl
	testb	$1, %cl
	jne	.LBB1_1
	jmp	.LBB1_4
.LBB1_4:                                # %for.end18
	leaq	-12000(%rbp), %rdx
	leaq	-8000(%rbp), %rsi
	leaq	-4000(%rbp), %rdi
	movl	-12004(%rbp), %ecx
	callq	_Z11histogramIfPiS_S_i
	xorl	%ecx, %ecx
	movl	%eax, -12016(%rbp)      # 4-byte Spill
	movl	%ecx, %eax
	addq	$12016, %rsp            # imm = 0x2EF0
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
