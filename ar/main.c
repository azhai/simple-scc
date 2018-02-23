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

enum {
	BEFORE,
	INDOT,
	AFTER,
};

struct tmp {
	char *name;
	FILE *fp;
} tmps[3];

char *argv0;

static int bflag, vflag, cflag, lflag, uflag, aflag;
static char *arfile, *posname;

struct member {
	FILE *src;
	struct ar_hdr hdr;
	int cur;
	char *fname;
	long size;
	long mode;
	long long date;
};

static void
cleanup(void)
{
	int i;

	for (i = 0; i < 3; i++) {
		if (tmps[i].name)
			remove(tmps[i].name);
	}
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
append(FILE *fp, char *argv[])
{
	char *fname;

	if (fseek(fp, 0, SEEK_END) == EOF) {
		perror("ar:seeking archive");
		exit(1);
	}

	for ( ; fname = *argv; ++argv) {
		*argv = NULL;
		archive(fname, fp, 'a');
	}

	if (fclose(fp) == EOF) {
		perror("ar:error writing archive");
		exit(1);
	}
}

static void
copy(struct member *m, struct tmp *tmp)
{
	int c;
	size_t siz = m->size;
	struct ar_hdr *hdr = &m->hdr;

	fwrite(hdr, sizeof(*hdr), 1, tmp->fp);
	if ((siz & 1) == 1)
		siz++;
	while (siz--) {
		if ((c = getc(m->src)) == EOF)
			break;
		fputc(c, tmp->fp);
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
perms(struct member *m)
{
	static char buf[10];

	letters(m->mode >> 6, buf);
	letters(m->mode >> 3, buf+3);
	letters(m->mode, buf +6);
	buf[9] = '\0';

	return buf;
}

static int
inlist(char *fname, int argc, char *argv[])
{
	for (; argc-- > 0; ++argv) {
		if (*argv && !strcmp(*argv, fname)) {
			*argv = NULL;
			return 1;
		}
	}
	return 0;
}

static void
move(struct member *m, int argc, char *argv[])
{
	int where;

	if (inlist(m->fname, argc, argv)) {
		if (vflag)
			printf("m - %s\n", m->fname);
		where = INDOT;
	} else if (posname && !strcmp(posname, m->fname)) {
		where = (bflag) ? AFTER : BEFORE;
		m->cur = AFTER;
	} else {
		where = m->cur;
	}
	copy(m, &tmps[where]);
}

static void
insert(int argc, char *argv[])
{
	for (; argc-- > 0; ++argv) {
		if (*argv) {
			archive(*argv, tmps[INDOT].fp, 'a');
			*argv = NULL;
		}
	}
}

static void
update(struct member *m, int argc, char *argv[])
{
	int where;
	FILE *fp = tmps[BEFORE].fp;

	if (inlist(m->fname, argc, argv)) {
		archive(m->fname, tmps[m->cur].fp, 'r');
		return;
	} else if (posname && !strcmp(posname, m->fname)) {
		where = (bflag) ? AFTER : BEFORE;
		m->cur = AFTER;
	} else {
		where = m->cur;
	}
	copy(m, &tmps[where]);
}

static void
extract(struct member *m, int argc, char *argv[])
{
	int c;
	long siz;
	FILE *fp;

	if (argc > 0 && !inlist(m->fname, argc, argv))
		return;
	if (vflag)
		printf("x - %s\n", m->fname);
	siz = m->size;

	if ((fp = fopen(m->fname, "wb")) == NULL)
		goto error_file;
	while (siz-- > 0 && (c = getc(m->src)) != EOF)
		putc(c, fp);
	fflush(fp);
	if (fclose(fp) == EOF)
		goto error_file;

	/* TODO: set attributes */
	return;


error_file:
	perror("ar:error extracting file");
	exit(1);
}

static void
print(struct member *m, int argc, char *argv[])
{
	long siz;
	int c;

	if (argc > 0 && !inlist(m->fname, argc, argv))
		return;
	if (vflag)
		printf("\n<%s>\n\n", m->fname);
	siz = m->size;
	while (siz-- > 0 && (c = getc(m->src)) != EOF)
		putchar(c);
}

static void
list(struct member *m, int argc, char *argv[])
{
	time_t t;
	struct ar_hdr *hdr = &m->hdr;
	char mtime[30];

	if (argc > 0  && !inlist(m->fname, argc, argv))
		return;
	if (!vflag) {
		printf("%s\n", m->fname);
	} else {
		t = totime(m->date);
		strftime(mtime, sizeof(mtime), "%c", localtime(&t));
		printf("%s %ld/%ld\t%s %s\n",
		       perms(m),
		       atol(hdr->ar_uid),
		       atol(hdr->ar_gid),
		       mtime,
		       m->fname);
	}
}

static void
del(struct member *m, int argc, char *argv[])
{
	if (inlist(m->fname, argc, argv)) {
		if (vflag)
			printf("d - %s\n", m->fname);
		return;
	}
	copy(m, &tmps[BEFORE]);
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
valid(struct member *m)
{
	struct ar_hdr *hdr = &m->hdr;

	m->fname = getfname(&m->hdr);
	m->size = getnum(hdr->ar_size, sizeof(hdr->ar_size), 10);
	m->mode = getnum(hdr->ar_mode, sizeof(hdr->ar_mode), 8);
	m->date = getnum(hdr->ar_date, sizeof(hdr->ar_date), 10);

	if (strncmp(hdr->ar_fmag, ARFMAG, sizeof(hdr->ar_fmag)) ||
	    m->size < 0 || m->mode < 0 || m->date < 0) {
		return 0;
	}
	return 1;
}

static void
run(FILE *fp, int argc, char *argv[],
    void (*fun)(struct member *, int argc, char *files[]))
{
	struct member m;

	m.src = fp;
	m.cur = BEFORE;

	while (fread(&m.hdr, sizeof(m.hdr), 1, fp) == 1) {
		fpos_t pos;

		if (!valid(&m)) {
			fprintf(stderr,
			        "ar:corrupted member '%s'\n",
			        m.fname);
			exit(1);
		}
		fgetpos(fp, &pos);
		(*fun)(&m, argc, argv);
		fsetpos(fp, &pos);
		fseek(fp, m.size+1 & ~1, SEEK_CUR);
	}
	if (ferror(fp) || fclose(fp) == EOF) {
		perror("ar:reading members");
		exit(1);
	}
}

static void
merge(void)
{
	FILE *fp, *fi;
	int c, i;


	if ((fp = fopen(arfile, "wb")) == NULL) {
		perror("ar:reopening archive");
		exit(1);
	}

	fputs(ARMAG, fp);

	for (i = 0; i < 3; i++) {
		if ((fi = tmps[i].fp) == NULL)
			continue;
		fseek(fi, 0, SEEK_SET);
		while ((c = getc(fi)) != EOF)
			putc(c, fp);
		if (ferror(fi)) {
			perror("ar:error in temporary");
			exit(1);
		}
	}

	if (fclose(fp) == EOF) {
		perror("ar:writing archive file");
		exit(1);
	}
}

static void
closetmp(int which)
{
	struct tmp *tmp = &tmps[which];

	if (!tmp->fp)
		return;
	if (fclose(tmp->fp) == EOF) {
		perror("ar:closing temporaries");
		exit(1);
	}
}

static void
opentmp(char *fname, int which)
{
	struct tmp *tmp = &tmps[which];

	if (lflag) {
		tmp->name = fname;
		tmp->fp = fopen(fname, "w+b");
	} else {
		tmp->fp = tmpfile();
	}

	if (tmp->fp == NULL) {
		perror("ar:creating temporary");
		exit(1);
	}
}

static void
doit(int key, char *argv[], int argc)
{
	FILE *fp;

	fp = openar();
	if (argc == 0 &&
	    (key == 'r' || key == 'd' || key == 'm' || key == 'q')) {
		if (fclose(fp) == EOF) {
			perror("ar:early close of archive file");
			exit(-1);
		}
		return;
	}

	if (key == 'r' || key == 'm' || key == 'd')
		opentmp("ar.tmp1", BEFORE);
	if (key == 'r' || key == 'm') {
		opentmp("ar.tmp2", INDOT);
		opentmp("ar.tmp3", AFTER);
	}

	switch (key) {
	case 'r':
		run(fp, argc, argv, update);
		insert(argc, argv);
		merge();
		break;
	case 'm':
		run(fp, argc, argv, move);
		merge();
		break;
	case 'd':
		run(fp, argc, argv, del);
		merge();
		break;
	case 't':
		run(fp, argc, argv, list);
		break;
	case 'p':
		run(fp, argc, argv, print);
		break;
	case 'x':
		run(fp, argc, argv, extract);
		break;
	case 'q':
		append(fp, argv);
		break;
	}

	closetmp(BEFORE);
	closetmp(INDOT);
	closetmp(AFTER);

	for ( ; argc-- > 0; ++argv) {
		if (*argv) {
			fprintf(stderr, "ar: No member named '%s'\n", *argv);
			exit(1);
		}
	}
}

static void
usage(void)
{
	fputs("ar [-drqtpmx][posname] [-vuaibcl] [posname] arfile name ...\n",
	      stderr);
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
	doit(key, ++argv, --argc);

	if (fflush(stdout) == EOF) {
		perror("ar:error writing to stdout");
		exit(1);
	}

	return 0;
}
