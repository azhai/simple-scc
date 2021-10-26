/* Special Section Indexes */
#define SHN_UNDEF	0		/* undefined */
#define SHN_LORESERVE	0xff00		/* lower bounds of reserved indexes */
#define SHN_LOPROC	0xff00		/* reserved range for processor */
#define SHN_HIPROC	0xff1f		/*   specific section indexes */
#define SHN_ABS		0xfff1		/* absolute value */
#define SHN_COMMON	0xfff2		/* common symbol */
#define SHN_XINDEX	0xffff		/* Escape -- index stored elsewhere. */
#define SHN_HIRESERVE	0xffff		/* upper bounds of reserved indexes */

/* sh_type */
#define SHT_NULL		0	/* inactive */
#define SHT_PROGBITS		1	/* program defined information */
#define SHT_SYMTAB		2	/* symbol table section */
#define SHT_STRTAB		3	/* string table section */
#define SHT_RELA		4	/* relocation section with addends*/
#define SHT_HASH		5	/* symbol hash table section */
#define SHT_DYNAMIC		6	/* dynamic section */
#define SHT_NOTE		7	/* note section */
#define SHT_NOBITS		8	/* no space section */
#define SHT_REL			9	/* relation section without addends */
#define SHT_SHLIB		10	/* reserved - purpose unknown */
#define SHT_DYNSYM		11	/* dynamic symbol table section */
#define SHT_NUM			12	/* number of section types */
#define SHT_INIT_ARRAY		14	/* pointers to init functions */
#define SHT_FINI_ARRAY		15	/* pointers to termination functions */
#define SHT_PREINIT_ARRAY	16	/* ptrs to funcs called before init */
#define SHT_GROUP		17	/* defines a section group */
#define SHT_SYMTAB_SHNDX	18	/* Section indexes (see SHN_XINDEX). */
#define SHT_LOOS	0x60000000	/* reserved range for OS specific */
#define SHT_SUNW_dof	0x6ffffff4	/* used by dtrace */
#define SHT_GNU_LIBLIST	0x6ffffff7	/* libraries to be prelinked */
#define SHT_SUNW_move	0x6ffffffa	/* inf for partially init'ed symbols */
#define SHT_SUNW_syminfo	0x6ffffffc	/* ad symbol information */
#define SHT_SUNW_verdef		0x6ffffffd	/* symbol versioning inf */
#define SHT_SUNW_verneed	0x6ffffffe	/* symbol versioning req */
#define SHT_SUNW_versym		0x6fffffff	/* symbol versioning table */
#define SHT_HIOS	0x6fffffff	/*  section header types */
#define SHT_LOPROC	0x70000000	/* reserved range for processor */
#define SHT_HIPROC	0x7fffffff	/*  specific section header types */
#define SHT_LOUSER	0x80000000	/* reserved range for application */
#define SHT_HIUSER	0xffffffff	/*  specific indexes */

#define SHT_GNU_HASH	0x6ffffff6	/* GNU-style hash table section */

/* Section Attribute Flags - sh_flags */
#define SHF_WRITE		0x1	/* Writable */
#define SHF_ALLOC		0x2	/* occupies memory */
#define SHF_EXECINSTR		0x4	/* executable */
#define SHF_MERGE		0x10	/* may be merged */
#define SHF_STRINGS		0x20	/* contains strings */
#define SHF_INFO_LINK		0x40	/* sh_info holds section index */
#define SHF_LINK_ORDER		0x80	/* ordering requirements */
#define SHF_OS_NONCONFORMING	0x100	/* OS-specific processing required */
#define SHF_GROUP		0x200	/* member of section group */
#define SHF_TLS			0x400	/* thread local storage */
#define SHF_COMPRESSED		0x800	/* contains compressed data */
#define SHF_MASKOS	0x0ff00000	/* OS-specific semantics */
#define SHF_MASKPROC	0xf0000000	/* reserved bits for processor */
					/*  specific section attributes */

#define ELFS32SZ                40
#define ELFS64SZ                64

typedef struct elf32_shdr Elf32_Shdr;
typedef struct elf64_shdr Elf64_Shdr;

/* Section Header */
struct elf32_shdr {
	Elf32_Word	sh_name;	/* section name */
	Elf32_Word	sh_type;	/* type */
	Elf32_Word	sh_flags;	/* flags */
	Elf32_Addr	sh_addr;	/* address */
	Elf32_Off	sh_offset;	/* file offset */
	Elf32_Word	sh_size;	/* section size */
	Elf32_Word	sh_link;	/* section header table index link */
	Elf32_Word	sh_info;	/* extra information */
	Elf32_Word	sh_addralign;	/* address alignment */
	Elf32_Word	sh_entsize;	/* section entry size */
};

struct elf64_shdr {
	Elf64_Word	sh_name;	/* section name */
	Elf64_Word	sh_type;	/* section type */
	Elf64_Xword	sh_flags;	/* section flags */
	Elf64_Addr	sh_addr;	/* virtual address */
	Elf64_Off	sh_offset;	/* file offset */
	Elf64_Xword	sh_size;	/* section size */
	Elf64_Word	sh_link;	/* link to another */
	Elf64_Word	sh_info;	/* misc info */
	Elf64_Xword	sh_addralign;	/* memory alignment */
	Elf64_Xword	sh_entsize;	/* table entry size */
};
