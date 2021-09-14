	.file 	"memchr.s"

	.text
	.globl	memchr,_memchr
_memchr:
memchr:
	cmpq	$0,%rdx
	je	notfound

	movq	%rdx,%rcx
	movb	%sil,%al
	cld
	repne
	scasb
	je 	found

notfound:
	xor	%eax,%eax
	ret

found:
	leaq	-1(%rdi),%rax
	ret
