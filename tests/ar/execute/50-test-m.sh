#!/bin/sh


exec >> test.log 2>&1
set -e

tmp1=`mktemp`
tmp2=`mktemp`
trap "rm -f file* $tmp1 $tmp2" 0 2 3

cp master.a file.a
############################################################################
#move 1st without specifier

ar -mv file.a file1
ar -t file.a > $tmp1

cat <<EOF > $tmp2
file2
file3
file1
EOF

cmp $tmp1 $tmp2
############################################################################
#move 1st at the end

cp master.a file.a
ar -mv -a file3 file.a file1
at -t file.a > $tmp1

cat <<EOF > $tmp2
file2
file3
file1
EOF

cmp $tmp1 $tmp2
