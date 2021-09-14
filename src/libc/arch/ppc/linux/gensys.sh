#!/bin/sh

sed -n "
     s/[ 	]*#.*//
     /$1/p" syscall.lst |
while read num name nargs
do
cat <<EOF > $name.s
	.file "$name.s"

	.globl $name
$name:
	li	0,$num
	sc
	mfcr	0
	b	_cerrno
EOF

done
