	.text
	.file	"bitonic_sort.cpp"
	.globl	_Z12bitonic_sortPi      # -- Begin function _Z12bitonic_sortPi
	.p2align	4, 0x90
	.type	_Z12bitonic_sortPi,@function
_Z12bitonic_sortPi:                     # @_Z12bitonic_sortPi
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	$2, %eax
	movq	%rdi, -8(%rbp)          # 8-byte Spill
	movl	%eax, -12(%rbp)         # 4-byte Spill
	jmp	.LBB0_1
.LBB0_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_3 Depth 2
                                        #       Child Loop BB0_4 Depth 3
	movl	-12(%rbp), %eax         # 4-byte Reload
	cmpl	$0, %eax
	movl	%eax, -16(%rbp)         # 4-byte Spill
	jle	.LBB0_9
# %bb.2:                                # %for.body3.lr.ph
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-16(%rbp), %eax         # 4-byte Reload
	movl	%eax, -20(%rbp)         # 4-byte Spill
	jmp	.LBB0_3
.LBB0_3:                                # %for.body3
                                        #   Parent Loop BB0_1 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB0_4 Depth 3
	movl	-20(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%eax, %edx
	sarl	$1, %edx
	movl	%eax, -24(%rbp)         # 4-byte Spill
	movl	%edx, -28(%rbp)         # 4-byte Spill
	movl	%ecx, -32(%rbp)         # 4-byte Spill
.LBB0_4:                                # %for.body6
                                        #   Parent Loop BB0_1 Depth=1
                                        #     Parent Loop BB0_3 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movl	-32(%rbp), %eax         # 4-byte Reload
	movl	%eax, %ecx
	movl	-28(%rbp), %edx         # 4-byte Reload
	xorl	%edx, %ecx
	movl	%eax, %esi
	movl	%esi, %edi
	movq	-8(%rbp), %r8           # 8-byte Reload
	movl	(%r8,%rdi,4), %esi
	movslq	%ecx, %rdi
	movl	(%r8,%rdi,4), %r9d
	cmpl	%eax, %ecx
	movl	%eax, -36(%rbp)         # 4-byte Spill
	movl	%ecx, -40(%rbp)         # 4-byte Spill
	movl	%esi, -44(%rbp)         # 4-byte Spill
	movl	%r9d, -48(%rbp)         # 4-byte Spill
	jle	.LBB0_7
# %bb.5:                                # %if.then
                                        #   in Loop: Header=BB0_4 Depth=3
	movl	-44(%rbp), %eax         # 4-byte Reload
	movl	-48(%rbp), %ecx         # 4-byte Reload
	subl	%ecx, %eax
	setg	%dl
	setl	%sil
	movl	-36(%rbp), %edi         # 4-byte Reload
	movl	-16(%rbp), %r8d         # 4-byte Reload
	testl	%r8d, %edi
	movl	%eax, -52(%rbp)         # 4-byte Spill
	movb	%sil, -53(%rbp)         # 1-byte Spill
	movb	%dl, -54(%rbp)          # 1-byte Spill
	je	.LBB0_12
# %bb.11:                               # %if.then
                                        #   in Loop: Header=BB0_4 Depth=3
	movb	-53(%rbp), %al          # 1-byte Reload
	movb	%al, -54(%rbp)          # 1-byte Spill
.LBB0_12:                               # %if.then
                                        #   in Loop: Header=BB0_4 Depth=3
	movb	-54(%rbp), %al          # 1-byte Reload
	testb	$1, %al
	jne	.LBB0_6
	jmp	.LBB0_7
.LBB0_6:                                # %if.then19
                                        #   in Loop: Header=BB0_4 Depth=3
	movl	-36(%rbp), %eax         # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movq	-8(%rbp), %rsi          # 8-byte Reload
	movl	-48(%rbp), %ecx         # 4-byte Reload
	movl	%ecx, (%rsi,%rdx,4)
	movl	-40(%rbp), %edi         # 4-byte Reload
	movslq	%edi, %rdx
	movl	-44(%rbp), %r8d         # 4-byte Reload
	movl	%r8d, (%rsi,%rdx,4)
.LBB0_7:                                # %for.inc
                                        #   in Loop: Header=BB0_4 Depth=3
	movl	-36(%rbp), %eax         # 4-byte Reload
	addl	$1, %eax
	cmpl	$64, %eax
	movl	%eax, -32(%rbp)         # 4-byte Spill
	jb	.LBB0_4
# %bb.8:                                # %for.inc26
                                        #   in Loop: Header=BB0_3 Depth=2
	movl	-24(%rbp), %eax         # 4-byte Reload
	cmpl	$3, %eax
	movl	-28(%rbp), %ecx         # 4-byte Reload
	movl	%ecx, -20(%rbp)         # 4-byte Spill
	jg	.LBB0_3
.LBB0_9:                                # %for.inc29
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-16(%rbp), %eax         # 4-byte Reload
	shll	$1, %eax
	cmpl	$65, %eax
	movl	%eax, %ecx
	movl	%eax, -60(%rbp)         # 4-byte Spill
	movl	%ecx, -12(%rbp)         # 4-byte Spill
	jl	.LBB0_1
# %bb.10:                               # %for.end30
	movl	-60(%rbp), %eax         # 4-byte Reload
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z12bitonic_sortPi, .Lfunc_end0-_Z12bitonic_sortPi
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
	subq	$272, %rsp              # imm = 0x110
	movl	$13, %edi
	callq	srand
.LBB1_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_2 Depth 2
	xorl	%eax, %eax
	movl	%eax, -260(%rbp)        # 4-byte Spill
	jmp	.LBB1_2
.LBB1_2:                                # %for.body3
                                        #   Parent Loop BB1_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-260(%rbp), %eax        # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movl	%eax, -256(%rbp,%rdx,4)
	addl	$1, %eax
	cmpl	$64, %eax
	movl	%eax, -260(%rbp)        # 4-byte Spill
	jb	.LBB1_2
# %bb.3:                                # %for.inc6
                                        #   in Loop: Header=BB1_1 Depth=1
	xorl	%eax, %eax
	movb	%al, %cl
	testb	$1, %cl
	jne	.LBB1_1
	jmp	.LBB1_4
.LBB1_4:                                # %for.end8
	leaq	-256(%rbp), %rdi
	callq	_Z12bitonic_sortPi
	xorl	%ecx, %ecx
	movl	%eax, -264(%rbp)        # 4-byte Spill
	movl	%ecx, %eax
	addq	$272, %rsp              # imm = 0x110
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
