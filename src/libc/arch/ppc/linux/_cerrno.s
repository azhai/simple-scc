	.file	"_cerrno.s"

	.text
	.globl	_cerrno
_cerrno:
	bso	err
	blr

err:
	lis	14,errno@h
	ori	14,14,errno@l
	neg	3,3
	stw	3,0(14)
	xor	3,3,3
	addi	3,3,-1
	blr
