#!/bin/sh

awk '
/^E/ && $2 > 0 {
	errno[$1] = $2
}

END {
	for (i in errno)
		print "#define", i, errno[i] | "sort -n -k3"
	close("sort -n -k3")
}' $@
