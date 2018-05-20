// GCC provides these header files automatically
// They give us access to useful things like fixed-width types
#include <stddef.h>
#include <stdint.h>


// Do some basic checking on this code so that it's used correctly
#if defined(__linux__)
    #error "This code must be compiled with a cross compiler!"
#elif !defined(__i386__)
    #error "This code must be compiled with an x86-elf compiler!"
#endif

/****
 * VGA BUFFER CONSTANTS
 ****/

// Since we are in protected mode, and not using the BIOS to write to the screen..
// We can access the "text-screen video memory for color monitors" directly at this memory address
volatile uint16_t* vga_buffer = (uint16_t*)0xB8000;
// GRUB/etc will have set up a 80x25 VGA mode buffer for us.
// We are defining the buffer size constants to match that
const int VGA_COLS = 80;
const int VGA_ROWS = 25;
// Color definition for a black background terminal with a white foreground (text color)
const int BLACK_BG_WHITE_FG = 0x0F;


// Set up a terminal state tracking struct
typedef struct terminal_state {
    // Indicates the current column and row that we are pointing to in the terminal
    int current_col;
    int current_row;
    volatile uint16_t* buffer;
} terminal_state;



// Reset the terminal pointer to the top left (row = 0, col = 0)
void term_pointer_reset(terminal_state* term_state) {
    term_state->current_col = 0;
    term_state->current_row = 0;
}

// Given the row and col we want to print to, give us a linear index into the 1D buffer
size_t compute_term_index(int col, int row) {
    return (VGA_COLS * row) + col;
}

// Use the current state structure to compute the current index
size_t compute_term_index_from_state(terminal_state* term_state) {
    return compute_term_index(term_state->current_col, term_state->current_row);
}

// Given the color of the terminal we want at this point + character we want to print
// Output the 2 byte VGA value
uint16_t get_vga_character(uint8_t term_color, char character) {
    return (((uint16_t)term_color << 8) | character);
}

// Go through every index in the VGA buffer and set the character there to a space
void term_clear(terminal_state* term_state) {
    for (int row = 0; row < VGA_ROWS; row++) {
        for (int col = 0; col < VGA_COLS; col++) {
            // Get index of this particular iteration
            const size_t index = compute_term_index(col, row);
            // Set the character in this area to be blank
            term_state->buffer[index] = get_vga_character(BLACK_BG_WHITE_FG, ' ');
        }
    }
}

void term_clear_row(terminal_state* term_state, int row) {
    for (int col = 0; col < VGA_COLS; col++) {
        const size_t current_index = compute_term_index(col, row);
        term_state->buffer[current_index] = get_vga_character(BLACK_BG_WHITE_FG, ' ');
    }
}

// Removes the first line of text in the terminal and moves all other lines 
// up by one line. Terminal cursor is set at the bottom left at the end.
void term_scroll(terminal_state* term_state) {
    // We only iterate through all rows except last. The last row 
    // will be cleared. Set the current character to be the same as the 
    // character in the same column but the next row, effectively shifting the row up.
    for (int row = 0; row < (VGA_ROWS - 1); row++) {
        // Clear the row before scrolling
        term_clear_row(term_state, row);
        for (int col = 0; col < VGA_COLS; col++) {
            // Clear the current row before we start copying stuff inside
            // Get index of this particular iteration
            const size_t current_index = compute_term_index(col, row);
            const size_t next_row_index = compute_term_index(col, row + 1);
            // Set the character in this area to be blank
            term_state->buffer[current_index] = term_state->buffer[next_row_index];
        }
    }

    // clear last row
    term_clear_row(term_state, VGA_ROWS - 1);

    // Reset the terminal pointer to the last row
    term_state->current_col = 0;
    term_state->current_row = VGA_ROWS - 1;
}

// Put a single character into the VGA buffer
void term_putchar(terminal_state* term_state, char c) {
    switch(c) {
        case '\n':
            {
                // For a newline, we need to set the row back to 0, and increment the column
                term_state->current_col = 0;
                term_state->current_row++;
                break;
            }
        default:
            {
                // Otherwise, we just print the character on the screen and advance the column
                // Precondition: we are not at the end of this column
                const size_t index = compute_term_index_from_state(term_state);
                term_state->buffer[index] = get_vga_character(BLACK_BG_WHITE_FG, c);
                term_state->current_col++;
                break;
            }
    }

    // Now that we have modified the terminal pointers, check if we need to handle:
    // 1. We have come to the end of the column and we need to increment the row
    // 2. We have come to the end of the available rows

    // Case 1
    if (term_state->current_col >= VGA_COLS) {
        term_state->current_col = 0;
        term_state->current_col++;
    }

    // Case 2
    if (term_state->current_row >= VGA_ROWS) {
        // For now - no scrolling. Just reset pointer
        // term_pointer_reset(term_state);
        
        // Upgrade: scroll the terminal
        term_scroll(term_state);
    }
}

// Put an entire string into the buffer
void term_printstr(terminal_state* term_state, const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        term_putchar(term_state, str[i]);
    }
}



// Clear the screen, and reset the pointer
void term_init(terminal_state* term_state) {
    term_clear(term_state);
    term_pointer_reset(term_state);
}


// itoa for numbers up to 99
void itoa(char* buf, int n) {
    if (n < 10) {
        buf[0] = n + '0';
        buf[1] = '\0';
    } else if (n >= 10) {
        buf[0] = (n / 10) + '0';
        buf[1] = (n % 10) + '0';
        buf[2] = '\0';
    }
}

void busysleep_tiny() {
    // sleep?
    for (int i = 0; i < 10000000; i++) {
        int a = 3;
        a++;
    }
}


void kernel_main() {
    // This will be called by start.s to start up the kernel
    // Remember: we only have 4K stack space at this point!

    // Set up initial state of the terminal to point to col = 0, row = 0 (top left)
    // Also pass it the vga buffer pointer
    terminal_state term_state = {0, 0, vga_buffer};

    // Initialize the terminal
    term_init(&term_state);
    
    // Say something!
    term_printstr(&term_state, "Hello world, from RamOS!\n");
    for (int i = 0; i < 105; i++) {
        term_printstr(&term_state, "This is row: ");
        char buffer[4];
        itoa(buffer, i);
        term_printstr(&term_state, buffer);
        term_printstr(&term_state, "\n");

        // sleep for a sec
        busysleep_tiny();

    }
}
