.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

OBJ       = main.o 

all: objdump

objdump: $(OBJ) $(LIBDIR)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

main.o: $(PROJECTDIR)/inc/scc.h $(PROJECTDIR)/inc/myro.h $(PROJECTDIR)/inc/arg.h

dep:
clean:
	rm -f objdump *.o

distclean: clean
