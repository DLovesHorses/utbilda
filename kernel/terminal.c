#include <stdint.h>
#include <klibc/string.h>
#include <io.h>
#include <terminal.h>

const int TERMINAL_WIDTH = 80;
const int TERMINAL_HEIGHT = 25;

const uint16_t VGA_ADDR_REG_PORT = 0x03D4;
const uint16_t VGA_DATA_REG_PORT = 0x03D5;

const uint8_t VGA_CURSOR_START_IDX = 0x0A;
const uint8_t VGA_CURSOR_START_CURSOR_DISABLE = 1 << 5;

const uint8_t VGA_CURSOR_END_IDX = 0x0B;

const uint8_t CURSOR_LINE_START = 14;
const uint8_t CURSOR_LINE_END = 15;

uint16_t *terminal_buffer;

int cursor_x;
int cursor_y;

void terminal_init_hardware_cursor();

/**
 * @brief Initializes the terminal.
 *
 * Sets up internal values and clears the screen, ready to be written to.
 */
void terminal_init() {
	terminal_buffer = (uint16_t *) 0xB8000;

	cursor_x = 0;
	cursor_y = 0;

	terminal_init_hardware_cursor();

	for (int i = 0; i < TERMINAL_WIDTH * TERMINAL_HEIGHT; ++i) {
		terminal_buffer[i] = 0x0F << 8 | ' ';
	}
}

/**
 * @brief Enables and initializes the hardware cursor.
 */
void terminal_init_hardware_cursor() {
	io_outb(VGA_ADDR_REG_PORT, VGA_CURSOR_START_IDX);
	uint8_t cursor_start_register = io_inb(VGA_DATA_REG_PORT);

	// Unset disable bit
	cursor_start_register &= ~VGA_CURSOR_START_CURSOR_DISABLE;

	// Set line start
	cursor_start_register |= CURSOR_LINE_START & 0x1F;

	io_outb(VGA_DATA_REG_PORT, cursor_start_register);

	io_outb(VGA_ADDR_REG_PORT, VGA_CURSOR_END_IDX);
	uint8_t cursor_end_register = io_inb(VGA_DATA_REG_PORT);

	// Set line end
	cursor_end_register |= CURSOR_LINE_END & 0x1F;

	io_outb(VGA_DATA_REG_PORT, cursor_end_register);
}

/**
 * @brief Moves the hardware cursor to the current cursor position.
 */
void terminal_update_hardware_cursor() {
	uint16_t cursor_position = cursor_x + cursor_y * TERMINAL_WIDTH;
	uint8_t cursor_position_msb = cursor_position >> 8;
	uint8_t cursor_position_lsb = cursor_position & 0xFF;

	io_outb(0x03D4, 0x0E);
	io_outb(0x03D5, cursor_position_msb);

	io_outb(0x03D4, 0x0F);
	io_outb(0x03D5, cursor_position_lsb);
}

/**
 * @brief Writes a single character to the screen, and moves the cursor forward.
 *
 * If the new cursor position is off the right-hand edge of the screen, the
 * cursor is positioned at the start of the next line. If the new cursor
 * position is off the bottom edge of the screen, the terminal is scrolled up
 * and the cursor placed at the start of the last line.
 *
 * @param c The character to print.
 */
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

	terminal_update_hardware_cursor();
}

/**
 * @brief Writes a string to the screen, and moves the cursor appropriately.
 *
 * @param s The string to print.
 *
 * @see terminal_putch
 */
void terminal_puts(char *s) {
	while (*s) {
		terminal_putch(*s++);
	}
}
