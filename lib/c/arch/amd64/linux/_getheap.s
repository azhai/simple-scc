	.file	"_getheap.s"

	.globl	_getheap
_getheap:
	movq	$0,%rax
	jmp	_brk
