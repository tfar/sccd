#include <sccd/core/fp.h>
#include <sccd/core/ec.h>
#include <sccd/core/util.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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

void test_fp_print() {
	printf("test_fp_print\n");
	char buffer[100];
	sccd_fp_t a;

	sccd_fp_random(a);
	sccd_fp_str_write(a, buffer, 100);
	printf("a = %s\n", buffer);
}

void test_fb() {
	test_fp_random();
	test_fp_add();
	test_fp_print();
}

void test_ec_random() {
	printf("test ec random\n");
	for (int a = 0; a < 100; a++) {
		sccd_ec_t n;

		sccd_ec_random(n);
	}
}

void test_ec_serialize() {
	for (int a = 0; a < 100; a++) {
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

void test_ec_print() {
	printf("test_ec_print\n");
	char buffer[300];
	sccd_ec_t a;

	sccd_ec_random(a);
	sccd_ec_str_write(a, buffer, 300);
	printf("a = %s\n", buffer);
}

void test_ecc() {
	test_ec_random();
	test_ec_serialize();
	test_ec_print();
}

void test_vbnn_ibs() {

}

int main(int argc, const char **argv) {
	sccd_init();
	test_fb();
	test_ecc();

	return 0;
}
