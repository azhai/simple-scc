include $(PROJECTDIR)/config/toolchain/gnu.mk

COMP = clang
ASM = clang
TOOLASFLAGS = -c
LINKER = ld.lld
OBJCOPY = llvm-objcopy
OBJDUMP = llvm-objdump
