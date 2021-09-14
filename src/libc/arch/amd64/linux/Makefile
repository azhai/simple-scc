.POSIX:

PROJECTDIR = ../../../../..

include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

GENOBJS =\
	_close.$O\
	_exit.$O\
	_getpid.$O\
	_getrusage.$O\
	_gettimeofday.$O\
	_kill.$O\
	_lseek.$O\
	_open.$O\
	_read.$O\
	_sigaction.$O\
	_sys_brk.$O\
	_unlink.$O\
	_write.$O\

OBJS =\
	$(GENOBJS)\
	_cerrno.o\
	_sys_errlist.o\

GENSRC = $(GENOBJS:.$O=.s)

all: $(OBJS) $(CRT)

$(CRT): ../crt-posix.s

$(GENSRC): syscall.lst
	./gensys.sh $(@:.s=)

clean:
	rm -f $(GENSRC) _sys_errlist.c

include deps.mk
