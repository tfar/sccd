#include <sccd/core/ec.h>
#include <sccd/core/fp.h>

sccd_ec_t_return sccd_ec_get_base() {
#if defined(SCCD_BACKEND_C25519)
	return &ed25519_base;
#elif defined(SCCD_BACKEND_RELIC)
	static sccd_ec_t base_point;
	ec_curve_get_gen(base_point);
	return (sccd_ec_t_return)&base_point;
#endif
}

sccd_ec_t_return sccd_ec_get_neutral() {
#if defined(SCCD_BACKEND_C25519)
	return &ed25519_neutral;
#elif defined(SCCD_BACKEND_RELIC)
	static sccd_ec_t neutral;
	ec_set_infty(neutral);
	return (sccd_ec_t_return)&neutral;
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
	uint8_t a_x[F25519_SIZE];
	uint8_t a_y[F25519_SIZE];
	uint8_t b_x[F25519_SIZE];
	uint8_t b_y[F25519_SIZE];

	/* Convert points to affine form. */
	ed25519_unproject(a_x, a_y, a);
	ed25519_unproject(b_x, b_y, b);

	/* Compare affine points. */
	int x_eq = f25519_eq(a_x, b_x);
	int y_eq = f25519_eq(a_y, b_y);
	int res = (x_eq && y_eq);
	return res;
#elif defined(SCCD_BACKEND_RELIC)
	sccd_ec_t aNorm;
	sccd_ec_t bNorm;
	ec_norm(aNorm, a);
	ec_norm(bNorm, b);
	return ec_cmp(aNorm, bNorm) == CMP_EQ ? 1 : 0;
#endif
}

size_t sccd_ec_bin_size(const sccd_ec_t a) {
	size_t binSize = -1;
#if defined(SCCD_BACKEND_C25519)
	binSize = ED25519_PACK_SIZE;
#elif defined(SCCD_BACKEND_RELIC)
	binSize = ec_size_bin(a, 0);
#endif
	return binSize;
}

int sccd_ec_bin_write(const sccd_ec_t a, uint8_t* data, size_t dataSize) {
	int failure = 1;

	#if defined(SCCD_BACKEND_C25519)
	/* Check if provided buffer is large enough. */
	if (dataSize >= ED25519_PACK_SIZE) {
		uint8_t x[F25519_SIZE];
		uint8_t y[F25519_SIZE];

		/* Convert point to affine form. */
		ed25519_unproject(x, y, a);

		/* Pack affine point into data buffer. */
		ed25519_pack(data, x, y);
		failure = 0;
	}
	#elif defined(SCCD_BACKEND_RELIC)
		ec_write_bin(data, dataSize, a, 0);
		failure = 0;
	#endif

	return failure;
}

int sccd_ec_bin_read(sccd_ec_t a, const uint8_t* data, size_t dataSize) {
	int failure = 1;
	#if defined(SCCD_BACKEND_C25519)
	/* Check if the provided buffer is of correct size. */
	if (dataSize == ED25519_PACK_SIZE) {
		/* Try to decompress data into a valid point. */
		if (ed25519_try_unpack(a->x, a->y, data) == 1) {
			/* Convert affine point to projective form. */
			ed25519_project(a, a->x, a->y);

			failure = 0;
		}
	}
	#elif defined(SCCD_BACKEND_RELIC)
		ec_read_bin(a, data, dataSize);
		failure = 0;
	#endif
	return failure;
}
