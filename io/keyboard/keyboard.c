#include "keyboard.h"


static void keyboard_callback(registers_t regs) {
    printf("Received Keyboard Interrupt (33)\n");
    unsigned char scan_code = inb(0x60);
    printf("Received keyboard scan code: %c\n", scan_code);
}


void keyboard_init(void) {

   // Firstly, register our timer callback.
   register_interrupt_handler(IRQ1, &keyboard_callback);

   // Just in case - enable interrupts
   printf("Keyboard initialized\n");

}

