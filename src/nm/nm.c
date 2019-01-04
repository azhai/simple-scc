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
	const Symbol *s1 = p1, *s2 = p2;

	if (vflag) {
		if (s1->value > s2->value)
			return 1;
		if (s1->value < s2->value)
			return -1;
		if (s1->type == 'U' && s2->type == 'U')
			return 0;
		if (s1->type == 'U')
			return -1;
		if (s2->type == 'U')
			return 1;
		return 0;
	} else {
		return strcmp(s1->name, s2->name);
	}
}

static void
printsyms(Obj *obj)
{
	unsigned long nsym;
	Symbol *sym;

	if (!obj->symtbl)
		return;
	sym = obj->symtbl;
	nsym = obj->nsym;

	qsort(sym, nsym, sizeof(*sym), cmp);

	if (multi)
		printf("%s:\n", (membname) ? membname : filename);

	for (sym = obj->symtbl; nsym--; sym++) {
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
filter(Symbol *sym)
{
	int type = sym->type;

	if (type == '?' || type == 'N')
		return 1;

	if (uflag && type != 'U')
		return 1;

	if (gflag && !isupper(type))
		return 1;

	return 0;
}

static void
newobject(FILE *fp, int type)
{
	Obj obj;

	if (objopen(fp, type, &obj) < 0)
		goto err1;

	if (objread(fp, &obj, filter) < 0)
		goto err2;

	printsyms(&obj);
	objclose(&obj);
	return;

err2:
	objclose(&obj);
err1:
	error("object file corrupted");
}

static int
newmember(FILE *fp, char *name, void *data)
{
	int t;

	multi = 1;
	membname = name;
	if ((t = objtest(fp, NULL)) != -1)
		newobject(fp, t);

	return 0;
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

	if ((t = objtest(fp, NULL)) != -1)
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

	fflush(stdout);
	if (ferror(stdout)) {
		fprintf(stderr, "nm: error writing in output");
		status = 1;
	}

	return status;
}
