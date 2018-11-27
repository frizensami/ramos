/* libk.c - Minimal kernel C library for essential functions */
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
#include "libk.h"

/* Kernel library strlen - count until null byte */
int strlen(const char* s) {
    int i = 0;
    while (s[i] != '\0') i++;
    return i;
}


/* Compare S1 and S2, returning less than, equal to or
   greater than zero if S1 is lexicographically less than,
   equal to or greater than S2. 
   Src: https://code.woboq.org/userspace/glibc/string/strcmp.c.html */
int strcmp (const char *p1, const char *p2)
{
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
        return c1 - c2;
    }
  while (c1 == c2);
  return c1 - c2;
}

/* strlstrip: String leading whitespace from a string */
char* strlstrip(const char* str) 
{
    // Iterate until we don't see a space
    char* ptr = str;
    while(*ptr == ' ') {
        ptr++;
    }
    return ptr;
}


/* starts_with: checks if string begins with prefix */
int starts_with(const char *string, const char *prefix)
{
    while(*prefix)
    {
        if(*prefix++ != *string++)
            return 0;
    }

    return 1;
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
/* This is the K&R standard */
void itoa(char s[], uint64_t n)
{
    uint64_t i, sign;

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


// A utility function to check whether x is numeric 
int isNumericChar(char x) 
{ 
    return (x >= '0' && x <= '9')? 1 : 0; 
} 

// A utility function to check whether x is numeric 
int isHexChar(char x) 
{ 
    return (isNumericChar(x) || (x >= 'A' && x <= 'F') || (x >= 'a' && x <= 'f')) ? 1 : 0; 
} 
   
// A simple atoi() function. If the given string contains 
// any invalid character, then this function returns INT_MAX 
// https://www.geeksforgeeks.org/write-your-own-atoi/
int atoi(char *str) 
{ 
    int res = 0;  // Initialize result 
    int sign = 1;  // Initialize sign as positive 
    int i = 0;  // Initialize index of first digit 
   
    // If number is negative, then update sign 
    if (str[0] == '-') 
    { 
        sign = -1; 
        i++;  // Also update index of first digit 
    } 
   
    // Iterate through all digits of input string and update result 
    for (; str[i] != '\0'; ++i) 
    { 
        if (!isNumericChar(str[i]))  {
            return INT_MAX;
        }
        res = res*10 + str[i] - '0'; 
    } 
   
    // Return result with sign 
    return sign*res; 
} 

int hex_str_to_int(char* str) 
{
    int res = 0;  // Initialize result 
    int sign = 1;  // Initialize sign as positive 
    int i = 0;  // Initialize index of first digit 
   
    // If number is negative, then update sign 
    if (str[0] == '-') 
    { 
        sign = -1; 
        i++;  // Also update index of first digit 
    } 

    // Check that the next 2 chars are 0x
    if (str[i] != '0' || str[i+1] != 'x')  {
        return INT_MAX;
    }
    // Move first digit checking
    i += 2;
   
    // Iterate through all digits of input string and update result 
    for (; str[i] != '\0'; ++i) 
    { 
        if (!isHexChar(str[i]))  {
            printf("Not a hex character: %c, idx: %d\n", str[i], i);
            return INT_MAX;
        }
        
        char c = str[i];
        int val = 0;

        if (c >= '0' && c <= '9') val = c - '0';
        else if (c >= 'a' && c <='f') val = c - 'a' + 10;
        else if (c >= 'A' && c <='F') val = c - 'A' + 10;    

        res = (res << 4) + val; 
    } 
   
    // Return result with sign 
    return sign*res; 
}


/* Sleep for a noticeable amount of time - best we can do without a timer for now */
void busysleep_tiny(void) {
    // sleep?
    for (int i = 0; i < 10000000; i++) {
        int a = 3;
        a++;
    }
}

void* memset(void* s, int c, size_t n)
{
    unsigned char* p = s;
    while(n--)
        *p++ = (unsigned char)c;
    return s;
}

void print_registers() {
    int esp; asm ("movl %%esp, %0" : "=r" (esp) );
    int ebp; asm ("movl %%ebp, %0" : "=r" (ebp) );
    int eax; asm ("movl %%eax, %0" : "=r" (eax) );
    int ebx; asm ("movl %%ebx, %0" : "=r" (ebx) );
    int ecx; asm ("movl %%ecx, %0" : "=r" (ecx) );
    int edx; asm ("movl %%edx, %0" : "=r" (edx) );
    int esi; asm ("movl %%esi, %0" : "=r" (esi) );
    int edi; asm ("movl %%edi, %0" : "=r" (edi) );

    printf("--- Register Values ---\nEAX: 0x%X\tEBX: 0x%X\tECX: 0x%X\tEDX: 0x%X\n", eax, ebx, ecx, edx);
    printf("ESI: 0x%X\tEDI: 0x%X\n", esi, edi);
    printf("ESP: 0x%X\tEBP: 0x%X\t", esp, ebp);

}

void die(char* custom_message)
{
    printf("\n********************* FATAL BUG :: CPU HALTED *********************\n");
    if (custom_message != NULL && strlen(custom_message) > 0) {
        printf("Error Message: %s\n", custom_message);
    }
    print_registers();
    asm("hlt");
}