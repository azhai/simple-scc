.POSIX:
PROJECTDIR =../../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

OBJS 	=\
	_Exit.o\
	_close.o\
	_open.o\
	_read.o\
	_write.o\
	_lseek.o\
	_brk.o\
	_sigaction.o\
	_getpid.o\
	_Exit.o\
	_kill.o\
	_getheap.o\
	crt.o\
	signal.o\
	raise.o\
	_cerrno.o\
	signal.o

all: syscall
	$(MAKE) objs

objs: $(OBJS)

crt.o:	../crt-posix.s

raise.o:	../../posix/raise.c

signal.o:	../../posix/signal.c

syscall: syscall.lst
	gensys.sh syscall.lst
	touch syscall

clean:
	rm -f `awk '/[0-9]*\t/{print $$2".s"}' syscall.lst`
	rm -f syscall _sys_errlist.c
