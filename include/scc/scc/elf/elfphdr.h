/* See http://www.sco.com/developers/gabi/latest/contents.html */

/* Segment types - p_type */
#define PT_NULL         0               /* unused */
#define PT_LOAD         1               /* loadable segment */
#define PT_DYNAMIC      2               /* dynamic linking section */
#define PT_INTERP       3               /* the RTLD */
#define PT_NOTE         4               /* auxiliary information */
#define PT_SHLIB        5               /* reserved - purpose undefined */
#define PT_PHDR         6               /* program header */
#define PT_TLS          7               /* thread local storage */
#define PT_LOOS         0x60000000      /* reserved range for OS */
#define PT_HIOS         0x6fffffff      /*  specific segment types */
#define PT_LOPROC       0x70000000      /* reserved range for processor */
#define PT_HIPROC       0x7fffffff      /*  specific segment types */

#define ELFP32SZ        32
#define ELFP64SZ        56

typedef struct elf32_phdr Elf32_Phdr;
typedef struct elf64_phdr Elf64_Phdr;

/* Program Header */
struct elf32_phdr {
	Elf32_Word p_type;              /* segment type */
	Elf32_Off p_offset;             /* segment offset */
	Elf32_Addr p_vaddr;             /* virtual address of segment */
	Elf32_Addr p_paddr;             /* physical address - ignored? */
	Elf32_Word p_filesz;            /* number of bytes in file for seg. */
	Elf32_Word p_memsz;             /* number of bytes in mem. for seg. */
	Elf32_Word p_flags;             /* flags */
	Elf32_Word p_align;             /* memory alignment */
};

struct elf64_phdr {
	Elf64_Word p_type;              /* entry type */
	Elf64_Word p_flags;             /* flags */
	Elf64_Off p_offset;             /* offset */
	Elf64_Addr p_vaddr;             /* virtual address */
	Elf64_Addr p_paddr;             /* physical address */
	Elf64_Xword p_filesz;           /* file size */
	Elf64_Xword p_memsz;            /* memory size */
	Elf64_Xword p_align;            /* memory & file alignment */
};
