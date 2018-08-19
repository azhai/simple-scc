.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBSCC)/libdep.mk

OBJ       = main.o coff32.o formats.o

all: $(BINDIR)/nm

$(BINDIR)/nm: nm
	cp nm $(PROJECTDIR)/rootdir/bin/nm

nm: $(OBJ) $(LIBSCC)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f $(BINDIR)/nm nm *.o

distclean: clean

include deps.mk
