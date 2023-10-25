	.text
	.file	"matching.cpp"
	.globl	_Z8matchingPiS_         # -- Begin function _Z8matchingPiS_
	.p2align	4, 0x90
	.type	_Z8matchingPiS_,@function
_Z8matchingPiS_:                        # @_Z8matchingPiS_
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movl	%eax, %ecx
	movq	%rdi, -8(%rbp)          # 8-byte Spill
	movq	%rsi, -16(%rbp)         # 8-byte Spill
	movl	%ecx, -20(%rbp)         # 4-byte Spill
	movl	%eax, -24(%rbp)         # 4-byte Spill
	jmp	.LBB0_1
.LBB0_1:                                # %while.body
                                        # =>This Inner Loop Header: Depth=1
	movl	-24(%rbp), %eax         # 4-byte Reload
	movl	-20(%rbp), %ecx         # 4-byte Reload
	movl	%eax, %edx
	shll	$1, %edx
	movl	%edx, %esi
	movl	%esi, %edi
	movq	-16(%rbp), %r8          # 8-byte Reload
	movl	(%r8,%rdi,4), %esi
	orl	$1, %edx
	movl	%edx, %edx
	movl	%edx, %edi
	movl	(%r8,%rdi,4), %edx
	movslq	%esi, %rdi
	movq	-8(%rbp), %r9           # 8-byte Reload
	movl	(%r9,%rdi,4), %r10d
	movslq	%edx, %rdi
	orl	(%r9,%rdi,4), %r10d
	cmpl	$0, %r10d
	movl	%ecx, %r10d
	movl	%eax, -28(%rbp)         # 4-byte Spill
	movl	%ecx, -32(%rbp)         # 4-byte Spill
	movl	%esi, -36(%rbp)         # 4-byte Spill
	movl	%edx, -40(%rbp)         # 4-byte Spill
	movl	%r10d, -44(%rbp)        # 4-byte Spill
	jne	.LBB0_3
# %bb.2:                                # %if.then
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-36(%rbp), %eax         # 4-byte Reload
	movslq	%eax, %rcx
	movq	-8(%rbp), %rdx          # 8-byte Reload
	movl	$1, (%rdx,%rcx,4)
	movl	-40(%rbp), %esi         # 4-byte Reload
	movslq	%esi, %rcx
	movl	$1, (%rdx,%rcx,4)
	movl	-32(%rbp), %edi         # 4-byte Reload
	addl	$1, %edi
	movl	%edi, -44(%rbp)         # 4-byte Spill
.LBB0_3:                                # %if.end
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-44(%rbp), %eax         # 4-byte Reload
	movl	-28(%rbp), %ecx         # 4-byte Reload
	addl	$1, %ecx
	cmpl	$1000, %ecx             # imm = 0x3E8
	movl	%eax, %edx
	movl	%eax, -48(%rbp)         # 4-byte Spill
	movl	%edx, -20(%rbp)         # 4-byte Spill
	movl	%ecx, -24(%rbp)         # 4-byte Spill
	jb	.LBB0_1
# %bb.4:                                # %while.end
	movl	-48(%rbp), %eax         # 4-byte Reload
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z8matchingPiS_, .Lfunc_end0-_Z8matchingPiS_
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
	subq	$16016, %rsp            # imm = 0x3E90
.LBB1_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_2 Depth 2
	xorl	%eax, %eax
	movl	%eax, -16004(%rbp)      # 4-byte Spill
	jmp	.LBB1_2
.LBB1_2:                                # %for.body3
                                        #   Parent Loop BB1_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-16004(%rbp), %eax      # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movl	%eax, -8000(%rbp,%rdx,4)
	movl	%eax, %ecx
	movl	%ecx, %edx
	movl	$0, -16000(%rbp,%rdx,4)
	addl	$1, %eax
	cmpl	$2000, %eax             # imm = 0x7D0
	movl	%eax, -16004(%rbp)      # 4-byte Spill
	jb	.LBB1_2
# %bb.3:                                # %for.inc10
                                        #   in Loop: Header=BB1_1 Depth=1
	xorl	%eax, %eax
	movb	%al, %cl
	testb	$1, %cl
	jne	.LBB1_1
	jmp	.LBB1_4
.LBB1_4:                                # %for.end12
	leaq	-8000(%rbp), %rsi
	leaq	-16000(%rbp), %rdi
	callq	_Z8matchingPiS_
	xorl	%ecx, %ecx
	movl	%eax, -16008(%rbp)      # 4-byte Spill
	movl	%ecx, %eax
	addq	$16016, %rsp            # imm = 0x3E90
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
