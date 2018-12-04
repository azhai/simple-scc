.POSIX:
PROJECTDIR =../../..
include $(PROJECTDIR)/scripts/rules.mk

MORECFLAGS = -w -I$(INCDIR) -I$(INCDIR)/bits/$(SYS) -I$(INCDIR)/bits/$(ARCH)

OBJS = _daysyear.o\
       asctime.o\
       ctime.o\
       difftime.o\
       gmtime.o\
       localtime.o\
       mktime.o\
       strftime.o\

all: $(OBJS)
