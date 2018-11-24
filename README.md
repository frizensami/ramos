# Ramos

Ramos is a small kernel that was initially developed to explore operating systems concepts and implementation. Its main goal is to be a teaching platform for learning about OSs. 

## Requirements to run this OS
1. **Make**
2. **i686-elf-gcc Cross-Compiler**. Refer to [Why do I need a cross-compiler?](https://wiki.osdev.org/Why_do_I_need_a_Cross_Compiler%3F) and for how to compile a cross-compiler from source code, [GCC Cross-Compiler](https://wiki.osdev.org/GCC_Cross-Compiler). An easier method if you just want to try this toy OS out is to get a pre-compiled cross-compiler at https://github.com/lordmilko/i686-elf-tools
3. **QEMU** to run the OS itself on emulated hardware, specifically **qemu-system-i386**
4. **(Optional) Bochs**: Another emulator. 
5. **(Optional) genisoimage** if you want to create an ISO for Bochs, or to put onto an external drive.

## How to run the OS
Ramos is most easily tested with an emulator like **QEMU** or **Bochs**, although I believe you could get the ISO onto a USB drive and boot from it. I have not tried this yet, do so at your own risk.
- `make run` will compile and run Ramos on QEMU. I tend use this for testing almost all the time. 
- `make run-bochs` will do the same for Bochs. An ISO file is generated in the process. I tend to do this when I need to confirm that I am seeing the same behavior on both emulators. Remember to type `c` into the Bochs terminal to start running the OS.

## Level System
I will continue active development on `master`. 

However, to encourage learning, I am creating multiple branches off master that are different "checkpoints" that learners can pick up OS development from. For example, the `level1` branch contains only enough code to boot the OS and print text to the screen. After switching to a `level*` branch, you can find that branch's state in `level*.MD`, e.g. for `level1` the file describing the current state will be in `level1.MD`. 

Have fun!

## Implemented Features
- Simple VGA-buffer text terminal with scrolling
- Ability to read Multiboot header information
- Prints memory map from Multiboot information
- Implement interrupt system for timer (PIT) and keyboard
- Implements a basic kernel command line shell

## Planned Features
- Implement basic C library in kernelspace
- Implement basic dynamic memory allocation in kernelspace
- Add kernel commands to explore memory map, register state, etc. Implement as function pointer array
- Create BUG/BUG_ON/DIE libk calls as in Linux to dump register and memory state on crash

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