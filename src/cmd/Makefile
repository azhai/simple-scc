.POSIX:

DIRS   =\
	qbe\
	ld\
	as\
	cc\
	make\

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET =\
	$(BINDIR)/scc-nm\
	$(BINDIR)/scc-ar\
	$(BINDIR)/scc-strip\
	$(BINDIR)/scc-size\
	$(BINDIR)/scc-ranlib\
	$(BINDIR)/scc-objdump\
	$(BINDIR)/scc-objcopy\
	$(BINDIR)/scc-addr2line\
	$(LIBEXEC)/scc/qbe\

all: $(TARGET) $(DIRS)

qbe/.git:
	git submodule init qbe
	git submodule update qbe

qbe: qbe/.git

qbe/qbe: qbe

$(LIBEXEC)/scc/qbe: qbe/qbe
	cp qbe/qbe $@

$(BINDIR)/scc-nm: nm.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) nm.o -lmach -lscc $(PROJ_LDLIBS) -o $@

$(BINDIR)/scc-strip: strip.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) strip.o -lmach -lscc $(PROJ_LDLIBS) -o $@

$(BINDIR)/scc-size: size.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) size.o -lmach -lscc $(PROJ_LDLIBS) -o $@

$(BINDIR)/scc-ranlib: ranlib.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) ranlib.o -lmach -lscc $(PROJ_LDLIBS) -o $@

$(BINDIR)/scc-objdump: objdump.o $(LIBMACH)
	$(CC) $(PROJ_LDFLAGS) objdump.o -lmach $(PROJ_LDLIBS) -o $@

$(BINDIR)/scc-objcopy: objcopy.o $(LIBMACH)
	$(CC) $(PROJ_LDFLAGS) objcopy.o -lmach $(PROJ_LDLIBS) -o $@

$(BINDIR)/scc-addr2line: addr2line.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) addr2line.o -lmach -lscc $(PROJ_LDLIBS) -o $@

$(BINDIR)/scc-ar: ar.o $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) ar.o $(PROJ_LDLIBS) -lscc -o $@

include deps.mk
