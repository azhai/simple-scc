#!/bin/sh

#
# This job is very easy because app and kernel ABI are identical
# until the 4th parameter, so we only have to set the syscall
# number in rax

awk 'BEGIN  {n = 33554432}
            {gsub(/ *#./, "")}
     /'$1'/ {printf("0x%x\t%s\t%s\n",  n + $1, $2, $3)}' syscall.lst |
while read num name nargs
do
cat <<EOF > $name.s
	.file	"$name.s"

	.globl	$name
$name:
	`case $nargs in 4|5|6)
		echo "movq       %rcx,%r10"
                ;;
        esac`
	movq	\$$num,%rax
	syscall
	jb	1f
	retq

1:	movq	%rax,_errno(%rip)
	movq	\$-1,%rax
	retq
EOF
done
