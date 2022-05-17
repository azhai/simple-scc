.POSIX:

PROJECTDIR =../../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

GENOBJS  =\
	_brk.$O\
	_close.$O\
	_exit.$O\
	_fork.$O\
	_getpid.$O\
	_gettimeofday.$O\
	_kill.$O\
	_lseek.$O\
	_open.$O\
	_read.$O\
	_setcontext.$O\
	_sigaction2.$O\
	_write.$O\
	_wait4.$O\

OBJS =\
	$(GENOBJS)\
	_sys_errlist.$O\

GENSRC = $(GENOBJS:.$O=.s)

all: $(OBJS) $(CRT)

crt.$O: ../crt-posix.s ../netbsd/crt.s

$(GENSRC): syscall.lst
	./gensys.sh $(@:.s=)

clean:
	rm -f $(GENSRC) _sys_errlist.c

include deps.mk
