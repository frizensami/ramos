# Ramos

Ramos is a small kernel that was initially developed to explore operating systems concepts and implementation. Its main goal is to be a teaching platform for learning about OSs. 

## Implemented Features
- Simple VGA-buffer text terminal with scrolling
- Ability to read Multiboot header information
- Prints memory map from Multiboot information

## Planned Features
- Implement basic C library in kernelspace
- Implement basic dynamic memory allocation in kernelspace
- Implement interrupt system for (at least) timer and keyboard
- Implement a "kernel console" for users to explore the low-level hardware, registers, etc.

## Not-planned Features
- No current plan exists for implementing a userspace

## Structure
- `boot`: Code dedicated to the boot process and `kernel_main` function (entry point to kernel)
    - Flow of control: 
      - Bootloader calls `start.s`, which loads the multiboot information onto the stack, then `kernel.c`'s `kernel_main` function. 
      - Terminal is initialized
      - The GDT and IDT are set up
    - `linker.ld` script: Links all object files into final `.elf` file for the bootloader to run. `start.s` is first to be executed, with the multiboot header at the top of the executable.


- `io`: Code related to io devices like the keyboard, terminal and their interrupt handlers

- `lib`: Common libraries and definitions for rest of codebase