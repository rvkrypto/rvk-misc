//	aes_otf_rvk64.h
//	2020-05-06	Markku-Juhani O. Saarinen <mjos@iki.fi> See LICENSE.

//	=== AES Encryption with on-the-fly key expansion.
//	*rk can point to expanded key or just the key.

#ifndef _AES_OTF_RVK64_H_
#define _AES_OTF_RVK64_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void aes128_enc_otf_rvk64(uint8_t ct[16], const uint8_t pt[16],
						   const uint32_t * rk);

void aes192_enc_otf_rvk64(uint8_t ct[16], const uint8_t pt[16],
						   const uint32_t * rk);

void aes256_enc_otf_rvk64(uint8_t ct[16], const uint8_t pt[16],
						   const uint32_t * rk);

#ifdef __cplusplus
}
#endif

#endif	//	_AES_OTF_RVK64_H_