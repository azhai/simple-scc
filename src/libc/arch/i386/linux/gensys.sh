#!/bin/sh
#
# This job is very easy because app and kernel ABI are identical
# until the 4th parameter, so we only have to set the syscall
# number in eax

sed 's/[ 	]*#.*//
     /^$/d' syscall.lst |
while read num name
do
cat <<EOF > $name.s
	.file	"$name.s"

	.globl	$name
$name:
	movl	\$$num,%eax
	syscall
	jmp	_cerrno
EOF
done
