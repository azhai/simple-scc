.POSIX:

PROJECTDIR = ../..
LIBDIR = .
include $(PROJECTDIR)/rules.mk
include objlst.mk

all: libscc.a

$(LIB-OBJ): $(PROJECTDIR)/inc/scc.h
$(LIBDIR)/wmyro.o: $(PROJECTDIR)/inc/myro.h
$(LIBDIR)/rmyro.o: $(PROJECTDIR)/inc/myro.h

libscc.a: $(LIB-OBJ)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

dep:

clean:
	rm -f *.o *.a

distclean: clean
