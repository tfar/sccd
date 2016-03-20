#pragma once

#include <stdint.h>
#include <stddef.h>

void sccd_init();

const char* sccd_backend_name();
void sccd_random_init();
uint8_t sccd_random_byte();
void sccd_random_bytes(uint8_t *data, size_t length);
// Returns 0 if equal, else otherwise.
int sccd_const_time_cmp(const uint8_t* a, const uint8_t* b, size_t size);
