#include <stdint.h>

void kernel_main() {
	uint16_t *vga_terminal_buffer = (uint16_t *) 0xB8000;
	char *str = "Hello, world!";

	while (*str != '\0') {
		*vga_terminal_buffer = 0x0A << 8 | *str;

		++str;
		++vga_terminal_buffer;
	}
}
