#include <terminal.h>

void kernel_main() {
	char *str = "Hello, world!\nThis is a test.\nThere's a lot of testing going on here!\n";

	terminal_init();
	terminal_puts(str);
}
