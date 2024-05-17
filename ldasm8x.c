
#include <stdio.h>
#include <stdlib.h>
#ifdef __linux__
#ifndef __USE_GNU
#if defined(__GNUC__) && !defined(__llvm__)
//#pragma message "Probably need to define __USE_GNU for basename() function or include it from <libgen.h> header."
#endif
#define __USE_GNU
#include <errno.h>
#endif
#endif
#include <string.h>

#include "i8xdasm.h"
#include "z80dasm.h"

#define LDASM_DEF_CPU	2
#if LDASM_DEF_CPU<0 || LDASM_DEF_CPU>2
#error LDASM_DEF_CPU is in the range 0-2
#endif

#define BUFSIZE	256
#define MINBUFSIZE 5
#if MINBUFSIZE < 4
# error ASSERTING MINBUFSIZE >= 4
#endif
#if BUFSIZE < MINBUFSIZE+MINBUFSIZE-1
# error BUFSIZE < MINBUFSIZE*2
#endif

#ifdef _MSC_VER
#ifndef _CRTAPI1
#define _CRTAPI1 __cdecl
#endif
#else
#define _CRTAPI1
#endif

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#ifdef __linux__
#define strcmpI strcasecmp
#define strncmpI strncasecmp
#elif defined(_WIN32) || defined(__MSDOS__)
#define strcmpI stricmp
#define strncmpI strnicmp
#endif

FILE *fin, *fout;
char ctype=LDASM_DEF_CPU;

int base = 0;
#ifdef _WIN32
char bname [_MAX_FNAME];
#else
char *bname;
#endif
const char *_estr = "";

const char *infname = NULL, *outfname = NULL;
unsigned char tabmode = 0;
int tabs = 0;

void print_usage (int f);
static const char *skipsep(const char *c, int *i, int argc, const char *argv[]);
static const char *words[] = {
	"80", "85", "z80", "output", "tab-mode", "tab", "no-code"
};
static const char shopts[] =
{	'o', 'm', 't', 'n'	};
static const unsigned char shoptsn[] =
{	3, 4, 5, 6	};
static const char *hxtail[] = {
	"  ", "     ", "        ", "\t", "\t\t"
};
static char hxtailn[] =
{	2, 1, 0, 0, 3, 3, 3, 3, 4, 3, 3, 3	};

unsigned char buf[BUFSIZE];
char str[32];

int _CRTAPI1 main (int argc, const char *argv[])
{
	int i, nn=0;
	const char *c, *co;
	char ferr=0, f, fnoc=0, j, finc=0, foutc=0;
	unsigned char *ps, *pe;

#if defined(_WIN32) || defined(__MSDOS__)
	_splitpath (argv[0], NULL, NULL, bname, NULL);
#else
	bname = basename(argv[0]);
#endif
	for (i=1; i<argc; i++) {
		c = co = argv[i];
#if defined(_WIN32) || defined(__MSDOS__)
		if ((*c=='-' || *c=='/') && c[1]!='\0') {
#else
		if (*c=='-' && c[1]!='\0') {
#endif
			if (*++c=='-') c++;
#ifdef _WIN32 || defined(__MSDOS__)
			if (*c=='/') c++;
#endif
			if (!strcmpI(c,"h") || !strcmpI(c,"help")
#ifdef _WIN32 || defined(__MSDOS__)
				|| !strcmp(c,"?")
#endif
				) {	ferr=1; goto shu;	}
			for (j=0; j<_countof(words); j++)
				if (!strncmpI(c, words[j], strlen(words[j]))) break;
			if (j<_countof(words)) {
				c += strlen(words[j]);
				f = *c=='\0';
				if (j<=2&&f) {
					ctype=j; continue;
				}
nsw:				switch(j) {
				case 3: if ((c=skipsep(c, &i, argc, argv))) {
						outfname = c; f=1;
					}	else f=0;
					break;
				case 4: {
					int oldi = i;
					if ((c=skipsep(c, &i, argc, argv))) {
						char *ce;
						tabmode = strtol (c, &ce, 10);
						if (c == ce || tabmode < 0 || tabmode > 2 || (oldi!=i && *c!='\0')) {
							if (tabmode < 0 || tabmode > 2) tabmode = 0;
							printf("%s: warning: bad tab mode `%s%s%s`, using default (%d)\n", bname, co,
							oldi != i ? " " : _estr, oldi != i ? argv[i] : _estr, tabmode);
						}
						c = ce;
						if (*ce!='\0' && oldi == i) goto nxc;
						f=1;
					}	else f=0;
					break;
				}
				case 5: {
					int oldi = i;
					if ((c=skipsep(c, &i, argc, argv))) {
						char *ce;
						unsigned char t = tabs = strtol (c, &ce, 10);
						if (c == ce || tabs < 0 || tabs > 16 || (oldi!=i && *c!='\0')) {
							if (tabs < 0) tabs = 2;
							else if (tabs > 16) tabs = 16;
							printf("%s: warning: bad tab `%s%s%s`", bname, co,
							oldi != i ? " " : _estr, oldi != i ? argv[i] : _estr);
							if (t != tabs) printf(", truncating (%d -> %d)\n", t, tabs);
							else	puts(_estr);
						}
						c = ce;
						if (*ce!='\0' && oldi == i) goto nxc;
						f=1;
					}	else f=0;
					break;
				}
				case 6: fnoc=1; break;
				default: f=0;
				}
				if (f) continue;
			} else {
nxc:				for (j=0;j<_countof(shopts);j++)
					if (*c==shopts[j]) {
						j = shoptsn[j]; c++; goto nsw;
					}
			}
			goto unko;
		}
		if (*c=='#') {
			char *ce = (char*) c+1;
			base = strtol (ce, &ce, 16);
			if (*ce=='\0' && base & 0xffff)	continue;
			printf("%s: error: hex constant `%s`\n", bname, c);
			goto ferr;
		} else	switch (nn++) {
		case 0: infname = argv[i]; continue;
		case 1: outfname = argv[i]; continue;
		default: printf("%s: warning: ignoring `%s`\n", bname, co);
			goto ferr;
		}
unko:		printf("%s: warning: unknown option `%s`\n", bname, co);
ferr:		ferr=1;
	}
	if (argc == 1 || ferr) {
		if (ferr) puts(_estr);
shu:		print_usage(ferr ? 1 : 0);
		return 1;
	}
	if (infname==NULL) {
		printf("%s: required arguments are not set\n", bname);
		goto shu;
	}
	if (tabs==2) tabs=(unsigned char)-1 | (ctype==2 ? Z80DASM_ASPC : I8XDASM_ASPC);
	if (strcmp(infname,"-")==0) fin=stdin;
	else {
		fin=fopen(infname, "rb");
		if (!fin) {
			printf("%s: error: can't open file `%s`: %s\n",
				bname, infname, strerror(errno));
			return 2;
		}
		finc=1;
	}
	f=2;
	if (outfname==NULL || strcmp(outfname,"-")==0) fout=stdout;
	else {
		fout=fopen(outfname, "wb");
		if (!fout) {
			printf("%s: error: can't open file `%s`: %s\n",
				bname, outfname, strerror(errno));
			goto ex1;
		}
		foutc=1;
	}
	if (tabmode==1&&tabs==0)
			tabs=(unsigned char)-1 | (ctype==2 ? Z80DASM_ASPC : I8XDASM_ASPC);
	ps = pe = buf;
	do {
		nn = pe-ps;
		if (nn < MINBUFSIZE) {
			if (ps > buf) {
				memmove(buf, ps, nn);
				ps = buf; pe = buf + nn;
			}
			if (feof(fin)) {
				if (ps == buf)
					memset(pe, 0, MINBUFSIZE*2-1-nn);
				goto td;
			}
			nn = fread(pe, 1, BUFSIZE-nn, fin);
			if (nn==0) {
				if (ferror(fin)) {
					printf("%s: error: can't read file `%s`: %s\n",
						bname, infname, strerror(errno));
					goto ext;
				}
			}
			pe += nn;
		}
td:		fprintf (fout, "%04X", base);
		fwrite (fnoc && tabmode >= 1 ? "\t" : "  ",
			(tabmode==1&&!fnoc) || (fnoc&&tabmode==0) ? 2 : 1, 1, fout);
		switch (ctype) {
		case 0: nn = i80Dasm (str, ps, tabs); break;
		case 1: nn = i85Dasm (str, ps, tabs); break;
		case 2: nn = Z80Dasm (str, ps, Z80DASM_PCVAL | tabs, base); break;
		}
		if (!fnoc) {
			for (i=0; i<nn; i++) {
				if (i!=0 && nn!=4) fputc(' ', fout);
				fprintf(fout, "%02X", ps[i]);
			}
			c = hxtail [hxtailn [tabmode*4+nn-1]];
			fwrite(c, strlen(c), 1, fout);
		}
		fwrite(str, strlen(str), 1, fout);
		fputc ('\n', fout);
		base = (base+nn) & 0xffff;
		ps += nn;
	} while (ps < pe);
	f=0;
ext:	if (foutc) fclose(fout);
ex1:	if (finc) fclose(fin);
	return f;
}

static const char *skipsep(const char *c, int *i, int argc, const char *argv[])
{
	int n = *i;
	if (*c=='=' || *c==':') c++;
	if (*c!='\0') return c;
	if (n++ >= argc) return NULL;
	*i = n;
	return argv[n];
}

#if (!defined(__GNUC__) || __GNUC__ > 7) && !defined(__TURBOC__)
__inline
#endif
size_t prints(const char *s) {
	return fwrite(s, strlen(s), 1, stdout);
}

void print_usage (int f) {
	printf ("Linear Intel 8080, 8085 / Zilog Z80 disassembler%s\nUsage: %s",
		f ? _estr:" (-h[elp] for help)", bname);
	puts(" [-80|-85|-z80] bin-file [out-file] [#base_addr]");
	if (f)	puts("Code:\t-80\t- Intel 8080"
#if LDASM_DEF_CPU==0
		" (default)"
#endif
		"\n\t-85\t- Intel 8085"
#if LDASM_DEF_CPU==1
		" (default)"
#endif
		"\n\t-z80\t- Zilog Z80"
#if LDASM_DEF_CPU==2
		" (default)"
#endif
		"\nFormat:\t-n, --no-code\t- do not print hex code columns\n"
		"\t-m, --tab-mode n  - tab style n=0-2, 0 default;\n"
		"\t\tmode 1 and 2 use tab chars to pad columns\n"
		"\t-t, --tab n\t- tabulation control, n is a number:\n"
		"\t\tn=1 - one space character; n=2 - one tab char;\n"
		"\t\tother n is an operand start position\n"
		"Other:\t-o, --output file  - name of output file\n"
		"\t-h, --help\t- help");
	puts("File name `-` is stdin (stdout for output file). Base address is a hexadecimal.");
}
