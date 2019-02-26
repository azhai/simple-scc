.POSIX:
PROJECTDIR = ../../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET = $(BINDIR)/ld

OBJS = main.o \
       symbol.o \
       pass1.o \
       pass2.o \
       pass3.o \


all: $(TARGET)

#TODO Add library dependencies
$(TARGET): $(OBJS)
	$(CC) $(SCC_LDFLAGS) $(OBJS) -lmach -lscc -o $@

dep: inc-dep

clean:
	rm -f $(OBJS)
