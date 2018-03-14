.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

OBJ = main.o formats.o coff32.o

all: ld
	cp ld $(PROJECTDIR)/rootdir/bin

ld: $(OBJ) $(LIBDIR)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f ld *.o

distclean: clean

#deps
coff32.c: ../inc/coff32/filehdr.h
coff32.c: ../inc/coff32/scnhdr.h
coff32.c: ../inc/coff32/syms.h
coff32.c: ../inc/scc.h
coff32.c: ld.h
formats.c: ld.h
main.c: ../inc/arg.h
main.c: ../inc/ar.h
main.c: ../inc/scc.h
main.c: ld.h
