.POSIX:

PROJECTDIR = ..
LIBDIR = $(PROJECTDIR)/lib/scc
include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

OBJ       = main.o coff32.o formats.o

all: nm

main.o: $(INCDIR)/scc.h $(INCDIR)/ar.h $(INCDIR)/arg.h nm.h
coff32.o: nm.h
coff32.o: ../inc/coff32/filehdr.h
coff32.o: ../inc/coff32/scnhdr.h
coff32.o: ../inc/coff32/syms.h
coff32.o: ../inc/scc.h
formats.o: nm.h

nm: $(OBJ) $(LIBDIR)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

$(LIBDIR)/libscc.a: $(LIB-OBJ)
	+cd $(LIBDIR) && $(MAKE)

dep:
clean:
	rm -f nm *.o

distclean: clean
