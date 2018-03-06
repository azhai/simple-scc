
/* This file is inspired in the book "Understanding and using COFF" */

struct filehdr {
	unsigned short f_magic;  /* magic number */
	unsigned short f_nscns;  /* number of sections */
	long f_timdat;           /* time stamp */
	long f_symptr;           /* file pointer to symbols */
	long f_nsyms;            /* number of symbols */
	unsigned short f_opthdr; /* size of optional header */
	unsigned short f_flags;  /* file flags */
};

#define FILHDR struct filehdr
#define FILHSZ 20

#define F_RELFLG (1 << 0)
#define F_EXEC   (1 << 1)
#define F_LMNO   (1 << 2)
#define F_SYMS   (1 << 3)

#define COFF_Z80MAGIC     0x805a
