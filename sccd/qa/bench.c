#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <sccd/core/fp.h>
#include <sccd/core/ec.h>
#include <sccd/core/clock.h>
#include <sccd/core/util.h>
#include <sccd/ibc/vbnn_ibs.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

#ifdef RIOT_BOARD
static uint32_t iterations = 10;
static uint32_t bench_iterations = 4;
#else
static uint32_t iterations = 10;
static uint32_t bench_iterations = 15;
#endif

static uint64_t min(const uint64_t a, const uint64_t b) {
	return a < b ? a : b;
}


static char bfr[20+1];

void u64toa_naive(uint64_t value, char* buffer) {
    char temp[20];
    char *p = temp;
    do {
        *p++ = ((char)(value % 10)) + '0';
        value /= 10;
    } while (value > 0);

    do {
        *buffer++ = *--p;
    } while (p != temp);

    *buffer = '\0';
}

static char* uint64ToDecimal(uint64_t v) {
	u64toa_naive(v, bfr);
  	return bfr;
}

static void print_bench_timing(const char* name, uint64_t cycles) {
#ifdef RIOT_BOARD
	printf("bench(%s): %s %s\n", name, uint64ToDecimal(cycles), sccd_cylces_unit());
#else
	printf("bench(%s): %llu %s\n", name, cycles, sccd_cylces_unit());
#endif
}

#define SCCD_BENCH_KERNEL(NAME, SCCD_BENCH) 									\
	uint64_t bench_times[bench_iterations]; 									\
	for (int q = 0; q < bench_iterations; q++) {								\
		uint64_t start = sccd_cycles(); 										\
		SCCD_BENCH 																\
		uint64_t end = sccd_cycles(); 											\
		end = (end - start); 													\
		if (end > LLONG_MAX) { 													\
			q = q - 1; 															\
		} else {																\
			bench_times[q] = end;												\
		}																		\
	} 																			\
	cycles_table[z] = bench_times[0]; 											\
	for (int q = 1; q < bench_iterations; q++) {								\
		cycles_table[z] = min(cycles_table[z], bench_times[q]); 				\
	}


#define SCCD_BENCH_RESULTS(NAME) \
	for (int z = 0; z < iterations; z++) { \
		print_bench_timing(NAME, cycles_table[z]); \
	}

void bench_noop(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		SCCD_BENCH_KERNEL("nop",
			__asm__ volatile("nop");
		)
	}
	SCCD_BENCH_RESULTS("nop")
}

void bench_cycles(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		uint64_t tmp;
		SCCD_BENCH_KERNEL("cycles",
			tmp = sccd_cycles();
		)
	}
	SCCD_BENCH_RESULTS("cycles")
}

void bench_fp_add(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_fp_t a, b, c;

		sccd_fp_random(a);
		sccd_fp_random(b);

		SCCD_BENCH_KERNEL("fp_add",
			sccd_fp_add(c, a, b);
		)
	}
	SCCD_BENCH_RESULTS("fp_add")
}

void bench_fp_sub(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_fp_t a, b, c;

		sccd_fp_random(a);
		sccd_fp_random(b);

		SCCD_BENCH_KERNEL("fp_sub",
			sccd_fp_sub(c, a, b);
		)
	}
	SCCD_BENCH_RESULTS("fp_sub")
}

void bench_fp_mul(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_fp_t a, b, c;

		sccd_fp_random(a);
		sccd_fp_random(b);

		SCCD_BENCH_KERNEL("fp_mul",
			sccd_fp_mul(c, a, b);
		)
	}
	SCCD_BENCH_RESULTS("fp_mul")
}

void bench_fp_inv(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_fp_t a, c;

		sccd_fp_random(a);

		SCCD_BENCH_KERNEL("fp_inv",
			sccd_fp_inv(c, a);
		)
	}
	SCCD_BENCH_RESULTS("fp_inv")
}

void bench_fp_equal(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_fp_t a, b;

		sccd_fp_random(a);
		sccd_fp_random(b);

		SCCD_BENCH_KERNEL("fp_equal",
			sccd_fp_equal(a, b);
		)
	}
	SCCD_BENCH_RESULTS("fp_equal")
}


void bench_fp_copy(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_fp_t a, b;

		sccd_fp_random(a);

		SCCD_BENCH_KERNEL("fp_copy",
			sccd_fp_copy(a, b);
		)
	}
	SCCD_BENCH_RESULTS("fp_copy")
}

void bench_fp_random(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_fp_t a;

		SCCD_BENCH_KERNEL("fp_random",
			sccd_fp_random(a);
		)
	}
	SCCD_BENCH_RESULTS("fp_random")
}

void bench_fp(void) {
	bench_fp_add();
	bench_fp_sub();
	bench_fp_mul();
	bench_fp_inv();
	bench_fp_equal();
	bench_fp_copy();
	bench_fp_random();
}

void bench_ec_add(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_ec_t a, b, c;

		sccd_ec_random(a);
		sccd_ec_random(b);

		SCCD_BENCH_KERNEL("ec_add",
			sccd_ec_add(c, a, b);
		)
	}
	SCCD_BENCH_RESULTS("ec_add")
}

void bench_ec_sub(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_ec_t a, b, c;

		sccd_ec_random(a);
		sccd_ec_random(b);

		SCCD_BENCH_KERNEL("ec_sub",
			sccd_ec_sub(c, a, b);
		)
	}
	SCCD_BENCH_RESULTS("ec_sub")
}

void bench_ec_double(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_ec_t a, c;

		sccd_ec_random(a);

		SCCD_BENCH_KERNEL("ec_double",
			sccd_ec_double(c, a);
		)
	}
	SCCD_BENCH_RESULTS("ec_double")
}

void bench_ec_mul(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_ec_t a, c;
		sccd_fp_t n;

		sccd_fp_random(n);
		sccd_ec_random(a);

		SCCD_BENCH_KERNEL("ec_mul",
			sccd_ec_mul(c, a, n);
		)
	}
	SCCD_BENCH_RESULTS("ec_mul")
}

void bench_ec_equal(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_ec_t a, b;

		sccd_ec_random(a);
		sccd_ec_random(b);

		SCCD_BENCH_KERNEL("ec_equal",
			sccd_ec_equal(a, b);
		)
	}
	SCCD_BENCH_RESULTS("ec_equal")
}

void bench_ec_copy(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_ec_t a, c;

		sccd_ec_random(a);

		SCCD_BENCH_KERNEL("ec_copy",
			sccd_ec_copy(c, a);
		)
	}
	SCCD_BENCH_RESULTS("ec_copy")
}

void bench_ec_bin_write(void) {
	uint64_t cycles_table[iterations];

	sccd_ec_t a;
	size_t binSize;
	uint8_t* bin;
	binSize = sccd_ec_bin_size(a);
	bin = malloc(binSize);

	for (int z = 0; z < iterations; z++) {
		sccd_ec_random(a);

		SCCD_BENCH_KERNEL("ec_bin_write",
			sccd_ec_bin_write(a, bin, binSize);
		)
	}
	free(bin);
	SCCD_BENCH_RESULTS("ec_bin_write")
}

void bench_ec_bin_read(void) {
	uint64_t cycles_table[iterations];

	sccd_ec_t a, b;
	size_t binSize;
	uint8_t* bin;
	binSize = sccd_ec_bin_size(a);
	bin = malloc(binSize);

	for (int z = 0; z < iterations; z++) {
		sccd_ec_random(a);

		sccd_ec_bin_write(a, bin, binSize);

		SCCD_BENCH_KERNEL("ec_bin_read",
			sccd_ec_bin_read(b, bin, binSize);
		)
	}
	free(bin);
	SCCD_BENCH_RESULTS("ec_bin_read")
}

void bench_ec_random(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_ec_t a;

		sccd_ec_random(a);

		SCCD_BENCH_KERNEL("ec_random",
			sccd_ec_random(a);
		)
	}
	SCCD_BENCH_RESULTS("ec_random")
}

void bench_ecc(void) {
	bench_ec_add();
	bench_ec_sub();
	bench_ec_double();
	bench_ec_mul();
	bench_ec_equal();
	bench_ec_copy();
	bench_ec_bin_write();
	bench_ec_bin_read();
	bench_ec_random();
}

void bench_vbnn_ibs_ta_init(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_fp_t random;
		sccd_fp_random(random);

		sccd_vbnn_ibs_ta_t ta;

		SCCD_BENCH_KERNEL("vbnn_ibs_ta_init",
			sccd_vbnn_ibs_ta_init(random, &ta);
		)
	}
	SCCD_BENCH_RESULTS("vbnn_ibs_ta_init")
}

void bench_vbnn_ibs_ta_extract_key(void) {
	uint64_t cycles_table[iterations];

	for (int z = 0; z < iterations; z++) {
		sccd_fp_t random;
		sccd_fp_random(random);

		sccd_vbnn_ibs_ta_t ta;
		sccd_vbnn_ibs_ta_init(random, &ta);

		uint8_t id[32];
		sccd_random_bytes(id, 32);

		sccd_fp_random(random);

		sccd_vbnn_ibs_user_t user;

		SCCD_BENCH_KERNEL("vbnn_ibs_ta_extract_key",
			sccd_vbnn_ibs_ta_extract_key(random, &ta, id, sizeof(id), &user);
		)
	}
	SCCD_BENCH_RESULTS("vbnn_ibs_ta_extract_key")
}

void bench_vbnn_ibs_sign(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_fp_t random;
		sccd_fp_random(random);

		sccd_vbnn_ibs_ta_t ta;
		sccd_vbnn_ibs_ta_init(random, &ta);

		uint8_t id[32];
		sccd_random_bytes(id, sizeof(id));

		sccd_fp_random(random);

		sccd_vbnn_ibs_user_t user;
		sccd_vbnn_ibs_ta_extract_key(random, &ta, id, sizeof(id), &user);

		uint8_t message[256];
		sccd_random_bytes(message, sizeof(message));

		sccd_fp_random(random);

		sccd_vbnn_ibs_sig_t signature;

		SCCD_BENCH_KERNEL("vbnn_ibs_sign",
			sccd_vbnn_ibs_sign(random, &user, id, sizeof(id), message, sizeof(message), &signature);
		)
	}
	SCCD_BENCH_RESULTS("vbnn_ibs_sign")
}

void bench_vbnn_ibs_verify(void) {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		sccd_fp_t random;
		sccd_fp_random(random);

		sccd_vbnn_ibs_ta_t ta;
		sccd_vbnn_ibs_ta_init(random, &ta);

		uint8_t id[32];
		sccd_random_bytes(id, sizeof(id));

		sccd_fp_random(random);

		sccd_vbnn_ibs_user_t user;
		sccd_vbnn_ibs_ta_extract_key(random, &ta, id, sizeof(id), &user);

		uint8_t message[256];
		sccd_random_bytes(message, sizeof(message));

		sccd_fp_random(random);

		sccd_vbnn_ibs_sig_t signature;
		sccd_vbnn_ibs_sign(random, &user, id, sizeof(id), message, sizeof(message), &signature);

		int result;
		if (z % 2) {
			sccd_random_bytes(message, sizeof(message));
		}

		SCCD_BENCH_KERNEL("vbnn_ibs_verify",
			result = sccd_vbnn_ibs_verify(ta.public_key, id, sizeof(id), message, sizeof(message), &signature);
		)
	}
	SCCD_BENCH_RESULTS("vbnn_ibs_verify")
}

void bench_vbnn_ibs(void) {
	bench_vbnn_ibs_ta_init();
	bench_vbnn_ibs_ta_extract_key();
	bench_vbnn_ibs_sign();
	bench_vbnn_ibs_verify();
}

void bench(void) {
	printf("backend: %s\n", sccd_backend_name());

	sccd_cycles_init();
	sccd_init();

	bench_cycles();
	bench_noop();

	bench_fp();
	bench_ecc();
	bench_vbnn_ibs();
	printf("done\n");
}

#pragma GCC diagnostic pop

#ifndef RIOT_BOARD
int main(int argc, const char **argv) {
	if (argc == 2) {
		iterations = atoi(argv[1]);
	}
	//cycles_table = malloc(sizeof(uint64_t) * iterations);

	printf("Benchmark system information:\n");
	if (getenv("Apple_PubSub_Socket_Render")) {
		system("sw_vers");
	}
	printf("\n");

	int x = 11;
	double y = 1.1;
	for (int m = 0; m < 99999; m++) {
		x = x + x + 3;
		y = y + 3.3;
	}

	bench();
	//free(cycles_table);
	return 0;
}
#endif
