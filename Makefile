# Compiler settings
CC = i686-elf-gcc
LD = i686-elf-ld
NASM = nasm
CCFLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc -I./src -std=gnu99
LDFLAGS = -g -relocatable
NASMFLAGS = -f elf -g

# Directories
BIN_DIR = ./bin
BUILD_DIR = ./build
IDT_DIR = $(BUILD_DIR)/idt
MEMORY_DIR = $(BUILD_DIR)/memory
IO_DIR = $(BUILD_DIR)/io

# Files
FILES = $(BUILD_DIR)/kernel.asm.o $(BUILD_DIR)/kernel.o $(IDT_DIR)/idt.asm.o $(IDT_DIR)/idt.o $(MEMORY_DIR)/memory.o  $(IO_DIR)/io.asm.o
KERNEL_BIN = $(BIN_DIR)/kernel.bin 
BOOT_BIN = $(BIN_DIR)/boot.bin
OS_IMAGE = $(BIN_DIR)/os.bin

# Phony targets
.PHONY: all dirs clean

# Default target
all: dirs $(BOOT_BIN) $(KERNEL_BIN) os_image

# OS image creation
os_image: 
	rm -rf $(OS_IMAGE)
	dd if=$(BOOT_BIN) of=$(OS_IMAGE)
	dd if=$(KERNEL_BIN) >> $(OS_IMAGE)
	dd if=/dev/zero bs=512 count=100 >> $(OS_IMAGE)

# Directory structure
dirs:
	mkdir -p $(BIN_DIR) $(BUILD_DIR) $(IDT_DIR) $(MEMORY_DIR) $(IO_DIR)
# Kernel binary
$(KERNEL_BIN): $(FILES)
	$(LD) $(LDFLAGS) $(FILES) -o $(BUILD_DIR)/kernelfull.o
	$(CC) $(CCFLAGS) -T ./src/linker.ld -o $(KERNEL_BIN) $(BUILD_DIR)/kernelfull.o

# Bootloader binary
$(BOOT_BIN): ./src/boot/boot.asm
	$(NASM) -f bin ./src/boot/boot.asm -o $(BOOT_BIN)

# Pattern rules for object files
$(BUILD_DIR)/%.o: ./src/%.c
	$(CC) $(CCFLAGS) -c $< -o $@

$(BUILD_DIR)/%.asm.o: ./src/%.asm
	$(NASM) $(NASMFLAGS) $< -o $@

# Cleaning up
clean:
	rm -rf $(BOOT_BIN) $(KERNEL_BIN) $(OS_IMAGE) $(BUILD_DIR)/kernelfull.o $(BUILD_DIR)