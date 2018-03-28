#include <terminal.h>

void kernel_main() {
	terminal_init();
	
	for (int i = 0; i < 30; ++i) {
		terminal_putch('0' + i);
		terminal_putch('\n');

		for (int j = 0; j < 100000000; ++j) {
			asm volatile("nop");
		}
	}

	terminal_puts("No terminating new line here, no new row should appear!");
}
