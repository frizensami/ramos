#ifndef KTERM_COMMANDS_H
#define KTERM_COMMANDS_H

#include "../../lib/common.h"
#include "programs/prog_mm.h"

typedef int (*Command_Handler)(char* args);
struct kterm_command {
    // A string that kterm will try to prefix-match to the incoming command
    // This means that command_string can theoretically have more than one word
    const char* command_string;
    // Function pointer for a function that can handle incoming args from kterm
    Command_Handler command_handler;
};

#define KTERM_COMMAND_LIST_NUM_ELEMS 2
extern struct kterm_command command_list[KTERM_COMMAND_LIST_NUM_ELEMS];

#endif