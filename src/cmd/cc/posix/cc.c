/*
 * FIXME: scc is a pure C99 source code, except this file that is
 * intended for a POSIX.2008 environment. This situation  creates
 * a problem where some systems require the macro _ANSI_SOURCE to
 * limit the namespace to a pure ISO namespace. NetBSD has a  bug
 * and using POSIX_C_SOURCE  and  _ANSI_SOURCE  at  the same time
 * generates a syntax  error in the system headers. A  patch  was
 * sent to NetBSD, but this temporary fix is added here until the
 * patch  arrives to the stable release.
 */
#define _POSIX_C_SOURCE 200809L
#undef _ANSI_SOURCE

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include <scc/arg.h>
#include <scc/scc.h>
#include <scc/sysincludes.h>
#include <scc/sysld.h>

enum {
	CC1,
	TEEIR,
	CC2,
	TEEQBE,
	QBE,
	TEEAS,
	AS,
	LD,
	LAST_TOOL,
};

static struct tool {
	char   cmd[PATH_MAX];
	char   bin[32];
	char  *outfile;
	struct items args;
	unsigned nparams;
	int    in, out, init;
	pid_t  pid;
} tools[] = {
	[CC1]    = {.cmd = "cc1"},
	[TEEIR]  = {.bin = "tee",   .cmd = "tee"},
	[CC2]    = {.cmd = "cc2"},
	[TEEQBE] = {.bin = "tee",   .cmd = "tee"},
	[QBE]    = {.bin = "qbe"},
	[TEEAS]  = {.bin = "tee",   .cmd = "tee"},
	[AS]     = {.bin = "as",    .cmd = "as"},
	[LD]     = {.bin = "ld",    .cmd = "ld"},
};

char *argv0;
static char *arch, *sys, *abi, *format;
static char *prefix, *objfile, *outfile;
static char *tmpdir;
static size_t tmpdirln;
static struct items objtmp, objout, linkargs;
static int Mflag, Eflag, Sflag, Wflag,
           cflag, dflag, kflag, sflag, Qflag = 1; /* TODO: Remove Qflag */
static int devnullfd = -1;

extern int failure;

static void
terminate(void)
{
	unsigned i;

	if (!kflag) {
		for (i = 0; i < objtmp.n; ++i)
			unlink(objtmp.s[i]);
	}
}

static void
addarg(int tool, char *arg)
{
	struct tool *t = &tools[tool];
	char *p, buff[FILENAME_MAX];
	size_t len, cnt;
	int n;

	if (t->args.n < 1)
		t->args.n = 1;

	if (!arg || arg[0] == '-') {
		newitem(&t->args, arg);
		return;
	}

	if (!strcmp(arg, "%c")) {
		for (n = 0; n < linkargs.n; ++n)
			newitem(&t->args, linkargs.s[n]);
		return;
	}

	for (cnt = 0 ; *arg && cnt < FILENAME_MAX; ++arg) {
		if (*arg != '%') {
			buff[cnt++] = *arg;
			continue;
		}

		switch (*++arg) {
		case 'a':
			p = arch;
			break;
		case 's':
			p = sys;
			break;
		case 'p':
			p = libprefix;
			break;
		case 'b':
			p = abi;
			break;
		case 'o':
			p = outfile;
			break;
		default:
			buff[cnt++] = *arg;
			continue;
		}

		len = strlen(p);
		if (len + cnt >= FILENAME_MAX)
			die("cc: pathname too long");
		memcpy(buff+cnt, p, len);
		cnt += len;
	}

	if (cnt >= FILENAME_MAX)
		abort();

	buff[cnt] = '\0';
	newitem(&t->args, xstrdup(buff));
}

static void
setargv0(int tool, char *arg)
{
	struct tool *t = &tools[tool];

	if (t->args.n > 0)
		t->args.s[0] = arg;
	else
		newitem(&t->args, arg);
}

static char *
cc12fmt(int tool)
{
	if (tool == CC1)
		return "%s";
	if (Qflag && !strcmp(arch, "amd64") && !strcmp(abi, "sysv"))
		return "%s-qbe_%s-%s";
	return "%s-%s-%s";
}

static int
inittool(int tool)
{
	struct tool *t = &tools[tool];
	char *crt, *fmt;
	int n;

	if (t->init)
		return tool;

	switch (tool) {
	case CC1:
		if (Wflag)
			addarg(tool, "-w");
		for (n = 0; sysincludes[n]; ++n) {
			addarg(tool, "-I");
			addarg(tool, sysincludes[n]);
		}
	case CC2:
		fmt = cc12fmt(tool);
		n = snprintf(t->bin, sizeof(t->bin), fmt, t->cmd, arch, abi);
		if (n < 0 || n >= sizeof(t->bin))
			die("cc: target tool name is too long");
	case QBE:
		n = snprintf(t->cmd, sizeof(t->cmd),
		             "%s/libexec/scc/%s", prefix, t->bin);
		if (n < 0 || n >= sizeof(t->cmd))
			die("cc: target tool path is too long");
		break;
	case LD:
		t->outfile = outfile;
		if (sflag)
			addarg(tool, "-s");
		for (n = 0; ldcmd[n]; n++)
			addarg(tool, ldcmd[n]);
		break;
	case AS:
		addarg(tool, "-o");
		break;
	default:
		break;
	}

	setargv0(tool, t->bin);
	t->nparams = t->args.n;
	t->init = 1;

	return tool;
}

static char *
outfname(char *path, char *type)
{
	char *new, sep, *p;
	size_t newsz, pathln;
	int tmpfd, n;

	if (path) {
		sep = '.';
		if (p = strrchr(path, '/'))
			path = p + 1;
		pathln = strlen(path);
		if (p = strrchr(path, '.'))
			pathln -= strlen(p);
	} else {
		sep = '/';
		type = "scc-XXXXXX";
		path = tmpdir;
		pathln = tmpdirln;
	}

	newsz = pathln + 1 + strlen(type) + 1;
	new = xmalloc(newsz);
	n = snprintf(new, newsz, "%.*s%c%s", (int)pathln, path, sep, type);
	if (n < 0 || n >= newsz)
		die("cc: wrong output filename");
	if (sep == '/') {
		if ((tmpfd = mkstemp(new)) < 0)
			die("cc: could not create output file '%s': %s",
			    new, strerror(errno));
		close(tmpfd);
	}

	return new;
}

static int
settool(int tool, char *infile, int nexttool)
{
	struct tool *t = &tools[tool];
	unsigned i;
	int fds[2];
	static int fdin = -1;

	switch (tool) {
	case TEEIR:
		t->outfile = outfname(infile, "ir");
		addarg(tool, t->outfile);
		break;
	case TEEQBE:
		t->outfile = outfname(infile, "qbe");
		addarg(tool, t->outfile);
		break;
	case TEEAS:
		t->outfile = outfname(infile, "s");
		addarg(tool, t->outfile);
		break;
	case AS:
		if (cflag && outfile) {
			objfile = outfile;
		} else {
			objfile = (cflag || kflag) ? infile : NULL;
			objfile = outfname(objfile, "o");
		}
		t->outfile = objfile;
		addarg(tool, t->outfile);
		break;
	default:
		break;
	}

	if (fdin > -1) {
		t->in = fdin;
		fdin = -1;
	} else {
		t->in = -1;
		if (infile)
			addarg(tool, infile);
	}

	if (nexttool < LAST_TOOL) {
		if (pipe(fds))
			die("cc: pipe: %s", strerror(errno));
		t->out = fds[1];
		fdin = fds[0];
	} else {
		t->out = -1;
	}

	addarg(tool, NULL);

	return tool;
}

static void
spawn(int tool)
{
	char **ap;
	struct tool *t = &tools[tool];

	switch (t->pid = fork()) {
	case -1:
		die("cc: %s: %s", t->bin, strerror(errno));
	case 0:
		if (t->out > -1)
			dup2(t->out, 1);
		if (t->in > -1)
			dup2(t->in, 0);
		if (!dflag && tool != CC1 && tool != LD)
			dup2(devnullfd, 2);
		if (dflag) {
			fprintf(stderr, "%s", t->cmd);
			for (ap = t->args.s+1; *ap; ap++)
				fprintf(stderr, " %s", *ap);
			putc('\n', stderr);
		}
		execvp(t->cmd, t->args.s);
		if (dflag) {
			fprintf(stderr,
			        "cc: execvp %s: %s\n",
				t->cmd,
			        strerror(errno));
		}
		abort();
	default:
		if (t->in > -1)
			close(t->in);
		if (t->out > -1)
			close(t->out);
		break;
	}
}

static int
toolfor(char *file)
{
	char *dot = strrchr(file, '.');

	if (Eflag)
		return CC1;

	if (dot) {
		if (!strcmp(dot, ".c"))
			return CC1;
		if (!strcmp(dot, ".ir"))
			return CC2;
		if (!strcmp(dot, ".qbe"))
			return QBE;
		if (!strcmp(dot, ".s"))
			return AS;
		if (!strcmp(dot, ".o"))
			return LD;
		if (!strcmp(dot, ".a"))
			return LD;
	} else if (!strcmp(file, "-")) {
		return CC1;
	}

	die("cc: unrecognized filetype of %s", file);
}

static int
valid(int tool, struct tool *t)
{
	int st;

	if (waitpid(t->pid, &st, 0) == -1 || WIFSIGNALED(st))
		goto internal;
	if (WIFEXITED(st) && WEXITSTATUS(st) == 0)
		return 1;
	if (!failure && (tool == CC1 || tool == LD))
		goto fail;

internal:
	if (!failure)
		fprintf(stderr, "cc:%s: internal error\n", t->bin);
fail:
	failure = 1;
	return 0;
}

static int
validatetools(void)
{
	struct tool *t;
	unsigned i;
	int tool, st, failed = LAST_TOOL;

	for (tool = 0; tool < LAST_TOOL; ++tool) {
		t = &tools[tool];
		if (!t->pid)
			continue;
		if (!valid(tool, t))
			failed = tool;
		if (tool >= failed && t->outfile)
			unlink(t->outfile);
		t->args.n = t->nparams;
		t->pid = 0;
	}
	if (failed < LAST_TOOL) {
		unlink(objfile);
		return 0;
	}

	return 1;
}

static int
buildfile(char *file, int tool)
{
	int nexttool;

	for (; tool < LAST_TOOL; tool = nexttool) {
		switch (tool) {
		case CC1:
			if (Eflag || Mflag)
				nexttool = LAST_TOOL;
			else
				nexttool = kflag ? TEEIR : CC2;
			break;
		case TEEIR:
			nexttool = CC2;
			break;
		case CC2:
			if (Qflag)
				nexttool = kflag ? TEEQBE : QBE;
			else
				nexttool = (Sflag || kflag) ? TEEAS : AS;
			break;
		case TEEQBE:
			nexttool = QBE;
			break;
		case QBE:
			nexttool = (Sflag || kflag) ? TEEAS : AS;
			break;
		case TEEAS:
			nexttool = Sflag ? LAST_TOOL : AS;
			break;
		case AS:
			nexttool = LAST_TOOL;
			break;
		default:
			nexttool = LAST_TOOL;
			continue;
		}

		spawn(settool(inittool(tool), file, nexttool));
	}

	return validatetools();
}

static void
build(struct items *chain, int link)
{
	int i, tool;

	for (i = 0; i < chain->n; ++i) {
		if (!strcmp(chain->s[i], "-l")) {
			newitem(&linkargs, chain->s[i++]);
			newitem(&linkargs, chain->s[i]);
			continue;
		}
		tool = toolfor(chain->s[i]);
		if (tool == LD) {
			newitem(&linkargs, chain->s[i]);
			continue;
		}
		if (buildfile(chain->s[i], tool)) {
			newitem(&linkargs, objfile);
			newitem((!link || kflag) ? &objout : &objtmp, objfile);
		}
	}
}

static void
usage(void)
{
	fputs("usage: cc [options] file...\n", stderr);
	exit(1);
}

int
main(int argc, char *argv[])
{
	struct items linkchain = { .n = 0, };
	int link, n;

	atexit(terminate);

	if (!(arch = getenv("ARCH")))
		arch = ARCH;
	if (!(sys = getenv("SYS")))
		sys = SYS;
	if (!(abi = getenv("ABI")))
		abi = ABI;
	if (!(format = getenv("FORMAT")))
		format = FORMAT;
	if (!(prefix = getenv("SCCPREFIX")))
		prefix = PREFIX;

	ARGBEGIN {
	case 'D':
		addarg(CC1, "-D");
		addarg(CC1, EARGF(usage()));
		break;
	case 'M':
		Mflag = 1;
		addarg(CC1, "-M");
		break;
	case 'E':
		Eflag = 1;
		addarg(CC1, "-E");
		break;
	case 'I':
		addarg(CC1, "-I");
		addarg(CC1, EARGF(usage()));
		break;
	case 'L':
		addarg(LD, "-L");
		addarg(LD, EARGF(usage()));
		break;
	case 'O':
		EARGF(usage());
		break;
	case 'S':
		Sflag = 1;
		break;
	case 'U':
		addarg(CC1, "-U");
		addarg(CC1, EARGF(usage()));
		break;
	case 'c':
		cflag = 1;
		break;
	case 'd':
		dflag = 1;
		break;
	case 'g':
		addarg(AS, "-g");
		addarg(LD, "-g");
		break;
	case 'k':
		kflag = 1;
		break;
	case 'l':
		newitem(&linkchain, "-l");
		newitem(&linkchain, EARGF(usage()));
		break;
	case 'm':
		arch = EARGF(usage());
		break;
	case 'o':
		outfile = EARGF(usage());
		break;
	case 's':
		sflag = 1;
		break;
	case 't':
		sys = EARGF(usage());
		break;
	case 'w':
		Wflag = 0;
		break;
	case 'W':
		Wflag = 1;
		break;
	case 'q':
		Qflag = 0;
		break;
	case 'Q':
		Qflag = 1;
		break;
	case '-':
		fprintf(stderr,
		        "cc: ignored parameter --%s\n", EARGF(usage()));
		break;
	default:
		usage();
	} ARGOPERAND {
operand:
		newitem(&linkchain, ARGOP());
	} ARGEND

	for (; *argv; --argc, ++argv)
		goto operand;

	if (Eflag && linkchain.n == 0)
		newitem(&linkchain, "-");

	if (Eflag && Mflag ||
            (Eflag || Mflag) && (Sflag || kflag) ||
	    linkchain.n == 0 ||
	    linkchain.n > 1 && cflag && outfile)
		usage();

	if (!dflag) {
		if ((devnullfd = open("/dev/null", O_WRONLY)) < 0)
			fputs("cc: could not open /dev/null\n", stderr);
	}

	if (!outfile)
		outfile = "a.out";

	if (!(tmpdir = getenv("TMPDIR")) || !tmpdir[0])
		tmpdir = ".";
	tmpdirln = strlen(tmpdir);

	build(&linkchain, (link = !(Mflag || Eflag || Sflag || cflag)));

	if (!(link || cflag))
		return failure;

	if (link && !failure) {
		inittool(LD);
		spawn(settool(LD, NULL, LAST_TOOL));
		validatetools();
	}

	return failure;
}
