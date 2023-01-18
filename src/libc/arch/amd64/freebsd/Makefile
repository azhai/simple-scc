.POSIX:

PROJECTDIR =../../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

GENOBJS =\
	_exit.$O\

OBJS =\
	$(GENOBJS)\
	_sys_errlist.$O\

GENSRC = $(GENOBJS:.$O=.s)

all: $(OBJS) $(CRT)

$(CRT): ../crt-posix.s

$(GENSRC): syscall.lst
	./gensys.sh $(@:.s=)

clean:
	rm -f $(GENSRC) _sys_errlist.c

include deps.mk
