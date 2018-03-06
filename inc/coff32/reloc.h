
/* This file is inspired in the book "Understanding and using COFF" */

struct reloc {
	long r_vaddr;           /* address of reference */
	long r_symndx;          /* index into symbol table */
	unsignedd short r_type; /* relocation type */
};
