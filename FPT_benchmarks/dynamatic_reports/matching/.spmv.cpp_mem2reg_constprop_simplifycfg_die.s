	.text
	.file	"spmv.cpp"
	.globl	_Z4spmvPA20_iiPiS1_     # -- Begin function _Z4spmvPA20_iiPiS1_
	.p2align	4, 0x90
	.type	_Z4spmvPA20_iiPiS1_,@function
_Z4spmvPA20_iiPiS1_:                    # @_Z4spmvPA20_iiPiS1_
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	$1, %eax
	movq	%rdx, -8(%rbp)          # 8-byte Spill
	movl	%esi, -12(%rbp)         # 4-byte Spill
	movq	%rdi, -24(%rbp)         # 8-byte Spill
	movq	%rcx, -32(%rbp)         # 8-byte Spill
	movl	%eax, -36(%rbp)         # 4-byte Spill
	jmp	.LBB0_1
.LBB0_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_2 Depth 2
	movl	-36(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%eax, -40(%rbp)         # 4-byte Spill
	movl	%ecx, -44(%rbp)         # 4-byte Spill
.LBB0_2:                                # %for.body3
                                        #   Parent Loop BB0_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-44(%rbp), %eax         # 4-byte Reload
	movl	-40(%rbp), %ecx         # 4-byte Reload
	movl	%ecx, %edx
	movl	%edx, %esi
	movl	%eax, %edx
	movl	%edx, %edi
	movq	-32(%rbp), %r8          # 8-byte Reload
	movslq	(%r8,%rdi,4), %rdi
	imulq	$80, %rsi, %rsi
	movq	-24(%rbp), %r9          # 8-byte Reload
	addq	%rsi, %r9
	shlq	$2, %rdi
	addq	%rdi, %r9
	movl	-12(%rbp), %edx         # 4-byte Reload
	imull	(%r9), %edx
	movl	%ecx, %r10d
	movl	%r10d, %esi
	movl	%eax, %r10d
	movl	%r10d, %edi
	movq	-8(%rbp), %r9           # 8-byte Reload
	movslq	(%r9,%rdi,4), %rdi
	imulq	$80, %rsi, %rsi
	movq	-24(%rbp), %r11         # 8-byte Reload
	addq	%rsi, %r11
	shlq	$2, %rdi
	addq	%rdi, %r11
	addl	(%r11), %edx
	movl	%edx, (%r11)
	addl	$1, %eax
	cmpl	$20, %eax
	movl	%eax, -44(%rbp)         # 4-byte Spill
	jb	.LBB0_2
# %bb.3:                                # %for.inc14
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-40(%rbp), %eax         # 4-byte Reload
	addl	$1, %eax
	cmpl	$20, %eax
	movl	%eax, %ecx
	movl	%eax, -48(%rbp)         # 4-byte Spill
	movl	%ecx, -36(%rbp)         # 4-byte Spill
	jb	.LBB0_1
# %bb.4:                                # %for.end16
	movl	-48(%rbp), %eax         # 4-byte Reload
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z4spmvPA20_iiPiS1_, .Lfunc_end0-_Z4spmvPA20_iiPiS1_
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
	subq	$1808, %rsp             # imm = 0x710
.LBB1_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_2 Depth 2
                                        #       Child Loop BB1_3 Depth 3
	callq	rand
	xorl	%ecx, %ecx
	movl	%eax, -4(%rbp)
	movl	%ecx, -1780(%rbp)       # 4-byte Spill
.LBB1_2:                                # %for.body3
                                        #   Parent Loop BB1_1 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB1_3 Depth 3
	movl	-1780(%rbp), %eax       # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%eax, -1784(%rbp)       # 4-byte Spill
	movl	%ecx, -1788(%rbp)       # 4-byte Spill
.LBB1_3:                                # %for.body6
                                        #   Parent Loop BB1_1 Depth=1
                                        #     Parent Loop BB1_2 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movl	-1788(%rbp), %eax       # 4-byte Reload
	movl	%eax, -1792(%rbp)       # 4-byte Spill
	callq	rand
	leaq	-1776(%rbp), %rcx
	movl	$100, %edx
	movl	%edx, -1796(%rbp)       # 4-byte Spill
	cltd
	movl	-1796(%rbp), %esi       # 4-byte Reload
	idivl	%esi
	movl	-1784(%rbp), %edi       # 4-byte Reload
	movl	%edi, %r8d
	movl	%r8d, %r9d
	movl	-1792(%rbp), %r8d       # 4-byte Reload
	movl	%r8d, %r10d
	movl	%r10d, %r11d
	imulq	$80, %r9, %r9
	addq	%r9, %rcx
	shlq	$2, %r11
	addq	%r11, %rcx
	movl	%edx, (%rcx)
	addl	$1, %r8d
	cmpl	$20, %r8d
	movl	%r8d, -1788(%rbp)       # 4-byte Spill
	jb	.LBB1_3
# %bb.4:                                # %for.end
                                        #   in Loop: Header=BB1_2 Depth=2
	movl	$20, %eax
	movl	-1784(%rbp), %ecx       # 4-byte Reload
	addl	$1, %ecx
	movl	%eax, -1800(%rbp)       # 4-byte Spill
	movl	%ecx, %eax
	xorl	%edx, %edx
	movl	-1800(%rbp), %ecx       # 4-byte Reload
	divl	%ecx
	movl	-1784(%rbp), %esi       # 4-byte Reload
	movl	%esi, %edi
	movl	%edi, %r8d
	movl	%edx, -96(%rbp,%r8,4)
	movl	%esi, %eax
	xorl	%edx, %edx
	divl	%ecx
	movl	%esi, %edi
	movl	%edi, %r8d
	movl	%edx, -176(%rbp,%r8,4)
	addl	$1, %esi
	cmpl	$20, %esi
	movl	%esi, -1780(%rbp)       # 4-byte Spill
	jb	.LBB1_2
# %bb.5:                                # %for.inc28
                                        #   in Loop: Header=BB1_1 Depth=1
	xorl	%eax, %eax
	movb	%al, %cl
	testb	$1, %cl
	jne	.LBB1_1
	jmp	.LBB1_6
.LBB1_6:                                # %for.end30
	leaq	-176(%rbp), %rcx
	leaq	-96(%rbp), %rdx
	leaq	-1776(%rbp), %rdi
	movl	-4(%rbp), %esi
	callq	_Z4spmvPA20_iiPiS1_
	xorl	%esi, %esi
	movl	%eax, -1804(%rbp)       # 4-byte Spill
	movl	%esi, %eax
	addq	$1808, %rsp             # imm = 0x710
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
