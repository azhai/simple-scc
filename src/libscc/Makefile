
PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS = debug.o \
       die.o \
       newitem.o \
       xcalloc.o \
       xmalloc.o \
       xrealloc.o \
       xstrdup.o \
       alloc.o \
       casecmp.o \
       lunpack.o \
       lpack.o \
       bunpack.o \
       bpack.o \
       wmyro.o \
       rmyro.o \

TARGET = $(LIBDIR)/libscc.a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

dep: inc-dep

include deps.mk
