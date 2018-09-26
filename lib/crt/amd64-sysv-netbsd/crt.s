	.file	"crt.s"

	.section .note.netbsd.ident
	.long   7
	.long   4
	.long   1
	.ascii "NetBSD\0"
	.p2align 2
	.long      200000000

	.bss
	.globl	_environ
_environ:
	.quad	0

	.text
	.align	8
	.global	_start
_start:
	movq	%rsp,%rbp
	andq	$-16,%rsp

	movq	(%rbp),%rdi             # rdi = argc
	leaq	8(%rbp),%rsi            # rsi = argv
	leaq	16(%rbp,%rdi,8),%rdx    # rdx = envp = argv +8*argc + 8
	movq	%rdx,_environ

	call	main
	movl    %eax,%edi
	jmp	exit
