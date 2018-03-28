#include <stdint.h>
#include <gdt.h>

struct GDT_GDTR {
	uint16_t limit;
	uint32_t base_address;
} __attribute__((packed));

const uint16_t FLAG_TYPE_DATA_RW = 0x2;
const uint16_t FLAG_TYPE_CODE_R = 0xA;

const uint16_t FLAG_DESCRIPTOR_TYPE_CODE_OR_DATA = 1 << 4;

const uint16_t FLAG_DPL_0 = 0 << 5;

const uint16_t FLAG_SEGMENT_PRESENT = 1 << 7;

const uint16_t FLAG_32_BIT_CODE_SEGMENT = 0 << 12;

const uint16_t FLAG_DEFAULT_OP_SIZE_32 = 1 << 13;

const uint16_t FLAG_GRANULARITY_4KB = 1 << 14;

uint64_t gdt[3];
struct GDT_GDTR gdtr;

// Defined in gdt_util.s
void gdt_reload_segment_registers();

void gdt_create_entry(
	int index,
	uint32_t segment_limit, uint32_t base_address,
	uint16_t flags
);

void gdt_create_null_entry(int index);

/**
 * @brief Initializes and installs the GDT.
 */
void gdt_init() {
	gdt_create_null_entry(0);
	gdt_create_entry(
		1,
		0xFFFFF, 0,
		FLAG_TYPE_CODE_R | FLAG_DESCRIPTOR_TYPE_CODE_OR_DATA | FLAG_DPL_0 |
		FLAG_SEGMENT_PRESENT | FLAG_32_BIT_CODE_SEGMENT |
		FLAG_DEFAULT_OP_SIZE_32 | FLAG_GRANULARITY_4KB
	);
	gdt_create_entry(
		2,
		0xFFFFF, 0,
		FLAG_TYPE_DATA_RW | FLAG_DESCRIPTOR_TYPE_CODE_OR_DATA | FLAG_DPL_0 |
		FLAG_SEGMENT_PRESENT | FLAG_32_BIT_CODE_SEGMENT |
		FLAG_DEFAULT_OP_SIZE_32 | FLAG_GRANULARITY_4KB
	);

	gdtr.limit = sizeof(gdt) - 1;
	gdtr.base_address = (uint32_t) &gdt;

	__asm__ (
		"lgdt (%0)\n"
		:
		: "r"(&gdtr)
	);

	gdt_reload_segment_registers();
}

void gdt_create_entry(
	int index,
	uint32_t segment_limit, uint32_t base_address,
	uint16_t flags
) {
	gdt[index] = 0;
	
	gdt[index] |= segment_limit & 0xFFFF;
	gdt[index] |= (uint64_t) ((segment_limit >> 16) & 0xF) << 48;

	gdt[index] |= (uint64_t) (base_address & 0xFFFF) << 16;
	gdt[index] |= (uint64_t) ((base_address >> 16) & 0xFF) << 32;
	gdt[index] |= (uint64_t) ((base_address >> 24) & 0xFF) << 56;

	gdt[index] |= (uint64_t) flags << 40;
}

void gdt_create_null_entry(int index) {
	gdt[index] = 0;
}
