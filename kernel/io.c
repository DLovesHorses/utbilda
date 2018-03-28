#include <io.h>

/**
 * @brief Writes a byte to an IO port.
 *
 * @param port The port to write to.
 * @param data The byte to write to the IO port.
 */
void io_outb(uint16_t port, uint8_t data) {
	__asm__(
		"out %%al, %%dx\n"
		:
		: "Nd"(port), "a"(data)
	);
}

/** 
 * @brief Reads a byte from an IO port.
 *
 * @param port The port to read from.
 * @returns The byte read from the port.
 */
uint8_t io_inb(uint16_t port) {
	uint8_t data = 0;

	__asm__(
		"in %%dx, %%al\n"
		: "=a"(data)
		: "Nd"(port)
	);

	return data;
}
