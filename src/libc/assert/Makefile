.POSIX:
PROJECTDIR =../../..
include $(PROJECTDIR)/scripts/rules.mk

MORECFLAGS = -w -I$(INCDIR) -I$(INCDIR)/bits/$(SYS) -I$(INCDIR)/bits/$(ARCH)

OBJS = __assert.o\
       assert.o\

all: $(OBJS)
