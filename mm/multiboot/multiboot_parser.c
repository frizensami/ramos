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
#include "../../lib/common.h"
#include "../../lib/libk.h"

#define MMAP_TYPE_AVAILABLE "AVAIL"
#define MMAP_TYPE_NOT_AVAILABLE "RESERVED"

void print_multiboot_info(multiboot_info_t* mbd, unsigned int magic) {
   printf("Accessing Multiboot Info...\n");

    if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
        printf("Multiboot Header Info is AVAILABLE!\n");

        if (mbd->flags & MULTIBOOT_INFO_MEMORY)  {
            printf("Multiboot: Basic Memory Info Available. Low: ");
            multiboot_uint32_t mem_lower = mbd->mem_lower;
            multiboot_uint32_t mem_higher = mbd->mem_upper;
            char temp[34];
            itoa(temp, mem_lower);
            printf(temp);
            printf(", High: ");
            itoa(temp, mem_higher);
            printf(temp);
            printf("\n");
        } else {
            printf("Multiboot: Basic Memory Info --NOT-- Available.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_BOOTDEV)  {
            printf("Multiboot: A boot device is set.\n");
        } else {
            printf("Multiboot: A boot device is --NOT-- set.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_CMDLINE)  {
            printf("Multiboot: There is a command line defined.\n");
        } else {
            printf("Multiboot: There is --NO-- command line defined.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_MODS)  {
            printf("Multiboot: There are modules.\n");
        } else {
            printf("Multiboot: There are --NO-- modules.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_AOUT_SYMS)  {
            printf("Multiboot: There is an a.out symbol table.\n");
        } else {
            printf("Multiboot: There is --NO-- a.out symbol table.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_ELF_SHDR)  {
            printf("Multiboot: There is an ELF section header table.\n");
        } else {
            printf("Multiboot: There is --NO-- ELF section header table.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_MEM_MAP)  {
            printf("Multiboot: There is a full memory map available.\n");

            multiboot_memory_map_t* mmap = (multiboot_memory_map_t*) mbd->mmap_addr;
            while((uint32_t)mmap < mbd->mmap_addr + mbd->mmap_length) {
                
                // This weird size addition to the pointer has to be done sinze the size var is 
                // "outside" of the memory map structure size
                mmap = (multiboot_memory_map_t*) ((unsigned int)mmap + mmap->size + sizeof(mmap->size));

                // Needs conversion to (int) to be accepts into printf - only supports 32 bit
                // But Multiboot gives it to us as 64 bit

                if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
                    printf("Mem Region Start: 0x%X | End: 0x%X | Len: 0x%X | Type: %s \n", (uint32_t)mmap->addr, ((uint32_t)(mmap->addr + mmap->len)), (uint32_t)mmap->len, MMAP_TYPE_AVAILABLE);
                } else {
                    printf("Mem Region Start: 0x%X | End: 0x%X | Len: 0x%X | Type: %s \n", (uint32_t)mmap->addr, ((uint32_t)(mmap->addr + mmap->len)), (uint32_t)mmap->len, MMAP_TYPE_NOT_AVAILABLE);
                }
            }
        } else {
            printf("Multiboot: There is --NO-- full memory map available.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_DRIVE_INFO)  {
            printf("Multiboot: There is drive info.\n");
        } else {
            printf("Multiboot: There is --NO-- drive info.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_CONFIG_TABLE)  {
            printf("Multiboot: There is a config table.\n");
        } else {
            printf("Multiboot: There is --NO-- config table.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_BOOT_LOADER_NAME)  {
            printf("Multiboot: There is a boot loader name.\n");
        } else {
            printf("Multiboot: There is --NO-- boot loader name.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_APM_TABLE)  {
            printf("Multiboot: There is an APM table.\n");
        } else {
            printf("Multiboot: There is --NO-- APM table.\n");
        }

        if (mbd->flags & MULTIBOOT_INFO_VBE_INFO)  {
            printf("Multiboot: There is VBE information.\n");
        } else {
            printf("Multiboot: There is --NO-- VBE information.\n");
        }

    } else {
        printf("Multiboot Header Info is --NOT-- AVAILABLE. ERROR.!\n");
    }


}

