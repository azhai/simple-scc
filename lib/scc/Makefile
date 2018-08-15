
PROJECTDIR = ../..
LIBSCC = .
include $(PROJECTDIR)/rules.mk
include objlst.mk

all: libscc.a

libscc.a: $(LIBSCC-OBJ)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f *.o *.a

include deps.mk
