.POSIX:

PROJECTDIR = ../../../../..
RULES = user

include $(PROJECTDIR)/scripts/rules.mk
include ../../../rules.mk

GENOBJS =\
	_close.$O\
	_creat.$O\
	_exit.$O\
	_getpid.$O\
	_kill.$O\
	_link.$O\
	_open.$O\
	_read.$O\
	_sys_brk.$O\
	_unlink.$O\
	_waitpid.$O\
	_write.$O\

OBJS =\
	_cerrno.o\
	_sys_errlist.o\
	$(GENOBJS)\

GENSRC = $(GENOBJS:.o=.s)

all: $(CRT) $(OBJS)

$(CRT): ../crt-posix.s

$(GENSRC): syscall.lst
	./gensys.sh $(@:.s=)

clean:
	rm -f $(GENSRC) _sys_errlist.c

include deps.mk
