#include <stdint.h>
#include <stddef.h>

const char* sccd_backend_name();
void sccd_random_init();
uint8_t sccd_random_byte();
void sccd_random_bytes(uint8_t *data, size_t length);
