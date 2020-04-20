	.file	"_cerrno.s"
	.globl	_cerrno
_cerrno:
	cmpwi	0,0
	bne	err
	blr
err:
	lis	0,errno@h
	ori	0,0,errno@l
	stw	3,0(0)
	addi	3,3,-1
	blr
