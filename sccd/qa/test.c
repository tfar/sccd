#include <sccd/core/fp.h>
#include <sccd/core/ec.h>
#include <sccd/core/util.h>

#include <assert.h>
#include <stdio.h>


void test_fp_random() {
	printf("test fb random\n");
	for (int a = 0; a < 100; a++) {
		sccd_fp_t n;

		sccd_fp_random(n);

		assert(sccd_fp_is_valid(n));
	}
}

void test_fp_add() {
	printf("test fb add\n");
	for (int n = 0; n < 100; n++) {
		sccd_fp_t a;
		sccd_fp_t b;

		sccd_fp_t c;
		sccd_fp_t d;

		sccd_fp_random(a);
		sccd_fp_random(b);

		sccd_fp_add(c, a, b);
		sccd_fp_add(d, b, a);

		assert(sccd_fp_equal(c, d));
	}
}

void test_fb() {
	test_fp_random();
	test_fp_add();
}

void test_ec_random() {
	printf("test ec random\n");
	for (int a = 0; a < 100; a++) {
		sccd_ec_t n;

		sccd_ec_random(n);
	}
}

void test_ecc() {
	test_ec_random();
}

void test_vbnn_ibs() {

}

int main(int argc, const char **argv) {
	sccd_init();
	test_fb();
	test_ecc();

	return 0;
}
