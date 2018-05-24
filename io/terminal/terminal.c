/* terminal.c - VGA terminal handling methods */
/* Copyright (C) 2018, Sriram Sami.
      *
      *  Permission is hereby granted, free of charge, to any person obtaining a copy
      *  of this software and associated documentation files (the "Software"), to
      *  deal in the Software without restriction, including without limitation the
      *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
      *  sell copies of the Software, and to permit persons to whom the Software is
      *  furnished to do so, subject to the following conditions:
      *
      *  The above copyright notice and this permission notice shall be included in
      *  all copies or substantial portions of the Software.
      *
      *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
      *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
      *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL ANY
      *  DEVELOPER OR DISTRIBUTOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
      *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
      *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
      */
     
#include "terminal.h"

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

// Set up initial state of the terminal to point to col = 0, row = 0 (top left)
// Also pass it the vga buffer pointer
terminal_state term_state;

// Reset the terminal pointer to the top left (row = 0, col = 0)
void term_pointer_reset(void) {
    term_state.current_col = 0;
    term_state.current_row = 0;
}

// Given the row and col we want to print to, give us a linear index into the 1D buffer
size_t compute_term_index(int col, int row) {
    return (VGA_COLS * row) + col;
}

// Use the current state structure to compute the current index
size_t compute_term_index_from_state(void) {
    return compute_term_index(term_state.current_col, term_state.current_row);
}

// Given the color of the terminal we want at this point + character we want to print
// Output the 2 byte VGA value
uint16_t get_vga_character(uint8_t term_color, char character) {
    return (((uint16_t)term_color << 8) | character);
}

// Go through every index in the VGA buffer and set the character there to a space
void term_clear(void) {
    for (int row = 0; row < VGA_ROWS; row++) {
        for (int col = 0; col < VGA_COLS; col++) {
            // Get index of this particular iteration
            const size_t index = compute_term_index(col, row);
            // Set the character in this area to be blank
            term_state.buffer[index] = get_vga_character(BLACK_BG_WHITE_FG, ' ');
        }
    }
}

void term_clear_row(int row) {
    for (int col = 0; col < VGA_COLS; col++) {
        const size_t current_index = compute_term_index(col, row);
        term_state.buffer[current_index] = get_vga_character(BLACK_BG_WHITE_FG, ' ');
    }
}

// Removes the first line of text in the terminal and moves all other lines 
// up by one line. Terminal cursor is set at the bottom left at the end.
void term_scroll(void) {
    // We only iterate through all rows except last. The last row 
    // will be cleared. Set the current character to be the same as the 
    // character in the same column but the next row, effectively shifting the row up.
    for (int row = 0; row < (VGA_ROWS - 1); row++) {
        // Clear the row before scrolling
        term_clear_row(row);
        for (int col = 0; col < VGA_COLS; col++) {
            // Clear the current row before we start copying stuff inside
            // Get index of this particular iteration
            const size_t current_index = compute_term_index(col, row);
            const size_t next_row_index = compute_term_index(col, row + 1);
            // Set the character in this area to be blank
            term_state.buffer[current_index] = term_state.buffer[next_row_index];
        }
    }

    // clear last row
    term_clear_row(VGA_ROWS - 1);

    // Reset the terminal pointer to the last row
    term_state.current_col = 0;
    term_state.current_row = VGA_ROWS - 1;
}

// Put a single character into the VGA buffer
void term_putchar(char c) {
    switch(c) {
        case '\n':
            {
                // For a newline, we need to set the row back to 0, and increment the column
                term_state.current_col = 0;
                term_state.current_row++;
                break;
            }
        default:
            {
                // Otherwise, we just print the character on the screen and advance the column
                // Precondition: we are not at the end of this column
                const size_t index = compute_term_index_from_state();
                term_state.buffer[index] = get_vga_character(BLACK_BG_WHITE_FG, c);
                term_state.current_col++;
                break;
            }
    }

    // Now that we have modified the terminal pointers, check if we need to handle:
    // 1. We have come to the end of the column and we need to increment the row
    // 2. We have come to the end of the available rows

    // Case 1
    if (term_state.current_col >= VGA_COLS) {
        term_state.current_col = 0;
        term_state.current_col++;
    }

    // Case 2
    if (term_state.current_row >= VGA_ROWS) {
        // For now - no scrolling. Just reset pointer
        // term_pointer_reset(term_state);
        
        // Upgrade: scroll the terminal
        term_scroll();
    }
}

// Put an entire string into the buffer
void term_printstr(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        term_putchar(str[i]);
    }
}



// Clear the screen, and reset the pointer
void term_init(void) {
    term_state.buffer = vga_buffer;
    term_clear();
    term_pointer_reset();
}
