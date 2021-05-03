TOOL_CFLAGS = -std=c99

COMP = gcc
ASM = as
LINKER = ld
RANLIB = ranlib
ARCHIVE = ar

ARCHIVE_FLAGS = -U
NOPIE_CFLAGS = -nopie
NOPIE_LDFLAGS = -nopie
TOOL_CFLAGS = -std=c99 -fno-stack-protector -static
