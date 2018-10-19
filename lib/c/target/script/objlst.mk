
LIBOBJ = bsearch.o qsort.o \
         abs.o __abs.o labs.o __labs.o llabs.o __llabs.o \
         perror.o strerror.o \
         rand.o tmpnam.o \
         sprintf.o snprintf.o vsprintf.o vsnprintf.o \
         printf.o fprintf.o vfprintf.o \
         fgets.o gets.o fgetc.o fputc.o getchar.o putchar.o \
         fputs.o puts.o fread.o fwrite.o \
         getc.o putc.o __putc.o __getc.o \
         ftell.o rewind.o fseek.o ferror.o feof.o clearerr.o \
         setbuf.o setvbuf.o \
         fclose.o fopen.o freopen.o _fpopen.o _flsbuf.o stdio.o \
         realloc.o calloc.o malloc.o \
         __assert.o strcpy.o strcmp.o strlen.o strchr.o \
         strrchr.o strcat.o strncmp.o strncpy.o strncat.o strcoll.o \
         strxfrm.o strstr.o strspn.o strcspn.o strpbrk.o strtok.o \
         memset.o memcpy.o memmove.o memcmp.o memchr.o \
         isalnum.o isalpha.o isascii.o isblank.o iscntrl.o isdigit.o \
         isgraph.o islower.o isprint.o ispunct.o isspace.o isupper.o \
         isxdigit.o toupper.o tolower.o ctype.o setlocale.o \
         localeconv.o atoi.o atol.o atoll.o atexit.o abort.o exit.o \
         mktime.o localtime.o gmtime.o difftime.o \
         _daysyear.o ctime.o asctime.o strftime.o \
         errno.o _sys_errlist.o strnlen.o

#rules
__abs.o: ../../__abs.c
	$(CC) $(SCC_CFLAGS) ../../__abs.c -c

__assert.o: ../../__assert.c
	$(CC) $(SCC_CFLAGS) ../../__assert.c -c

__getc.o: ../../__getc.c
	$(CC) $(SCC_CFLAGS) ../../__getc.c -c

__labs.o: ../../__labs.c
	$(CC) $(SCC_CFLAGS) ../../__labs.c -c

__llabs.o: ../../__llabs.c
	$(CC) $(SCC_CFLAGS) ../../__llabs.c -c

__putc.o: ../../__putc.c
	$(CC) $(SCC_CFLAGS) ../../__putc.c -c

_daysyear.o: ../../_daysyear.c
	$(CC) $(SCC_CFLAGS) ../../_daysyear.c -c

_flsbuf.o: ../../_flsbuf.c
	$(CC) $(SCC_CFLAGS) ../../_flsbuf.c -c

_fpopen.o: ../../_fpopen.c
	$(CC) $(SCC_CFLAGS) ../../_fpopen.c -c

abort.o: ../../abort.c
	$(CC) $(SCC_CFLAGS) ../../abort.c -c

abs.o: ../../abs.c
	$(CC) $(SCC_CFLAGS) ../../abs.c -c

asctime.o: ../../asctime.c
	$(CC) $(SCC_CFLAGS) ../../asctime.c -c

atexit.o: ../../atexit.c
	$(CC) $(SCC_CFLAGS) ../../atexit.c -c

atoi.o: ../../atoi.c
	$(CC) $(SCC_CFLAGS) ../../atoi.c -c

atol.o: ../../atol.c
	$(CC) $(SCC_CFLAGS) ../../atol.c -c

atoll.o: ../../atoll.c
	$(CC) $(SCC_CFLAGS) ../../atoll.c -c

bsearch.o: ../../bsearch.c
	$(CC) $(SCC_CFLAGS) ../../bsearch.c -c

calloc.o: ../../calloc.c
	$(CC) $(SCC_CFLAGS) ../../calloc.c -c

clearerr.o: ../../clearerr.c
	$(CC) $(SCC_CFLAGS) ../../clearerr.c -c

ctime.o: ../../ctime.c
	$(CC) $(SCC_CFLAGS) ../../ctime.c -c

ctype.o: ../../ctype.c
	$(CC) $(SCC_CFLAGS) ../../ctype.c -c

difftime.o: ../../difftime.c
	$(CC) $(SCC_CFLAGS) ../../difftime.c -c

errno.o: ../../errno.c
	$(CC) $(SCC_CFLAGS) ../../errno.c -c

exit.o: ../../exit.c
	$(CC) $(SCC_CFLAGS) ../../exit.c -c

fclose.o: ../../fclose.c
	$(CC) $(SCC_CFLAGS) ../../fclose.c -c

feof.o: ../../feof.c
	$(CC) $(SCC_CFLAGS) ../../feof.c -c

ferror.o: ../../ferror.c
	$(CC) $(SCC_CFLAGS) ../../ferror.c -c

fgetc.o: ../../fgetc.c
	$(CC) $(SCC_CFLAGS) ../../fgetc.c -c

fgets.o: ../../fgets.c
	$(CC) $(SCC_CFLAGS) ../../fgets.c -c

fopen.o: ../../fopen.c
	$(CC) $(SCC_CFLAGS) ../../fopen.c -c

fprintf.o: ../../fprintf.c
	$(CC) $(SCC_CFLAGS) ../../fprintf.c -c

fputc.o: ../../fputc.c
	$(CC) $(SCC_CFLAGS) ../../fputc.c -c

fputs.o: ../../fputs.c
	$(CC) $(SCC_CFLAGS) ../../fputs.c -c

fread.o: ../../fread.c
	$(CC) $(SCC_CFLAGS) ../../fread.c -c

freopen.o: ../../freopen.c
	$(CC) $(SCC_CFLAGS) ../../freopen.c -c

fseek.o: ../../fseek.c
	$(CC) $(SCC_CFLAGS) ../../fseek.c -c

ftell.o: ../../ftell.c
	$(CC) $(SCC_CFLAGS) ../../ftell.c -c

fwrite.o: ../../fwrite.c
	$(CC) $(SCC_CFLAGS) ../../fwrite.c -c

getc.o: ../../getc.c
	$(CC) $(SCC_CFLAGS) ../../getc.c -c

getchar.o: ../../getchar.c
	$(CC) $(SCC_CFLAGS) ../../getchar.c -c

gets.o: ../../gets.c
	$(CC) $(SCC_CFLAGS) ../../gets.c -c

gmtime.o: ../../gmtime.c
	$(CC) $(SCC_CFLAGS) ../../gmtime.c -c

isalnum.o: ../../isalnum.c
	$(CC) $(SCC_CFLAGS) ../../isalnum.c -c

isalpha.o: ../../isalpha.c
	$(CC) $(SCC_CFLAGS) ../../isalpha.c -c

isascii.o: ../../isascii.c
	$(CC) $(SCC_CFLAGS) ../../isascii.c -c

isblank.o: ../../isblank.c
	$(CC) $(SCC_CFLAGS) ../../isblank.c -c

iscntrl.o: ../../iscntrl.c
	$(CC) $(SCC_CFLAGS) ../../iscntrl.c -c

isdigit.o: ../../isdigit.c
	$(CC) $(SCC_CFLAGS) ../../isdigit.c -c

isgraph.o: ../../isgraph.c
	$(CC) $(SCC_CFLAGS) ../../isgraph.c -c

islower.o: ../../islower.c
	$(CC) $(SCC_CFLAGS) ../../islower.c -c

isprint.o: ../../isprint.c
	$(CC) $(SCC_CFLAGS) ../../isprint.c -c

ispunct.o: ../../ispunct.c
	$(CC) $(SCC_CFLAGS) ../../ispunct.c -c

isspace.o: ../../isspace.c
	$(CC) $(SCC_CFLAGS) ../../isspace.c -c

isupper.o: ../../isupper.c
	$(CC) $(SCC_CFLAGS) ../../isupper.c -c

isxdigit.o: ../../isxdigit.c
	$(CC) $(SCC_CFLAGS) ../../isxdigit.c -c

labs.o: ../../labs.c
	$(CC) $(SCC_CFLAGS) ../../labs.c -c

llabs.o: ../../llabs.c
	$(CC) $(SCC_CFLAGS) ../../llabs.c -c

localeconv.o: ../../localeconv.c
	$(CC) $(SCC_CFLAGS) ../../localeconv.c -c

localtime.o: ../../localtime.c
	$(CC) $(SCC_CFLAGS) ../../localtime.c -c

malloc.o: ../../malloc.c
	$(CC) $(SCC_CFLAGS) ../../malloc.c -c

memchr.o: ../../memchr.c
	$(CC) $(SCC_CFLAGS) ../../memchr.c -c

memcmp.o: ../../memcmp.c
	$(CC) $(SCC_CFLAGS) ../../memcmp.c -c

memcpy.o: ../../memcpy.c
	$(CC) $(SCC_CFLAGS) ../../memcpy.c -c

memmove.o: ../../memmove.c
	$(CC) $(SCC_CFLAGS) ../../memmove.c -c

memset.o: ../../memset.c
	$(CC) $(SCC_CFLAGS) ../../memset.c -c

mktime.o: ../../mktime.c
	$(CC) $(SCC_CFLAGS) ../../mktime.c -c

perror.o: ../../perror.c
	$(CC) $(SCC_CFLAGS) ../../perror.c -c

printf.o: ../../printf.c
	$(CC) $(SCC_CFLAGS) ../../printf.c -c

putc.o: ../../putc.c
	$(CC) $(SCC_CFLAGS) ../../putc.c -c

putchar.o: ../../putchar.c
	$(CC) $(SCC_CFLAGS) ../../putchar.c -c

puts.o: ../../puts.c
	$(CC) $(SCC_CFLAGS) ../../puts.c -c

qsort.o: ../../qsort.c
	$(CC) $(SCC_CFLAGS) ../../qsort.c -c

rand.o: ../../rand.c
	$(CC) $(SCC_CFLAGS) ../../rand.c -c

realloc.o: ../../realloc.c
	$(CC) $(SCC_CFLAGS) ../../realloc.c -c

rewind.o: ../../rewind.c
	$(CC) $(SCC_CFLAGS) ../../rewind.c -c

setbuf.o: ../../setbuf.c
	$(CC) $(SCC_CFLAGS) ../../setbuf.c -c

setlocale.o: ../../setlocale.c
	$(CC) $(SCC_CFLAGS) ../../setlocale.c -c

setvbuf.o: ../../setvbuf.c
	$(CC) $(SCC_CFLAGS) ../../setvbuf.c -c

snprintf.o: ../../snprintf.c
	$(CC) $(SCC_CFLAGS) ../../snprintf.c -c

sprintf.o: ../../sprintf.c
	$(CC) $(SCC_CFLAGS) ../../sprintf.c -c

stdio.o: ../../stdio.c
	$(CC) $(SCC_CFLAGS) ../../stdio.c -c

strcat.o: ../../strcat.c
	$(CC) $(SCC_CFLAGS) ../../strcat.c -c

strchr.o: ../../strchr.c
	$(CC) $(SCC_CFLAGS) ../../strchr.c -c

strcmp.o: ../../strcmp.c
	$(CC) $(SCC_CFLAGS) ../../strcmp.c -c

strcoll.o: ../../strcoll.c
	$(CC) $(SCC_CFLAGS) ../../strcoll.c -c

strcpy.o: ../../strcpy.c
	$(CC) $(SCC_CFLAGS) ../../strcpy.c -c

strcspn.o: ../../strcspn.c
	$(CC) $(SCC_CFLAGS) ../../strcspn.c -c

strerror.o: ../../strerror.c
	$(CC) $(SCC_CFLAGS) ../../strerror.c -c

strftime.o: ../../strftime.c
	$(CC) $(SCC_CFLAGS) ../../strftime.c -c

strlen.o: ../../strlen.c
	$(CC) $(SCC_CFLAGS) ../../strlen.c -c

strncat.o: ../../strncat.c
	$(CC) $(SCC_CFLAGS) ../../strncat.c -c

strncmp.o: ../../strncmp.c
	$(CC) $(SCC_CFLAGS) ../../strncmp.c -c

strncpy.o: ../../strncpy.c
	$(CC) $(SCC_CFLAGS) ../../strncpy.c -c

strnlen.o: ../../strnlen.c
	$(CC) $(SCC_CFLAGS) ../../strnlen.c -c

strpbrk.o: ../../strpbrk.c
	$(CC) $(SCC_CFLAGS) ../../strpbrk.c -c

strrchr.o: ../../strrchr.c
	$(CC) $(SCC_CFLAGS) ../../strrchr.c -c

strspn.o: ../../strspn.c
	$(CC) $(SCC_CFLAGS) ../../strspn.c -c

strstr.o: ../../strstr.c
	$(CC) $(SCC_CFLAGS) ../../strstr.c -c

strtok.o: ../../strtok.c
	$(CC) $(SCC_CFLAGS) ../../strtok.c -c

strxfrm.o: ../../strxfrm.c
	$(CC) $(SCC_CFLAGS) ../../strxfrm.c -c

tmpnam.o: ../../tmpnam.c
	$(CC) $(SCC_CFLAGS) ../../tmpnam.c -c

tolower.o: ../../tolower.c
	$(CC) $(SCC_CFLAGS) ../../tolower.c -c

toupper.o: ../../toupper.c
	$(CC) $(SCC_CFLAGS) ../../toupper.c -c

vfprintf.o: ../../vfprintf.c
	$(CC) $(SCC_CFLAGS) ../../vfprintf.c -c

vsnprintf.o: ../../vsnprintf.c
	$(CC) $(SCC_CFLAGS) ../../vsnprintf.c -c

vsprintf.o: ../../vsprintf.c
	$(CC) $(SCC_CFLAGS) ../../vsprintf.c -c

