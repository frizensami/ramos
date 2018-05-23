#include "descriptor_tables.h"

// Structure and code taken from http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html

// Assembly code in gdt.s that flushes the GDT and installs our new one
extern void gdt_flush(uint32_t);

// Segments we will install: null segment, kernel code seg, kernel data seg, user code seg, user data seg
gdt_entry_t gdt_entries[5];
// Pointer that we need to pass to the GDT register
gdt_ptr_t   gdt_ptr;

// Set the value of one GDT entry.
static void gdt_set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}

// Initializes the Global Descriptor Table
// This is needed to set up the Interrupt Descriptor table later
// We are setting the GDT to have full access to whole memory space. Access rights will be done by paging
static void gdt_init(void) {


    // Set the GDT pointer structure to the current struct array of entries
    gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries; // Address of the first entry

    gdt_set_gate(0, 0, 0, 0, 0); // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel Code segment - offset 0x08 (each entry 8 bytes)
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel Data segment - offset 0x10
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

    // Write the address of our POINTER STRUCT to the gdtr
    // This is an EXTERN function inside gdt.s
    gdt_flush((uint32_t)&gdt_ptr);

    printf("GDT Initialization Complete!\n");

    // This section of code confirms that mov 4(%esp), eax is getting the &gdt_ptr arg
    /*
    int i;
    asm("\t movl %%eax,%0" : "=r"(i));
    printf("EAX Value: %d\n", i);
    printf("gdt_ptr address: %d\n", &gdt_ptr);
    */
}





void descriptor_tables_init(void) {
    gdt_init();
}