#include <stdint.h>
#include <terminal.h>

const int TERMINAL_WIDTH = 80;
const int TERMINAL_HEIGHT = 25;

uint16_t *terminal_buffer;

int cursor_x;
int cursor_y;

void terminal_init() {
	terminal_buffer = (uint16_t *) 0xB8000;

	cursor_x = 0;
	cursor_y = 0;
}

void terminal_putch(char c) {
	if (c != '\n') {
		uint16_t entry = 0x0F << 8 | c;
		terminal_buffer[cursor_x + cursor_y * TERMINAL_WIDTH] = entry;

		++cursor_x;
	} else {
		cursor_x = 0;
		++cursor_y;
	}

	if (cursor_x >= TERMINAL_WIDTH) {
		cursor_x = 0;
		++cursor_y;

		if (cursor_y >= TERMINAL_HEIGHT) {
			// We can't really handle this right now.
			// Let's just set the cursor back to the very top.

			cursor_x = 0;
			cursor_y = 0;
		}
	}
}

void terminal_puts(char *s) {
	while (*s) {
		terminal_putch(*s++);
	}
}
