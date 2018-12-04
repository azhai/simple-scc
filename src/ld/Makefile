.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS = main.o \
       coff32.o \
       obj.o \

TARGET = $(BINDIR)/ld

all: $(TARGET)

$(TARGET): $(LIBDIR)/libcoff32.a $(LIBDIR)/libscc.a

$(TARGET): $(OBJS)
	$(CC) $(SCC_LDFLAGS) $(OBJS) -lcoff32 -lscc -o $@

dep: inc-dep

include deps.mk
