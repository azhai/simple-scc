COMP = gcc
OBJCOPY = objcopy
OBJDUMP = objdump
ASM = as
LINKER = ld
RANLIB = ranlib
ARCHIVE = ar
PRECOMP = cpp

ARCHIVE_FLAGS = -U
NOPIE_CFLAGS = -fno-pie
TOOL_CFLAGS = -std=c99 -fno-stack-protector -static $(PROFILE_CFLAGS)
TOOL_LDLIBS = $(PROFILE_LDLIBS)
TOOL_LIBC_CFLAGS = -ffreestanding
