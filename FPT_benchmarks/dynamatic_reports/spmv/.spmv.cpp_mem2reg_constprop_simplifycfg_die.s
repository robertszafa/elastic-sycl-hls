	.text
	.file	"spmv.cpp"
	.globl	_Z4spmvPiiS_S_          # -- Begin function _Z4spmvPiiS_S_
	.p2align	4, 0x90
	.type	_Z4spmvPiiS_S_,@function
_Z4spmvPiiS_S_:                         # @_Z4spmvPiiS_S_
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	xorl	%eax, %eax
	movl	$1, %r8d
	movq	%rdx, -8(%rbp)          # 8-byte Spill
	movl	%esi, -12(%rbp)         # 4-byte Spill
	movq	%rdi, -24(%rbp)         # 8-byte Spill
	movq	%rcx, -32(%rbp)         # 8-byte Spill
	movl	%eax, -36(%rbp)         # 4-byte Spill
	movl	%r8d, -40(%rbp)         # 4-byte Spill
	jmp	.LBB0_1
.LBB0_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_2 Depth 2
	movl	-40(%rbp), %eax         # 4-byte Reload
	movl	-36(%rbp), %ecx         # 4-byte Reload
	xorl	%edx, %edx
	movl	%eax, -44(%rbp)         # 4-byte Spill
	movl	%edx, -48(%rbp)         # 4-byte Spill
	movl	%ecx, -52(%rbp)         # 4-byte Spill
.LBB0_2:                                # %for.body3
                                        #   Parent Loop BB0_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-52(%rbp), %eax         # 4-byte Reload
	movl	-48(%rbp), %ecx         # 4-byte Reload
	movslq	%eax, %rdx
	movq	-32(%rbp), %rsi         # 8-byte Reload
	movslq	(%rsi,%rdx,4), %rdx
	movl	-12(%rbp), %edi         # 4-byte Reload
	movq	-24(%rbp), %r8          # 8-byte Reload
	imull	(%r8,%rdx,4), %edi
	movslq	%eax, %rdx
	movq	-8(%rbp), %r9           # 8-byte Reload
	movslq	(%r9,%rdx,4), %rdx
	addl	(%r8,%rdx,4), %edi
	movl	%edi, (%r8,%rdx,4)
	addl	$1, %eax
	addl	$1, %ecx
	cmpl	$20, %ecx
	movl	%eax, %edi
	movl	%eax, -56(%rbp)         # 4-byte Spill
	movl	%ecx, -48(%rbp)         # 4-byte Spill
	movl	%edi, -52(%rbp)         # 4-byte Spill
	jb	.LBB0_2
# %bb.3:                                # %for.inc11
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-44(%rbp), %eax         # 4-byte Reload
	addl	$1, %eax
	cmpl	$20, %eax
	movl	-56(%rbp), %ecx         # 4-byte Reload
	movl	%eax, %edx
	movl	%eax, -60(%rbp)         # 4-byte Spill
	movl	%ecx, -36(%rbp)         # 4-byte Spill
	movl	%edx, -40(%rbp)         # 4-byte Spill
	jb	.LBB0_1
# %bb.4:                                # %for.end13
	movl	-60(%rbp), %eax         # 4-byte Reload
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z4spmvPiiS_S_, .Lfunc_end0-_Z4spmvPiiS_S_
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
	pushq	%rbx
	subq	$4872, %rsp             # imm = 0x1308
	.cfi_offset %rbx, -24
	xorl	%eax, %eax
	movl	%eax, -4820(%rbp)       # 4-byte Spill
.LBB1_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_2 Depth 2
                                        #       Child Loop BB1_3 Depth 3
	movl	-4820(%rbp), %eax       # 4-byte Reload
	movl	%eax, -4824(%rbp)       # 4-byte Spill
	callq	rand
	xorl	%ecx, %ecx
	movl	%eax, -12(%rbp)
	movl	%ecx, %eax
	movl	%eax, -4828(%rbp)       # 4-byte Spill
	movl	%ecx, -4832(%rbp)       # 4-byte Spill
.LBB1_2:                                # %for.body3
                                        #   Parent Loop BB1_1 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB1_3 Depth 3
	movl	-4832(%rbp), %eax       # 4-byte Reload
	movl	-4828(%rbp), %ecx       # 4-byte Reload
	xorl	%edx, %edx
	movl	%eax, -4836(%rbp)       # 4-byte Spill
	movl	%edx, -4840(%rbp)       # 4-byte Spill
	movl	%ecx, -4844(%rbp)       # 4-byte Spill
.LBB1_3:                                # %for.body6
                                        #   Parent Loop BB1_1 Depth=1
                                        #     Parent Loop BB1_2 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movl	-4844(%rbp), %eax       # 4-byte Reload
	movl	-4840(%rbp), %ecx       # 4-byte Reload
	movl	%eax, -4848(%rbp)       # 4-byte Spill
	movl	%ecx, -4852(%rbp)       # 4-byte Spill
	callq	rand
	leaq	-3216(%rbp), %rdx
	movl	$3, %ecx
	leaq	-1616(%rbp), %rsi
	leaq	-4816(%rbp), %rdi
	movl	$100, %r8d
	movq	%rdx, -4864(%rbp)       # 8-byte Spill
	cltd
	idivl	%r8d
	movl	-4824(%rbp), %r8d       # 4-byte Reload
	movl	%r8d, %r9d
	movl	%r9d, %r10d
	movl	-4848(%rbp), %r9d       # 4-byte Reload
	movslq	%r9d, %r11
	imulq	$1600, %r10, %r10       # imm = 0x640
	addq	%r10, %rdi
	shlq	$2, %r11
	addq	%r11, %rdi
	movl	%edx, (%rdi)
	movl	%r9d, %eax
	cltd
	idivl	%ecx
	movl	%r8d, %ebx
	movl	%ebx, %edi
	movslq	%r9d, %r10
	imulq	$1600, %rdi, %rdi       # imm = 0x640
	addq	%rdi, %rsi
	shlq	$2, %r10
	addq	%r10, %rsi
	movl	%edx, (%rsi)
	addl	$1, %r9d
	movl	%r9d, %eax
	cltd
	idivl	%ecx
	movl	%r8d, %ecx
	movl	%ecx, %esi
	movl	-4848(%rbp), %ecx       # 4-byte Reload
	movslq	%ecx, %rdi
	imulq	$1600, %rsi, %rsi       # imm = 0x640
	movq	-4864(%rbp), %r10       # 8-byte Reload
	addq	%rsi, %r10
	shlq	$2, %rdi
	addq	%rdi, %r10
	movl	%edx, (%r10)
	addl	$1, %ecx
	movl	-4852(%rbp), %edx       # 4-byte Reload
	addl	$1, %edx
	cmpl	$20, %edx
	movl	%ecx, %r9d
	movl	%ecx, -4868(%rbp)       # 4-byte Spill
	movl	%edx, -4840(%rbp)       # 4-byte Spill
	movl	%r9d, -4844(%rbp)       # 4-byte Spill
	jb	.LBB1_3
# %bb.4:                                # %for.inc23
                                        #   in Loop: Header=BB1_2 Depth=2
	movl	-4836(%rbp), %eax       # 4-byte Reload
	addl	$1, %eax
	cmpl	$20, %eax
	movl	-4868(%rbp), %ecx       # 4-byte Reload
	movl	%ecx, -4828(%rbp)       # 4-byte Spill
	movl	%eax, -4832(%rbp)       # 4-byte Spill
	jb	.LBB1_2
# %bb.5:                                # %for.inc26
                                        #   in Loop: Header=BB1_1 Depth=1
	xorl	%eax, %eax
	movb	%al, %cl
	movl	-4824(%rbp), %eax       # 4-byte Reload
	addl	$1, %eax
	testb	$1, %cl
	movl	%eax, -4820(%rbp)       # 4-byte Spill
	jne	.LBB1_1
# %bb.6:                                # %for.end28
	leaq	-3216(%rbp), %rcx
	leaq	-1616(%rbp), %rdx
	leaq	-4816(%rbp), %rdi
	movl	-12(%rbp), %esi
	callq	_Z4spmvPiiS_S_
	xorl	%esi, %esi
	movl	%eax, -4872(%rbp)       # 4-byte Spill
	movl	%esi, %eax
	addq	$4872, %rsp             # imm = 0x1308
	popq	%rbx
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
