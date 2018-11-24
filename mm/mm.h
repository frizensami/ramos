#ifndef MM_H
#define MM_H

#include "multiboot/multiboot.h"
#include "../lib/libk.h"

#define HEAP_REGION_USED 0
#define HEAP_REGION_FREE 1
struct heap_node {
    // region-start isn't required, but i'm keeping it for mental convenience for now
    // TODO: refactor region_start value from uint32_t
    uint32_t region_start;
    uint32_t region_size_bytes;
    uint8_t region_type;
    struct heap_node* next_region;
    struct heap_node* prev_region;
};

void mm_init(multiboot_info_t* mbd, unsigned int magic);
void init_heap(void);
void print_heap_info(void);
void print_region(struct heap_node*);
void* malloc(uint32_t);

#endif