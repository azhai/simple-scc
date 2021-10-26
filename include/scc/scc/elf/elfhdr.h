/* See http://www.sco.com/developers/gabi/latest/contents.html */

/* e_ident[] identification indexes */
#define EI_MAG0                 0       /* file ID */
#define EI_MAG1                 1       /* file ID */
#define EI_MAG2                 2       /* file ID */
#define EI_MAG3                 3       /* file ID */
#define EI_CLASS                4       /* file class */
#define EI_DATA                 5       /* data encoding */
#define EI_VERSION              6       /* ELF header version */
#define EI_OSABI                7       /* OS/ABI ID */
#define EI_ABIVERSION           8       /* ABI version */
#define EI_PAD                  9       /* start of pad bytes */
#define EI_NIDENT               16      /* Size of e_ident[] */

/* e_ident[] magic number */
#define ELFMAG0                 0x7f    /* e_ident[EI_MAG0] */
#define ELFMAG1                 'E'     /* e_ident[EI_MAG1] */
#define ELFMAG2                 'L'     /* e_ident[EI_MAG2] */
#define ELFMAG3                 'F'     /* e_ident[EI_MAG3] */
#define ELFMAG                  "\177ELF" /* magic */
#define SELFMAG                 4       /* size of magic */

/* e_ident[] file class */
#define ELFCLASSNONE            0       /* invalid */
#define ELFCLASS32              1       /* 32-bit objs */
#define ELFCLASS64              2       /* 64-bit objs */

/* e_ident[] data encoding */
#define ELFDATANONE             0       /* invalid */
#define ELFDATA2LSB             1       /* Little-Endian */
#define ELFDATA2MSB             2       /* Big-Endian */

/* e_ident[] Operating System/ABI */
#define ELFOSABI_SYSV		0	/* UNIX System V ABI */
#define ELFOSABI_HPUX		1	/* HP-UX operating system */
#define ELFOSABI_NETBSD		2	/* NetBSD */
#define ELFOSABI_LINUX		3	/* GNU/Linux */
#define ELFOSABI_HURD		4	/* GNU/Hurd */
#define ELFOSABI_86OPEN		5	/* 86Open common IA32 ABI */
#define ELFOSABI_SOLARIS	6	/* Solaris */
#define ELFOSABI_MONTEREY	7	/* Monterey */
#define ELFOSABI_IRIX		8	/* IRIX */
#define ELFOSABI_FREEBSD	9	/* FreeBSD */
#define ELFOSABI_TRU64		10	/* TRU64 UNIX */
#define ELFOSABI_MODESTO	11	/* Novell Modesto */
#define ELFOSABI_OPENBSD	12	/* OpenBSD */
#define ELFOSABI_OPENVMS	13	/* Open VMS */
#define ELFOSABI_NSK	        14	/* Hewlett-Packard Non-Stop Kernel */
#define ELFOSABI_AROS	        15	/* Amiga Research OS */
#define ELFOSABI_FENIXOS	16	/* The FenixOS multi-core OS */
#define ELFOSABI_CLOUDABI	17	/* Nuxi CloudABI */
#define ELFOSABI_OPENVOS	18	/* Stratus Technologies OpenVOS */
#define ELFOSABI_ARM		97	/* ARM */
#define ELFOSABI_STANDALONE	255	/* Standalone (embedded) application */

/* e_ident */
#define IS_ELF(ehdr) ((ehdr).e_ident[EI_MAG0] == ELFMAG0 && \
                      (ehdr).e_ident[EI_MAG1] == ELFMAG1 && \
                      (ehdr).e_ident[EI_MAG2] == ELFMAG2 && \
                      (ehdr).e_ident[EI_MAG3] == ELFMAG3)

/* e_machine */
#define EM_NONE                 0       /* No machine */
#define EM_M32                  1       /* AT&T WE 32100 */
#define EM_SPARC                2       /* SPARC */
#define EM_386                  3       /* Intel 80386 */
#define EM_68K                  4       /* Motorola 68000 */
#define EM_88K                  5       /* Motorola 88000 */
#define EM_IAMCU                6       /* Intel MCU */
#define EM_860                  7       /* Intel 80860 */
#define EM_MIPS                 8       /* MIPS I Architecture */
#define EM_S370                 9       /* IBM System/370 Processor */
#define EM_MIPS_RS3_LE         10       /* MIPS RS3000 Little-endian */
#define EM_PARISC              15       /* Hewlett-Packard PA-RISC */
#define EM_VPP500              17       /* Fujitsu VPP500 */
#define EM_SPARC32PLUS         18       /* Enhanced instruction set SPARC */
#define EM_960                 19       /* Intel 80960 */
#define EM_PPC                 20       /* PowerPC */
#define EM_PPC64               21       /* 64-bit PowerPC */
#define EM_S390                22       /* IBM System/390 */
#define EM_SPU                 23       /* IBM SPU/SPC */
#define EM_V800                36       /* NEC V800 */
#define EM_FR20                37       /* Fujitsu FR20 */
#define EM_RH32                38       /* TRW RH-32 */
#define EM_RCE                 39       /* Motorola RCE */
#define EM_ARM                 40       /* ARM AARCH32 */
#define EM_ALPHA               41       /* Digital Alpha */
#define EM_SH                  42       /* Hitachi SH */
#define EM_SPARCV9             43       /* SPARC Version 9 */
#define EM_TRICORE             44       /* Siemens TriCore */
#define EM_ARC                 45       /* Argonaut RISC Core */
#define EM_H8_300              46       /* Hitachi H8/300 */
#define EM_H8_300H             47       /* Hitachi H8/300H */
#define EM_H8S                 48       /* Hitachi H8S */
#define EM_H8_500              49       /* Hitachi H8/500 */
#define EM_IA_64               50       /* Intel IA-64 */
#define EM_MIPS_X              51       /* Stanford MIPS-X */
#define EM_COLDFIRE            52       /* Motorola ColdFire */
#define EM_68HC12              53       /* Motorola M68HC12 */
#define EM_MMA                 54       /* Fujitsu MMA */
#define EM_PCP                 55       /* Siemens PCP */
#define EM_NCPU                56       /* Sony nCPU */
#define EM_NDR1                57       /* Denso NDR1 */
#define EM_STARCORE            58       /* Motorola Star*Core */
#define EM_ME16                59       /* Toyota ME16 */
#define EM_ST100               60       /* STMicroelectronics ST100 */
#define EM_TINYJ               61       /* Advanced Logic Corp. TinyJ */
#define EM_X86_64              62       /* AMD x86-64 */
#define EM_PDSP                63       /* Sony DSP Processor */
#define EM_PDP10               64       /* DEC PDP-10 */
#define EM_PDP11               65       /* DEC PDP-11 */
#define EM_FX66                66       /* Siemens FX66 */
#define EM_ST9PLUS             67       /* STMicroelectronics ST9+ */
#define EM_ST7                 68       /* STMicroelectronics ST7 */
#define EM_68HC16              69       /* Motorola MC68HC16 */
#define EM_68HC11              70       /* Motorola MC68HC11 */
#define EM_68HC08              71       /* Motorola MC68HC08 */
#define EM_68HC05              72       /* Motorola MC68HC05 */
#define EM_SVX                 73       /* Silicon Graphics SVx */
#define EM_ST19                74       /* STMicroelectronics ST19 */
#define EM_VAX                 75       /* Digital VAX */
#define EM_CRIS                76       /* Axis Communications 32-bit */
#define EM_JAVELIN             77       /* Infineon Technologies 32-bit */
#define EM_FIREPATH            78       /* Element 14 64-bit DSP Processor */
#define EM_ZSP                 79       /* LSI Logic 16-bit DSP Processor */
#define EM_MMIX                80       /* Donald Knuth's educational 64-bit */
#define EM_HUANY               81       /* Harvard machine-independent */
#define EM_PRISM               82       /* SiTera Prism */
#define EM_AVR                 83       /* Atmel AVR 8-bit */
#define EM_FR30                84       /* Fujitsu FR30 */
#define EM_D10V                85       /* Mitsubishi D10V */
#define EM_D30V                86       /* Mitsubishi D30V */
#define EM_V850                87       /* NEC v850 */
#define EM_M32R                88       /* Mitsubishi M32R */
#define EM_MN10300             89       /* Matsushita MN10300 */
#define EM_MN10200             90       /* Matsushita MN10200 */
#define EM_PJ                  91       /* picoJava */
#define EM_OPENRISC            92       /* OpenRISC 32-bit */
#define EM_ARC_A5              93       /* ARC ARCompact */
#define EM_ARC_COMPACT         93       /* ARC ARCompact */
#define EM_XTENSA              94       /* Tensilica Xtensa */
#define EM_VIDEOCORE           95       /* Alphamosaic VideoCore */
#define EM_TMM_GPP             96       /* Thompson Multimedia GPP */
#define EM_NS32K               97       /* National 32000 series */
#define EM_TPC                 98       /* Tenor Network TPC */
#define EM_SNP1K               99       /* Trebia SNP 1000 */
#define EM_ST200              100       /* STMicroelectronics ST200 */
#define EM_IP2K               101       /* Ubicom IP2xxx */
#define EM_MAX                102       /* MAX Processor */
#define EM_CR                 103       /* National CompactRISC */
#define EM_F2MC16             104       /* Fujitsu F2MC16 */
#define EM_MSP430             105       /* Texas msp430 */
#define EM_BLACKFIN           106       /* Analog Devices Blackfin */
#define EM_SE_C33             107       /* S1C33 of Seiko Epson */
#define EM_SEP                108       /* Sharp embedded */
#define EM_ARCA               109       /* Arca RISC */
#define EM_UNICORE            110       /* PKU-Unity Ltd. and MPRC */
#define EM_EXCESS             111       /* eXcess CPU */
#define EM_DXP                112       /* Deep Execution Processor */
#define EM_ALTERA_NIOS2       113       /* Altera Nios II */
#define EM_CRX                114       /* National CompactRISC CRX */
#define EM_XGATE              115       /* Motorola XGATE */
#define EM_C166               116       /* Infineon C16x/XC16x */
#define EM_M16C               117       /* Renesas M16C */
#define EM_DSPIC30F           118       /* Microchip dsPIC30F */
#define EM_CE                 119       /* Freescale Communication Engine */
#define EM_M32C               120       /* Renesas M32C */
#define EM_TSK3000            131       /* Altium TSK3000 core */
#define EM_RS08               132       /* Freescale RS08 */
#define EM_SHARC              133       /* Analog Devices SHARC */
#define EM_ECOG2              134       /* Cyan Technology eCOG2 */
#define EM_SCORE7             135       /* Sunplus S+core7 */
#define EM_DSP24              136       /* NJR 24-bit DSP */
#define EM_VIDEOCORE3         137       /* Broadcom VideoCore III */
#define EM_LATTICEMICO3       138       /* RISC processor for Lattice FPGA */
#define EM_SE_C17             139       /* Seiko Epson C17 */
#define EM_TI_C6000           140       /* TMS320C6000 DSP family */
#define EM_TI_C2000           141       /* TMS320C2000 DSP family */
#define EM_TI_C5500           142       /* TMS320C55x DSP family */
#define EM_TI_ARP32           143       /* Texas Application Specific RISC */
#define EM_TI_PRU             144       /* Texas Programmable Realtime Unit */
#define EM_MMDSP_PLUS         160       /* STMicroelectronics 64bit VLIW */
#define EM_CYPRESS_M8C        161       /* Cypress M8C microprocessor */
#define EM_R32C               162       /* Renesas R32C series */
#define EM_TRIMEDIA           163       /* NXP Semiconductors TriMedia */
#define EM_QDSP6              164       /* QUALCOMM DSP6 Processor */
#define EM_8051               165       /* Intel 8051 and variants */
#define EM_STXP7X             166       /* STMicroelectronics STxP7x */
#define EM_NDS32              167       /* Andes Technology embedded RISC */
#define EM_ECOG1              168       /* Cyan Technology eCOG1X family */
#define EM_ECOG1X             168       /* Cyan Technology eCOG1X family */
#define EM_MAXQ30             169       /* MAXQ30 Core Micro-controllers */
#define EM_XIMO16             170       /* NJR 16-bit DSP Processor */
#define EM_MANIK              171       /* M2000 Reconfigurable RISC */
#define EM_CRAYNV2            172       /* Cray Inc. NV2 vector architecture */
#define EM_RX                 173       /* Renesas RX family */
#define EM_METAG              174       /* Imagination Technologies META */
#define EM_MCST_ELBRUS        175       /* MCST Elbrus */
#define EM_ECOG16             176       /* Cyan Technology eCOG16 family */
#define EM_CR16               177       /* National CompactRISC CR16 */
#define EM_ETPU               178       /* Freescale Extended Time Unit */
#define EM_SLE9X              179       /* Infineon Technologies SLE9X core */
#define EM_L10M               180       /* Intel L10M */
#define EM_K10M               181       /* Intel K10M */
#define EM_AARCH64            183       /* ARM AARCH64 */
#define EM_AVR32              185       /* Atmel 32-bit */
#define EM_STM8               186       /* STMicroeletronics STM8  */
#define EM_TILE64             187       /* Tilera TILE64 */
#define EM_TILEPRO            188       /* Tilera TILEPro */
#define EM_MICROBLAZE         189       /* Xilinx MicroBlaze 32-bit */
#define EM_CUDA               190       /* NVIDIA CUDA architecture */
#define EM_TILEGX             191       /* Tilera TILE-Gx family */
#define EM_CLOUDSHIELD        192       /* CloudShield architecture family */
#define EM_COREA_1ST          193       /* KIPO-KAIST Core-A 1st gen family */
#define EM_COREA_2ND          194       /* KIPO-KAIST Core-A 2nd gen family */
#define EM_ARC_COMPACT2       195       /* Synopsys ARCompact V2 */
#define EM_OPEN8              196       /* Open8 8-bit RISC soft processor core */
#define EM_RL78               197       /* Renesas RL78 family */
#define EM_VIDEOCORE5         198       /* Broadcom VideoCore V processor */
#define EM_78KOR              199       /* Renesas 78KOR family */
#define EM_56800EX            200       /* Freescale 56800EX (DSC) */
#define EM_BA1                201       /* Beyond BA1 CPU architecture */
#define EM_BA2                202       /* Beyond BA2 CPU architecture */
#define EM_XCORE              203       /* XMOS xCORE processor family */
#define EM_MCHP_PIC           204       /* Microchip 8-bit PIC(r) family */
#define EM_KM32               210       /* KM211 KM32 32-bit processor */
#define EM_KMX32              211       /* KM211 KMX32 32-bit processor */
#define EM_KMX16              212       /* KM211 KMX16 16-bit processor */
#define EM_KMX8               213       /* KM211 KMX8 8-bit processor */
#define EM_KVARC              214       /* KM211 KVARC processor */
#define EM_CDP                215       /* Paneve CDP architecture family */
#define EM_COGE               216       /* Cognitive Smart Memory Processor */
#define EM_COOL               217       /* Bluechip Systems CoolEngine */
#define EM_NORC               218       /* Nanoradio Optimized RISC */
#define EM_CSR_KALIMBA        219       /* CSR Kalimba architecture family */
#define EM_Z80                220       /* Zilog Z80 */
#define EM_VISIUM             221       /* VISIUMcore processor */
#define EM_FT32               222       /* FTDI Chip FT32 */
#define EM_MOXIE              223       /* Moxie processor family */
#define EM_AMDGPU             224       /* AMD GPU architecture */
#define EM_RISCV              243       /* RISC-V */
#define EM_BPF                247       /* Linux BPF */
#define EM_CSKY               252       /* C-SKY */

/* e_type */
#define ET_NONE	              0	        /* No file type */
#define ET_REL	              1	        /* Relocatable file */
#define ET_EXEC	              2	        /* Executable file */
#define ET_DYN	              3	        /* Shared object file */
#define ET_CORE	              4		/* Core file */
#define ET_LOOS	              0xfe00	/* Operating system-specific */
#define ET_HIOS	              0xfeff	/* Operating system-specific */
#define ET_LOPROC	      0xff00	/* Processor-specific */
#define ET_HIPROC	      0xffff	/* Processor-specific */

/* Version */
#define EV_NONE		      0		/* Invalid */
#define EV_CURRENT	      1		/* Current */

/* Magic for e_phnum: get real value from sh_info of first section header */
#define PN_XNUM		0xffff

#define ELFH32SZ              54
#define ELFH64SZ              64

typedef struct elfhdr32 Elf32_Ehdr;
typedef struct elfhdr64 Elf64_Ehdr;

struct elfhdr32 {
	unsigned char e_ident[EI_NIDENT];/* ELF Identification */
	Elf32_Half e_type;              /* object file type */
	Elf32_Half e_machine;           /* machine */
	Elf32_Word e_version;	        /* object file version */
	Elf32_Addr e_entry;             /* virtual entry point */
	Elf32_Off e_phoff;              /* program header table offset */
	Elf32_Off e_shoff;              /* section header table offset */
	Elf32_Word e_flags;             /* processor-specific flags */
	Elf32_Half e_ehsize;            /* ELF header size */
	Elf32_Half e_phentsize;         /* program header entry size */
	Elf32_Half e_phnum;             /* number of program header entries */
	Elf32_Half e_shentsize;         /* section header entry size */
	Elf32_Half e_shnum;             /* number of section header entries */
	Elf32_Half e_shstrndx;          /* section header table's "section
					   header string table" entry offset */
};

struct elfhdr64 {
	unsigned char e_ident[EI_NIDENT];/* Id bytes */
	Elf64_Half e_type;              /* file type */
	Elf64_Half e_machine;           /* machine type */
	Elf64_Word e_version;           /* version number */
	Elf64_Addr e_entry;             /* entry point */
	Elf64_Off e_phoff;              /* Program hdr offset */
	Elf64_Off e_shoff;              /* Section hdr offset */
	Elf64_Word e_flags;             /* Processor flags */
	Elf64_Half e_ehsize;            /* sizeof ehdr */
	Elf64_Half e_phentsize;         /* Program header entry size */
	Elf64_Half e_phnum;             /* Number of program headers */
	Elf64_Half e_shentsize;         /* Section header entry size */
	Elf64_Half e_shnum;             /* Number of section headers */
	Elf64_Half e_shstrndx;          /* String table index */
};
