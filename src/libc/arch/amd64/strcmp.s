	.file "strcmp.s"

	.text
	.globl	strcmp,_strcmp
_strcmp:
strcmp:
	movq	%rdi,%r8
	movb	$0,%al
	movq	$-1,%rcx
	cld
	repne
	scasb

	leaq	-1(%rdi),%rcx
	subq	%r8,%rcx
	movq	%r8,%rdi

	xor	%rax,%rax	/* set ZF to 1 */

	movq	%rdi,%r8
	movq	%rsi,%rdi
	movq	%r8,%rsi
	rep
	cmpsb
	je	equa
	jb	less
	ja	grea

grea:
	movq	$1,%rax
	ret

less:
	movq	$-1,%rax
	ret

equa:
	movq	$0,%rax
	ret
