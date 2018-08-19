.POSIX:

PROJECTDIR = ../..
LIBCOFF32 = .
include $(PROJECTDIR)/rules.mk
include objlst.mk

MORECFLAGS = $(STDCFLAGS)

all: libcoff32.a

libcoff32.a: $(LIBCOFF32-OBJ)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f *.o *.a

include deps.mk
