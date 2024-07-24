//	aes_zvkned.h
//	2024-07-24	Markku-Juhani O. Saarinen <mjos@iki.fi> See LICENSE.

//	=== Implementation prototypes for aes_zvkned.S

#ifndef _AES_ZVKNED_H_
#define _AES_ZVKNED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//	Set encryption key

void aes128_enc_key_zvkned(uint32_t rk[AES128_RK_WORDS],
						   const uint8_t key[16]);

void aes256_enc_key_zvkned(uint32_t rk[AES256_RK_WORDS],
						   const uint8_t key[32]);

//	Encrypt a block

void aes128_enc_ecb_zvkned_lmul1(uint8_t *ct, const uint8_t *pt, size_t n,
								 const uint32_t rk[AES128_RK_WORDS]);

void aes256_enc_ecb_zvkned_lmul1(uint8_t *ct, const uint8_t *pt, size_t n,
								 const uint32_t rk[AES256_RK_WORDS]);


//	Set decryption key

void aes128_dec_key_zvkned(uint32_t rk[AES128_RK_WORDS],
						   const uint8_t key[16]);

void aes256_dec_key_zvkned(uint32_t rk[AES256_RK_WORDS],
						   const uint8_t key[32]);

//	Decrypt a block

void aes128_dec_ecb_zvkned_lmul1(uint8_t *pt, const uint8_t *ct, size_t n,
								 const uint32_t rk[AES128_RK_WORDS]);

void aes256_dec_ecb_zvkned_lmul1(uint8_t *pt, const uint8_t *ct, size_t n,
								 const uint32_t rk[AES256_RK_WORDS]);

#ifdef __cplusplus
}
#endif

#endif										//	_AES_ZVKNED_H_
