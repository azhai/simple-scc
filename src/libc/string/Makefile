.POSIX:
PROJECTDIR =../../..
include $(PROJECTDIR)/scripts/rules.mk

MORECFLAGS = -w -I$(INCDIR) -I$(INCDIR)/bits/$(SYS) -I$(INCDIR)/bits/$(ARCH)

OBJS = memchr.o\
       memcmp.o\
       memcpy.o\
       memmove.o\
       memset.o\
       strcat.o\
       strchr.o\
       strcmp.o\
       strcoll.o\
       strcpy.o\
       strcspn.o\
       strerror.o\
       strlen.o\
       strncat.o\
       strncmp.o\
       strncpy.o\
       strnlen.o\
       strpbrk.o\
       strrchr.o\
       strspn.o\
       strstr.o\
       strtok.o\
       strxfrm.o\

all: $(OBJS)
