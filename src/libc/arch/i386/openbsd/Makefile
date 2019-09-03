.POSIX:
PROJECTDIR =../../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

all:

clean:
	rm -f `awk '/[0-9]*	_/ {print $$2".s"}' syscall.lst`
	rm -f syscall _sys_errlist.c
