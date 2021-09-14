	.globl	_environ
	.comm	_environ,8,8

	.text
	.globl	_start
_start:
	ldr	x0,[sp]		/* argc */
	add	x1,sp,#8	/* argv */
	adr	x2,_environ
	add	x9,x1,x0,lsl #3	/* envp = argc + 8*argc + 8 */
	add	x9,x9,#8
	ldr	x9,[x9]
	str	x9,[x2]
	bl	main
	b	exit
