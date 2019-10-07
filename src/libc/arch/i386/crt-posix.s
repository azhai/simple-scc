	.bss
	.globl	_environ
_environ:
	.long	0

	.text
	.global	start
_start:
	movl	%esp,%ebp

	movl	(%ebp),%edi
	leal	8(%ebp),%esi
	leal	16(%ebp,%edi,8),%edx
	movl	%edx,_environ

	call 	main
	movl	%eax,%edi
	jmp	exit
