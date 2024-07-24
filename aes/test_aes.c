//	test_aes.c
//	2020-03-21	Markku-Juhani O. Saarinen <mjos@iki.fi> See LICENSE.

//	=== Unit tests for AES-128/192/256 (FIPS 197)

#include <string.h>
#include "plat_local.h"
#include "test_rvkat.h"

#include "aes/aes_api.h"
#include "aes/aes_zvkned.h"
#include "aes/aes_rv32zkned.h"
#include "aes/aes_rv64zkned.h"

//	count cycles for 1024 bytes / 64 blocks

uint32_t test_aes_cycle(void (*ciph)(uint8_t *dst, const uint8_t *src,
						size_t n, const uint32_t * rk),
						const uint32_t *rk)
{
	uint8_t xt[1024];
	uint32_t cc;

	for (size_t i = 0; i < 1024; i++) {
		xt[i] = i;
	}
	cc = plat_get_cycle();
	ciph(xt, xt, 1024, rk);
	cc = plat_get_cycle() - cc;

	return cc;
}

int test_aes128_ecb_tv(bool dec)
{
	uint8_t pt[16], ct[16], xt[16], key[16];
	uint32_t rk[AES128_RK_WORDS];
	int fail = 0;

	//	FIPS 197 test vectors
	rvkat_gethex(pt, sizeof(pt),
		"00112233445566778899AABBCCDDEEFF");
	rvkat_gethex(key, sizeof(key),
		"000102030405060708090A0B0C0D0E0F");
	aes128_enc_key(rk, key);
	aes128_enc_ecb(ct, pt, 16, rk);
	fail += rvkat_chkhex("AES-128 Enc 1", ct, 16,
		"69C4E0D86A7B0430D8CDB78070B4C55A");

	if (dec) {
		aes128_dec_key(rk, key);
		aes128_dec_ecb(xt, ct, 16, rk);
		fail += rvkat_chkhex("AES-128 Dec 1", xt, 16,
			"00112233445566778899AABBCCDDEEFF");
	}

	//	another test vector set (picked from SP800-38A)
	rvkat_gethex(key, sizeof(key),
		"2B7E151628AED2A6ABF7158809CF4F3C");
	aes128_enc_key(rk, key);
	rvkat_gethex(pt, sizeof(pt),
		"6BC1BEE22E409F96E93D7E117393172A");
	aes128_enc_ecb(ct, pt, 16, rk);
	fail += rvkat_chkhex("AES-128 Enc 2", ct, 16,
		"3AD77BB40D7A3660A89ECAF32466EF97");

	if (dec) {
		aes128_dec_key(rk, key);
		aes128_dec_ecb(xt, ct, 16, rk);
		fail += rvkat_chkhex("AES-128 Dec 2", xt, 16,
			"6BC1BEE22E409F96E93D7E117393172A");
	}

	rvkat_dec("AES-128 ECB Encrypt / 1024 B:",
		test_aes_cycle(aes128_enc_ecb, rk));
	if (dec) {
		rvkat_dec("AES-128 ECB Decrypt / 1024 B:",
			test_aes_cycle(aes128_dec_ecb, rk));
	}

	return fail;
}

int test_aes192_ecb_tv(bool dec)
{
	uint8_t pt[16], ct[16], xt[16], key[24];
	uint32_t rk[AES192_RK_WORDS];
	int fail = 0;

	//	FIPS 197
	rvkat_gethex(pt, sizeof(pt),
		"00112233445566778899AABBCCDDEEFF");
	rvkat_gethex(key, sizeof(key),
		"000102030405060708090A0B0C0D0E0F1011121314151617");
	aes192_enc_key(rk, key);
	aes192_enc_ecb(ct, pt, 16, rk);
	fail += rvkat_chkhex("AES-192 Enc 1", ct, 16,
		"DDA97CA4864CDFE06EAF70A0EC0D7191");

	if (dec) {
		aes192_dec_key(rk, key);
		aes192_dec_ecb(xt, ct, 16, rk);
		fail += rvkat_chkhex("AES-192 Dec 1", xt, 16,
			"00112233445566778899AABBCCDDEEFF");
	}

	//	Second
	rvkat_gethex(key, sizeof(key),
		"8E73B0F7DA0E6452C810F32B809079E562F8EAD2522C6B7B");
	aes192_enc_key(rk, key);
	rvkat_gethex(pt, sizeof(pt),
		"AE2D8A571E03AC9C9EB76FAC45AF8E51");
	aes192_enc_ecb(ct, pt, 16, rk);
	fail += rvkat_chkhex("AES-192 Enc 2", ct, 16,
		"974104846D0AD3AD7734ECB3ECEE4EEF");

	if (dec) {
		aes192_dec_key(rk, key);
		aes192_dec_ecb(xt, ct, 16, rk);
		fail += rvkat_chkhex("AES-192 Dec 2", xt, 16,
			"AE2D8A571E03AC9C9EB76FAC45AF8E51");
	}

	rvkat_dec("AES-192 ECB Encrypt / 1024 B:",
		test_aes_cycle(aes192_enc_ecb, rk));
	if (dec) {
		rvkat_dec("AES-192 ECB Decrypt / 1024 B:",
			test_aes_cycle(aes192_dec_ecb, rk));
	}

	return fail;
}

int test_aes256_ecb_tv(bool dec)
{
	uint8_t pt[16], ct[16], xt[16], key[32];
	uint32_t rk[AES256_RK_WORDS];
	int fail = 0;

	//	FIPS 197
	rvkat_gethex(pt, sizeof(pt),
		"00112233445566778899AABBCCDDEEFF");
	rvkat_gethex(key, sizeof(key),
		"000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F");
	aes256_enc_key(rk, key);
	aes256_enc_ecb(ct, pt, 16, rk);
	fail += rvkat_chkhex("AES-256 Enc 1", ct, 16,
		"8EA2B7CA516745BFEAFC49904B496089");

	if (dec) {
		aes256_dec_key(rk, key);
		aes256_dec_ecb(xt, ct, 16, rk);
		fail += rvkat_chkhex("AES-256 Dec 1", xt, 16,
			"00112233445566778899AABBCCDDEEFF");
	}

	//	Second
	rvkat_gethex(key, sizeof(key),
		"603DEB1015CA71BE2B73AEF0857D77811F352C073B6108D72D9810A30914DFF4");
	aes256_enc_key(rk, key);
	rvkat_gethex(pt, sizeof(pt),
		"30C81C46A35CE411E5FBC1191A0A52EF");
	aes256_enc_ecb(ct, pt, 16, rk);
	fail += rvkat_chkhex("AES-256 Enc 2", ct, 16,
		"B6ED21B99CA6F4F9F153E7B1BEAFED1D");

	if (dec) {
		aes256_dec_key(rk, key);
		aes256_dec_ecb(xt, ct, 16, rk);
		fail += rvkat_chkhex("AES-256 Dec 2", xt, 16,
			"30C81C46A35CE411E5FBC1191A0A52EF");
	}

	rvkat_dec("AES-256 ECB Encrypt / 1024 B:",
		test_aes_cycle(aes256_enc_ecb, rk));
	if (dec) {
		rvkat_dec("AES-256 ECB Decrypt / 1024 B:",
			test_aes_cycle(aes256_dec_ecb, rk));
	}
	return fail;
}

//	AES implementation tests

int test_aes()
{
	int fail = 0;

#ifdef PLAT_ARCH_RV32
	rvkat_info("=== AES Scalar 32-bit Zkned ===");

	aes128_enc_key = aes128_enc_key_rv32zkned;
	aes128_enc_ecb = aes128_enc_ecb_rv32zkned;
	aes128_dec_key = aes128_dec_key_rv32zkned;
	aes128_dec_ecb = aes128_dec_ecb_rv32zkned;
	fail += test_aes128_ecb_tv(true);

	aes192_enc_key = aes192_enc_key_rv32zkned;
	aes192_enc_ecb = aes192_enc_ecb_rv32zkned;
	aes192_dec_key = aes192_dec_key_rv32zkned;
	aes192_dec_ecb = aes192_dec_ecb_rv32zkned;
	fail += test_aes192_ecb_tv(true);

	aes256_enc_key = aes256_enc_key_rv32zkned;
	aes256_enc_ecb = aes256_enc_ecb_rv32zkned;
	aes256_dec_key = aes256_dec_key_rv32zkned;
	aes256_dec_ecb = aes256_dec_ecb_rv32zkned;
	fail += test_aes256_ecb_tv(true);

#endif

#ifdef PLAT_ARCH_RV64
	rvkat_info("=== AES Scalar 64-bit Zkned ===");

	aes128_enc_key = aes128_enc_key_rv64zkned;
	aes128_enc_ecb = aes128_enc_ecb_rv64zkned;
	aes128_dec_key = aes128_dec_key_rv64zkned;
	aes128_dec_ecb = aes128_dec_ecb_rv64zkned;
	fail += test_aes128_ecb_tv(true);

	aes192_enc_key = aes192_enc_key_rv64zkned;
	aes192_enc_ecb = aes192_enc_ecb_rv64zkned;
	aes192_dec_key = aes192_dec_key_rv64zkned;
	aes192_dec_ecb = aes192_dec_ecb_rv64zkned;
	fail += test_aes192_ecb_tv(true);

	aes256_enc_key = aes256_enc_key_rv64zkned;
	aes256_enc_ecb = aes256_enc_ecb_rv64zkned;
	aes256_dec_key = aes256_dec_key_rv64zkned;
	aes256_dec_ecb = aes256_dec_ecb_rv64zkned;
	fail += test_aes256_ecb_tv(true);
#endif

#ifdef PLAT_ARCH_RV64
	rvkat_info("=== AES Scalar 64-bit Zkned with On-the-fly keying ===");

	aes128_enc_ecb = aes128_enc_otf_rv64zkned;
	fail += test_aes128_ecb_tv(false);

	aes192_enc_ecb = aes192_enc_otf_rv64zkned;
	fail += test_aes192_ecb_tv(false);

	aes256_enc_ecb = aes256_enc_otf_rv64zkned;
	fail += test_aes256_ecb_tv(false);
#endif

#ifdef PLAT_RVV
	rvkat_info("=== AES Vector Zvkned ===");
	aes128_enc_key = aes128_enc_key_zvkned;
	aes128_enc_ecb = aes128_enc_ecb_zvkned_lmul1;
	aes128_dec_key = aes128_dec_key_zvkned;
	aes128_dec_ecb = aes128_dec_ecb_zvkned_lmul1;
	fail += test_aes128_ecb_tv(true);

	aes256_enc_key = aes256_enc_key_zvkned;
	aes256_enc_ecb = aes256_enc_ecb_zvkned_lmul1;
	aes256_dec_key = aes256_dec_key_zvkned;
	aes256_dec_ecb = aes256_dec_ecb_zvkned_lmul1;
	fail += test_aes256_ecb_tv(true);
#endif

	return fail;
}
