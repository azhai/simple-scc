static char sccsid[] = "@(#) ./ar/main.c";

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <stat.h>

#include "../inc/ar.h"
#include "../inc/arg.h"
#include "../inc/scc.h"

#define NOSAVE 0
#define SAVE   1

char *argv0;

static int bflag, vflag, cflag, lflag, uflag, aflag, haslist;
static char *posname, *tmpafile1, *tmpafile2, *arfile;

struct arop {
	FILE *src;
	FILE *dst;
	FILE *tmp;
	struct ar_hdr hdr;
	char *fname;
	long size;
	long mode;
	long long date;
};

static void
cleanup(void)
{
	if (tmpafile1)
		remove(tmpafile1);
	if (tmpafile2)
		remove(tmpafile2);
}

static void
sigfun(int signum)
{
	exit(1);
}

static FILE *
openar(void)
{
	FILE *fp;
	char magic[SARMAG+1];

	if ((fp = fopen(arfile,"r+b")) == NULL) {
		if (!cflag)
			fprintf(stderr, "ar: creating %s\n", arfile);
		if ((fp = fopen(arfile, "w+b")) == NULL) {
			perror("ar:opening archive");
			exit(1);
		}
		fputs(ARMAG, fp);
		if (fflush(fp) == EOF) {
			perror("ar:writing magic number");
			exit(1);
		}
	} else {
		if (fgets(magic, sizeof(magic), fp) == NULL) {
			perror("ar:error reading magic number");
			exit(1);
		}
		if (strcmp(magic, ARMAG)) {
			fprintf(stderr,
			        "ar:%s:invalid magic number '%s'\n",
			        arfile,
			        magic);
			exit(1);
		}
	}
	return fp;
}

static void
archive(char *fname, FILE *to, char letter)
{
	int c;
	size_t n;
	FILE *from;
	char mtime[13];
	struct stat st;

	if (vflag)
		printf("%c - %s\n", letter, fname);
	if (strlen(fname) > 16)
		fprintf(stderr, "ar:%s: too long name\n", fname);
	if ((from = fopen(fname, "rb")) == NULL) {
		fprintf(stderr,
		        "ar:opening member '%s':%s\n",
		        fname,
		        strerror(errno));
		exit(1);
	}
	if (stat(fname, &st) < 0) {
		fprintf(stderr, "ar:error getting '%s' attributes\n", fname);
		exit(1);
	}
	strftime(mtime, sizeof(mtime), "%s", gmtime(&st.st_mtime));
	fprintf(to,
	        "%-16.16s%-12s%-6u%-6u%-8o%-10llu`\n",
	        fname,
	        mtime,
	        st.st_uid,
	        st.st_gid,
	        st.st_mode,
	        (unsigned long long) st.st_size);
	for (n = 0; (c = getc(from)) != EOF; n++)
		putc(c, to);
	if (n & 1)
		putc('\n', to);
	if (ferror(from)) {
		fprintf(stderr,
		        "ar:reading input '%s':%s\n",
		        fname, strerror(errno));
		exit(1);
	}
	fclose(from);
}

static void
append(FILE *fp, char *list[])
{
	char *fname;

	if (fseek(fp, 0, SEEK_END) == EOF) {
		perror("ar:seeking archive");
		exit(1);
	}

	for ( ; fname = *list; ++list) {
		*list = NULL;
		archive(fname, fp, 'a');
	}

	if (fclose(fp) == EOF) {
		perror("ar:error writing archive");
		exit(1);
	}
}

static void
cat(FILE *src1, FILE *src2, FILE *dst)
{
	int c;

	while ((c = getc(src1)) != EOF)
		fputc(c, dst);
	while ((c = getc(src2)) != EOF)
		fputc(c, dst);
	if (ferror(src1) || ferror(src2) || fclose(dst) == EOF) {
		perror("ar:moving files in archive");
		exit(1);
	}
}

static void
copy(struct ar_hdr *hdr, long siz, FILE *src, FILE *dst)
{
	int c;

	fwrite(hdr, sizeof(*hdr), 1, dst);
	if ((siz & 1) == 1)
		siz++;
	while (siz--) {
		if ((c = getc(src)) == EOF)
			break;
		fputc(c, dst);
	}
}

static void
letters(unsigned long val, char *s)
{
	*s++ = (val & 04) ? 'r' : '-';
	*s++ = (val & 02) ? 'w' : '-';
	*s++ = (val & 01) ? 'x' : '-';
}

static char *
perms(struct arop *op)
{
	static char buf[10];

	letters(op->mode >> 6, buf);
	letters(op->mode >> 3, buf+3);
	letters(op->mode, buf +6);
	buf[9] = '\0';

	return buf;
}

static int
inlist(char *fname, char *list[])
{
	while (*list && strcmp(*list, fname))
		++list;
	if (*list == NULL)
		return 0;
	for (; *list; ++list)
		list[0] = list[1];
	return 1;
}

static void
split(struct arop *op, char *files[])
{
	if (!inlist(op->fname, files)) {
		copy(&op->hdr, op->size, op->src, op->dst);
		return;
	} else {
		if (vflag)
			printf("m - %s\n", op->fname);
		copy(&op->hdr, op->size, op->src, op->tmp);
	}
}

static void
merge(struct arop *op, char *list[])
{
	int c;

	if (strcmp(op->fname, posname)) {
		copy(&op->hdr, op->size, op->src, op->dst);
		return;
	}

	if (aflag)
		copy(&op->hdr, op->size, op->src, op->dst);

	while ((c = getc(op->tmp)) != EOF)
		putc(c, op->dst);

	if (bflag)
		copy(&op->hdr, op->size, op->src, op->dst);
}

static void
insert(struct arop *op, char *list[])
{
	if (!posname || strcmp(op->fname, posname)) {
		copy(&op->hdr, op->size, op->src, op->dst);
		return;
	}

	if (aflag)
		copy(&op->hdr, op->size, op->src, op->dst);

	for ( ; *list; ++list)
		archive(*list, op->dst, 'a');

	if (bflag)
		copy(&op->hdr, op->size, op->src, op->dst);
}

static void
update(struct arop *op, char *files[])
{
	char **l;

	if (!inlist(op->fname, files))
		copy(&op->hdr, op->size, op->src, op->dst);
	else
		archive(op->fname, op->dst, 'r');
}

static void
extract(struct arop *op, char *files[])
{
	int c;
	long siz;
	FILE *fp;

	if (haslist && !inlist(op->fname, files))
		return;
	if (vflag)
		printf("x - %s\n", op->fname);
	siz = op->size;

	if ((fp = fopen(op->fname, "wb")) == NULL)
		goto error_file;
	while (siz-- > 0 && (c = getc(op->src)) != EOF)
		putc(c, fp);
	fflush(fp);
	if (ferror(op->src) || ferror(fp))
		goto error_file;
	fclose(fp);

	/* TODO: set attributes */
	return;


error_file:
	perror("ar:error extracting file");
	exit(1);
}

static void
print(struct arop *op, char *files[])
{
	long siz;
	int c;

	if (haslist && !inlist(op->fname, files))
		return;
	if (vflag)
		printf("\n<%s>\n\n", op->fname);
	siz = op->size;
	while (siz-- > 0 && (c = getc(op->src)) != EOF)
		putchar(c);
}

static void
list(struct arop *op, char *files[])
{
	time_t t;
	struct ar_hdr *hdr = &op->hdr;
	char mtime[30];

	if (haslist && !inlist(op->fname, files))
		return;
	if (!vflag) {
		printf("%s\n", op->fname);
	} else {
		t = totime(op->date);
		strftime(mtime, sizeof(mtime), "%c", localtime(&t));
		printf("%s %ld/%ld\t%s %s\n",
		       perms(op),
		       atol(hdr->ar_uid),
		       atol(hdr->ar_gid),
		       mtime,
		       op->fname);
	}
}

static void
del(struct arop *op, char *files[])
{
	if (inlist(op->fname, files)) {
		if (vflag)
			printf("d - %s\n", op->fname);
		return;
	}
	copy(&op->hdr, op->size, op->src, op->dst);
}

static char *
getfname(struct ar_hdr *hdr)
{
	static char fname[FILENAME_MAX];
	size_t i;
	char *bp = fname;

	for (i = 0; i < sizeof(hdr->ar_name); i++) {
		if ((*bp = hdr->ar_name[i]) == ' ')
			break;
		++bp;
	}
	*bp = '\0';
	return fname;
}

static long long
getnum(char *s, int size, int base)
{
	int c;
	long long val;
	char *p;
	static char digits[] = "0123456789";

	for (val = 0; size > 0; val += c) {
		--size;
		if ((c = *s++) == ' ')
			break;
		if ((p = strchr(digits, c)) == NULL)
			return -1;
		if ((c = p - digits) >= base)
			return -1;
		val *= base;
	}

	while (size > 0 && *s++ == ' ')
		--size;
	return (size == 0) ? val : -1;
}

static int
valid(struct arop *op)
{
	struct ar_hdr *hdr = &op->hdr;

	op->fname = getfname(&op->hdr);
	op->size = getnum(hdr->ar_size, sizeof(hdr->ar_size), 10);
	op->mode = getnum(hdr->ar_mode, sizeof(hdr->ar_mode), 8);
	op->date = getnum(hdr->ar_date, sizeof(hdr->ar_date), 10);

	if (strncmp(hdr->ar_fmag, ARFMAG, sizeof(hdr->ar_fmag)) ||
	    op->size < 0 || op->mode < 0 || op->date < 0) {
		return 0;
	}
	return 1;
}

static void
run(FILE *fp, FILE *tmp1, FILE *tmp2,
    char *files[], void (*fun)(struct arop *, char *files[]))
{
	struct arop op;

	op.src = fp;
	op.dst = tmp1;
	op.tmp = tmp2;
	while (!ferror(fp) && fread(&op.hdr, sizeof(op.hdr), 1, fp) == 1) {
		fpos_t pos;

		if (!valid(&op)) {
			fprintf(stderr,
			        "ar:corrupted member '%s'\n",
			        op.fname);
			exit(1);
		}
		/* TODO: Implement early break */
		fgetpos(fp, &pos);
		(*fun)(&op, files);
		fsetpos(fp, &pos);
		fseek(fp, op.size+1 & ~1, SEEK_CUR);
	}
	if (ferror(fp)) {
		perror("ar:reading members");
		exit(1);
	}
	fclose(fp);
	if (tmp1 && fflush(tmp1) == EOF) {
		perror("ar:writing in temporary file");
		exit(1);
	}
	if (tmp2 && fflush(tmp2) == EOF) {
		perror("ar:writing in temporary file");
		exit(1);
	}
}

static void
closetmp(FILE *tmp, char **name, int save)
{
	int c;
	FILE *fp;

	if (lflag) {
		if (save && rename(*name, arfile) < 0) {
			perror("ar:renaming temporary");
			exit(1);
		}
		*name = NULL;
	} else if (save) {
		if ((fp = fopen(arfile, "wb")) == NULL) {
			perror("ar:reopening archive file");
			exit(1);
		}
		rewind(tmp);
		while ((c = getc(tmp)) != EOF)
			fputc(c, fp);
		fflush(fp);
		if (ferror(fp) || ferror(tmp)) {
			perror("ar:copying from temporary");
			exit(1);
		}
		fclose(fp);
	}
	fclose(tmp);
}

static FILE *
opentmp(char *fname, char **dst)
{
	FILE *tmp;

	if (lflag) {
		*dst = fname;
		tmp = fopen(fname, "w+b");
	} else {
		tmp = tmpfile();
	}
	if (tmp == NULL) {
		perror("ar:creating temporary");
		exit(1);
	}
	fputs(ARMAG, tmp);

	return tmp;
}

static void
usage(void)
{
	fputs("ar [-drqtpmx][posname] [-vuaibcl] [posname] arfile name ...\n",
	      stderr);
	exit(1);
}

static void
doit(int key, FILE *fp, char *flist[])
{
	FILE *tmp1, *tmp2;

	if (*flist == NULL &&
	    (key == 'r' || key == 'd' || key == 'm' || key == 'q')) {
		if (fclose(fp) == EOF) {
			perror("ar:early close of archive file");
			exit(-1);
		}
		return;
	}
	haslist = *flist != NULL;

	switch (key) {
	case 'r':
		tmp1 = opentmp("ar.tmp1", &tmpafile1);
		run(fp, tmp1, NULL, flist, update);

		if (!posname) {
			append(tmp1, flist);
			break;
		}

		fseek(tmp1, SARMAG, SEEK_SET);
		tmp2 = opentmp("ar.tmp2", &tmpafile2);
		run(tmp1, tmp2, NULL, flist, insert);
		closetmp(tmp1, &tmpafile1, NOSAVE);
		closetmp(tmp2, &tmpafile2, SAVE);
		break;
	case 'q':
		append(fp, flist);
		break;
	case 'd':
		tmp1 = opentmp("ar.tmp", &tmpafile1);
		run(fp, tmp1, NULL, flist, del);
		closetmp(tmp1, &tmpafile1, SAVE);
		break;
	case 't':
		run(fp, NULL, NULL, flist, list);
		break;
	case 'p':
		run(fp, NULL, NULL, flist, print);
		break;
	case 'x':
		run(fp, NULL, NULL, flist, extract);
		break;
	case 'm':
		tmp1 = opentmp("ar.tmp1", &tmpafile1);
		tmp2 = opentmp("ar.tmp2", &tmpafile2);
		run(fp, tmp1, tmp2, flist, split);

		fp = openar();
		fseek(tmp1, SARMAG, SEEK_SET);
		fseek(tmp2, SARMAG, SEEK_SET);
		if (!posname) {
			cat(tmp1, tmp2, fp);
			break;
		}
		run(tmp1, fp, tmp2, NULL, merge);
		closetmp(tmp1, &tmpafile1, NOSAVE);
		closetmp(tmp2, &tmpafile2, NOSAVE);
		break;
	}
	if (*flist == NULL)
		return;

	while (*flist)
		fprintf(stderr, "ar: No member named '%s\n", *flist++);
	exit(1);
}

int
main(int argc, char *argv[])
{
	int key, nkey = 0, pos = 0;

	atexit(cleanup);
	ARGBEGIN {
	case 'd':
		nkey++;
		key = 'd';
		break;
	case 'r':
		nkey++;
		key = 'r';
		break;
	case 'q':
		nkey++;
		key = 'q';
		break;
	case 't':
		nkey++;
		key = 't';
		break;
	case 'p':
		nkey++;
		key = 'p';
		break;
	case 'm':
		nkey++;
		key = 'm';
		break;
	case 'x':
		nkey++;
		key = 'x';
		break;
	case 'a':
		aflag = 1;
		pos++;
		posname = EARGF(usage());
		break;
	case 'i':
	case 'b':
		bflag = 1;
		pos++;
		posname = EARGF(usage());
		break;
	case 'v':
		vflag = 1;
		break;
	case 'c':
		cflag = 1;
		break;
	case 'l':
		lflag = 1;
		break;
	case 'u':
		uflag = 1;
		break;
	default:
		usage();
	} ARGEND

	if (nkey == 0 || nkey > 1 || pos > 1 || argc == 0)
		usage();

	signal(SIGINT, sigfun);
	signal(SIGQUIT, sigfun);
	signal(SIGTERM, sigfun);

	arfile = *argv;
	doit(key, openar(), argv+1);

	if (fflush(stdout) == EOF) {
		perror("ar:error writing to stdout");
		exit(1);
	}

	return 0;
}
