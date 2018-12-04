.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS  = main.o \
        coff32.o \
        formats.o \

TARGET = $(BINDIR)/nm

all: $(TARGET)

$(TARGET): $(LIBDIR)/libscc.a

$(TARGET): $(OBJS)
	$(CC) $(SCC_LDFLAGS) $(OBJS) -lscc -o $@

dep: inc-dep

include deps.mk
