/* Symbol table index */
#define STN_UNDEF       0               /* undefined */

/* Extract symbol info - st_info */
#define ELF32_ST_BIND(x)        ((x) >> 4)
#define ELF32_ST_TYPE(x)        (((unsigned int) x) & 0xf)
#define ELF32_ST_INFO(b,t)      (((b) << 4) + ((t) & 0xf))

#define ELF64_ST_BIND(x)        ((x) >> 4)
#define ELF64_ST_TYPE(x)        (((unsigned int) x) & 0xf)
#define ELF64_ST_INFO(b,t)      (((b) << 4) + ((t) & 0xf))

/* Symbol Binding - ELF32_ST_BIND - st_info */
#define STB_LOCAL       0               /* Local symbol */
#define STB_GLOBAL      1               /* Global symbol */
#define STB_WEAK        2               /* like global - lower precedence */
#define STB_NUM         3               /* number of symbol bindings */
#define STB_LOPROC      13              /* reserved range for processor */
#define STB_HIPROC      15              /*  specific symbol bindings */

/* Symbol type - ELF32_ST_TYPE - st_info */
#define STT_NOTYPE      0               /* not specified */
#define STT_OBJECT      1               /* data object */
#define STT_FUNC        2               /* function */
#define STT_SECTION     3               /* section */
#define STT_FILE        4               /* file */
#define STT_COMMON      5               /* common symbol */
#define STT_TLS         6               /* thread local storage */
#define STT_LOPROC      13              /* reserved range for processor */
#define STT_HIPROC      15              /*  specific symbol types */

/* Extract symbol visibility - st_other */
#define ELF_ST_VISIBILITY(v)            ((v) & 0x3)
#define ELF32_ST_VISIBILITY             ELF_ST_VISIBILITY
#define ELF64_ST_VISIBILITY             ELF_ST_VISIBILITY

#define STV_DEFAULT     0               /* Visibility set by binding type */
#define STV_INTERNAL    1               /* OS specific version of STV_HIDDEN */
#define STV_HIDDEN      2               /* can only be seen inside own .so */
#define STV_PROTECTED   3               /* HIDDEN inside, DEFAULT outside */

#define ELFE32SZ        16
#define ELFE64SZ        24

typedef struct elf32_sym Elf32_Sym;
typedef struct elf64_sym Elf64_Sym;

/* Symbol Table Entry */
struct elf32_sym {
	Elf32_Word      st_name;        /* name - index into string table */
	Elf32_Addr      st_value;       /* symbol value */
	Elf32_Word      st_size;        /* symbol size */
	unsigned char   st_info;        /* type and binding */
	unsigned char   st_other;       /* 0 - no defined meaning */
	Elf32_Half      st_shndx;       /* section header index */
};

struct elf64_sym {
	Elf64_Word      st_name;        /* Symbol name index in str table */
	unsigned char   st_info;        /* type / binding attrs */
	unsigned char   st_other;       /* unused */
	Elf64_Half      st_shndx;       /* section index of symbol */
	Elf64_Addr      st_value;       /* value of symbol */
	Elf64_Xword     st_size;        /* size of symbol */
};
