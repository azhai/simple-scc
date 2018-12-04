.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

MORECFLAGS = -I$(DRIVER)

OBJS   = main.o \
         $(DRIVER)/driver.o \

TARGET = $(BINDIR)/ar

all: $(TARGET)

$(TARGET): $(LIBDIR)/libscc.a

$(TARGET): $(OBJS)
	$(CC) $(SCC_LDFLAGS) $(OBJS) -lscc -o $@

dep: inc-dep

include deps.mk
