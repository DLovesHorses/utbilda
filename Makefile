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
CC_SOURCES+=$(wildcard kernel/*/*.c)
OBJECTS:=$(AS_SOURCES:.s=.o) $(CC_SOURCES:.c=.o)

HEADER_DEPS:=$(CC_SOURCES:.c=.d)

# If DEBUG file exists, and its contents are the character '1', then turn on
# debugging
ifeq ($(shell test -e DEBUG && echo 1),1)
ifeq ($(shell cat DEBUG),1)
	AS_FLAGS+=-g
	CC_FLAGS+=-g

	QEMU_FLAGS+=-s -S
endif
endif

all: utbilda.iso Makefile

clean:
	rm -f $(OBJECTS) $(HEADER_DEPS) iso/boot/utbilda.bin utbilda.iso

###################
# DEBUGGING TASKS #
###################
run_qemu:
	$(QEMU) $(QEMU_FLAGS) $(QEMU_EXTRA_FLAGS)

run_gdb:
	-gdb iso/boot/utbilda.bin -command gdb_commands

# If the debug flag file doesn't exist, make one which turns off debugging
DEBUG:
	$(shell echo 0 > DEBUG)

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

kernel/%.o: kernel/%.s DEBUG
	$(AS) $< -o $@ $(AS_FLAGS)

kernel/%.o: kernel/%.c DEBUG
	$(CC) -c $< -o $@ $(CC_FLAGS) -I kernel/include

-include $(HEADER_DEPS)

.PHONY: all clean run_qemu run_gdb
