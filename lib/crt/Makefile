.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/rules.mk

#TARGETS = $(LIBDIR)/amd64-sysv-linux/crt.o \
#          $(LIBDIR)/amd64-sysv-openbsd/crt.o \
#          $(LIBDIR)/i386-sysv-linux/crt.o \
#          $(LIBDIR)/i386-sysv-openbsd/crt.o \
#          $(LIBDIR)/z80-scc-none/z80-scc-none/crt.o

TARGETS = $(LIBDIR)/amd64-sysv-linux/crt.o

all: $(TARGETS)

$(LIBDIR)/amd64-sysv-linux/crt.o: amd64-sysv-linux/crt.o
	cp amd64-sysv-linux/crt.o $@

$(LIBDIR)/amd64-sysv-openbsd/crt.o: amd64-sysv-openbsd/crt.o
	cp amd64-sysv-openbsd/crt.o $@

$(LIBDIR)/i386-sysv-linux/crt.o: i386-sysv-linux/crt.o
	cp i386-sysv-linux/crt.o $@

$(LIBDIR)/i386-sysv-openbsd/crt.o: i386-sysv-openbsd/crt.o
	cp i386-sysv-openbsd/crt.o $@

$(LIBDIR)/z80-scc-none/crt.o: z80-scc-none/crt.o
	cp z80-scc-none/crt.o $@

clean:
	rm -f */*.o
	rm -f $(TARGETS)
