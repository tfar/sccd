#include <c25519/src/ed25519.h>

typedef struct ed25519_pt sccd_ec_t;

const sccd_ec_t* sccd_ec_get_base();
const sccd_ec_t* sccd_ec_get_neutral();

void sccd_ec_add(sccd_ec_t* result, const sccd_ec_t* a, const sccd_ec_t* b);
void sccd_ec_double(sccd_ec_t* result, const sccd_ec_t* p);
void sccd_ec_sub(sccd_ec_t* result, const sccd_ec_t* a, const sccd_ec_t* b);
void sccd_ec_mul(sccd_ec_t* result, const sccd_ec_t* p, const uint8_t* k);
void sccd_ec_random(sccd_ec_t* result);
int sccd_ec_equal(const sccd_ec_t* a, const sccd_ec_t* b);
void sccd_ec_copy(sccd_ec_t* result, const sccd_ec_t* source);
