//	test_sm3.c
//	2020-03-30	Markku-Juhani O. Saarinen <mjos@iki.fi> See LICENSE.

//	=== The Chinese Standard SM3 Hash Function
//	GB/T 32905-2016, GM/T 0004-2012, ISO/IEC 10118-3:2018

#include "plat_local.h"
#include "test_rvkat.h"
#include "sm3/sm3_api.h"

//	SM3: test vectors and algorithm tests

int test_sm3_vec()
{
	int fail = 0;
	uint8_t md[32], in[1024];
	uint32_t cc;

	//	simplified test with "abc" test vector from the standard
	sm3_256(md, "abc", 3);
	fail += rvkat_chkhex("SM3-256 1", md, 32,
		"66C7F0F462EEEDD9D1F2D46BDC10E4E24167C4875CF2F7A2297DA02B8F4BA8E0");

	//	second test
	sm3_256(md, in, rvkat_gethex(in, sizeof(in),
		"6162636461626364616263646162636461626364616263646162636461626364"
		"6162636461626364616263646162636461626364616263646162636461626364"));
	fail += rvkat_chkhex("SM3-256 2", md, 32,
		"DEBE9FF92275B8A138604889C18E5A4D6FDB70E5387E5765293DCBA39C0C5732");

	//	created with openssl
	for (size_t i = 0; i < 256; i++) {
		in[i] = i;
	}
	sm3_256(md, in, 256);
	fail += rvkat_chkhex("SM3-256 3", md, 32,
		"59D171DBFD251D5A4CD77D6BA2B7109B7D64A4CD7FA8182BEB100A016FA3AC44");

	for (size_t i = 0; i < 1024; i++) {
		in[i] = i & 0xFF;
	}
	cc = plat_get_cycle();
	sm3_compress((uint32_t *) md, (uint32_t *) in, 1024);	// 16 blocks
	cc = plat_get_cycle() - cc;
	rvkat_dec("SM3 hash / 1024 B:", cc);

	return fail;
}

int test_sm3()
{
	int fail = 0;

	rvkat_info("=== SM3 Scalar Intrinsics (sm3_cf256_zksh) ===");
	sm3_compress = &sm3_cf256_zksh;
	fail += test_sm3_vec();

	rvkat_info("=== SM3 Vector (sm3_cf256_zvksh_lmul1) ===");
	sm3_compress = &sm3_cf256_zvksh_lmul1;
	fail += test_sm3_vec();

/*
	rvkat_info("=== SM3 (sm3_cf256_zvksh_lmul2) ===");
	sm3_compress = &sm3_cf256_zvksh_lmul2;
	fail += test_sm3_vec();

	rvkat_info("=== SM3 (sm3_cf256_zvksh_lmul4) ===");
	sm3_compress = &sm3_cf256_zvksh_lmul4;
	fail += test_sm3_vec();
*/

	return fail;
}
