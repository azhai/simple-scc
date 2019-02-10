static char sccsid[] = "@(#) ./nm/main.c";

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/arg.h>
#include <scc/mach.h>


struct symtbl {
	Objsym **buf;
	size_t nsyms;
};

char *argv0;
static int status, multi;
static int radix = 16;
static int Pflag;
static int Aflag;
static int vflag;
static int gflag;
static int uflag;
static char *filename, *membname;

static void
error(char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	fprintf(stderr, "nm: %s: ", filename);
	if (membname)
		fprintf(stderr, "%s: ", membname);
	vfprintf(stderr, fmt, va);
	putc('\n', stderr);
	va_end(va);

	status = EXIT_FAILURE;
}

static int
cmp(const void *p1, const void *p2)
{
	Objsym **s1 = (Objsym **) p1, **s2 = (Objsym **) p2;
	Objsym *sym1 = *s1, *sym2 = *s2;

	if (vflag) {
		if (sym1->value > sym2->value)
			return 1;
		if (sym1->value < sym2->value)
			return -1;
		if (sym1->type == 'U' && sym2->type == 'U')
			return 0;
		if (sym1->type == 'U')
			return -1;
		if (sym2->type == 'U')
			return 1;
		return 0;
	} else {
		return strcmp(sym1->name, sym2->name);
	}
}

static void
printsyms(Objsym **syms, size_t nsym)
{
	size_t i;

	qsort(syms, nsym, sizeof(syms), cmp);

	if (multi)
		printf("%s:\n", (membname) ? membname : filename);

	for (i = 0; i < nsym; i++) {
		Objsym *sym = syms[i];
		int type = sym->type;
		char *fmt;

		if (Aflag) {
			fmt = (membname) ? "%s[%s]: " : "%s: ";
			printf(fmt, filename, membname);
		}

		if (Pflag) {
			printf("%s %c", sym->name, sym->type);
			if (type != 'U') {
				if (radix == 8)
					fmt = " %016.16llo %lo";
				else if (radix == 10)
					fmt = " %016.16llu %lu";
				else
					fmt = " %016.16llx %lx";
				printf(fmt, sym->value, sym->size);
			}
		} else {
			if (type == 'U')
				fmt = "                ";
			else if (radix == 8)
				fmt = "%016.16llo";
			else if (radix == 10)
				fmt = "%016.16lld";
			else
				fmt = "%016.16llx";
			printf(fmt, sym->value);
			printf(" %c %s", sym->type, sym->name);
		}
		putchar('\n');
	}
}

static int
newsym(Objsym *sym, void *data)
{
	struct symtbl *tbl = data;
	Objsym **p;
	size_t n, size;
	int type = sym->type;

	if (type == '?' || type == 'N')
		return 1;

	if (uflag && type != 'U')
		return 1;

	if (gflag && !isupper(type))
		return 1;

	n = tbl->nsyms+1;
	if (n == 0 || n > SIZE_MAX / sizeof(*p))
		return 0;
	size = n *sizeof(*p);

	if ((p = realloc(tbl->buf, size)) == NULL)
		return 0;
	tbl->buf = p;
	p[tbl->nsyms++] = sym;

	return 1;
}

static void
newobject(FILE *fp, int type)
{
	int err = 1;
	Obj *obj;
	struct symtbl tbl = {NULL, 0};

	if ((obj = objnew(type)) == NULL) {
		error("out of memory");
		return;
	}

	if (objread(obj, fp) < 0)
		goto error;

	if (!forsym(obj, newsym, &tbl))
		goto error;

	printsyms(tbl.buf, tbl.nsyms);
	err = 0;

error:
	free(tbl.buf);
	objdel(obj);
	if (err)
		error("object file corrupted");
}

static int
newmember(FILE *fp, char *name, void *data)
{
	int t;

	multi = 1;
	membname = name;
	if ((t = objtype(fp, NULL)) != -1)
		newobject(fp, t);

	return 1;
}

static void
nm(char *fname)
{
	int t;
	FILE *fp;

	filename = fname;
	membname = NULL;

	if ((fp = fopen(fname, "rb")) == NULL) {
		error(strerror(errno));
		return;
	}

	if ((t = objtype(fp, NULL)) != -1)
		newobject(fp, t);
	else if (archive(fp))
		artraverse(fp, newmember, NULL);
	else
		error("bad format");

	if (ferror(fp))
		error(strerror(errno));

	fclose(fp);
}

static void
usage(void)
{
	fputs("nm [-APv][ -g| -u][-t format] [file...]\n", stderr);
	exit(1);
}

int
main(int argc, char *argv[])
{
	char *t;

	ARGBEGIN {
	case 'P':
		Pflag = 1;
		break;
	case 'A':
		Aflag = 1;
		break;
	case 'g':
		gflag = 1;
		break;
	case 'u':
		uflag = 1;
		break;
	case 'v':
		vflag = 1;
		break;
	case 't':
		t = EARGF(usage());
		if (!strcmp(t, "o"))
			radix = 8;
		else if (!strcmp(t, "d"))
			radix = 10;
		else if (!strcmp(t, "x"))
			radix = 16;
		else
			usage();
		break;
	default:
		usage();
	} ARGEND

	if (argc == 0) {
		nm("a.out");
	} else {
		if (argc > 1)
			multi = 1;
		for ( ; *argv; ++argv)
			nm(*argv);
	}

	if (fflush(stdout)) {
		fprintf(stderr, "nm: error writing in output");
		status = 1;
	}

	return status;
}
