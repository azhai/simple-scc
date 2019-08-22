.POSIX:
PROJECTDIR =../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET = $(LIBDIR)/libmach.a

OBJS = objnew.o \
       objpos.o \
       objread.o \
       objfree.o \
       objstrip.o \
       objdel.o \
       objaddseg.o \
       objsync.o \
       addr2line.o \
       archive.o \
       armember.o \
       objlookup.o \
       getindex.o \
       setindex.o \
       formember.o \
       objtype.o \
       objwrite.o \
       pack.o \
       unpack.o \

DIRS = coff32


all: $(OBJS) $(DIRS) objlst.mk
	+@$(MAKE) -f Makefile.mach

$(DIRS): FORCE
	+@cd $@ && $(MAKE)

objlst.mk: $(OBJS) $(DIRS)
	mklst $@

dep: inc-dep
	$(FORALL)

clean:
	$(FORALL)
	rm -f $(TBLS) objlst.mk


include deps.mk
