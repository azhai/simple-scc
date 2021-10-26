#include <scc/elf/elftypes.h>
#include <scc/elf/elfhdr.h>
#include <scc/elf/elfphdr.h>
#include <scc/elf/elfshdr.h>
#include <scc/elf/elfent.h>

#define ELF_ST_BIND ELF64_ST_BIND
#define ELF_ST_TYPE ELF64_ST_TYPE
#define Elf_Ehdr Elf64_Ehdr
#define Elf_Phdr Elf64_Phdr
#define Elf_Shdr Elf64_Shdr
#define Elf_Sym Elf64_Sym
#define ELFHSZ ELFH64SZ
#define ELFPSZ ELFP64SZ
#define ELFSSZ ELFS64SZ
#define ELFESZ ELFE64SZ


#define SEC_STRTBL 0
#define SYM_STRTBL 1

typedef struct elf64 Elf64;

struct elf64 {
	Elf_Ehdr hdr;
	Elf_Phdr *phdr;
	Elf_Shdr *shdr;
	Elf_Shdr *symtab;
	Elf_Sym *syms;

	char *strtbl[2];
	size_t strsiz[2];

	size_t nsec;
	size_t nsym;
};

extern int elf64new(Obj *);
extern int elf64read(Obj *, FILE *);
extern int elf64setidx(long, char **, long *, FILE *);
extern int elf64getidx(long *, char ***, long **, FILE *);
extern int elf64pc2line(Obj *, unsigned long long , char *, int *);
extern int elf64strip(Obj *);
extern void elf64del(Obj *);
extern int elf64write(Obj *, Map *, FILE *);
extern int elf64probe(unsigned char *, char **);

extern int elf64xsetidx(int long , char *[], long [], FILE *);
extern int elf64xgetidx(int, long *, char ***, long **, FILE *);

extern Symbol *elf64getsym(Obj *, int *, Symbol *);
extern Section *elf64getsec(Obj *, int *, Section *);
extern Map *elf64loadmap(Obj *, FILE *);
