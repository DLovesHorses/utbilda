#include <gdt.h>
#include <terminal.h>

/**
 * @brief Kernel main function, called from <tt>boot.s</tt>.
 */
void kernel_main() {
	terminal_init();

	gdt_init();

	terminal_puts("Kernel has booted.\n");
}
