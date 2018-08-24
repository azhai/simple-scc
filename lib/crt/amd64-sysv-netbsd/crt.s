	.file	"crt.s"

	.section ".note.netbsd.ident", "a"
	.p2align 2
	.long	8
	.long	4
	.long	1
	.ascii	"NetBSD\0"
	.p2align 2
	.long	200000000

	.text
	.align	8
	.global	_start
_start:
	call	main
	movl    %eax, %edi
	jp	exit
