.POSIX:

PROJECTDIR =../..
include $(PROJECTDIR)/scripts/rules.mk
include coff32/rules.mk
include elf64/rules.mk

TARGET = $(LIBDIR)/scc/libmach.a

all: $(TARGET)

OBJS =\
	archive.o\
	armember.o\
	delobj.o\
	findsec.o\
	getindex.o\
	getsec.o\
	getsym.o\
	loadmap.o\
	mach.o\
	newmap.o\
	newobj.o \
	newobj.o\
	objpos.o\
	objprobe.o\
	pack.o\
	pc2line.o\
	readobj.o\
	rebase.o\
	setindex.o\
	setmap.o\
	strip.o\
	unpack.o\
	writeobj.o\
	$(COFF32_OBJS)\
	$(ELF64_OBJS)\

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(PROJ_ARFLAGS) $@ $?
	$(RL) $(PROJ_RLFLAGS) $@

clean:
	rm -f $(TBLS) objlst.mk
	rm -f */*.o

include deps.mk
