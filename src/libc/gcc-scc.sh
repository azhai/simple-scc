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
	-c)
		onlycc=1;
		shift
		;;
	-*)
		echo usage: gcc-scc [-o outfile][-c][-r root][-a abi][-s sys] file
		exit 1
		;;
	esac
done

sys=${sys:-`uname | tr 'A-Z' 'a-z'`}
abi=${abi:-amd64}
out=${out:-a.out}
root=${root:-${SCCPREFIX:-`dirname $0`/..}}
inc=$root/include
arch_inc=$inc/bits/$abi
sys_inc=$inc/bits/$sys
sys_arch_inc=$inc/bits/$sys/$abi
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

includes="-nostdinc -I$inc -I$arch_inc -I$sys_inc -I$sys_arch_inc"
cflags="-std=c99 -g -w -fno-pie -fno-stack-protector -ffreestanding -static"
ldflags="-g -z nodefaultlib -static -L$lib"

if test ${onlycc:-0} -eq 1
then
	$cc $cflags $includes -c $@
else
	for i
	do
		case $i in
		*.c)
			$cc $cflags $includes -c $i
			;;
		esac
	done
	$ld $ldflags $nopie `echo $@ | sed 's/\.c$/.o/g'` $crt -lc -lcrt -o $out
fi
