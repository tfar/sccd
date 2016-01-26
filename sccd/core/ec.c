#include <sccd/core/ec.h>
#include <sccd/core/fp.h>

sccd_ec_t_return sccd_ec_get_base() {
#if defined(SCCD_BACKEND_C25519)
	return &ed25519_base;
#elif defined(SCCD_BACKEND_RELIC)
	static sccd_ec_t base_point;
	ec_curve_get_gen(base_point);
	return &base_point;
#endif
}

sccd_ec_t_return sccd_ec_get_neutral() {
#if defined(SCCD_BACKEND_C25519)
	return &ed25519_neutral;
#elif defined(SCCD_BACKEND_RELIC)
	static sccd_ec_t neutral;
	ec_set_infty(neutral);
	return &neutral;
#endif
}

void sccd_ec_add(sccd_ec_t result, const sccd_ec_t a, const sccd_ec_t b) {
#if defined(SCCD_BACKEND_C25519)
	ed25519_add(result, a, b);
#elif defined(SCCD_BACKEND_RELIC)
	ec_add(result, a, b);
#endif
}

void sccd_ec_double(sccd_ec_t result, const sccd_ec_t p) {
#if defined(SCCD_BACKEND_C25519)
	ed25519_double(result, p);
#elif defined(SCCD_BACKEND_RELIC)

#endif
}

void sccd_ec_sub(sccd_ec_t result, const sccd_ec_t a, const sccd_ec_t b) {
#if defined(SCCD_BACKEND_C25519)
	sccd_ec_t tmp;
	sccd_fp_neg(tmp->x, b->x);
	sccd_fp_neg(tmp->t, b->t);
	sccd_fp_copy(tmp->y, b->y);
	sccd_fp_copy(tmp->z, b->z);
	sccd_ec_add(result, a, tmp);
#elif defined(SCCD_BACKEND_RELIC)
	ec_sub(result, a, b);
#endif
}

void sccd_ec_mul(sccd_ec_t result, const sccd_ec_t a, const uint8_t* k) {
#if defined(SCCD_BACKEND_C25519)
	ed25519_smult(result, a, k);
#elif defined(SCCD_BACKEND_RELIC)
	ec_mul_dig(result, a, *((dig_t*)k));
#endif
}

void sccd_ec_random(sccd_ec_t result) {
#if defined(SCCD_BACKEND_C25519)
	sccd_fp_t n;

	sccd_fp_random(n);

	sccd_ec_copy(result, sccd_ec_get_base());
	sccd_ec_mul(result, result, n);
#elif defined(SCCD_BACKEND_RELIC)
	ec_rand(result);
#endif
}

void sccd_ec_copy(sccd_ec_t result, const sccd_ec_t source) {
#if defined(SCCD_BACKEND_C25519)
	memcpy(result, source, sizeof(*result));
#elif defined(SCCD_BACKEND_RELIC)
	ec_copy(result, source);
#endif
}

int sccd_ec_equal(const sccd_ec_t a, const sccd_ec_t b) {
#if defined(SCCD_BACKEND_C25519)
	int x_eq = f25519_eq(a->x, b->x);
	int y_eq = f25519_eq(a->y, b->y);
	int t_eq = f25519_eq(a->t, b->t);
	int z_eq = f25519_eq(a->z, b->z);
	int res = (x_eq && y_eq && t_eq && z_eq);
	return res;
#elif defined(SCCD_BACKEND_RELIC)
	return ec_cmp(a, b);
#endif
}
