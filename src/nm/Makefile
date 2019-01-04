.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS  = main.o \

TARGET = $(BINDIR)/nm

all: $(TARGET)

$(TARGET): $(LIBDIR)/libscc.a $(LIBDIR)/libmach.a

$(TARGET): $(OBJS)
	$(CC) $(SCC_LDFLAGS) $(OBJS) -lmach -lscc -o $@

dep: inc-dep

include deps.mk
