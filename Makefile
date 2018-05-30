all: ramos.elf

# These are files that are part of the distribution but are not source code
AUXFILES := Makefile lessons.txt

# All project directories to search for files in - only current dir for now 
PROJDIRS := .

CC := i686-elf-gcc
# Enabled warnings for the compiler
WARNINGS := -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wconversion -Wstrict-prototypes
# Actual compiler settings: -g (debug symbols) and 
CFLAGS := -g -std=gnu99 -ffreestanding $(WARNINGS)

# iso builder
ISO = genisoimage

# The types of files we will be compiling
SRCFILES    := $(shell find $(PROJDIRS) -type f -name "*.c")
ASMFILES    := $(shell find $(PROJDIRS) -type f -name "*.s")
HDRFILES    := $(shell find $(PROJDIRS) -type f -name "*.h")
OBJASMFILES := $(patsubst %.s,%.o,$(ASMFILES))
OBJCFILES   := $(patsubst %.c,%.o,$(SRCFILES))
OBJFILES    := $(OBJCFILES) $(OBJASMFILES)
# Depfiles is special: GCC will generate .d dependancy files so that Make can parse them
DEPFILES    := $(patsubst %.c,%.d,$(SRCFILES))
-include $(DEPFILES) 
ALLFILES    := $(SRCFILES) $(ASMFILES) $(HDRFILES) $(AUXFILES)

LINKSCRIPT  := boot/linker.ld
# This is to avoid a situation where we have a file called "all" or "clean"
# And make does nothing, since the "all" file already exists
.PHONY: all clean run ramos.elf

ramos.elf: $(LINKSCRIPT) $(OBJFILES) $(HDRFILES) 
	$(CC) -ffreestanding -nostdlib -g -T $(LINKSCRIPT) $(OBJFILES) -o ramos.elf -lgcc

%.o: %.c Makefile
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

%.o: %.s Makefile
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(wildcard $(OBJFILES) $(DEPFILES))

run: ramos.elf
	qemu-system-i386 -m 4G -kernel ramos.elf

os.iso: ramos.elf
	cp ramos.elf iso/boot/ramos.elf
	$(ISO)  -R                              \
            -b boot/grub/stage2_eltorito    \
            -no-emul-boot                   \
            -boot-load-size 4               \
            -A os                           \
            -input-charset utf8             \
            -quiet                          \
            -boot-info-table                \
            -o os.iso                       \
            iso

run-bochs: os.iso
	bochs -f bochsrc.txt -q



run-debug: ramos.elf
	qemu-system-i386 -m 4G -kernel ramos.elf -gdb tcp::9000 -S


