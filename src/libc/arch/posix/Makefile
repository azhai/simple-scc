.POSIX:
PROJECTDIR =../../../..
RULES = user
include $(PROJECTDIR)/scripts/rules.mk
include $(SRCDIR)/libc/rules.mk

OBJS=\
	_getheap.$O\
	_open.$O\
	_systime.$O\
	_tzone.$O\
	clock.$O\
	getenv.$O\
	raise.$O\
	signal.$O\
	system.$O\
	time.$O\
	tmpfile.$O\

all: $(OBJS)

include deps.mk
