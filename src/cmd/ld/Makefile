.POSIX:

PROJECTDIR = ../../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS = main.o \

TARGET = $(BINDIR)/ld

all: $(TARGET)

$(TARGET): $(LIBDIR)/libscc.a

$(TARGET): $(OBJS)
	$(CC) $(SCC_LDFLAGS) $(OBJS) -lmach -o $@

dep: inc-dep

include deps.mk
