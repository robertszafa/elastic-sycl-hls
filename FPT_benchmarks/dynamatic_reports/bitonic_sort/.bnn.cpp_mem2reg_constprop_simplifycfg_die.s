	.text
	.file	"bitonic_sort.cpp"
	.globl	_Z3bitonic_sortPiS_S_S_S_S_      # -- Begin function _Z3bitonic_sortPiS_S_S_S_S_
	.p2align	4, 0x90
	.type	_Z3bitonic_sortPiS_S_S_S_S_,@function
_Z3bitonic_sortPiS_S_S_S_S_:                     # @_Z3bitonic_sortPiS_S_S_S_S_
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	.cfi_offset %rbx, -24
	xorl	%eax, %eax
	movq	%r8, -16(%rbp)          # 8-byte Spill
	movq	%rcx, -24(%rbp)         # 8-byte Spill
	movq	%rdx, -32(%rbp)         # 8-byte Spill
	movq	%rsi, -40(%rbp)         # 8-byte Spill
	movq	%rdi, -48(%rbp)         # 8-byte Spill
	movq	%r9, -56(%rbp)          # 8-byte Spill
	movl	%eax, -60(%rbp)         # 4-byte Spill
	jmp	.LBB0_1
.LBB0_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_2 Depth 2
	movl	-60(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	%eax, -64(%rbp)         # 4-byte Spill
	movl	%ecx, -68(%rbp)         # 4-byte Spill
.LBB0_2:                                # %for.body3
                                        #   Parent Loop BB0_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-68(%rbp), %eax         # 4-byte Reload
	movl	-64(%rbp), %ecx         # 4-byte Reload
	imull	$100, %ecx, %edx
	addl	%eax, %edx
	movl	%edx, %esi
	movl	%esi, %edi
	movq	-40(%rbp), %r8          # 8-byte Reload
	movl	(%r8,%rdi,4), %esi
	movl	%edx, %r9d
	movl	%r9d, %edi
	movq	-48(%rbp), %r10         # 8-byte Reload
	xorl	(%r10,%rdi,4), %esi
	shll	$1, %esi
	movl	%edx, %edx
	movl	%edx, %edi
	movq	-24(%rbp), %r11         # 8-byte Reload
	movslq	(%r11,%rdi,4), %rdi
	movq	-56(%rbp), %rbx         # 8-byte Reload
	addl	(%rbx,%rdi,4), %esi
	movl	%esi, (%rbx,%rdi,4)
	addl	$1, %eax
	cmpl	$100, %eax
	movl	%eax, -68(%rbp)         # 4-byte Spill
	jb	.LBB0_2
# %bb.3:                                # %for.inc12
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-64(%rbp), %eax         # 4-byte Reload
	addl	$1, %eax
	cmpl	$100, %eax
	movl	%eax, %ecx
	movl	%eax, -72(%rbp)         # 4-byte Spill
	movl	%ecx, -60(%rbp)         # 4-byte Spill
	jb	.LBB0_1
# %bb.4:                                # %for.end14
	xorl	%eax, %eax
	movl	%eax, -76(%rbp)         # 4-byte Spill
	jmp	.LBB0_5
.LBB0_5:                                # %for.body17
                                        # =>This Inner Loop Header: Depth=1
	movl	-76(%rbp), %eax         # 4-byte Reload
	xorl	%ecx, %ecx
	movl	-64(%rbp), %edx         # 4-byte Reload
	imull	$100, %edx, %esi
	addl	%eax, %esi
	movl	%esi, %edi
	movl	%edi, %r8d
	movq	-32(%rbp), %r9          # 8-byte Reload
	movl	(%r9,%r8,4), %edi
	movl	%esi, %r10d
	movl	%r10d, %r8d
	movq	-16(%rbp), %r11         # 8-byte Reload
	movslq	(%r11,%r8,4), %r8
	movq	-56(%rbp), %rbx         # 8-byte Reload
	movl	(%rbx,%r8,4), %r10d
	subl	%edi, %ecx
	cmpl	$0, %r10d
	cmovgl	%ecx, %edi
	addl	%edi, %r10d
	movl	%esi, %ecx
	movl	%ecx, %r8d
	movslq	(%r11,%r8,4), %r8
	movl	%r10d, (%rbx,%r8,4)
	addl	$1, %eax
	cmpl	$100, %eax
	movl	%eax, -76(%rbp)         # 4-byte Spill
	jb	.LBB0_5
# %bb.6:                                # %for.end35
	movl	-72(%rbp), %eax         # 4-byte Reload
	popq	%rbx
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z3bitonic_sortPiS_S_S_S_S_, .Lfunc_end0-_Z3bitonic_sortPiS_S_S_S_S_
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
	subq	$240016, %rsp           # imm = 0x3A990
	movl	$13, %edi
	callq	srand
.LBB1_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_2 Depth 2
	xorl	%eax, %eax
	movl	%eax, -240004(%rbp)     # 4-byte Spill
	jmp	.LBB1_2
.LBB1_2:                                # %for.body3
                                        #   Parent Loop BB1_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-240004(%rbp), %eax     # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movl	%eax, -40000(%rbp,%rdx,4)
	movl	%eax, %ecx
	movl	%ecx, %edx
	movl	%eax, -80000(%rbp,%rdx,4)
	movl	%eax, -240008(%rbp)     # 4-byte Spill
	callq	rand
	movl	$100, %ecx
	cltd
	idivl	%ecx
	movl	-240008(%rbp), %ecx     # 4-byte Reload
	movl	%ecx, %esi
	movl	%esi, %edi
	movl	%edx, -120000(%rbp,%rdi,4)
	movl	%ecx, %edx
	movl	%edx, %edi
	movl	$7, -160000(%rbp,%rdi,4)
	movl	%ecx, %edx
	movl	%edx, %edi
	movl	$5, -200000(%rbp,%rdi,4)
	movl	%ecx, %edx
	movl	%edx, %edi
	movl	$3, -240000(%rbp,%rdi,4)
	addl	$1, %ecx
	cmpl	$10000, %ecx            # imm = 0x2710
	movl	%ecx, -240004(%rbp)     # 4-byte Spill
	jb	.LBB1_2
# %bb.3:                                # %for.inc26
                                        #   in Loop: Header=BB1_1 Depth=1
	xorl	%eax, %eax
	movb	%al, %cl
	testb	$1, %cl
	jne	.LBB1_1
	jmp	.LBB1_4
.LBB1_4:                                # %for.end28
	leaq	-120000(%rbp), %r9
	leaq	-80000(%rbp), %r8
	leaq	-40000(%rbp), %rcx
	leaq	-240000(%rbp), %rdx
	leaq	-200000(%rbp), %rsi
	leaq	-160000(%rbp), %rdi
	callq	_Z3bitonic_sortPiS_S_S_S_S_
	xorl	%r10d, %r10d
	movl	%eax, -240012(%rbp)     # 4-byte Spill
	movl	%r10d, %eax
	addq	$240016, %rsp           # imm = 0x3A990
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
