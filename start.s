# AT&T Syntax - GNU AS or GAS
# start.s: assembly code to set up x86 environment and start the kernel

# kernel_main is a symbol external to this .s file
# this is the main entry point into our kernel in the kernel.c file
.extern kernel_main

# The start label is indicated as accessible from outside this file
# This is so that the linker can have access to it
.global start

# The following constants are for GRUB - the bootloader
# It is in the GRUB Multiboot format
# These are for the GRUB Multiboot Header

# This is a 'magic' constant that GRUB will use to detect our kernel's location.
.set MB_MAGIC, 0x1BADB002
# Options to tell GRUB to: load modules on page boundaries + provide a memory map
# Looks like first two LSBits are set
.set MB_FLAGS, (1 << 0) | (1 << 1)
# Calculate a checksum
.set MB_CHECKSUM, (0 - (MB_MAGIC + MB_FLAGS))

# We now actually DEFINE the multiboot section at the start of the executable
.section .multiboot
	# The data should be aligned on 4 byte boundaries
	.align 4
	# Put in the constants and checksum that is part of the multiboot header
	.long MB_MAGIC
	.long MB_FLAGS
	.long MB_CHECKSUM


# Define a section that should be filled with zeroes (the .bss section)
# Essentially we are creating the stack for the C code - 4096 byte (4K) stack
.section .bss
	.align 16 # QN: WHY Align to 16 instead of 4?
	stack_bottom:
		# Skip 4K bytes from this label
		.skip 4096
	stack_top:



# Actual executable code starts here
.section .text
	# Define the start label - the first code that ever gets run in our kernel
	start:
		# All we need to do to set up the environement for C code is to
		# set up the stack pointer %esp
		# The stack grows DOWNWARDS - that's why start at top
		mov $stack_top, %esp 

		# We want to get the "multiboot info" from the bootloader
		# EBP contains the address of this info
		# This will become an argument to main
		# eax will contain the multiboot magic number - argument #2
		push %eax
		push %ebx

		# That's all! Now, just call the C start function
		call kernel_main

		# If the C-code ever returns... er...
		# We just hang the CPU
		hang:
			# Disable CPU interrupts
			cli 
			# Halt the CPU
			hlt
			# If we couldn't halt, try again
			jmp hang
