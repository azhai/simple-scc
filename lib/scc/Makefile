

PROJECTDIR = ../..
LIBSCC = .
include $(PROJECTDIR)/rules.mk
include objlst.mk

all: libscc.a

$(LIBSCC-OBJ): $(INCDIR)/scc.h
$(LIBSCC)/wmyro.o: $(INCDIR)/myro.h
$(LIBSCC)/rmyro.o: $(INCDIR)/myro.h

libscc.a: $(LIBSCC-OBJ)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

dep:

clean:
	rm -f *.o *.a
