.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBSCC)/libdep.mk
include $(LIBCOFF32)/libdep.mk

OBJ = main.o coff32.o obj.o
LIB = $(LIBSCC)/libscc.a $(LIBCOFF32)/libcoff32.a
MORELFLAGS = -L$(LIBCOFF32)

all: $(BINDIR)/ld

$(BINDIR)/ld: $(OBJ) $(LIB)
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lcoff32 -lscc -o $@

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f $(BINDIR)/ld *.o

include deps.mk
