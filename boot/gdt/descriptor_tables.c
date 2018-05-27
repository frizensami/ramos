#include "descriptor_tables.h"

// Structure and code taken from http://www.jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html

// Assembly code in gdt.s that flushes the GDT/IDT and installs our new one
extern void gdt_flush(uint32_t);
extern void idt_flush(uint32_t);
// Segments we will install: null segment, kernel code seg, kernel data seg, user code seg, user data seg
gdt_entry_t gdt_entries[5];
// Pointer that we need to pass to the GDT register
gdt_ptr_t   gdt_ptr;

// 256 possible IDT entries for the 256 interrupts possible
idt_entry_t idt_entries[256];
// Same as GDT - pointer to the IDT entries
idt_ptr_t   idt_ptr;

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

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
   idt_entries[num].base_lo = base & 0xFFFF;
   idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

   idt_entries[num].sel     = sel;
   idt_entries[num].always0 = 0;
   // TODO:
   // We must uncomment the OR below when we get to using user-mode.
   // It sets the interrupt gate's privilege level to 3.
   idt_entries[num].flags   = flags /* | 0x60 */;
}


static void idt_init()
{
    // There are 256 possible interrupt numbers
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    // Zero out the interrupt handler space
    memset(&idt_entries, 0, sizeof(idt_entry_t) * 256);

    // Remap the irq table - otherwise PICs mapping conflicts with CPU internal interrupts for faults
    // This remaps the PICs that are currently incorrectly mapped.
    // Code at http://www.jamesmolloy.co.uk/tutorial_html/5.-IRQs%20and%20the%20PIT.html
    // Apparently "difficult and obsfucated"
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);

    // Point the interrupt numbers to asm functions that handle them
    // E.g. - Software interrupt asm volatile ("int $0x1") sends an interrupt to isr1 (nope, see below)
    // This is CHANGED by the remapping above. 
    // We want IRQ 0 - 15 from the PIC to actually send us interrupt number 32 - 47, 
    // which will map to ISR 32 - 47.
    // This is ISR 0 - 31 is going to be dedicated to the CPU - the CPU reserves interrupts 0 - 31
    // So actually, the interrupt handlers we care about from EXTERNAL devices start from 32!
    
    // We are going to keep these around to handle CPU interrupts. 
    // TODO: Create isr2-31
    idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(2, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(3, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(4, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(5, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(6, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(7, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(8, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(9, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr1, 0x08, 0x8E);

    // We need --IRQ-- handlers here  - needs to handle talking to the PICs (master and slave)
    idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);

    idt_flush((uint32_t)&idt_ptr);

    printf("IDT Initialization complete!\n");
}




void descriptor_tables_init(void) {
    gdt_init();
    idt_init();
}