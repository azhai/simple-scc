.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

OBJ = main.o coff32.o obj.o

all: ld
	cp ld $(PROJECTDIR)/rootdir/bin

ld: $(OBJ) $(LIBDIR)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f ld *.o

distclean: clean

include deps.mk
