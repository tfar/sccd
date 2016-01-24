#include <sccd/ibc/vbnn_ibs.h>

#include <blake2.h>

void sccd_vbnn_ibs_ta_init(const sccd_fp_t random, sccd_vbnn_ibs_ta_t* ta) {
	// private key generation: generate x \in Z_p (TA private key)
	// random generation externalized for testing
	sccd_fp_copy(ta->private, random);

	// public key generation: calculate P_0 = x * P
	sccd_ec_mul(&ta->public, sccd_ec_get_base(), ta->private);
}

void sccd_vbnn_ibs_ta_extract_key(const sccd_fp_t random, const sccd_vbnn_ibs_ta_t* ta, const uint8_t* id, const size_t id_len, sccd_vbnn_ibs_user_t *extracted_user_key) {
	// generate random \in Z_p
	// random generation externalized for testing

	// compute R = random * P
	sccd_ec_mul(&extracted_user_key->R, sccd_ec_get_base(), random);

	// using the master secret key x, calculate:
	// c = H1(ID, R)
	sccd_fp_t c;

	blake2s_state S[1];
	blake2s_init(S, sizeof(c));
	blake2s_update(S, (const uint8_t*)id, id_len);
	blake2s_update(S, (const uint8_t*)&extracted_user_key->R, sizeof(sccd_ec_t));
	blake2s_final(S, c, sizeof(c));
	sccd_fp_normalize(c);

	// s = random + cx
	sccd_fp_mul(c, c, ta->private);
	sccd_fp_add(extracted_user_key->s, random, c);
}

void sccd_vbnn_ibs_sign(
	const sccd_fp_t random,
	const sccd_ec_t* ta_public,
	const sccd_vbnn_ibs_user_t* user_key,
	const uint8_t* id, const size_t id_len,
	const uint8_t* msg, const size_t msg_len,
	sccd_vbnn_ibs_sig_t* signature) {

	// generate random y \in Zp and compute Y = yP .
	// random generation externalized for testing.
	sccd_fp_t y;
	sccd_ec_t Y;
	sccd_fp_copy(y, random);
	sccd_ec_mul(&Y, sccd_ec_get_base(), y);

	// h = H2(ID, m, R, Y )
	blake2s_state S[1];
	blake2s_init(S, sizeof(signature->h));
	blake2s_update(S, (const uint8_t*)id, id_len);
	blake2s_update(S, (const uint8_t*)msg, msg_len);
	blake2s_update(S, (const uint8_t*)id, id_len);
	blake2s_update(S, (const uint8_t*)&Y, sizeof(Y));
	blake2s_final(S, signature->h, sizeof(signature->h));
	sccd_fp_normalize(signature->h);

	// z = y + hs
	sccd_fp_mul(signature->z, signature->h, user_key->s);
	sccd_fp_add(signature->z, y, signature->z);

	sccd_ec_copy(&signature->R, &user_key->R);
}

int sccd_vbnn_ibs_verify(const sccd_ec_t* ta_public, const uint8_t* id, const size_t id_len, const uint8_t* msg, const size_t msg_len, const sccd_vbnn_ibs_sig_t* signature) {
	int valid = 0;


	// 1. Compute the following:
	// c = H1(ID, R)
	sccd_fp_t c;

	blake2s_state S[1];
	blake2s_init(S, sizeof(c));
	blake2s_update(S, (const uint8_t*)id, id_len);
	blake2s_update(S, (const uint8_t*)&signature->R, sizeof(signature->R));
	blake2s_final(S, c, sizeof(c));
	sccd_fp_normalize(c);

	// T = zP − h(R + cP0)
	sccd_ec_t T;
	sccd_ec_t tmp;
	sccd_ec_mul(&T, sccd_ec_get_base(), signature->z);
	sccd_ec_mul(&tmp, ta_public, c);
	sccd_ec_add(&tmp, &tmp, &signature->R);
	sccd_ec_mul(&tmp, &tmp, signature->h);
	sccd_ec_sub(&T, &T, &tmp);

	// 2. To verify the signature, check whether the following equation holds:
	// h ?=?  H2(ID,m,R,T)
	sccd_fp_t h_rhs;

	blake2s_init(S, sizeof(h_rhs));
	blake2s_update(S, (const uint8_t*)id, id_len);
	blake2s_update(S, (const uint8_t*)msg, msg_len);
	blake2s_update(S, (const uint8_t*)&signature->R, sizeof(signature->R));
	blake2s_update(S, (const uint8_t*)&T, sizeof(T));
	blake2s_final(S, h_rhs, sizeof(h_rhs));
	sccd_fp_normalize(h_rhs);

	valid = sccd_fp_equal(signature->h, h_rhs);

	return valid;
}
