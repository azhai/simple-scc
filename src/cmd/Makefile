.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET = $(BINDIR)/nm \
         $(BINDIR)/strip \
         $(BINDIR)/size \
         $(BINDIR)/ar \

LIBS   = -lmach

all: $(TARGET)

$(TARGET): $(LIBDIR)/libmach.a

$(BINDIR)/nm: nm.o
	$(CC) $(SCC_LDFLAGS) nm.o -lmach -o $@

$(BINDIR)/strip: strip.o
	$(CC) $(SCC_LDFLAGS) strip.o -lmach -o $@

$(BINDIR)/size: size.o
	$(CC) $(SCC_LDFLAGS) size.o -lmach -o $@

$(BINDIR)/ar: ar.o ar-$(DRIVER).o
	$(CC) $(SCC_LDFLAGS) ar.o ar-$(DRIVER).o -o $@

dep: inc-dep

include deps.mk
