	.text
	.file	"chaosNCG.cpp"
	.globl	_Z8chaosNCGPiS_iii      # -- Begin function _Z8chaosNCGPiS_iii
	.p2align	4, 0x90
	.type	_Z8chaosNCGPiS_iii,@function
_Z8chaosNCGPiS_iii:                     # @_Z8chaosNCGPiS_iii
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r14
	pushq	%rbx
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	xorl	%eax, %eax
	movl	%ecx, -20(%rbp)         # 4-byte Spill
	movl	%edx, -24(%rbp)         # 4-byte Spill
	movq	%rsi, -32(%rbp)         # 8-byte Spill
	movq	%rdi, -40(%rbp)         # 8-byte Spill
	movl	%r8d, -44(%rbp)         # 4-byte Spill
	movl	%eax, -48(%rbp)         # 4-byte Spill
	jmp	.LBB0_1
.LBB0_1:                                # %for.body
                                        # =>This Inner Loop Header: Depth=1
	movl	-48(%rbp), %eax         # 4-byte Reload
	movl	$16, %ecx
	movl	%eax, %edx
	movl	-24(%rbp), %esi         # 4-byte Reload
	addl	%esi, %edx
	addl	$2, %edx
	movslq	%edx, %rdi
	movq	-32(%rbp), %r8          # 8-byte Reload
	movl	(%r8,%rdi,4), %edx
	movl	%eax, %r9d
	addl	%esi, %r9d
	addl	$-2, %r9d
	movslq	%r9d, %rdi
	movl	(%r8,%rdi,4), %r9d
	movslq	%edx, %rdi
	movq	-40(%rbp), %r10         # 8-byte Reload
	movl	(%r10,%rdi,4), %edx
	movslq	%r9d, %rdi
	movl	%edx, %r9d
	xorl	(%r10,%rdi,4), %r9d
	movl	%r9d, %r11d
	andl	$15, %r11d
	movl	%ecx, -52(%rbp)         # 4-byte Spill
	movl	%r11d, %ecx
                                        # kill: def %cl killed %ecx
	movl	-44(%rbp), %r11d        # 4-byte Reload
	shll	%cl, %r11d
	movl	%r9d, %ebx
	andl	$15, %ebx
	movl	-52(%rbp), %r14d        # 4-byte Reload
	subl	%ebx, %r14d
	movl	%r14d, %ecx
                                        # kill: def %cl killed %ecx
	movl	-44(%rbp), %ebx         # 4-byte Reload
	sarl	%cl, %ebx
	orl	%ebx, %r11d
	xorl	%r11d, %r9d
	xorl	%r11d, %edx
	movl	%edx, %r11d
	andl	$15, %r11d
	movl	%r11d, %ecx
                                        # kill: def %cl killed %ecx
	movl	-20(%rbp), %r11d        # 4-byte Reload
	shll	%cl, %r11d
	movl	%edx, %ebx
	andl	$15, %ebx
	movl	-52(%rbp), %r14d        # 4-byte Reload
	subl	%ebx, %r14d
	movl	%r14d, %ecx
                                        # kill: def %cl killed %ecx
	movl	-20(%rbp), %ebx         # 4-byte Reload
	sarl	%cl, %ebx
	orl	%ebx, %r11d
	addl	%r11d, %edx
	movl	%eax, %r11d
	movl	%r11d, %edi
	movslq	(%r8,%rdi,4), %rdi
	movl	%r9d, (%r10,%rdi,4)
	movl	%eax, %r9d
	orl	$1, %r9d
	movl	%r9d, %r9d
	movl	%r9d, %edi
	movslq	(%r8,%rdi,4), %rdi
	movl	%edx, (%r10,%rdi,4)
	addl	$2, %eax
	cmpl	$2000, %eax             # imm = 0x7D0
	movl	%eax, %edx
	movl	%eax, -56(%rbp)         # 4-byte Spill
	movl	%edx, -48(%rbp)         # 4-byte Spill
	jb	.LBB0_1
# %bb.2:                                # %for.end
	movl	-56(%rbp), %eax         # 4-byte Reload
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z8chaosNCGPiS_iii, .Lfunc_end0-_Z8chaosNCGPiS_iii
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
	subq	$24016, %rsp            # imm = 0x5DD0
	movl	$13, %edi
	callq	srand
.LBB1_1:                                # %for.body
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_2 Depth 2
	xorl	%eax, %eax
	movl	%eax, -24004(%rbp)      # 4-byte Spill
	jmp	.LBB1_2
.LBB1_2:                                # %for.body3
                                        #   Parent Loop BB1_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	-24004(%rbp), %eax      # 4-byte Reload
	movl	%eax, %ecx
	movl	%ecx, %edx
	movl	%eax, -12000(%rbp,%rdx,4)
	movl	%eax, -24008(%rbp)      # 4-byte Spill
	callq	rand
	movl	$100, %ecx
	cltd
	idivl	%ecx
	movl	-24008(%rbp), %ecx      # 4-byte Reload
	movl	%ecx, %esi
	movl	%esi, %edi
	movl	%edx, -24000(%rbp,%rdi,4)
	addl	$1, %ecx
	cmpl	$3000, %ecx             # imm = 0xBB8
	movl	%ecx, -24004(%rbp)      # 4-byte Spill
	jb	.LBB1_2
# %bb.3:                                # %for.inc10
                                        #   in Loop: Header=BB1_1 Depth=1
	xorl	%eax, %eax
	movb	%al, %cl
	testb	$1, %cl
	jne	.LBB1_1
	jmp	.LBB1_4
.LBB1_4:                                # %for.end12
	movl	$2, %edx
	movl	$1, %eax
	leaq	-12000(%rbp), %rsi
	leaq	-24000(%rbp), %rdi
	movl	%eax, %ecx
	movl	%eax, %r8d
	callq	_Z8chaosNCGPiS_iii
	xorl	%ecx, %ecx
	movl	%eax, -24012(%rbp)      # 4-byte Spill
	movl	%ecx, %eax
	addq	$24016, %rsp            # imm = 0x5DD0
	popq	%rbp
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"
	.section	".note.GNU-stack","",@progbits
