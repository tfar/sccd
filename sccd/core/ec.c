#include <sccd/core/ec.h>
#include <sccd/core/fp.h>

const sccd_ec_t* sccd_ec_get_base() {
	return &ed25519_base;
}

const sccd_ec_t* sccd_ec_get_neutral() {
	return &ed25519_neutral;
}

void sccd_ec_add(sccd_ec_t* result, const sccd_ec_t* a, const sccd_ec_t* b) {
	ed25519_add(result, a, b);
}

void sccd_ec_double(sccd_ec_t* result, const sccd_ec_t* p) {
	ed25519_double(result, p);
}

void sccd_ec_sub(sccd_ec_t* result, const sccd_ec_t* a, const sccd_ec_t* b) {
	sccd_ec_t tmp;
	sccd_fp_neg(tmp.x, b->x);
	sccd_fp_neg(tmp.t, b->t);
	sccd_fp_copy(tmp.y, b->y);
	sccd_fp_copy(tmp.z, b->z);
	sccd_ec_add(result, a, &tmp);
}

void sccd_ec_mul(sccd_ec_t* result, const sccd_ec_t* a, const uint8_t* k) {
	ed25519_smult(result, a, k);
}

void sccd_ec_random(sccd_ec_t* result) {
	sccd_fp_t n;

	sccd_fp_random(n);

	sccd_ec_copy(result, sccd_ec_get_base());
	sccd_ec_mul(result, result, n);
}

void sccd_ec_copy(sccd_ec_t* result, const sccd_ec_t* source) {
	memcpy(result, source, sizeof(*result));
}

int sccd_ec_equal(const sccd_ec_t* a, const sccd_ec_t* b) {
	int x_eq = f25519_eq(a->x, b->x);
	int y_eq = f25519_eq(a->y, b->y);
	int t_eq = f25519_eq(a->t, b->t);
	int z_eq = f25519_eq(a->z, b->z);
	int res = (x_eq && y_eq && t_eq && z_eq);
	return res;
}
