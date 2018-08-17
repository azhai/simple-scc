.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBSCC)/libdep.mk

OBJ = main.o coff32.o obj.o

all: $(BINDIR)/ld

$(BINDIR)/ld: ld
	cp ld $@

ld: $(OBJ) $(LIBSCC)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

$(INCDIR)/syslibs.h:
	+@cd $(INCDIR) && $(MAKE)

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f $(BINDIR)/ld ld *.o

include deps.mk
