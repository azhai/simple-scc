.POSIX:

PROJECTDIR = ../../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS =\
	defaults.o\
	main.o\
	parser.o\
	rules.o\
	$(DRIVER).o\

TARGET = $(BINDIR)/scc-make

all: $(TARGET)

$(BINDIR)/scc-make: $(OBJS)
	$(CC) $(PROJ_LDFLAGS) $(OBJS) $(PROJ_LDLIBS) -o $@

$(OBJS): make.h
