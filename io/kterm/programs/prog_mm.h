#ifndef PROG_MM_H
#define PROG_MM_H

#include "../../../lib/libk.h"
#define PROG_MALLOC_COMMAND_STRING "malloc"
int malloc_command_handler(char*);

#define PROG_FREE_COMMAND_STRING "free"
int free_command_handler(char*);

#endif