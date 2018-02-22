#!/bin/sh

set -e

trap "rm -f file* $tmp1 $tmp2" 0 2 3

tmp1=`mktemp`
tmp2=`mktemp`

############################################################################
#Update one member that doesn't exist

echo First > file-1

cp master.a file.a
ar -rv file.a file-1

ar -p file.a file-1 > $tmp1

cat <<EOF > $tmp2
First
EOF

cmp $tmp1 $tmp2
