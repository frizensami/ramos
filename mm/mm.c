#include "mm.h"

void mm_init(multiboot_info_t* mbd, unsigned int magic)
{
    printf("Accessing Multiboot Info...\n");
    if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
        if (mbd->flags & MULTIBOOT_INFO_MEM_MAP) {
            printf("\nInitial System Memory Map:\n");

            multiboot_memory_map_t* mmap = (multiboot_memory_map_t*)mbd->mmap_addr;
            while ((uint32_t)mmap < mbd->mmap_addr + mbd->mmap_length) {

                // This weird size addition to the pointer has to be done sinze the size var is
                // "outside" of the memory map structure size
                mmap = (multiboot_memory_map_t*)((unsigned int)mmap + mmap->size + sizeof(mmap->size));

                // Needs conversion to (int) to be accepts into printf - only supports 32 bit
                // But Multiboot gives it to us as 64 bit

                if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
                    printf("0x%08X -- 0x%08X (%d MB or %d KB) : %s \n", (uint32_t)mmap->addr, ((uint32_t)(mmap->addr + mmap->len)), (uint32_t)mmap->len >> 20, (uint32_t)mmap->len >> 10, "Available");
                } else {
                    printf("0x%08X -- 0x%08X (%d MB or %d KB) : %s \n", (uint32_t)mmap->addr, ((uint32_t)(mmap->addr + mmap->len)), (uint32_t)mmap->len >> 20, (uint32_t)mmap->len >> 10, "Reserved");
                }
            }
            printf("\n");
        } else {
            printf("Multiboot: There is --NO-- full memory map available.\n");
            die(NULL);
        }

    } else {
        printf("Multiboot Header Info is --NOT-- AVAILABLE. ERROR.!\n");
        die(NULL);
    }
}