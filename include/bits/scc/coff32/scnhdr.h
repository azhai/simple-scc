/* This file is inspired in the book "Understanding and using COFF" */

#define SCNNMLEN 8

struct scnhdr {
	char s_name[SCNNMLEN];    /* section name */
	long s_paddr;             /* physical address */
	long s_vaddr;             /* virtual address */
	long s_size;              /* section size */
	long s_scnptr;            /* file ptr to raw data */
	long s_relptr;            /* file ptr to relo info */
	long s_lnnoptr;           /* file ptr to line number */
	unsigned short s_nrelloc; /* number of relocation entries */
	unsigned short s_nlnno;   /* number of lines entries */
	long s_flags;             /* type and content flags */
};

#define SCNHDR struct scnhdr
#define SCNHSZ 40


#define STYP_REG        0x00
#define STYP_DSECT      0x01
#define STYP_NOLOAD     0x02
#define STYP_GROUP      0x04
#define STYP_PAD        0x08
#define STYP_COPY       0x10
#define STYP_TEXT       0x20
#define STYP_DATA       0x40
#define STYP_BSS        0x80
#define STYP_RDATA      0x100
#define STYP_INFO       0x200
#define STYP_OVER       0x400
#define STYP_LIB        0x800
#define STYP_MERGE      0x2000
#define TYP_REVERSE_PAD 0x4000
#define STYP_LIT        0x8020
