.POSIX:
PROJECTDIR =../../..
include $(PROJECTDIR)/scripts/rules.mk
include ../rules.mk

OBJS = \
	memchr.$O\
	memcmp.$O\
	memcpy.$O\
	memmove.$O\
	memset.$O\
	strcat.$O\
	strchr.$O\
	strcmp.$O\
	strcoll.$O\
	strcpy.$O\
	strcspn.$O\
	strerror.$O\
	strlen.$O\
	strncat.$O\
	strncmp.$O\
	strncpy.$O\
	strnlen.$O\
	strpbrk.$O\
	strrchr.$O\
	strspn.$O\
	strstr.$O\
	strtok.$O\
	strxfrm.$O\

all: $(LIBC)

$(LIBCLST): $(OBJS)
	$(MKLST)

dep: inc-dep

include deps.mk
