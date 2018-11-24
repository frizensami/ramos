#include "timer.h"

uint32_t current_PIT_timer_frequency = 0;
uint32_t ticks = 0;

static void timer_callback(registers_t regs)
{
   ticks++;
   // printf("Tick (int $32) - OS scheduler should run: %d\n", tick);

   if (ticks % current_PIT_timer_frequency == 0) {
      printf("1 second passed\n");
   }

}


void timer_init(uint32_t frequency)
{
   // Firstly, register our timer callback.
   register_interrupt_handler(IRQ0, &timer_callback);

   // The value we send to the PIT is the value to divide it's input clock
   // (1193180 Hz) by, to get our required frequency. Important to note is
   // that the divisor must be small enough to fit into 16-bits.
   uint32_t divisor = 1193180 / frequency;

   if (divisor > 65535) {
      printf("TIMER INIT BUG :: DIVISOR GIVEN IS %d, CANNOT EXCEED 65535 (min freq 19 Hz)\n", divisor);
      die(NULL);
   }

   // Set global timer frequency
   current_PIT_timer_frequency = frequency;
   ticks = 0;

   // Send the command byte.
   outb(0x43, 0x36);

   // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
   uint8_t l = (uint8_t)(divisor & 0xFF);
   uint8_t h = (uint8_t)((divisor>>8) & 0xFF);

   printf("Frequency of PIT requested: %d, Divisor: %d (%X), High: %X, Low: %X\n", frequency, divisor, divisor, l, h);

   // Send the frequency divisor.
   outb(0x40, l);
   outb(0x40, h);

   printf("Timer Initialization Complete!\n");
}

