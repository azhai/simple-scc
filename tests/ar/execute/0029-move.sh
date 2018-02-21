#!/bin/sh


exec >> test.log 2>&1

set -e

trap "rm -f file* $tmp1 $tmp2" 0 2 3

tmp1=`mktemp`
tmp2=`mktemp`

############################################################################
#move 1st without specifier

cp master.a file.a
ar -mv file.a file1
ar -t file.a > $tmp1

cat <<EOF > $tmp2
file2
file3
file1
EOF

cmp $tmp1 $tmp2
