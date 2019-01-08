.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

TARGET = $(BINDIR)/nm \
         $(BINDIR)/strip \
         $(BINDIR)/size \

LIBS   = -lmach

all: $(TARGET)

nm strip: $(LIBDIR)/libmach.a

$(BINDIR)/nm: nm
	cp nm $@

$(BINDIR)/strip: strip
	cp strip $@

$(BINDIR)/size: size
	cp size $@

clean:
	rm -f nm strip size

dep: inc-dep

include deps.mk
