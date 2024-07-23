//	sha3_f1600_rvb64.c
//	2020-03-05	Markku-Juhani O. Saarinen <mjos@iki.fi> See LICENSE.

//	=== FIPS 202 Keccak permutation implementation for a 64-bit target.

#include "plat_local.h"
#ifdef PLAT_ARCH_RV64
#include <riscv_bitmanip.h>

//	Keccak-p[1600,24](S) = Keccak-f1600(S)

void sha3_f1600_rvb64(void *s)
{
	//	round constants
	const uint64_t rc[24] = {
		0x0000000000000001LLU, 0x0000000000008082LLU, 0x800000000000808ALLU,
		0x8000000080008000LLU, 0x000000000000808BLLU, 0x0000000080000001LLU,
		0x8000000080008081LLU, 0x8000000000008009LLU, 0x000000000000008ALLU,
		0x0000000000000088LLU, 0x0000000080008009LLU, 0x000000008000000ALLU,
		0x000000008000808BLLU, 0x800000000000008BLLU, 0x8000000000008089LLU,
		0x8000000000008003LLU, 0x8000000000008002LLU, 0x8000000000000080LLU,
		0x000000000000800ALLU, 0x800000008000000ALLU, 0x8000000080008081LLU,
		0x8000000000008080LLU, 0x0000000080000001LLU, 0x8000000080008008LL
	};

	int i;
	uint64_t t, u, v, w;
	uint64_t sa, sb, sc, sd, se, sf, sg, sh, si, sj, sk, sl, sm,
		sn, so, sp, sq, sr, ss, st, su, sv, sw, sx, sy;

	//	load state, little endian, aligned

	uint64_t *vs = (uint64_t *) s;

	sa = vs[0];
	sb = vs[1];
	sc = vs[2];
	sd = vs[3];
	se = vs[4];
	sf = vs[5];
	sg = vs[6];
	sh = vs[7];
	si = vs[8];
	sj = vs[9];
	sk = vs[10];
	sl = vs[11];
	sm = vs[12];
	sn = vs[13];
	so = vs[14];
	sp = vs[15];
	sq = vs[16];
	sr = vs[17];
	ss = vs[18];
	st = vs[19];
	su = vs[20];
	sv = vs[21];
	sw = vs[22];
	sx = vs[23];
	sy = vs[24];

	//	iteration

	for (i = 0; i < 24; i++) {

		//	Theta

		u = sa ^ sf ^ sk ^ sp ^ su;
		v = sb ^ sg ^ sl ^ sq ^ sv;
		w = se ^ sj ^ so ^ st ^ sy;
		t = w ^ __riscv_ror_64(v, 63);
		sa = sa ^ t;
		sf = sf ^ t;
		sk = sk ^ t;
		sp = sp ^ t;
		su = su ^ t;

		t = sd ^ si ^ sn ^ ss ^ sx;
		v = v ^ __riscv_ror_64(t, 63);
		t = t ^ __riscv_ror_64(u, 63);
		se = se ^ t;
		sj = sj ^ t;
		so = so ^ t;
		st = st ^ t;
		sy = sy ^ t;

		t = sc ^ sh ^ sm ^ sr ^ sw;
		u = u ^ __riscv_ror_64(t, 63);
		t = t ^ __riscv_ror_64(w, 63);
		sc = sc ^ v;
		sh = sh ^ v;
		sm = sm ^ v;
		sr = sr ^ v;
		sw = sw ^ v;

		sb = sb ^ u;
		sg = sg ^ u;
		sl = sl ^ u;
		sq = sq ^ u;
		sv = sv ^ u;

		sd = sd ^ t;
		si = si ^ t;
		sn = sn ^ t;
		ss = ss ^ t;
		sx = sx ^ t;

		//	Rho Pi

		t  = __riscv_ror_64(sb, 63);
		sb = __riscv_ror_64(sg, 20);
		sg = __riscv_ror_64(sj, 44);
		sj = __riscv_ror_64(sw, 3);
		sw = __riscv_ror_64(so, 25);
		so = __riscv_ror_64(su, 46);
		su = __riscv_ror_64(sc, 2);
		sc = __riscv_ror_64(sm, 21);
		sm = __riscv_ror_64(sn, 39);
		sn = __riscv_ror_64(st, 56);
		st = __riscv_ror_64(sx, 8);
		sx = __riscv_ror_64(sp, 23);
		sp = __riscv_ror_64(se, 37);
		se = __riscv_ror_64(sy, 50);
		sy = __riscv_ror_64(sv, 62);
		sv = __riscv_ror_64(si, 9);
		si = __riscv_ror_64(sq, 19);
		sq = __riscv_ror_64(sf, 28);
		sf = __riscv_ror_64(sd, 36);
		sd = __riscv_ror_64(ss, 43);
		ss = __riscv_ror_64(sr, 49);
		sr = __riscv_ror_64(sl, 54);
		sl = __riscv_ror_64(sh, 58);
		sh = __riscv_ror_64(sk, 61);
		sk = t;

		//	Chi

		t = (se &~ sd);
		se = se ^ (sb &~ sa);
		sb = sb ^ (sd &~ sc);
		sd = sd ^ (sa &~ se);
		sa = sa ^ (sc &~ sb);
		sc = sc ^ t;

		t = (sj &~ si);
		sj = sj ^ (sg &~ sf);
		sg = sg ^ (si &~ sh);
		si = si ^ (sf &~ sj);
		sf = sf ^ (sh &~ sg);
		sh = sh ^ t;

		t = (so &~ sn);
		so = so ^ (sl &~ sk);
		sl = sl ^ (sn &~ sm);
		sn = sn ^ (sk &~ so);
		sk = sk ^ (sm &~ sl);
		sm = sm ^ t;

		t = (st &~ ss);
		st = st ^ (sq &~ sp);
		sq = sq ^ (ss &~ sr);
		ss = ss ^ (sp &~ st);
		sp = sp ^ (sr &~ sq);
		sr = sr ^ t;

		t = (sy &~ sx);
		sy = sy ^ (sv &~ su);
		sv = sv ^ (sx &~ sw);
		sx = sx ^ (su &~ sy);
		su = su ^ (sw &~ sv);
		sw = sw ^ t;

		//	Iota

		sa = sa ^ rc[i];
	}

	//	store state

	vs[0] = sa;
	vs[1] = sb;
	vs[2] = sc;
	vs[3] = sd;
	vs[4] = se;
	vs[5] = sf;
	vs[6] = sg;
	vs[7] = sh;
	vs[8] = si;
	vs[9] = sj;
	vs[10] = sk;
	vs[11] = sl;
	vs[12] = sm;
	vs[13] = sn;
	vs[14] = so;
	vs[15] = sp;
	vs[16] = sq;
	vs[17] = sr;
	vs[18] = ss;
	vs[19] = st;
	vs[20] = su;
	vs[21] = sv;
	vs[22] = sw;
	vs[23] = sx;
	vs[24] = sy;
}

#endif
