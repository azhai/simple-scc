	.file	"crt.s"

	.section .note.netbsd.ident
	.long   7
	.long   4
	.long   1
	.ascii "NetBSD\0"
	.p2align 2
	.long      200000000

	.bss
_environ:
	.quad	0

	.text
	.align	8
	.global	_start
_start:
	andq	$-16,%rsp
	movq	%rsp,%rbp

	movq	16(%rbp),%rbx
	movq	%rbx,_environ
	call	main
	movl    %eax,%edi
	jmp	exit
