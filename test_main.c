//	test_main.c
//	2024-07-17	Markku-Juhani O. Saarinen <mjos@iki.fi> See LICENSE.

#include <stdio.h>
#include <stdlib.h>

//	prototypes here
int test_aes();
int test_gcm();
int test_sha2();
int test_sha3();
int test_sm4();
int test_sm3();
int test_present();

int main()
{
	int fail = 0;

	fail += test_aes();
	fail += test_gcm();
	fail += test_sha2();
	fail += test_sha3();
	fail += test_present();
	fail += test_sm4();
	fail += test_sm3();

	printf("[%s] === TOTAL %d ERRORS.\n",
		fail == 0 ? "PASS" : "FAIL", fail);

	return fail;
}
