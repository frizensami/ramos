/* libk.h - Minimal kernel C library for essential functions */
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
#ifndef LIBK_H
#define LIBK_H

#include "common.h"
#include "printf.h"
#include "asm.h"

int strlen(const char* s);
void reverse(char s[]);
void itoa(char s[], uint64_t n);
void busysleep_tiny(void);
// Fills the first n bytes of the memory area pointed to by s with the constant byte c.
void* memset(void* s, int c, size_t n);
// Kills the processor with a custom error message
void* die(char* custom_message);


#endif