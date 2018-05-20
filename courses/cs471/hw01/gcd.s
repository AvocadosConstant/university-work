	.file	"gcd.c"
	.text
	.globl	gcdI
	.type	gcdI, @function
gcdI:
.LFB0:
	.cfi_startproc
	pushq	%rbp                # Save %rbp (Create stack frame)
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp          # Point %rbp to top of stack
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)      # paramater i
	movl	%esi, -8(%rbp)      # parameter j
	jmp	.L2
.L4:
	movl	-4(%rbp), %eax      # Load i into %eax
	cmpl	-8(%rbp), %eax      # Compare j and %eax (i)
	jle	.L3                     # if (i > j) GOTO L3
	movl	-8(%rbp), %eax
	subl	%eax, -4(%rbp)      # j = j - i
	jmp	.L2                     # Continues loop back to L2
.L3:
	movl	-4(%rbp), %eax
	subl	%eax, -8(%rbp)      # i = i - j
.L2:
	movl	-4(%rbp), %eax      # Load i into %eax
	cmpl	-8(%rbp), %eax      # Comparing i and j
	jne	.L4                     # if (i!=j) GOTO L4
	movl	-4(%rbp), %eax      # Broke from loop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret                         # Returning i
	.cfi_endproc
.LFE0:
	.size	gcdI, .-gcdI
	.ident	"GCC: (Ubuntu 4.8.4-2ubuntu1~14.04.3) 4.8.4"
	.section	.note.GNU-stack,"",@progbits
