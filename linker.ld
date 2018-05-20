/* 
Defines the symbol that the linker will consider as the start point 
We want this to be "start" in start.s
*/
ENTRY(start)

/* Lay out the sections for insertion into the final executable */
SECTIONS
{
	/* Put sections starting at 1 MB (2^20 bytes) */
	/* Everything below this is x86-reserved */
	/* Note that this means we must be in a non-Real mode, Real mode only goes to 1 MB */
	/* The "." refers to the current location counter (inside SECTIONS) */
	. = 1M;

	/* All sections are to be aligned at 4K boundaries for paging purposes */

	/* Read-only data - only our multiboot block at the start */
	.rodata BLOCK(4K) : ALIGN(4K) 
	{
		*(.multiboot)
	}

	/* Executable code - .text from start.s */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
	}

	/* Rest of the read-only data - VGA_COLS, VGA_ROWS, etc */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	/* All data from .s and .c. Includes vga_buffer pointer. */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	/* The stack (.bss from start.s) is here, and so is the COMMON section */
	/* COMMON: all uninitialized data from all input files (so far as it is not yet allocated) */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
}