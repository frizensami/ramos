#include "../interrupts/isr.h"
#include "../../lib/libk.h"

void keyboard_init(void);
char* get_key_buffer(void);
uint32_t get_key_buffer_idx(void);
uint32_t get_key_buffer_size(void);