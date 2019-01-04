.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS  = nm.o \

TARGET = $(BINDIR)/nm

all: $(TARGET)

$(TARGET): $(LIBDIR)/libmach.a

$(TARGET): $(OBJS)
	$(CC) $(SCC_LDFLAGS) $(OBJS) -lmach -o $@

dep: inc-dep

include deps.mk
