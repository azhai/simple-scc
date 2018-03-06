.POSIX:

PROJECTDIR = ..
LIBDIR = $(PROJECTDIR)/lib/scc
include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

OBJ       = main.o coff.o formats.o

all: nm

main.o: $(INCDIR)/scc.h $(INCDIR)/ar.h $(INCDIR)/arg.h nm.h
coff.o: nm.h
formats.o: nm.h

nm: $(OBJ) $(LIBDIR)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

$(LIBDIR)/libscc.a: $(LIB-OBJ)
	+cd $(LIBDIR) && $(MAKE)

dep:
clean:
	rm -f nm *.o

distclean: clean
