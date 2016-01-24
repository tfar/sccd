#include <sccd/core/fp.h>
#include <sccd/core/util.h>

void sccd_fp_add(sccd_fp_t res, const sccd_fp_t a, const sccd_fp_t b) {
	f25519_add(res, a, b);
}

void sccd_fp_sub(sccd_fp_t res, const sccd_fp_t a, const sccd_fp_t b) {
	f25519_sub(res, a, b);
}

void sccd_fp_mul(sccd_fp_t res, const sccd_fp_t a, const sccd_fp_t b) {
	f25519_mul(res, a, b);
}

void sccd_fp_mul_dig(sccd_fp_t res, const sccd_fp_t a, uint8_t digit) {
	f25519_mul_c(res, a, digit);
}

int sccd_fp_equal(const sccd_fp_t a, const sccd_fp_t b) {
	return f25519_eq(a, b);
}

void sccd_fp_copy(sccd_fp_t target, const sccd_fp_t source) {
	f25519_copy(target, source);
}

void sccd_fp_inv(sccd_fp_t res, const sccd_fp_t a) {
	f25519_inv(res, a);
}

void sccd_fp_normalize(sccd_fp_t res) {
	f25519_normalize(res);
}


int sccd_fp_is_valid(const sccd_fp_t n) {
	sccd_fp_t inv;
	sccd_fp_t res;

	sccd_fp_copy(inv, n);
	sccd_fp_inv(res, inv);
	sccd_fp_inv(res, res);
	sccd_fp_normalize(inv);
	sccd_fp_normalize(res);
	return sccd_fp_equal(res, inv);
}

void sccd_fp_random(sccd_fp_t res) {
	sccd_random_bytes(&res[0], F25519_SIZE);
	sccd_fp_normalize(res);
}

void sccd_fp_neg(sccd_fp_t res, const sccd_fp_t a) {
	f25519_neg(res, a);
}
