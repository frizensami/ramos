#include "isr.h"
#include "../../lib/libk.h"

// Our array of interrupt handling functions!
isr_t interrupt_handlers[256];

// Handles basic ISRs from the CPU (interrupt numbers 0 - 31)
void isr_handler(registers_t regs)
{
    printf("Recieved --ISR (from CPU)-- interrupt: %d. ", regs.int_no);
    // http://www.osdever.net/bkerndev/Docs/isrs.htm
    switch (regs.int_no) {
        case 0:
            printf("Divide by 0 exception!\n");
            break;
        default:
            printf("Unknown interrupt reason!\n");
            break;
    }
}

// This gets called from our ASM interrupt handler stub.
// Slightly different than isr_handler. We need to send an ACK (called an EOI) to
// either the master, or the master AND the slave, depending on interrupt source
void irq_handler(registers_t regs)
{
    printf("IRQ Handler Called!\n");


    // Custom mechanism with interrupt_handlers array to get interrupt handler function handle
    if (interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }

    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (regs.int_no >= 40) {
        // Send reset signal to slave.
        outb(0xA0, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);


}

// Utility functions to register our interrupt handling function
void register_interrupt_handler(uint8_t n, isr_t handler)
{
    interrupt_handlers[n] = handler;
}