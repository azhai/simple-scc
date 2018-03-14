.POSIX:

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

OBJ = main.o symbol.o ins.o parser.o expr.o myro.o
HDR = $(PROJECTDIR)/inc/scc.h as.h
MOREFLAGS = -I$(PROJECTDIR)/inc/$(STD) $(AS_CFLAGS)

all:

$(OBJ): $(HDR)

as: $(OBJ)
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

myro.o: $(PROJECTDIR)/inc/myro.h

dep:
	./gendep.sh $(TARGETS)

clean:
	rm -f *.o target/*/*.o
	rm -f target/*/*tbl.c
	rm -f as-*
	rm -f *.log

distclean: clean
	rm -f makefile

