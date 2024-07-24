//	aes_rv32zkned.h
//	2020-05-05	Markku-Juhani O. Saarinen <mjos@iki.fi> See LICENSE.

//	=== Implementation prototypes for aes_rv32zkned.c

#ifndef _AES_RV32ZKNED_H_
#define _AES_RV32ZKNED_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//	Set encryption key

void aes128_enc_key_rv32zkned(uint32_t rk[AES128_RK_WORDS],
							  const uint8_t key[16]);

void aes192_enc_key_rv32zkned(uint32_t rk[AES192_RK_WORDS],
							  const uint8_t key[24]);

void aes256_enc_key_rv32zkned(uint32_t rk[AES256_RK_WORDS],
							  const uint8_t key[32]);

//	Encrypt a block

void aes128_enc_ecb_rv32zkned(uint8_t *ct, const uint8_t *pt, size_t n,
							  const uint32_t rk[AES128_RK_WORDS]);

void aes192_enc_ecb_rv32zkned(uint8_t *ct, const uint8_t *pt, size_t n,
							  const uint32_t rk[AES192_RK_WORDS]);

void aes256_enc_ecb_rv32zkned(uint8_t *ct, const uint8_t *pt, size_t n,
							  const uint32_t rk[AES256_RK_WORDS]);

//	Set decryption key

void aes128_dec_key_rv32zkned(uint32_t rk[AES128_RK_WORDS],
							  const uint8_t key[16]);

void aes192_dec_key_rv32zkned(uint32_t rk[AES192_RK_WORDS],
							  const uint8_t key[24]);

void aes256_dec_key_rv32zkned(uint32_t rk[AES256_RK_WORDS],
							  const uint8_t key[32]);

//	Decrypt a block

void aes128_dec_ecb_rv32zkned(uint8_t *pt, const uint8_t *ct, size_t n,
							  const uint32_t rk[AES128_RK_WORDS]);

void aes192_dec_ecb_rv32zkned(uint8_t *pt, const uint8_t *ct, size_t n,
							  const uint32_t rk[AES192_RK_WORDS]);

void aes256_dec_ecb_rv32zkned(uint8_t *pt, const uint8_t *ct, size_t n,
							  const uint32_t rk[AES256_RK_WORDS]);

#ifdef __cplusplus
}
#endif

#endif										//	_AES_RV32ZKNED_H_
