/* configure below your system linker command line */

char *ldcmd[] = {
	"-static",
	"-z","nodefaultlib",
	%NOPIE%
	"-o","%o",
	"-L","%p/lib/scc/%a-%s",
	"%p/lib/scc/%a-%s/crt.o",
	"%c",
	"-lc",
	"-lcrt",
        NULL
};