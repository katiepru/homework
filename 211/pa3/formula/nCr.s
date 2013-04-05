.globl nCr
	.type	nCr, @function
nCr:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$40, %rsp
	movl	%edi, -36(%rbp)
	movl	%esi, -40(%rbp)
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	.cfi_offset 3, -24
	call	Factorial
	movl	%eax, -24(%rbp)
	cmpl	$0, -24(%rbp)
	je	.L6
	cmpl	$-1, -24(%rbp)
	jne	.L7
.L6:
	movl	$0, %eax
	jmp	.L8
.L7:
	movl	-40(%rbp), %eax
	movl	%eax, %edi
	call	Factorial
	movl	%eax, %ebx
	movl	-40(%rbp), %eax
	movl	-36(%rbp), %edx
	movl	%edx, %ecx
	subl	%eax, %ecx
	movl	%ecx, %eax
	movl	%eax, %edi
	call	Factorial
	imull	%ebx, %eax
	movl	%eax, -20(%rbp)
	cmpl	$0, -20(%rbp)
	jg	.L9
	movl	$0, %eax
	jmp	.L8
.L9:
	cvtsi2ss	-24(%rbp), %xmm0
	cvtsi2ss	-20(%rbp), %xmm1
	divss	%xmm1, %xmm0
	movss	%xmm0, -28(%rbp)
	movss	-28(%rbp), %xmm0
	cvttss2si	%xmm0, %eax
.L8:
	addq	$40, %rsp
	popq	%rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc

.globl Factorial
	.type	Factorial, @function
Factorial:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movl	$1, -12(%rbp)
	movl	$1, -8(%rbp)
	movl	$0, -4(%rbp)
	cmpl	$0, -20(%rbp)
	jne	.L12
	movl	$1, %eax
	jmp	.L13
.L12:
	cmpl	$0, -20(%rbp)
	jns	.L15
	movl	$-1, %eax
	jmp	.L13
.L18:
	movl	-12(%rbp), %eax
	imull	-20(%rbp), %eax
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %eax
	cmpl	-8(%rbp), %eax
	jge	.L16
	movl	$1, -4(%rbp)
	jmp	.L17
.L16:
	movl	-12(%rbp), %eax
	movl	%eax, -8(%rbp)
	subl	$1, -20(%rbp)
.L15:
	cmpl	$0, -20(%rbp)
	jg	.L18
.L17:
	cmpl	$0, -4(%rbp)
	je	.L19
	movl	$0, %eax
	jmp	.L13
.L19:
	movl	-12(%rbp), %eax
.L13:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
