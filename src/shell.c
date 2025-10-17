/* src/shell.c - minimal interactive shell */
#include "vga.h"
#include "keyboard.h"
#include <stdint.h>
#include <stddef.h>
#include "string.h"

#define PROMPT "FossilOS> "
#define INPUT_MAX 128

/* outb helper — write byte to I/O port */
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ __volatile__("outb %0, %1" : : "a"(val), "Nd"(port));
}


static void cmd_help(void) {
    vga_puts("Available commands:\n", 0x0F);
    vga_puts("  help   - show this help\n", 0x0F);
    vga_puts("  clear  - clear the screen\n", 0x0F);
    vga_puts("  echo X - print X\n", 0x0F);
    vga_puts("  reboot - reboot the machine\n", 0x0F);
}

static void cmd_clear(void) {
    vga_clear(0x00);
}

static void cmd_echo(const char *arg) {
    if (arg) vga_puts(arg, 0x0F);
    vga_puts("\n", 0x0F);
}

static void cmd_reboot(void) {
    /* try keyboard controller reset (may work in VirtualBox) */
    __asm__ __volatile__("cli");
    outb(0x64, 0xFE);
    for (;;); /* hang if it didn't work */
}

/* small helper to write single char and advance */
static void putch(char c) { vga_putc(c, 0x0F); }

void shell_run(void) {
    char buf[INPUT_MAX];
    size_t len = 0;
    vga_puts("Welcome to FossilOS shell!\nType 'help' for commands.\n\n", 0x0F);

    for (;;) {
        vga_puts(PROMPT, 0x0F);
        len = 0;
        memset(buf, 0, sizeof(buf));
        while (1) {
            char c = keyboard_getchar();
            if (!c) continue;
            if (c == '\r' || c == '\n') {
                putch('\n');
                buf[len] = '\0';
                break;
            } else if (c == '\b' || c == 127) {
                if (len > 0) {
                    len--;
                    /* move cursor back, print space, move back again */
                    size_t x,y;
                    vga_get_cursor(&x,&y);
                    if (x == 0 && y > 0) { x = 79; y--; }
                    else if (x > 0) x--;
                    vga_set_cursor(x,y);
                    vga_putc(' ', 0x0F);
                    vga_set_cursor(x,y);
                }
            } else {
                if (len + 1 < INPUT_MAX) {
                    buf[len++] = c;
                    putch(c);
                }
            }
        }

        /* parse command (simple) */
        if (len == 0) continue;
        if (strcmp(buf, "help") == 0) cmd_help();
        else if (strcmp(buf, "clear") == 0) cmd_clear();
        else if (strncmp(buf, "echo ", 5) == 0) cmd_echo(buf + 5);
        else if (strcmp(buf, "reboot") == 0) cmd_reboot();
        else {
            vga_puts("Unknown command: ", 0x0F);
            vga_puts(buf, 0x0F);
            vga_puts("\n", 0x0F);
        }
    }
}


