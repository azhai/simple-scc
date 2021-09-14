	.file 	"memmove.s"

	.text
	.globl	memmove,_memmove
_memmove:
memmove:
	movq	%rdi,%rax

	movq	%rdx,%rcx

	cmpq	%rdi,%rsi
	jg	forward
	jl	backward
	ret

forward:
	cld
	rep
	movsb
	ret

backward:
	std
	movq	%rdx,%r8
	subq	$1,%r8
	addq	%r8,%rdi
	addq	%r8,%rsi
	rep;	movsb
	ret
