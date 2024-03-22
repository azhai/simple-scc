#include <stdint.h>

typedef uint32_t Elf32_Addr;	/* Unsigned program address */
typedef uint32_t Elf32_Off;	/* Unsigned file offset */
typedef int32_t Elf32_Sword;	/* Signed large integer */
typedef uint32_t Elf32_Word;	/* Unsigned large integer */
typedef uint16_t Elf32_Half;	/* Unsigned medium integer */
typedef uint64_t Elf32_Lword;

typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;
typedef int32_t Elf64_Shalf;

typedef int32_t Elf64_Sword;
typedef uint32_t Elf64_Word;

typedef int64_t Elf64_Sxword;
typedef uint64_t Elf64_Xword;
typedef uint64_t Elf64_Lword;

typedef uint16_t Elf64_Half;
