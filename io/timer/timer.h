#include "../interrupts/isr.h"
#include "../../lib/libk.h"

// This file describes a PIT (Programmable Interrupt Timer)

// Frequency ranges from 19 Hz to 1193182 Hz, inclusive
void timer_init(uint32_t frequency);