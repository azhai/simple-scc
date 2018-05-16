.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

OBJ = main.o symbol.o ins.o parser.o expr.o myro.o
MOREFLAGS = -I$(PROJECTDIR)/inc/$(STD) $(AS_CFLAGS)

all: as-amd64 as-i386 as-z80

as: $(OBJ)
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

dep:
	$(PROJECTDIR)/mkdep.sh

distclean: clean
clean:
	rm -f *.o target/*/*.o
	rm -f target/*/*tbl.c
	rm -f as-*
	rm -f *.log

include target/amd64.mk
include target/i386.mk
include target/z80.mk

#deps
expr.c: ./../inc/scc.h
expr.c: ./as.h
ins.c: ./../inc/scc.h
ins.c: ./as.h
main.c: ./../inc/arg.h
main.c: ./../inc/scc.h
main.c: ./as.h
myro.c: ./../inc/myro.h
myro.c: ./../inc/scc.h
myro.c: ./as.h
parser.c: ./../inc/scc.h
parser.c: ./as.h
symbol.c: ./../inc/scc.h
symbol.c: ./as.h
target/x80/ins.c: target/x80/../../../inc/scc.h
target/x80/ins.c: target/x80/../../as.h
target/x80/ins.c: target/x80/proc.h
target/x80/z80.c: target/x80/../../../inc/scc.h
target/x80/z80.c: target/x80/../../as.h
target/x80/z80.c: target/x80/../x80/proc.h
target/x86/amd64.c: target/x86/../../../inc/scc.h
target/x86/amd64.c: target/x86/../../as.h
target/x86/i386.c: target/x86/../../../inc/scc.h
target/x86/i386.c: target/x86/../../as.h
target/x86/i386.c: target/x86/../x86/proc.h
target/x86/i8086.c: target/x86/arch16.h
target/x86/ins.c: target/x86/../../../inc/scc.h
target/x86/ins.c: target/x86/../../as.h
target/x86/ins.c: target/x86/proc.h
