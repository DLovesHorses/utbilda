#include <stdint.h>
#include <klibc/string.h>

void *memcpy(void *destination, const void *source, size_t num) {
	uint8_t *destination_byte = (uint8_t *) destination;
	uint8_t *source_byte = (uint8_t *) source;

	for (size_t i = 0; i < num; ++i) {
		destination_byte[i] = source_byte[i];
	}

	return destination;
}
