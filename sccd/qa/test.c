#include <sccd/core/fp.h>
#include <sccd/core/ec.h>
#include <sccd/core/util.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef RIOT_BOARD
const int test_iterations = 3;
#else
const int test_iterations = 100;
#endif

void test_fp_random(void) {
	printf("test fb random\n");
	for (int a = 0; a < test_iterations; a++) {
		sccd_fp_t n;

		sccd_fp_random(n);
		assert(sccd_fp_is_valid(n));
	}
}

void test_fp_add(void) {
	printf("test fb add\n");
	for (int n = 0; n < test_iterations; n++) {
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

void test_fp_print(void) {
	printf("test_fp_print\n");
	char buffer[100];
	sccd_fp_t a;

	sccd_fp_random(a);
	sccd_fp_str_write(a, buffer, 100);
	printf("a = %s\n", buffer);
}

void test_fb(void) {
	test_fp_random();
	test_fp_add();
	test_fp_print();
}

void test_ec_random(void) {
	printf("test_ec_random\n");
	sccd_ec_t last_n;
	for (int a = 0; a < test_iterations; a++) {
		sccd_ec_t n;
		char buffer[300];

		sccd_ec_random(n);

		if (a) {
			assert(sccd_ec_equal(n, last_n) == 0);
		}
		sccd_ec_copy(last_n, n);

		sccd_ec_str_write(n, buffer, 300);
		printf("n = %s\n", buffer);
	}
}

void test_ec_serialize(void) {
	printf("test_ec_serialize\n");
	for (int a = 0; a < test_iterations; a++) {
		sccd_ec_t n;
		sccd_ec_t m;
		sccd_ec_random(n);

		size_t binSize = sccd_ec_bin_size(n);

		uint8_t* data = malloc(binSize);
		assert(data);

		assert(sccd_ec_bin_write(n, data, binSize) == 0);
		assert(sccd_ec_bin_read(m, data, binSize) == 0);
		assert(sccd_ec_equal(n, m));

		free(data);
	}
}

void test_ec_print(void) {
	printf("test_ec_print\n");
	char buffer[300];
	sccd_ec_t a;

	sccd_ec_random(a);
	sccd_ec_str_write(a, buffer, 300);
	printf("a = %s\n", buffer);
}

void test_ecc(void) {
	test_ec_random();
	test_ec_serialize();
	test_ec_print();
}

void test_vbnn_ibs(void) {

}

void test(void) {
	sccd_init();
	test_fb();
	test_ecc();
	test_vbnn_ibs();

	printf("done\n");
}

#ifndef RIOT_BOARD
int main(int argc, const char **argv) {
	test();
	return 0;
}
#endif
