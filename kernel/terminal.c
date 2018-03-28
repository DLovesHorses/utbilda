#include <stdint.h>
#include <klibc/string.h>
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

	for (int i = 0; i < TERMINAL_WIDTH * TERMINAL_HEIGHT; ++i) {
		terminal_buffer[i] = 0x0F << 8 | ' ';
	}
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
		// The cursor has gone off the right side of the screen.
		// Bring it back to the left edge, and move down a row.
		cursor_x = 0;
		++cursor_y;
	}

	if (cursor_y >= TERMINAL_HEIGHT) {
		// The cursor has gone off the bottom of the screen.
		// Scroll the terminal up one row, and move the cursor up.

		// Scroll the terminal up one row
		size_t bytes_per_row = TERMINAL_WIDTH * sizeof(*terminal_buffer);
		memcpy(
			terminal_buffer,
			terminal_buffer + TERMINAL_WIDTH,
			bytes_per_row * (TERMINAL_HEIGHT - 1)
		);

		// Clear the bottom line
		int y = TERMINAL_HEIGHT - 1;
		uint16_t entry = 0x0F << 8 | ' ';
		for (int i = 0; i < TERMINAL_WIDTH; ++i) {
			terminal_buffer[i + y * TERMINAL_WIDTH] = entry;
		}

		// Move the cursor to the bottom row
		cursor_y = TERMINAL_HEIGHT - 1;
	}
}

void terminal_puts(char *s) {
	while (*s) {
		terminal_putch(*s++);
	}
}
