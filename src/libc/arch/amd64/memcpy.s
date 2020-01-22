	.file 	"memcpy.s"
	.text
	.globl	memcpy,_memcpy

memcpy:
_memcpy:
	mov	%rdi,%rax
	mov	%rdx,%rcx
	rep
	movsb
	ret
