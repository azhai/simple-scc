.POSIX:
PROJECTDIR =../../..
include $(PROJECTDIR)/scripts/rules.mk

MORECFLAGS = -w -I$(INCDIR) -I$(INCDIR)/bits/$(SYS) -I$(INCDIR)/bits/$(ARCH)

OBJS = ctype.o\
       isalnum.o\
       isalpha.o\
       isascii.o\
       isblank.o\
       iscntrl.o\
       isdigit.o\
       isgraph.o\
       islower.o\
       isprint.o\
       ispunct.o\
       isspace.o\
       isupper.o\
       isxdigit.o\
       tolower.o\
       toupper.o\

all: $(OBJS)
