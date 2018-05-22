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

# The types of files we will be compiling
SRCFILES := $(shell find $(PROJDIRS) -type f -name "*.c")
HDRFILES := $(shell find $(PROJDIRS) -type f -name "*.h")
OBJFILES := $(patsubst %.c,%.o,$(SRCFILES))
# Depfiles is special: GCC will generate .d dependancy files so that Make can parse them
DEPFILES := $(patsubst %.c,%.d,$(SRCFILES))
ALLFILES := $(SRCFILES) $(HDRFILES) $(AUXFILES)

# This is to avoid a situation where we have a file called "all" or "clean"
# And make does nothing, since the "all" file already exists
.PHONY: all clean

all: ramos.elf

ramos.elf: start.o linker.ld $(OBJFILES) 
	@$(CC) -ffreestanding -nostdlib -g -T linker.ld $(OBJFILES) start.o  -o mykernel.elf -lgcc

%.o: %.c Makefile
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@


start.o: start.s
# Translation: use the CC program with flags FLAGS
# -c input file is first dependancy in this rule ($<)
# -o output file is the name of this file ($@)
	@$(CC) $(CFLAGS) -c $< -o $@