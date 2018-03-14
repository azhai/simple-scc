.POSIX:

# Makefile is only used as a template for makefile.
# If you modify Makefile remember to run make dep

PROJECTDIR = ..
include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

MOREFLAGS = -I$(PROJECTDIR)/inc/$(STD) $(CC2_CFLAGS)

OBJ = main.o parser.o peep.o symbol.o node.o code.o optm.o
HDR = cc2.h $(PROJECTDIR)/inc/$(STD)/cstd.h $(PROJECTDIR)/inc/scc.h

all:
	cp cc2-* $(PROJECTDIR)/rootdir/libexec/scc/

dep:
	MKQBE=${MKQBE} ./gendep.sh $(TARGETS)

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
