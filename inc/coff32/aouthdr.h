
/* This file is inspired in the book "Understanding and using COFF" */

struct aouthdr {
	short magic;        /* magic number */
	short vstamp;       /* version stamp */
	long tsize;         /* text size in bytes */
	long dsize;         /* initialized data size */
	long bsize;         /* uinitialized data size */
	long entry;         /* entry point */
	long text_start;    /* base of text segment */
	long data_start;    /* base of data segment */
};

typedef struct aouthdr AOUTHDR;
