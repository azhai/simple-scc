.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS =\
	debug.o\
	die.o\
	newitem.o\
	xcalloc.o\
	xmalloc.o\
	xrealloc.o\
	xstrdup.o\
	alloc.o\
	casecmp.o\
	genhash.o\
	fromepoch-$(DRIVER).o\
	totime-$(DRIVER).o\
	canonical-$(DRIVER).o\
	getstat-$(DRIVER).o\
	setstat-$(DRIVER).o\

TARGET = $(LIBDIR)/scc/libscc.a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(PROJ_ARFLAGS) $@ $?
	$(RL) $(PROJ_RLFLAGS) $@

include deps.mk
