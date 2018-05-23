# Export the gdt_flush function to replace the existing GDTR value with our own
.global gdt_flush

# Our argument to gdt_flush is a 32 bit pointer address that is passed onto the stack
# It's essentially pushed onto the stack with `push gdt_ptr`
# A "push" instruction internally does:
# sub esp, 4 and then mov [esp], X (in NASM syntax) 
# esp points to the FIRST FREE ADDRESS
# Essentially: subtract ("grow") 4 bytes from stack pointer, then move the value we want there

# Usual start sequence of any function is:
# 1. push ebp (saving the old ebp value - the frame pointer for the CALLER function - must restore!)
# 2. mov ebp, esp (ebp is now = esp) - ebp is our new frame pointer
# 3. We can now manipulate esp to grow the stack for our own local variables

# In our gdt_flush - we allocate no local variables. 
# Therefore, we don't need to modify esp
# Which also means we don't need to change ebp (the old frame pointer of the CALLER) to be esp
# EBP was designed to be a base pointer for the paramters/local vals for the current function
# since the stack can grow and shrink with instructions
gdt_flush:
    # Note: GAS instructions are: <mnemonic> <source>, <destination>
    # ESP points to the first FREE ADDRESS here. We aren't allocating vars. So no need to save EBP.
    # We access the only parameter we have: gdt_ptr , which is on the stack
    mov 4(%esp), %eax

    # # This is the crucial line -> load %eax value (the gdt pointer) into the GDT register
    lgdt (%eax)

    # # Now: update all the segment registers
    mov $0x10, %ax # 0x10 - offset into the GDT to get our data segment
    # # Update rest of DATA segment registers with the same value
    mov %ax, %ds 
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss
    # Finally, change the code segment. This can only be done with a far jump (why?)
    jmp $0x08,$.flush # Jump to segment selector address 0x08 (kernel code) + .flush label as offset

# Flush is a local label should not be exported to symbol table. So it has a dot.
.flush:
    ret


