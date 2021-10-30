.POSIX:

DIRS   =\
	qbe\
	ld\
	as\
	cc\

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
	$(BINDIR)/scc\
	$(LIBEXEC)/scc/qbe\

all: $(TARGET) $(DIRS)

qbe: qbe/.git

qbe/.git:
	git submodule init qbe
	git submodule update qbe

qbe/obj/qbe: qbe

$(LIBEXEC)/scc/qbe: qbe/obj/qbe
	cp qbe/obj/qbe $@

$(BINDIR)/scc: scc.sh
	cp scc.sh $@
	chmod +x $@

$(BINDIR)/scc-nm: nm.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) nm.o -lmach -lscc -o $@

$(BINDIR)/scc-strip: strip.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) strip.o -lmach -lscc -o $@

$(BINDIR)/scc-size: size.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) size.o -lmach -lscc -o $@

$(BINDIR)/scc-ranlib: ranlib.o $(DRIVER).o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) ranlib.o $(DRIVER).o -lmach -lscc -o $@

$(BINDIR)/scc-objdump: objdump.o $(LIBMACH)
	$(CC) $(PROJ_LDFLAGS) objdump.o -lmach -o $@

$(BINDIR)/scc-objcopy: objcopy.o $(LIBMACH)
	$(CC) $(PROJ_LDFLAGS) objcopy.o -lmach -o $@

$(BINDIR)/scc-addr2line: addr2line.o $(LIBMACH) $(LIBSCC)
	$(CC) $(PROJ_LDFLAGS) addr2line.o -lmach -lscc -o $@

$(BINDIR)/scc-ar: ar.o $(DRIVER).o
	$(CC) $(PROJ_LDFLAGS) ar.o $(DRIVER).o -o $@

clean: qbe/.git
	git submodule deinit qbe

include deps.mk
