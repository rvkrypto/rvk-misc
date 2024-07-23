//	test_present.c
//	2021-10-30	Markku-Juhani O. Saarinen <mjos@iki.fi> See LICENSE.

//	=== Unit tests for PRESENT (CHES 2007 / ISO/IEC 29192-2:2019)

#include "plat_local.h"
#include <string.h>
#include <riscv_crypto.h>
#include "test_rvkat.h"
#include "present/present_api.h"

//	Test PRESENT-80

int test_present80()
{
	//	test triplets (key, pt, ct)
	const char *present80_tv[][3] = {

		//	from the Ches 2007 paper
		{	"00000000000000000000", "0000000000000000", "5579C1387B228445"	},
		{	"FFFFFFFFFFFFFFFFFFFF", "0000000000000000", "E72C46C0F5945049"	},
		{	"00000000000000000000", "FFFFFFFFFFFFFFFF", "A112FFC72F68417B"	},
		{	"FFFFFFFFFFFFFFFFFFFF", "FFFFFFFFFFFFFFFF", "3333DCD3213210D2"	},

		//	Additional test vector
		//{ "0F1E2D3C4B5A69788796", "40CCA0AD9FA9043C", "0123456789ABCDEF"	},
	};

	int fail = 0;
	int i;

	uint8_t pt[8], ct[8], key[10];

	for (i = 0; i < 4; i++) {

		//	PRESENT-80 key
		rvkat_gethex(key, sizeof(key), present80_tv[i][0]);

		//	Test encrypt
		rvkat_gethex(pt, sizeof(pt), present80_tv[i][1]);
		memset(ct, 0x55, sizeof(ct));

		present80_enc(ct, pt, key);
		fail += rvkat_chkhex("PRESENT-80 Encrypt",
								ct, 8, present80_tv[i][2]);

		//	Test decrypt
		rvkat_gethex(ct, sizeof(pt), present80_tv[i][2]);
		memset(pt, 0x55, sizeof(pt));
		present80_dec(pt, ct, key);

		fail += rvkat_chkhex("PRESENT-80 Decrypt",
								pt, 8, present80_tv[i][1]);
	}

	return fail;
}

//	Test PRESENT-128

int test_present128()
{
	//	test triplets (key, pt, ct)
	const char *present128_tv[][3] = {

		//	The first test vector is in A. Y. Poschmann's PhD Thesis (2009)
		{	"00000000000000000000000000000000",
			"0000000000000000", "96DB702A2E6900AF"	},
		{	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF",
			"0000000000000000", "13238C710272A5D8"	},
		{	"00000000000000000000000000000000",
			"FFFFFFFFFFFFFFFF", "3C6019E5E5EDD563"	},
		{	"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF",
			"FFFFFFFFFFFFFFFF", "628D9FBD4218E5B4"	},

		//	Additional test vector
		{	"0F1E2D3C4B5A69788796A5B4C3D2E1F0",
			"DA0E854A1E8D03E0", "0123456789ABCDEF"	},
	};

	int fail = 0;
	int i;

	uint8_t pt[8], ct[8], key[16];

	for (i = 0; i < 5; i++) {

		//	PRESENT-128 key
		rvkat_gethex(key, sizeof(key), present128_tv[i][0]);

		//	Test encrypt
		rvkat_gethex(pt, sizeof(pt), present128_tv[i][1]);
		memset(ct, 0x55, sizeof(ct));

		present128_enc(ct, pt, key);
		fail += rvkat_chkhex("PRESENT-128 Encrypt",
								ct, 8, present128_tv[i][2]);

		//	Test decrypt
		rvkat_gethex(ct, sizeof(pt), present128_tv[i][2]);
		memset(pt, 0x55, sizeof(pt));
		present128_dec(pt, ct, key);

		fail += rvkat_chkhex("PRESENT-128 Decrypt",
								pt, 8, present128_tv[i][1]);
	}

	return fail;
}

//	present driver

int test_present()
{
	int fail = 0;

#ifdef PLAT_ARCH_RV32
	rvkat_info("=== PRESENT using present_rv32.c ===");
	present_rk_enc = present_enc_rv32;
	present_rk_dec = present_dec_rv32;

	fail += test_present80();
	fail += test_present128();
#endif

#ifdef PLAT_ARCH_RV64
	rvkat_info("=== PRESENT using present_rv64.c ===");
	present_rk_enc = present_enc_rv64;
	present_rk_dec = present_dec_rv64;

	fail += test_present80();
	fail += test_present128();
#endif
	return fail;
}
