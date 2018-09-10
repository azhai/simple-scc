
PROJECTDIR = ../..
LIBSCC = .
include $(PROJECTDIR)/rules.mk
include objlst.mk

TARGET = $(LIBDIR)/libscc.a

all: $(TARGET)

$(TARGET): $(LIBSCC-OBJ)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f *.o $(LIBDIR)/libscc.a

include deps.mk
