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

sys=${sys:-`uname | tr 'A-Z' 'a-z'`}
abi=${abi:-amd64-sysv}
root=${root:-$SCCPREFIX}
inc=$root/include/scc
arch_inc=$root/include/scc/bits/$abi
lib=$root/lib/scc/${abi}-${sys}
obj=${1%.c}.o

gcc -fno-stack-protector -std=c99 -static -nostdinc -I$inc -I$arch_inc -c $1
ld -z nodefaultlib -static -L$lib $lib/crt.o $obj -lc
