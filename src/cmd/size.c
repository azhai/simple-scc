#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <scc/arg.h>
#include <scc/mach.h>

static int status;
static char *filename;
static int tflag;
static unsigned long long ttext, tdata, tbss, ttotal;
char *argv0;

static void
error(char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	fprintf(stderr, "size: %s: ", filename);
	vfprintf(stderr, fmt, va);
	putc('\n', stderr);
	va_end(va);

	status = EXIT_FAILURE;
}

static void
size(char *fname)
{
	int type;
	Obj *obj;
	FILE *fp;
	unsigned long long text, data, bss, total;

	filename = fname;
	if ((fp = fopen(fname, "rb")) == NULL) {
		error(strerror(errno));
		return;
	}
	if ((type = objtype(fp, NULL)) < 0) {
		error(strerror(errno));
		goto err1;
	}
	if ((obj = objnew(type)) == NULL) {
		error("out of memory");
		goto err1;
	}
	if (objread(obj, fp) < 0) {
		error("file corrupted");
		goto err2;
	}
	objsize(obj, &text, &data, &bss);
	total = text + data + bss;
	printf("%llu\t%llu\t%llu\t%llu\t%llx\t%s\n",
	       text, data, bss, total, total, fname);

	ttext += text;
	tdata += data;
	tbss += bss;
	ttotal += total;

err2:
	objdel(obj);
err1:
	fclose(fp);
	return;
}

static void
usage(void)
{
	fputs("usage: size [-t] file ...\n", stderr);
	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
	unsigned long long total;

	ARGBEGIN {
	case 't':
		tflag = 1;
		break;
	default:
		usage();
	} ARGEND

	if (argc == 1)
		usage;

	puts("text\tdata\tbss\tdec\thex\tfilename");
	for (argc--; argc > 0; argc--)
		size(*argv++);

	if (tflag) {
		total = ttext + tdata + tbss;
		printf("%llu\t%llu\t%llu\t%llu\t%llx\t%s\n",
		       ttext, tdata, tbss, total, total, "(TOTALS)");
	}

	if (fflush(stdout)) {
		filename = "stdout";
		error(strerror(errno));
	}

	return status;
}
