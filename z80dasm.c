
#if defined(__linux__) || defined(__MINGW32__)
#include <stdio.h>
#endif
#include <string.h>
#include <stdlib.h>
#include "z80dasm.h"

static const char z80mnemo[] =
	"NOPLDINCDECADDEXXDJNZJRRLCARRCARLARRADAACPLSCFCCFB"
	"CDEHLSPHALTADCSUBSBCANDXORCPRETNPOPJPCALLPUSHRSTOU"
	"TINDIEIBCDEHLMA0123456789ABCDEFBCDEHLAFHLIXIYSLASR"
	"ASLLSRLBITRESSETNEGIMRETIRRDRLDLDIRCPIROUTINIRLDDR"
	"CPDRINDROUTDOTIROTDRDB";

#define I(off, len, fmt, ir) (off | ((len-1) << 8) | (fmt << 10) | (ir ? 0x8000 : 0))
#define IOFF(ind) (z80mdescr [ind] & 0xFF)
#define ILEN(ind) (((z80mdescr [ind] >> 8) & 0x3) + 1)
#define IFMT(ind) ((z80mdescr [ind] >> 10) & 0x1F)
#define IIR(ind) (z80mdescr [ind] & 0x8000)

static const unsigned short z80mdescr [256] =
{
	I( 0,3,0,0), I( 3,2,10,0),I( 3,2,15,0),I( 5,3,1,0), I( 5,3,2,0), I( 8,3,2,0), I( 3,2,3,0), I(23,4,0,0),
	I(14,2,11,0),I(11,3,14,1),I( 3,2,16,0),I( 8,3,1,0), I( 5,3,2,0), I( 8,3,2,0), I( 3,2,3,0), I(27,4,0,0),
	I(17,4,12,0),I( 3,2,10,0),I( 3,2,15,0),I( 5,3,1,0), I( 5,3,2,0), I( 8,3,2,0), I( 3,2,3,0), I(31,3,0,0),
	I(21,2,12,0),I(11,3,14,1),I( 3,2,16,0),I( 8,3,1,0), I( 5,3,2,0), I( 8,3,2,0), I( 3,2,3,0), I(34,3,0,0),
	I(21,2,13,0),I( 3,2,10,1),I( 3,2,15,1),I( 5,3,1,1), I( 5,3,2,1), I( 8,3,2,1), I( 3,2,3,1), I(37,3,0,0),
	I(21,2,13,0),I(11,3,14,1),I( 3,2,16,1),I( 8,3,1,1), I( 5,3,2,1), I( 8,3,2,1), I( 3,2,3,0), I(40,3,0,0),
	I(21,2,13,0),I( 3,2,10,0),I( 3,2,15,0),I( 5,3,1,0), I( 5,3,4,1), I( 8,3,4,1), I( 3,2,17,1),I(43,3,0,0),
	I(21,2,13,0),I(11,3,14,1),I( 3,2,16,0),I( 8,3,1,0), I( 5,3,2,0), I( 8,3,2,0), I( 3,2,3,0), I(46,3,0,0),

	I( 3,2,5,0), I( 3,2,5,0), I( 3,2,5,0), I( 3,2,5,0), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,0),
	I( 3,2,5,0), I( 3,2,5,0), I( 3,2,5,0), I( 3,2,5,0), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,0),
	I( 3,2,5,0), I( 3,2,5,0), I( 3,2,5,0), I( 3,2,5,0), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,0),
	I( 3,2,5,0), I( 3,2,5,0), I( 3,2,5,0), I( 3,2,5,0), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,0),
	I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1),
	I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1),
	I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I(57,4,0,0), I( 3,2,5,1),
	I( 3,2,5,0), I( 3,2,5,0), I( 3,2,5,0), I( 3,2,5,0), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,1), I( 3,2,5,0),

	I(11,3,6,0), I(11,3,6,0), I(11,3,6,0), I(11,3,6,0), I(11,3,6,1), I(11,3,6,1), I(11,3,6,1), I(11,3,6,0),
	I(61,3,6,0), I(61,3,6,0), I(61,3,6,0), I(61,3,6,0), I(61,3,6,1), I(61,3,6,1), I(61,3,6,1), I(61,3,6,0),
	I(64,3,6,0), I(64,3,6,0), I(64,3,6,0), I(64,3,6,0), I(64,3,6,1), I(64,3,6,1), I(64,3,6,1), I(64,3,6,0),
	I(67,3,6,0), I(67,3,6,0), I(67,3,6,0), I(67,3,6,0), I(67,3,6,1), I(67,3,6,1), I(67,3,6,1), I(67,3,6,0),
	I(70,3,6,0), I(70,3,6,0), I(70,3,6,0), I(70,3,6,0), I(70,3,6,1), I(70,3,6,1), I(70,3,6,1), I(70,3,6,0),
	I(73,3,6,0), I(73,3,6,0), I(73,3,6,0), I(73,3,6,0), I(73,3,6,1), I(73,3,6,1), I(73,3,6,1), I(73,3,6,0),
	I(74,2,6,0), I(74,2,6,0), I(74,2,6,0), I(74,2,6,0), I(74,2,6,1), I(74,2,6,1), I(74,2,6,1), I(74,2,6,0),
	I(76,2,6,0), I(76,2,6,0), I(76,2,6,0), I(76,2,6,0), I(76,2,6,1), I(76,2,6,0), I(76,2,6,1), I(76,2,6,0),

	I(78,3,8,0), I(82,3,9,0), I(85,2,7,0), I(85,2,18,0),I(87,4,7,0), I(91,4,9,0), I(11,3,19,0), I(95,3,20,0),
	I(78,3,8,0), I(78,3,0,0), I(85,2,7,0), I( 0,1,0,0), I(87,4,7,0), I(87,4,18,0),I(61,3,19,0), I(95,3,20,0),
	I(78,3,8,0), I(82,3,9,0), I(85,2,7,0), I(98,3,21,0),I(87,4,7,0), I(91,4,9,0), I(64,3,19,0), I(95,3,20,0),
	I(78,3,8,0), I(14,3,0,0), I(85,2,7,0), I(101,2,22,0),I(87,4,7,0),I( 0,1,0,0), I(67,3,19,0), I(95,3,20,0),
	I(78,3,8,0), I(82,3,9,1), I(85,2,7,0), I(14,2,23,1),I(87,4,7,0), I(91,4,9,1), I(70,3,19,0), I(95,3,20,0),
	I(78,3,8,0), I(85,2,24,1),I(85,2,7,0), I(14,2,25,0),I(87,4,7,0), I( 0,1,0,0), I(73,3,19,0), I(95,3,20,0),
	I(78,3,8,0), I(82,3,9,0), I(85,2,7,0), I(103,2,0,0),I(87,4,7,0), I(91,4,9,0), I(74,2,19,0), I(95,3,20,0),
	I(78,3,8,0), I(3,2,26,1), I(85,2,7,0), I(105,2,0,0),I(87,4,7,0), I( 0,1,0,0), I(76,2,19,0), I(95,3,20,0)
};

static const unsigned short z80mdsced [64] =
{
	I(101,2,27,0),I(98,3,28,0),I(67,3,14,0),I(3,2,29,0),I(166,3,0,0),I( 78,4,0,0),I(169,2,31,0),I( 3,2,1,0),
	I(101,2,27,0),I(98,3,28,0),I(61,3,14,0),I(3,2,30,0),I(166,3,0,1),I(171,4,0,0),I(169,2,31,1),I( 3,2,1,0),
	I(101,2,27,0),I(98,3,28,0),I(67,3,14,0),I(3,2,29,0),I(166,3,0,1),I( 78,4,0,1),I(169,2,31,0),I( 3,2,2,0),
	I(101,2,27,0),I(98,3,28,0),I(61,3,14,0),I(3,2,30,0),I(166,3,0,1),I(171,4,0,1),I(169,2,31,0),I( 3,2,2,0),
	I(101,2,27,0),I(98,3,28,0),I(67,3,14,0),I(3,2,29,0),I(166,3,0,1),I( 78,4,0,1),I(169,2,31,1),I(175,3,0,0),
	I(101,2,27,0),I(98,3,28,0),I(61,3,14,0),I(3,2,30,0),I(166,3,0,1),I(171,4,0,1),I(169,2,31,1),I(178,3,0,0),
	I(101,2,3,0), I( 0,1,0,0), I(67,3,14,0),I(3,2,29,0),I(166,3,0,1),I( 78,4,0,1),I(169,2,31,1),I( 0,1,0,0),
	I(101,2,27,0),I(98,3,28,0),I(61,3,14,0),I(3,2,30,0),I(166,3,0,1),I(171,4,0,1),I(169,2,31,1),I( 0,1,0,0)
};

#define IEDOFF(ind) (z80mdsced [ind] & 0xFF)
#define IEDLEN(ind) (((z80mdsced [ind] >> 8) & 0x3) + 1)
#define IEDFMT(ind) ((z80mdsced [ind] >> 10) & 0x1F)
#define J(off, len) ((off-EDSTART) | ((len-1) << 6))
#define IED2OFS(ind) ((z80mdsced2 [ind] & 0x3F) + EDSTART)
#define IED2LEN(ind) (((z80mdsced2 [ind] >> 6) & 3)+1)

#define HXOFS	115
#define PAIROFS	49
#define REGOFFS	107
#define PAIRAF	131
#define PAIRHLIR 139
#define EDSTART	181
#define EDDBOFS	220

static const unsigned char z80mdsced2 [16] =
{
	J(181,3), J(185,3), J(192,3), J(189,4), J(196,3), J(200,3), J(204,3), J(208,4),
	J(181,4), J(185,4), J(192,4), J(212,4), J(196,4), J(200,4), J(204,4), J(216,4)
};

static const unsigned char z80cbmofs1 [8] =
{	23, 27, 23, 27, 145, 148, 151, 154 };
static const unsigned char z80cbmofs2 [3] =
{	157, 160, 163	};

#define I8XDASM_SPCMASK	0xFF

#define O(ofs, len) (ofs | (len << 7))
#define OPROFS(n) (opdscol [n] & 0x3F)
#define OPRLEN(n) (opdscol [n] >> 7)

static const unsigned char opdsc [] = {
	1, 0, 0, 3, 4, 5, 5, 6, 2, 7, 8, 7, 10, 1, 8, 26, 27, 11, 12, 11, 13, 1, 14, 15, 15, 14,
	4, 3, 8, 6, 3, 16, 17, 25, 25, 17, 28, 13, 18, 29, 30, 31, 13, 0, 32, 32, 0, 19, 1, 1,
	19, 20, 21, 25, 25, 21, 34, 32, 22, 23, 22, 33, 24
};
static const unsigned char opdscol [] = {
	O(0,0),  O(1,0),  O(2,1),  O(4,0),  O(5,1),  O(7,1),  O(9,1),  O(11,0), O(12,0), O(13,1),
	O(15,1), O(17,0), O(18,1), O(20,1), O(22,1), O(24,1), O(26,1), O(28,0), O(29,1), O(31,0),
	O(32,1), O(34,1), O(36,1), O(38,0), O(39,1), O(41,1), O(43,1), O(45,1), O(47,1), O(49,1),
	O(51,0), O(52,1), O(54,1), O(56,1), O(58,0), O(59,1), O(61,1), O(63,1)
};
#ifndef _MSC_VER
static const char *opstr [] = {
	"A", "AF", "AF'", "(SP)", "DE", "HL", "SP", "(C)", "ED", "F"
};
#define OPSTR(i) opstr [i]
#else
static const char opstrs [] = "A\0AF\0AF'\0(SP)\0DE\0HL\0SP\0(C)\0ED\0F\0";
static const unsigned char opstri [] = { 0, 2, 5, 9, 14, 17, 20, 23, 27, 30 };
#define OPSTR(i) (opstrs + opstri [i])
#endif

static char *regpair  (char *s, int p);
static char *pairhlir (char *s, int p);
static char *hxbyte   (char *s, int p);
static char *hxword   (char *s, int p);
static char *addrir   (char *s, int p, int arg);
static char *regirhl  (char *s, int ir, int op);
static void emtpref   (char *s, int ir);
static int  chkpref   (int opc, int f);

static const char ccflags [] = "NZNCPOPEM";
static const unsigned char ccflagol [] =
	{ 0x20, 0x11, 0x22, 0x13, 0x24, 0x26, 0x14, 0x18 };

int Z80Dasm (char *instr, const unsigned char *op, int f, int pc)
{
	int ilen, ioff, ifmt, i;
	char *ip;
	signed char n = f & I8XDASM_SPCMASK, irn;
	unsigned char opc, ir = 0, pos = 0, il = 0, j;
	unsigned short arg;

pref:	switch (il++, op [pos++]) {
	case 0xdd: if ((!ir && chkpref (op [pos], f)) || ir)
			{ emtpref (instr, ir); return 1; }
		ir = 1; goto pref;
	case 0xfd: if ((!ir && chkpref (op [pos], f)) || ir)
			{ emtpref (instr, ir); return 1; }
		ir = 2; goto pref;
	case 0xcb: irn = op [pos];
		opc = op [pos + (ir ? 1 : 0)]; ilen = 3;
		if (opc < 0x40) {
			ioff = z80cbmofs1 [opc >> 3]; ifmt = 35;
			if ((opc & 0x30) == 0x10) ilen = 2;
		} else	ioff = z80cbmofs2 [(opc >> 6) - 1], ifmt = 36;
		if (ir) il++; break;
	case 0xed: if (ir && (f & Z80DASM_IPSEP))
			{ emtpref (instr, ir); return 1; }
		opc = op [pos++]; il ++; ir = 0;
		if ((opc & 0xC0) == 0x40) {
			i = opc - 0x40;
			if (z80mdsced [i] == 0) goto und;
			ioff = IEDOFF (i); ilen = IEDLEN (i); ifmt = IEDFMT (i);
			if (ifmt > 0 && ifmt <= 3) ifmt = 31 + ifmt;
			goto opca;
		} else	if ((opc & 0xE4) == 0xA0) {
			i = (opc & 0x18) >> 1 | (opc & 3);
			ioff = IED2OFS (i); ilen = IED2LEN (i); ifmt = 0;
		} else	{
und:			ioff = EDDBOFS; ilen = 2; ifmt = 37;
		}
		break;
	default: opc = op [pos-1];
		ilen = ILEN (opc); ioff = IOFF (opc); ifmt = IFMT (opc);
opca:		if (ir) irn = op [pos], arg = op [pos+1];
		else	arg = *(unsigned short*)&op [pos];
	}
	for (i = 0; i < ilen; i++) instr [i] = z80mnemo [ioff + i];
	ip = & instr [ilen];

	if (ifmt != 0) {
		if ( n < 0 ) for ( i = 0, n = -n; i < n; i++, ip++)
			*ip = '\t';
		else {
			if ( n == 0 ) n = 5;
			i = n - ilen;
			if ( i < 1 ) i = 1;
			for (; i > 0; i--) *ip++ = ' ';
		}

	pos = OPROFS (ifmt-1);
	n = OPRLEN (ifmt-1);
	for (ifmt = 0; ifmt <= n; ifmt++) {
		if (ifmt && j != 6) {
			*ip++ = ',';
			if (f & Z80DASM_ASPC) *ip++ = ' ';
		}
	switch (j = opdsc [ifmt + pos]) {
	case 0: i = opc >> 3 & 7; goto reg;
	case 1:	i = opc >> 4 & 0x3;
pair:		if (i == 2) ip = pairhlir (ip, ir);
		else	ip = regpair (ip, i); break;
	case 2: i = opc & 7;
reg:		if (ir && (i & 6) == 4) ip = regirhl (ip, ir, opc);
		else if (i == 6) {
			ip = addrir (ip, ir, arg); if (ir) il++;
		} else	*ip++ = z80mnemo [REGOFFS + i];
		break;
	case 3:	ip = hxbyte (ip, arg); il++; break;
	case 4:	ip = addrir (ip, ir, arg); if (ir) il++; break;
	case 5:	i = (ifmt ? opc : opc >> 3) & 7;
		if (i == 6) ip = addrir (ip, ir, irn);
		else if (ir && (i & 6) == 4 &&
				(opc >> 3 & 7) != 6 && (opc & 7) != 6)
			ip = regirhl (ip, ir, i);
		else	*ip++ = z80mnemo [REGOFFS + i];
		break;
	case 6:	if ((opc & 0x20) == 0 && (opc & 0x18) != 0x10) {
			*ip++ = 'A'; *ip++ = ',';
			if (f & Z80DASM_ASPC) *ip++ = ' ';
		}
		break;
	case 7:	i = opc >> 3 & 0x7;
cc:		j = ccflagol [i & 0x7] >> 4;
		strncpy (ip, ccflags + (ccflagol [i & 0x7] & 0xf), j); ip += j;
		break;
	case 8:	ip = hxword (ip, arg); il += 2; break;
	case 9:	*ip++ = ','; if (f & Z80DASM_ASPC) *ip++ = ' ';	break;
	case 10: i = opc >> 4 & 0x3;
		if (i == 2) ip = pairhlir (ip, ir);
		else	strncpy (ip, z80mnemo + PAIRAF + i * 2, 2), ip += 2;
		break;
	case 11: i = (signed char) arg + 2; if (i > 0) *ip++ = '+';
#ifndef __linux__
		ltoa (i, ip, 10); ip += strlen (ip);
#else
		ip += snprintf(ip, 10, "%d", i);
#endif
		il++; break;
	case 12: i = opc >> 3 & 0x3; goto cc;
	case 13: i = 2; goto pair;
	case 14: *ip++ = '(';
		if (opc & 0x20) ip = hxword (ip, arg), il += 2;
		else	ip = regpair (ip, opc >> 4 & 0x1);
		*ip++ = ')'; break;
	case 15: if (opc & 0x20 && ~opc & 0x10)
			ip = pairhlir (ip, ir);
		else	*ip++ = 'A'; break;
	case 16: ip = hxbyte (ip, opc & 0x38); break;
	case 17: *ip++ = '('; ip = hxbyte (ip, arg); *ip++ = ')'; il++;	break;
	case 18: *ip++ = '('; ip = pairhlir (ip, ir); *ip++ = ')'; break;
	case 19: *ip++ = '('; ip = hxword (ip, arg); *ip++ = ')'; il += 2; break;
	case 20: i = opc >> 3 & 3; if (i) i--; *ip++ = '0' + i; break;
	case 21: *ip++ = opc & 8 ? 'R' : 'I'; break;
	case 22: i = opc & 7;
		if (i == 6 || ir) {
			ip = addrir (ip, ir, irn);
			if (i != 6) *ip++ = ':';
		}
		if (i != 6) *ip++ = z80mnemo [REGOFFS + i];
		break;
	case 23: *ip++ = '0' + (opc >> 3 & 7); break;
	case 24: ip = hxbyte (ip, opc); break;
	default: strcpy (ip, OPSTR (j - 25)); ip += strlen (ip);
	} } }
	*ip = '\0';
	return il;
}

static char *regpair (char *s, int p)
{
	strncpy (s, z80mnemo + PAIROFS + p * 2, 2);
	return s + 2;
}

static char *pairhlir (char *s, int p)
{
	strncpy (s, z80mnemo + PAIRHLIR + p * 2, 2);
	return s + 2;
}

static char *hxbyte (char *s, int p)
{
	s [0] = z80mnemo [HXOFS + (p >> 4 & 0xF)];
	s [1] = z80mnemo [HXOFS + (p & 0xF)];
	return s + 2;
}

static char *hxword (char *s, int p)
{
	s = hxbyte (s, p >> 8);
	s = hxbyte (s, p);
	return s;
}

static char *addrir (char *s, int p, int arg)
{
	*s++ = '(';
	s = pairhlir (s, p);
	if (p != 0) {
		*s++ = '+';
#if defined(__linux__) || defined(__MINGW32__)
		s += snprintf(s, 10, "%u", arg & 0xFF);
#else
		ultoa (arg & 0xFF, s, 10);
		s += strlen (s);
#endif
	}
	*s++ = ')';
	return s;
}

static char *regirhl (char *s, int ir, int opc)
{
	s [0] = 'I';
	if (ir == 1) s [1] = 'X';
	else if (ir == 2) s [1] = 'Y';
	else s [1] = '?';
	s [2] = opc & 1 ? 'L' : 'H';
	return s + 3;
}

static const char sprf [] = " prefix";

static void emtpref (char *s, int ir)
{
	switch (ir) {
	case 1: s = hxbyte (s, 0xdd); break;
	case 2: s = hxbyte (s, 0xfd); break;
	default: s [0] = '?'; s [1] = '?'; s += 2;
	}
	strcpy (s, sprf);
}

static int chkpref (int opc, int f)
{
	return f & Z80DASM_IPSEP && !IIR (opc);
}
