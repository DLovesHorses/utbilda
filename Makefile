AS:=i686-elf-as
AS_FLAGS:=

CC:=i686-elf-gcc
CC_FLAGS:=-std=gnu99 -ffreestanding -O2 -Wall -Wextra -Wpedantic

LD:=i686-elf-gcc
LD_FLAGS:=-ffreestanding -O2 -nostdlib -lgcc

QEMU:=qemu-system-i386
QEMU_FLAGS:=-cdrom utbilda.iso

AS_SOURCES:=$(wildcard kernel/*.s)
CC_SOURCES:=$(wildcard kernel/*.c)
OBJECTS:=$(AS_SOURCES:.s=.o) $(CC_SOURCES:.c=.o)

all: utbilda.iso Makefile

clean:
	rm -f $(OBJECTS) iso/boot/utbilda.bin utbilda.iso

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
	$(CC) -c $< -o $@ $(CC_FLAGS)

.PHONY: all clean run_qemu
