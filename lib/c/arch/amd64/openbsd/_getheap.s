	.file	"_getheap.s"

	.globl	_getheap
_getheap:
	movq	$end,%rax
	retq
