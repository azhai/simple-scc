#!/bin/sh

set -e


(cd ..
echo H
echo '/^#rules/+;$c'

for i in *.c
do
	cat <<EOF
${i%.c}.o: ../../$i
	\$(CC) \$(SCC_CFLAGS) ../../$i -c

EOF
done

echo .
echo w)  | ed -s objlst.mk
