// GCC provides these header files automatically
// They give us access to useful things like fixed-width types
#include <stddef.h>
#include <stdint.h>
#include "terminal.c"


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


void kernel_main() {
    // This will be called by start.s to start up the kernel
    // Remember: we only have 4K stack space at this point!

    // Initialize the terminal
    term_init();
    
    // Say something!
    term_printstr("Hello world, from RamOS!\n");

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
