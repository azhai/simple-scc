.POSIX:

DIRS =\
	linux\

PROJECTDIR =../../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../../rules.mk

OBJS =\
	longjmp.$O\
	setjmp.$O\
	memchr.$O\
	memcmp.$O\
	memcpy.$O\
	memmove.$O\
	memset.$O\

all: $(LIBC) $(SYS)

$(LIBC): $(OBJS)
	$(MKLST)

dep: inc-dep

dep clean:
	$(FORALL)

include deps.mk
