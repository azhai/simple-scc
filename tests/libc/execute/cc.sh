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
	-o)
		out=$2
		shift 2
		;;
	-*)
		echo usage: cc.sh [-o outfile][-r root][-a abi][-s sys] file
		exit 1
		;;
	esac
done

sys=${sys:-`uname | tr 'A-Z' 'a-z'`}
abi=${abi:-amd64}
out=${out:-a.out}
root=${root:-${SCCPREFIX:-../../..}}
inc=$root/include
arch_inc=$inc/bits/$abi
sys_inc=$inc/bits/$sys
lib=$root/lib/scc/${abi}-${sys}
crt=$root/lib/scc/${abi}-${sys}/crt.o
obj=${1%.c}.o
cc=${CROSS_COMPILE}gcc
ld=${CROSS_COMPILE}ld

case `uname` in
OpenBSD)
	nopie=-no-pie
	;;
esac

includes="-nostdinc -I$inc -I$arch_inc -I$sys_inc"
cflags="-std=c99 -g -w -fno-pie -fno-stack-protector -ffreestanding -static"
ldflags="-g -z nodefaultlib -static -L$lib"

$cc $cflags $includes -c $1
$ld $ldflags $nopie $obj $crt -lc -lcrt -o $out
