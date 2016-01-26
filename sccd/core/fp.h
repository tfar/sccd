#if defined(SCCD_BACKEND_C25519)
	#include <c25519/src/f25519.h>
#elif defined(SCCD_BACKEND_RELIC)
	#include <relic/relic.h>
#else
	#error No crypto backend defined. See README.
#endif


#if defined(SCCD_BACKEND_C25519)
	typedef uint8_t sccd_fp_t[F25519_SIZE];
#elif defined(SCCD_BACKEND_RELIC)
	typedef fp_t sccd_fp_t;
#endif

void sccd_fp_add(sccd_fp_t res, const sccd_fp_t a, const sccd_fp_t b);

void sccd_fp_sub(sccd_fp_t res, const sccd_fp_t a, const sccd_fp_t b);

void sccd_fp_neg(sccd_fp_t res, const sccd_fp_t a);

void sccd_fp_mul(sccd_fp_t res, const sccd_fp_t a, const sccd_fp_t b);

void sccd_fp_mul_dig(sccd_fp_t res, const sccd_fp_t a, uint8_t digit);

void sccd_fp_inv(sccd_fp_t res, const sccd_fp_t a);

void sccd_fp_normalize(sccd_fp_t res);

/**
 * Sets the parameter to a random field element.
 */
void sccd_fp_random(sccd_fp_t n);

int sccd_fp_is_valid(const sccd_fp_t n);

/**
 * Returns zero if unequal and non-zero if a and b are equal.
 */
int sccd_fp_equal(const sccd_fp_t a, const sccd_fp_t b);

void sccd_fp_copy(sccd_fp_t target, const sccd_fp_t source);
