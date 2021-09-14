
	.comm _environ,4,4

	.text
	.globl	_start
_start:
	stwu	1,-16(1)
	la	31,16(1)
	lwz	3,0(31)		/* argc */
	la	4,4(31)		/* argv */
	lis	14,_environ@h
	ori	14,14,_environ@l
	la	5,0(14)
	mulli	31,3,4		/* p = argc*4 */
	add	31,4,31		/* q = argv + p */
	addi	31,31,4		/* r = q + 4 */
	lwz	31,0(31)	/* r = *r */
	stw	31,0(5)		/* envp = r */
	bl	main
	b	exit
