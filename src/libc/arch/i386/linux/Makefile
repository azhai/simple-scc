.POSIX:

PROJECTDIR =../../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

GENOBJS =\
	_Exit.$O\
	_close.$O\
	_open.$O\
	_read.$O\
	_write.$O\
	_lseek.$O\
	_brk.$O\
	_sigaction.$O\
	_getpid.$O\
	_kill.$O\

GENSRC = $(GENOBJS:.$O=.s)

OBJS =\
	$(GENOBJS)\
	_cerrno.$O\
	_sys_errlist.$O\

all: $(OBJS) $(CRT)

crt.$O: ../crt-posix.s

$(GENSRC): syscall.lst
	./gensys.sh $(@:.s=)

clean:
	rm -f $(GENSRC)
	rm -f syscall _sys_errlist.c

include deps.mk
