	.file	"_cerrno.s"
	.globl	_cerrno

_cerrno:
	cmp	x0,#0
	b.lt	1f
	ret

1:	neg	x0,x0
	adr	x1,errno
	str	w1,[x0]
	mov	x0,#-1
	ret
