#include <sccd/core/fp.h>
#include <sccd/core/util.h>

void sccd_fp_add(sccd_fp_t res, const sccd_fp_t a, const sccd_fp_t b) {
#if defined(SCCD_BACKEND_C25519)
	f25519_add(res, a, b);
#elif defined(SCCD_BACKEND_RELIC)
	fp_add(res, a, b);
#endif
}

void sccd_fp_sub(sccd_fp_t res, const sccd_fp_t a, const sccd_fp_t b) {
#if defined(SCCD_BACKEND_C25519)
	f25519_sub(res, a, b);
#elif defined(SCCD_BACKEND_RELIC)
	fp_sub(res, a, b);
#endif
}

void sccd_fp_mul(sccd_fp_t res, const sccd_fp_t a, const sccd_fp_t b) {
#if defined(SCCD_BACKEND_C25519)
	f25519_mul(res, a, b);
#elif defined(SCCD_BACKEND_RELIC)
	fp_mul(res, a, b);
#endif
}

void sccd_fp_mul_dig(sccd_fp_t res, const sccd_fp_t a, uint8_t digit) {
#if defined(SCCD_BACKEND_C25519)
	f25519_mul_c(res, a, digit);
#elif defined(SCCD_BACKEND_RELIC)
	fp_mul_dig(res, a, digit);
#endif
}

int sccd_fp_equal(const sccd_fp_t a, const sccd_fp_t b) {
#if defined(SCCD_BACKEND_C25519)
	return f25519_eq(a, b);
#elif defined(SCCD_BACKEND_RELIC)
	return fp_cmp(a, b) == CMP_EQ;
#endif
}

void sccd_fp_copy(sccd_fp_t target, const sccd_fp_t source) {
#if defined(SCCD_BACKEND_C25519)
	f25519_copy(target, source);
#elif defined(SCCD_BACKEND_RELIC)
	fp_copy(target, source);
#endif
}

void sccd_fp_inv(sccd_fp_t res, const sccd_fp_t a) {
#if defined(SCCD_BACKEND_C25519)
	f25519_inv(res, a);
#elif defined(SCCD_BACKEND_RELIC)
	fp_inv(res, a);
#endif
}

void sccd_fp_normalize(sccd_fp_t res) {
#if defined(SCCD_BACKEND_C25519)
	f25519_normalize(res);
#elif defined(SCCD_BACKEND_RELIC)
	fp_rdc(res, res);
#endif
}


int sccd_fp_is_valid(const sccd_fp_t n) {
#if defined(SCCD_BACKEND_C25519)
	sccd_fp_t inv;
	sccd_fp_t res;

	sccd_fp_copy(inv, n);
	sccd_fp_inv(res, inv);
	sccd_fp_inv(res, res);
	sccd_fp_normalize(inv);
	sccd_fp_normalize(res);
	return sccd_fp_equal(res, inv);
#elif defined(SCCD_BACKEND_RELIC)
	return fp_cmp(n, n) == CMP_EQ;
#endif
}

void sccd_fp_random(sccd_fp_t res) {
#if defined(SCCD_BACKEND_C25519)
	sccd_random_bytes(&res[0], F25519_SIZE);
	sccd_fp_normalize(res);
#elif defined(SCCD_BACKEND_RELIC)
	fp_rand(res);
#endif
}

void sccd_fp_neg(sccd_fp_t res, const sccd_fp_t a) {
#if defined(SCCD_BACKEND_C25519)
	f25519_neg(res, a);
#elif defined(SCCD_BACKEND_RELIC)
	fp_neg(res, a);
#endif
}
