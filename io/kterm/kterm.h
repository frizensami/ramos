#ifndef KTERM_H
#define KTERM_H

// Kernel terminal module

#include "../../lib/common.h"
#include "../../lib/libk.h"
#include "../keyboard/keyboard.h"
#include "../terminal/terminal.h"
#include "kterm_commands.h"

void start_kterm(void);
void print_prompt(void);
void handle_input(void);
void handle_command_string(void);

#endif