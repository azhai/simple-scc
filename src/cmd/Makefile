.POSIX:

DIRS   =\
	qbe\
	ld\
	as\
	cc\

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET =\
	$(BINDIR)/nm\
	$(BINDIR)/ar\
	$(BINDIR)/strip\
	$(BINDIR)/size\
	$(BINDIR)/ranlib\
	$(BINDIR)/objdump\
	$(BINDIR)/objcopy\
	$(BINDIR)/addr2line\
	$(BINDIR)/qbe\

all: $(TARGET) $(DIRS)

qbe: qbe/.git

qbe/.git:
	git submodule init qbe
	git submodule update

qbe/obj/qbe: qbe

$(BINDIR)/qbe: qbe/obj/qbe
	cp qbe/obj/qbe $@

$(BINDIR)/nm: nm.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) nm.o -lmach -lscc -o $@

$(BINDIR)/strip: strip.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) strip.o -lmach -lscc -o $@

$(BINDIR)/size: size.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) size.o -lmach -lscc -o $@

$(BINDIR)/ranlib: ranlib.o $(DRIVER).o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) ranlib.o $(DRIVER).o -lmach -lscc -o $@

$(BINDIR)/objdump: objdump.o $(LIBMACH)
	$(CC) $(PROJ_LDFLAGS) objdump.o -lmach -o $@

$(BINDIR)/objcopy: objcopy.o $(LIBMACH)
	$(CC) $(PROJ_LDFLAGS) objcopy.o -lmach -o $@

$(BINDIR)/addr2line: addr2line.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) addr2line.o -lmach -lscc -o $@

$(BINDIR)/ar: ar.o $(DRIVER).o
	$(CC) $(PROJ_LDFLAGS) ar.o $(DRIVER).o -o $@

clean: qbe/.git
	git submodule deinit qbe

include deps.mk
