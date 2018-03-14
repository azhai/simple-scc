.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

OBJ       = main.o coff32.o formats.o

all: nm
	cp nm $(PROJECTDIR)/rootdir/bin/nm

nm: $(OBJ) $(LIBDIR)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f nm *.o

distclean: clean

#deps
coff32.c: ../inc/coff32/filehdr.h
coff32.c: ../inc/coff32/scnhdr.h
coff32.c: ../inc/coff32/syms.h
coff32.c: ../inc/scc.h
coff32.c: nm.h
formats.c: nm.h
main.c: ../inc/arg.h
main.c: ../inc/ar.h
main.c: ../inc/scc.h
main.c: nm.h
