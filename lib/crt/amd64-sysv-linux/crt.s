	.file	"crt.s"

	.text
	.global	_start
_start:
	call	main
	movl    %eax, %edi
	jmp	exit
