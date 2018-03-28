AS:=i686-elf-as
AS_FLAGS:=

CC:=i686-elf-gcc
CC_FLAGS:=-std=gnu99 -ffreestanding -O2 -Wall -Wextra -Wpedantic -Werror=implicit-function-declaration -MD -MP

LD:=i686-elf-gcc
LD_FLAGS:=-ffreestanding -O2 -nostdlib -lgcc

QEMU:=qemu-system-i386
QEMU_FLAGS:=-cdrom utbilda.iso

AS_SOURCES:=$(wildcard kernel/*.s)
CC_SOURCES:=$(wildcard kernel/*.c)
OBJECTS:=$(AS_SOURCES:.s=.o) $(CC_SOURCES:.c=.o)

HEADER_DEPS:=$(CC_SOURCES:.c=.d)

all: utbilda.iso Makefile

clean:
	rm -f $(OBJECTS) $(HEADER_DEPS) iso/boot/utbilda.bin utbilda.iso

###################
# DEBUGGING TASKS #
###################
run_qemu:
	$(QEMU) $(QEMU_FLAGS)

################
# ISO CREATION #
################
utbilda.iso: iso/boot/utbilda.bin iso/
	rm -f utbilda.iso
	grub-mkrescue -o $@ iso/

###############
# COMPILATION #
###############
iso/boot/utbilda.bin: $(OBJECTS) kernel/linker.ld
	$(LD) -T kernel/linker.ld -o $@ $(LD_FLAGS) $(OBJECTS)

kernel/%.o: kernel/%.s
	$(AS) $< -o $@ $(AS_FLAGS)

kernel/%.o: kernel/%.c
	$(CC) -c $< -o $@ $(CC_FLAGS) -I kernel/include

-include $(HEADER_DEPS)

.PHONY: all clean run_qemu
