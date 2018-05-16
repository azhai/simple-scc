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
coff32.o: ./../inc/coff32/filehdr.h
coff32.o: ./../inc/coff32/scnhdr.h
coff32.o: ./../inc/coff32/syms.h
coff32.o: ./../inc/scc.h
coff32.o: ./nm.h
formats.o: ./nm.h
main.o: ./../inc/ar.h
main.o: ./../inc/arg.h
main.o: ./../inc/scc.h
main.o: ./nm.h
