//	sm3_api.c
//	2020-03-10	Markku-Juhani O. Saarinen <mjos@iki.fi> See LICENSE.

//	=== The Chinese Standard SM3 Hash Function
//	GB/T 32905-2016, GM/T 0004-2012, ISO/IEC 10118-3:2018

//	Simple wrapper for the compression function

#include <string.h>
#include "sm3_api.h"

#include <stdio.h>

//	pointer to the compression function
void (*sm3_compress)(uint32_t *sp, const uint32_t *mp, size_t n)
	= &sm3_cf256_zvksh_lmul1;		//	vector version, lmul1

//	Compute 32-byte message digest to "md" from "in" which has "inlen" bytes
void sm3_256(uint8_t *md, const void *in, size_t inlen)
{
	size_t i;
	uint64_t x;
	uint32_t s[8], m[16];

	uint8_t *bp = (uint8_t *) m;
	const uint8_t *p = in;

	//	initial values (represented as little endian)
	s[0] = 0x6f168073;
	s[1] = 0xb9b21449;
	s[2] = 0xd7422417;
	s[3] = 0x00068ada;
	s[4] = 0xbc306fa9;
	s[5] = 0xaa383116;
	s[6] = 0x4dee8de3;
	s[7] = 0x4e0efbb0;

	//	"md padding"
	x = inlen << 3;							//	length in bits

	while (inlen >= 64) {					//	full blocks
		i = inlen & ~63;
		sm3_compress(s, (uint32_t *) p, i);
		inlen -= i;
		p += i;
	}
	memcpy(bp, p, inlen);					//	last data block
	bp[inlen++] = 0x80;
	if (inlen > 56) {
		memset(bp + inlen, 0x00, 64 - inlen);
		sm3_compress(s, m, 64);
		inlen = 0;
	}
	i = 64;									//	process length
	while (x > 0) {
		bp[--i] = x & 0xFF;
		x >>= 8;
	}
	memset(&bp[inlen], 0x00, i - inlen);
	sm3_compress(s, m, 64);

	//	store output
	memcpy(md, s, 32);
}
