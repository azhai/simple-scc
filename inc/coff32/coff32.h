typedef int (*packfun)(unsigned char *, char *, ...);

#ifdef AOUTHDR
extern void coff32_pack_aout(packfun fun, unsigned char *buff, AOUTHDR *aout);
extern void coff32_unpack_aout(packfun fun, unsigned char *buff, AOUTHDR *ent);
#endif

#ifdef SYMENT
extern void coff32_pack_ent(packfun fun, unsigned char *buff, SYMENT *ent);
extern void coff32_unpack_ent(packfun fun, unsigned char *buff, SYMENT *ent);
#endif

#ifdef FILHDR
extern void coff32_pack_hdr(packfun fun, unsigned char *buff, FILHDR *hdr);
extern void coff32_unpack_hdr(packfun fun, unsigned char *buff, FILHDR *hdr);
#endif

#ifdef SCNHDR
extern void coff32_pack_scn(packfun fun, unsigned char *buff, SCNHDR *scn);
extern void coff32_unpack_scn(packfun fun, unsigned char *buff, SCNHDR *scn);
#endif
