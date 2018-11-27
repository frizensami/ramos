#include "kterm.h"

#define KTERM_BUFFER_CHARS 100

char kterm_buffer[KTERM_BUFFER_CHARS];
char command_string[KTERM_BUFFER_CHARS];
uint32_t kterm_buffer_idx = 0;
uint32_t command_read_idx = 0;

void start_kterm(void)
{
    // Handle keyboard
    uint32_t local_key_buffer_idx  = 0;
    uint32_t key_buffer_size = get_key_buffer_size();
    char* key_buffer = get_key_buffer();

    print_prompt();

    while (1) {
        if (local_key_buffer_idx != get_key_buffer_idx()) {
            char cur_char = key_buffer[local_key_buffer_idx];
            local_key_buffer_idx = (local_key_buffer_idx + 1) % key_buffer_size;

            switch (cur_char) {
                case '\n':
                    {
                        handle_input();
                        print_prompt();
                        break;
                    }
                default:
                    {
                        printf("%c", cur_char);
                        if (cur_char != '\b') {
                            kterm_buffer[kterm_buffer_idx] = cur_char;
                            kterm_buffer_idx = (kterm_buffer_idx + 1) % KTERM_BUFFER_CHARS;
                        } else {
                            kterm_buffer_idx = (kterm_buffer_idx - 1) % KTERM_BUFFER_CHARS;
                            kterm_buffer[kterm_buffer_idx] = ' ';
                        }
                    }
            }
        }
    }
}

void print_prompt(void) {
    printf("kterm@ramos >> ");
}

void handle_input(void) {
    // Grab all input from command buffer and print
    // printf("\nCOMMAND RECEIVED: ");
    
    int i = 0;
    while (command_read_idx != kterm_buffer_idx) {
        command_string[i] = kterm_buffer[command_read_idx];
        command_read_idx = (command_read_idx + 1) % KTERM_BUFFER_CHARS;
        i++;
    }

    command_string[i] = '\0';
    // printf("%s\n", command_string);

    handle_command_string();
}

void handle_command_string() {
    printf("\n");
    if (strcmp(command_string, "ls") == 0) {
        printf("No filesystems in this kernel-land, sorry.\n");
    } else if (strcmp(command_string, "cd") == 0) {
        printf("No filesystems in this kernel-land, sorry.\n");
    } else if (strcmp(command_string, "clear") == 0) {
        term_clear();
        term_pointer_reset();
    } else {
        int command_list_elems = sizeof(command_list) / sizeof(struct kterm_command);
        for (int i = 0; i < command_list_elems; i++) {
            char* command = command_list[i].command_string;
            if (starts_with(command_string, command)) {
                // Passing in this string will be a problem once we can run these processes in 
                // parallel - this is a shared buffer that might be overwritten
                return command_list[i].command_handler((command_string + strlen(command)));
            }
        }
        printf("%s: Command not recognized.\n", command_string);
    }
}
