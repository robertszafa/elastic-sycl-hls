	.text
	.file	"floydWarshall.cpp"
	.globl	_Z13floydWarshallPi     # -- Begin function _Z13floydWarshallPi
	.p2align	4, 0x90
	.type	_Z13floydWarshallPi,@function
_Z13floydWarshallPi:                    # @_Z13floydWarshallPi
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movq	%rdi, -8(%rbp)          # 8-byte Spill
	movl	%eax, -12(%rbp)         # 4-byte Spill
	jmp	.LBB0_1
.LBB0_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_2 Depth 2
                                        #       Child Loop BB0_3 Depth 3
	movl	-12(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%eax, -16(%rbp)         # 4-byte Spill
	movl	%ecx, -20(%rbp)         # 4-byte Spill
.LBB0_2:                                # %for.body3
                                        #   Parent Loop BB0_1 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB0_3 Depth 3
	movl	-20(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%eax, -24(%rbp)         # 4-byte Spill
	movl	%ecx, -28(%rbp)         # 4-byte Spill
.LBB0_3:                                # %for.body6
                                        #   Parent Loop BB0_1 Depth=1
                                        #     Parent Loop BB0_2 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movl	-28(%rbp), %eax         # 4-byte Reload
	movl	-24(%rbp), %ecx         # 4-byte Reload
	imull	$10, %ecx, %edx
	movl	%edx, %esi
	addl	%eax, %esi
	movl	%esi, %esi
	movl	%esi, %edi
	movq	-8(%rbp), %r8           # 8-byte Reload
	movl	(%r8,%rdi,4), %esi
	imull	$10, %ecx, %r9d
	movl	-16(%rbp), %r10d        # 4-byte Reload
	addl	%r10d, %r9d
	movl	%r9d, %r9d
	movl	%r9d, %edi
	movl	(%r8,%rdi,4), %r9d
	imull	$10, %r10d, %r11d
	addl	%eax, %r11d
	movl	%r11d, %r11d
	movl	%r11d, %edi
	addl	(%r8,%rdi,4), %r9d
	cmpl	%r9d, %esi
	movl	%eax, -32(%rbp)         # 4-byte Spill
	movl	%edx, -36(%rbp)         # 4-byte Spill
	jle	.LBB0_5
# %bb.4:                                # %if.then
                                        #   in Loop: Header=BB0_3 Depth=3
	movl	-36(%rbp), %eax         # 4-byte Reload
	movl	-16(%rbp), %ecx         # 4-byte Reload
	addl	%ecx, %eax
	movl	%eax, %eax
	movl	%eax, %edx
	movq	-8(%rbp), %rsi          # 8-byte Reload
	movl	(%rsi,%rdx,4), %eax
	imull	$10, %ecx, %edi
	movl	-32(%rbp), %r8d         # 4-byte Reload
	addl	%r8d, %edi
	movl	%edi, %edi
	movl	%edi, %edx
	addl	(%rsi,%rdx,4), %eax
	movl	-24(%rbp), %edi         # 4-byte Reload
	imull	$10, %edi, %r9d
	addl	%r8d, %r9d
	movl	%r9d, %r9d
	movl	%r9d, %edx
	movl	%eax, (%rsi,%rdx,4)
.LBB0_5:                                # %for.inc
                                        #   in Loop: Header=BB0_3 Depth=3
	movl	-32(%rbp), %eax         # 4-byte Reload
	addl	$1, %eax
	cmpl	$10, %eax
	movl	%eax, -28(%rbp)         # 4-byte Spill
	jb	.LBB0_3
# %bb.6:                                # %for.inc30
                                        #   in Loop: Header=BB0_2 Depth=2
	movl	-24(%rbp), %eax         # 4-byte Reload
	addl	$1, %eax
	cmpl	$10, %eax
	movl	%eax, -20(%rbp)         # 4-byte Spill
	jb	.LBB0_2
# %bb.7:                                # %for.inc33
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-16(%rbp), %eax         # 4-byte Reload
	addl	$1, %eax
	cmpl	$10, %eax
	movl	%eax, %ecx
	movl	%eax, -40(%rbp)         # 4-byte Spill
	movl	%ecx, -12(%rbp)         # 4-byte Spill
	jb	.LBB0_1
# %bb.8:                                # %for.end35
	movl	-40(%rbp), %eax         # 4-byte Reload
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z13floydWarshallPi, .Lfunc_end0-_Z13floydWarshallPi
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
	subq	$432, %rsp              # imm = 0x1B0
	movl	$13, %edi
	callq	srand
.LBB1_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_2 Depth 2
                                        #       Child Loop BB1_3 Depth 3
	xorl	%eax, %eax
	movl	%eax, -404(%rbp)        # 4-byte Spill
	jmp	.LBB1_2
.LBB1_2:                                # %for.body3
                                        #   Parent Loop BB1_1 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB1_3 Depth 3
	movl	-404(%rbp), %eax        # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%eax, -408(%rbp)        # 4-byte Spill
	movl	%ecx, -412(%rbp)        # 4-byte Spill
.LBB1_3:                                # %for.body6
                                        #   Parent Loop BB1_1 Depth=1
                                        #     Parent Loop BB1_2 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movl	-412(%rbp), %eax        # 4-byte Reload
	xorl	%ecx, %ecx
	movl	-408(%rbp), %edx        # 4-byte Reload
	cmpl	%eax, %edx
	movl	%eax, -416(%rbp)        # 4-byte Spill
	movl	%ecx, -420(%rbp)        # 4-byte Spill
	je	.LBB1_5
# %bb.4:                                # %cond.false
                                        #   in Loop: Header=BB1_3 Depth=3
	callq	rand
	movl	$10, %ecx
	cltd
	idivl	%ecx
	movl	%edx, -420(%rbp)        # 4-byte Spill
.LBB1_5:                                # %cond.end
                                        #   in Loop: Header=BB1_3 Depth=3
	movl	-420(%rbp), %eax        # 4-byte Reload
	movl	-408(%rbp), %ecx        # 4-byte Reload
	imull	$10, %ecx, %edx
	movl	-416(%rbp), %esi        # 4-byte Reload
	addl	%esi, %edx
	movl	%edx, %edx
	movl	%edx, %edi
	movl	%eax, -400(%rbp,%rdi,4)
	addl	$1, %esi
	cmpl	$10, %esi
	movl	%esi, -412(%rbp)        # 4-byte Spill
	jb	.LBB1_3
# %bb.6:                                # %for.end
                                        #   in Loop: Header=BB1_2 Depth=2
	callq	rand
	movl	-408(%rbp), %ecx        # 4-byte Reload
	addl	$1, %ecx
	cmpl	$10, %ecx
	movl	%eax, -424(%rbp)        # 4-byte Spill
	movl	%ecx, -404(%rbp)        # 4-byte Spill
	jb	.LBB1_2
# %bb.7:                                # %for.inc19
                                        #   in Loop: Header=BB1_1 Depth=1
	xorl	%eax, %eax
	movb	%al, %cl
	testb	$1, %cl
	jne	.LBB1_1
	jmp	.LBB1_8
.LBB1_8:                                # %for.end21
	leaq	-400(%rbp), %rdi
	callq	_Z13floydWarshallPi
	xorl	%ecx, %ecx
	movl	%eax, -428(%rbp)        # 4-byte Spill
	movl	%ecx, %eax
	addq	$432, %rsp              # imm = 0x1B0
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
