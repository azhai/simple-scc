.POSIX:
PROJECTDIR =../../..
include $(PROJECTDIR)/scripts/rules.mk

MORECFLAGS = -w -I$(INCDIR) -I$(INCDIR)/bits/$(SYS) -I$(INCDIR)/bits/$(ARCH)

OBJS = localeconv.o\
       setlocale.o\

all: $(OBJS)
