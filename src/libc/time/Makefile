.POSIX:
PROJECTDIR =../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../rules.mk

OBJS =\
	_daysyear.$O\
	asctime.$O\
	ctime.$O\
	difftime.$O\
	gmtime.$O\
	localtime.$O\
	mktime.$O\
	strftime.$O\

all: $(OBJS)

$(LIBC): $(OBJS)
	$(MKLIB)
