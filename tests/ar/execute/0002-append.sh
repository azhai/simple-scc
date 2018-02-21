#!/bin/sh

exec >> test.log 2>&1
set -e

trap "rm -f file.a f1 f2 f3 $tmp1 $tmp2" 0 2 3

tmp1=`mktemp`
tmp2=`mktemp`

###########################################################################
#Append generated files one by one to an empty file

echo First > file1
echo Second > file2
echo Third > file3

rm -f file.a
ar -qv file.a file1
ar -qv file.a file2
ar -qv file.a file3
ar -t file.a > $tmp1

cat <<EOF > $tmp2
file1
file2
file3
EOF

cmp $tmp1 $tmp2

ar -p file.a > $tmp1

cat <<EOF > $tmp2
First
Second
Third
EOF

cmp $tmp1 $tmp2
