.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

OBJ = main.o symbol.o ins.o parser.o expr.o myro.o
MOREFLAGS = -I$(PROJECTDIR)/inc/$(STD) $(AS_CFLAGS)

all: as-amd64 as-i386 as-i286 as-z80

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
include target/i286.mk
include target/z80.mk

#deps
expr.o: ./../inc/scc.h
expr.o: ./as.h
ins.o: ./../inc/scc.h
ins.o: ./as.h
main.o: ./../inc/arg.h
main.o: ./../inc/scc.h
main.o: ./as.h
myro.o: ./../inc/myro.h
myro.o: ./../inc/scc.h
myro.o: ./as.h
parser.o: ./../inc/scc.h
parser.o: ./as.h
symbol.o: ./../inc/scc.h
symbol.o: ./as.h
target/x80/ins.o: ./target/x80/../../../inc/scc.h
target/x80/ins.o: ./target/x80/../../as.h
target/x80/ins.o: ./target/x80/proc.h
target/x80/z80.o: ./target/x80/../../../inc/scc.h
target/x80/z80.o: ./target/x80/../../as.h
target/x80/z80.o: ./target/x80/../x80/proc.h
target/x80/z80tbl.o: ./target/x80/../../../inc/scc.h
target/x80/z80tbl.o: ./target/x80/../../as.h
target/x80/z80tbl.o: ./target/x80/../x80/proc.h
target/x86/amd64.o: ./target/x86/../../../inc/scc.h
target/x86/amd64.o: ./target/x86/../../as.h
target/x86/amd64tbl.o: ./target/x86/../../../inc/scc.h
target/x86/amd64tbl.o: ./target/x86/../../as.h
target/x86/amd64tbl.o: ./target/x86/../x86/proc.h
target/x86/i286.o: ./target/x86/../../../inc/scc.h
target/x86/i286.o: ./target/x86/../../as.h
target/x86/i286.o: ./target/x86/../x86/proc.h
target/x86/i286tbl.o: ./target/x86/../../../inc/scc.h
target/x86/i286tbl.o: ./target/x86/../../as.h
target/x86/i286tbl.o: ./target/x86/../x86/proc.h
target/x86/i386.o: ./target/x86/../../../inc/scc.h
target/x86/i386.o: ./target/x86/../../as.h
target/x86/i386.o: ./target/x86/../x86/proc.h
target/x86/i386tbl.o: ./target/x86/../../../inc/scc.h
target/x86/i386tbl.o: ./target/x86/../../as.h
target/x86/i386tbl.o: ./target/x86/../x86/proc.h
target/x86/ins.o: ./target/x86/../../../inc/scc.h
target/x86/ins.o: ./target/x86/../../as.h
target/x86/ins.o: ./target/x86/proc.h
