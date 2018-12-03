	.file	"_getheap.s"

	.globl	_getheap
_getheap:
	mov	r0,#0
	b	_brk
