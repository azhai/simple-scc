#define LDBIN "ld"
#define ASBIN "as"

/* configure below your standard sys include paths */
char *sysincludes[] = {
	"%p/include/",
	NULL
};

/* configure below your system linker command line */
#define GCCLIBPATH "/usr/lib/gcc/x86_64-unknown-linux-gnu/10.2/"

char *ldcmd[] = {
	"-static",
	"-z","nodefaultlib",
        %NOPIE%
	"-o","%o",
	"-L","%p/lib/",
	"-L",GCCLIBPATH,
	"%p/lib/Scrt1.o",
	"%p/lib/crti.o",
	GCCLIBPATH "crtbeginS.o",
	"%c",
	GCCLIBPATH "crtendS.o",
	"%p/lib/crtn.o",
	"-lc",
	"-lgcc",
	"-lgcc_eh",
	NULL
};

/* configure below your system assembler command line */
char *ascmd[] = {
        "-o", "%o"
        NULL
};
