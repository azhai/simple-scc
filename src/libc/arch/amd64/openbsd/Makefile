.POSIX:

PROJECTDIR =../../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

GENOBJS =\
	_access.$O\
	_brk.$O\
	_close.$O\
	_execve.$O\
	_exit.$O\
	_fork.$O\
	_getpid.$O\
	_gettimeofday.$O\
	_kill.$O\
	_lseek.$O\
	_open.$O\
	_read.$O\
	_sigaction.$O\
	_write.$O\
	_wait4.$O\

OBJS =\
	$(GENOBJS)\
	_sys_errlist.$O\

GENSRC = $(GENOBJS:.$O=.s)

all: $(OBJS) $(CRT)

$(CRT): ../crt-posix.s ../openbsd/crt.s

$(GENSRC): syscall.lst
	./gensys.sh $(@:.s=)

clean:
	rm -f $(GENSRC) _sys_errlist.c

include deps.mk
