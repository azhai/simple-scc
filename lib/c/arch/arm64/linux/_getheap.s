	.file	"_getheap.s"

	.globl	_getheap
_getheap:
	mov	x0,#0
	b	_brk
