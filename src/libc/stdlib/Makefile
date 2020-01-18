.POSIX:
PROJECTDIR =../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../rules.mk

OBJS =\
	_dtoi.$O\
	abort.$O\
	abs.$O\
	atexit.$O\
	atoi.$O\
	atol.$O\
	atoll.$O\
	bsearch.$O\
	calloc.$O\
	errno.$O\
	exit.$O\
	labs.$O\
	llabs.$O\
	malloc.$O\
	qsort.$O\
	rand.$O\
	realloc.$O\
	strtol.$O\
	strtoll.$O\
	strtoul.$O\
	strtoull.$O\

all: $(OBJS)

$(LIBC): $(OBJS)
	$(MKLIB)

dep: inc-dep

include deps.mk
