.POSIX:

PROJECTDIR = ../../../../..

include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

GENOBJS =\
	_close.$O\
	_execve.$O\
	_exit.$O\
	_getpid.$O\
	_kill.$O\
	_lseek.$O\
	_openat.$O\
	_read.$O\
	__sigaction.$O\
	_sys_brk.$O\
	_write.$O\
	_wait4.$O\

OBJS =\
	$(GENOBJS)\
	_cerrno.$O\
	_sys_errlist.$O\

GENSRC = $(GENOBJS:.$O=.s)

all: $(OBJS) $(CRT)

$(CRT): ../crt-posix.s

$(GENSRC): syscall.lst
	./gensys.sh $(@:.s=)

clean:
	rm -f $(GENSRC) _sys_errlist.c

include deps.mk
