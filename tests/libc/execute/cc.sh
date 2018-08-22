#!/bin/sh

set -e

for i
do
	case "$i" in
	-r)
		root=$2
		shift 2
		;;
	-a)
		abi=$2
		shift 2
		;;
	-s)
		sys=$2
		shift 2
		;;
	-*)
		echo usage: cc.sh [-t target] file
		exit 1
		;;
	esac
done

sys=${sys:-linux}
abi=${abi:-amd64-sysv}
root=${root:-$SCCPREFIX}
inc=$root/include/scc
arch_inc=$root/include/scc/bits/$abi
lib=$root/lib/scc/${abi}-${sys}
obj=${1%.c}.o

c99 -nostdinc -I$inc -I$arch_inc -c $1
echo ld -L$lib $lib/crt.o $obj -lc >&2
ld -L$lib $lib/crt.o $obj -lc
