.global gdt_reload_segment_registers
.type gdt_reload_segment_registers, @function
gdt_reload_segment_registers:
	ljmp $0x08, $reload_cs
reload_cs:
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es
	mov %ax, %fs
	mov %ax, %gs
	mov %ax, %ss
	ret
