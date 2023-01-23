.POSIX:

PROJECTDIR=../../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS =\
	umoddi3.o\
	udivdi3.o\

TARGET = $(LIBCDIR)/libcrt.a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(PROJ_ARFLAGS) $@ $?
	$(RL) $(PROJ_RLFLAGS) $@

include deps.mk
