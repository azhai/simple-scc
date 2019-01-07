.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET = $(BINDIR)/nm \
         $(BINDIR)/strip \

LIBS   = -lmach

all: $(TARGET)

nm strip: $(LIBDIR)/libmach.a

$(BINDIR)/nm: nm
	cp nm $@

$(BINDIR)/strip: strip
	cp strip $@

clean:
	rm -f nm strip

dep: inc-dep

include deps.mk
