#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <sccd/core/fp.h>
#include <sccd/core/ec.h>
#include <sccd/core/clock.h>
#include <sccd/core/util.h>
#include <sccd/ibc/vbnn_ibs.h>


static uint32_t iterations = 10;
static uint32_t bench_iterations = 15;

static uint64_t min(const uint64_t a, const uint64_t b) {
	return a < b ? a : b;
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
		printf("bench("NAME"): %llu cycles\n", cycles_table[z]); \
	}

void bench_noop() {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		SCCD_BENCH_KERNEL("nop",
			__asm__ volatile("nop");
		)
	}
	SCCD_BENCH_RESULTS("nop")
}

void bench_cycles() {
	uint64_t cycles_table[iterations];
	for (int z = 0; z < iterations; z++) {
		uint64_t tmp;
		SCCD_BENCH_KERNEL("cycles",
			tmp = sccd_cycles();
		)
	}
	SCCD_BENCH_RESULTS("cycles")
}

void bench_fp_add() {
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

void bench_fp_sub() {
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

void bench_fp_mul() {
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

void bench_fp_inv() {
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

void bench_fp_equal() {
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


void bench_fp_copy() {
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



void bench_fp() {
	bench_fp_add();
	bench_fp_sub();
	bench_fp_mul();
	bench_fp_inv();
	bench_fp_equal();
	bench_fp_copy();
}

void bench_ec_add() {
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

void bench_ec_sub() {
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

void bench_ec_double() {
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

void bench_ec_mul() {
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

void bench_ec_equal() {
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

void bench_ec_copy() {
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

void bench_ec_bin_write() {
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

void bench_ec_bin_read() {
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

void bench_ecc() {
	bench_ec_add();
	bench_ec_sub();
	bench_ec_double();
	bench_ec_mul();
	bench_ec_equal();
	bench_ec_copy();
	bench_ec_bin_write();
	bench_ec_bin_read();
}

void bench_vbnn_ibs_ta_init() {
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

void bench_vbnn_ibs_ta_extract_key() {
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

void bench_vbnn_ibs_sign() {
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

void bench_vbnn_ibs_verify() {
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

void bench_vbnn_ibs() {
	bench_vbnn_ibs_ta_init();
	bench_vbnn_ibs_ta_extract_key();
	bench_vbnn_ibs_sign();
	bench_vbnn_ibs_verify();
}

void bench() {
	printf("backend: %s\n", sccd_backend_name());

	sccd_cycles_init();
	sccd_init();

	bench_cycles();
	bench_noop();

	bench_fp();
	bench_ecc();
	bench_vbnn_ibs();
}

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
