COMP = gcc
ASM = as
LINKER = ld
RANLIB = ranlib
ARCHIVE = ar
PRECOMP = cpp

ARCHIVE_FLAGS = -U
NOPIE_CFLAGS = -fno-pie
TOOL_CFLAGS = -std=c99 -fno-stack-protector -static
TOOL_LIBC_CFLAGS = -ffreestanding
