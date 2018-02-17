.POSIX:

# Makefile is only used as a template for makefile.
# If you modify Makefile remember to run make dep

PROJECTDIR = ..
LIBDIR = $(PROJECTDIR)/lib/scc
include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

MOREFLAGS = -I$(PROJECTDIR)/inc/$(STD) $(CC2_CFLAGS)

OBJ = main.o parser.o peep.o symbol.o node.o code.o optm.o
HDR = cc2.h $(PROJECTDIR)/inc/$(STD)/cstd.h $(PROJECTDIR)/inc/scc.h

all:
	mkdir -p $(PROJECTDIR)/rootdir/libexec/scc/
	cp cc2-* $(PROJECTDIR)/rootdir/libexec/scc/

dep:
	MKQBE=${MKQBE} ./gendep.sh $(TARGETS)

$(LIBDIR)/libscc.a: $(LIB-OBJ)
	+cd $(LIBDIR) && $(MAKE)

main.o: error.h

error.h: cc2.h
	rm -f $@; trap 'rm -f $$$$.h' EXIT INT QUIT ;\
	awk -f generror.awk cc2.h > $$$$.h && mv $$$$.h $@

clean:
	rm -f *.o
	rm -f target/*/*.o
	rm -f cc2-* error.h

distclean: clean
	rm -f makefile
