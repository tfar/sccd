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

static const char *random_iv = "sccd_crypto_iv";

static uint8_t random_state[32];

const char* sccd_backend_name() {
	return "c25519";
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
