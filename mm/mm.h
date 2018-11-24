#ifndef MM_H
#define MM_H

#include "multiboot/multiboot.h"
#include "../lib/libk.h"

void mm_init(multiboot_info_t* mbd, unsigned int magic);

#endif