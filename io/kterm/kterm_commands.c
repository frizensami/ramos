#include "kterm_commands.h"

struct kterm_command command_list[] = {
    { .command_string = PROG_MALLOC_COMMAND_STRING, .command_handler = &malloc_command_handler},
    { .command_string = PROG_FREE_COMMAND_STRING, .command_handler = &free_command_handler}
};

