#pragma once

#if defined(SCCD_BACKEND_C25519)
	#include <c25519/src/ed25519.h>
#elif defined(SCCD_BACKEND_RELIC)
	#include <relic/relic.h>
#else
	#error No crypto backend defined. See README.
#endif

#if defined(SCCD_BACKEND_C25519)
	typedef struct ed25519_pt sccd_ec_t[1];
	typedef const struct ed25519_pt *sccd_ec_t_return;
#elif defined(SCCD_BACKEND_RELIC)
	typedef ec_t sccd_ec_t;
	typedef ed_st const *sccd_ec_t_return;
#endif

sccd_ec_t_return sccd_ec_get_base();
sccd_ec_t_return sccd_ec_get_neutral();

void sccd_ec_add(sccd_ec_t result, const sccd_ec_t a, const sccd_ec_t b);
void sccd_ec_double(sccd_ec_t result, const sccd_ec_t p);
void sccd_ec_sub(sccd_ec_t result, const sccd_ec_t a, const sccd_ec_t b);
void sccd_ec_mul(sccd_ec_t result, const sccd_ec_t p, const uint8_t* k);
void sccd_ec_random(sccd_ec_t result);
int sccd_ec_equal(const sccd_ec_t a, const sccd_ec_t b);
void sccd_ec_copy(sccd_ec_t result, const sccd_ec_t source);

size_t sccd_ec_bin_size(const sccd_ec_t a);
int sccd_ec_bin_write(const sccd_ec_t a, uint8_t* data, size_t dataSize);
int sccd_ec_bin_read(sccd_ec_t a, const uint8_t* data, size_t dataSize);

void sccd_ec_str_write(const sccd_ec_t a, char* str, size_t strSize);
