#!/bin/sh

set -e

trap "rm -f file.a f1 f2 f3 $tmp1 $tmp2" 0 2 3

tmp1=`mktemp`
tmp2=`mktemp`

###########################################################################
#Append generated files at once to an existing archive

echo First > file1
echo Second > file2
echo Third > file3

cp master.a file.a
ar -qv file.a file1 file2 file3

ar -t file.a file1 file2 file3 > $tmp1

cat <<EOF > $tmp2
file1
file2
file3
EOF

cmp $tmp1 $tmp2

ar -p file.a file1 file2 file3 > $tmp1

cat <<EOF > $tmp2
First
Second
Third
EOF

cmp $tmp1 $tmp2
