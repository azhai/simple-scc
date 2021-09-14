	.file 	"memcpy.s"

	.text
	.globl	memcpy,_memcpy

_memcpy:
memcpy:
	cld
	mov	%rdi,%rax
	mov	%rdx,%rcx
	rep
	movsb
	ret
