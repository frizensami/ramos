#include "keyboard.h"

/* KBDUS means US Keyboard Layout. This is a scancode table
*  used to layout a standard US keyboard. I have left some
*  comments in to give you an idea of what key is what, even
*  though I set it's array index to 0. You can change that to
*  whatever you want using a macro, if you wish! */
// From http://www.osdever.net/bkerndev/Docs/keyboard.htm
// http://www.cs.umd.edu/~hollings/cs412/s98/project/proj1/scancode with mods

char kbdus[81][2] =
     {
       {   0,0   } , // NOT DEFINED
       {   1,1   } , // ESC key
       { '1','!' } ,
       { '2','@' } ,
       { '3','#' } ,
       { '4','$' } ,
       { '5','%' } ,
       { '6','^' } ,
       { '7','&' } ,
       { '8','*' } ,
       { '9','(' } ,
       { '0',')' } ,
       { '-','_' } ,
       { '=','+' } ,
       {'\b','\b'} , // BACKSPACE
       {'\t','\t'} ,
       { 'q','Q' } ,
       { 'w','W' } ,
       { 'e','E' } ,
       { 'r','R' } ,
       { 't','T' } ,
       { 'y','Y' } ,
       { 'u','U' } ,
       { 'i','I' } ,
       { 'o','O' } ,
       { 'p','P' } ,
       { '[','{' } ,
       { ']','}' } ,
       {'\n','\n'} , // ENTER
       {   0,0   } ,
       { 'a','A' } ,
       { 's','S' } ,
       { 'd','D' } ,
       { 'f','F' } ,
       { 'g','G' } ,
       { 'h','H' } ,
       { 'j','J' } ,
       { 'k','K' } ,
       { 'l','L' } ,
       { ';',':' } ,
       {'\'','\''} ,
       { '`','~' } ,
       {   0,0   } , // LEFT SHIFT
       { '\\','|'} ,
       { 'z','Z' } ,
       { 'x','X' } ,
       { 'c','C' } ,
       { 'v','V' } ,
       { 'b','B' } ,
       { 'n','N' } ,
       { 'm','M' } ,
       { ',','<' } ,
       { '.','>' } ,
       { '/','?' } ,
       {   0,0   } , // RIGHT SHIFT
       { '*','*' } , // KEYPAD *
       {   0,0   } , // LEFT ALT
       { ' ',' ' } , // SPACEBAR
       {   0,0   } , // CAPSLOCK
       {   0,0   } , // 59: F1 key
       {   0,0   } , // F2 key
       {   0,0   } , // F3 key
       {   0,0   } , // F4 key
       {   0,0   } , // F5 key
       {   0,0   } , // F6 key
       {   0,0   } , // F7 key
       {   0,0   } , // F8 key
       {   0,0   } , // F9 key
       {   0,0   } , // F10 key
       {   0,0   } , // Num lock key
       {   0,0   } , // 69: Scroll lock key
       {   0,0   } , // HOME key
       { UP_ARROW_KEY, UP_ARROW_KEY } , // Up arrow: 17 == Device Control 1 (ASCII)
       {   0,0   } , // Page up
       { '-','-' } , // Keypad -?
       {  LEFT_ARROW_KEY, LEFT_ARROW_KEY  } , // Left arrow: 15 == SHIFT IN (ASCII)
       {   0,0   } , // ?
       {  RIGHT_ARROW_KEY, RIGHT_ARROW_KEY } , // Right arrow: 14 == SHIFT OUT (ASCII)
       { '+','+' } , // Keypad plus
       {   0,0   } , // End key
       {  DOWN_ARROW_KEY, DOWN_ARROW_KEY } , // Down arrow: 18 == Device Control 2 (ASCII)
   };


int shift_pressed = 0;

// THESE VALUES SHOULD BE LOCKED! RACE CONDITIONS POSSIBLE!
#define KEY_BUFFER_SIZE 50
char key_buffer[KEY_BUFFER_SIZE];
uint32_t key_buffer_idx = 0;

char* get_key_buffer(void) { return (char*) &key_buffer; }
uint32_t get_key_buffer_idx(void) { return key_buffer_idx; }
uint32_t get_key_buffer_size(void) { return KEY_BUFFER_SIZE; }
int is_arrow_key(char* key) { return (key == UP_ARROW_KEY || key == DOWN_ARROW_KEY || key == LEFT_ARROW_KEY || key == RIGHT_ARROW_KEY); }

static void keyboard_callback(registers_t regs)
{
    // printf("Received Keyboard Interrupt (33)\n");
    unsigned char scancode = inb(0x60);
    /* If the top bit of the byte we read from the keyboard is
    *  set, that means that a key has just been released */
    if (scancode & 0x80) {
        /* You can use this one to see if the user released the
        *  shift, alt, or control keys... */
        // printf("Some key was released\n");

        // Released scancodes are 128 higher than normal.
        scancode -= 128;

        if (scancode == 0x2A || scancode == 0x36) {
            shift_pressed = 0;
        }
    } else {
        /* Here, a key was just pressed. Please note that if you
        *  hold a key down, you will get repeated key press
        *  interrupts. */

        if (scancode == 0x2A || scancode == 0x36) {
            shift_pressed = 1;
        } else {
            /* Just to show you how this works, we simply translate
            *  the keyboard scancode into an ASCII value, and then
            *  display it to the screen. You can get creative and
            *  use some flags to see if a shift is pressed and use a
            *  different layout, or you can add another 128 entries
            *  to the above layout to correspond to 'shift' being
            *  held. If shift is held using the larger lookup table,
            *  you would add 128 to the scancode when you look for it */
            //printf("Key: %c\n", (kbdus[scancode][shift_pressed]));
            //printf("%c", (kbdus[scancode][shift_pressed]));
            key_buffer[key_buffer_idx] = kbdus[scancode][shift_pressed];
            key_buffer_idx = (key_buffer_idx + 1) % KEY_BUFFER_SIZE;
        }
    }
}

void keyboard_init(void) {

   // Firstly, register our timer callback.
   register_interrupt_handler(IRQ1, &keyboard_callback);

   // Just in case - enable interrupts
   printf("Keyboard Initialization Complete!\n");
}

