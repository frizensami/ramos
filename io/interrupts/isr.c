#include "isr.h"
#include "../../lib/printf.h"

void isr_handler(registers_t regs)
{
   printf("Recieved interrupt: %d\n", regs.int_no);
}