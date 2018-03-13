.POSIX:

# Makefile is only used as a template for makefile.
# If you modify Makefile remember to run make dep

PROJECTDIR = ..

include $(PROJECTDIR)/rules.mk
include $(LIBDIR)/libdep.mk

MOREFLAGS = -I$(PROJECTDIR)/inc/$(STD) $(CC1_CFLAGS)

OBJ = types.o decl.o lex.o error.o symbol.o main.o expr.o \
      code.o stmt.o cpp.o fold.o init.o builtin.o

HDR = cc1.h \
      $(PROJECTDIR)/inc/scc.h \
      $(PROJECTDIR)/inc/$(STD)/cstd.h \
      $(PROJECTDIR)/inc/sysincludes.h

all:
	cp cc1-* $(PROJECTDIR)/rootdir/libexec/scc/

dep:
	./gendep.sh $(TARGETS)

clean:
	rm -f *.o
	rm -f target/*/*.o
	rm -f cc1-*

distclean: clean
	rm -f makefile
