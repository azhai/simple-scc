#include <stdio.h>
#include <stdlib.h>

#include <scc/arg.h>
#include <scc/config.h>
#include <scc/scc.h>

#define MAXCMD 400

char *argv0;

static char *arch, *format, *prefix, *outfile;
static int debug;

static void
usage(void)
{
	fputs("usage: as [-o outfile][-m arch][-f format] file ...\n",
	      stderr);
	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
	int cnt, len, rem, r;
	char cmd[MAXCMD];

	if (!(arch = getenv("ARCH")))
		arch = ARCH;
	if (!(format = getenv("FORMAT")))
		format = FORMAT;
	if (!(prefix = getenv("SCCPREFIX")))
		prefix = PREFIX;

	ARGBEGIN {
	case 'o':
		outfile = EARGF(usage());
		break;
	case 'm':
		arch = EARGF(usage());
		break;
	case 'f':
		format = EARGF(usage());
		break;
	case 'd':
		debug = 1;
		break;
	} ARGEND

	if (argc < 1)
		usage();

	r = snprintf(cmd, sizeof(cmd), "%s/libexec/scc/as-%s", prefix, arch);
	if (r < 0 || r >= sizeof(cmd))
		goto toolong;

	cnt = r;
	rem = sizeof(cmd) - cnt;
	if (outfile) {
		r = snprintf(cmd + cnt, rem, " -o %s", outfile);
		if (r < 0 || r >= rem)
			goto toolong;
		rem -= r;
		cnt += r;
	}

	for ( ; *argv; ++argv) {
		r = snprintf(cmd + cnt, rem, " %s", *argv);
		if (r < 0 || r >= rem)
			goto toolong;
		rem -= r;
		cnt += r;
	}

	if (debug)
		fprintf(stderr, "as: command line '%s'\n", cmd);

	r = system(cmd);
	if (r == 0)
		return 0;
	if (r < 0)
		perror("as");
	exit(EXIT_FAILURE);

toolong:
	fputs("as: too long command line\n", stderr);
	exit(EXIT_FAILURE);
}
