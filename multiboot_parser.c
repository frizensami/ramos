/* multiboot_parser.c - Multiboot header parser. */
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


#include "multiboot_parser.h"
#include "libk.c"
#include "terminal.h"

void print_multiboot_info(multiboot_info_t* mbd, unsigned int magic) {
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


}

