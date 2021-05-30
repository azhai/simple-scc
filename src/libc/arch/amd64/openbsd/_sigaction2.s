	.file	"_sigaction2.s"

	.globl	_sigaction2
_sigaction2:
	movq	$46,%rax
	syscall
	jb	1f
	retq

1:	movq	%rax,(errno)
	movq	$-1,%rax
	retq
