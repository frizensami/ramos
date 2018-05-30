/* kernel.c - Entry point to kernel */
/* Copyright (C) 2018, Sriram Sami.
      *
      *  Permission is hereby granted, free of charge, to any person obtaining a copy
      *  of this software and associated documentation files (the "Software"), to
      *  deal in the Software without restriction, including without limitation the
      *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
      *  sell copies of the Software, and to permit persons to whom the Software is
      *  furnished to do so, subject to the following conditions:
      *
      *  The above copyright notice and this permission notice shall be included in
      *  all copies or substantial portions of the Software.
      *
      *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
      *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
      *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL ANY
      *  DEVELOPER OR DISTRIBUTOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
      *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
      *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
      */



// GCC provides these header files automatically
// They give us access to useful things like fixed-width types
#include "../lib/common.h"
#include "../io/terminal/terminal.h"
#include "./multiboot/multiboot_parser.h"
#include "../lib/libk.h"
#include "./gdt/descriptor_tables.h"
#include "../io/keyboard/keyboard.h"
#include "../io/timer/timer.h"


// Do some basic checking on this code so that it's used correctly
#if defined(__linux__)
    #error "This code must be compiled with a cross compiler!"
#elif !defined(__i386__)
    #error "This code must be compiled with an x86-elf compiler!"
#endif

void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
    // This will be called by start.s to start up the kernel
    // Remember: we only have 4K stack space at this point!

    // Initialize the terminal
    term_init();

    // Basic boot info    
    printf("--------------------------------[RAMOS BOOTING]--------------------------------\n");

    // Print out all information given from the Multiboot-compliant bootloader header
    print_multiboot_info(mbd, magic);

    // Set up the GDT and IDT!
    descriptor_tables_init();

    // Set up our keyboard to fire off interrupts
    keyboard_init();

    // Set up a timer
    timer_init(1);

    // Unmask all interrupts
    //outb(0x21, 0xff);
    //outb(0xa1, 0xff);

    // These masks work: setting 0x21, 0x01 masks the timer interrupt (bit 0)
    outb(0x21,0x00);
    outb(0xa1,0x00);

    // Ensure interrupts are on -- we DO NOT get any interrupts if this is not on!
    asm volatile("sti");

    while (1) {
        // We need to do nothing here, otherwise CPU will halt!
        ;
    }

    //asm volatile ("int $0x1");
    //asm volatile ("int $33");

/*
    register int eax asm("eax");
    register int ebx asm("ebx");
    register int ecx asm("ecx");
    register int esp asm("esp");

    eax = 0;
    ebx = 0;
    ecx = 0;
    // esp = 0; --> DON'T DO THIS - if stack pointer is modified, BAD STUFF happens

    int cEax = eax;
    int cEbx = ebx;
    int cEcx = ecx;
    int cEsp = esp;
*/
}
