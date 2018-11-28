#include "kterm.h"

#define KTERM_BUFFER_CHARS 100

char kterm_buffer[KTERM_BUFFER_CHARS];
char command_string[KTERM_BUFFER_CHARS];
uint32_t kterm_buffer_idx = 0;
uint32_t command_read_idx = 0;

// Description of buffer system:
// [ a b c d \b \b \n f** g h* _ _ _ ] <--- key buffer straight from keyboard, * is the key buffer idx
// This file has a local_key_buffer index (indicated by **) which is where kterm is currently reading
// kterm constantly attempts to catch up with the key buffer idx

// [ a b c d f g h* _ _ _ _ _ _ ] <---- kterm buffer (no backspaces or newlines), w/ kterm_buffer_idx
// Copying of the characters to the command buffer is triggered on newlines

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
                        // reset terminal index and command index
                        // since we've already processed this command
                        kterm_buffer_idx = 0;
                        command_read_idx = 0;
                        break;
                    }
                default:
                    {
                        if (cur_char == '\b') {
                            if (kterm_buffer_idx != command_read_idx) {
                                printf("%c", cur_char);
                                kterm_buffer_idx = (kterm_buffer_idx - 1) % KTERM_BUFFER_CHARS;
                                kterm_buffer[kterm_buffer_idx] = ' ';
                            }
                        } else if (is_arrow_key(cur_char)) {
                            if (cur_char == UP_ARROW_KEY) {

                            } else if (cur_char == DOWN_ARROW_KEY) {

                            } else if (cur_char == LEFT_ARROW_KEY) {

                            } else if (cur_char == RIGHT_ARROW_KEY) {

                            }
                        } else if (!isprintable(cur_char)) {
                            // Current character is not printable 
                            // and does not fall into previous categories. Ignore.
                        } else {
                            printf("%c", cur_char);
                            kterm_buffer[kterm_buffer_idx] = cur_char;
                            kterm_buffer_idx = (kterm_buffer_idx + 1) % KTERM_BUFFER_CHARS;
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

    if (command_read_idx == kterm_buffer_idx) {
        // No input entered
        printf("\n");
        return;
    }
    
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
            const char* command = command_list[i].command_string;
            // If the given command is "malloc": 
            // Allowed match examples: "malloc", "malloc 2", "malloc 2 3 4"
            // Disallowed examples: "malloc-2", "malloc2"

            // Strategy: try to directly match the command (no arguments)
            // If that fails, attempt to prefix-match the command_string with our intended command
            // If that succeeds: we want the next character after the prefix match to be a space
            if (strcmp(command_string, command) == 0 || (starts_with(command_string, command) && command_string[strlen(command)] == ' ')) 
            {
                // Passing in this string will be a problem once we can run these processes in 
                // parallel - this is a shared buffer that might be overwritten
                // We pass in everything except the command_string we were asked to match
                int retval = command_list[i].command_handler(strlstrip(command_string + strlen(command)));

                if (retval) {
                    printf("kterm: Error in '%s' command\n", command);
                }
                return;
            }
        }
        // Fallthrough: we didn't find a valid command
        printf("%s: Command not recognized.\n", command_string);
    }
}
