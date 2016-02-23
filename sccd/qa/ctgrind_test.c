
#include <sccd/core/fp.h>
#include <sccd/core/ec.h>
#include <sccd/core/clock.h>
#include <sccd/core/util.h>
#include <sccd/ibc/vbnn_ibs.h>

#include "ctgrind.h"

char bad_memory_access(unsigned char *a) {
  static const char r[2] = {1, 0};
  return r[a[0] & 1];
}

int main(const int argc, const char* argv[]) {
	sccd_init();

	sccd_ec_t a, c;
	sccd_fp_t n;

	sccd_fp_random(n);
	sccd_ec_random(a);

	ct_poison(n, sizeof(n));

	sccd_ec_mul(c, a, n);

	return 0;
}
