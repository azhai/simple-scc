.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS = objdump.o 
TARGET = $(BINDIR)/objdump

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBDIR)/libmach.a
	$(CC) $(SCC_LDFLAGS) $(OBJS) -lscc -o $@

dep: inc-dep

include deps.mk
