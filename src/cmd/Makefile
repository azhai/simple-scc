.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET = $(BINDIR)/nm \
         $(BINDIR)/strip \
         $(BINDIR)/size \
         $(BINDIR)/ar \
         $(BINDIR)/ranlib \
         $(BINDIR)/objdump \
         $(BINDIR)/objcopy \
         $(BINDIR)/addr2line \

LIBS   = -lmach
DIRS   = as ld scc

all: $(TARGET) $(DIRS)

$(DIRS): FORCE
	@+cd $@ && $(MAKE)

$(TARGET): $(LIBDIR)/libmach.a

$(BINDIR)/nm: nm.o
	$(CC) $(SCC_LDFLAGS) nm.o -lmach -o $@

$(BINDIR)/strip: strip.o
	$(CC) $(SCC_LDFLAGS) strip.o -lmach -o $@

$(BINDIR)/size: size.o
	$(CC) $(SCC_LDFLAGS) size.o -lmach -o $@

$(BINDIR)/ranlib: ranlib.o
	$(CC) $(SCC_LDFLAGS) ranlib.o -lmach -o $@

$(BINDIR)/objdump: objdump.o
	$(CC) $(SCC_LDFLAGS) objdump.o -lmach -o $@

$(BINDIR)/objcopy: objcopy.o
	$(CC) $(SCC_LDFLAGS) objcopy.o -lmach -o $@

$(BINDIR)/addr2line: addr2line.o
	$(CC) $(SCC_LDFLAGS) addr2line.o -lmach -o $@

$(BINDIR)/ar: ar.o $(DRIVER).o
	$(CC) $(SCC_LDFLAGS) ar.o $(DRIVER).o -o $@

dep: inc-dep

clean:
	$(FORALL)

include deps.mk
