.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS = coff32_pack_hdr.o \
       coff32_unpack_hdr.o \
       coff32_pack_scn.o \
       coff32_unpack_scn.o \
       coff32_pack_aout.o \
       coff32_unpack_aout.o \
       coff32_pack_ent.o \
       coff32_unpack_ent.o \

TARGET = $(LIBDIR)/libcoff32.a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

dep: inc-dep

include deps.mk
