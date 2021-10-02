	.file 	"strchr.s"

	.text
	.globl	strchr,_strchr
_strchr:
strchr:
	movq	%rdi,%r8

	movb	$0,%al
	movq	$-1,%rcx
	cld
	repne
	scasb

	leaq	-1(%rdi),%rcx
	subq	%r8,%rcx

	cmp     %rdi,%rsi       /* set ZF to 0 */

	movq	%r8,%rdi
	movb	%sil,%al
	repne
	scasb
	je 	found

none:
	xor	%rax,%rax
	ret

found:
	leaq	-1(%rdi),%rax
	ret
