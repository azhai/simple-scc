.POSIX:
PROJECTDIR =../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../rules.mk

OBJS =\
	_daysyear.$O\
	_newyear.$O\
	asctime.$O\
	ctime.$O\
	difftime.$O\
	gmtime.$O\
	localtime.$O\
	mktime.$O\
	strftime.$O\
	tz.$O\

all: $(OBJS)

tz.c: timezone.lst
	awk -f gentz.awk $? >$@

clean:
	rm -f tz.c

include deps.mk
