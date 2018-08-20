#!/bin/sh

(echo '/SYS/c'
 awk 'BEGIN  {printf "SYS = "}
      ! /^#/ {printf "%s.o ", $2}
      END    {print ""}' syscall.lst
 echo .
 echo w) | ed -s syscall.mk
