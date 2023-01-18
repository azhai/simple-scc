.POSIX:

PROJECTDIR = ../../../..
DIRS =\
	darwin\
	dragonfly\
	freebsd\
	linux\
	netbsd\
	openbsd\

include $(PROJECTDIR)/scripts/rules.mk
include ../../rules.mk

OBJS =\
	longjmp.$O\
	memchr.$O\
	memcmp.$O\
	memcpy.$O\
	memmove.$O\
	memset.$O\
	setjmp.$O\
	strchr.$O\
	strcmp.$O\
	strcpy.$O\

all: $(OBJS) $(SYS)

include deps.mk
