# Define the target all as default
all:

# Define default configuration variables
CONF = amd64-linux
TOOL = unix
HOST = posix
ROFF = gnu

# Define helper macros for project directories
DOCDIR    = $(PROJECTDIR)/doc
INCDIR    = $(PROJECTDIR)/include
BINDIR    = $(PROJECTDIR)/bin
SRCDIR    = $(PROJECTDIR)/src
SCRIPTDIR = $(PROJECTDIR)/scripts
LIBDIR    = $(PROJECTDIR)/lib
LIBEXEC   = $(PROJECTDIR)/libexec
BUILDDIR  = $(PROJECTDIR)/scripts/build
CRTDIR    = $(PROJECTDIR)/lib/scc
LIBCDIR   = $(CRTDIR)/$(ARCH)-$(SYS)

# library dependences helpers
LIBMACH = $(LIBDIR)/scc/libmach.a
LIBSCC  = $(LIBDIR)/scc/libscc.a

# Include configuration definitions
include $(BUILDDIR)/conf/$(CONF).mk
include $(BUILDDIR)/tool/$(TOOL).mk
include $(BUILDDIR)/host/$(HOST).mk

# Locations for -I or -L in compiler, assembler or linker
CINCLUDES = -I$(INCDIR)/scc
ASINCLUDES= -I$(INCDIR)/scc
LDINCLUDES= -L$(LIBDIR)/scc

# C standard for the target compiler
STD = c99

# Definition of command line for cc, as, ld and emu
PROJ_CFLAGS =\
	$(MORE_CFLAGS)\
	$(HOST_CFLAGS)\
	$(SYS_CFLAGS)\
	$(TOOL_CFLAGS)\
	$(CINCLUDES)\
	$(CFLAGS)

PROJ_LDFLAGS =\
	$(MORE_LDFLAGS)\
	$(HOST_LDFLAGS)\
	$(SYS_LDFLAGS)\
	$(TOOL_LDFLAGS)\
	$(LDINCLUDES)\
	$(LDFLAGS)

PROJ_ASFLAGS =\
	$(MORE_ASFLAGS)\
	$(HOST_ASFLAGS)\
	$(SYS_ASFLAGS)\
	$(TOOL_ASFLAGS)\
	$(ASINCLUDES)\
	$(ASFLAGS)

PROJ_ARFLAGS =\
	$(MORE_ARFLAGS)\
	$(HOST_ARFLAGS)\
	$(SYS_ARFLAGS)\
	$(TOOL_ARFLAGS)\
	$(ARFLAGS)

PROJ_RLFLAGS =\
	$(MORE_RLFLAGS)\
	$(HOST_RLFLAGS)\
	$(SYS_RLFLAGS)\
	$(TOOL_RLFLAGS)\
	$(RLFLAGS)

PROJ_LDLIBS =\
	$(MORE_LDLIBS)\
	$(HOST_LDLIBS)\
	$(SYS_LDLIBS)\
	$(TOOL_LDLIBS)\
	$(LIBS)

# Definition of tools
CC = $(CROSS_COMPILE)$(COMP)
AS = $(CROSS_COMPILE)$(ASM)
LD = $(CROSS_COMPILE)$(LINKER)
OC = $(CROSS_COMPILE)$(OBJCOPY)
OD = $(CROSS_COMPILE)$(OBJDUMP)
RL = $(CROSS_COMPILE)$(RANLIB)
AR = $(CROSS_COMPILE)$(ARCHIVE)
CPP = $(CROSS_COMPILE)$(PRECOMP)
GS = gs

# helper macro to run over all the directories
FORALL = +@set -e ;\
	pwd=$$PWD; \
	. $(ENVIRON); \
	for i in $(DIRS); \
	do \
		cd $$i; \
		$(MAKE) $@; \
		cd $$pwd; \
	done

$(DIRS): FORCE
	+@cd $@ && $(MAKE)

# Generic rules
.SUFFIXES:
.SUFFIXES: .c .map .dump .elf .bin\
	   .i .a .o .s .S .ko .ld .tmpl\
	   .pdf .ps .eps .puml\
	   .ms .1 .2 .3 .4 .5 .6 .7\

.s.o:
	$(AS) $(PROJ_ASFLAGS) $< -o $@

.s.ko:
	$(AS) $(PROJ_ASFLAGS) $< -o $@

.c.o:
	$(CC) $(PROJ_CFLAGS) -o $@ -c $<

.c.s:
	$(CC) $(PROJ_CFLAGS) -S -o $@ $<

.c.i:
	$(CPP) $(PROJ_CFLAGS) -o $@ $<

.elf.bin:
	$(OC) -O binary $< $@

.o.dump:
	trap "rm -f $$$$.eps" EXIT QUIT INT TERM;\
	$(OD) -D $< > $$$$.dump && mv $$$$.dump $@

.elf.map:
	trap "rm -f $$$$.eps" EXIT QUIT INT TERM;\
	$(LD) -Map=$@ $< -o /dev/null

.puml.eps:
	trap "rm -f $$$$.eps" EXIT QUIT INT TERM;\
	plantuml -p -teps < $< > $$$$.eps && mv $$$$.eps $@

.ms.ps:
	trap "rm -f $$$$.ps" EXIT QUIT INT TERM;\
	$(SOELIM) $< | $(GRAP) | $(PIC) | $(EQN) | $(TBL) |\
	$(TROFF) -ms | $(DPOST) > $$$$.ps &&\
	mv $$$$.ps $@
.1.ps:
	trap "rm -f $$$$.ps" EXIT QUIT INT TERM;\
	$(TBL) $< | $(TROFF) -man | $(DPOST) > $$$$.ps && mv $$$$.ps $@

.2.ps:
	trap "rm -f $$$$.ps" EXIT QUIT INT TERM;\
	$(TBL) $< | $(TROFF) -man | $(DPOST) > $$$$.ps && mv $$$$.ps $@

.3.ps:
	trap "rm -f $$$$.ps" EXIT QUIT INT TERM;\
	$(TBL) $< | $(TROFF) -man | $(DPOST) > $$$$.ps && mv $$$$.ps $@

.4.ps:
	trap "rm -f $$$$.ps" EXIT QUIT INT TERM;\
	$(TBL) $< | $(TROFF) -man | $(DPOST) > $$$$.ps && mv $$$$.ps $@

.5.ps:
	trap "rm -f $$$$.ps" EXIT QUIT INT TERM;\
	$(TBL) $< | $(TROFF) -man | $(DPOST) > $$$$.ps && mv $$$$.ps $@

.6.ps:
	trap "rm -f $$$$.ps" EXIT QUIT INT TERM;\
	$(TBL) $< | $(TROFF) -man | $(DPOST) > $$$$.ps && mv $$$$.ps $@

.7.ps:
	trap "rm -f $$$$.ps" EXIT QUIT INT TERM;\
	$(TBL) $< | $(TROFF) -man | $(DPOST) > $$$$.ps && mv $$$$.ps $@

.ps.pdf:
	$(GS) -P- -dSAFER -q -P- -dNOPAUSE -dBATCH -sDEVICE=pdfwrite \
	    -ssdout=%stderr "-sOutputFile=$@" -P- -dSAFER "$<"

.eps.pdf:
	$(GS) -P- -dSAFER -q -P- -dNOPAUSE -dBATCH -sDEVICE=pdfwrite \
	    -ssdout=%stderr "-sOutputFile=$@" -P- -dSAFER "$<"
FORCE:

clean: clean-files clean-dirs

clean-dirs:
	+@set -e; \
	for i in $(DIRS); \
	do \
		cd $$i; \
		$(MAKE) clean; \
		cd -; \
	done

clean-files:
	rm -f *.i *.d *.o *.a *.ko *.elf $(TARGET)
	rm -f *.csmes *.csexe

dep:

inc-dep: FORCE
	mkdep
