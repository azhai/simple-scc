.global _gettimeofday
_gettimeofday:
	movq	$418,%rax
	syscall
	ret
