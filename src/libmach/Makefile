.POSIX:
PROJECTDIR =../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET = $(LIBDIR)/libmach.a

OBJS = addr2line.o \
       archive.o \
       armember.o \
       objdel.o \
       objlookup.o \
       objnew.o \
       objpos.o \
       objread.o \
       objreset.o \
       objstrip.o \
       objsect.o \
       getindex.o \
       setindex.o \
       namindex.o \
       forsym.o \
       formember.o \
       objtype.o \
       objwrite.o \
       objfree.o \
       pack.o \
       unpack.o \
       new.o \
       read.o \
       del.o \
       strip.o \
       probe.o \
       write.o \
       getidx.o \
       setidx.o \
       namidx.o \
       getsect.o \


DIRS = coff32

TBLS = setidx.c \
       getidx.c \
       getsect.c \
       namidx.c \
       new.c \
       read.c \
       del.c \
       strip.c \
       probe.c \
       write.c \


all: $(OBJS) $(DIRS) objlst.mk
	+@$(MAKE) -f Makefile.mach

$(DIRS): FORCE
	+@cd $@ && $(MAKE)

$(TBLS): formats.lst
	mktbl -t `echo $@ | sed 's/\.c//'` -o $@

objlst.mk: $(OBJS) $(DIRS)
	mklst $@

dep: inc-dep
	$(FORALL)

clean:
	$(FORALL)
	rm -f $(TBLS) objlst.mk


include deps.mk
