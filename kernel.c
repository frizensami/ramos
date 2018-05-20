// GCC provides these header files automatically
// They give us access to useful things like fixed-width types
#include <stddef.h>
#include <stdint.h>
#include "terminal.c"
#include "multiboot.h"


// Do some basic checking on this code so that it's used correctly
#if defined(__linux__)
    #error "This code must be compiled with a cross compiler!"
#elif !defined(__i386__)
    #error "This code must be compiled with an x86-elf compiler!"
#endif

// Define my own strlen - just count until null byte
int strlen(char* s) {
    int i = 0;
    while (s[i] != '\0') i++;
    return i;
}

/* reverse:  reverse string s in place */
void reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

/* itoa:  convert n to characters in s */
// This is the K&R standard
void itoa(char s[], int n)
{
    int i, sign;

    if ((sign = n) < 0) /* record sign */
        n = -n; /* make n positive */
    i = 0;
    do { /* generate digits in reverse order */
        s[i++] = n % 10 + '0'; /* get next digit */
    } while ((n /= 10) > 0); /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}



void busysleep_tiny() {
    // sleep?
    for (int i = 0; i < 10000000; i++) {
        int a = 3;
        a++;
    }
}


void kernel_main(multiboot_info_t* mbd, unsigned int magic) {
    // This will be called by start.s to start up the kernel
    // Remember: we only have 4K stack space at this point!

    // Initialize the terminal
    term_init();

    // Basic boot info    
    term_printstr("Hello world, from RamOS!\n");
    term_printstr("Accessing Multiboot Info...\n");

    if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
        term_printstr("Multiboot Header Info is AVAILABLE!\n");

        if (mbd->flags & MULTIBOOT_INFO_MEMORY)  {
            term_printstr("Multiboot: Basic Memory Info Available. Low: ");
            multiboot_uint32_t mem_lower = mbd->mem_lower;
            multiboot_uint32_t mem_higher = mbd->mem_upper;
            char temp[34];
            itoa(temp, mem_lower);
            term_printstr(temp);
            term_printstr(", High: ");
            itoa(temp, mem_higher);
            term_printstr(temp);
            term_printstr("\n");
        } else {
            term_printstr("Multiboot: Basic Memory Info --NOT-- Available.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_BOOTDEV)  {
            term_printstr("Multiboot: A boot device is set.\n");
        } else {
            term_printstr("Multiboot: A boot device is --NOT-- set.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_CMDLINE)  {
            term_printstr("Multiboot: There is a command line defined.\n");
        } else {
            term_printstr("Multiboot: There is --NO-- command line defined.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_MODS)  {
            term_printstr("Multiboot: There are modules.\n");
        } else {
            term_printstr("Multiboot: There are --NO-- modules.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_AOUT_SYMS)  {
            term_printstr("Multiboot: There is an a.out symbol table.\n");
        } else {
            term_printstr("Multiboot: There is --NO-- a.out symbol table.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_ELF_SHDR)  {
            term_printstr("Multiboot: There is an ELF section header table.\n");
        } else {
            term_printstr("Multiboot: There is --NO-- ELF section header table.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_MEM_MAP)  {
            term_printstr("Multiboot: There is a full memory map available.\n");
        } else {
            term_printstr("Multiboot: There is --NO-- full memory map available.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_DRIVE_INFO)  {
            term_printstr("Multiboot: There is drive info.\n");
        } else {
            term_printstr("Multiboot: There is --NO-- drive info.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_CONFIG_TABLE)  {
            term_printstr("Multiboot: There is a config table.\n");
        } else {
            term_printstr("Multiboot: There is --NO-- config table.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME)  {
            term_printstr("Multiboot: There is a boot loader name.\n");
        } else {
            term_printstr("Multiboot: There is --NO-- boot loader name.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_APM_TABLE)  {
            term_printstr("Multiboot: There is an APM table.\n");
        } else {
            term_printstr("Multiboot: There is --NO-- APM table.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_VBE_INFO)  {
            term_printstr("Multiboot: There is VBE information.\n");
        } else {
            term_printstr("Multiboot: There is --NO-- VBE information.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_FRAMEBUFFER_INFO)  {
            term_printstr("Multiboot: There is framebuffer information.\n");
        } else {
            term_printstr("Multiboot: There is --NO-- framebuffer information.\n");
        }
    } else {
        term_printstr("Multiboot Header Info is --NOT-- AVAILABLE. ERROR.!\n");
    }

    // Access multiboot header for information
//    if (mbd->flags & )

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



    char buffer[34];
    term_printstr("eax value: \n");
    itoa(buffer, cEax);
    term_printstr(buffer);
    term_printstr("\n");

    int b = strlen("hello");

    // eax should be 5!
    cEax = eax;
    term_printstr("eax value after: \n");
    itoa(buffer, cEax);
    term_printstr(buffer);
    term_printstr("\n");

    term_printstr("ebx value: \n");
    itoa(buffer, cEbx);
    term_printstr(buffer);
    term_printstr("\n");

    term_printstr("ecx value: \n");
    itoa(buffer, cEcx);
    term_printstr(buffer);
    term_printstr("\n");

    term_printstr("esp value: \n");
    itoa(buffer, cEsp);
    term_printstr(buffer);
    term_printstr("\n");
    /*
    for (int i = 0; i < 105; i++) {
        term_printstr("This is row: ");
        char buffer[4];
        itoa(buffer, i);
        term_printstr(buffer);
        term_printstr("\n");

        // sleep for a sec
        busysleep_tiny();

    }
    */
}
