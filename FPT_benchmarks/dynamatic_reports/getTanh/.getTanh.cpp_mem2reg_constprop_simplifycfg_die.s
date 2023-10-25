	.text
	.file	"getTanh.cpp"
	.globl	_Z7getTanhPiS_          # -- Begin function _Z7getTanhPiS_
	.p2align	4, 0x90
	.type	_Z7getTanhPiS_,@function
_Z7getTanhPiS_:                         # @_Z7getTanhPiS_
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
	movq	%rdi, -16(%rbp)         # 8-byte Spill
	movq	%rsi, -24(%rbp)         # 8-byte Spill
	movl	%eax, -28(%rbp)         # 4-byte Spill
	jmp	.LBB0_1
.LBB0_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_2 Depth 2
	movl	-28(%rbp), %eax         # 4-byte Reload
	movl	$1, %ecx
	movl	$4945, %edx             # imm = 0x1351
	movl	$4, %esi
	movl	$6, %edi
	movl	$5, %r8d
	movl	%eax, %r9d
	movl	%r9d, %r10d
	movq	-24(%rbp), %r11         # 8-byte Reload
	movslq	(%r11,%r10,4), %r10
	movq	-16(%rbp), %rbx         # 8-byte Reload
	movl	(%rbx,%r10,4), %r9d
	cmpl	$4095, %r9d             # imm = 0xFFF
	cmovgl	%r8d, %edi
	cmpl	$8191, %r9d             # imm = 0x1FFF
	cmovgl	%esi, %edi
	movl	%edi, %esi
	shll	$12, %esi
	subl	%esi, %r9d
	movl	%ecx, %esi
	movl	%eax, -32(%rbp)         # 4-byte Spill
	movl	%edx, -36(%rbp)         # 4-byte Spill
	movl	%r9d, -40(%rbp)         # 4-byte Spill
	movl	%edi, -44(%rbp)         # 4-byte Spill
	movl	%esi, -48(%rbp)         # 4-byte Spill
	movl	%ecx, -52(%rbp)         # 4-byte Spill
.LBB0_2:                                # %for.body18
                                        #   Parent Loop BB0_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-52(%rbp), %eax         # 4-byte Reload
	movl	-36(%rbp), %ecx         # 4-byte Reload
	movl	-40(%rbp), %edx         # 4-byte Reload
	movl	-48(%rbp), %esi         # 4-byte Reload
	movl	%ecx, -56(%rbp)         # 4-byte Spill
	movl	%esi, %ecx
                                        # kill: def %cl killed %ecx
	movl	%eax, %edi
	sarl	%cl, %edi
	movl	-56(%rbp), %r8d         # 4-byte Reload
	subl	%edi, %r8d
	movl	%esi, %ecx
                                        # kill: def %cl killed %ecx
	movl	-56(%rbp), %edi         # 4-byte Reload
	sarl	%cl, %edi
	subl	%edi, %eax
	movl	-44(%rbp), %edi         # 4-byte Reload
	addl	%edi, %edx
	movl	%esi, %ecx
                                        # kill: def %cl killed %ecx
	movl	%eax, %r9d
	sarl	%cl, %r9d
	movl	%r8d, %r10d
	subl	%r9d, %r10d
	movl	%esi, %ecx
                                        # kill: def %cl killed %ecx
	sarl	%cl, %r8d
	subl	%r8d, %eax
	addl	%edi, %edx
	addl	$1, %esi
	cmpl	$13, %esi
	movl	%r10d, %r8d
	movl	%eax, %r9d
	movl	%eax, -60(%rbp)         # 4-byte Spill
	movl	%esi, -48(%rbp)         # 4-byte Spill
	movl	%r10d, -64(%rbp)        # 4-byte Spill
	movl	%edx, -40(%rbp)         # 4-byte Spill
	movl	%r8d, -36(%rbp)         # 4-byte Spill
	movl	%r9d, -52(%rbp)         # 4-byte Spill
	jb	.LBB0_2
# %bb.3:                                # %for.end
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	-64(%rbp), %eax         # 4-byte Reload
	movl	-60(%rbp), %ecx         # 4-byte Reload
	addl	%ecx, %eax
	movl	-44(%rbp), %edx         # 4-byte Reload
	imull	%eax, %edx
	movl	-64(%rbp), %eax         # 4-byte Reload
	addl	%ecx, %eax
	movl	-44(%rbp), %esi         # 4-byte Reload
	imull	%eax, %esi
	sarl	$12, %esi
	orl	$1, %edx
	orl	$1, %esi
	imull	%esi, %edx
	movl	-32(%rbp), %eax         # 4-byte Reload
	movl	%eax, %esi
	movl	%esi, %edi
	movq	-24(%rbp), %r8          # 8-byte Reload
	movslq	(%r8,%rdi,4), %rdi
	movq	-16(%rbp), %r9          # 8-byte Reload
	movl	%edx, (%r9,%rdi,4)
	addl	$1, %eax
	cmpl	$1000, %eax             # imm = 0x3E8
	movl	%edx, -68(%rbp)         # 4-byte Spill
	movl	%eax, -28(%rbp)         # 4-byte Spill
	jb	.LBB0_1
# %bb.4:                                # %for.end42
	movl	-68(%rbp), %eax         # 4-byte Reload
	popq	%rbx
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z7getTanhPiS_, .Lfunc_end0-_Z7getTanhPiS_
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
	movl	%eax, %ecx
	movl	%ecx, %edx
	movl	%eax, -4000(%rbp,%rdx,4)
	movl	%eax, %ecx
	movl	%ecx, %edx
	movl	%eax, -8000(%rbp,%rdx,4)
	movl	%eax, %ecx
	movl	%ecx, %edx
	movl	$0, -4000(%rbp,%rdx,4)
	addl	$1, %eax
	cmpl	$1000, %eax             # imm = 0x3E8
	movl	%eax, -8004(%rbp)       # 4-byte Spill
	jb	.LBB1_2
# %bb.3:                                # %for.inc14
                                        #   in Loop: Header=BB1_1 Depth=1
	xorl	%eax, %eax
	movb	%al, %cl
	testb	$1, %cl
	jne	.LBB1_1
	jmp	.LBB1_4
.LBB1_4:                                # %for.end16
	leaq	-8000(%rbp), %rsi
	leaq	-4000(%rbp), %rdi
	callq	_Z7getTanhPiS_
	xorl	%ecx, %ecx
	movl	%eax, -8008(%rbp)       # 4-byte Spill
	movl	%ecx, %eax
	addq	$8016, %rsp             # imm = 0x1F50
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	.L_ZZ4mainE5atanh,@object # @_ZZ4mainE5atanh
	.section	.rodata,"a",@progbits
	.p2align	4
.L_ZZ4mainE5atanh:
	.long	2249                    # 0x8c9
	.long	1046                    # 0x416
	.long	514                     # 0x202
	.long	256                     # 0x100
	.long	128                     # 0x80
	.long	100                     # 0x64
	.long	50                      # 0x32
	.long	16                      # 0x10
	.long	8                       # 0x8
	.long	4                       # 0x4
	.long	2                       # 0x2
	.long	1                       # 0x1
	.size	.L_ZZ4mainE5atanh, 48

	.type	.L_ZZ4mainE4cosh,@object # @_ZZ4mainE4cosh
	.p2align	4
.L_ZZ4mainE4cosh:
	.long	4096                    # 0x1000
	.long	6320                    # 0x18b0
	.long	15409                   # 0x3c31
	.long	41237                   # 0xa115
	.long	111854                  # 0x1b4ee
	.size	.L_ZZ4mainE4cosh, 20

	.type	.L_ZZ4mainE4sinh,@object # @_ZZ4mainE4sinh
	.p2align	4
.L_ZZ4mainE4sinh:
	.long	0                       # 0x0
	.long	4813                    # 0x12cd
	.long	14855                   # 0x3a07
	.long	41033                   # 0xa049
	.long	111779                  # 0x1b4a3
	.size	.L_ZZ4mainE4sinh, 20


	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
