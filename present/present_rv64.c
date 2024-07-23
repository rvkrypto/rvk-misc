//	present_rv64.c
//	2021-11-03	Markku-Juhani O. Saarinen <mjos@iki.fi> See LICENSE.

//	=== RV64: Block Cipher PRESENT-80/128 (CHES 2007 / ISO/IEC 29192-2:2019).

#include "plat_local.h"
#ifdef PLAT_ARCH_RV64

#include <riscv_crypto.h>
#include <riscv_bitmanip.h>
#include "present_api.h"

//	S-Box (sLayer)
#define SBOX64_ENC 0x21748FE3DA09B65CLLU
#define SBOX64_DEC 0xA970364BD21C8FE5LLU

//	Nybble shuffle for pLayer
#define P64_NYBBLE 0xFB73EA62D951C840LLU

//	--- RV64 block encrypt (PRESENT-80/128)

uint64_t present_enc_rv64(uint64_t x, const uint64_t rk[32])
{
	int i;
	uint64_t y, z;

	for (i = 0; i < 31; i++) {

		//	key addition
		x ^= rk[i];

		//	sLayer
		x = __riscv_xperm4_64(SBOX64_ENC, x);

		//	pLayer
		y = x & 0x1111111111111111;
		y |= y >> 6;
		y |= y >> 3;
		z = y & 0x000F000F000F000F;

		y = x & 0x2222222222222222;
		y |= y >> 6;
		y |= y << 3;
		z |= y & 0x00F000F000F000F0;

		y = x & 0x4444444444444444;
		y |= y << 6;
		y |= y >> 3;
		z |= y & 0x0F000F000F000F00;

		y = x & 0x8888888888888888;
		y |= y << 6;
		y |= y << 3;
		z |= y & 0xF000F000F000F000;

		x = __riscv_xperm4_64(z, P64_NYBBLE);
	}
	x ^= rk[i];

	return x;
}

//	--- RV64 block decrypt (PRESENT-80/128)

uint64_t present_dec_rv64(uint64_t x, const uint64_t rk[32])
{
	int i;
	uint64_t y, z;

	for (i = 31; i > 0; i--) {

		//	key addition
		x ^= rk[i];

		//	inverse pLayer
		z = __riscv_xperm4_64(x, P64_NYBBLE);

		y = z & 0x000F000F000F000F;
		y |= y << 3;
		y |= y << 6;
		x = y & 0x1111111111111111;

		y = z & 0x00F000F000F000F0;
		y |= y >> 3;
		y |= y << 6;
		x |= y & 0x2222222222222222;

		y = z & 0x0F000F000F000F00;
		y |= y << 3;
		y |= y >> 6;
		x |= y & 0x4444444444444444;

		y = z & 0xF000F000F000F000;
		y |= y >> 3;
		y |= y >> 6;
		x |= y & 0x8888888888888888;

		//	inverse sLayer
		x = __riscv_xperm4_64(SBOX64_DEC, x);
	}
	x ^= rk[i];

	return x;
}

#endif
