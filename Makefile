# makefile for WylarOS

BUILD_DIR = build
CC = gcc # c compiler
LD = ld # linker
AS = nasm # assembler

ARCH := i386

ENTRY = entry.asm
ENTRY_OBJ = $(BUILD_DIR)/entry.o

KERNEL_SRC = $(shell find src -name "*.c")
KERNEL_OBJ = $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(KERNEL_SRC))

ASM_SRC = $(shell find src -name "*.asm")
ASM_OBJ = $(patsubst src/%.asm,$(BUILD_DIR)/%.o,$(ASM_SRC))

LD_SCRIPT = src/link.ld

# note: NEVER delete werror.
CFLAGS = -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -Wall -Wextra -Werror -Iinclude
LFLAGS =
ASFLAGS =

ISO_DIR = $(BUILD_DIR)/iso
GRUB_CFG = $(ISO_DIR)/boot/grub/grub.cfg
ISO_IMG = $(BUILD_DIR)/wylar.iso

ifeq ($(ARCH), x86_64)
CFLAGS += -m64
LFLAGS += -m elf_x86_64
ASFLAGS += -f elf64
else
CFLAGS += -m32
LFLAGS += -m elf_i386
ASFLAGS += -f elf32
endif

.PHONY: all clean run iso run-iso dirs

all: dirs $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(ENTRY_OBJ) $(KERNEL_OBJ) $(ASM_OBJ)
	$(LD) $(LFLAGS) -T $(LD_SCRIPT) -o $@ $^

$(ENTRY_OBJ): $(ENTRY)
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $< -MMD -MF $(@:.o=.d)

$(BUILD_DIR)/%.o: src/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

iso: dirs $(ISO_IMG)

$(ISO_IMG): $(BUILD_DIR)/kernel.bin $(GRUB_CFG)
	grub-mkrescue -o $@ $(ISO_DIR) --modules="part_msdos part_gpt normal multiboot multiboot2" \
		--product-name="WylarOS" --product-version="1.0"

$(GRUB_CFG):
	@mkdir -p $(ISO_DIR)/boot/grub
	cp $(BUILD_DIR)/kernel.bin $(ISO_DIR)/boot/
	cp src/disk.img $(ISO_DIR)/boot/
	@echo 'set gfxmode=1024x768x32' >> $(GRUB_CFG)
	@echo 'set gfxpayload=keep' >> $(GRUB_CFG)

	@echo 'menuentry "WylarOS" {' >> $(GRUB_CFG)
	@echo '    multiboot2 /boot/kernel.bin' >> $(GRUB_CFG)
	@echo '    boot' >> $(GRUB_CFG)
	@echo '}' >> $(GRUB_CFG)

run: $(BUILD_DIR)/kernel.bin
	qemu-system-x86_64 -kernel $< -m 512M -vga std -no-reboot

run-iso: iso
	qemu-system-x86_64 -cdrom $(ISO_IMG) -m 512M -vga std -no-reboot

dirs:
	@mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

-include $(wildcard $(BUILD_DIR)/**/*.d)