.POSIX:

PROJECTDIR =../../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

GENOBJS =\
	__Exit.$O\
	__close.$O\
	__getpid.$O\
	__kill.$O\
	__lseek.$O\
	__open.$O\
	__read.$O\
	__write.$O\
	__sigaction.$O\

GENSRC = $(GENOBJS:.$O=.s)

OBJS  =\
	$(GENOBJS)\
	_getheap.$O\
	_sys_errlist.$O\

# Rules

all: $(OBJS) $(CRT)

crt.$O: ../crt-posix.s

$(GENSRC): syscall.lst
	./gensys.sh $(@:.s=)

clean:
	rm -f $(GENSRC)
	rm -f syscall _sys_errlist.c

include deps.mk
