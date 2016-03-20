#include <sccd/core/util.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

#if defined(__APPLE__)
#include <sys/uio.h>
#endif

#include <blake2.h>

#if defined(SCCD_BACKEND_RELIC)
	#include <relic/relic.h>
#endif

static const char *random_iv = "sccd_crypto_iv";

static uint8_t random_state[32];

const char* sccd_backend_name() {
#if defined(SCCD_BACKEND_C25519)
	return "c25519";
#elif defined(SCCD_BACKEND_RELIC)
	return "relic";
#else
	#error No crypto backend defined. See README.
#endif
}

void sccd_init() {
#if defined(SCCD_BACKEND_RELIC)
	core_init();
	ec_param_set_any();
#endif
	sccd_random_init();
}

uint8_t sccd_random_byte() {
	uint8_t randomByte = 0;

	sccd_random_byte(&randomByte, 1);

	return randomByte;
}

void sccd_random_init() {
	blake2s_state S[1];
	blake2s_init(S, 32);
	blake2s_update(S, (const uint8_t*)random_iv, strlen(random_iv));
	blake2s_final(S, random_state, 32);
}

void sccd_random_bytes(uint8_t *data, size_t length) {
#if 0
	int fd = open("/dev/urandom", O_RDONLY);
	ssize_t result = read(fd, data, length);
	if (result != length) {
		// error, unable to read /dev/random
		assert(0);
	}
	close(fd);
#else
	while (length > 0) {
		size_t copy_n = length < 32 ? length : 32;
		memcpy(data, random_state, copy_n);
		data = data + copy_n;
		length = length - copy_n;

		blake2s_state S[1];
		blake2s_init(S, 32);
		blake2s_update(S, (const uint8_t*)random_iv, strlen(random_iv));
		blake2s_final(S, random_state, 32);
	}
#endif
}

int sccd_const_time_cmp(const uint8_t* a, const uint8_t* b, size_t size) {
	const uint8_t* a_ = a;
    const uint8_t* b_ = b;
    uint8_t result = 0;
    size_t i;

    for (i = 0; i < size; i++) {
      result |= a_[i] ^ b_[i];
    }

    return result;
}
