	.bss
	.globl	_environ
_environ:
	.long	0

	.text
	.globl	_start
_start:
	movl	%esp,%ebp

	leal	16(%ebp,%edi,8),%edx
	movl	%edx,_environ
	pushl	%edx
	leal	8(%ebp),%esi
	pushl	%esi
	movl	(%ebp),%edi
	pushl	%edi

	call 	main
	movl	%eax,%edi
	jmp	exit
