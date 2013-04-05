	.file	"mystery.c"
	.comm	num,192,32
	.comm	result,4,4
	.text
.globl add
	.type	add, @function
add:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-8(%rbp), %eax
	movl	-4(%rbp), %edx
	leal	(%rdx,%rax), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	add, .-add
.globl compute_fib
	.type	compute_fib, @function
compute_fib:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	pushq	%rbx
	subq	$28, %rsp

	movl	%edi, -20(%rbp)
	movl	-20(%rbp), %eax
	cltq
	movl	num(,%rax,4), %eax
	cmpl	$-1, %eax
	je	.L4
	.cfi_offset 3, -32
	.cfi_offset 12, -24
	movl	-20(%rbp), %eax
	cltq
	movl	num(,%rax,4), %eax
	jmp	.L5
.L4:
	cmpl	$0, -20(%rbp)
	jne	.L6
	movl	-20(%rbp), %eax
	cltq
	movl	$0, num(,%rax,4)
	movl	-20(%rbp), %eax
	cltq
	movl	num(,%rax,4), %eax
	jmp	.L5
.L6:
	cmpl	$1, -20(%rbp)
	jne	.L7
	movl	-20(%rbp), %eax
	cltq
	movl	$1, num(,%rax,4)
	movl	-20(%rbp), %eax
	cltq
	movl	num(,%rax,4), %eax
	jmp	.L5
.L7:
	movl	-20(%rbp), %r12d
	movl	-20(%rbp), %eax
	subl	$1, %eax
	movl	%eax, %edi
	call	compute_fib
	movl	%eax, %ebx
	movl	-20(%rbp), %eax
	subl	$2, %eax
	movl	%eax, %edi
	call	compute_fib
	movl	%ebx, %esi
	movl	%eax, %edi
	call	add
	movslq	%r12d, %rdx
	movl	%eax, num(,%rdx,4)
	movl	-20(%rbp), %eax
	cltq
	movl	num(,%rax,4), %eax
.L5:
	addq	$28, %rsp
	popq	%rbx
	popq	%r12
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	compute_fib, .-compute_fib
	.section	.rodata
	.align 8
.LC0:
	.string	"ERROR. Too many arguments! Syntax: ./mystery <pos_int>\n"
	.align 8
.LC1:
	.string	"ERROR. No Argument Provided! Syntax: ./mystery <pos_int>\n"
	.align 8
.LC2:
	.string	"ERROR. Negative Integer! Syntax: ./mystery <pos_int>\n"
	.align 8
.LC3:
	.string	"ERROR. Integer too large! Syntax: ./mystery <pos_int>\n"
.LC4:
	.string	"Value: %d\n"
	.text
.globl main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movq	%rsi, -32(%rbp)
	cmpl	$2, -20(%rbp)
	jle	.L10
	movq	stderr(%rip), %rax
	movq	%rax, %rdx
	movl	$.LC0, %eax
	movq	%rdx, %rcx
	movl	$55, %edx
	movl	$1, %esi
	movq	%rax, %rdi
	call	fwrite
	movl	$0, %eax
	jmp	.L11
.L10:
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	testq	%rax, %rax
	jne	.L12
	movq	stderr(%rip), %rax
	movq	%rax, %rdx
	movl	$.LC1, %eax
	movq	%rdx, %rcx
	movl	$57, %edx
	movl	$1, %esi
	movq	%rax, %rdi
	call	fwrite
	movl	$0, %eax
	jmp	.L11
.L12:
	movq	-32(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	atoi
	movl	%eax, -8(%rbp)
	cmpl	$0, -8(%rbp)
	jns	.L13
	movq	stderr(%rip), %rax
	movq	%rax, %rdx
	movl	$.LC2, %eax
	movq	%rdx, %rcx
	movl	$53, %edx
	movl	$1, %esi
	movq	%rax, %rdi
	call	fwrite
	movl	$0, %eax
	jmp	.L11
.L13:
	cmpl	$46, -8(%rbp)
	jle	.L14
	movq	stderr(%rip), %rax
	movq	%rax, %rdx
	movl	$.LC3, %eax
	movq	%rdx, %rcx
	movl	$54, %edx
	movl	$1, %esi
	movq	%rax, %rdi
	call	fwrite
	movl	$0, %eax
	jmp	.L11
.L14:
	movl	$0, -4(%rbp)
	jmp	.L15
.L16:
	movl	-4(%rbp), %eax
	cltq
	movl	$-1, num(,%rax,4)
	addl	$1, -4(%rbp)
.L15:
	movl	-4(%rbp), %eax
	cmpl	$47, %eax
	jbe	.L16
	movl	-8(%rbp), %eax
	movl	%eax, %edi
	call	compute_fib
	movl	%eax, -12(%rbp)
	movl	$.LC4, %eax
	movl	-12(%rbp), %edx
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
.L11:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-3)"
	.section	.note.GNU-stack,"",@progbits
