# Adding Commands to the KTERM Subsystem
1. The commands you create must expose two pieces of information in their header file(s) for each command. 
- `#define PROG_*_COMMAND_STRING <string that will be prefix-matched by kterm to call your command>`
- `int *_command_handler(char* args)`: The function that will be called if the user enters your command

As an example, for memory management commands, `prog_mm.c` contains `int malloc_command_handler(char* args)` and `int free_command_handler(char* args)`. `prog_mm.h` contains the declarations for both these commands, and 2 defines: e.g. `#define PROG_MALLOC_COMMAND_STRING "malloc"` and `#define PROG_FREE_COMMAND_STRING "free"`

2. These files should be placed in the `io/kterm/programs` folder
3. `kterm_commands.h` should be modified to include your header files
4. `KTERM_COMMAND_LIST_NUM_ELEMS` in `kterm_commmands.h` should be increased by the number of commands you are adding
5. `kterm_commands.c`'s `command_list` array should be modified to include your command's details. 