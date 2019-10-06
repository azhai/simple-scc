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

all: syscall
	$(MAKE) objs

objs: $(OBJS)

syscall: syscall.lst
	gensys.sh syscall.lst
	touch syscall

clean:
	rm -f `awk '/[0-9]*	_/ {print $$2".s"}' syscall.lst`
	rm -f syscall _sys_errlist.c
