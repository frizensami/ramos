#include "mm.h"

// We are going to add this to our heap_start as detected by GRUB 
// to start our heap memory allocations from here
#define ASSUMED_KERNEL_SIZE_WITHOUT_HEAP 0x100000

// Track if the memory managed has successfully initialized
uint8_t initialized = 0;

void* heap_start = NULL;
void* heap_end = NULL;
uint32_t heap_size = 0;

uint8_t found_first_free_partition = 0;

// First-fit linked-list memory allocator
// We linearly search the region list until we find a free one that matches our requested size.
// A NULL pointer next_region indicates the end of available memory



// Check multiboot header for memory info, grab an area to use as the heap
// Requires an assumption of how much memory the kernel is going to take without the heap allocs.
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

                    if (!found_first_free_partition) {
                        heap_start = (void*)(mmap->addr + ASSUMED_KERNEL_SIZE_WITHOUT_HEAP);
                        heap_end = (void*)(heap_start + mmap->len - ASSUMED_KERNEL_SIZE_WITHOUT_HEAP);
                        heap_size = (uint32_t)(heap_end - heap_start);
                        found_first_free_partition = 1;
                    }
                } else {
                    printf("0x%08X -- 0x%08X (%d MB or %d KB) : %s \n", (uint32_t)mmap->addr, ((uint32_t)(mmap->addr + mmap->len)), (uint32_t)mmap->len >> 20, (uint32_t)mmap->len >> 10, "Reserved");
                }
            }

            init_heap();

            printf("\n");
            initialized = 1;

        } else {
            printf("Multiboot: There is --NO-- full memory map available.\n");
            die(NULL);
        }

    } else {
        printf("Multiboot Header Info is --NOT-- AVAILABLE. ERROR.!\n");
        die(NULL);
    }
}

// This was a tough one conceptually. I need to put things into the heap, but I'm the one
// creating malloc. Ref: https://stackoverflow.com/questions/15592721/creating-a-struct-at-address-pointed-by-pointer
void init_heap()
{
    // Select heap region
    printf("REGION SELECTED FOR OS HEAP: 0x%08X -- 0x%08X (%d MB or %d KB)\n", heap_start, heap_end, heap_size >> 20, heap_size >> 10);
    
    // We need to place our first heap tracking block
    // The available size of the remaining heap should be the total size minus this node size
    struct heap_node* head_node = (struct heap_node *)(heap_start);  
    // heap_start refers to the first unallocated byte. Available space usage starts from 
    // that offset plus the size of this tracking struct
    head_node->region_start = heap_start + sizeof(struct heap_node);
    head_node->region_size_bytes = heap_size - sizeof(struct heap_node);
    head_node->region_type = HEAP_REGION_FREE;
    head_node->next_region = NULL;
    head_node->prev_region = NULL;
}

void print_heap_info() 
{
    printf("Current Heap Status:\n");
    printf("True heap start address:  0x%08x, sizeof(heap_node): %d bytes\n", heap_start, sizeof(struct heap_node));
    struct heap_node* next_region = (struct heap_node *)heap_start;
    while (next_region != NULL) {
        print_region(next_region);
        next_region = next_region->next_region;
    }
    printf("\n");
}

void print_region(struct heap_node* region)
{
    printf("%s: 0x%08X - 0x%08X (%u bytes)\n", 
        region->region_type == HEAP_REGION_FREE ? "FREE" : "USED", 
        (uint32_t) region->region_start, 
        (uint32_t) region->region_start + (uint32_t) region->region_size_bytes, 
        (uint32_t) region->region_size_bytes);
}

/*
    Malloc Implementation: 
    Linearly search the heap regions until we find a region with sufficient space.
    Then, split that region into USED -> FREE
*/
void* malloc(uint32_t bytes_requested) 
{
    // We need space to store the actual heap region header AND the requested space
    uint32_t total_space_needed = bytes_requested + sizeof(struct heap_node);

    // Find a block that has sufficient space to satify the request
    struct heap_node* next_region = (struct heap_node *)heap_start;
    while (next_region != NULL) {
        if (next_region->region_size_bytes >= bytes_requested && 
            next_region->region_type == HEAP_REGION_FREE) {
                // We have found a block that works. 
                // Split this existing block into two parts, a USED part and a FREE part.
                uint32_t current_region_size = next_region->region_size_bytes;
                uint32_t new_free_region_size = current_region_size - total_space_needed;

                // Start changing this current region to mark it as used
                next_region->region_type = HEAP_REGION_USED;
                next_region->region_size_bytes = bytes_requested;

                // Set up the new FREE block
                struct heap_node* new_region = (struct heap_node *) (next_region->region_start + bytes_requested); 
                new_region->region_start = ((uint32_t) new_region) + ((uint32_t) sizeof(struct heap_node));
                new_region->region_size_bytes = new_free_region_size;
                new_region->region_type = HEAP_REGION_FREE;
                new_region->next_region = new_region->next_region;
                new_region->prev_region = next_region;

                // Point the now USED block to the rest of the FREE block
                next_region->next_region = new_region;

                return next_region->region_start;
            }
        
        next_region = next_region->next_region;
    }

    // If we have reached here, no available space for this request
    // Return null pointer
    return NULL;
}

void free(void* heap_object) {
    // Find the header object of this heap address
    struct heap_node* cur_node = (struct heap_node * ) (((uint32_t) heap_object) - sizeof(struct heap_node));

    // Sanity check
    if (cur_node->region_start != ((uint32_t) heap_object)) {
        printf("Pointer 0x%08X passed to free is invalid!\n", (uint32_t) heap_object);
        printf("Arg: 0x%08X | Expected: 0x%08X\n", (uint32_t) heap_object, cur_node->region_start);
        die(NULL);
    }

    cur_node->region_type = HEAP_REGION_FREE;

    // Algorithm: check previous and next nodes to see if they are free
    // Merge free nodes with current node
    // Do this until both left and right are either occupied or NULL
    // Recursion would be more elegant, but we have limited stack space

    struct heap_node* prev_node = cur_node->prev_region;
    struct heap_node* next_node = cur_node->next_region;

    while (prev_node != NULL && prev_node->region_type == HEAP_REGION_FREE) {
        // Merge this node with current node
        // Increase size of previous node to make this entire space free
        prev_node->region_size_bytes += cur_node->region_size_bytes + sizeof(struct heap_node);

        // Point the previous node to our next node
        prev_node->next_region = cur_node->next_region;

        // Set our "target node for merging (cur_node)" to our newly merged block
        cur_node = prev_node;

        // Create another candidate previous node to check
        prev_node = prev_node->prev_region;
    }

    while (next_node != NULL && next_node->region_type == HEAP_REGION_FREE) {
        // Merge this node with current node
        // Increase size of this node (we are left-merging with the next node)
        cur_node->region_size_bytes += next_node->region_size_bytes + sizeof(struct heap_node);

        // Point our node to the region after our next node
        cur_node->next_region = next_node->next_region;

        // Create another candidate previous node to check
        next_node = next_node->next_region;
    }
}