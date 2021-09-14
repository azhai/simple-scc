	.file 	"strcpy.s"

	.text
	.globl	strcpy,_strcpy

strcpy:
_strcpy:
	movq	%rdi,%r9

	movb	$0,%al
	movq	$-1,%rcx
	cld
	movq	%rsi,%rdi
	repne
	scasb
	movq	%rdi,%rcx
	subq	%rsi,%rcx

	movq	%r9,%rdi
	mov	%rdi,%rax
	rep;	movsb
	ret
