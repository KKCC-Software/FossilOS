# =====================================================
# FossilOS Makefile — builds a bootable ISO
# =====================================================

TARGET = FossilOS
ISO = $(TARGET).iso

SRCDIR = src
SOURCES_C := $(wildcard $(SRCDIR)/*.c)
SOURCES_ASM := $(wildcard $(SRCDIR)/*.s) $(wildcard $(SRCDIR)/*.asm)

OBJ = $(SOURCES_C:.c=.o) $(SOURCES_ASM:.s=.o)
OBJ := $(OBJ:.asm=.o)

CC = gcc
LD = ld
ASM = nasm

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -T linker.ld -m elf_i386
ASFLAGS = -f elf32

.PHONY: all iso clean run

# =====================================================
# Build rules
# =====================================================

all: $(TARGET).bin

# Compile C source
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Assemble NASM source
$(SRCDIR)/%.o: $(SRCDIR)/%.asm
	$(ASM) $(ASFLAGS) $< -o $@

# Assemble GAS source
$(SRCDIR)/%.o: $(SRCDIR)/%.s
	$(CC) -m32 -c $< -o $@

# Link kernel binary
$(TARGET).bin: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

# =====================================================
# Create bootable ISO
# =====================================================

iso: $(TARGET).bin
	mkdir -p iso/boot/grub
	cp $(TARGET).bin iso/boot/$(TARGET).bin
	printf '%s\n' \
		'set timeout=0' \
		'set default=0' \
		'' \
		'menuentry "FossilOS" {' \
		'  multiboot /boot/$(TARGET).bin' \
		'  boot' \
		'}' > iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso

# =====================================================
# Run using QEMU
# =====================================================

run: iso
	qemu-system-i386 -cdrom $(ISO)

# =====================================================
# Cleanup
# =====================================================

clean:
	rm -f $(OBJ) $(TARGET).bin $(ISO)
	rm -rf iso
