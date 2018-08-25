	.file	"crt.s"

	.section .note.netbsd.ident
	.long   7
	.long   4
	.long   1
	.ascii "NetBSD\0"
	.p2align 2
	.long      200000000

	.text
	.align	8
	.global	_start
_start:
	call	main
	movl    %eax, %edi
	jp	exit
