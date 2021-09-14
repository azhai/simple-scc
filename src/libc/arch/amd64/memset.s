	.file "memset.s"

	.text
	.globl	memset,_memset
_memset:
memset:
	cld
	movq	%rdi,%r8
	movq	%rdx,%rcx
	movl	%esi,%eax
	rep
	stosb
	movq	%r8,%rax
	ret
