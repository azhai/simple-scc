.POSIX:

PROJECTDIR = ../..
LIBCOFF32 = .
include $(PROJECTDIR)/rules.mk
include objlst.mk

TARGET = $(LIBDIR)/libcoff32.a

all: $(TARGET)

$(TARGET): $(LIBCOFF32-OBJ)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

dep:
	$(PROJECTDIR)/mkdep.sh

clean:
	rm -f *.o $(TARGET)

include deps.mk
