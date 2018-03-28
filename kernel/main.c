#include <terminal.h>

void kernel_main() {
	char *str = "Hello, world!\nThis is a test.\nThere's a lot of testing going on here!\n";

	terminal_init();

	while (*str != '\0') {
		terminal_putch(*str++);
	}
}
