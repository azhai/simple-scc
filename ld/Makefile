.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

OBJ = main.o formats.o coff32.o obj.o

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
coff32.o: ./../inc/coff32/filehdr.h
coff32.o: ./../inc/coff32/scnhdr.h
coff32.o: ./../inc/coff32/syms.h
coff32.o: ./../inc/scc.h
coff32.o: ./ld.h
formats.o: ./../inc/scc.h
formats.o: ./ld.h
main.o: ./../inc/ar.h
main.o: ./../inc/scc.h
main.o: ./../inc/syslibs.h
main.o: ./ld.h
obj.o: ./../inc/scc.h
obj.o: ./ld.h
