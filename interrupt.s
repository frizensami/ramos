.global isr0
.global isr1
.extern isr_handler

isr0:
  cli                 # Disable interrupts
  push $0            # Push a dummy error code (if ISR0 doesn't push it's own error code)
  push $0            # Push the interrupt number (0)
  jmp isr_common_stub # Go to our common handler.

isr1:
  cli                 # Disable interrupts
  push $0            # Push a dummy error code (if ISR0 doesn't push it's own error code)
  push $1            # Push the interrupt number (1)
  jmp isr_common_stub # Go to our common handler.


# This is our common ISR stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.
isr_common_stub: 
   pusha                    # Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax

   mov %ds,%ax             # Lower 16-bits of eax = ds.
   push %eax               # save the data segment descriptor

   mov $0x10,%ax # load the kernel data segment descriptor
   mov %ax,%ds
   mov %ax,%es
   mov %ax,%fs
   mov %ax,%gs

   call isr_handler

   pop %eax      # reload the original data segment descriptor
   mov %ax,%ds
   mov %ax,%es
   mov %ax,%fs
   mov %ax,%gs

   popa                     # Pops edi,esi,ebp...
   add $8,%esp   # Cleans up the pushed error code and pushed ISR number
   iret           # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP
