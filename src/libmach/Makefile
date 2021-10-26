.POSIX:

DIRS =\
	coff32\
	elf64\

PROJECTDIR =../..
include $(PROJECTDIR)/scripts/rules.mk

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
	objtype.o\
	pack.o\
	pc2line.o\
	readobj.o\
	rebase.o\
	setindex.o\
	setmap.o\
	strip.o\
	unpack.o\
	writeobj.o\
	coff32/coff32.o\
	coff32/coff32del.o\
	coff32/coff32getidx.o\
	coff32/coff32getsec.o\
	coff32/coff32getsym.o\
	coff32/coff32loadmap.o\
	coff32/coff32new.o\
	coff32/coff32pc2line.o\
	coff32/coff32probe.o\
	coff32/coff32read.o\
	coff32/coff32setidx.o\
	coff32/coff32strip.o\
	coff32/coff32write.o\
	coff32/coff32xgetidx.o\
	coff32/coff32xsetidx.o\
	elf64/elf64.o\
	elf64/elf64new.o\

all: $(TARGET)

$(OBJS): $(DIRS)

$(TARGET): $(OBJS)
	$(AR) $(PROJ_ARFLAGS) $@ $?
	$(RL) $(PROJ_RLFLAGS) $@

clean:
	rm -f $(TBLS) objlst.mk

include deps.mk
