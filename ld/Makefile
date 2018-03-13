.POSIX:

PROJECTDIR = ..
LIBDIR = $(PROJECTDIR)/lib/scc
include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

OBJ = main.o formats.o coff32.o

all: ld
	mkdir -p $(PROJECTDIR)
	cp ld $(PROJECTDIR)/rootdir/bin

ld: $(OBJ)
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

$(LIBDIR)/libscc.a: $(LIB-OBJ)
	+cd $(LIBDIR) && $(MAKE)

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f ld *.o

distclean: clean

#deps
coff32.c: ld.h
formats.c: ld.h
main.c: ../inc/arg.h
main.c: ../inc/ar.h
main.c: ../inc/scc.h
main.c: ld.h
