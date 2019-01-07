.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET = $(BINDIR)/nm
LIBS   = -lmach

all: $(TARGET)

nm: $(LIBDIR)/libmach.a
$(BINDIR)/nm: nm
	cp nm $@

clean:
	rm -f nm

dep: inc-dep

include deps.mk
