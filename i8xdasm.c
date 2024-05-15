
#include <memory.h>
#include "i8xdasm.h"

static const char i8xmnemo[] =
	"NOP???LXISTALDASTAXLDAXSHLDLHLDINXDCXINRDCRDADMVIR"
	"LCRRCRALRARDAACMASTCCMCMOVHLTADDADCSUBSBBANAXRAORA"
	"CMPPUSHPOPJMPCALLRETXTHLXCHGDIEIINOUTADIACISUISBIA"
	"NIXRIORICPIRSTJNZJZJNCJCJPOJPEJPJMCNZCZCNCCCCPOCPE"
	"CPCMRNZRZRNCRCRPORPERPRMPCHLSPHLBCDEHLMABDHSPBDHPS"
	"W0123456789ABCDEF"
	"DSUBARSHRLDERIMSIMLDEHLDESSHLXLHLXJNXJX"
;

#define I(off, len, fmt) (off | ((len-1) << 9) | (fmt << 11))
#define IOFF(ind) (i8xmdescr [ind] & 0x1FF)
#define ILEN(ind) (((i8xmdescr [ind] >> 9) & 0x3) + 1)
#define IFMT(ind) ((i8xmdescr [ind] >> 11) & 0xF)
#define	I85(ind) (i8xmdescr [ind] & 0x8000)
#define J(off, len, fmt) (I(off,len,fmt) | 0x8000)

static const unsigned short i8xmdescr [256] =
{
	I(  0,3,0), I(  6,3,9), I( 15,4,3), I( 31,3,3), I( 37,3,4), I( 40,3,4), I( 46,3,10), I( 49,3,0),
	J(267,4,0), I( 43,3,3), I( 19,4,3), I( 34,3,3), I( 37,3,4), I( 40,3,4), I( 46,3,10), I( 52,3,0),
	J(271,4,0), I(  6,3,9), I( 15,4,3), I( 31,3,3), I( 37,3,4), I( 40,3,4), I( 46,3,10), I( 55,3,0),
	J(275,4,0), I( 43,3,3), I( 19,4,3), I( 34,3,3), I( 37,3,4), I( 40,3,4), I( 46,3,10), I( 58,3,0),
	J(279,3,0), I(  6,3,9), I( 23,4,2), I( 31,3,3), I( 37,3,4), I( 40,3,4), I( 46,3,10), I( 61,3,0),
	J(285,4,0), I( 43,3,3), I( 27,4,2), I( 34,3,3), I( 37,3,4), I( 40,3,4), I( 46,3,10), I( 64,3,0),
	J(282,3,0), I(  6,3,9), I(  9,3,2), I( 31,3,3), I( 37,3,4), I( 40,3,4), I( 46,3,10), I( 67,3,0),
	J(289,4,0), I( 43,3,3), I( 12,3,2), I( 34,3,3), I( 37,3,4), I( 40,3,4), I( 46,3,10), I( 70,3,0),

	I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5),
	I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5),
	I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5),
	I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5),
	I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5),
	I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5),
	I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 76,3,0), I( 73,3,5),
	I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5), I( 73,3,5),

	I( 79,3,6), I( 79,3,6), I( 79,3,6), I( 79,3,6), I( 79,3,6), I( 79,3,6), I( 79,3,6), I( 79,3,6),
	I( 82,3,6), I( 82,3,6), I( 82,3,6), I( 82,3,6), I( 82,3,6), I( 82,3,6), I( 82,3,6), I( 82,3,6),
	I( 85,3,6), I( 85,3,6), I( 85,3,6), I( 85,3,6), I( 85,3,6), I( 85,3,6), I( 85,3,6), I( 85,3,6),
	I( 88,3,6), I( 88,3,6), I( 88,3,6), I( 88,3,6), I( 88,3,6), I( 88,3,6), I( 88,3,6), I( 88,3,6),
	I( 91,3,6), I( 91,3,6), I( 91,3,6), I( 91,3,6), I( 91,3,6), I( 91,3,6), I( 91,3,6), I( 91,3,6),
	I( 94,3,6), I( 94,3,6), I( 94,3,6), I( 94,3,6), I( 94,3,6), I( 94,3,6), I( 94,3,6), I( 94,3,6),
	I( 97,3,6), I( 97,3,6), I( 97,3,6), I( 97,3,6), I( 97,3,6), I( 97,3,6), I( 97,3,6), I( 97,3,6),
	I(100,3,6), I(100,3,6), I(100,3,6), I(100,3,6), I(100,3,6), I(100,3,6), I(100,3,6), I(100,3,6),

	I(204,3,0), I(107,3,7), I(164,3,2), I(110,3,2), I(184,3,2), I(103,4,7), I(137,3,1), I(161,3,8),
	I(207,2,0), I(117,3,0), I(167,2,2), J(161,3,11),I(187,2,2), I(113,4,2), I(140,3,1), I(161,3,8),
	I(209,3,0), I(107,3,7), I(169,3,2), I(134,3,1), I(189,3,2), I(103,4,7), I(143,3,1), I(161,3,8),
	I(212,2,0), J(293,4,0), I(172,2,2), I(132,2,1), I(192,2,2), J(301,3,2), I(146,3,1), I(161,3,8),
	I(214,3,0), I(107,3,7), I(174,3,2), I(120,4,0), I(194,3,2), I(103,4,7), I(149,3,1), I(161,3,8),
	I(217,3,0), I(224,4,0), I(177,3,2), I(124,4,0), I(197,3,2), J(297,4,0), I(152,3,1), I(161,3,8),
	I(220,2,0), I(107,3,7), I(180,2,2), I(128,2,0), I(200,2,2), I(103,4,7), I(155,3,1), I(161,3,8),
	I(222,2,0), I(228,4,0), I(182,2,2), I(130,2,0), I(202,2,2), J(304,2,2), I(158,3,1), I(161,3,8)
};

unsigned char i80ilen [256] =
{
	1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
	1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
	1, 3, 3, 1, 1, 1, 2, 1, 1, 1, 3, 1, 1, 1, 2, 1,
	1, 3, 3, 1, 1, 1, 2, 1, 1, 1, 3, 1, 1, 1, 2, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 3, 3, 3, 1, 2, 1, 1, 1, 3, 1, 3, 3, 2, 1,
	1, 1, 3, 2, 3, 1, 2, 1, 1, 1, 3, 2, 3, 1, 2, 1,
	1, 1, 3, 1, 3, 1, 2, 1, 1, 1, 3, 1, 3, 1, 2, 1,
	1, 1, 3, 1, 3, 1, 2, 1, 1, 1, 3, 1, 3, 1, 2, 1
};

#define HXOFS	251
#define PAIRPSW	245
#define PAIRSP	240
#define REGOFFS	232
#define IUNKN	3

#define I8XDASM_SPCMASK	0xFF

int i8xDasm (char *instr, const unsigned char op[3], int f)
{
	int ilen, ioff, ifmt, i;
	char *ip;
	signed char n = f & I8XDASM_SPCMASK, e = 0;

	ilen = ILEN (op[0]);
	ioff = IOFF (op[0]);
	if (I85(op [0]) && (f & I8XDASM_85) == 0)
		ioff = IUNKN, ilen = 3, e = 1;
	for (i = 0; i < ilen; i++) instr [i] = i8xmnemo [ioff + i];
	if (e) goto ext;
	ip = & instr [ilen];

	ifmt = IFMT (op[0]);
	if (ifmt != 0) {
		if ( n < 0 ) for ( i = 0, n = -n; i < n; i++, ip++)
			*ip = '\t';
		else {
			if ( n == 0 ) n = 5;
			i = n - ilen;
			if ( i < 1 ) i = 1;
			for (; i > 0; i--) *ip++ = ' ';
		}
	}

	switch (ifmt) {
	case 0: break;
	case 1:	ip [0] = i8xmnemo [HXOFS + (( op[1] >> 4 ) & 0xF)];
		ip [1] = i8xmnemo [HXOFS + (  op[1] & 0xF)];
		ip += 2;
		break;
	case 2:	ip [0] = i8xmnemo [HXOFS + (( op[2] >> 4 ) & 0xF)];
		ip [1] = i8xmnemo [HXOFS + (  op[2] & 0xF)];
		ip [2] = i8xmnemo [HXOFS + (( op[1] >> 4 ) & 0xF)];
		ip [3] = i8xmnemo [HXOFS + (  op[1] & 0xF)];
		ip += 4;
		break;
	case 3:	i = (op[0] >> 4) & 0x3;
		*ip++ = i8xmnemo [PAIRSP + i];
		if (i == 3) *ip++ = 'P';
		break;
	case 4:	i = (op[0] >> 3) & 0xF;
		*ip++ = i8xmnemo [REGOFFS + i];
		break;
	case 5:	*ip++ = i8xmnemo [REGOFFS + ((op[0] >> 3) & 0x7)];
		*ip++ = ',';
		if (f & I8XDASM_ASPC) *ip++ = ' ';
		*ip++ = i8xmnemo [REGOFFS + (op[0] & 0x7)];
		break;
	case 6:	*ip++ = i8xmnemo [REGOFFS + (op[0] & 0x7)];
		break;
	case 7:	i = (op[0] >> 4) & 0x3;
		*ip++ = i8xmnemo [PAIRPSW + i];
		if (i == 3) *ip++ = 'S', *ip++ = 'W';
		break;
	case 8:	*ip++ = '0' + ((op[0] >> 3) & 0x7);
		break;
	case 9:	i = ((op[0] >> 4) & 0x3);
		*ip++ = i8xmnemo [PAIRSP + i];
		if (i == 3) *ip++ = 'P';
		*ip++ = ',';
		if (f & I8XDASM_ASPC) *ip++ = ' ';
		*ip++ = i8xmnemo [HXOFS + (( op[2] >> 4 ) & 0xF)];
		*ip++ = i8xmnemo [HXOFS + (  op[2] & 0xF)];
		*ip++ = i8xmnemo [HXOFS + (( op[1] >> 4 ) & 0xF)];
		*ip++ = i8xmnemo [HXOFS + (  op[1] & 0xF)];
		break;
	case 10:*ip++ = i8xmnemo [REGOFFS + ((op[0] >> 3) & 0xF)];
		*ip++ = ',';
		if (f & I8XDASM_ASPC) *ip++ = ' ';
		*ip++ = i8xmnemo [HXOFS + (( op[1] >> 4 ) & 0xF)];
		*ip++ = i8xmnemo [HXOFS + (  op[1] & 0xF)];
		break;
	case 11: *ip++ = 'V'; break;
	default: *ip++ = '?';
	}
ext:	*ip = '\0';
	return i80ilen [ op[0] ];
}
