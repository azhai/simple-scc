#!/bin/sh

case `uname` in
*BSD)
	sys=BSD
	;;
*Plan9)
	sys=Plan9
	;;
*)
	exit
	;;
esac


(echo '/^# '$sys'/+;/^###$/- s/^#//'; echo w) | ed -s config.mk
