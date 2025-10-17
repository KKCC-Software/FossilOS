/* FossilOS is Free and Open Source under MIT, hence the name FOSSilOS */

/* src/kernel.c - FossilOS kernel (32-bit)
 * Initializes basic subsystems (VGA + Keyboard) and launches the shell.
 */

#include "vga.h"
#include "keyboard.h"

/* Forward declaration for the shell entrypoint */
void shell_run(void);

void kmain(void) {
    /* Clear the VGA text buffer and print a boot message */
    vga_clear(0x00);
    vga_puts("Welcome to FossilOS!\n", 0x0F);
    vga_puts("Initializing subsystems...\n", 0x0F);

    /* Initialize basic I/O (keyboard polling) */
    keyboard_init();

    vga_puts("Starting shell...\n\n", 0x0F);

    /* Enter the main FossilOS shell */
    shell_run();

    /* Fallback infinite halt loop (should never reach here) */
    for (;;) {
        __asm__ __volatile__("hlt");
    }
}
