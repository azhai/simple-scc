.POSIX:

PROJECTDIR =../../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

GENOBJS =\
	__close.$O\
	__exit.$O\
	__fork.$O\
	__getpid.$O\
	__kill.$O\
	__lseek.$O\
	__open.$O\
	__read.$O\
	__sigaction.$O\
	__write.$O\

GENSRC = $(GENOBJS:.$O=.s)

OBJS  =\
	$(GENOBJS)\
	_sys_errlist.$O\

all: $(OBJS) $(CRT)

crt.$O: ../crt-posix.s

$(GENSRC): syscall.lst
	./gensys.sh $(@:.s=)

clean:
	rm -f $(GENSRC) _sys_errlist.c

include deps.mk
