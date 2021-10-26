.POSIX:

DIRS =\
	coff32\
	elf64\

PROJECTDIR =../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET = $(LIBDIR)/scc/libmach.a

OBJS =\
	mach.o\
	newobj.o \
	delobj.o\
	objpos.o\
	archive.o\
	armember.o\
	objtype.o\
	readobj.o\
	writeobj.o\
	getsym.o\
	getsec.o\
	rebase.o\
	loadmap.o\
	newmap.o\
	setmap.o\
	findsec.o\
	strip.o\
	pc2line.o\
	pack.o\
	unpack.o\
	setindex.o\
	getindex.o\


all: $(OBJS) $(DIRS) objlst.mk
	+@$(MAKE) -f Makefile.mach

objlst.mk: $(OBJS) $(DIRS)
	./mklst $@

clean:
	rm -f $(TBLS) objlst.mk

include deps.mk
