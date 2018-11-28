#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../interrupts/isr.h"
#include "../../lib/libk.h"

#define RIGHT_ARROW_KEY 14
#define LEFT_ARROW_KEY 15
#define UP_ARROW_KEY 17
#define DOWN_ARROW_KEY 18

int is_arrow_key(char*);
void keyboard_init(void);
char* get_key_buffer(void);
uint32_t get_key_buffer_idx(void);
uint32_t get_key_buffer_size(void);


#endif