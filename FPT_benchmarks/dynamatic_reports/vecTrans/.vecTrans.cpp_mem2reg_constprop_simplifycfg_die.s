	.text
	.file	"vecTrans.cpp"
	.globl	_Z8vecTransPiS_         # -- Begin function _Z8vecTransPiS_
	.p2align	4, 0x90
	.type	_Z8vecTransPiS_,@function
_Z8vecTransPiS_:                        # @_Z8vecTransPiS_
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
	movq	-8(%rbp), %rsi          # 8-byte Reload
	movl	(%rsi,%rdx,4), %ecx
	movl	%ecx, %edi
	addl	$112, %edi
	imull	%ecx, %edi
	addl	$23, %edi
	imull	%ecx, %edi
	addl	$36, %edi
	imull	%ecx, %edi
	addl	$82, %edi
	imull	%ecx, %edi
	addl	$127, %edi
	imull	%ecx, %edi
	addl	$2, %edi
	imull	%ecx, %edi
	addl	$20, %edi
	imull	%ecx, %edi
	addl	$100, %edi
	movl	%eax, %ecx
	movl	%ecx, %edx
	movq	-16(%rbp), %r8          # 8-byte Reload
	movslq	(%r8,%rdx,4), %rdx
	movl	%edi, (%rsi,%rdx,4)
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
	.size	_Z8vecTransPiS_, .Lfunc_end0-_Z8vecTransPiS_
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
	movl	$1000, %edx             # imm = 0x3E8
	movl	%eax, %esi
	movl	%esi, %edi
	movl	%eax, -4000(%rbp,%rdi,4)
	movl	%eax, %esi
	addl	$1, %esi
	movl	%eax, -8008(%rbp)       # 4-byte Spill
	movl	%esi, %eax
	xorl	%esi, %esi
	movl	%edx, -8012(%rbp)       # 4-byte Spill
	movl	%esi, %edx
	movl	-8012(%rbp), %esi       # 4-byte Reload
	divl	%esi
	movl	-8008(%rbp), %r8d       # 4-byte Reload
	movl	%r8d, %r9d
	movl	%r9d, %edi
	movl	%edx, -8000(%rbp,%rdi,4)
	movl	%r8d, %eax
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
# %bb.5:                                # %for.inc16
                                        #   in Loop: Header=BB1_1 Depth=1
	xorl	%eax, %eax
	movb	%al, %cl
	testb	$1, %cl
	jne	.LBB1_1
	jmp	.LBB1_6
.LBB1_6:                                # %for.end18
	leaq	-8000(%rbp), %rsi
	leaq	-4000(%rbp), %rdi
	callq	_Z8vecTransPiS_
	xorl	%ecx, %ecx
	movl	%eax, -8016(%rbp)       # 4-byte Spill
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
