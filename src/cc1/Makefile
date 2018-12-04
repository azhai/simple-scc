.POSIX:

PROJECTDIR = ../..
include $(PROJECTDIR)/scripts/rules.mk

OBJS = types.o \
       decl.o \
       lex.o \
       error.o \
       symbol.o \
       main.o \
       expr.o \
       code.o \
       stmt.o \
       cpp.o \
       fold.o \
       init.o \
       builtin.o \

TARGET   = $(LIBEXEC)/cc1-amd64-sysv \
           $(LIBEXEC)/cc1-arm64-sysv \
           $(LIBEXEC)/cc1-i386-sysv \
           $(LIBEXEC)/cc1-z80-scc \

all: $(TARGET)

$(TARGET): $(LIBDIR)/libscc.a

dep: inc-dep

clean:
	rm -f target/*/*.o

include target/amd64-sysv/arch.mk
include target/arm64-sysv/arch.mk
include target/i386-sysv/arch.mk
include target/z80-scc/arch.mk
include deps.mk
