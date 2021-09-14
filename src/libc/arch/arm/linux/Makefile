.POSIX:

PROJECTDIR =../../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

GENOBJS =\
	_close.$O\
	_exit.$O\
	_getpid.$O\
	_kill.$O\
	_lseek.$O\
	_openat.$O\
	_read.$O\
	_sigaction.$O\
	_sys_brk.$O\
	_write.$O\

GENSRC = $(GENOBJS:.$O=.s)

OBJS =\
	$(GENOBJS)\
	_cerrno.$O\
	_sys_errlist.$O\

all: $(OBJS) $(CRT)

crt.o: ../crt-posix.s

$(GENSRC): syscall.lst
	./gensys.sh $(@:.s=)

clean:
	rm -f $(GENSRC)
	rm -f syscall _sys_errlist.c

include deps.mk
