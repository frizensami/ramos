#ifndef KTERM_COMMANDS_H
#define KTERM_COMMANDS_H

#include "../../lib/common.h"
#include "programs/prog_malloc.h"

typedef int (*Command_Handler)(char* args);
struct kterm_command {
    // A string that kterm will try to prefix-match to the incoming command
    // This means that command_string can theoretically have more than one word
    const char* command_string;
    // Function pointer for a function that can handle incoming args from kterm
    Command_Handler command_handler;
};

struct kterm_command command_list[] = {
    { .command_string = PROG_MALLOC_COMMAND_STRING, .command_handler = &malloc_command_handler}
};

#endif