
#define MAX_LIB_PATHS	32

char *syslibs[MAX_LIB_PATHS + 1] = {
	PREFIX "/lib/scc/" ,
	/* configure below your standard sys include paths */
	PREFIX "/lib/",
	PREFIX "/local/lib/",
	NULL
};
