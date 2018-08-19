.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBSCC)/libdep.mk

OBJ       = main.o 
MORECFLAGS = $(STDCFLAGS)

all: $(BINDIR)/objdump

$(BINDIR)/objdump: objdump
	cp objdump $@

objdump: $(OBJ) $(LIBSCC)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f $(BINDIR)/objdump objdump *.o

include deps.mk
