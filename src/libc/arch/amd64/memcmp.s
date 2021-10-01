	.file "memcmp.s"

	.text
	.globl	memcmp,_memcmp
_memcmp:
memcmp:
	cmpq	$0,%rdx
	je	equa
	cld
	movq	%rdx,%rcx
	movq	%rdi,%r8
	movq	%rsi,%rdi
	movq	%r8,%rsi
	rep
	cmpsb
	je	equa
	jb	less
	ja	grea

equa:
	movq	$0,%rax
	ret

less:
	movq	$-1,%rax
	ret

grea:
	movq	$1,%rax
	ret
